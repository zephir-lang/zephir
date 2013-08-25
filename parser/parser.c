/* Driver template for the LEMON parser generator.
** The author disclaims copyright to this source code.
*/
/* First off, code is include which follows the "include" declaration
** in the input file. */
#include <stdio.h>
// 44 "parser.lemon"


#include "json/json.h"

#include "string.h"
#include "parser.h"
#include "scanner.h"
#include "xx.h"

static json_object *xx_ret_literal(int type, xx_parser_token *T, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	switch (type) {
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
		case XX_T_SCHAR:
			json_object_object_add(ret, "type", json_object_new_string("schar"));
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

static json_object *xx_ret_class(xx_parser_token *T, json_object *class_definition, int is_abstract, xx_parser_token *E, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("class"));
	json_object_object_add(ret, "name", json_object_new_string(T->token));
	json_object_object_add(ret, "abtract", json_object_new_int(is_abstract));

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

static json_object *xx_ret_throw_exception(xx_parser_token *T, json_object *parameters, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("throw"));
	json_object_object_add(ret, "domain", json_object_new_string(T->token));

	if (parameters) {
		json_object_object_add(ret, "parameters", parameters);
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


// 744 "parser.c"
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
#define YYNOCODE 152
#define YYACTIONTYPE unsigned short int
#define xx_TOKENTYPE xx_parser_token*
typedef union {
  xx_TOKENTYPE yy0;
  json_object* yy289;
  int yy303;
} YYMINORTYPE;
#define YYSTACKDEPTH 100
#define xx_ARG_SDECL xx_parser_status *status;
#define xx_ARG_PDECL ,xx_parser_status *status
#define xx_ARG_FETCH xx_parser_status *status = yypParser->status
#define xx_ARG_STORE yypParser->status = status
#define YYNSTATE 473
#define YYNRULE 229
#define YYERRORSYMBOL 88
#define YYERRSYMDT yy303
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
 /*     0 */   321,  127,  129,  131,  404,  390,  425,  324,  405,  435,
 /*    10 */   436,  437,  438,  426,  285,  309,  315,  312,  414,  318,
 /*    20 */    45,   47,   49,   99,   58,  180,  181,  230,   69,   73,
 /*    30 */    78,  198,  199,   85,  182,  183,  185,  184,  526,  494,
 /*    40 */   473,  387,  449,  527,  247,  258,  261,  386,  388,  276,
 /*    50 */   294,  297,  300,  303,  306,  173,  321,    8,  146,  526,
 /*    60 */   402,  307,  293,  324,  527,  382,  383,  384,  385,  528,
 /*    70 */   285,  309,  315,  312,  252,  318,   45,   47,   49,  144,
 /*    80 */    58,  251,  198,  199,   69,   73,   78,  198,  199,   85,
 /*    90 */   528,  430,  431,  432,  433,  495,  206,  387,  389,  529,
 /*   100 */   247,  258,  261,  386,  388,  276,  294,  297,  300,  303,
 /*   110 */   306,   11,  321,  364,  368,  372,   84,  440,  172,  324,
 /*   120 */   529,  382,  383,  384,  385,    9,  285,  309,  315,  312,
 /*   130 */   235,  318,   45,   47,   49,  231,   58,   14,  198,  199,
 /*   140 */    69,   73,   78,  208,  210,   85,   61,  430,  431,  432,
 /*   150 */   433,  497,   21,  387,  389,  380,  247,  258,  261,  386,
 /*   160 */   388,  276,  294,  297,  300,  303,  306,   65,  321,  346,
 /*   170 */   288,  288,  343,  424,  331,  324,   20,  382,  383,  384,
 /*   180 */   385,   22,  285,  309,  315,  312,  197,  318,   45,   47,
 /*   190 */    49,   99,   58,  308,  311,  234,   69,   73,   78,  198,
 /*   200 */   199,   85,  322,  178,   18,   19,  196,  492,  405,  387,
 /*   210 */   389,  380,  247,  258,  261,  386,  388,  276,  294,  297,
 /*   220 */   300,  303,  306,  216,  321,  310,  293,   80,  350,  313,
 /*   230 */   293,  324,  391,  382,  383,  384,  385,  178,  285,  309,
 /*   240 */   315,  312,   24,  318,   45,   47,   49,  171,   58,  190,
 /*   250 */   175,  186,   69,   73,   78,  399,  288,   85,  379,  368,
 /*   260 */   372,  316,  293,  491,  108,  387,  389,   46,  247,  258,
 /*   270 */   261,  386,  388,  276,  294,  297,  300,  303,  306,  314,
 /*   280 */   321,   54,  288,   48,   25,  177,  441,  324,  212,  382,
 /*   290 */   383,  384,  385,  442,  285,  309,  315,  312,   51,  318,
 /*   300 */    45,   47,   49,   59,   58,  317,  344,   66,   69,   73,
 /*   310 */    78,  110,  112,   85,  198,  199,  150,  205,  288,  496,
 /*   320 */   267,  387,  389,   64,  247,  258,  261,  386,  388,  276,
 /*   330 */   294,  297,  300,  303,  306,   55,  321,  216,   88,  407,
 /*   340 */    57,  320,  376,  324,  408,  409,   19,  178,   60,  405,
 /*   350 */   285,  309,  315,  312,  148,  318,   45,   47,   49,  365,
 /*   360 */    58,   87,  198,  199,   69,   73,   78,  270,  273,   85,
 /*   370 */   187,   86,  246,  138,  139,  140,  141,  142,  143,  279,
 /*   380 */   247,  258,  261,  277,   67,  276,  294,  297,  300,  303,
 /*   390 */   306,   70,  321,  174,  175,  186,   72,  319,  293,  324,
 /*   400 */    75,  435,  436,  437,  438,   79,  285,  309,  315,  312,
 /*   410 */    90,  318,   45,   47,   49,  197,   58,  189,  188,   89,
 /*   420 */    69,   73,   78,  263,  266,   85,  182,  183,  185,  184,
 /*   430 */    62,  349,  179,  387,  363,  203,  247,  258,  261,  386,
 /*   440 */   388,  276,  294,  297,  300,  303,  306,   12,  321,  453,
 /*   450 */   286,  293,  396,  197,  358,  324,   97,   91,    3,    4,
 /*   460 */     5,    6,  285,  309,  315,  312,  235,  318,   45,   47,
 /*   470 */    49,  240,   58,  283,  198,  199,   69,   73,   78,  180,
 /*   480 */   181,   85,  106,   92,   93,   94,   95,   96,  182,  183,
 /*   490 */   185,  184,  247,  258,  261,  295,  293,  276,  294,  297,
 /*   500 */   300,  303,  306,  462,  321,  466,  288,  288,  345,  105,
 /*   510 */   107,  324,  109,  382,  383,  384,  385,  147,  285,  309,
 /*   520 */   315,  312,  111,  318,   45,   47,   49,   99,   58,  287,
 /*   530 */   296,  226,   69,   73,   78,  198,  199,   85,  703,    1,
 /*   540 */     2,  472,    4,    5,    6,  387,  298,  293,  247,  258,
 /*   550 */   261,  386,  388,  276,  294,  297,  300,  303,  306,  113,
 /*   560 */   321,  152,   99,  197,  360,    7,  243,  324,   10,   91,
 /*   570 */   198,  199,  459,  324,  285,  309,  315,  312,  155,  318,
 /*   580 */    45,   47,   49,  214,   58,  288,  198,  199,   69,   73,
 /*   590 */    78,   74,  151,   85,  344,   92,   93,   94,   95,   96,
 /*   600 */   157,  227,  198,  199,  247,  258,  261,  158,  299,  276,
 /*   610 */   294,  297,  300,  303,  306,  159,  321,  176,   63,  153,
 /*   620 */   398,  357,  686,  324,  379,  368,  372,  198,  199,  213,
 /*   630 */   285,  309,  315,  312,  201,  318,   45,   47,   49,   65,
 /*   640 */    58,  346,  301,  293,   69,   73,   78,  252,  474,   85,
 /*   650 */   192,  363,  193,  248,  253,  198,  199,  288,  207,  687,
 /*   660 */   247,  258,  261,  304,  293,  276,  294,  297,  300,  303,
 /*   670 */   306,  400,  321,  688,    7,  153,  337,   10,  197,  324,
 /*   680 */   305,  459,  324,  198,  199,  282,  285,  309,  315,  312,
 /*   690 */   201,  318,   45,   47,   49,   99,   58,  250,  220,   98,
 /*   700 */    69,   73,   78,  198,  199,   85,  423,  288,  434,  209,
 /*   710 */   211,  390,  217,  224,  405,  218,  247,  258,  261,  225,
 /*   720 */   249,  276,  294,  297,  300,  303,  306,  229,  321,   99,
 /*   730 */   302,  153,  342,  239,  232,  324,  233,  198,  199,  198,
 /*   740 */   199,  219,  285,  309,  315,  312,  201,  318,   45,   47,
 /*   750 */    49,   50,   58,  238,  344,  241,   69,   73,   78,  242,
 /*   760 */   153,   85,  198,  199,  255,  257,  265,  262,  198,  199,
 /*   770 */   202,  264,  247,  258,  261,  201,  268,  276,  294,  297,
 /*   780 */   300,  303,  306,  269,  321,  271,  272,  153,  355,  274,
 /*   790 */   275,  324,  278,  280,  281,  198,  199,  195,  285,  309,
 /*   800 */   315,  312,  201,  318,   45,   47,   49,  153,   58,  284,
 /*   810 */   161,  290,   69,   73,   78,  198,  199,   85,  198,  199,
 /*   820 */   289,  291,  200,  339,  292,  333,  323,  332,  247,  258,
 /*   830 */   261,  198,  199,  276,  294,  297,  300,  303,  306,  353,
 /*   840 */   321,  334,  347,  352,  362,  359,  366,  324,  367,  198,
 /*   850 */   199,  369,  370,  371,  285,  309,  315,  312,   81,  318,
 /*   860 */    45,   47,   49,  373,   58,  374,  198,  199,   69,   73,
 /*   870 */    78,  327,  375,   85,  222,  377,  378,  541,  381,  198,
 /*   880 */   199,  392,  198,  199,  247,  258,  261,  393,  395,  276,
 /*   890 */   294,  297,  300,  303,  306,  412,  321,  401,  413,  415,
 /*   900 */   356,  417,  454,  324,  419,  418,  420,  429,  427,  428,
 /*   910 */   285,  309,  315,  312,  102,  318,   45,   47,   49,  444,
 /*   920 */    58,  443,  198,  199,   69,   73,   78,  455,  114,   85,
 /*   930 */   259,  457,  460,  461,  467,  116,  198,  199,  198,  199,
 /*   940 */   247,  258,  261,  198,  199,  276,  294,  297,  300,  303,
 /*   950 */   306,  464,  321,  468,  382,  118,  351,  470,  382,  324,
 /*   960 */   382,  382,  382,  198,  199,  382,  285,  309,  315,  312,
 /*   970 */   120,  318,   45,   47,   49,  382,   58,  382,  198,  199,
 /*   980 */    69,   73,   78,  122,  382,   85,  382,  382,  382,  382,
 /*   990 */   382,  198,  199,  382,  382,  382,  247,  258,  261,  382,
 /*  1000 */   382,  276,  294,  297,  300,  303,  306,  382,  321,  382,
 /*  1010 */   382,  382,  330,  382,  382,  324,  382,  382,  382,  382,
 /*  1020 */   382,  382,  285,  309,  315,  312,  124,  318,   45,   47,
 /*  1030 */    49,  382,   58,  382,  198,  199,   69,   73,   78,  382,
 /*  1040 */   382,   85,  382,  382,  382,  382,  382,  126,  382,  382,
 /*  1050 */   382,  382,  247,  258,  261,  198,  199,  276,  294,  297,
 /*  1060 */   300,  303,  306,  382,  321,  382,  382,  382,   53,  382,
 /*  1070 */   382,  324,  382,  382,  382,  382,  382,  382,  285,  309,
 /*  1080 */   315,  312,  128,  318,   45,   47,   49,  382,   58,  382,
 /*  1090 */   198,  199,   69,   73,   78,  382,  130,   85,  382,  382,
 /*  1100 */   382,  382,  382,  132,  198,  199,  382,  382,  247,  258,
 /*  1110 */   261,  198,  199,  276,  294,  297,  300,  303,  306,  382,
 /*  1120 */   321,  382,  382,  382,   27,  382,  382,  324,  382,  382,
 /*  1130 */   382,  382,  382,  382,  285,  309,  315,  312,  134,  318,
 /*  1140 */    45,   47,   49,  382,   58,  382,  198,  199,   69,   73,
 /*  1150 */    78,  382,  382,   85,  382,  382,  382,  382,  382,  136,
 /*  1160 */   382,  382,  382,  382,  247,  258,  261,  198,  199,  276,
 /*  1170 */   294,  297,  300,  303,  306,  382,  321,  382,  382,  382,
 /*  1180 */    77,  382,  382,  324,  382,  382,  382,  382,  382,  382,
 /*  1190 */   285,  309,  315,  312,  382,  318,   45,   47,   49,  382,
 /*  1200 */    58,  376,  382,  382,   69,   73,   78,  382,  382,   85,
 /*  1210 */   382,  382,  382,  382,  382,  382,  394,  382,  365,  382,
 /*  1220 */   247,  258,  261,  382,  321,  276,  294,  297,  300,  303,
 /*  1230 */   306,  324,  138,  139,  140,  141,  142,  143,  285,  309,
 /*  1240 */   315,  312,  382,  318,   45,   47,   49,  382,   58,  376,
 /*  1250 */   382,  382,   69,   73,   78,  382,  382,   85,  382,  382,
 /*  1260 */   382,  382,  382,  382,   23,  382,  365,  382,  247,  258,
 /*  1270 */   261,  382,  382,  276,  294,  297,  300,  303,  306,  382,
 /*  1280 */   138,  139,  140,  141,  142,  143,  382,  382,  104,  156,
 /*  1290 */   160,  154,  191,  101,  382,  382,  149,  382,  382,  382,
 /*  1300 */    44,  382,  382,  383,  384,  385,  382,  137,  382,  382,
 /*  1310 */   138,  139,  140,  141,  142,  143,  382,  382,   28,   29,
 /*  1320 */    30,   31,   32,   33,   34,   35,   36,   37,   38,   39,
 /*  1330 */    40,   41,   42,   43,  387,  382,  169,  411,  382,  382,
 /*  1340 */   386,  388,  382,  162,  163,  254,  256,  382,   44,  382,
 /*  1350 */   382,  164,  165,  166,  167,  168,  382,  435,  436,  437,
 /*  1360 */   438,  403,  382,  382,  382,  382,  325,   29,   30,   31,
 /*  1370 */    32,   33,   34,   35,   36,   37,   38,   39,   40,   41,
 /*  1380 */    42,   43,  382,  382,   44,  382,  382,  465,  382,  387,
 /*  1390 */   422,  382,  416,  254,  256,  386,  388,  329,  382,  382,
 /*  1400 */   382,  382,  325,   29,   30,   31,   32,   33,   34,   35,
 /*  1410 */    36,   37,   38,   39,   40,   41,   42,   43,  463,   16,
 /*  1420 */   446,  452,  451,  439,  382,  434,  445,  409,   19,  254,
 /*  1430 */   256,  405,   44,  382,  100,  115,  119,  121,  123,  125,
 /*  1440 */   117,  103,  127,  129,  131,  397,  382,  382,  382,  382,
 /*  1450 */   325,   29,   30,   31,   32,   33,   34,   35,   36,   37,
 /*  1460 */    38,   39,   40,   41,   42,   43,   44,  382,  382,  382,
 /*  1470 */   382,  382,  382,  382,  382,   91,  382,  254,  256,  361,
 /*  1480 */   382,  382,  382,  382,  325,   29,   30,   31,   32,   33,
 /*  1490 */    34,   35,   36,   37,   38,   39,   40,   41,   42,   43,
 /*  1500 */    44,   92,   93,   94,   95,   96,  223,  236,  382,  244,
 /*  1510 */   245,  254,  256,   76,  382,  382,  382,  382,  325,   29,
 /*  1520 */    30,   31,   32,   33,   34,   35,   36,   37,   38,   39,
 /*  1530 */    40,   41,   42,   43,  382,  382,  382,  382,  382,  382,
 /*  1540 */   382,   44,  382,  382,  382,  254,  256,  382,  382,  382,
 /*  1550 */   382,  382,  382,  382,   71,  382,  382,  382,  382,  325,
 /*  1560 */    29,   30,   31,   32,   33,   34,   35,   36,   37,   38,
 /*  1570 */    39,   40,   41,   42,   43,   44,  382,  382,  382,  382,
 /*  1580 */   382,  382,  382,  382,  382,  382,  254,  256,  336,  382,
 /*  1590 */   382,  382,  382,  325,   29,   30,   31,   32,   33,   34,
 /*  1600 */    35,   36,   37,   38,   39,   40,   41,   42,   43,   44,
 /*  1610 */   382,  382,  382,  382,  382,  382,  382,  382,  382,  382,
 /*  1620 */   254,  256,   68,  382,  382,  382,  382,  325,   29,   30,
 /*  1630 */    31,   32,   33,   34,   35,   36,   37,   38,   39,   40,
 /*  1640 */    41,   42,   43,  382,  382,  382,  382,  382,  382,  382,
 /*  1650 */    44,  382,  382,  382,  254,  256,  382,  382,  382,  382,
 /*  1660 */   382,  382,  382,   83,  382,  382,  382,  382,  325,   29,
 /*  1670 */    30,   31,   32,   33,   34,   35,   36,   37,   38,   39,
 /*  1680 */    40,   41,   42,   43,   44,  382,  382,  382,  382,  382,
 /*  1690 */   382,  382,  382,  382,  382,  254,  256,  341,  382,  382,
 /*  1700 */   382,  382,  325,   29,   30,   31,   32,   33,   34,   35,
 /*  1710 */    36,   37,   38,   39,   40,   41,   42,   43,   44,  382,
 /*  1720 */   382,  382,  382,  382,  382,  382,  382,  382,  382,  254,
 /*  1730 */   256,   56,  382,  382,  382,  382,  325,   29,   30,   31,
 /*  1740 */    32,   33,   34,   35,   36,   37,   38,   39,   40,   41,
 /*  1750 */    42,   43,  382,  382,  382,  382,  382,  382,  382,   44,
 /*  1760 */   382,  382,  382,  254,  256,  382,  382,  382,  382,  382,
 /*  1770 */   382,  382,   26,  382,  382,  382,  382,  325,   29,   30,
 /*  1780 */    31,   32,   33,   34,   35,   36,   37,   38,   39,   40,
 /*  1790 */    41,   42,   43,   44,  382,  382,  382,  382,  382,  382,
 /*  1800 */   382,  382,  382,  382,  254,  256,  348,  382,  382,  382,
 /*  1810 */   382,  325,   29,   30,   31,   32,   33,   34,   35,   36,
 /*  1820 */    37,   38,   39,   40,   41,   42,   43,   44,  382,  382,
 /*  1830 */   382,  382,  382,  382,  382,  382,  382,  382,  254,  256,
 /*  1840 */    52,  382,  382,  382,  382,  325,   29,   30,   31,   32,
 /*  1850 */    33,   34,   35,   36,   37,   38,   39,   40,   41,   42,
 /*  1860 */    43,  382,  382,  382,  382,  382,  382,  382,   44,  382,
 /*  1870 */   382,  382,  254,  256,  382,  382,  382,  382,  382,  382,
 /*  1880 */   382,  354,  382,  382,  382,  382,  325,   29,   30,   31,
 /*  1890 */    32,   33,   34,   35,   36,   37,   38,   39,   40,   41,
 /*  1900 */    42,   43,  435,  436,  437,  438,  382,  104,  156,  160,
 /*  1910 */   154,  191,  101,  254,  256,  149,  382,  382,  382,  382,
 /*  1920 */   382,  382,  382,  382,  382,  382,  137,  104,  156,  160,
 /*  1930 */   154,  191,  101,  221,  387,  149,  382,  411,  382,  382,
 /*  1940 */   386,  388,  382,  382,  382,  382,  137,  326,  104,  156,
 /*  1950 */   160,  154,  191,  101,  194,  169,  149,  382,  382,  382,
 /*  1960 */   382,  382,  162,  163,  382,  382,  382,  137,  382,  382,
 /*  1970 */   164,  165,  166,  167,  168,  169,  382,  382,  382,  382,
 /*  1980 */   382,  382,  162,  163,  382,  382,  382,  382,  382,  382,
 /*  1990 */   164,  165,  166,  167,  168,  382,  169,  382,  382,  382,
 /*  2000 */   382,  382,  382,  162,  163,  382,  382,  382,  382,  382,
 /*  2010 */   382,  164,  165,  166,  167,  168,  104,  156,  160,  154,
 /*  2020 */   191,  101,  215,  382,  149,  382,  382,  382,  382,  382,
 /*  2030 */   382,  382,  382,  382,  382,  137,  104,  156,  160,  154,
 /*  2040 */   191,  101,  382,  382,  149,  447,  382,  450,  451,  439,
 /*  2050 */   382,  434,  408,  409,   19,  137,  382,  405,  104,  156,
 /*  2060 */   160,  154,  191,  101,  169,  382,  149,  382,  382,  382,
 /*  2070 */   382,  162,  163,  382,  382,  382,  338,  137,  382,  164,
 /*  2080 */   165,  166,  167,  168,  169,  382,  382,  382,  382,  382,
 /*  2090 */   382,  162,  163,  382,  382,  382,  382,  382,  382,  164,
 /*  2100 */   165,  166,  167,  168,  382,  382,  169,  237,  382,  382,
 /*  2110 */   382,  382,  382,  162,  163,  382,  382,  382,  382,  382,
 /*  2120 */   382,  164,  165,  166,  167,  168,  104,  156,  160,  154,
 /*  2130 */   191,  101,  204,  382,  149,  382,  382,  382,  382,  382,
 /*  2140 */   382,  382,  382,  382,  382,  137,  104,  156,  160,  154,
 /*  2150 */   191,  101,  382,  382,  149,  382,  382,  382,  382,  382,
 /*  2160 */   382,  382,  382,  382,  382,  137,  382,  104,  156,  160,
 /*  2170 */   154,  191,  101,  382,  169,  149,  382,  382,  382,  382,
 /*  2180 */   382,  162,  163,  382,  382,  382,  137,  382,  382,  164,
 /*  2190 */   165,  166,  167,  168,  169,  228,  382,  382,  382,  382,
 /*  2200 */   382,  162,  163,  382,  382,  382,  382,  382,  382,  164,
 /*  2210 */   165,  166,  167,  168,  382,  169,  382,  382,  382,  382,
 /*  2220 */   382,  382,  162,  163,  435,  436,  437,  438,  382,  382,
 /*  2230 */   164,  165,  166,  167,  168,  382,  382,  133,  135,  100,
 /*  2240 */   115,  119,  121,  123,  125,  117,  103,  127,  129,  131,
 /*  2250 */   382,  382,  382,  382,  471,  382,  387,  422,  382,  416,
 /*  2260 */   382,   82,  386,  388,  382,  382,  382,  382,  133,  135,
 /*  2270 */   100,  115,  119,  121,  123,  125,  117,  103,  127,  129,
 /*  2280 */   131,  448,  421,  439,  382,  434,  382,  409,   19,  382,
 /*  2290 */   382,  405,  700,  382,  382,  382,  382,  382,  382,  133,
 /*  2300 */   135,  100,  115,  119,  121,  123,  125,  117,  103,  127,
 /*  2310 */   129,  131,  133,  135,  100,  115,  119,  121,  123,  125,
 /*  2320 */   117,  103,  127,  129,  131,  382,  406,   17,  421,  439,
 /*  2330 */   382,  434,  445,  409,   19,  382,  328,  405,  382,  382,
 /*  2340 */   382,  382,  382,  133,  135,  100,  115,  119,  121,  123,
 /*  2350 */   125,  117,  103,  127,  129,  131,  435,  436,  437,  438,
 /*  2360 */   382,  605,  382,  382,  382,  260,  382,  382,  133,  135,
 /*  2370 */   100,  115,  119,  121,  123,  125,  117,  103,  127,  129,
 /*  2380 */   131,  382,  435,  436,  437,  438,  458,  382,  387,  422,
 /*  2390 */   382,  416,  340,  382,  386,  388,  382,  382,  382,  133,
 /*  2400 */   135,  100,  115,  119,  121,  123,  125,  117,  103,  127,
 /*  2410 */   129,  131,   15,  382,  387,  422,  382,  416,  145,  382,
 /*  2420 */   386,  388,  382,  382,  382,  133,  135,  100,  115,  119,
 /*  2430 */   121,  123,  125,  117,  103,  127,  129,  131,  382,  382,
 /*  2440 */   382,  382,  382,  382,  382,  382,  382,  382,  382,  335,
 /*  2450 */   382,  382,  382,  382,  382,  382,  133,  135,  100,  115,
 /*  2460 */   119,  121,  123,  125,  117,  103,  127,  129,  131,  456,
 /*  2470 */    16,  446,  452,  451,  439,  382,  434,  445,  409,   19,
 /*  2480 */   382,  382,  405,  382,  382,  382,  382,  382,  382,  382,
 /*  2490 */   382,  382,  382,  382,  382,  382,   13,   16,  446,  452,
 /*  2500 */   451,  439,  382,  434,  445,  409,   19,  382,  382,  405,
 /*  2510 */   382,  435,  436,  437,  438,  382,  382,  382,  382,  382,
 /*  2520 */   382,  382,  469,   16,  446,  452,  451,  439,  382,  434,
 /*  2530 */   445,  409,   19,  382,  382,  405,  382,  382,  383,  384,
 /*  2540 */   385,  489,  382,  387,  422,  382,  416,  382,  382,  386,
 /*  2550 */   388,  435,  436,  437,  438,  187,  382,  382,  382,  382,
 /*  2560 */   382,  382,  382,  382,  382,  382,  382,  493,  382,  387,
 /*  2570 */   410,  382,  416,  382,  382,  386,  388,  382,  382,  382,
 /*  2580 */   382,  490,  382,  387,  422,  382,  416,  382,  382,  386,
 /*  2590 */   388,  382,  382,  382,  382,  382,  170,  382,  382,  382,
 /*  2600 */   382,  382,  189,  188,  382,  382,  382,  382,  382,  382,
 /*  2610 */   382,  182,  183,  185,  184,
};
static YYCODETYPE yy_lookahead[] = {
 /*     0 */    27,   16,   17,   18,   31,  106,   28,   34,  109,    1,
 /*    10 */     2,    3,    4,   35,   41,   42,   43,   44,  102,   46,
 /*    20 */    47,   48,   49,  132,   51,   74,   75,  136,   55,   56,
 /*    30 */    57,  140,  141,   60,   83,   84,   85,   86,   16,   31,
 /*    40 */     0,   33,   34,   16,   71,   72,   73,   39,   40,   76,
 /*    50 */    77,   78,   79,   80,   81,    5,   27,   27,  111,   37,
 /*    60 */    31,  143,  144,   34,   37,    1,    2,    3,    4,   16,
 /*    70 */    41,   42,   43,   44,  132,   46,   47,   48,   49,  132,
 /*    80 */    51,  139,  140,  141,   55,   56,   57,  140,  141,   60,
 /*    90 */    37,    1,    2,    3,    4,   31,   27,   33,   34,   16,
 /*   100 */    71,   72,   73,   39,   40,   76,   77,   78,   79,   80,
 /*   110 */    81,   27,   27,  110,  111,  112,   31,   27,   68,   34,
 /*   120 */    37,    1,    2,    3,    4,   28,   41,   42,   43,   44,
 /*   130 */   132,   46,   47,   48,   49,  137,   51,   31,  140,  141,
 /*   140 */    55,   56,   57,   74,   75,   60,   31,    1,    2,    3,
 /*   150 */     4,   31,   27,   33,   34,   16,   71,   72,   73,   39,
 /*   160 */    40,   76,   77,   78,   79,   80,   81,   52,   27,   54,
 /*   170 */     5,    5,   31,   27,    5,   34,   37,    1,    2,    3,
 /*   180 */     4,   38,   41,   42,   43,   44,    5,   46,   47,   48,
 /*   190 */    49,  132,   51,   28,   28,  136,   55,   56,   57,  140,
 /*   200 */   141,   60,   66,  102,  105,  106,   25,   31,  109,   33,
 /*   210 */    34,   16,   71,   72,   73,   39,   40,   76,   77,   78,
 /*   220 */    79,   80,   81,   87,   27,  143,  144,   58,   31,  143,
 /*   230 */   144,   34,   37,    1,    2,    3,    4,  102,   41,   42,
 /*   240 */    43,   44,   30,   46,   47,   48,   49,  146,   51,  148,
 /*   250 */   149,  150,   55,   56,   57,  107,    5,   60,  110,  111,
 /*   260 */   112,  143,  144,   31,   27,   33,   34,   28,   71,   72,
 /*   270 */    73,   39,   40,   76,   77,   78,   79,   80,   81,   28,
 /*   280 */    27,   50,    5,   28,   31,  150,   28,   34,   38,    1,
 /*   290 */     2,    3,    4,   35,   41,   42,   43,   44,   30,   46,
 /*   300 */    47,   48,   49,  129,   51,   28,  132,  102,   55,   56,
 /*   310 */    57,   74,   75,   60,  140,  141,   66,   67,    5,   31,
 /*   320 */    27,   33,   34,  131,   71,   72,   73,   39,   40,   76,
 /*   330 */    77,   78,   79,   80,   81,   30,   27,   87,    5,   99,
 /*   340 */    31,   28,   10,   34,  104,  105,  106,  102,   30,  109,
 /*   350 */    41,   42,   43,   44,  132,   46,   47,   48,   49,   27,
 /*   360 */    51,   28,  140,  141,   55,   56,   57,   74,   75,   60,
 /*   370 */    27,  133,  134,   41,   42,   43,   44,   45,   46,   23,
 /*   380 */    71,   72,   73,   27,   53,   76,   77,   78,   79,   80,
 /*   390 */    81,   30,   27,  148,  149,  150,   31,  143,  144,   34,
 /*   400 */    30,    1,    2,    3,    4,   27,   41,   42,   43,   44,
 /*   410 */    27,   46,   47,   48,   49,    5,   51,   74,   75,  134,
 /*   420 */    55,   56,   57,   66,   67,   60,   83,   84,   85,   86,
 /*   430 */   130,  131,   27,   33,    5,   25,   71,   72,   73,   39,
 /*   440 */    40,   76,   77,   78,   79,   80,   81,   30,   27,   32,
 /*   450 */   143,  144,   31,    5,   25,   34,  135,   35,   92,   93,
 /*   460 */    94,   95,   41,   42,   43,   44,  132,   46,   47,   48,
 /*   470 */    49,  137,   51,   25,  140,  141,   55,   56,   57,   74,
 /*   480 */    75,   60,   27,   61,   62,   63,   64,   65,   83,   84,
 /*   490 */    85,   86,   71,   72,   73,  143,  144,   76,   77,   78,
 /*   500 */    79,   80,   81,   30,   27,   32,    5,    5,   31,  145,
 /*   510 */    67,   34,   68,    1,    2,    3,    4,   25,   41,   42,
 /*   520 */    43,   44,   68,   46,   47,   48,   49,  132,   51,   28,
 /*   530 */    28,  136,   55,   56,   57,  140,  141,   60,   89,   90,
 /*   540 */    91,   92,   93,   94,   95,   33,  143,  144,   71,   72,
 /*   550 */    73,   39,   40,   76,   77,   78,   79,   80,   81,   68,
 /*   560 */    27,   38,  132,    5,   31,   26,  136,   34,   29,   35,
 /*   570 */   140,  141,   33,   34,   41,   42,   43,   44,  132,   46,
 /*   580 */    47,   48,   49,   25,   51,    5,  140,  141,   55,   56,
 /*   590 */    57,  129,   27,   60,  132,   61,   62,   63,   64,   65,
 /*   600 */    27,   67,  140,  141,   71,   72,   73,    5,   28,   76,
 /*   610 */    77,   78,   79,   80,   81,  145,   27,   53,   31,  132,
 /*   620 */    31,  107,   53,   34,  110,  111,  112,  140,  141,  142,
 /*   630 */    41,   42,   43,   44,  147,   46,   47,   48,   49,   52,
 /*   640 */    51,   54,  143,  144,   55,   56,   57,  132,    0,   60,
 /*   650 */    27,    5,   38,  138,  139,  140,  141,    5,   68,   53,
 /*   660 */    71,   72,   73,  143,  144,   76,   77,   78,   79,   80,
 /*   670 */    81,   25,   27,   53,   26,  132,   31,   29,    5,   34,
 /*   680 */    28,   33,   34,  140,  141,  142,   41,   42,   43,   44,
 /*   690 */   147,   46,   47,   48,   49,  132,   51,    5,   25,  136,
 /*   700 */    55,   56,   57,  140,  141,   60,  101,    5,  103,   68,
 /*   710 */    68,  106,   27,   27,  109,   38,   71,   72,   73,  135,
 /*   720 */    28,   76,   77,   78,   79,   80,   81,  135,   27,  132,
 /*   730 */    28,  132,   31,  136,   68,   34,  135,  140,  141,  140,
 /*   740 */   141,  142,   41,   42,   43,   44,  147,   46,   47,   48,
 /*   750 */    49,  129,   51,  135,  132,   68,   55,   56,   57,  135,
 /*   760 */   132,   60,  140,  141,   28,   28,   28,   27,  140,  141,
 /*   770 */   142,   27,   71,   72,   73,  147,   68,   76,   77,   78,
 /*   780 */    79,   80,   81,   28,   27,   68,   28,  132,   31,   68,
 /*   790 */    28,   34,   28,   27,   38,  140,  141,  142,   41,   42,
 /*   800 */    43,   44,  147,   46,   47,   48,   49,  132,   51,   28,
 /*   810 */   132,   27,   55,   56,   57,  140,  141,   60,  140,  141,
 /*   820 */   144,   35,  147,  132,  102,   58,   27,   27,   71,   72,
 /*   830 */    73,  140,  141,   76,   77,   78,   79,   80,   81,   30,
 /*   840 */    27,  132,   53,   50,   31,   30,   35,   34,  102,  140,
 /*   850 */   141,   27,   35,  102,   41,   42,   43,   44,  132,   46,
 /*   860 */    47,   48,   49,   27,   51,   35,  140,  141,   55,   56,
 /*   870 */    57,  132,  102,   60,  132,   27,   11,   27,  109,  140,
 /*   880 */   141,   27,  140,  141,   71,   72,   73,   38,   30,   76,
 /*   890 */    77,   78,   79,   80,   81,   27,   27,   30,   35,   28,
 /*   900 */    31,   27,   27,   34,  102,   35,   28,  103,  102,   28,
 /*   910 */    41,   42,   43,   44,  132,   46,   47,   48,   49,   28,
 /*   920 */    51,  102,  140,  141,   55,   56,   57,   30,  132,   60,
 /*   930 */   132,   31,   29,   27,   27,  132,  140,  141,  140,  141,
 /*   940 */    71,   72,   73,  140,  141,   76,   77,   78,   79,   80,
 /*   950 */    81,   31,   27,   30,  151,  132,   31,   31,  151,   34,
 /*   960 */   151,  151,  151,  140,  141,  151,   41,   42,   43,   44,
 /*   970 */   132,   46,   47,   48,   49,  151,   51,  151,  140,  141,
 /*   980 */    55,   56,   57,  132,  151,   60,  151,  151,  151,  151,
 /*   990 */   151,  140,  141,  151,  151,  151,   71,   72,   73,  151,
 /*  1000 */   151,   76,   77,   78,   79,   80,   81,  151,   27,  151,
 /*  1010 */   151,  151,   31,  151,  151,   34,  151,  151,  151,  151,
 /*  1020 */   151,  151,   41,   42,   43,   44,  132,   46,   47,   48,
 /*  1030 */    49,  151,   51,  151,  140,  141,   55,   56,   57,  151,
 /*  1040 */   151,   60,  151,  151,  151,  151,  151,  132,  151,  151,
 /*  1050 */   151,  151,   71,   72,   73,  140,  141,   76,   77,   78,
 /*  1060 */    79,   80,   81,  151,   27,  151,  151,  151,   31,  151,
 /*  1070 */   151,   34,  151,  151,  151,  151,  151,  151,   41,   42,
 /*  1080 */    43,   44,  132,   46,   47,   48,   49,  151,   51,  151,
 /*  1090 */   140,  141,   55,   56,   57,  151,  132,   60,  151,  151,
 /*  1100 */   151,  151,  151,  132,  140,  141,  151,  151,   71,   72,
 /*  1110 */    73,  140,  141,   76,   77,   78,   79,   80,   81,  151,
 /*  1120 */    27,  151,  151,  151,   31,  151,  151,   34,  151,  151,
 /*  1130 */   151,  151,  151,  151,   41,   42,   43,   44,  132,   46,
 /*  1140 */    47,   48,   49,  151,   51,  151,  140,  141,   55,   56,
 /*  1150 */    57,  151,  151,   60,  151,  151,  151,  151,  151,  132,
 /*  1160 */   151,  151,  151,  151,   71,   72,   73,  140,  141,   76,
 /*  1170 */    77,   78,   79,   80,   81,  151,   27,  151,  151,  151,
 /*  1180 */    31,  151,  151,   34,  151,  151,  151,  151,  151,  151,
 /*  1190 */    41,   42,   43,   44,  151,   46,   47,   48,   49,  151,
 /*  1200 */    51,   10,  151,  151,   55,   56,   57,  151,  151,   60,
 /*  1210 */   151,  151,  151,  151,  151,  151,   25,  151,   27,  151,
 /*  1220 */    71,   72,   73,  151,   27,   76,   77,   78,   79,   80,
 /*  1230 */    81,   34,   41,   42,   43,   44,   45,   46,   41,   42,
 /*  1240 */    43,   44,  151,   46,   47,   48,   49,  151,   51,   10,
 /*  1250 */   151,  151,   55,   56,   57,  151,  151,   60,  151,  151,
 /*  1260 */   151,  151,  151,  151,   25,  151,   27,  151,   71,   72,
 /*  1270 */    73,  151,  151,   76,   77,   78,   79,   80,   81,  151,
 /*  1280 */    41,   42,   43,   44,   45,   46,  151,  151,   19,   20,
 /*  1290 */    21,   22,   23,   24,  151,  151,   27,  151,  151,  151,
 /*  1300 */    95,  151,    1,    2,    3,    4,  151,   38,  151,  151,
 /*  1310 */    41,   42,   43,   44,   45,   46,  151,  151,  113,  114,
 /*  1320 */   115,  116,  117,  118,  119,  120,  121,  122,  123,  124,
 /*  1330 */   125,  126,  127,  128,   33,  151,   67,   36,  151,  151,
 /*  1340 */    39,   40,  151,   74,   75,  140,  141,  151,   95,  151,
 /*  1350 */   151,   82,   83,   84,   85,   86,  151,    1,    2,    3,
 /*  1360 */     4,  108,  151,  151,  151,  151,  113,  114,  115,  116,
 /*  1370 */   117,  118,  119,  120,  121,  122,  123,  124,  125,  126,
 /*  1380 */   127,  128,  151,  151,   95,  151,  151,   31,  151,   33,
 /*  1390 */    34,  151,   36,  140,  141,   39,   40,  108,  151,  151,
 /*  1400 */   151,  151,  113,  114,  115,  116,  117,  118,  119,  120,
 /*  1410 */   121,  122,  123,  124,  125,  126,  127,  128,   96,   97,
 /*  1420 */    98,   99,  100,  101,  151,  103,  104,  105,  106,  140,
 /*  1430 */   141,  109,   95,  151,    8,    9,   10,   11,   12,   13,
 /*  1440 */    14,   15,   16,   17,   18,  108,  151,  151,  151,  151,
 /*  1450 */   113,  114,  115,  116,  117,  118,  119,  120,  121,  122,
 /*  1460 */   123,  124,  125,  126,  127,  128,   95,  151,  151,  151,
 /*  1470 */   151,  151,  151,  151,  151,   35,  151,  140,  141,  108,
 /*  1480 */   151,  151,  151,  151,  113,  114,  115,  116,  117,  118,
 /*  1490 */   119,  120,  121,  122,  123,  124,  125,  126,  127,  128,
 /*  1500 */    95,   61,   62,   63,   64,   65,   66,   67,  151,   69,
 /*  1510 */    70,  140,  141,  108,  151,  151,  151,  151,  113,  114,
 /*  1520 */   115,  116,  117,  118,  119,  120,  121,  122,  123,  124,
 /*  1530 */   125,  126,  127,  128,  151,  151,  151,  151,  151,  151,
 /*  1540 */   151,   95,  151,  151,  151,  140,  141,  151,  151,  151,
 /*  1550 */   151,  151,  151,  151,  108,  151,  151,  151,  151,  113,
 /*  1560 */   114,  115,  116,  117,  118,  119,  120,  121,  122,  123,
 /*  1570 */   124,  125,  126,  127,  128,   95,  151,  151,  151,  151,
 /*  1580 */   151,  151,  151,  151,  151,  151,  140,  141,  108,  151,
 /*  1590 */   151,  151,  151,  113,  114,  115,  116,  117,  118,  119,
 /*  1600 */   120,  121,  122,  123,  124,  125,  126,  127,  128,   95,
 /*  1610 */   151,  151,  151,  151,  151,  151,  151,  151,  151,  151,
 /*  1620 */   140,  141,  108,  151,  151,  151,  151,  113,  114,  115,
 /*  1630 */   116,  117,  118,  119,  120,  121,  122,  123,  124,  125,
 /*  1640 */   126,  127,  128,  151,  151,  151,  151,  151,  151,  151,
 /*  1650 */    95,  151,  151,  151,  140,  141,  151,  151,  151,  151,
 /*  1660 */   151,  151,  151,  108,  151,  151,  151,  151,  113,  114,
 /*  1670 */   115,  116,  117,  118,  119,  120,  121,  122,  123,  124,
 /*  1680 */   125,  126,  127,  128,   95,  151,  151,  151,  151,  151,
 /*  1690 */   151,  151,  151,  151,  151,  140,  141,  108,  151,  151,
 /*  1700 */   151,  151,  113,  114,  115,  116,  117,  118,  119,  120,
 /*  1710 */   121,  122,  123,  124,  125,  126,  127,  128,   95,  151,
 /*  1720 */   151,  151,  151,  151,  151,  151,  151,  151,  151,  140,
 /*  1730 */   141,  108,  151,  151,  151,  151,  113,  114,  115,  116,
 /*  1740 */   117,  118,  119,  120,  121,  122,  123,  124,  125,  126,
 /*  1750 */   127,  128,  151,  151,  151,  151,  151,  151,  151,   95,
 /*  1760 */   151,  151,  151,  140,  141,  151,  151,  151,  151,  151,
 /*  1770 */   151,  151,  108,  151,  151,  151,  151,  113,  114,  115,
 /*  1780 */   116,  117,  118,  119,  120,  121,  122,  123,  124,  125,
 /*  1790 */   126,  127,  128,   95,  151,  151,  151,  151,  151,  151,
 /*  1800 */   151,  151,  151,  151,  140,  141,  108,  151,  151,  151,
 /*  1810 */   151,  113,  114,  115,  116,  117,  118,  119,  120,  121,
 /*  1820 */   122,  123,  124,  125,  126,  127,  128,   95,  151,  151,
 /*  1830 */   151,  151,  151,  151,  151,  151,  151,  151,  140,  141,
 /*  1840 */   108,  151,  151,  151,  151,  113,  114,  115,  116,  117,
 /*  1850 */   118,  119,  120,  121,  122,  123,  124,  125,  126,  127,
 /*  1860 */   128,  151,  151,  151,  151,  151,  151,  151,   95,  151,
 /*  1870 */   151,  151,  140,  141,  151,  151,  151,  151,  151,  151,
 /*  1880 */   151,  108,  151,  151,  151,  151,  113,  114,  115,  116,
 /*  1890 */   117,  118,  119,  120,  121,  122,  123,  124,  125,  126,
 /*  1900 */   127,  128,    1,    2,    3,    4,  151,   19,   20,   21,
 /*  1910 */    22,   23,   24,  140,  141,   27,  151,  151,  151,  151,
 /*  1920 */   151,  151,  151,  151,  151,  151,   38,   19,   20,   21,
 /*  1930 */    22,   23,   24,   25,   33,   27,  151,   36,  151,  151,
 /*  1940 */    39,   40,  151,  151,  151,  151,   38,   59,   19,   20,
 /*  1950 */    21,   22,   23,   24,   25,   67,   27,  151,  151,  151,
 /*  1960 */   151,  151,   74,   75,  151,  151,  151,   38,  151,  151,
 /*  1970 */    82,   83,   84,   85,   86,   67,  151,  151,  151,  151,
 /*  1980 */   151,  151,   74,   75,  151,  151,  151,  151,  151,  151,
 /*  1990 */    82,   83,   84,   85,   86,  151,   67,  151,  151,  151,
 /*  2000 */   151,  151,  151,   74,   75,  151,  151,  151,  151,  151,
 /*  2010 */   151,   82,   83,   84,   85,   86,   19,   20,   21,   22,
 /*  2020 */    23,   24,   25,  151,   27,  151,  151,  151,  151,  151,
 /*  2030 */   151,  151,  151,  151,  151,   38,   19,   20,   21,   22,
 /*  2040 */    23,   24,  151,  151,   27,   97,  151,   99,  100,  101,
 /*  2050 */   151,  103,  104,  105,  106,   38,  151,  109,   19,   20,
 /*  2060 */    21,   22,   23,   24,   67,  151,   27,  151,  151,  151,
 /*  2070 */   151,   74,   75,  151,  151,  151,   59,   38,  151,   82,
 /*  2080 */    83,   84,   85,   86,   67,  151,  151,  151,  151,  151,
 /*  2090 */   151,   74,   75,  151,  151,  151,  151,  151,  151,   82,
 /*  2100 */    83,   84,   85,   86,  151,  151,   67,   68,  151,  151,
 /*  2110 */   151,  151,  151,   74,   75,  151,  151,  151,  151,  151,
 /*  2120 */   151,   82,   83,   84,   85,   86,   19,   20,   21,   22,
 /*  2130 */    23,   24,   25,  151,   27,  151,  151,  151,  151,  151,
 /*  2140 */   151,  151,  151,  151,  151,   38,   19,   20,   21,   22,
 /*  2150 */    23,   24,  151,  151,   27,  151,  151,  151,  151,  151,
 /*  2160 */   151,  151,  151,  151,  151,   38,  151,   19,   20,   21,
 /*  2170 */    22,   23,   24,  151,   67,   27,  151,  151,  151,  151,
 /*  2180 */   151,   74,   75,  151,  151,  151,   38,  151,  151,   82,
 /*  2190 */    83,   84,   85,   86,   67,   68,  151,  151,  151,  151,
 /*  2200 */   151,   74,   75,  151,  151,  151,  151,  151,  151,   82,
 /*  2210 */    83,   84,   85,   86,  151,   67,  151,  151,  151,  151,
 /*  2220 */   151,  151,   74,   75,    1,    2,    3,    4,  151,  151,
 /*  2230 */    82,   83,   84,   85,   86,  151,  151,    6,    7,    8,
 /*  2240 */     9,   10,   11,   12,   13,   14,   15,   16,   17,   18,
 /*  2250 */   151,  151,  151,  151,   31,  151,   33,   34,  151,   36,
 /*  2260 */   151,   30,   39,   40,  151,  151,  151,  151,    6,    7,
 /*  2270 */     8,    9,   10,   11,   12,   13,   14,   15,   16,   17,
 /*  2280 */    18,   99,  100,  101,  151,  103,  151,  105,  106,  151,
 /*  2290 */   151,  109,   30,  151,  151,  151,  151,  151,  151,    6,
 /*  2300 */     7,    8,    9,   10,   11,   12,   13,   14,   15,   16,
 /*  2310 */    17,   18,    6,    7,    8,    9,   10,   11,   12,   13,
 /*  2320 */    14,   15,   16,   17,   18,  151,   98,   99,  100,  101,
 /*  2330 */   151,  103,  104,  105,  106,  151,   30,  109,  151,  151,
 /*  2340 */   151,  151,  151,    6,    7,    8,    9,   10,   11,   12,
 /*  2350 */    13,   14,   15,   16,   17,   18,    1,    2,    3,    4,
 /*  2360 */   151,   68,  151,  151,  151,   28,  151,  151,    6,    7,
 /*  2370 */     8,    9,   10,   11,   12,   13,   14,   15,   16,   17,
 /*  2380 */    18,  151,    1,    2,    3,    4,   31,  151,   33,   34,
 /*  2390 */   151,   36,   30,  151,   39,   40,  151,  151,  151,    6,
 /*  2400 */     7,    8,    9,   10,   11,   12,   13,   14,   15,   16,
 /*  2410 */    17,   18,   31,  151,   33,   34,  151,   36,   25,  151,
 /*  2420 */    39,   40,  151,  151,  151,    6,    7,    8,    9,   10,
 /*  2430 */    11,   12,   13,   14,   15,   16,   17,   18,  151,  151,
 /*  2440 */   151,  151,  151,  151,  151,  151,  151,  151,  151,   30,
 /*  2450 */   151,  151,  151,  151,  151,  151,    6,    7,    8,    9,
 /*  2460 */    10,   11,   12,   13,   14,   15,   16,   17,   18,   96,
 /*  2470 */    97,   98,   99,  100,  101,  151,  103,  104,  105,  106,
 /*  2480 */   151,  151,  109,  151,  151,  151,  151,  151,  151,  151,
 /*  2490 */   151,  151,  151,  151,  151,  151,   96,   97,   98,   99,
 /*  2500 */   100,  101,  151,  103,  104,  105,  106,  151,  151,  109,
 /*  2510 */   151,    1,    2,    3,    4,  151,  151,  151,  151,  151,
 /*  2520 */   151,  151,   96,   97,   98,   99,  100,  101,  151,  103,
 /*  2530 */   104,  105,  106,  151,  151,  109,  151,    1,    2,    3,
 /*  2540 */     4,   31,  151,   33,   34,  151,   36,  151,  151,   39,
 /*  2550 */    40,    1,    2,    3,    4,   27,  151,  151,  151,  151,
 /*  2560 */   151,  151,  151,  151,  151,  151,  151,   31,  151,   33,
 /*  2570 */    34,  151,   36,  151,  151,   39,   40,  151,  151,  151,
 /*  2580 */   151,   31,  151,   33,   34,  151,   36,  151,  151,   39,
 /*  2590 */    40,  151,  151,  151,  151,  151,   68,  151,  151,  151,
 /*  2600 */   151,  151,   74,   75,  151,  151,  151,  151,  151,  151,
 /*  2610 */   151,   83,   84,   85,   86,
};
#define YY_SHIFT_USE_DFLT (-50)
static short yy_shift_ofst[] = {
 /*     0 */   539,   40,  648,  -50,  -50,  -50,  -50,   30,   97,  -50,
 /*    10 */    84,  417, 2381,  106,  -50,  -50, 2510,  176,  -50,  139,
 /*    20 */   125,  143, 1239,  212,  253,  -50, 1093,  -50,  -50,  -50,
 /*    30 */   -50,  -50,  -50,  -50,  -50,  -50,  -50,  -50,  -50,  -50,
 /*    40 */   -50,  -50,  -50,  -50,  -50,  239,  -50,  255,  -50, 2148,
 /*    50 */   268,  925, 1037,  231,  305,  197,  309,  -50, 2148,  318,
 /*    60 */   115,  -50,  587,  -50,  -50,  -49,  331, 1197, 1197,  361,
 /*    70 */   477,  365,  -50, 2148,  370,  141, 1149,  -50,  378,  169,
 /*    80 */  1888, 2231, 1197,   85,  -50,  383,  333,  -50,  383,  -50,
 /*    90 */  1440,  -50,  -50,  -50,  -50,  -50,  -50, 2148,  -50, 2450,
 /*   100 */  2148, 2148,  -50, 2148,  455,  -50,  443,  237,  444,  -50,
 /*   110 */   454,  -50,  491,  -50,  -15, 2148,  -15, 2148,  -15, 2148,
 /*   120 */   -15, 2148,  -15, 2148,  -15, 2148,  -15, 2148,  -50, 2148,
 /*   130 */   -50, 2148,  -50, 2148, 1426, 2148, 1426, 1269,  -50,  -50,
 /*   140 */   -50,  -50,  -50,  -50, 2393,  -50,  492, 2148,  -50,  250,
 /*   150 */   565,  523, 2107, 2450, 2148,  -50,  573,  602,  455,  -50,
 /*   160 */  2148,  -50,  -50,  -50,  -50,  -50,  -50,  -50,  -50, 2528,
 /*   170 */   -50,   50,  -50,  343,  -50,  564,  405,  -50,  -50,  -50,
 /*   180 */   -50,  -50,  -50,  -50,  -50,  -50,  -50,  569,  606,  620,
 /*   190 */   -50,  623,  614, 1929,  -50,  181,  -50, 2148,  -50,  -50,
 /*   200 */   -50,  -50,  410,  -50,  -50,   69,  590,  -50,  641,  -50,
 /*   210 */   642,  -50, 1997,  558,  -50,  -50,  685,  677, 1908,  673,
 /*   220 */   -50,  -50,  -15,  686,  534, 2148,  -50, 2127,  422, 2148,
 /*   230 */   -50,  666,  422, 2148,  -50, 2293, 2039,  422, 2148,  -50,
 /*   240 */   687,  422, 2148,  -50,  -50,  -50,  -50, 2148,  692,  -50,
 /*   250 */  2148,  -50, 2450,  -50,  736,  -50,  737,  -50, 2148, 2337,
 /*   260 */   -50,  740,  357,  744,  738,  -50,  293,  708,  755,  -50,
 /*   270 */   717,  758,  -50,  721,  762,  -50,  356,  764,  -50,  766,
 /*   280 */   756, 2148,  448,  781,  -50,  784,  501,  -50,  784,  -50,
 /*   290 */   786,  -49,  -50,  -50,  784,  502,  -50,  784,  580,  -50,
 /*   300 */   784,  702,  -50,  784,  652,  -50,  784,  165,  -50,  784,
 /*   310 */   166,  -50,  784,  251,  -50,  784,  277,  -50,  784,  313,
 /*   320 */   -50,  136,  799,  523,  -50,  -50, 2148, 2306, 1197,  981,
 /*   330 */   -50,  800,  767, 2017, 2419, 1197,  645,  -50, 2148, 2362,
 /*   340 */  1197,  701,  -50,  -50, 2262,  -50,  789, 1197, 1197,  -50,
 /*   350 */   -50,  793,  809,  869,  757,  -50,  -50,  429,  815,  533,
 /*   360 */   -50,  813,  -50,  332,  -50,  811,  -49,  -50,  824,  817,
 /*   370 */   -49,  -50,  836,  830,  -49,  -50,  848,  865,  850,  -50,
 /*   380 */   512,  -50,  -50,  -50,  -50,  -50,  -50,  -50,  -50,  512,
 /*   390 */   195,  854,  849, 1191,  858,  421,  -50,  589,  -50,  646,
 /*   400 */   867,   29,  -50,  -27,  -50,  -50, 2536,  288,  -50,  -50,
 /*   410 */  1301,  868,  863,  -49,  871,  -50,  874,  870,  -49,  878,
 /*   420 */   -50,  -50, 1901,  146,  -22,  -50,  -49,  881,  -50,  -50,
 /*   430 */   -50,  -50,  -50,  -50,  -50,   22,   27,   53,   83,   90,
 /*   440 */   258,  -50,  -49,  891,  -50,  -50, 2550,    8,  120,  400,
 /*   450 */    64,  -50,  232,  875,  897, 2355,  900,  -50,  -50,  903,
 /*   460 */   906,  473, 1356,  920,  -50,  -50,  907,  923, 2223,  926,
 /*   470 */   -50,  -50,  -50,
};
#define YY_REDUCE_USE_DFLT (-110)
static short yy_reduce_ofst[] = {
 /*     0 */   449, -110,  366, -110, -110, -110, -110, -110, -110, -110,
 /*    10 */  -110, -110, 2400, -110, -110, -110, 2228,   99, -110, -110,
 /*    20 */  -110, -110,  514, -110, 1664, -110, 1205, -110, -110, -110,
 /*    30 */  -110, -110, -110, -110, -110, -110, -110, -110, -110, -110,
 /*    40 */  -110, -110, -110, -110, -110, -110, -110, -110, -110,  622,
 /*    50 */  -110, 1732, 1205, -110, -110, 1623, 1205, -110,  174, -110,
 /*    60 */   300, -110,  192, -110, -110,  205, -110, 1514, 1205, -110,
 /*    70 */  1446, 1205, -110,  462, -110, 1405, 1205, -110, -110, -110,
 /*    80 */   726, -110, 1555, 1205, -110,  238, -110, -110,  285, -110,
 /*    90 */   321, -110, -110, -110, -110, -110, -110,  563, -110, -110,
 /*   100 */   742,  782, -110,  796,  364, -110, -110, -110, -110, -110,
 /*   110 */  -110, -110, -110, -110, -110,  803, -110,  823, -110,  838,
 /*   120 */  -110,  851, -110,  894, -110,  915, -110,  950, -110,  964,
 /*   130 */  -110,  971, -110, 1006, -110, 1027, -110,  -53, -110, -110,
 /*   140 */  -110, -110, -110, -110, -110, -110, -110,  222, -110, -110,
 /*   150 */  -110, -110,  628, -110,  446, -110, -110, -110,  470, -110,
 /*   160 */   678, -110, -110, -110, -110, -110, -110, -110, -110,  101,
 /*   170 */  -110, -110, -110,  245, -110, -110,  135, -110, -110, -110,
 /*   180 */  -110, -110, -110, -110, -110, -110, -110, -110, -110, -110,
 /*   190 */  -110, -110, -110,  655, -110, -110, -110,  675, -110, -110,
 /*   200 */  -110, -110, -110, -110, -110, -110, -110, -110, -110, -110,
 /*   210 */  -110, -110,  487, -110, -110, -110, -110, -110,  599, -110,
 /*   220 */  -110, -110, -110, -110,  584,  395, -110,   -2,  592, -109,
 /*   230 */  -110, -110,  601,   59, -110, -110,  334,  618,  597, -110,
 /*   240 */  -110,  624,  430, -110, -110, -110, -110,  515, -110, -110,
 /*   250 */   -58, -110, -110, -110, -110, -110, -110, -110,  798, -110,
 /*   260 */  -110, -110, -110, -110, -110, -110, -110, -110, -110, -110,
 /*   270 */  -110, -110, -110, -110, -110, -110, -110, -110, -110, -110,
 /*   280 */  -110,  543, -110, -110, -110,  307, -110, -110,  676, -110,
 /*   290 */  -110,  722, -110, -110,  352, -110, -110,  403, -110, -110,
 /*   300 */   499, -110, -110,  520, -110, -110,  -82, -110, -110,   82,
 /*   310 */  -110, -110,   86, -110, -110,  118, -110, -110,  254, -110,
 /*   320 */  -110, -110, -110, -110, -110, -110,  739, -110, 1289, 1205,
 /*   330 */  -110, -110, -110,  709, -110, 1480, 1205, -110,  691, -110,
 /*   340 */  1589, 1205, -110, -110, -110, -110, -110, 1698, 1205, -110,
 /*   350 */  -110, -110, -110, 1773, 1205, -110, -110, -110, -110, 1371,
 /*   360 */  -110, 1205, -110,    3, -110, -110,  746, -110, -110, -110,
 /*   370 */   751, -110, -110, -110,  770, -110, -110, -110, -110, -110,
 /*   380 */   769, -110, -110, -110, -110, -110, -110, -110, -110, -101,
 /*   390 */  -110, -110, -110,  148, -110, 1337, -110, 1205, -110, -110,
 /*   400 */  -110, 1253, -110, 1205, -110, -110,  240,   99, -110, -110,
 /*   410 */  -101, -110, -110,  -84, -110, -110, -110, -110,  802, -110,
 /*   420 */  -110, -110,  605,  804, -110, -110,  806, -110, -110, -110,
 /*   430 */  -110, -110, -110, -110, -110, -110, -110, -110, -110,  804,
 /*   440 */  -110, -110,  819, -110, -110, -110, 1948, 2182,   99,  605,
 /*   450 */    99, -110,   99, -110, -110, 2373, -110, -110, -110, -110,
 /*   460 */  -110, -110, 1322, -110, -110, -110, -110, -110, 2426, -110,
 /*   470 */  -110, -110, -110,
};
static YYACTIONTYPE yy_default[] = {
 /*     0 */   702,  702,  702,  475,  477,  478,  479,  702,  702,  480,
 /*    10 */   702,  702,  702,  702,  481,  485,  702,  702,  512,  702,
 /*    20 */   702,  702,  702,  702,  702,  516,  702,  518,  548,  550,
 /*    30 */   551,  552,  553,  554,  555,  556,  557,  558,  559,  560,
 /*    40 */   561,  562,  563,  564,  565,  702,  566,  702,  567,  702,
 /*    50 */   702,  702,  702,  570,  702,  702,  702,  571,  702,  702,
 /*    60 */   702,  574,  702,  575,  576,  702,  702,  702,  578,  702,
 /*    70 */   702,  702,  581,  702,  702,  702,  702,  583,  702,  702,
 /*    80 */   702,  702,  702,  702,  584,  702,  702,  588,  702,  589,
 /*    90 */   702,  591,  592,  593,  594,  595,  596,  702,  597,  633,
 /*   100 */   702,  702,  634,  702,  702,  635,  702,  702,  702,  697,
 /*   110 */   702,  698,  702,  699,  637,  702,  638,  702,  639,  702,
 /*   120 */   640,  702,  641,  702,  642,  702,  643,  702,  650,  702,
 /*   130 */   651,  702,  652,  702,  653,  702,  654,  702,  542,  543,
 /*   140 */   544,  545,  546,  547,  702,  644,  702,  702,  645,  658,
 /*   150 */   702,  646,  702,  681,  702,  655,  702,  702,  702,  656,
 /*   160 */   702,  657,  659,  660,  661,  662,  663,  664,  665,  702,
 /*   170 */   666,  702,  667,  702,  682,  702,  702,  684,  689,  690,
 /*   180 */   691,  692,  693,  694,  695,  696,  685,  690,  692,  691,
 /*   190 */   683,  702,  668,  702,  669,  702,  670,  702,  673,  674,
 /*   200 */   679,  680,  702,  677,  678,  702,  702,  647,  702,  648,
 /*   210 */   702,  649,  702,  702,  671,  672,  702,  702,  702,  702,
 /*   220 */   675,  676,  636,  702,  702,  702,  598,  702,  702,  702,
 /*   230 */   599,  702,  702,  702,  600,  702,  702,  702,  702,  601,
 /*   240 */   702,  702,  702,  602,  603,  604,  590,  702,  702,  606,
 /*   250 */   702,  607,  609,  608,  702,  610,  702,  611,  702,  702,
 /*   260 */   612,  702,  702,  702,  702,  613,  702,  702,  702,  614,
 /*   270 */   702,  702,  615,  702,  702,  616,  702,  702,  617,  702,
 /*   280 */   702,  702,  702,  702,  618,  702,  702,  619,  702,  629,
 /*   290 */   631,  702,  632,  630,  702,  702,  620,  702,  702,  621,
 /*   300 */   702,  702,  622,  702,  702,  623,  702,  702,  624,  702,
 /*   310 */   702,  625,  702,  702,  626,  702,  702,  627,  702,  702,
 /*   320 */   628,  702,  702,  702,  701,  549,  702,  702,  702,  702,
 /*   330 */   585,  702,  702,  702,  702,  702,  702,  586,  702,  702,
 /*   340 */   702,  702,  587,  582,  702,  580,  702,  702,  579,  577,
 /*   350 */   572,  568,  702,  702,  702,  573,  569,  702,  702,  702,
 /*   360 */   517,  702,  519,  702,  533,  535,  702,  538,  702,  536,
 /*   370 */   702,  539,  702,  537,  702,  540,  702,  702,  702,  534,
 /*   380 */   702,  524,  526,  527,  528,  529,  530,  531,  532,  702,
 /*   390 */   702,  702,  702,  702,  702,  702,  520,  702,  522,  702,
 /*   400 */   702,  702,  521,  702,  523,  525,  702,  702,  510,  513,
 /*   410 */   702,  702,  702,  702,  702,  514,  702,  702,  702,  702,
 /*   420 */   515,  498,  702,  702,  702,  500,  702,  702,  502,  504,
 /*   430 */   506,  507,  508,  509,  505,  506,  507,  508,  509,  702,
 /*   440 */   702,  501,  702,  702,  503,  511,  702,  702,  702,  702,
 /*   450 */   702,  499,  702,  702,  702,  702,  702,  482,  486,  702,
 /*   460 */   702,  702,  702,  702,  483,  487,  702,  702,  702,  702,
 /*   470 */   484,  488,  476,
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
  "PRIVATE",       "COMMA",         "AND",           "OR",          
  "EQUALS",        "IDENTICAL",     "LESS",          "GREATER",     
  "LESSEQUAL",     "GREATEREQUAL",  "NOTIDENTICAL",  "NOTEQUALS",   
  "ADD",           "SUB",           "CONCAT",        "ISSET",       
  "FETCH",         "TYPEOF",        "INSTANCEOF",    "NEW",         
  "NOT",           "PARENTHESES_CLOSE",  "NAMESPACE",     "IDENTIFIER",  
  "DOTCOMMA",      "CLASS",         "BRACKET_OPEN",  "BRACKET_CLOSE",
  "EXTENDS",       "ABSTRACT",      "COMMENT",       "ASSIGN",      
  "CONST",         "FUNCTION",      "PARENTHESES_OPEN",  "INLINE",      
  "FINAL",         "TYPE_INTEGER",  "TYPE_DOUBLE",   "TYPE_BOOL",   
  "TYPE_STRING",   "TYPE_ARRAY",    "TYPE_VAR",      "BREAK",       
  "CONTINUE",      "IF",            "ELSE",          "SWITCH",      
  "CASE",          "COLON",         "DEFAULT",       "LOOP",        
  "WHILE",         "FOR",           "IN",            "REVERSE",     
  "LET",           "ADDASSIGN",     "SUBASSIGN",     "MULASSIGN",   
  "DIVASSIGN",     "CONCATASSIGN",  "ARROW",         "SBRACKET_OPEN",
  "SBRACKET_CLOSE",  "INCR",          "DECR",          "ECHO",        
  "RETURN",        "UNSET",         "INTEGER",       "STRING",      
  "THROW",         "TYPE_UINTEGER",  "TYPE_CHAR",     "TYPE_UCHAR",  
  "TYPE_LONG",     "TYPE_ULONG",    "SCHAR",         "DOUBLE",      
  "NULL",          "TRUE",          "FALSE",         "DOUBLECOLON", 
  "error",         "program",       "xx_language",   "xx_top_statement_list",
  "xx_top_statement",  "xx_namespace_def",  "xx_class_def",  "xx_comment",  
  "xx_class_definition",  "xx_class_properties_definition",  "xx_class_consts_definition",  "xx_class_methods_definition",
  "xx_class_property_definition",  "xx_property_visibility_list",  "xx_literal_expr",  "xx_property_visibility",
  "xx_class_const_definition",  "xx_class_method_definition",  "xx_method_visibility_list",  "xx_parameter_list",
  "xx_statement_list",  "xx_method_visibility",  "xx_parameter",  "xx_parameter_type",
  "xx_parameter_cast",  "xx_statement",  "xx_let_statement",  "xx_if_statement",
  "xx_loop_statement",  "xx_echo_statement",  "xx_return_statement",  "xx_mcall_statement",
  "xx_scall_statement",  "xx_unset_statement",  "xx_throw_statement",  "xx_declare_statement",
  "xx_break_statement",  "xx_continue_statement",  "xx_while_statement",  "xx_switch_statement",
  "xx_for_statement",  "xx_eval_expr",  "xx_case_clauses",  "xx_case_clause",
  "xx_common_expr",  "xx_let_assignments",  "xx_let_assignment",  "xx_assignment_operator",
  "xx_assign_expr",  "xx_index_expr",  "xx_echo_expressions",  "xx_echo_expression",
  "xx_mcall_expr",  "xx_scall_expr",  "xx_call_parameters",  "xx_declare_variable_list",
  "xx_declare_variable",  "xx_isset_expr",  "xx_array_list",  "xx_call_parameter",
  "xx_array_item",  "xx_array_key",  "xx_array_value",
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
 /*   6 */ "xx_top_statement ::= xx_comment",
 /*   7 */ "xx_namespace_def ::= NAMESPACE IDENTIFIER DOTCOMMA",
 /*   8 */ "xx_class_def ::= CLASS IDENTIFIER BRACKET_OPEN xx_class_definition BRACKET_CLOSE",
 /*   9 */ "xx_class_def ::= CLASS IDENTIFIER EXTENDS IDENTIFIER BRACKET_OPEN xx_class_definition BRACKET_CLOSE",
 /*  10 */ "xx_class_def ::= ABSTRACT CLASS IDENTIFIER BRACKET_OPEN xx_class_definition BRACKET_CLOSE",
 /*  11 */ "xx_class_def ::= ABSTRACT CLASS IDENTIFIER EXTENDS IDENTIFIER BRACKET_OPEN xx_class_definition BRACKET_CLOSE",
 /*  12 */ "xx_class_def ::= CLASS IDENTIFIER BRACKET_OPEN BRACKET_CLOSE",
 /*  13 */ "xx_class_def ::= CLASS IDENTIFIER EXTENDS IDENTIFIER BRACKET_OPEN BRACKET_CLOSE",
 /*  14 */ "xx_class_def ::= ABSTRACT CLASS IDENTIFIER BRACKET_OPEN BRACKET_CLOSE",
 /*  15 */ "xx_class_def ::= ABSTRACT CLASS IDENTIFIER EXTENDS IDENTIFIER BRACKET_OPEN BRACKET_CLOSE",
 /*  16 */ "xx_class_definition ::= xx_class_properties_definition",
 /*  17 */ "xx_class_definition ::= xx_class_consts_definition",
 /*  18 */ "xx_class_definition ::= xx_class_methods_definition",
 /*  19 */ "xx_class_definition ::= xx_class_properties_definition xx_class_methods_definition",
 /*  20 */ "xx_class_definition ::= xx_class_properties_definition xx_class_consts_definition",
 /*  21 */ "xx_class_definition ::= xx_class_consts_definition xx_class_properties_definition",
 /*  22 */ "xx_class_definition ::= xx_class_consts_definition xx_class_methods_definition",
 /*  23 */ "xx_class_definition ::= xx_class_properties_definition xx_class_consts_definition xx_class_methods_definition",
 /*  24 */ "xx_class_definition ::= xx_class_consts_definition xx_class_properties_definition xx_class_methods_definition",
 /*  25 */ "xx_class_properties_definition ::= xx_class_properties_definition xx_class_property_definition",
 /*  26 */ "xx_class_properties_definition ::= xx_class_property_definition",
 /*  27 */ "xx_class_property_definition ::= COMMENT xx_property_visibility_list IDENTIFIER DOTCOMMA",
 /*  28 */ "xx_class_property_definition ::= xx_property_visibility_list IDENTIFIER DOTCOMMA",
 /*  29 */ "xx_class_property_definition ::= COMMENT xx_property_visibility_list IDENTIFIER ASSIGN xx_literal_expr DOTCOMMA",
 /*  30 */ "xx_class_property_definition ::= xx_property_visibility_list IDENTIFIER ASSIGN xx_literal_expr DOTCOMMA",
 /*  31 */ "xx_property_visibility_list ::= xx_property_visibility_list xx_property_visibility",
 /*  32 */ "xx_property_visibility_list ::= xx_property_visibility",
 /*  33 */ "xx_property_visibility ::= PUBLIC",
 /*  34 */ "xx_property_visibility ::= PROTECTED",
 /*  35 */ "xx_property_visibility ::= STATIC",
 /*  36 */ "xx_property_visibility ::= PRIVATE",
 /*  37 */ "xx_class_consts_definition ::= xx_class_consts_definition xx_class_const_definition",
 /*  38 */ "xx_class_consts_definition ::= xx_class_const_definition",
 /*  39 */ "xx_class_methods_definition ::= xx_class_methods_definition xx_class_method_definition",
 /*  40 */ "xx_class_methods_definition ::= xx_class_method_definition",
 /*  41 */ "xx_class_const_definition ::= COMMENT CONST IDENTIFIER ASSIGN xx_literal_expr DOTCOMMA",
 /*  42 */ "xx_class_const_definition ::= CONST IDENTIFIER ASSIGN xx_literal_expr DOTCOMMA",
 /*  43 */ "xx_class_method_definition ::= xx_method_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  44 */ "xx_class_method_definition ::= xx_method_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  45 */ "xx_class_method_definition ::= xx_method_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  46 */ "xx_class_method_definition ::= xx_method_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  47 */ "xx_class_method_definition ::= COMMENT xx_method_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  48 */ "xx_class_method_definition ::= COMMENT xx_method_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  49 */ "xx_class_method_definition ::= COMMENT xx_method_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  50 */ "xx_class_method_definition ::= COMMENT xx_method_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  51 */ "xx_method_visibility_list ::= xx_method_visibility_list ADD xx_method_visibility",
 /*  52 */ "xx_method_visibility_list ::= xx_method_visibility",
 /*  53 */ "xx_method_visibility ::= PUBLIC",
 /*  54 */ "xx_method_visibility ::= PROTECTED",
 /*  55 */ "xx_method_visibility ::= STATIC",
 /*  56 */ "xx_method_visibility ::= PRIVATE",
 /*  57 */ "xx_method_visibility ::= INLINE",
 /*  58 */ "xx_method_visibility ::= ABSTRACT",
 /*  59 */ "xx_method_visibility ::= FINAL",
 /*  60 */ "xx_parameter_list ::= xx_parameter_list COMMA xx_parameter",
 /*  61 */ "xx_parameter_list ::= xx_parameter",
 /*  62 */ "xx_parameter ::= IDENTIFIER",
 /*  63 */ "xx_parameter ::= xx_parameter_type IDENTIFIER",
 /*  64 */ "xx_parameter ::= xx_parameter_cast IDENTIFIER",
 /*  65 */ "xx_parameter ::= IDENTIFIER ASSIGN xx_literal_expr",
 /*  66 */ "xx_parameter ::= xx_parameter_type IDENTIFIER ASSIGN xx_literal_expr",
 /*  67 */ "xx_parameter ::= xx_parameter_cast IDENTIFIER ASSIGN xx_literal_expr",
 /*  68 */ "xx_parameter_cast ::= LESS IDENTIFIER GREATER",
 /*  69 */ "xx_parameter_type ::= TYPE_INTEGER",
 /*  70 */ "xx_parameter_type ::= TYPE_DOUBLE",
 /*  71 */ "xx_parameter_type ::= TYPE_BOOL",
 /*  72 */ "xx_parameter_type ::= TYPE_STRING",
 /*  73 */ "xx_parameter_type ::= TYPE_ARRAY",
 /*  74 */ "xx_parameter_type ::= TYPE_VAR",
 /*  75 */ "xx_statement_list ::= xx_statement_list xx_statement",
 /*  76 */ "xx_statement_list ::= xx_statement",
 /*  77 */ "xx_statement ::= xx_let_statement",
 /*  78 */ "xx_statement ::= xx_if_statement",
 /*  79 */ "xx_statement ::= xx_loop_statement",
 /*  80 */ "xx_statement ::= xx_echo_statement",
 /*  81 */ "xx_statement ::= xx_return_statement",
 /*  82 */ "xx_statement ::= xx_mcall_statement",
 /*  83 */ "xx_statement ::= xx_scall_statement",
 /*  84 */ "xx_statement ::= xx_unset_statement",
 /*  85 */ "xx_statement ::= xx_throw_statement",
 /*  86 */ "xx_statement ::= xx_declare_statement",
 /*  87 */ "xx_statement ::= xx_break_statement",
 /*  88 */ "xx_statement ::= xx_continue_statement",
 /*  89 */ "xx_statement ::= xx_while_statement",
 /*  90 */ "xx_statement ::= xx_switch_statement",
 /*  91 */ "xx_statement ::= xx_for_statement",
 /*  92 */ "xx_statement ::= xx_comment",
 /*  93 */ "xx_break_statement ::= BREAK DOTCOMMA",
 /*  94 */ "xx_continue_statement ::= CONTINUE DOTCOMMA",
 /*  95 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /*  96 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE ELSE BRACKET_OPEN BRACKET_CLOSE",
 /*  97 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  98 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE ELSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  99 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE ELSE BRACKET_OPEN BRACKET_CLOSE",
 /* 100 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE ELSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 101 */ "xx_switch_statement ::= SWITCH xx_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /* 102 */ "xx_switch_statement ::= SWITCH xx_eval_expr BRACKET_OPEN xx_case_clauses BRACKET_CLOSE",
 /* 103 */ "xx_case_clauses ::= xx_case_clauses xx_case_clause",
 /* 104 */ "xx_case_clauses ::= xx_case_clause",
 /* 105 */ "xx_case_clause ::= CASE xx_literal_expr COLON xx_statement_list",
 /* 106 */ "xx_case_clause ::= DEFAULT COLON xx_statement_list",
 /* 107 */ "xx_loop_statement ::= LOOP BRACKET_OPEN BRACKET_CLOSE",
 /* 108 */ "xx_loop_statement ::= LOOP BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 109 */ "xx_while_statement ::= WHILE xx_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /* 110 */ "xx_while_statement ::= WHILE xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 111 */ "xx_for_statement ::= FOR IDENTIFIER IN xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 112 */ "xx_for_statement ::= FOR IDENTIFIER IN REVERSE xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 113 */ "xx_for_statement ::= FOR IDENTIFIER COMMA IDENTIFIER IN xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 114 */ "xx_for_statement ::= FOR IDENTIFIER COMMA IDENTIFIER IN REVERSE xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 115 */ "xx_let_statement ::= LET xx_let_assignments DOTCOMMA",
 /* 116 */ "xx_let_assignments ::= xx_let_assignments COMMA xx_let_assignment",
 /* 117 */ "xx_let_assignments ::= xx_let_assignment",
 /* 118 */ "xx_assignment_operator ::= ASSIGN",
 /* 119 */ "xx_assignment_operator ::= ADDASSIGN",
 /* 120 */ "xx_assignment_operator ::= SUBASSIGN",
 /* 121 */ "xx_assignment_operator ::= MULASSIGN",
 /* 122 */ "xx_assignment_operator ::= DIVASSIGN",
 /* 123 */ "xx_assignment_operator ::= CONCATASSIGN",
 /* 124 */ "xx_let_assignment ::= IDENTIFIER xx_assignment_operator xx_assign_expr",
 /* 125 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER xx_assignment_operator xx_assign_expr",
 /* 126 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 127 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER SBRACKET_OPEN xx_index_expr SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 128 */ "xx_let_assignment ::= IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 129 */ "xx_let_assignment ::= IDENTIFIER SBRACKET_OPEN xx_index_expr SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 130 */ "xx_let_assignment ::= IDENTIFIER INCR",
 /* 131 */ "xx_let_assignment ::= IDENTIFIER DECR",
 /* 132 */ "xx_index_expr ::= xx_common_expr",
 /* 133 */ "xx_echo_statement ::= ECHO xx_echo_expressions DOTCOMMA",
 /* 134 */ "xx_echo_expressions ::= xx_echo_expressions COMMA xx_echo_expression",
 /* 135 */ "xx_echo_expressions ::= xx_echo_expression",
 /* 136 */ "xx_echo_expression ::= xx_common_expr",
 /* 137 */ "xx_mcall_statement ::= xx_mcall_expr DOTCOMMA",
 /* 138 */ "xx_scall_statement ::= xx_scall_expr DOTCOMMA",
 /* 139 */ "xx_return_statement ::= RETURN xx_common_expr DOTCOMMA",
 /* 140 */ "xx_unset_statement ::= UNSET IDENTIFIER ARROW IDENTIFIER DOTCOMMA",
 /* 141 */ "xx_unset_statement ::= UNSET IDENTIFIER SBRACKET_OPEN IDENTIFIER SBRACKET_CLOSE DOTCOMMA",
 /* 142 */ "xx_unset_statement ::= UNSET IDENTIFIER SBRACKET_OPEN INTEGER SBRACKET_CLOSE DOTCOMMA",
 /* 143 */ "xx_unset_statement ::= UNSET IDENTIFIER SBRACKET_OPEN STRING SBRACKET_CLOSE DOTCOMMA",
 /* 144 */ "xx_throw_statement ::= THROW IDENTIFIER DOTCOMMA",
 /* 145 */ "xx_throw_statement ::= THROW NEW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE DOTCOMMA",
 /* 146 */ "xx_declare_statement ::= TYPE_INTEGER xx_declare_variable_list DOTCOMMA",
 /* 147 */ "xx_declare_statement ::= TYPE_UINTEGER xx_declare_variable_list DOTCOMMA",
 /* 148 */ "xx_declare_statement ::= TYPE_CHAR xx_declare_variable_list DOTCOMMA",
 /* 149 */ "xx_declare_statement ::= TYPE_UCHAR xx_declare_variable_list DOTCOMMA",
 /* 150 */ "xx_declare_statement ::= TYPE_LONG xx_declare_variable_list DOTCOMMA",
 /* 151 */ "xx_declare_statement ::= TYPE_ULONG xx_declare_variable_list DOTCOMMA",
 /* 152 */ "xx_declare_statement ::= TYPE_DOUBLE xx_declare_variable_list DOTCOMMA",
 /* 153 */ "xx_declare_statement ::= TYPE_STRING xx_declare_variable_list DOTCOMMA",
 /* 154 */ "xx_declare_statement ::= TYPE_BOOL xx_declare_variable_list DOTCOMMA",
 /* 155 */ "xx_declare_statement ::= TYPE_VAR xx_declare_variable_list DOTCOMMA",
 /* 156 */ "xx_declare_variable_list ::= xx_declare_variable_list COMMA xx_declare_variable",
 /* 157 */ "xx_declare_variable_list ::= xx_declare_variable",
 /* 158 */ "xx_declare_variable ::= IDENTIFIER",
 /* 159 */ "xx_declare_variable ::= IDENTIFIER ASSIGN xx_literal_expr",
 /* 160 */ "xx_assign_expr ::= xx_common_expr",
 /* 161 */ "xx_common_expr ::= NOT xx_common_expr",
 /* 162 */ "xx_common_expr ::= ISSET xx_isset_expr",
 /* 163 */ "xx_common_expr ::= xx_common_expr EQUALS xx_common_expr",
 /* 164 */ "xx_common_expr ::= xx_common_expr NOTEQUALS xx_common_expr",
 /* 165 */ "xx_common_expr ::= xx_common_expr IDENTICAL xx_common_expr",
 /* 166 */ "xx_common_expr ::= xx_common_expr NOTIDENTICAL xx_common_expr",
 /* 167 */ "xx_common_expr ::= xx_common_expr LESS xx_common_expr",
 /* 168 */ "xx_common_expr ::= xx_common_expr GREATER xx_common_expr",
 /* 169 */ "xx_common_expr ::= xx_common_expr LESSEQUAL xx_common_expr",
 /* 170 */ "xx_common_expr ::= xx_common_expr GREATEREQUAL xx_common_expr",
 /* 171 */ "xx_common_expr ::= PARENTHESES_OPEN xx_common_expr PARENTHESES_CLOSE",
 /* 172 */ "xx_common_expr ::= PARENTHESES_OPEN xx_parameter_type PARENTHESES_CLOSE xx_common_expr",
 /* 173 */ "xx_common_expr ::= IDENTIFIER ARROW IDENTIFIER",
 /* 174 */ "xx_common_expr ::= IDENTIFIER SBRACKET_OPEN IDENTIFIER SBRACKET_CLOSE",
 /* 175 */ "xx_common_expr ::= IDENTIFIER SBRACKET_OPEN INTEGER SBRACKET_CLOSE",
 /* 176 */ "xx_common_expr ::= IDENTIFIER SBRACKET_OPEN STRING SBRACKET_CLOSE",
 /* 177 */ "xx_common_expr ::= xx_common_expr ADD xx_common_expr",
 /* 178 */ "xx_common_expr ::= xx_common_expr SUB xx_common_expr",
 /* 179 */ "xx_common_expr ::= xx_common_expr CONCAT xx_common_expr",
 /* 180 */ "xx_common_expr ::= xx_common_expr AND xx_common_expr",
 /* 181 */ "xx_common_expr ::= xx_common_expr OR xx_common_expr",
 /* 182 */ "xx_common_expr ::= INSTANCEOF xx_common_expr",
 /* 183 */ "xx_common_expr ::= FETCH IDENTIFIER COMMA xx_isset_expr",
 /* 184 */ "xx_common_expr ::= TYPEOF xx_common_expr",
 /* 185 */ "xx_common_expr ::= IDENTIFIER",
 /* 186 */ "xx_common_expr ::= INTEGER",
 /* 187 */ "xx_common_expr ::= STRING",
 /* 188 */ "xx_common_expr ::= SCHAR",
 /* 189 */ "xx_common_expr ::= DOUBLE",
 /* 190 */ "xx_common_expr ::= NULL",
 /* 191 */ "xx_common_expr ::= TRUE",
 /* 192 */ "xx_common_expr ::= FALSE",
 /* 193 */ "xx_common_expr ::= SBRACKET_OPEN SBRACKET_CLOSE",
 /* 194 */ "xx_common_expr ::= SBRACKET_OPEN xx_array_list SBRACKET_CLOSE",
 /* 195 */ "xx_common_expr ::= NEW IDENTIFIER",
 /* 196 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 197 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 198 */ "xx_common_expr ::= IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 199 */ "xx_common_expr ::= IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 200 */ "xx_common_expr ::= xx_mcall_expr",
 /* 201 */ "xx_common_expr ::= xx_scall_expr",
 /* 202 */ "xx_scall_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 203 */ "xx_scall_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 204 */ "xx_mcall_expr ::= IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 205 */ "xx_mcall_expr ::= IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 206 */ "xx_call_parameters ::= xx_call_parameters COMMA xx_call_parameter",
 /* 207 */ "xx_call_parameters ::= xx_call_parameter",
 /* 208 */ "xx_call_parameter ::= xx_common_expr",
 /* 209 */ "xx_array_list ::= xx_array_list COMMA xx_array_item",
 /* 210 */ "xx_array_list ::= xx_array_item",
 /* 211 */ "xx_array_item ::= xx_array_key COLON xx_array_value",
 /* 212 */ "xx_array_item ::= xx_array_value",
 /* 213 */ "xx_array_key ::= IDENTIFIER",
 /* 214 */ "xx_array_key ::= STRING",
 /* 215 */ "xx_array_key ::= INTEGER",
 /* 216 */ "xx_array_value ::= xx_literal_expr",
 /* 217 */ "xx_array_value ::= IDENTIFIER",
 /* 218 */ "xx_literal_expr ::= INTEGER",
 /* 219 */ "xx_literal_expr ::= STRING",
 /* 220 */ "xx_literal_expr ::= DOUBLE",
 /* 221 */ "xx_literal_expr ::= NULL",
 /* 222 */ "xx_literal_expr ::= FALSE",
 /* 223 */ "xx_literal_expr ::= TRUE",
 /* 224 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN IDENTIFIER SBRACKET_CLOSE",
 /* 225 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN INTEGER SBRACKET_CLOSE",
 /* 226 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN STRING SBRACKET_CLOSE",
 /* 227 */ "xx_eval_expr ::= xx_common_expr",
 /* 228 */ "xx_comment ::= COMMENT",
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
// 814 "parser.lemon"
{
	/*if ((yypminor->yy0)) {
		if ((yypminor->yy0)->free_flag) {
			efree((yypminor->yy0)->token);
		}
		efree((yypminor->yy0));
	}*/
}
// 2053 "parser.c"
      break;
    case 90:
// 827 "parser.lemon"
{ json_object_put((yypminor->yy289)); }
// 2058 "parser.c"
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
  { 89, 1 },
  { 90, 1 },
  { 91, 2 },
  { 91, 1 },
  { 92, 1 },
  { 92, 1 },
  { 92, 1 },
  { 93, 3 },
  { 94, 5 },
  { 94, 7 },
  { 94, 6 },
  { 94, 8 },
  { 94, 4 },
  { 94, 6 },
  { 94, 5 },
  { 94, 7 },
  { 96, 1 },
  { 96, 1 },
  { 96, 1 },
  { 96, 2 },
  { 96, 2 },
  { 96, 2 },
  { 96, 2 },
  { 96, 3 },
  { 96, 3 },
  { 97, 2 },
  { 97, 1 },
  { 100, 4 },
  { 100, 3 },
  { 100, 6 },
  { 100, 5 },
  { 101, 2 },
  { 101, 1 },
  { 103, 1 },
  { 103, 1 },
  { 103, 1 },
  { 103, 1 },
  { 98, 2 },
  { 98, 1 },
  { 99, 2 },
  { 99, 1 },
  { 104, 6 },
  { 104, 5 },
  { 105, 7 },
  { 105, 8 },
  { 105, 8 },
  { 105, 9 },
  { 105, 8 },
  { 105, 9 },
  { 105, 9 },
  { 105, 10 },
  { 106, 3 },
  { 106, 1 },
  { 109, 1 },
  { 109, 1 },
  { 109, 1 },
  { 109, 1 },
  { 109, 1 },
  { 109, 1 },
  { 109, 1 },
  { 107, 3 },
  { 107, 1 },
  { 110, 1 },
  { 110, 2 },
  { 110, 2 },
  { 110, 3 },
  { 110, 4 },
  { 110, 4 },
  { 112, 3 },
  { 111, 1 },
  { 111, 1 },
  { 111, 1 },
  { 111, 1 },
  { 111, 1 },
  { 111, 1 },
  { 108, 2 },
  { 108, 1 },
  { 113, 1 },
  { 113, 1 },
  { 113, 1 },
  { 113, 1 },
  { 113, 1 },
  { 113, 1 },
  { 113, 1 },
  { 113, 1 },
  { 113, 1 },
  { 113, 1 },
  { 113, 1 },
  { 113, 1 },
  { 113, 1 },
  { 113, 1 },
  { 113, 1 },
  { 113, 1 },
  { 124, 2 },
  { 125, 2 },
  { 115, 4 },
  { 115, 7 },
  { 115, 5 },
  { 115, 9 },
  { 115, 8 },
  { 115, 8 },
  { 127, 4 },
  { 127, 5 },
  { 130, 2 },
  { 130, 1 },
  { 131, 4 },
  { 131, 3 },
  { 116, 3 },
  { 116, 4 },
  { 126, 4 },
  { 126, 5 },
  { 128, 7 },
  { 128, 8 },
  { 128, 9 },
  { 128, 10 },
  { 114, 3 },
  { 133, 3 },
  { 133, 1 },
  { 135, 1 },
  { 135, 1 },
  { 135, 1 },
  { 135, 1 },
  { 135, 1 },
  { 135, 1 },
  { 134, 3 },
  { 134, 5 },
  { 134, 7 },
  { 134, 8 },
  { 134, 5 },
  { 134, 6 },
  { 134, 2 },
  { 134, 2 },
  { 137, 1 },
  { 117, 3 },
  { 138, 3 },
  { 138, 1 },
  { 139, 1 },
  { 119, 2 },
  { 120, 2 },
  { 118, 3 },
  { 121, 5 },
  { 121, 6 },
  { 121, 6 },
  { 121, 6 },
  { 122, 3 },
  { 122, 7 },
  { 123, 3 },
  { 123, 3 },
  { 123, 3 },
  { 123, 3 },
  { 123, 3 },
  { 123, 3 },
  { 123, 3 },
  { 123, 3 },
  { 123, 3 },
  { 123, 3 },
  { 143, 3 },
  { 143, 1 },
  { 144, 1 },
  { 144, 3 },
  { 136, 1 },
  { 132, 2 },
  { 132, 2 },
  { 132, 3 },
  { 132, 3 },
  { 132, 3 },
  { 132, 3 },
  { 132, 3 },
  { 132, 3 },
  { 132, 3 },
  { 132, 3 },
  { 132, 3 },
  { 132, 4 },
  { 132, 3 },
  { 132, 4 },
  { 132, 4 },
  { 132, 4 },
  { 132, 3 },
  { 132, 3 },
  { 132, 3 },
  { 132, 3 },
  { 132, 3 },
  { 132, 2 },
  { 132, 4 },
  { 132, 2 },
  { 132, 1 },
  { 132, 1 },
  { 132, 1 },
  { 132, 1 },
  { 132, 1 },
  { 132, 1 },
  { 132, 1 },
  { 132, 1 },
  { 132, 2 },
  { 132, 3 },
  { 132, 2 },
  { 132, 4 },
  { 132, 5 },
  { 132, 4 },
  { 132, 3 },
  { 132, 1 },
  { 132, 1 },
  { 141, 6 },
  { 141, 5 },
  { 140, 6 },
  { 140, 5 },
  { 142, 3 },
  { 142, 1 },
  { 147, 1 },
  { 146, 3 },
  { 146, 1 },
  { 148, 3 },
  { 148, 1 },
  { 149, 1 },
  { 149, 1 },
  { 149, 1 },
  { 150, 1 },
  { 150, 1 },
  { 102, 1 },
  { 102, 1 },
  { 102, 1 },
  { 102, 1 },
  { 102, 1 },
  { 102, 1 },
  { 145, 4 },
  { 145, 4 },
  { 145, 4 },
  { 129, 1 },
  { 95, 1 },
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
// 823 "parser.lemon"
{
	status->ret = yymsp[0].minor.yy289;
}
// 2504 "parser.c"
        break;
      case 1:
      case 4:
      case 5:
      case 6:
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
      case 132:
      case 160:
      case 200:
      case 201:
      case 208:
      case 216:
      case 227:
// 829 "parser.lemon"
{
	yygotominor.yy289 = yymsp[0].minor.yy289;
}
// 2537 "parser.c"
        break;
      case 2:
      case 25:
      case 31:
      case 37:
      case 39:
      case 75:
      case 103:
// 833 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_list(yymsp[-1].minor.yy289, yymsp[0].minor.yy289);
}
// 2550 "parser.c"
        break;
      case 3:
      case 26:
      case 32:
      case 38:
      case 40:
      case 52:
      case 61:
      case 76:
      case 104:
      case 117:
      case 135:
      case 157:
      case 207:
      case 210:
// 837 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_list(NULL, yymsp[0].minor.yy289);
}
// 2570 "parser.c"
        break;
      case 7:
// 853 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_namespace(yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(26,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 2579 "parser.c"
        break;
      case 8:
// 857 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[-1].minor.yy289, 0, NULL, status->scanner_state);
  yy_destructor(29,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 2589 "parser.c"
        break;
      case 9:
// 861 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_class(yymsp[-5].minor.yy0, yymsp[-1].minor.yy289, 0, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(29,&yymsp[-6].minor);
  yy_destructor(32,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 2600 "parser.c"
        break;
      case 10:
// 865 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[-1].minor.yy289, 1, NULL, status->scanner_state);
  yy_destructor(33,&yymsp[-5].minor);
  yy_destructor(29,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 2611 "parser.c"
        break;
      case 11:
// 869 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_class(yymsp[-5].minor.yy0, yymsp[-1].minor.yy289, 1, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(33,&yymsp[-7].minor);
  yy_destructor(29,&yymsp[-6].minor);
  yy_destructor(32,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 2623 "parser.c"
        break;
      case 12:
// 873 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_class(yymsp[-2].minor.yy0, NULL, 0, NULL, status->scanner_state);
  yy_destructor(29,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 2633 "parser.c"
        break;
      case 13:
// 877 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_class(yymsp[-4].minor.yy0, NULL, 0, yymsp[-2].minor.yy0, status->scanner_state);
  yy_destructor(29,&yymsp[-5].minor);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 2644 "parser.c"
        break;
      case 14:
// 881 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_class(yymsp[-2].minor.yy0, NULL, 1, NULL, status->scanner_state);
  yy_destructor(33,&yymsp[-4].minor);
  yy_destructor(29,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 2655 "parser.c"
        break;
      case 15:
// 885 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_class(yymsp[-4].minor.yy0, NULL, 1, yymsp[-2].minor.yy0, status->scanner_state);
  yy_destructor(33,&yymsp[-6].minor);
  yy_destructor(29,&yymsp[-5].minor);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 2667 "parser.c"
        break;
      case 16:
// 889 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_class_definition(yymsp[0].minor.yy289, NULL, NULL, status->scanner_state);
}
// 2674 "parser.c"
        break;
      case 17:
// 893 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_class_definition(NULL, NULL, yymsp[0].minor.yy289, status->scanner_state);
}
// 2681 "parser.c"
        break;
      case 18:
// 897 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_class_definition(NULL, yymsp[0].minor.yy289, NULL, status->scanner_state);
}
// 2688 "parser.c"
        break;
      case 19:
// 901 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_class_definition(yymsp[-1].minor.yy289, yymsp[0].minor.yy289, NULL, status->scanner_state);
}
// 2695 "parser.c"
        break;
      case 20:
// 905 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_class_definition(yymsp[-1].minor.yy289, NULL, yymsp[0].minor.yy289, status->scanner_state);
}
// 2702 "parser.c"
        break;
      case 21:
// 909 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_class_definition(yymsp[0].minor.yy289, NULL, yymsp[-1].minor.yy289, status->scanner_state);
}
// 2709 "parser.c"
        break;
      case 22:
// 913 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_class_definition(NULL, yymsp[0].minor.yy289, yymsp[-1].minor.yy289, status->scanner_state);
}
// 2716 "parser.c"
        break;
      case 23:
// 917 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_class_definition(yymsp[-2].minor.yy289, yymsp[0].minor.yy289, yymsp[-1].minor.yy289, status->scanner_state);
}
// 2723 "parser.c"
        break;
      case 24:
// 921 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_class_definition(yymsp[-1].minor.yy289, yymsp[0].minor.yy289, yymsp[-2].minor.yy289, status->scanner_state);
}
// 2730 "parser.c"
        break;
      case 27:
// 933 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_class_property(yymsp[-2].minor.yy289, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(28,&yymsp[0].minor);
}
// 2738 "parser.c"
        break;
      case 28:
// 937 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_class_property(yymsp[-2].minor.yy289, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[0].minor);
}
// 2746 "parser.c"
        break;
      case 29:
// 941 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_class_property(yymsp[-4].minor.yy289, yymsp[-3].minor.yy0, yymsp[-1].minor.yy289, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(35,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 2755 "parser.c"
        break;
      case 30:
// 945 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_class_property(yymsp[-4].minor.yy289, yymsp[-3].minor.yy0, yymsp[-1].minor.yy289, NULL, status->scanner_state);
  yy_destructor(35,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 2764 "parser.c"
        break;
      case 33:
      case 53:
// 957 "parser.lemon"
{
	yygotominor.yy289 = json_object_new_string("public");
  yy_destructor(1,&yymsp[0].minor);
}
// 2773 "parser.c"
        break;
      case 34:
      case 54:
// 961 "parser.lemon"
{
	yygotominor.yy289 = json_object_new_string("protected");
  yy_destructor(2,&yymsp[0].minor);
}
// 2782 "parser.c"
        break;
      case 35:
      case 55:
// 965 "parser.lemon"
{
	yygotominor.yy289 = json_object_new_string("static");
  yy_destructor(3,&yymsp[0].minor);
}
// 2791 "parser.c"
        break;
      case 36:
      case 56:
// 969 "parser.lemon"
{
	yygotominor.yy289 = json_object_new_string("private");
  yy_destructor(4,&yymsp[0].minor);
}
// 2800 "parser.c"
        break;
      case 41:
// 989 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy289, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(35,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 2810 "parser.c"
        break;
      case 42:
// 993 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy289, NULL, status->scanner_state);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(35,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 2820 "parser.c"
        break;
      case 43:
// 997 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_class_method(yymsp[-6].minor.yy289, yymsp[-4].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(37,&yymsp[-5].minor);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 2832 "parser.c"
        break;
      case 44:
// 1001 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_class_method(yymsp[-7].minor.yy289, yymsp[-5].minor.yy0, yymsp[-3].minor.yy289, NULL, NULL, status->scanner_state);
  yy_destructor(37,&yymsp[-6].minor);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 2844 "parser.c"
        break;
      case 45:
// 1005 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_class_method(yymsp[-7].minor.yy289, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy289, NULL, status->scanner_state);
  yy_destructor(37,&yymsp[-6].minor);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(25,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 2856 "parser.c"
        break;
      case 46:
// 1009 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_class_method(yymsp[-8].minor.yy289, yymsp[-6].minor.yy0, yymsp[-4].minor.yy289, yymsp[-1].minor.yy289, NULL, status->scanner_state);
  yy_destructor(37,&yymsp[-7].minor);
  yy_destructor(38,&yymsp[-5].minor);
  yy_destructor(25,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 2868 "parser.c"
        break;
      case 47:
// 1013 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_class_method(yymsp[-6].minor.yy289, yymsp[-4].minor.yy0, NULL, NULL, yymsp[-7].minor.yy0, status->scanner_state);
  yy_destructor(37,&yymsp[-5].minor);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 2880 "parser.c"
        break;
      case 48:
// 1017 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_class_method(yymsp[-7].minor.yy289, yymsp[-5].minor.yy0, yymsp[-3].minor.yy289, NULL, yymsp[-8].minor.yy0, status->scanner_state);
  yy_destructor(37,&yymsp[-6].minor);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 2892 "parser.c"
        break;
      case 49:
// 1021 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_class_method(yymsp[-7].minor.yy289, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy289, yymsp[-8].minor.yy0, status->scanner_state);
  yy_destructor(37,&yymsp[-6].minor);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(25,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 2904 "parser.c"
        break;
      case 50:
// 1025 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_class_method(yymsp[-8].minor.yy289, yymsp[-6].minor.yy0, yymsp[-4].minor.yy289, yymsp[-1].minor.yy289, yymsp[-9].minor.yy0, status->scanner_state);
  yy_destructor(37,&yymsp[-7].minor);
  yy_destructor(38,&yymsp[-5].minor);
  yy_destructor(25,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 2916 "parser.c"
        break;
      case 51:
// 1029 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_list(yymsp[-2].minor.yy289, yymsp[0].minor.yy289);
  yy_destructor(16,&yymsp[-1].minor);
}
// 2924 "parser.c"
        break;
      case 57:
// 1053 "parser.lemon"
{
	yygotominor.yy289 = json_object_new_string("private");
  yy_destructor(39,&yymsp[0].minor);
}
// 2932 "parser.c"
        break;
      case 58:
// 1057 "parser.lemon"
{
	yygotominor.yy289 = json_object_new_string("abstract");
  yy_destructor(33,&yymsp[0].minor);
}
// 2940 "parser.c"
        break;
      case 59:
// 1061 "parser.lemon"
{
	yygotominor.yy289 = json_object_new_string("final");
  yy_destructor(40,&yymsp[0].minor);
}
// 2948 "parser.c"
        break;
      case 60:
      case 116:
      case 134:
      case 156:
      case 206:
      case 209:
// 1105 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_list(yymsp[-2].minor.yy289, yymsp[0].minor.yy289);
  yy_destructor(5,&yymsp[-1].minor);
}
// 2961 "parser.c"
        break;
      case 62:
// 1113 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_parameter(NULL, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 2968 "parser.c"
        break;
      case 63:
// 1117 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_parameter(yymsp[-1].minor.yy289, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 2975 "parser.c"
        break;
      case 64:
// 1121 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_parameter(NULL, yymsp[-1].minor.yy289, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 2982 "parser.c"
        break;
      case 65:
// 1125 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_parameter(NULL, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy289, status->scanner_state);
  yy_destructor(35,&yymsp[-1].minor);
}
// 2990 "parser.c"
        break;
      case 66:
// 1129 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_parameter(yymsp[-3].minor.yy289, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy289, status->scanner_state);
  yy_destructor(35,&yymsp[-1].minor);
}
// 2998 "parser.c"
        break;
      case 67:
// 1133 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_parameter(NULL, yymsp[-3].minor.yy289, yymsp[-2].minor.yy0, yymsp[0].minor.yy289, status->scanner_state);
  yy_destructor(35,&yymsp[-1].minor);
}
// 3006 "parser.c"
        break;
      case 68:
// 1137 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(10,&yymsp[-2].minor);
  yy_destructor(11,&yymsp[0].minor);
}
// 3015 "parser.c"
        break;
      case 69:
// 1141 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_type(XX_TYPE_INTEGER);
  yy_destructor(41,&yymsp[0].minor);
}
// 3023 "parser.c"
        break;
      case 70:
// 1145 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_type(XX_TYPE_DOUBLE);
  yy_destructor(42,&yymsp[0].minor);
}
// 3031 "parser.c"
        break;
      case 71:
// 1149 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_type(XX_TYPE_BOOL);
  yy_destructor(43,&yymsp[0].minor);
}
// 3039 "parser.c"
        break;
      case 72:
// 1153 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_type(XX_TYPE_STRING);
  yy_destructor(44,&yymsp[0].minor);
}
// 3047 "parser.c"
        break;
      case 73:
// 1157 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_type(XX_TYPE_ARRAY);
  yy_destructor(45,&yymsp[0].minor);
}
// 3055 "parser.c"
        break;
      case 74:
// 1161 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_type(XX_TYPE_VAR);
  yy_destructor(46,&yymsp[0].minor);
}
// 3063 "parser.c"
        break;
      case 93:
// 1237 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_break_statement(status->scanner_state);
  yy_destructor(47,&yymsp[-1].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3072 "parser.c"
        break;
      case 94:
// 1241 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_continue_statement(status->scanner_state);
  yy_destructor(48,&yymsp[-1].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3081 "parser.c"
        break;
      case 95:
// 1245 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_if_statement(yymsp[-2].minor.yy289, NULL, NULL, status->scanner_state);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3091 "parser.c"
        break;
      case 96:
// 1249 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_if_statement(yymsp[-5].minor.yy289, NULL, NULL, status->scanner_state);
  yy_destructor(49,&yymsp[-6].minor);
  yy_destructor(30,&yymsp[-4].minor);
  yy_destructor(31,&yymsp[-3].minor);
  yy_destructor(50,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3104 "parser.c"
        break;
      case 97:
// 1253 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_if_statement(yymsp[-3].minor.yy289, yymsp[-1].minor.yy289, NULL, status->scanner_state);
  yy_destructor(49,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3114 "parser.c"
        break;
      case 98:
// 1257 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_if_statement(yymsp[-7].minor.yy289, yymsp[-5].minor.yy289, yymsp[-1].minor.yy289, status->scanner_state);
  yy_destructor(49,&yymsp[-8].minor);
  yy_destructor(30,&yymsp[-6].minor);
  yy_destructor(31,&yymsp[-4].minor);
  yy_destructor(50,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3127 "parser.c"
        break;
      case 99:
// 1261 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_if_statement(yymsp[-6].minor.yy289, yymsp[-4].minor.yy289, NULL, status->scanner_state);
  yy_destructor(49,&yymsp[-7].minor);
  yy_destructor(30,&yymsp[-5].minor);
  yy_destructor(31,&yymsp[-3].minor);
  yy_destructor(50,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3140 "parser.c"
        break;
      case 100:
// 1265 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_if_statement(yymsp[-6].minor.yy289, NULL, yymsp[-1].minor.yy289, status->scanner_state);
  yy_destructor(49,&yymsp[-7].minor);
  yy_destructor(30,&yymsp[-5].minor);
  yy_destructor(31,&yymsp[-4].minor);
  yy_destructor(50,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3153 "parser.c"
        break;
      case 101:
// 1269 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_switch_statement(yymsp[-2].minor.yy289, NULL, status->scanner_state);
  yy_destructor(51,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3163 "parser.c"
        break;
      case 102:
// 1273 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_switch_statement(yymsp[-3].minor.yy289, yymsp[-1].minor.yy289, status->scanner_state);
  yy_destructor(51,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3173 "parser.c"
        break;
      case 105:
// 1285 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_case_clause(yymsp[-2].minor.yy289, yymsp[0].minor.yy289, status->scanner_state);
  yy_destructor(52,&yymsp[-3].minor);
  yy_destructor(53,&yymsp[-1].minor);
}
// 3182 "parser.c"
        break;
      case 106:
// 1289 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_case_clause(NULL, yymsp[0].minor.yy289, status->scanner_state);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(53,&yymsp[-1].minor);
}
// 3191 "parser.c"
        break;
      case 107:
// 1293 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_loop_statement(NULL, status->scanner_state);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3201 "parser.c"
        break;
      case 108:
// 1297 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_loop_statement(yymsp[-1].minor.yy289, status->scanner_state);
  yy_destructor(55,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3211 "parser.c"
        break;
      case 109:
// 1301 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_while_statement(yymsp[-2].minor.yy289, NULL, status->scanner_state);
  yy_destructor(56,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3221 "parser.c"
        break;
      case 110:
// 1305 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_while_statement(yymsp[-3].minor.yy289, yymsp[-1].minor.yy289, status->scanner_state);
  yy_destructor(56,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3231 "parser.c"
        break;
      case 111:
// 1309 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_for_statement(yymsp[-3].minor.yy289, NULL, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy289, status->scanner_state);
  yy_destructor(57,&yymsp[-6].minor);
  yy_destructor(58,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3242 "parser.c"
        break;
      case 112:
// 1313 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_for_statement(yymsp[-3].minor.yy289, NULL, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy289, status->scanner_state);
  yy_destructor(57,&yymsp[-7].minor);
  yy_destructor(58,&yymsp[-5].minor);
  yy_destructor(59,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3254 "parser.c"
        break;
      case 113:
// 1317 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_for_statement(yymsp[-3].minor.yy289, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy289, status->scanner_state);
  yy_destructor(57,&yymsp[-8].minor);
  yy_destructor(5,&yymsp[-6].minor);
  yy_destructor(58,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3266 "parser.c"
        break;
      case 114:
// 1321 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_for_statement(yymsp[-3].minor.yy289, yymsp[-8].minor.yy0, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy289, status->scanner_state);
  yy_destructor(57,&yymsp[-9].minor);
  yy_destructor(5,&yymsp[-7].minor);
  yy_destructor(58,&yymsp[-5].minor);
  yy_destructor(59,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3279 "parser.c"
        break;
      case 115:
// 1325 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_let_statement(yymsp[-1].minor.yy289, status->scanner_state);
  yy_destructor(60,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3288 "parser.c"
        break;
      case 118:
// 1338 "parser.lemon"
{
	yygotominor.yy289 = json_object_new_string("assign");
  yy_destructor(35,&yymsp[0].minor);
}
// 3296 "parser.c"
        break;
      case 119:
// 1343 "parser.lemon"
{
	yygotominor.yy289 = json_object_new_string("add-assign");
  yy_destructor(61,&yymsp[0].minor);
}
// 3304 "parser.c"
        break;
      case 120:
// 1348 "parser.lemon"
{
	yygotominor.yy289 = json_object_new_string("sub-assign");
  yy_destructor(62,&yymsp[0].minor);
}
// 3312 "parser.c"
        break;
      case 121:
// 1352 "parser.lemon"
{
	yygotominor.yy289 = json_object_new_string("mult-assign");
  yy_destructor(63,&yymsp[0].minor);
}
// 3320 "parser.c"
        break;
      case 122:
// 1356 "parser.lemon"
{
	yygotominor.yy289 = json_object_new_string("div-assign");
  yy_destructor(64,&yymsp[0].minor);
}
// 3328 "parser.c"
        break;
      case 123:
// 1360 "parser.lemon"
{
	yygotominor.yy289 = json_object_new_string("concat-assign");
  yy_destructor(65,&yymsp[0].minor);
}
// 3336 "parser.c"
        break;
      case 124:
// 1365 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_let_assignment("variable", yymsp[-1].minor.yy289, yymsp[-2].minor.yy0, NULL, NULL, yymsp[0].minor.yy289, status->scanner_state);
}
// 3343 "parser.c"
        break;
      case 125:
// 1370 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_let_assignment("object-property", yymsp[-1].minor.yy289, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy289, status->scanner_state);
  yy_destructor(66,&yymsp[-3].minor);
}
// 3351 "parser.c"
        break;
      case 126:
// 1375 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_let_assignment("object-property-append", yymsp[-1].minor.yy289, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy289, status->scanner_state);
  yy_destructor(66,&yymsp[-5].minor);
  yy_destructor(67,&yymsp[-3].minor);
  yy_destructor(68,&yymsp[-2].minor);
}
// 3361 "parser.c"
        break;
      case 127:
// 1380 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_let_assignment("object-property-array-index", yymsp[-1].minor.yy289, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy289, yymsp[0].minor.yy289, status->scanner_state);
  yy_destructor(66,&yymsp[-6].minor);
  yy_destructor(67,&yymsp[-4].minor);
  yy_destructor(68,&yymsp[-2].minor);
}
// 3371 "parser.c"
        break;
      case 128:
// 1385 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_let_assignment("variable-append", yymsp[-1].minor.yy289, yymsp[-4].minor.yy0, NULL, NULL, yymsp[0].minor.yy289, status->scanner_state);
  yy_destructor(67,&yymsp[-3].minor);
  yy_destructor(68,&yymsp[-2].minor);
}
// 3380 "parser.c"
        break;
      case 129:
// 1390 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_let_assignment("array-index", yymsp[-1].minor.yy289, yymsp[-5].minor.yy0, NULL, yymsp[-3].minor.yy289, yymsp[0].minor.yy289, status->scanner_state);
  yy_destructor(67,&yymsp[-4].minor);
  yy_destructor(68,&yymsp[-2].minor);
}
// 3389 "parser.c"
        break;
      case 130:
// 1395 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_let_assignment("incr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(69,&yymsp[0].minor);
}
// 3397 "parser.c"
        break;
      case 131:
// 1400 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_let_assignment("decr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(70,&yymsp[0].minor);
}
// 3405 "parser.c"
        break;
      case 133:
// 1408 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_echo_statement(yymsp[-1].minor.yy289, status->scanner_state);
  yy_destructor(71,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3414 "parser.c"
        break;
      case 136:
// 1420 "parser.lemon"
{
	yygotominor.yy289 = yymsp[0].minor.yy289;;
}
// 3421 "parser.c"
        break;
      case 137:
// 1425 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_mcall_statement(yymsp[-1].minor.yy289, status->scanner_state);
  yy_destructor(28,&yymsp[0].minor);
}
// 3429 "parser.c"
        break;
      case 138:
// 1430 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_scall_statement(yymsp[-1].minor.yy289, status->scanner_state);
  yy_destructor(28,&yymsp[0].minor);
}
// 3437 "parser.c"
        break;
      case 139:
// 1435 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_return_statement(yymsp[-1].minor.yy289, status->scanner_state);
  yy_destructor(72,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3446 "parser.c"
        break;
      case 140:
// 1440 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(73,&yymsp[-4].minor);
  yy_destructor(66,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3460 "parser.c"
        break;
      case 141:
// 1449 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(73,&yymsp[-5].minor);
  yy_destructor(67,&yymsp[-3].minor);
  yy_destructor(68,&yymsp[-1].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3475 "parser.c"
        break;
      case 142:
// 1458 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_INTEGER, yymsp[-2].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(73,&yymsp[-5].minor);
  yy_destructor(67,&yymsp[-3].minor);
  yy_destructor(68,&yymsp[-1].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3490 "parser.c"
        break;
      case 143:
// 1467 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_STRING, yymsp[-2].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(73,&yymsp[-5].minor);
  yy_destructor(67,&yymsp[-3].minor);
  yy_destructor(68,&yymsp[-1].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3505 "parser.c"
        break;
      case 144:
// 1476 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_throw_exception(yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(76,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3514 "parser.c"
        break;
      case 145:
// 1481 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_throw_exception(yymsp[-4].minor.yy0, yymsp[-2].minor.yy289, status->scanner_state);
  yy_destructor(76,&yymsp[-6].minor);
  yy_destructor(23,&yymsp[-5].minor);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(25,&yymsp[-1].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3526 "parser.c"
        break;
      case 146:
// 1485 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_declare_statement(XX_T_TYPE_INTEGER, yymsp[-1].minor.yy289, status->scanner_state);
  yy_destructor(41,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3535 "parser.c"
        break;
      case 147:
// 1489 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_declare_statement(XX_T_TYPE_UINTEGER, yymsp[-1].minor.yy289, status->scanner_state);
  yy_destructor(77,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3544 "parser.c"
        break;
      case 148:
// 1493 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_declare_statement(XX_T_TYPE_CHAR, yymsp[-1].minor.yy289, status->scanner_state);
  yy_destructor(78,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3553 "parser.c"
        break;
      case 149:
// 1497 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_declare_statement(XX_T_TYPE_UCHAR, yymsp[-1].minor.yy289, status->scanner_state);
  yy_destructor(79,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3562 "parser.c"
        break;
      case 150:
// 1501 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_declare_statement(XX_T_TYPE_LONG, yymsp[-1].minor.yy289, status->scanner_state);
  yy_destructor(80,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3571 "parser.c"
        break;
      case 151:
// 1505 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_declare_statement(XX_T_TYPE_ULONG, yymsp[-1].minor.yy289, status->scanner_state);
  yy_destructor(81,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3580 "parser.c"
        break;
      case 152:
// 1509 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_declare_statement(XX_T_TYPE_DOUBLE, yymsp[-1].minor.yy289, status->scanner_state);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3589 "parser.c"
        break;
      case 153:
// 1513 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_declare_statement(XX_T_TYPE_STRING, yymsp[-1].minor.yy289, status->scanner_state);
  yy_destructor(44,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3598 "parser.c"
        break;
      case 154:
// 1517 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_declare_statement(XX_T_TYPE_BOOL, yymsp[-1].minor.yy289, status->scanner_state);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3607 "parser.c"
        break;
      case 155:
// 1521 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_declare_statement(XX_T_TYPE_VAR, yymsp[-1].minor.yy289, status->scanner_state);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3616 "parser.c"
        break;
      case 158:
// 1533 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_declare_variable(yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3623 "parser.c"
        break;
      case 159:
// 1537 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_declare_variable(yymsp[-2].minor.yy0, yymsp[0].minor.yy289, status->scanner_state);
  yy_destructor(35,&yymsp[-1].minor);
}
// 3631 "parser.c"
        break;
      case 161:
// 1545 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_expr("not", yymsp[0].minor.yy289, NULL, NULL, status->scanner_state);
  yy_destructor(24,&yymsp[-1].minor);
}
// 3639 "parser.c"
        break;
      case 162:
// 1549 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_expr("isset", yymsp[0].minor.yy289, NULL, NULL, status->scanner_state);
  yy_destructor(19,&yymsp[-1].minor);
}
// 3647 "parser.c"
        break;
      case 163:
// 1553 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_expr("equals", yymsp[-2].minor.yy289, yymsp[0].minor.yy289, NULL, status->scanner_state);
  yy_destructor(8,&yymsp[-1].minor);
}
// 3655 "parser.c"
        break;
      case 164:
// 1557 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_expr("not-equals", yymsp[-2].minor.yy289, yymsp[0].minor.yy289, NULL, status->scanner_state);
  yy_destructor(15,&yymsp[-1].minor);
}
// 3663 "parser.c"
        break;
      case 165:
// 1561 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_expr("identical", yymsp[-2].minor.yy289, yymsp[0].minor.yy289, NULL, status->scanner_state);
  yy_destructor(9,&yymsp[-1].minor);
}
// 3671 "parser.c"
        break;
      case 166:
// 1565 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_expr("not-identical", yymsp[-2].minor.yy289, yymsp[0].minor.yy289, NULL, status->scanner_state);
  yy_destructor(14,&yymsp[-1].minor);
}
// 3679 "parser.c"
        break;
      case 167:
// 1569 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_expr("less", yymsp[-2].minor.yy289, yymsp[0].minor.yy289, NULL, status->scanner_state);
  yy_destructor(10,&yymsp[-1].minor);
}
// 3687 "parser.c"
        break;
      case 168:
// 1573 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_expr("greater", yymsp[-2].minor.yy289, yymsp[0].minor.yy289, NULL, status->scanner_state);
  yy_destructor(11,&yymsp[-1].minor);
}
// 3695 "parser.c"
        break;
      case 169:
// 1577 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_expr("less-equal", yymsp[-2].minor.yy289, yymsp[0].minor.yy289, NULL, status->scanner_state);
  yy_destructor(12,&yymsp[-1].minor);
}
// 3703 "parser.c"
        break;
      case 170:
// 1581 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_expr("greater-equal", yymsp[-2].minor.yy289, yymsp[0].minor.yy289, NULL, status->scanner_state);
  yy_destructor(13,&yymsp[-1].minor);
}
// 3711 "parser.c"
        break;
      case 171:
// 1585 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_expr("list", yymsp[-1].minor.yy289, NULL, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(25,&yymsp[0].minor);
}
// 3720 "parser.c"
        break;
      case 172:
// 1589 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_expr("cast", yymsp[-2].minor.yy289, yymsp[0].minor.yy289, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(25,&yymsp[-1].minor);
}
// 3729 "parser.c"
        break;
      case 173:
// 1593 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_expr("property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(66,&yymsp[-1].minor);
}
// 3737 "parser.c"
        break;
      case 174:
      case 224:
// 1597 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(67,&yymsp[-2].minor);
  yy_destructor(68,&yymsp[0].minor);
}
// 3747 "parser.c"
        break;
      case 175:
      case 225:
// 1602 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_INTEGER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(67,&yymsp[-2].minor);
  yy_destructor(68,&yymsp[0].minor);
}
// 3757 "parser.c"
        break;
      case 176:
      case 226:
// 1607 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_STRING, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(67,&yymsp[-2].minor);
  yy_destructor(68,&yymsp[0].minor);
}
// 3767 "parser.c"
        break;
      case 177:
// 1612 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_expr("add", yymsp[-2].minor.yy289, yymsp[0].minor.yy289, NULL, status->scanner_state);
  yy_destructor(16,&yymsp[-1].minor);
}
// 3775 "parser.c"
        break;
      case 178:
// 1617 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_expr("sub", yymsp[-2].minor.yy289, yymsp[0].minor.yy289, NULL, status->scanner_state);
  yy_destructor(17,&yymsp[-1].minor);
}
// 3783 "parser.c"
        break;
      case 179:
// 1622 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_expr("concat", yymsp[-2].minor.yy289, yymsp[0].minor.yy289, NULL, status->scanner_state);
  yy_destructor(18,&yymsp[-1].minor);
}
// 3791 "parser.c"
        break;
      case 180:
// 1627 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_expr("and", yymsp[-2].minor.yy289, yymsp[0].minor.yy289, NULL, status->scanner_state);
  yy_destructor(6,&yymsp[-1].minor);
}
// 3799 "parser.c"
        break;
      case 181:
// 1632 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_expr("or", yymsp[-2].minor.yy289, yymsp[0].minor.yy289, NULL, status->scanner_state);
  yy_destructor(7,&yymsp[-1].minor);
}
// 3807 "parser.c"
        break;
      case 182:
// 1637 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_expr("instanceof", yymsp[0].minor.yy289, NULL, NULL, status->scanner_state);
  yy_destructor(22,&yymsp[-1].minor);
}
// 3815 "parser.c"
        break;
      case 183:
// 1642 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_expr("fetch", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy289, NULL, status->scanner_state);
  yy_destructor(20,&yymsp[-3].minor);
  yy_destructor(5,&yymsp[-1].minor);
}
// 3824 "parser.c"
        break;
      case 184:
// 1647 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_expr("typeof", yymsp[0].minor.yy289, NULL, NULL, status->scanner_state);
  yy_destructor(21,&yymsp[-1].minor);
}
// 3832 "parser.c"
        break;
      case 185:
      case 213:
      case 217:
// 1652 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state);
}
// 3841 "parser.c"
        break;
      case 186:
      case 215:
      case 218:
// 1657 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_literal(XX_T_INTEGER, yymsp[0].minor.yy0, status->scanner_state);
}
// 3850 "parser.c"
        break;
      case 187:
      case 214:
      case 219:
// 1662 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_literal(XX_T_STRING, yymsp[0].minor.yy0, status->scanner_state);
}
// 3859 "parser.c"
        break;
      case 188:
// 1667 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_literal(XX_T_SCHAR, yymsp[0].minor.yy0, status->scanner_state);
}
// 3866 "parser.c"
        break;
      case 189:
      case 220:
// 1672 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_literal(XX_T_DOUBLE, yymsp[0].minor.yy0, status->scanner_state);
}
// 3874 "parser.c"
        break;
      case 190:
      case 221:
// 1677 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_literal(XX_T_NULL, NULL, status->scanner_state);
  yy_destructor(84,&yymsp[0].minor);
}
// 3883 "parser.c"
        break;
      case 191:
      case 223:
// 1682 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_literal(XX_T_TRUE, NULL, status->scanner_state);
  yy_destructor(85,&yymsp[0].minor);
}
// 3892 "parser.c"
        break;
      case 192:
      case 222:
// 1687 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_literal(XX_T_FALSE, NULL, status->scanner_state);
  yy_destructor(86,&yymsp[0].minor);
}
// 3901 "parser.c"
        break;
      case 193:
// 1692 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_expr("empty-array", NULL, NULL, NULL, status->scanner_state);
  yy_destructor(67,&yymsp[-1].minor);
  yy_destructor(68,&yymsp[0].minor);
}
// 3910 "parser.c"
        break;
      case 194:
// 1697 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_expr("array", yymsp[-1].minor.yy289, NULL, NULL, status->scanner_state);
  yy_destructor(67,&yymsp[-2].minor);
  yy_destructor(68,&yymsp[0].minor);
}
// 3919 "parser.c"
        break;
      case 195:
// 1702 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_new_instance(yymsp[0].minor.yy0, NULL, status->scanner_state);
  yy_destructor(23,&yymsp[-1].minor);
}
// 3927 "parser.c"
        break;
      case 196:
// 1707 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_new_instance(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(23,&yymsp[-3].minor);
  yy_destructor(38,&yymsp[-1].minor);
  yy_destructor(25,&yymsp[0].minor);
}
// 3937 "parser.c"
        break;
      case 197:
// 1712 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_new_instance(yymsp[-3].minor.yy0, yymsp[-1].minor.yy289, status->scanner_state);
  yy_destructor(23,&yymsp[-4].minor);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(25,&yymsp[0].minor);
}
// 3947 "parser.c"
        break;
      case 198:
// 1717 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_fcall(yymsp[-3].minor.yy0, yymsp[-1].minor.yy289, status->scanner_state);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(25,&yymsp[0].minor);
}
// 3956 "parser.c"
        break;
      case 199:
// 1722 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_fcall(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-1].minor);
  yy_destructor(25,&yymsp[0].minor);
}
// 3965 "parser.c"
        break;
      case 202:
// 1737 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_scall(yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy289, status->scanner_state);
  yy_destructor(87,&yymsp[-4].minor);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(25,&yymsp[0].minor);
}
// 3975 "parser.c"
        break;
      case 203:
// 1742 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_scall(yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(87,&yymsp[-3].minor);
  yy_destructor(38,&yymsp[-1].minor);
  yy_destructor(25,&yymsp[0].minor);
}
// 3985 "parser.c"
        break;
      case 204:
// 1747 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_mcall(yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy289, status->scanner_state);
  yy_destructor(66,&yymsp[-4].minor);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(25,&yymsp[0].minor);
}
// 3995 "parser.c"
        break;
      case 205:
// 1752 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_mcall(yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(66,&yymsp[-3].minor);
  yy_destructor(38,&yymsp[-1].minor);
  yy_destructor(25,&yymsp[0].minor);
}
// 4005 "parser.c"
        break;
      case 211:
// 1776 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_array_item(yymsp[-2].minor.yy289, yymsp[0].minor.yy289, status->scanner_state);
  yy_destructor(53,&yymsp[-1].minor);
}
// 4013 "parser.c"
        break;
      case 212:
// 1780 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_array_item(NULL, yymsp[0].minor.yy289, status->scanner_state);
}
// 4020 "parser.c"
        break;
      case 228:
// 1847 "parser.lemon"
{
	yygotominor.yy289 = xx_ret_comment(yymsp[0].minor.yy0, status->scanner_state);
}
// 4027 "parser.c"
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
// 780 "parser.lemon"


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

// 4103 "parser.c"
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
			case XX_T_EXTENDS:
				xx_(xx_parser, XX_EXTENDS, NULL, parser_status);
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
				xx_(xx_parser, XX_BRACKET_OPEN, NULL, parser_status);
				break;
			case XX_T_BRACKET_CLOSE:
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
				xx_parse_with_token(xx_parser, XX_T_COMMENT, XX_COMMENT, &token, parser_status);
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
			case XX_T_SCHAR:
				xx_parse_with_token(xx_parser, XX_T_SCHAR, XX_SCHAR, &token, parser_status);
				break;
			case XX_T_IDENTIFIER:
				xx_parse_with_token(xx_parser, XX_T_IDENTIFIER, XX_IDENTIFIER, &token, parser_status);
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
