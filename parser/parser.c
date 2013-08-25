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
#define YYNOCODE 150
#define YYACTIONTYPE unsigned short int
#define xx_TOKENTYPE xx_parser_token*
typedef union {
  xx_TOKENTYPE yy0;
  json_object* yy107;
  int yy299;
} YYMINORTYPE;
#define YYSTACKDEPTH 100
#define xx_ARG_SDECL xx_parser_status *status;
#define xx_ARG_PDECL ,xx_parser_status *status
#define xx_ARG_FETCH xx_parser_status *status = yypParser->status
#define xx_ARG_STORE yypParser->status = status
#define YYNSTATE 478
#define YYNRULE 234
#define YYERRORSYMBOL 87
#define YYERRSYMDT yy299
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
 /*     0 */   321,  428,  536,  439,  337,  376,  381,  324,  551,  440,
 /*    10 */   441,  442,  443,  285,  309,  315,  312,  252,  318,   45,
 /*    20 */    47,   49,  365,   58,  251,  198,  199,   69,   73,   78,
 /*    30 */    50,  288,   85,  344,  178,  138,  139,  140,  141,  142,
 /*    40 */   143,  198,  199,  247,  258,  261,  225,  408,  276,  294,
 /*    50 */   297,  300,  303,  306,  317,  321,  173,  153,  534,  343,
 /*    60 */    86,  246,  324,  286,  293,  198,  199,  282,  285,  309,
 /*    70 */   315,  312,  201,  318,   45,   47,   49,  171,   58,  190,
 /*    80 */   175,  186,   69,   73,   78,  538,  452,   85,  455,  456,
 /*    90 */   444,   63,  439,  413,  414,   19,  197,  279,  247,  258,
 /*   100 */   261,  277,  377,  276,  294,  297,  300,  303,  306,  378,
 /*   110 */   321,   65,  272,  346,   57,  535,  283,  324,  172,  400,
 /*   120 */   402,  396,  405,  285,  309,  315,  312,  235,  318,   45,
 /*   130 */    47,   49,  240,   58,  211,  198,  199,   69,   73,   78,
 /*   140 */   295,  293,   85,  411,   17,  426,  444,  288,  439,  450,
 /*   150 */   414,   19,  537,  247,  258,  261,  375,  408,  276,  294,
 /*   160 */   297,  300,  303,  306,  390,  321,  379,  368,  372,  393,
 /*   170 */   296,  374,  324,   88,  440,  441,  442,  443,  285,  309,
 /*   180 */   315,  312,  373,  318,   45,   47,   49,  357,   58,  379,
 /*   190 */   368,  372,   69,   73,   78,  209,   87,   85,  212,  453,
 /*   200 */   426,  444,  322,  439,  499,  414,   19,  454,  247,  258,
 /*   210 */   261,  371,  408,  276,  294,  297,  300,  303,  306,  409,
 /*   220 */   321,  410,   61,  216,  356,  150,  205,  324,  288,   91,
 /*   230 */    12,  178,  458,  285,  309,  315,  312,  472,  318,   45,
 /*   240 */    47,   49,   65,   58,  346,  178,  216,   69,   73,   78,
 /*   250 */    74,  287,   85,  344,   92,   93,   94,   95,   96,  370,
 /*   260 */   227,  198,  199,  247,  258,  261,  298,  293,  276,  294,
 /*   270 */   297,  300,  303,  306,  369,  321,  174,  175,  186,   77,
 /*   280 */   397,  398,  324,  399,  400,  402,  396,  405,  285,  309,
 /*   290 */   315,  312,  177,  318,   45,   47,   49,   99,   58,  403,
 /*   300 */   207,  226,   69,   73,   78,  198,  199,   85,  108,  400,
 /*   310 */   402,  396,  405,  229,  497,  288,  541,  380,  247,  258,
 /*   320 */   261,  367,  408,  276,  294,  297,  300,  303,  306,  401,
 /*   330 */   321,  381,   99,  540,  360,  404,  230,  324,  299,  496,
 /*   340 */   198,  199,  380,  285,  309,  315,  312,  408,  318,   45,
 /*   350 */    47,   49,  406,   58,  110,  112,  417,   69,   73,   78,
 /*   360 */   180,  181,   85,  539,  400,  402,  396,  405,  366,  182,
 /*   370 */   183,  185,  184,  247,  258,  261,  301,  293,  276,  294,
 /*   380 */   297,  300,  303,  306,   67,  321,  542,   99,  407,  395,
 /*   390 */   359,  243,  324,  419,  500,  198,  199,  380,  285,  309,
 /*   400 */   315,  312,  408,  318,   45,   47,   49,   99,   58,  288,
 /*   410 */   418,  234,   69,   73,   78,  198,  199,   85,   66,  400,
 /*   420 */   402,  396,  405,  420,  435,  436,  437,  438,  247,  258,
 /*   430 */   261,  197,  302,  276,  294,  297,  300,  303,  306,  288,
 /*   440 */   321,  127,  129,  131,   53,  304,  293,  324,  288,  502,
 /*   450 */   429,  196,  380,  285,  309,  315,  312,  408,  318,   45,
 /*   460 */    47,   49,  305,   58,  364,  368,  372,   69,   73,   78,
 /*   470 */    59,  308,   85,  344,  713,    1,    2,  477,    4,    5,
 /*   480 */     6,  198,  199,  247,  258,  261,  307,  293,  276,  294,
 /*   490 */   297,  300,  303,  306,  288,  321,  310,  293,  288,  351,
 /*   500 */   313,  293,  324,  232,  400,  402,  396,  405,  285,  309,
 /*   510 */   315,  312,  430,  318,   45,   47,   49,  311,   58,  431,
 /*   520 */   153,  314,   69,   73,   78,  353,  412,   85,  198,  199,
 /*   530 */   219,  413,  414,   19,  501,  201,  422,  380,  247,  258,
 /*   540 */   261,  423,  408,  276,  294,  297,  300,  303,  306,  540,
 /*   550 */   321,  404,  153,  288,  362,  316,  293,  324,   11,   91,
 /*   560 */   198,  199,  202,  285,  309,  315,  312,  201,  318,   45,
 /*   570 */    47,   49,   81,   58,  319,  293,  320,   69,   73,   78,
 /*   580 */   198,  199,   85,  352,   92,   93,   94,   95,   96,  435,
 /*   590 */   436,  437,  438,  247,  258,  261,   18,   19,  276,  294,
 /*   600 */   297,  300,  303,  306,  542,  321,  407,  363,  235,   72,
 /*   610 */    62,  349,  324,  231,  233,  445,  198,  199,  285,  309,
 /*   620 */   315,  312,  446,  318,   45,   47,   49,  358,   58,  447,
 /*   630 */   153,  197,   69,   73,   78,  252,  479,   85,  198,  199,
 /*   640 */   195,  248,  253,  198,  199,  201,  424,  331,  247,  258,
 /*   650 */   261,  203,  425,  276,  294,  297,  300,  303,  306,  250,
 /*   660 */   321,  347,    7,  334,   25,   10,  197,  324,  113,  464,
 /*   670 */   324,  198,  199,  285,  309,  315,  312,  267,  318,   45,
 /*   680 */    47,   49,  249,   58,  193,  153,  214,   69,   73,   78,
 /*   690 */   263,  266,   85,  198,  199,  213,  192,  111,   99,   80,
 /*   700 */   201,   64,   98,  247,  258,  261,  198,  199,  276,  294,
 /*   710 */   297,  300,  303,  306,  467,  321,  471,  153,   48,  387,
 /*   720 */   333,  363,  324,  270,  273,  198,  199,  698,  285,  309,
 /*   730 */   315,  312,  200,  318,   45,   47,   49,  197,   58,  434,
 /*   740 */   473,  391,   69,   73,   78,  332,  161,   85,    3,    4,
 /*   750 */     5,    6,  146,  697,  198,  199,  696,  220,  247,  258,
 /*   760 */   261,  432,  433,  276,  294,  297,  300,  303,  306,  109,
 /*   770 */   321,   99,  238,  144,  350,  239,  152,  324,   60,  198,
 /*   780 */   199,  198,  199,  285,  309,  315,  312,  107,  318,   45,
 /*   790 */    47,   49,  323,   58,  241,    8,  327,   69,   73,   78,
 /*   800 */     7,  339,   85,   10,  198,  199,  242,  464,  324,  198,
 /*   810 */   199,  105,  206,  247,  258,  261,  106,  290,  276,  294,
 /*   820 */   297,  300,  303,  306,  264,  321,  136,  271,  539,  342,
 /*   830 */    75,  157,  324,    9,  198,  199,  541,  448,  285,  309,
 /*   840 */   315,  312,  148,  318,   45,   47,   49,   22,   58,  475,
 /*   850 */   198,  199,   69,   73,   78,  176,  114,   85,  208,  210,
 /*   860 */   449,  151,   20,  102,  198,  199,  478,   79,  247,  258,
 /*   870 */   261,  198,  199,  276,  294,  297,  300,  303,  306,   55,
 /*   880 */   321,  222,   14,  259,   27,   54,  269,  324,  292,  198,
 /*   890 */   199,  198,  199,  285,  309,  315,  312,  116,  318,   45,
 /*   900 */    47,   49,  155,   58,  291,  198,  199,   69,   73,   78,
 /*   910 */   198,  199,   85,  118,   24,  289,  262,   51,  459,   97,
 /*   920 */    46,  198,  199,  247,  258,  261,  460,  284,  276,  294,
 /*   930 */   297,  300,  303,  306,   89,  321,  120,  462,  130,  389,
 /*   940 */   281,  265,  324,   21,  198,  199,  198,  199,  285,  309,
 /*   950 */   315,  312,  122,  318,   45,   47,   49,  132,   58,  147,
 /*   960 */   198,  199,   69,   73,   78,  198,  199,   85,  255,   90,
 /*   970 */   465,  280,  466,  128,  278,  257,  268,  275,  247,  258,
 /*   980 */   261,  198,  199,  276,  294,  297,  300,  303,  306,  159,
 /*   990 */   321,  469,  274,  529,  330,  392,  218,  324,  531,   70,
 /*  1000 */   217,  533,  158,  285,  309,  315,  312,  124,  318,   45,
 /*  1010 */    47,   49,  134,   58,  386,  198,  199,   69,   73,   78,
 /*  1020 */   198,  199,   85,  126,  530,  383,  384,  224,  532,  382,
 /*  1030 */   385,  198,  199,  247,  258,  261,  385,  385,  276,  294,
 /*  1040 */   297,  300,  303,  306,  385,  321,  385,  385,  385,   84,
 /*  1050 */   385,  385,  324,  385,  385,  385,  385,  385,  285,  309,
 /*  1060 */   315,  312,  385,  318,   45,   47,   49,  385,   58,  385,
 /*  1070 */   385,  385,   69,   73,   78,  385,  385,   85,  385,  385,
 /*  1080 */   385,  385,  385,  385,  385,  385,  385,  385,  247,  258,
 /*  1090 */   261,  385,  385,  276,  294,  297,  300,  303,  306,  385,
 /*  1100 */   321,  385,  385,  385,  355,  385,  385,  324,  385,  385,
 /*  1110 */   385,  385,  385,  285,  309,  315,  312,  385,  318,   45,
 /*  1120 */    47,   49,  385,   58,  385,  385,  385,   69,   73,   78,
 /*  1130 */   385,  385,   85,  385,  385,  385,  385,  385,  385,  385,
 /*  1140 */   385,  385,  385,  247,  258,  261,  385,  385,  276,  294,
 /*  1150 */   297,  300,  303,  306,  385,  321,  385,  385,  385,  345,
 /*  1160 */   385,  385,  324,  385,  385,  385,  385,  385,  285,  309,
 /*  1170 */   315,  312,  385,  318,   45,   47,   49,  385,   58,  385,
 /*  1180 */   385,  187,   69,   73,   78,  385,  385,   85,  385,  385,
 /*  1190 */   385,  385,  385,  385,  385,  385,  385,  385,  247,  258,
 /*  1200 */   261,  385,  321,  276,  294,  297,  300,  303,  306,  324,
 /*  1210 */   385,  385,  385,  385,  385,  285,  309,  315,  312,  385,
 /*  1220 */   318,   45,   47,   49,  385,   58,  376,  189,  188,   69,
 /*  1230 */    73,   78,  385,  385,   85,  385,  182,  183,  185,  184,
 /*  1240 */   385,  385,  385,  365,  385,  247,  258,  261,  385,  385,
 /*  1250 */   276,  294,  297,  300,  303,  306,  138,  139,  140,  141,
 /*  1260 */   142,  143,  385,  385,  385,  104,  156,  160,  154,  191,
 /*  1270 */   101,  385,  385,  149,  385,  385,  385,  385,  385,   44,
 /*  1280 */   385,  385,  385,  385,  137,  385,  138,  139,  140,  141,
 /*  1290 */   142,  143,   26,  385,  385,  385,  325,   29,   30,   31,
 /*  1300 */    32,   33,   34,   35,   36,   37,   38,   39,   40,   41,
 /*  1310 */    42,   43,  169,  385,  385,  385,  385,  385,  385,  162,
 /*  1320 */   163,  385,  385,  254,  256,   44,  385,  164,  165,  166,
 /*  1330 */   167,  168,  385,  385,  385,  385,  385,  385,  394,  385,
 /*  1340 */   385,  385,  325,   29,   30,   31,   32,   33,   34,   35,
 /*  1350 */    36,   37,   38,   39,   40,   41,   42,   43,  385,  385,
 /*  1360 */    44,  385,  440,  441,  442,  443,  385,  385,  385,  254,
 /*  1370 */   256,  385,  385,   71,  385,  385,  385,  325,   29,   30,
 /*  1380 */    31,   32,   33,   34,   35,   36,   37,   38,   39,   40,
 /*  1390 */    41,   42,   43,   44,  385,  385,  385,  416,  385,  385,
 /*  1400 */   408,  385,  385,  385,  254,  256,   76,  385,  385,  385,
 /*  1410 */   325,   29,   30,   31,   32,   33,   34,   35,   36,   37,
 /*  1420 */    38,   39,   40,   41,   42,   43,   44,  400,  402,  396,
 /*  1430 */   405,  385,  385,  385,  385,  385,  385,  254,  256,  354,
 /*  1440 */   385,  385,  385,  325,   29,   30,   31,   32,   33,   34,
 /*  1450 */    35,   36,   37,   38,   39,   40,   41,   42,   43,  385,
 /*  1460 */   385,  385,  416,  385,   44,  408,  385,  385,  385,  385,
 /*  1470 */   254,  256,  385,  385,  385,  385,  385,  348,  385,  385,
 /*  1480 */   385,  325,   29,   30,   31,   32,   33,   34,   35,   36,
 /*  1490 */    37,   38,   39,   40,   41,   42,   43,  385,  385,   44,
 /*  1500 */   385,  385,  385,  385,  385,  385,  385,  385,  254,  256,
 /*  1510 */   385,  385,   68,  385,  385,  385,  325,   29,   30,   31,
 /*  1520 */    32,   33,   34,   35,   36,   37,   38,   39,   40,   41,
 /*  1530 */    42,   43,   44,  385,  385,  385,  385,  385,  385,  385,
 /*  1540 */   385,  385,  385,  254,  256,   56,  385,  385,  385,  325,
 /*  1550 */    29,   30,   31,   32,   33,   34,   35,   36,   37,   38,
 /*  1560 */    39,   40,   41,   42,   43,   44,  385,  385,  385,  385,
 /*  1570 */   385,  385,  385,  385,  385,  385,  254,  256,  388,  385,
 /*  1580 */   385,  385,  325,   29,   30,   31,   32,   33,   34,   35,
 /*  1590 */    36,   37,   38,   39,   40,   41,   42,   43,  385,  385,
 /*  1600 */   385,  385,  385,   44,  385,  385,  385,  385,  385,  254,
 /*  1610 */   256,  385,  385,  385,  385,  385,  361,  385,  385,  385,
 /*  1620 */   325,   29,   30,   31,   32,   33,   34,   35,   36,   37,
 /*  1630 */    38,   39,   40,   41,   42,   43,  385,  385,   44,  385,
 /*  1640 */   385,  385,  385,  385,  385,  385,  385,  254,  256,  385,
 /*  1650 */   385,   83,  385,  385,  385,  325,   29,   30,   31,   32,
 /*  1660 */    33,   34,   35,   36,   37,   38,   39,   40,   41,   42,
 /*  1670 */    43,   44,  385,  385,  385,  385,  385,  385,  385,  385,
 /*  1680 */   385,  385,  254,  256,  341,  385,  385,  385,  325,   29,
 /*  1690 */    30,   31,   32,   33,   34,   35,   36,   37,   38,   39,
 /*  1700 */    40,   41,   42,   43,   44,  385,  385,  385,  385,  385,
 /*  1710 */   385,  385,  385,  385,  385,  254,  256,  329,  385,  385,
 /*  1720 */   385,  325,   29,   30,   31,   32,   33,   34,   35,   36,
 /*  1730 */    37,   38,   39,   40,   41,   42,   43,  385,  385,  385,
 /*  1740 */   385,  385,   44,  385,  385,  385,  385,  385,  254,  256,
 /*  1750 */   440,  441,  442,  443,  385,  336,  385,  385,  385,  325,
 /*  1760 */    29,   30,   31,   32,   33,   34,   35,   36,   37,   38,
 /*  1770 */    39,   40,   41,   42,   43,  385,  385,   44,  385,  385,
 /*  1780 */   495,  385,  385,  427,  385,  421,  254,  256,  408,  385,
 /*  1790 */    52,  385,  385,  385,  325,   29,   30,   31,   32,   33,
 /*  1800 */    34,   35,   36,   37,   38,   39,   40,   41,   42,   43,
 /*  1810 */    44,  385,  385,  385,  385,  385,  385,  385,  385,  385,
 /*  1820 */   385,  254,  256,  385,  385,  385,  385,   28,   29,   30,
 /*  1830 */    31,   32,   33,   34,   35,   36,   37,   38,   39,   40,
 /*  1840 */    41,   42,   43,  385,  385,   91,  385,  385,  104,  156,
 /*  1850 */   160,  154,  191,  101,  254,  256,  149,  385,  385,  385,
 /*  1860 */   385,  104,  156,  160,  154,  191,  101,  137,  385,  149,
 /*  1870 */    92,   93,   94,   95,   96,  223,  236,  385,  244,  245,
 /*  1880 */   137,  104,  156,  160,  154,  191,  101,  204,  385,  149,
 /*  1890 */   385,  385,  385,  385,  385,  169,  228,  385,  385,  385,
 /*  1900 */   137,  385,  162,  163,  385,  385,  385,  385,  169,  237,
 /*  1910 */   164,  165,  166,  167,  168,  162,  163,  385,  385,  385,
 /*  1920 */   385,  385,  385,  164,  165,  166,  167,  168,  169,  104,
 /*  1930 */   156,  160,  154,  191,  101,  162,  163,  149,  385,  385,
 /*  1940 */   385,  385,  385,  164,  165,  166,  167,  168,  137,  385,
 /*  1950 */   104,  156,  160,  154,  191,  101,  194,  385,  149,  385,
 /*  1960 */   385,  385,  385,  385,  385,  385,  385,  385,  338,  137,
 /*  1970 */    13,   16,  451,  457,  456,  444,  169,  439,  450,  414,
 /*  1980 */    19,  385,  385,  162,  163,  385,  385,  385,  385,  385,
 /*  1990 */   385,  164,  165,  166,  167,  168,  385,  169,  385,  385,
 /*  2000 */   385,  385,  385,  385,  162,  163,  385,  385,  385,  385,
 /*  2010 */   385,  385,  164,  165,  166,  167,  168,  104,  156,  160,
 /*  2020 */   154,  191,  101,  221,  385,  149,  385,  385,  385,  385,
 /*  2030 */   385,  385,  385,  385,  385,  385,  137,  104,  156,  160,
 /*  2040 */   154,  191,  101,  215,  385,  149,  385,  385,  385,  385,
 /*  2050 */   385,  179,  385,  385,  385,  385,  137,  104,  156,  160,
 /*  2060 */   154,  191,  101,  385,  169,  149,  385,  385,  385,  385,
 /*  2070 */   385,  162,  163,  385,  385,  385,  137,  385,  385,  164,
 /*  2080 */   165,  166,  167,  168,  169,  385,  385,  385,  385,  385,
 /*  2090 */   385,  162,  163,  385,  385,  385,  326,  180,  181,  164,
 /*  2100 */   165,  166,  167,  168,  169,  385,  182,  183,  185,  184,
 /*  2110 */   385,  162,  163,  385,  385,  385,  385,  385,  385,  164,
 /*  2120 */   165,  166,  167,  168,  104,  156,  160,  154,  191,  101,
 /*  2130 */   385,  385,  149,  385,  385,  385,  385,  474,   16,  451,
 /*  2140 */   457,  456,  444,  137,  439,  450,  414,   19,  385,  385,
 /*  2150 */   133,  135,  100,  115,  119,  121,  123,  125,  117,  103,
 /*  2160 */   127,  129,  131,  385,  385,  385,  385,  385,  385,  385,
 /*  2170 */   385,  169,  385,  385,  328,  385,  385,  385,  162,  163,
 /*  2180 */   385,  385,  385,  385,  385,  385,  164,  165,  166,  167,
 /*  2190 */   168,  385,  385,  133,  135,  100,  115,  119,  121,  123,
 /*  2200 */   125,  117,  103,  127,  129,  131,  100,  115,  119,  121,
 /*  2210 */   123,  125,  117,  103,  127,  129,  131,  335,  385,  385,
 /*  2220 */   385,  385,  385,  385,  133,  135,  100,  115,  119,  121,
 /*  2230 */   123,  125,  117,  103,  127,  129,  131,  468,   16,  451,
 /*  2240 */   457,  456,  444,  385,  439,  450,  414,   19,  340,  385,
 /*  2250 */   385,  385,  385,  385,  385,  133,  135,  100,  115,  119,
 /*  2260 */   121,  123,  125,  117,  103,  127,  129,  131,  385,  385,
 /*  2270 */   385,  385,  385,  385,  385,  385,  385,  385,  385,  710,
 /*  2280 */   385,  385,  385,  385,  385,  385,  133,  135,  100,  115,
 /*  2290 */   119,  121,  123,  125,  117,  103,  127,  129,  131,  385,
 /*  2300 */   385,  385,  461,   16,  451,  457,  456,  444,  260,  439,
 /*  2310 */   450,  414,   19,  385,  385,  133,  135,  100,  115,  119,
 /*  2320 */   121,  123,  125,  117,  103,  127,  129,  131,  385,  440,
 /*  2330 */   441,  442,  443,  400,  402,  396,  405,  385,  385,   82,
 /*  2340 */   385,  385,  385,  385,  385,  385,  133,  135,  100,  115,
 /*  2350 */   119,  121,  123,  125,  117,  103,  127,  129,  131,  470,
 /*  2360 */   385,  385,  427,  498,  421,  145,  415,  408,  421,  385,
 /*  2370 */   385,  408,  133,  135,  100,  115,  119,  121,  123,  125,
 /*  2380 */   117,  103,  127,  129,  131,  133,  135,  100,  115,  119,
 /*  2390 */   121,  123,  125,  117,  103,  127,  129,  131,  385,  385,
 /*  2400 */   187,  385,  385,  385,  376,  385,  385,  385,  385,  385,
 /*  2410 */   385,  385,  385,  385,  385,  385,  385,  385,  385,   23,
 /*  2420 */   385,  365,  385,  385,  385,  385,  385,  385,  385,  385,
 /*  2430 */   385,  385,  385,  615,  138,  139,  140,  141,  142,  143,
 /*  2440 */   170,  440,  441,  442,  443,  385,  189,  188,  440,  441,
 /*  2450 */   442,  443,  385,  385,  385,  182,  183,  185,  184,  440,
 /*  2460 */   441,  442,  443,  440,  441,  442,  443,  385,  385,  385,
 /*  2470 */   385,  494,  385,  385,  427,  385,  421,  385,   15,  408,
 /*  2480 */   385,  427,  385,  421,  385,  385,  408,  385,  385,  463,
 /*  2490 */   385,  385,  427,  476,  421,  385,  427,  408,  421,  385,
 /*  2500 */   385,  408,
};
static YYCODETYPE yy_lookahead[] = {
 /*     0 */    27,  100,   37,  102,   31,   10,  105,   34,   27,    1,
 /*    10 */     2,    3,    4,   40,   41,   42,   43,  130,   45,   46,
 /*    20 */    47,   48,   27,   50,  137,  138,  139,   54,   55,   56,
 /*    30 */   127,    5,   59,  130,  101,   40,   41,   42,   43,   44,
 /*    40 */    45,  138,  139,   70,   71,   72,  133,   39,   75,   76,
 /*    50 */    77,   78,   79,   80,   28,   27,    5,  130,   37,   31,
 /*    60 */   131,  132,   34,  141,  142,  138,  139,  140,   40,   41,
 /*    70 */    42,   43,  145,   45,   46,   47,   48,  144,   50,  146,
 /*    80 */   147,  148,   54,   55,   56,   37,   96,   59,   98,   99,
 /*    90 */   100,   31,  102,  103,  104,  105,    5,   23,   70,   71,
 /*   100 */    72,   27,   27,   75,   76,   77,   78,   79,   80,   11,
 /*   110 */    27,   51,   28,   53,   31,   37,   25,   34,   67,    1,
 /*   120 */     2,    3,    4,   40,   41,   42,   43,  130,   45,   46,
 /*   130 */    47,   48,  135,   50,   67,  138,  139,   54,   55,   56,
 /*   140 */   141,  142,   59,   97,   98,   99,  100,    5,  102,  103,
 /*   150 */   104,  105,   37,   70,   71,   72,  101,   39,   75,   76,
 /*   160 */    77,   78,   79,   80,  106,   27,  108,  109,  110,   31,
 /*   170 */    28,   35,   34,    5,    1,    2,    3,    4,   40,   41,
 /*   180 */    42,   43,   27,   45,   46,   47,   48,  106,   50,  108,
 /*   190 */   109,  110,   54,   55,   56,   67,   28,   59,   38,   98,
 /*   200 */    99,  100,   65,  102,   31,  104,  105,   34,   70,   71,
 /*   210 */    72,  101,   39,   75,   76,   77,   78,   79,   80,    2,
 /*   220 */    27,    4,   31,   86,   31,   65,   66,   34,    5,   35,
 /*   230 */    30,  101,   32,   40,   41,   42,   43,   27,   45,   46,
 /*   240 */    47,   48,   51,   50,   53,  101,   86,   54,   55,   56,
 /*   250 */   127,   28,   59,  130,   60,   61,   62,   63,   64,   35,
 /*   260 */    66,  138,  139,   70,   71,   72,  141,  142,   75,   76,
 /*   270 */    77,   78,   79,   80,   27,   27,  146,  147,  148,   31,
 /*   280 */     1,    2,   34,    4,    1,    2,    3,    4,   40,   41,
 /*   290 */    42,   43,  148,   45,   46,   47,   48,  130,   50,    3,
 /*   300 */    67,  134,   54,   55,   56,  138,  139,   59,   27,    1,
 /*   310 */     2,    3,    4,  133,   31,    5,   37,   34,   70,   71,
 /*   320 */    72,  101,   39,   75,   76,   77,   78,   79,   80,    3,
 /*   330 */    27,  105,  130,   37,   31,   39,  134,   34,   28,   31,
 /*   340 */   138,  139,   34,   40,   41,   42,   43,   39,   45,   46,
 /*   350 */    47,   48,    3,   50,   73,   74,   27,   54,   55,   56,
 /*   360 */    73,   74,   59,   37,    1,    2,    3,    4,   35,   82,
 /*   370 */    83,   84,   85,   70,   71,   72,  141,  142,   75,   76,
 /*   380 */    77,   78,   79,   80,   52,   27,   37,  130,   39,   31,
 /*   390 */    30,  134,   34,  101,   31,  138,  139,   34,   40,   41,
 /*   400 */    42,   43,   39,   45,   46,   47,   48,  130,   50,    5,
 /*   410 */    35,  134,   54,   55,   56,  138,  139,   59,  101,    1,
 /*   420 */     2,    3,    4,   28,    1,    2,    3,    4,   70,   71,
 /*   430 */    72,    5,   28,   75,   76,   77,   78,   79,   80,    5,
 /*   440 */    27,   16,   17,   18,   31,  141,  142,   34,    5,   31,
 /*   450 */    27,   25,   34,   40,   41,   42,   43,   39,   45,   46,
 /*   460 */    47,   48,   28,   50,  108,  109,  110,   54,   55,   56,
 /*   470 */   127,   28,   59,  130,   88,   89,   90,   91,   92,   93,
 /*   480 */    94,  138,  139,   70,   71,   72,  141,  142,   75,   76,
 /*   490 */    77,   78,   79,   80,    5,   27,  141,  142,    5,   31,
 /*   500 */   141,  142,   34,   67,    1,    2,    3,    4,   40,   41,
 /*   510 */    42,   43,   28,   45,   46,   47,   48,   28,   50,   35,
 /*   520 */   130,   28,   54,   55,   56,   30,   98,   59,  138,  139,
 /*   530 */   140,  103,  104,  105,   31,  145,   27,   34,   70,   71,
 /*   540 */    72,   35,   39,   75,   76,   77,   78,   79,   80,   37,
 /*   550 */    27,   39,  130,    5,   31,  141,  142,   34,   27,   35,
 /*   560 */   138,  139,  140,   40,   41,   42,   43,  145,   45,   46,
 /*   570 */    47,   48,  130,   50,  141,  142,   28,   54,   55,   56,
 /*   580 */   138,  139,   59,   49,   60,   61,   62,   63,   64,    1,
 /*   590 */     2,    3,    4,   70,   71,   72,  104,  105,   75,   76,
 /*   600 */    77,   78,   79,   80,   37,   27,   39,    5,  130,   31,
 /*   610 */   128,  129,   34,  135,  133,   27,  138,  139,   40,   41,
 /*   620 */    42,   43,   28,   45,   46,   47,   48,   25,   50,   35,
 /*   630 */   130,    5,   54,   55,   56,  130,    0,   59,  138,  139,
 /*   640 */   140,  136,  137,  138,  139,  145,  101,    5,   70,   71,
 /*   650 */    72,   25,   28,   75,   76,   77,   78,   79,   80,    5,
 /*   660 */    27,   52,   26,  130,   31,   29,    5,   34,   67,   33,
 /*   670 */    34,  138,  139,   40,   41,   42,   43,   27,   45,   46,
 /*   680 */    47,   48,   28,   50,   38,  130,   25,   54,   55,   56,
 /*   690 */    65,   66,   59,  138,  139,  140,   27,   67,  130,   57,
 /*   700 */   145,  129,  134,   70,   71,   72,  138,  139,   75,   76,
 /*   710 */    77,   78,   79,   80,   30,   27,   32,  130,   28,   31,
 /*   720 */    57,    5,   34,   73,   74,  138,  139,   52,   40,   41,
 /*   730 */    42,   43,  145,   45,   46,   47,   48,    5,   50,  102,
 /*   740 */    30,   25,   54,   55,   56,   27,  130,   59,   91,   92,
 /*   750 */    93,   94,  109,   52,  138,  139,   52,   25,   70,   71,
 /*   760 */    72,  101,   28,   75,   76,   77,   78,   79,   80,   67,
 /*   770 */    27,  130,  133,  130,   31,  134,   38,   34,   30,  138,
 /*   780 */   139,  138,  139,   40,   41,   42,   43,   66,   45,   46,
 /*   790 */    47,   48,   27,   50,   67,   27,  130,   54,   55,   56,
 /*   800 */    26,  130,   59,   29,  138,  139,  133,   33,   34,  138,
 /*   810 */   139,  143,   27,   70,   71,   72,   27,   27,   75,   76,
 /*   820 */    77,   78,   79,   80,   27,   27,  130,   67,   37,   31,
 /*   830 */    30,   27,   34,   28,  138,  139,   37,  101,   40,   41,
 /*   840 */    42,   43,  130,   45,   46,   47,   48,   38,   50,   31,
 /*   850 */   138,  139,   54,   55,   56,   52,  130,   59,   73,   74,
 /*   860 */    28,   27,   37,  130,  138,  139,    0,   27,   70,   71,
 /*   870 */    72,  138,  139,   75,   76,   77,   78,   79,   80,   30,
 /*   880 */    27,  130,   31,  130,   31,   49,   28,   34,  101,  138,
 /*   890 */   139,  138,  139,   40,   41,   42,   43,  130,   45,   46,
 /*   900 */    47,   48,  130,   50,   35,  138,  139,   54,   55,   56,
 /*   910 */   138,  139,   59,  130,   30,  142,   27,   30,   27,  133,
 /*   920 */    28,  138,  139,   70,   71,   72,   30,   28,   75,   76,
 /*   930 */    77,   78,   79,   80,  132,   27,  130,   31,  130,   31,
 /*   940 */    38,   28,   34,   27,  138,  139,  138,  139,   40,   41,
 /*   950 */    42,   43,  130,   45,   46,   47,   48,  130,   50,   25,
 /*   960 */   138,  139,   54,   55,   56,  138,  139,   59,   28,   27,
 /*   970 */    29,   27,   27,  130,   28,   28,   67,   28,   70,   71,
 /*   980 */    72,  138,  139,   75,   76,   77,   78,   79,   80,  143,
 /*   990 */    27,   31,   67,   37,   31,   30,   38,   34,   37,   30,
 /*  1000 */    27,   37,    5,   40,   41,   42,   43,  130,   45,   46,
 /*  1010 */    47,   48,  130,   50,   30,  138,  139,   54,   55,   56,
 /*  1020 */   138,  139,   59,  130,   37,   27,   38,   27,   37,   37,
 /*  1030 */   149,  138,  139,   70,   71,   72,  149,  149,   75,   76,
 /*  1040 */    77,   78,   79,   80,  149,   27,  149,  149,  149,   31,
 /*  1050 */   149,  149,   34,  149,  149,  149,  149,  149,   40,   41,
 /*  1060 */    42,   43,  149,   45,   46,   47,   48,  149,   50,  149,
 /*  1070 */   149,  149,   54,   55,   56,  149,  149,   59,  149,  149,
 /*  1080 */   149,  149,  149,  149,  149,  149,  149,  149,   70,   71,
 /*  1090 */    72,  149,  149,   75,   76,   77,   78,   79,   80,  149,
 /*  1100 */    27,  149,  149,  149,   31,  149,  149,   34,  149,  149,
 /*  1110 */   149,  149,  149,   40,   41,   42,   43,  149,   45,   46,
 /*  1120 */    47,   48,  149,   50,  149,  149,  149,   54,   55,   56,
 /*  1130 */   149,  149,   59,  149,  149,  149,  149,  149,  149,  149,
 /*  1140 */   149,  149,  149,   70,   71,   72,  149,  149,   75,   76,
 /*  1150 */    77,   78,   79,   80,  149,   27,  149,  149,  149,   31,
 /*  1160 */   149,  149,   34,  149,  149,  149,  149,  149,   40,   41,
 /*  1170 */    42,   43,  149,   45,   46,   47,   48,  149,   50,  149,
 /*  1180 */   149,   27,   54,   55,   56,  149,  149,   59,  149,  149,
 /*  1190 */   149,  149,  149,  149,  149,  149,  149,  149,   70,   71,
 /*  1200 */    72,  149,   27,   75,   76,   77,   78,   79,   80,   34,
 /*  1210 */   149,  149,  149,  149,  149,   40,   41,   42,   43,  149,
 /*  1220 */    45,   46,   47,   48,  149,   50,   10,   73,   74,   54,
 /*  1230 */    55,   56,  149,  149,   59,  149,   82,   83,   84,   85,
 /*  1240 */   149,   25,  149,   27,  149,   70,   71,   72,  149,  149,
 /*  1250 */    75,   76,   77,   78,   79,   80,   40,   41,   42,   43,
 /*  1260 */    44,   45,  149,  149,  149,   19,   20,   21,   22,   23,
 /*  1270 */    24,  149,  149,   27,  149,  149,  149,  149,  149,   94,
 /*  1280 */   149,  149,  149,  149,   38,  149,   40,   41,   42,   43,
 /*  1290 */    44,   45,  107,  149,  149,  149,  111,  112,  113,  114,
 /*  1300 */   115,  116,  117,  118,  119,  120,  121,  122,  123,  124,
 /*  1310 */   125,  126,   66,  149,  149,  149,  149,  149,  149,   73,
 /*  1320 */    74,  149,  149,  138,  139,   94,  149,   81,   82,   83,
 /*  1330 */    84,   85,  149,  149,  149,  149,  149,  149,  107,  149,
 /*  1340 */   149,  149,  111,  112,  113,  114,  115,  116,  117,  118,
 /*  1350 */   119,  120,  121,  122,  123,  124,  125,  126,  149,  149,
 /*  1360 */    94,  149,    1,    2,    3,    4,  149,  149,  149,  138,
 /*  1370 */   139,  149,  149,  107,  149,  149,  149,  111,  112,  113,
 /*  1380 */   114,  115,  116,  117,  118,  119,  120,  121,  122,  123,
 /*  1390 */   124,  125,  126,   94,  149,  149,  149,   36,  149,  149,
 /*  1400 */    39,  149,  149,  149,  138,  139,  107,  149,  149,  149,
 /*  1410 */   111,  112,  113,  114,  115,  116,  117,  118,  119,  120,
 /*  1420 */   121,  122,  123,  124,  125,  126,   94,    1,    2,    3,
 /*  1430 */     4,  149,  149,  149,  149,  149,  149,  138,  139,  107,
 /*  1440 */   149,  149,  149,  111,  112,  113,  114,  115,  116,  117,
 /*  1450 */   118,  119,  120,  121,  122,  123,  124,  125,  126,  149,
 /*  1460 */   149,  149,   36,  149,   94,   39,  149,  149,  149,  149,
 /*  1470 */   138,  139,  149,  149,  149,  149,  149,  107,  149,  149,
 /*  1480 */   149,  111,  112,  113,  114,  115,  116,  117,  118,  119,
 /*  1490 */   120,  121,  122,  123,  124,  125,  126,  149,  149,   94,
 /*  1500 */   149,  149,  149,  149,  149,  149,  149,  149,  138,  139,
 /*  1510 */   149,  149,  107,  149,  149,  149,  111,  112,  113,  114,
 /*  1520 */   115,  116,  117,  118,  119,  120,  121,  122,  123,  124,
 /*  1530 */   125,  126,   94,  149,  149,  149,  149,  149,  149,  149,
 /*  1540 */   149,  149,  149,  138,  139,  107,  149,  149,  149,  111,
 /*  1550 */   112,  113,  114,  115,  116,  117,  118,  119,  120,  121,
 /*  1560 */   122,  123,  124,  125,  126,   94,  149,  149,  149,  149,
 /*  1570 */   149,  149,  149,  149,  149,  149,  138,  139,  107,  149,
 /*  1580 */   149,  149,  111,  112,  113,  114,  115,  116,  117,  118,
 /*  1590 */   119,  120,  121,  122,  123,  124,  125,  126,  149,  149,
 /*  1600 */   149,  149,  149,   94,  149,  149,  149,  149,  149,  138,
 /*  1610 */   139,  149,  149,  149,  149,  149,  107,  149,  149,  149,
 /*  1620 */   111,  112,  113,  114,  115,  116,  117,  118,  119,  120,
 /*  1630 */   121,  122,  123,  124,  125,  126,  149,  149,   94,  149,
 /*  1640 */   149,  149,  149,  149,  149,  149,  149,  138,  139,  149,
 /*  1650 */   149,  107,  149,  149,  149,  111,  112,  113,  114,  115,
 /*  1660 */   116,  117,  118,  119,  120,  121,  122,  123,  124,  125,
 /*  1670 */   126,   94,  149,  149,  149,  149,  149,  149,  149,  149,
 /*  1680 */   149,  149,  138,  139,  107,  149,  149,  149,  111,  112,
 /*  1690 */   113,  114,  115,  116,  117,  118,  119,  120,  121,  122,
 /*  1700 */   123,  124,  125,  126,   94,  149,  149,  149,  149,  149,
 /*  1710 */   149,  149,  149,  149,  149,  138,  139,  107,  149,  149,
 /*  1720 */   149,  111,  112,  113,  114,  115,  116,  117,  118,  119,
 /*  1730 */   120,  121,  122,  123,  124,  125,  126,  149,  149,  149,
 /*  1740 */   149,  149,   94,  149,  149,  149,  149,  149,  138,  139,
 /*  1750 */     1,    2,    3,    4,  149,  107,  149,  149,  149,  111,
 /*  1760 */   112,  113,  114,  115,  116,  117,  118,  119,  120,  121,
 /*  1770 */   122,  123,  124,  125,  126,  149,  149,   94,  149,  149,
 /*  1780 */    31,  149,  149,   34,  149,   36,  138,  139,   39,  149,
 /*  1790 */   107,  149,  149,  149,  111,  112,  113,  114,  115,  116,
 /*  1800 */   117,  118,  119,  120,  121,  122,  123,  124,  125,  126,
 /*  1810 */    94,  149,  149,  149,  149,  149,  149,  149,  149,  149,
 /*  1820 */   149,  138,  139,  149,  149,  149,  149,  111,  112,  113,
 /*  1830 */   114,  115,  116,  117,  118,  119,  120,  121,  122,  123,
 /*  1840 */   124,  125,  126,  149,  149,   35,  149,  149,   19,   20,
 /*  1850 */    21,   22,   23,   24,  138,  139,   27,  149,  149,  149,
 /*  1860 */   149,   19,   20,   21,   22,   23,   24,   38,  149,   27,
 /*  1870 */    60,   61,   62,   63,   64,   65,   66,  149,   68,   69,
 /*  1880 */    38,   19,   20,   21,   22,   23,   24,   25,  149,   27,
 /*  1890 */   149,  149,  149,  149,  149,   66,   67,  149,  149,  149,
 /*  1900 */    38,  149,   73,   74,  149,  149,  149,  149,   66,   67,
 /*  1910 */    81,   82,   83,   84,   85,   73,   74,  149,  149,  149,
 /*  1920 */   149,  149,  149,   81,   82,   83,   84,   85,   66,   19,
 /*  1930 */    20,   21,   22,   23,   24,   73,   74,   27,  149,  149,
 /*  1940 */   149,  149,  149,   81,   82,   83,   84,   85,   38,  149,
 /*  1950 */    19,   20,   21,   22,   23,   24,   25,  149,   27,  149,
 /*  1960 */   149,  149,  149,  149,  149,  149,  149,  149,   58,   38,
 /*  1970 */    95,   96,   97,   98,   99,  100,   66,  102,  103,  104,
 /*  1980 */   105,  149,  149,   73,   74,  149,  149,  149,  149,  149,
 /*  1990 */   149,   81,   82,   83,   84,   85,  149,   66,  149,  149,
 /*  2000 */   149,  149,  149,  149,   73,   74,  149,  149,  149,  149,
 /*  2010 */   149,  149,   81,   82,   83,   84,   85,   19,   20,   21,
 /*  2020 */    22,   23,   24,   25,  149,   27,  149,  149,  149,  149,
 /*  2030 */   149,  149,  149,  149,  149,  149,   38,   19,   20,   21,
 /*  2040 */    22,   23,   24,   25,  149,   27,  149,  149,  149,  149,
 /*  2050 */   149,   27,  149,  149,  149,  149,   38,   19,   20,   21,
 /*  2060 */    22,   23,   24,  149,   66,   27,  149,  149,  149,  149,
 /*  2070 */   149,   73,   74,  149,  149,  149,   38,  149,  149,   81,
 /*  2080 */    82,   83,   84,   85,   66,  149,  149,  149,  149,  149,
 /*  2090 */   149,   73,   74,  149,  149,  149,   58,   73,   74,   81,
 /*  2100 */    82,   83,   84,   85,   66,  149,   82,   83,   84,   85,
 /*  2110 */   149,   73,   74,  149,  149,  149,  149,  149,  149,   81,
 /*  2120 */    82,   83,   84,   85,   19,   20,   21,   22,   23,   24,
 /*  2130 */   149,  149,   27,  149,  149,  149,  149,   95,   96,   97,
 /*  2140 */    98,   99,  100,   38,  102,  103,  104,  105,  149,  149,
 /*  2150 */     6,    7,    8,    9,   10,   11,   12,   13,   14,   15,
 /*  2160 */    16,   17,   18,  149,  149,  149,  149,  149,  149,  149,
 /*  2170 */   149,   66,  149,  149,   30,  149,  149,  149,   73,   74,
 /*  2180 */   149,  149,  149,  149,  149,  149,   81,   82,   83,   84,
 /*  2190 */    85,  149,  149,    6,    7,    8,    9,   10,   11,   12,
 /*  2200 */    13,   14,   15,   16,   17,   18,    8,    9,   10,   11,
 /*  2210 */    12,   13,   14,   15,   16,   17,   18,   30,  149,  149,
 /*  2220 */   149,  149,  149,  149,    6,    7,    8,    9,   10,   11,
 /*  2230 */    12,   13,   14,   15,   16,   17,   18,   95,   96,   97,
 /*  2240 */    98,   99,  100,  149,  102,  103,  104,  105,   30,  149,
 /*  2250 */   149,  149,  149,  149,  149,    6,    7,    8,    9,   10,
 /*  2260 */    11,   12,   13,   14,   15,   16,   17,   18,  149,  149,
 /*  2270 */   149,  149,  149,  149,  149,  149,  149,  149,  149,   30,
 /*  2280 */   149,  149,  149,  149,  149,  149,    6,    7,    8,    9,
 /*  2290 */    10,   11,   12,   13,   14,   15,   16,   17,   18,  149,
 /*  2300 */   149,  149,   95,   96,   97,   98,   99,  100,   28,  102,
 /*  2310 */   103,  104,  105,  149,  149,    6,    7,    8,    9,   10,
 /*  2320 */    11,   12,   13,   14,   15,   16,   17,   18,  149,    1,
 /*  2330 */     2,    3,    4,    1,    2,    3,    4,  149,  149,   30,
 /*  2340 */   149,  149,  149,  149,  149,  149,    6,    7,    8,    9,
 /*  2350 */    10,   11,   12,   13,   14,   15,   16,   17,   18,   31,
 /*  2360 */   149,  149,   34,   31,   36,   25,   34,   39,   36,  149,
 /*  2370 */   149,   39,    6,    7,    8,    9,   10,   11,   12,   13,
 /*  2380 */    14,   15,   16,   17,   18,    6,    7,    8,    9,   10,
 /*  2390 */    11,   12,   13,   14,   15,   16,   17,   18,  149,  149,
 /*  2400 */    27,  149,  149,  149,   10,  149,  149,  149,  149,  149,
 /*  2410 */   149,  149,  149,  149,  149,  149,  149,  149,  149,   25,
 /*  2420 */   149,   27,  149,  149,  149,  149,  149,  149,  149,  149,
 /*  2430 */   149,  149,  149,   67,   40,   41,   42,   43,   44,   45,
 /*  2440 */    67,    1,    2,    3,    4,  149,   73,   74,    1,    2,
 /*  2450 */     3,    4,  149,  149,  149,   82,   83,   84,   85,    1,
 /*  2460 */     2,    3,    4,    1,    2,    3,    4,  149,  149,  149,
 /*  2470 */   149,   31,  149,  149,   34,  149,   36,  149,   31,   39,
 /*  2480 */   149,   34,  149,   36,  149,  149,   39,  149,  149,   31,
 /*  2490 */   149,  149,   34,   31,   36,  149,   34,   39,   36,  149,
 /*  2500 */   149,   39,
};
#define YY_SHIFT_USE_DFLT (-36)
static short yy_shift_ofst[] = {
 /*     0 */   774,  866,  636,  -36,  -36,  -36,  -36,  768,  805,  -36,
 /*    10 */   531,  200, 2447,  851,  -36,  -36, 2440,  283,  -36,  825,
 /*    20 */   916,  809, 2394,  884,  633,  -36,  853,  -36,  -36,  -36,
 /*    30 */   -36,  -36,  -36,  -36,  -36,  -36,  -36,  -36,  -36,  -36,
 /*    40 */   -36,  -36,  -36,  -36,  -36,  892,  -36,  690,  -36, 2105,
 /*    50 */   887,  468,  413,  836,  849,  743,   83,  -36, 2105,  748,
 /*    60 */   191,  -36,   60,  -36,  -36,  287,  332, 1175, 1175,  969,
 /*    70 */  1128,  578,  -36, 2105,  800,   28,  248,  -36,  840,  642,
 /*    80 */  2038, 2309, 1175, 1018,  -36,  942,  168,  -36,  942,  -36,
 /*    90 */  1810,  -36,  -36,  -36,  -36,  -36,  -36, 2105,  -36, 2379,
 /*   100 */  2105, 2105,  -36, 2105,  789,  -36,  721,  281,  702,  -36,
 /*   110 */   630,  -36,  601,  -36,  425, 2105,  425, 2105,  425, 2105,
 /*   120 */   425, 2105,  425, 2105,  425, 2105,  425, 2105,  -36, 2105,
 /*   130 */   -36, 2105,  -36, 2105, 2198, 2105, 2198, 1246,  -36,  -36,
 /*   140 */   -36,  -36,  -36,  -36, 2340,  -36,  934, 2105,  -36,  160,
 /*   150 */   834,  738, 1862, 2379, 2105,  -36,  804,  997,  789,  -36,
 /*   160 */  2105,  -36,  -36,  -36,  -36,  -36,  -36,  -36,  -36, 2373,
 /*   170 */   -36,   51,  -36, 1154,  -36,  803, 2024,  -36,  -36,  -36,
 /*   180 */   -36,  -36,  -36,  -36,  -36,  -36,  -36,  704,  701,  675,
 /*   190 */   -36,  669,  646, 1931,  -36,  426,  -36, 2105,  -36,  -36,
 /*   200 */   -36,  -36,  626,  -36,  -36,  785,  233,  -36,  128,  -36,
 /*   210 */    67,  -36, 2018,  661,  -36,  -36,  973,  958, 1998,  732,
 /*   220 */   -36,  -36,  425, 1000,  194, 2105,  -36, 1829,  524, 2105,
 /*   230 */   -36,  436,  524, 2105,  -36, 2366, 1842,  524, 2105,  -36,
 /*   240 */   727,  524, 2105,  -36,  -36,  -36,  -36, 2105,  654,  -36,
 /*   250 */  2105,  -36, 2379,  -36,  940,  -36,  947,  -36, 2105, 2280,
 /*   260 */   -36,  889,  625,  797,  913,  -36,  650,  909,  858,  -36,
 /*   270 */   760,   84,  -36,  925,  949,  -36,   74,  946,  -36,  944,
 /*   280 */   902, 2105,   91,  899,  -36,  790,  223,  -36,  790,  -36,
 /*   290 */   869,  287,  -36,  -36,  790,  142,  -36,  790,  310,  -36,
 /*   300 */   790,  404,  -36,  790,  434,  -36,  790,  443,  -36,  790,
 /*   310 */   489,  -36,  790,  493,  -36,  790,   26,  -36,  790,  548,
 /*   320 */   -36,  137,  765,  738,  -36,  -36, 2105, 2144, 1175,  963,
 /*   330 */   -36,  718,  663, 1910, 2187, 1175,  -27,  -36, 2105, 2218,
 /*   340 */  1175,  798,  -36,  -36, 2249,  -36,  609, 1175, 1175,  -36,
 /*   350 */   -36,  534,  495,  193, 1073,  -36,  -36,  602,  360,  303,
 /*   360 */   -36,  523,  -36,   -5,  -36,  333,  287,  -36,  247,  224,
 /*   370 */   287,  -36,  155,  136,  287,  -36,   75,   98,  -19,  -36,
 /*   380 */   118,  992,  998,  988, 1216,  984,  688,  -36,  908,  -36,
 /*   390 */   716,  965,  138,  -36,  358,  -36,  279,  956,  961,  964,
 /*   400 */   326,  987,  296,  991,  -35,  349,   21,   48,  217,   78,
 /*   410 */   115, 2332,  503,  -36,  -36, 1426,  329,  375,  287,  395,
 /*   420 */   -36,  509,  506,  287,  624,  -36,  -36, 1361,  423,  484,
 /*   430 */   -36,  287,  734,  -36,  -36,  -36,  -36,  -36,  -36,  -36,
 /*   440 */   791,  512,  799,  567,  588,  594,  -36,  287,  832,  -36,
 /*   450 */   -36, 1749,  173,  418,    8,  363,  -36,  308,  891,  896,
 /*   460 */  2458,  906,  -36,  -36,  941,  945,  684, 2328,  960,  -36,
 /*   470 */   -36,  210,  710, 2462,  818,  -36,  -36,  -36,
};
#define YY_REDUCE_USE_DFLT (-114)
static short yy_reduce_ofst[] = {
 /*     0 */   386, -114,  657, -114, -114, -114, -114, -114, -114, -114,
 /*    10 */  -114, -114, 1875, -114, -114, -114,   46,  492, -114, -114,
 /*    20 */  -114, -114,   81, -114, 1185, -114, 1716, -114, -114, -114,
 /*    30 */  -114, -114, -114, -114, -114, -114, -114, -114, -114, -114,
 /*    40 */  -114, -114, -114, -114, -114, -114, -114, -114, -114,  -97,
 /*    50 */  -114, 1683, 1716, -114, -114, 1438, 1716, -114,  343, -114,
 /*    60 */   482, -114,  572, -114, -114,  317, -114, 1405, 1716, -114,
 /*    70 */  1266, 1716, -114,  123, -114, 1299, 1716, -114, -114, -114,
 /*    80 */   442, -114, 1544, 1716, -114,  -71, -114, -114,  802, -114,
 /*    90 */   786, -114, -114, -114, -114, -114, -114,  568, -114, -114,
 /*   100 */   751,  733, -114,  726,  668, -114, -114, -114, -114, -114,
 /*   110 */  -114, -114, -114, -114, -114,  767, -114,  783, -114,  806,
 /*   120 */  -114,  822, -114,  877, -114,  893, -114,  843, -114,  808,
 /*   130 */  -114,  827, -114,  882, -114,  696, -114,  643, -114, -114,
 /*   140 */  -114, -114, -114, -114, -114, -114, -114,  712, -114, -114,
 /*   150 */  -114, -114,  422, -114,  772, -114, -114, -114,  846, -114,
 /*   160 */   616, -114, -114, -114, -114, -114, -114, -114, -114,  -67,
 /*   170 */  -114, -114, -114,  130, -114, -114,  144, -114, -114, -114,
 /*   180 */  -114, -114, -114, -114, -114, -114, -114, -114, -114, -114,
 /*   190 */  -114, -114, -114,  500, -114, -114, -114,  587, -114, -114,
 /*   200 */  -114, -114, -114, -114, -114, -114, -114, -114, -114, -114,
 /*   210 */  -114, -114,  555, -114, -114, -114, -114, -114,  390, -114,
 /*   220 */  -114, -114, -114, -114,  -87,  167, -114,  478,  180,  202,
 /*   230 */  -114, -114,  481,  277, -114, -114,   -3,  639,  641, -114,
 /*   240 */  -114,  673,  257, -114, -114, -114, -114,  505, -114, -114,
 /*   250 */  -113, -114, -114, -114, -114, -114, -114, -114,  753, -114,
 /*   260 */  -114, -114, -114, -114, -114, -114, -114, -114, -114, -114,
 /*   270 */  -114, -114, -114, -114, -114, -114, -114, -114, -114, -114,
 /*   280 */  -114,  -73, -114, -114, -114,  -78, -114, -114,  773, -114,
 /*   290 */  -114,  787, -114, -114,   -1, -114, -114,  125, -114, -114,
 /*   300 */   235, -114, -114,  304, -114, -114,  345, -114, -114,  355,
 /*   310 */  -114, -114,  359, -114, -114,  414, -114, -114,  433, -114,
 /*   320 */  -114, -114, -114, -114, -114, -114,  666, -114, 1610, 1716,
 /*   330 */  -114, -114, -114,  533, -114, 1648, 1716, -114,  671, -114,
 /*   340 */  1577, 1716, -114, -114, -114, -114, -114, 1370, 1716, -114,
 /*   350 */  -114, -114, -114, 1332, 1716, -114, -114, -114, -114, 1509,
 /*   360 */  -114, 1716, -114,  356, -114, -114,  220, -114, -114, -114,
 /*   370 */   110, -114, -114, -114,   55, -114, -114, -114, -114, -114,
 /*   380 */   226, -114, -114, -114,   58, -114, 1471, -114, 1716, -114,
 /*   390 */  -114, -114, 1231, -114, 1716, -114, -114, -114, -114, -114,
 /*   400 */  -114, -114, -114, -114, -114, -114, -114, -114, -114, -114,
 /*   410 */  -114,  428,  492, -114, -114,  226, -114, -114,  292, -114,
 /*   420 */  -114, -114, -114,  545, -114, -114, -114,  -99,  637, -114,
 /*   430 */  -114,  660, -114, -114, -114, -114, -114, -114, -114, -114,
 /*   440 */  -114, -114, -114, -114,  637, -114, -114,  736, -114, -114,
 /*   450 */  -114,  -10,  101,  492,  -99,  492, -114,  492, -114, -114,
 /*   460 */  2207, -114, -114, -114, -114, -114, -114, 2142, -114, -114,
 /*   470 */  -114, -114, -114, 2042, -114, -114, -114, -114,
};
static YYACTIONTYPE yy_default[] = {
 /*     0 */   712,  712,  712,  480,  482,  483,  484,  712,  712,  485,
 /*    10 */   712,  712,  712,  712,  486,  490,  712,  712,  517,  712,
 /*    20 */   712,  712,  712,  712,  712,  521,  712,  523,  558,  560,
 /*    30 */   561,  562,  563,  564,  565,  566,  567,  568,  569,  570,
 /*    40 */   571,  572,  573,  574,  575,  712,  576,  712,  577,  712,
 /*    50 */   712,  712,  712,  580,  712,  712,  712,  581,  712,  712,
 /*    60 */   712,  584,  712,  585,  586,  712,  712,  712,  588,  712,
 /*    70 */   712,  712,  591,  712,  712,  712,  712,  593,  712,  712,
 /*    80 */   712,  712,  712,  712,  594,  712,  712,  598,  712,  599,
 /*    90 */   712,  601,  602,  603,  604,  605,  606,  712,  607,  643,
 /*   100 */   712,  712,  644,  712,  712,  645,  712,  712,  712,  707,
 /*   110 */   712,  708,  712,  709,  647,  712,  648,  712,  649,  712,
 /*   120 */   650,  712,  651,  712,  652,  712,  653,  712,  660,  712,
 /*   130 */   661,  712,  662,  712,  663,  712,  664,  712,  552,  553,
 /*   140 */   554,  555,  556,  557,  712,  654,  712,  712,  655,  668,
 /*   150 */   712,  656,  712,  691,  712,  665,  712,  712,  712,  666,
 /*   160 */   712,  667,  669,  670,  671,  672,  673,  674,  675,  712,
 /*   170 */   676,  712,  677,  712,  692,  712,  712,  694,  699,  700,
 /*   180 */   701,  702,  703,  704,  705,  706,  695,  700,  702,  701,
 /*   190 */   693,  712,  678,  712,  679,  712,  680,  712,  683,  684,
 /*   200 */   689,  690,  712,  687,  688,  712,  712,  657,  712,  658,
 /*   210 */   712,  659,  712,  712,  681,  682,  712,  712,  712,  712,
 /*   220 */   685,  686,  646,  712,  712,  712,  608,  712,  712,  712,
 /*   230 */   609,  712,  712,  712,  610,  712,  712,  712,  712,  611,
 /*   240 */   712,  712,  712,  612,  613,  614,  600,  712,  712,  616,
 /*   250 */   712,  617,  619,  618,  712,  620,  712,  621,  712,  712,
 /*   260 */   622,  712,  712,  712,  712,  623,  712,  712,  712,  624,
 /*   270 */   712,  712,  625,  712,  712,  626,  712,  712,  627,  712,
 /*   280 */   712,  712,  712,  712,  628,  712,  712,  629,  712,  639,
 /*   290 */   641,  712,  642,  640,  712,  712,  630,  712,  712,  631,
 /*   300 */   712,  712,  632,  712,  712,  633,  712,  712,  634,  712,
 /*   310 */   712,  635,  712,  712,  636,  712,  712,  637,  712,  712,
 /*   320 */   638,  712,  712,  712,  711,  559,  712,  712,  712,  712,
 /*   330 */   595,  712,  712,  712,  712,  712,  712,  596,  712,  712,
 /*   340 */   712,  712,  597,  592,  712,  590,  712,  712,  589,  587,
 /*   350 */   582,  578,  712,  712,  712,  583,  579,  712,  712,  712,
 /*   360 */   522,  712,  524,  712,  543,  545,  712,  548,  712,  546,
 /*   370 */   712,  549,  712,  547,  712,  550,  712,  712,  712,  544,
 /*   380 */   712,  712,  712,  712,  712,  712,  712,  525,  712,  527,
 /*   390 */   712,  712,  712,  526,  712,  528,  712,  712,  712,  712,
 /*   400 */   712,  712,  712,  712,  712,  712,  712,  712,  712,  712,
 /*   410 */   712,  712,  712,  515,  518,  712,  712,  712,  712,  712,
 /*   420 */   519,  712,  712,  712,  712,  520,  503,  712,  712,  712,
 /*   430 */   505,  712,  712,  507,  509,  511,  512,  513,  514,  510,
 /*   440 */   511,  512,  513,  514,  712,  712,  506,  712,  712,  508,
 /*   450 */   516,  712,  712,  712,  712,  712,  504,  712,  712,  712,
 /*   460 */   712,  712,  487,  491,  712,  712,  712,  712,  712,  488,
 /*   470 */   492,  712,  712,  712,  712,  489,  493,  481,
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
  "TYPE_INTEGER",  "TYPE_DOUBLE",   "TYPE_BOOL",     "TYPE_STRING", 
  "TYPE_ARRAY",    "TYPE_VAR",      "BREAK",         "CONTINUE",    
  "IF",            "ELSE",          "SWITCH",        "CASE",        
  "COLON",         "DEFAULT",       "LOOP",          "WHILE",       
  "FOR",           "IN",            "REVERSE",       "LET",         
  "ADDASSIGN",     "SUBASSIGN",     "MULASSIGN",     "DIVASSIGN",   
  "CONCATASSIGN",  "ARROW",         "SBRACKET_OPEN",  "SBRACKET_CLOSE",
  "INCR",          "DECR",          "ECHO",          "RETURN",      
  "UNSET",         "INTEGER",       "STRING",        "THROW",       
  "TYPE_UINTEGER",  "TYPE_CHAR",     "TYPE_UCHAR",    "TYPE_LONG",   
  "TYPE_ULONG",    "SCHAR",         "DOUBLE",        "NULL",        
  "TRUE",          "FALSE",         "DOUBLECOLON",   "error",       
  "program",       "xx_language",   "xx_top_statement_list",  "xx_top_statement",
  "xx_namespace_def",  "xx_class_def",  "xx_comment",    "xx_class_definition",
  "xx_class_properties_definition",  "xx_class_consts_definition",  "xx_class_methods_definition",  "xx_class_property_definition",
  "xx_property_visibility_list",  "xx_literal_expr",  "xx_property_visibility",  "xx_class_const_definition",
  "xx_class_method_definition",  "xx_method_visibility_list",  "xx_parameter_list",  "xx_statement_list",
  "xx_parameter",  "xx_parameter_type",  "xx_parameter_cast",  "xx_statement",
  "xx_let_statement",  "xx_if_statement",  "xx_loop_statement",  "xx_echo_statement",
  "xx_return_statement",  "xx_mcall_statement",  "xx_scall_statement",  "xx_unset_statement",
  "xx_throw_statement",  "xx_declare_statement",  "xx_break_statement",  "xx_continue_statement",
  "xx_while_statement",  "xx_switch_statement",  "xx_for_statement",  "xx_eval_expr",
  "xx_case_clauses",  "xx_case_clause",  "xx_common_expr",  "xx_let_assignments",
  "xx_let_assignment",  "xx_assignment_operator",  "xx_assign_expr",  "xx_index_expr",
  "xx_echo_expressions",  "xx_echo_expression",  "xx_mcall_expr",  "xx_scall_expr",
  "xx_call_parameters",  "xx_declare_variable_list",  "xx_declare_variable",  "xx_isset_expr",
  "xx_array_list",  "xx_call_parameter",  "xx_array_item",  "xx_array_key",
  "xx_array_value",
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
 /*  51 */ "xx_method_visibility_list ::= STATIC PUBLIC",
 /*  52 */ "xx_method_visibility_list ::= PUBLIC STATIC",
 /*  53 */ "xx_method_visibility_list ::= STATIC PROTECTED",
 /*  54 */ "xx_method_visibility_list ::= PROTECTED STATIC",
 /*  55 */ "xx_method_visibility_list ::= STATIC PRIVATE",
 /*  56 */ "xx_method_visibility_list ::= PRIVATE STATIC",
 /*  57 */ "xx_method_visibility_list ::= INLINE PROTECTED",
 /*  58 */ "xx_method_visibility_list ::= PROTECTED INLINE",
 /*  59 */ "xx_method_visibility_list ::= INLINE PRIVATE",
 /*  60 */ "xx_method_visibility_list ::= PRIVATE INLINE",
 /*  61 */ "xx_method_visibility_list ::= PUBLIC",
 /*  62 */ "xx_method_visibility_list ::= PROTECTED",
 /*  63 */ "xx_method_visibility_list ::= STATIC",
 /*  64 */ "xx_method_visibility_list ::= PRIVATE",
 /*  65 */ "xx_parameter_list ::= xx_parameter_list COMMA xx_parameter",
 /*  66 */ "xx_parameter_list ::= xx_parameter",
 /*  67 */ "xx_parameter ::= IDENTIFIER",
 /*  68 */ "xx_parameter ::= xx_parameter_type IDENTIFIER",
 /*  69 */ "xx_parameter ::= xx_parameter_cast IDENTIFIER",
 /*  70 */ "xx_parameter ::= IDENTIFIER ASSIGN xx_literal_expr",
 /*  71 */ "xx_parameter ::= xx_parameter_type IDENTIFIER ASSIGN xx_literal_expr",
 /*  72 */ "xx_parameter ::= xx_parameter_cast IDENTIFIER ASSIGN xx_literal_expr",
 /*  73 */ "xx_parameter_cast ::= LESS IDENTIFIER GREATER",
 /*  74 */ "xx_parameter_type ::= TYPE_INTEGER",
 /*  75 */ "xx_parameter_type ::= TYPE_DOUBLE",
 /*  76 */ "xx_parameter_type ::= TYPE_BOOL",
 /*  77 */ "xx_parameter_type ::= TYPE_STRING",
 /*  78 */ "xx_parameter_type ::= TYPE_ARRAY",
 /*  79 */ "xx_parameter_type ::= TYPE_VAR",
 /*  80 */ "xx_statement_list ::= xx_statement_list xx_statement",
 /*  81 */ "xx_statement_list ::= xx_statement",
 /*  82 */ "xx_statement ::= xx_let_statement",
 /*  83 */ "xx_statement ::= xx_if_statement",
 /*  84 */ "xx_statement ::= xx_loop_statement",
 /*  85 */ "xx_statement ::= xx_echo_statement",
 /*  86 */ "xx_statement ::= xx_return_statement",
 /*  87 */ "xx_statement ::= xx_mcall_statement",
 /*  88 */ "xx_statement ::= xx_scall_statement",
 /*  89 */ "xx_statement ::= xx_unset_statement",
 /*  90 */ "xx_statement ::= xx_throw_statement",
 /*  91 */ "xx_statement ::= xx_declare_statement",
 /*  92 */ "xx_statement ::= xx_break_statement",
 /*  93 */ "xx_statement ::= xx_continue_statement",
 /*  94 */ "xx_statement ::= xx_while_statement",
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
 /* 116 */ "xx_for_statement ::= FOR IDENTIFIER IN xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 117 */ "xx_for_statement ::= FOR IDENTIFIER IN REVERSE xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 118 */ "xx_for_statement ::= FOR IDENTIFIER COMMA IDENTIFIER IN xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 119 */ "xx_for_statement ::= FOR IDENTIFIER COMMA IDENTIFIER IN REVERSE xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 120 */ "xx_let_statement ::= LET xx_let_assignments DOTCOMMA",
 /* 121 */ "xx_let_assignments ::= xx_let_assignments COMMA xx_let_assignment",
 /* 122 */ "xx_let_assignments ::= xx_let_assignment",
 /* 123 */ "xx_assignment_operator ::= ASSIGN",
 /* 124 */ "xx_assignment_operator ::= ADDASSIGN",
 /* 125 */ "xx_assignment_operator ::= SUBASSIGN",
 /* 126 */ "xx_assignment_operator ::= MULASSIGN",
 /* 127 */ "xx_assignment_operator ::= DIVASSIGN",
 /* 128 */ "xx_assignment_operator ::= CONCATASSIGN",
 /* 129 */ "xx_let_assignment ::= IDENTIFIER xx_assignment_operator xx_assign_expr",
 /* 130 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER xx_assignment_operator xx_assign_expr",
 /* 131 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 132 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER SBRACKET_OPEN xx_index_expr SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 133 */ "xx_let_assignment ::= IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 134 */ "xx_let_assignment ::= IDENTIFIER SBRACKET_OPEN xx_index_expr SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 135 */ "xx_let_assignment ::= IDENTIFIER INCR",
 /* 136 */ "xx_let_assignment ::= IDENTIFIER DECR",
 /* 137 */ "xx_index_expr ::= xx_common_expr",
 /* 138 */ "xx_echo_statement ::= ECHO xx_echo_expressions DOTCOMMA",
 /* 139 */ "xx_echo_expressions ::= xx_echo_expressions COMMA xx_echo_expression",
 /* 140 */ "xx_echo_expressions ::= xx_echo_expression",
 /* 141 */ "xx_echo_expression ::= xx_common_expr",
 /* 142 */ "xx_mcall_statement ::= xx_mcall_expr DOTCOMMA",
 /* 143 */ "xx_scall_statement ::= xx_scall_expr DOTCOMMA",
 /* 144 */ "xx_return_statement ::= RETURN xx_common_expr DOTCOMMA",
 /* 145 */ "xx_unset_statement ::= UNSET IDENTIFIER ARROW IDENTIFIER DOTCOMMA",
 /* 146 */ "xx_unset_statement ::= UNSET IDENTIFIER SBRACKET_OPEN IDENTIFIER SBRACKET_CLOSE DOTCOMMA",
 /* 147 */ "xx_unset_statement ::= UNSET IDENTIFIER SBRACKET_OPEN INTEGER SBRACKET_CLOSE DOTCOMMA",
 /* 148 */ "xx_unset_statement ::= UNSET IDENTIFIER SBRACKET_OPEN STRING SBRACKET_CLOSE DOTCOMMA",
 /* 149 */ "xx_throw_statement ::= THROW IDENTIFIER DOTCOMMA",
 /* 150 */ "xx_throw_statement ::= THROW NEW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE DOTCOMMA",
 /* 151 */ "xx_declare_statement ::= TYPE_INTEGER xx_declare_variable_list DOTCOMMA",
 /* 152 */ "xx_declare_statement ::= TYPE_UINTEGER xx_declare_variable_list DOTCOMMA",
 /* 153 */ "xx_declare_statement ::= TYPE_CHAR xx_declare_variable_list DOTCOMMA",
 /* 154 */ "xx_declare_statement ::= TYPE_UCHAR xx_declare_variable_list DOTCOMMA",
 /* 155 */ "xx_declare_statement ::= TYPE_LONG xx_declare_variable_list DOTCOMMA",
 /* 156 */ "xx_declare_statement ::= TYPE_ULONG xx_declare_variable_list DOTCOMMA",
 /* 157 */ "xx_declare_statement ::= TYPE_DOUBLE xx_declare_variable_list DOTCOMMA",
 /* 158 */ "xx_declare_statement ::= TYPE_STRING xx_declare_variable_list DOTCOMMA",
 /* 159 */ "xx_declare_statement ::= TYPE_BOOL xx_declare_variable_list DOTCOMMA",
 /* 160 */ "xx_declare_statement ::= TYPE_VAR xx_declare_variable_list DOTCOMMA",
 /* 161 */ "xx_declare_variable_list ::= xx_declare_variable_list COMMA xx_declare_variable",
 /* 162 */ "xx_declare_variable_list ::= xx_declare_variable",
 /* 163 */ "xx_declare_variable ::= IDENTIFIER",
 /* 164 */ "xx_declare_variable ::= IDENTIFIER ASSIGN xx_literal_expr",
 /* 165 */ "xx_assign_expr ::= xx_common_expr",
 /* 166 */ "xx_common_expr ::= NOT xx_common_expr",
 /* 167 */ "xx_common_expr ::= ISSET xx_isset_expr",
 /* 168 */ "xx_common_expr ::= xx_common_expr EQUALS xx_common_expr",
 /* 169 */ "xx_common_expr ::= xx_common_expr NOTEQUALS xx_common_expr",
 /* 170 */ "xx_common_expr ::= xx_common_expr IDENTICAL xx_common_expr",
 /* 171 */ "xx_common_expr ::= xx_common_expr NOTIDENTICAL xx_common_expr",
 /* 172 */ "xx_common_expr ::= xx_common_expr LESS xx_common_expr",
 /* 173 */ "xx_common_expr ::= xx_common_expr GREATER xx_common_expr",
 /* 174 */ "xx_common_expr ::= xx_common_expr LESSEQUAL xx_common_expr",
 /* 175 */ "xx_common_expr ::= xx_common_expr GREATEREQUAL xx_common_expr",
 /* 176 */ "xx_common_expr ::= PARENTHESES_OPEN xx_common_expr PARENTHESES_CLOSE",
 /* 177 */ "xx_common_expr ::= PARENTHESES_OPEN xx_parameter_type PARENTHESES_CLOSE xx_common_expr",
 /* 178 */ "xx_common_expr ::= IDENTIFIER ARROW IDENTIFIER",
 /* 179 */ "xx_common_expr ::= IDENTIFIER SBRACKET_OPEN IDENTIFIER SBRACKET_CLOSE",
 /* 180 */ "xx_common_expr ::= IDENTIFIER SBRACKET_OPEN INTEGER SBRACKET_CLOSE",
 /* 181 */ "xx_common_expr ::= IDENTIFIER SBRACKET_OPEN STRING SBRACKET_CLOSE",
 /* 182 */ "xx_common_expr ::= xx_common_expr ADD xx_common_expr",
 /* 183 */ "xx_common_expr ::= xx_common_expr SUB xx_common_expr",
 /* 184 */ "xx_common_expr ::= xx_common_expr CONCAT xx_common_expr",
 /* 185 */ "xx_common_expr ::= xx_common_expr AND xx_common_expr",
 /* 186 */ "xx_common_expr ::= xx_common_expr OR xx_common_expr",
 /* 187 */ "xx_common_expr ::= INSTANCEOF xx_common_expr",
 /* 188 */ "xx_common_expr ::= FETCH IDENTIFIER COMMA xx_isset_expr",
 /* 189 */ "xx_common_expr ::= TYPEOF xx_common_expr",
 /* 190 */ "xx_common_expr ::= IDENTIFIER",
 /* 191 */ "xx_common_expr ::= INTEGER",
 /* 192 */ "xx_common_expr ::= STRING",
 /* 193 */ "xx_common_expr ::= SCHAR",
 /* 194 */ "xx_common_expr ::= DOUBLE",
 /* 195 */ "xx_common_expr ::= NULL",
 /* 196 */ "xx_common_expr ::= TRUE",
 /* 197 */ "xx_common_expr ::= FALSE",
 /* 198 */ "xx_common_expr ::= SBRACKET_OPEN SBRACKET_CLOSE",
 /* 199 */ "xx_common_expr ::= SBRACKET_OPEN xx_array_list SBRACKET_CLOSE",
 /* 200 */ "xx_common_expr ::= NEW IDENTIFIER",
 /* 201 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 202 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 203 */ "xx_common_expr ::= IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 204 */ "xx_common_expr ::= IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 205 */ "xx_common_expr ::= xx_mcall_expr",
 /* 206 */ "xx_common_expr ::= xx_scall_expr",
 /* 207 */ "xx_scall_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 208 */ "xx_scall_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 209 */ "xx_mcall_expr ::= IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 210 */ "xx_mcall_expr ::= IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 211 */ "xx_call_parameters ::= xx_call_parameters COMMA xx_call_parameter",
 /* 212 */ "xx_call_parameters ::= xx_call_parameter",
 /* 213 */ "xx_call_parameter ::= xx_common_expr",
 /* 214 */ "xx_array_list ::= xx_array_list COMMA xx_array_item",
 /* 215 */ "xx_array_list ::= xx_array_item",
 /* 216 */ "xx_array_item ::= xx_array_key COLON xx_array_value",
 /* 217 */ "xx_array_item ::= xx_array_value",
 /* 218 */ "xx_array_key ::= IDENTIFIER",
 /* 219 */ "xx_array_key ::= STRING",
 /* 220 */ "xx_array_key ::= INTEGER",
 /* 221 */ "xx_array_value ::= xx_literal_expr",
 /* 222 */ "xx_array_value ::= IDENTIFIER",
 /* 223 */ "xx_literal_expr ::= INTEGER",
 /* 224 */ "xx_literal_expr ::= STRING",
 /* 225 */ "xx_literal_expr ::= DOUBLE",
 /* 226 */ "xx_literal_expr ::= NULL",
 /* 227 */ "xx_literal_expr ::= FALSE",
 /* 228 */ "xx_literal_expr ::= TRUE",
 /* 229 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN IDENTIFIER SBRACKET_CLOSE",
 /* 230 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN INTEGER SBRACKET_CLOSE",
 /* 231 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN STRING SBRACKET_CLOSE",
 /* 232 */ "xx_eval_expr ::= xx_common_expr",
 /* 233 */ "xx_comment ::= COMMENT",
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
// 814 "parser.lemon"
{
	/*if ((yypminor->yy0)) {
		if ((yypminor->yy0)->free_flag) {
			efree((yypminor->yy0)->token);
		}
		efree((yypminor->yy0));
	}*/
}
// 2035 "parser.c"
      break;
    case 89:
// 827 "parser.lemon"
{ json_object_put((yypminor->yy107)); }
// 2040 "parser.c"
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
  { 88, 1 },
  { 89, 1 },
  { 90, 2 },
  { 90, 1 },
  { 91, 1 },
  { 91, 1 },
  { 91, 1 },
  { 92, 3 },
  { 93, 5 },
  { 93, 7 },
  { 93, 6 },
  { 93, 8 },
  { 93, 4 },
  { 93, 6 },
  { 93, 5 },
  { 93, 7 },
  { 95, 1 },
  { 95, 1 },
  { 95, 1 },
  { 95, 2 },
  { 95, 2 },
  { 95, 2 },
  { 95, 2 },
  { 95, 3 },
  { 95, 3 },
  { 96, 2 },
  { 96, 1 },
  { 99, 4 },
  { 99, 3 },
  { 99, 6 },
  { 99, 5 },
  { 100, 2 },
  { 100, 1 },
  { 102, 1 },
  { 102, 1 },
  { 102, 1 },
  { 102, 1 },
  { 97, 2 },
  { 97, 1 },
  { 98, 2 },
  { 98, 1 },
  { 103, 6 },
  { 103, 5 },
  { 104, 7 },
  { 104, 8 },
  { 104, 8 },
  { 104, 9 },
  { 104, 8 },
  { 104, 9 },
  { 104, 9 },
  { 104, 10 },
  { 105, 2 },
  { 105, 2 },
  { 105, 2 },
  { 105, 2 },
  { 105, 2 },
  { 105, 2 },
  { 105, 2 },
  { 105, 2 },
  { 105, 2 },
  { 105, 2 },
  { 105, 1 },
  { 105, 1 },
  { 105, 1 },
  { 105, 1 },
  { 106, 3 },
  { 106, 1 },
  { 108, 1 },
  { 108, 2 },
  { 108, 2 },
  { 108, 3 },
  { 108, 4 },
  { 108, 4 },
  { 110, 3 },
  { 109, 1 },
  { 109, 1 },
  { 109, 1 },
  { 109, 1 },
  { 109, 1 },
  { 109, 1 },
  { 107, 2 },
  { 107, 1 },
  { 111, 1 },
  { 111, 1 },
  { 111, 1 },
  { 111, 1 },
  { 111, 1 },
  { 111, 1 },
  { 111, 1 },
  { 111, 1 },
  { 111, 1 },
  { 111, 1 },
  { 111, 1 },
  { 111, 1 },
  { 111, 1 },
  { 111, 1 },
  { 111, 1 },
  { 111, 1 },
  { 122, 2 },
  { 123, 2 },
  { 113, 4 },
  { 113, 7 },
  { 113, 5 },
  { 113, 9 },
  { 113, 8 },
  { 113, 8 },
  { 125, 4 },
  { 125, 5 },
  { 128, 2 },
  { 128, 1 },
  { 129, 4 },
  { 129, 3 },
  { 114, 3 },
  { 114, 4 },
  { 124, 4 },
  { 124, 5 },
  { 126, 7 },
  { 126, 8 },
  { 126, 9 },
  { 126, 10 },
  { 112, 3 },
  { 131, 3 },
  { 131, 1 },
  { 133, 1 },
  { 133, 1 },
  { 133, 1 },
  { 133, 1 },
  { 133, 1 },
  { 133, 1 },
  { 132, 3 },
  { 132, 5 },
  { 132, 7 },
  { 132, 8 },
  { 132, 5 },
  { 132, 6 },
  { 132, 2 },
  { 132, 2 },
  { 135, 1 },
  { 115, 3 },
  { 136, 3 },
  { 136, 1 },
  { 137, 1 },
  { 117, 2 },
  { 118, 2 },
  { 116, 3 },
  { 119, 5 },
  { 119, 6 },
  { 119, 6 },
  { 119, 6 },
  { 120, 3 },
  { 120, 7 },
  { 121, 3 },
  { 121, 3 },
  { 121, 3 },
  { 121, 3 },
  { 121, 3 },
  { 121, 3 },
  { 121, 3 },
  { 121, 3 },
  { 121, 3 },
  { 121, 3 },
  { 141, 3 },
  { 141, 1 },
  { 142, 1 },
  { 142, 3 },
  { 134, 1 },
  { 130, 2 },
  { 130, 2 },
  { 130, 3 },
  { 130, 3 },
  { 130, 3 },
  { 130, 3 },
  { 130, 3 },
  { 130, 3 },
  { 130, 3 },
  { 130, 3 },
  { 130, 3 },
  { 130, 4 },
  { 130, 3 },
  { 130, 4 },
  { 130, 4 },
  { 130, 4 },
  { 130, 3 },
  { 130, 3 },
  { 130, 3 },
  { 130, 3 },
  { 130, 3 },
  { 130, 2 },
  { 130, 4 },
  { 130, 2 },
  { 130, 1 },
  { 130, 1 },
  { 130, 1 },
  { 130, 1 },
  { 130, 1 },
  { 130, 1 },
  { 130, 1 },
  { 130, 1 },
  { 130, 2 },
  { 130, 3 },
  { 130, 2 },
  { 130, 4 },
  { 130, 5 },
  { 130, 4 },
  { 130, 3 },
  { 130, 1 },
  { 130, 1 },
  { 139, 6 },
  { 139, 5 },
  { 138, 6 },
  { 138, 5 },
  { 140, 3 },
  { 140, 1 },
  { 145, 1 },
  { 144, 3 },
  { 144, 1 },
  { 146, 3 },
  { 146, 1 },
  { 147, 1 },
  { 147, 1 },
  { 147, 1 },
  { 148, 1 },
  { 148, 1 },
  { 101, 1 },
  { 101, 1 },
  { 101, 1 },
  { 101, 1 },
  { 101, 1 },
  { 101, 1 },
  { 143, 4 },
  { 143, 4 },
  { 143, 4 },
  { 127, 1 },
  { 94, 1 },
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
	status->ret = yymsp[0].minor.yy107;
}
// 2491 "parser.c"
        break;
      case 1:
      case 4:
      case 5:
      case 6:
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
      case 137:
      case 165:
      case 205:
      case 206:
      case 213:
      case 221:
      case 232:
// 829 "parser.lemon"
{
	yygotominor.yy107 = yymsp[0].minor.yy107;
}
// 2524 "parser.c"
        break;
      case 2:
      case 25:
      case 31:
      case 37:
      case 39:
      case 80:
      case 108:
// 833 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_list(yymsp[-1].minor.yy107, yymsp[0].minor.yy107);
}
// 2537 "parser.c"
        break;
      case 3:
      case 26:
      case 32:
      case 38:
      case 40:
      case 66:
      case 81:
      case 109:
      case 122:
      case 140:
      case 162:
      case 212:
      case 215:
// 837 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_list(NULL, yymsp[0].minor.yy107);
}
// 2556 "parser.c"
        break;
      case 7:
// 853 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_namespace(yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(26,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 2565 "parser.c"
        break;
      case 8:
// 857 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[-1].minor.yy107, 0, NULL, status->scanner_state);
  yy_destructor(29,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 2575 "parser.c"
        break;
      case 9:
// 861 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_class(yymsp[-5].minor.yy0, yymsp[-1].minor.yy107, 0, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(29,&yymsp[-6].minor);
  yy_destructor(32,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 2586 "parser.c"
        break;
      case 10:
// 865 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[-1].minor.yy107, 1, NULL, status->scanner_state);
  yy_destructor(33,&yymsp[-5].minor);
  yy_destructor(29,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 2597 "parser.c"
        break;
      case 11:
// 869 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_class(yymsp[-5].minor.yy0, yymsp[-1].minor.yy107, 1, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(33,&yymsp[-7].minor);
  yy_destructor(29,&yymsp[-6].minor);
  yy_destructor(32,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 2609 "parser.c"
        break;
      case 12:
// 873 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_class(yymsp[-2].minor.yy0, NULL, 0, NULL, status->scanner_state);
  yy_destructor(29,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 2619 "parser.c"
        break;
      case 13:
// 877 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_class(yymsp[-4].minor.yy0, NULL, 0, yymsp[-2].minor.yy0, status->scanner_state);
  yy_destructor(29,&yymsp[-5].minor);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 2630 "parser.c"
        break;
      case 14:
// 881 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_class(yymsp[-2].minor.yy0, NULL, 1, NULL, status->scanner_state);
  yy_destructor(33,&yymsp[-4].minor);
  yy_destructor(29,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 2641 "parser.c"
        break;
      case 15:
// 885 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_class(yymsp[-4].minor.yy0, NULL, 1, yymsp[-2].minor.yy0, status->scanner_state);
  yy_destructor(33,&yymsp[-6].minor);
  yy_destructor(29,&yymsp[-5].minor);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 2653 "parser.c"
        break;
      case 16:
// 889 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_class_definition(yymsp[0].minor.yy107, NULL, NULL, status->scanner_state);
}
// 2660 "parser.c"
        break;
      case 17:
// 893 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_class_definition(NULL, NULL, yymsp[0].minor.yy107, status->scanner_state);
}
// 2667 "parser.c"
        break;
      case 18:
// 897 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_class_definition(NULL, yymsp[0].minor.yy107, NULL, status->scanner_state);
}
// 2674 "parser.c"
        break;
      case 19:
// 901 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_class_definition(yymsp[-1].minor.yy107, yymsp[0].minor.yy107, NULL, status->scanner_state);
}
// 2681 "parser.c"
        break;
      case 20:
// 905 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_class_definition(yymsp[-1].minor.yy107, NULL, yymsp[0].minor.yy107, status->scanner_state);
}
// 2688 "parser.c"
        break;
      case 21:
// 909 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_class_definition(yymsp[0].minor.yy107, NULL, yymsp[-1].minor.yy107, status->scanner_state);
}
// 2695 "parser.c"
        break;
      case 22:
// 913 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_class_definition(NULL, yymsp[0].minor.yy107, yymsp[-1].minor.yy107, status->scanner_state);
}
// 2702 "parser.c"
        break;
      case 23:
// 917 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_class_definition(yymsp[-2].minor.yy107, yymsp[0].minor.yy107, yymsp[-1].minor.yy107, status->scanner_state);
}
// 2709 "parser.c"
        break;
      case 24:
// 921 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_class_definition(yymsp[-1].minor.yy107, yymsp[0].minor.yy107, yymsp[-2].minor.yy107, status->scanner_state);
}
// 2716 "parser.c"
        break;
      case 27:
// 933 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_class_property(yymsp[-2].minor.yy107, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(28,&yymsp[0].minor);
}
// 2724 "parser.c"
        break;
      case 28:
// 937 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_class_property(yymsp[-2].minor.yy107, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[0].minor);
}
// 2732 "parser.c"
        break;
      case 29:
// 941 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_class_property(yymsp[-4].minor.yy107, yymsp[-3].minor.yy0, yymsp[-1].minor.yy107, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(35,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 2741 "parser.c"
        break;
      case 30:
// 945 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_class_property(yymsp[-4].minor.yy107, yymsp[-3].minor.yy0, yymsp[-1].minor.yy107, NULL, status->scanner_state);
  yy_destructor(35,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 2750 "parser.c"
        break;
      case 33:
// 957 "parser.lemon"
{
	yygotominor.yy107 = json_object_new_string("public");
  yy_destructor(1,&yymsp[0].minor);
}
// 2758 "parser.c"
        break;
      case 34:
// 961 "parser.lemon"
{
	yygotominor.yy107 = json_object_new_string("protected");
  yy_destructor(2,&yymsp[0].minor);
}
// 2766 "parser.c"
        break;
      case 35:
// 965 "parser.lemon"
{
	yygotominor.yy107 = json_object_new_string("static");
  yy_destructor(3,&yymsp[0].minor);
}
// 2774 "parser.c"
        break;
      case 36:
// 969 "parser.lemon"
{
	yygotominor.yy107 = json_object_new_string("private");
  yy_destructor(4,&yymsp[0].minor);
}
// 2782 "parser.c"
        break;
      case 41:
// 989 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy107, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(35,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 2792 "parser.c"
        break;
      case 42:
// 993 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy107, NULL, status->scanner_state);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(35,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 2802 "parser.c"
        break;
      case 43:
// 997 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_class_method(yymsp[-6].minor.yy107, yymsp[-4].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(37,&yymsp[-5].minor);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 2814 "parser.c"
        break;
      case 44:
// 1001 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_class_method(yymsp[-7].minor.yy107, yymsp[-5].minor.yy0, yymsp[-3].minor.yy107, NULL, NULL, status->scanner_state);
  yy_destructor(37,&yymsp[-6].minor);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 2826 "parser.c"
        break;
      case 45:
// 1005 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_class_method(yymsp[-7].minor.yy107, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy107, NULL, status->scanner_state);
  yy_destructor(37,&yymsp[-6].minor);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(25,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 2838 "parser.c"
        break;
      case 46:
// 1009 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_class_method(yymsp[-8].minor.yy107, yymsp[-6].minor.yy0, yymsp[-4].minor.yy107, yymsp[-1].minor.yy107, NULL, status->scanner_state);
  yy_destructor(37,&yymsp[-7].minor);
  yy_destructor(38,&yymsp[-5].minor);
  yy_destructor(25,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 2850 "parser.c"
        break;
      case 47:
// 1013 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_class_method(yymsp[-6].minor.yy107, yymsp[-4].minor.yy0, NULL, NULL, yymsp[-7].minor.yy0, status->scanner_state);
  yy_destructor(37,&yymsp[-5].minor);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 2862 "parser.c"
        break;
      case 48:
// 1017 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_class_method(yymsp[-7].minor.yy107, yymsp[-5].minor.yy0, yymsp[-3].minor.yy107, NULL, yymsp[-8].minor.yy0, status->scanner_state);
  yy_destructor(37,&yymsp[-6].minor);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 2874 "parser.c"
        break;
      case 49:
// 1021 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_class_method(yymsp[-7].minor.yy107, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy107, yymsp[-8].minor.yy0, status->scanner_state);
  yy_destructor(37,&yymsp[-6].minor);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(25,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 2886 "parser.c"
        break;
      case 50:
// 1025 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_class_method(yymsp[-8].minor.yy107, yymsp[-6].minor.yy0, yymsp[-4].minor.yy107, yymsp[-1].minor.yy107, yymsp[-9].minor.yy0, status->scanner_state);
  yy_destructor(37,&yymsp[-7].minor);
  yy_destructor(38,&yymsp[-5].minor);
  yy_destructor(25,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 2898 "parser.c"
        break;
      case 51:
// 1037 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_list(json_object_new_string("static"), json_object_new_string("public"));
  yy_destructor(3,&yymsp[-1].minor);
  yy_destructor(1,&yymsp[0].minor);
}
// 2907 "parser.c"
        break;
      case 52:
// 1041 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_list(json_object_new_string("static"), json_object_new_string("public"));
  yy_destructor(1,&yymsp[-1].minor);
  yy_destructor(3,&yymsp[0].minor);
}
// 2916 "parser.c"
        break;
      case 53:
// 1045 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_list(json_object_new_string("static"), json_object_new_string("protected"));
  yy_destructor(3,&yymsp[-1].minor);
  yy_destructor(2,&yymsp[0].minor);
}
// 2925 "parser.c"
        break;
      case 54:
// 1049 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_list(json_object_new_string("static"), json_object_new_string("protected"));
  yy_destructor(2,&yymsp[-1].minor);
  yy_destructor(3,&yymsp[0].minor);
}
// 2934 "parser.c"
        break;
      case 55:
// 1053 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_list(json_object_new_string("static"), json_object_new_string("private"));
  yy_destructor(3,&yymsp[-1].minor);
  yy_destructor(4,&yymsp[0].minor);
}
// 2943 "parser.c"
        break;
      case 56:
// 1057 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_list(json_object_new_string("static"), json_object_new_string("private"));
  yy_destructor(4,&yymsp[-1].minor);
  yy_destructor(3,&yymsp[0].minor);
}
// 2952 "parser.c"
        break;
      case 57:
// 1061 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_list(json_object_new_string("inline"), json_object_new_string("protected"));
  yy_destructor(39,&yymsp[-1].minor);
  yy_destructor(2,&yymsp[0].minor);
}
// 2961 "parser.c"
        break;
      case 58:
// 1065 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_list(json_object_new_string("inline"), json_object_new_string("protected"));
  yy_destructor(2,&yymsp[-1].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 2970 "parser.c"
        break;
      case 59:
// 1069 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_list(json_object_new_string("inline"), json_object_new_string("private"));
  yy_destructor(39,&yymsp[-1].minor);
  yy_destructor(4,&yymsp[0].minor);
}
// 2979 "parser.c"
        break;
      case 60:
// 1073 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_list(json_object_new_string("inline"), json_object_new_string("private"));
  yy_destructor(4,&yymsp[-1].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 2988 "parser.c"
        break;
      case 61:
// 1077 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_list(NULL, json_object_new_string("public"));
  yy_destructor(1,&yymsp[0].minor);
}
// 2996 "parser.c"
        break;
      case 62:
// 1081 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_list(NULL, json_object_new_string("protected"));
  yy_destructor(2,&yymsp[0].minor);
}
// 3004 "parser.c"
        break;
      case 63:
// 1085 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_list(NULL, json_object_new_string("static"));
  yy_destructor(3,&yymsp[0].minor);
}
// 3012 "parser.c"
        break;
      case 64:
// 1089 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_list(NULL, json_object_new_string("private"));
  yy_destructor(4,&yymsp[0].minor);
}
// 3020 "parser.c"
        break;
      case 65:
      case 121:
      case 139:
      case 161:
      case 211:
      case 214:
// 1093 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_list(yymsp[-2].minor.yy107, yymsp[0].minor.yy107);
  yy_destructor(5,&yymsp[-1].minor);
}
// 3033 "parser.c"
        break;
      case 67:
// 1101 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_parameter(NULL, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3040 "parser.c"
        break;
      case 68:
// 1105 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_parameter(yymsp[-1].minor.yy107, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3047 "parser.c"
        break;
      case 69:
// 1109 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_parameter(NULL, yymsp[-1].minor.yy107, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3054 "parser.c"
        break;
      case 70:
// 1113 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_parameter(NULL, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy107, status->scanner_state);
  yy_destructor(35,&yymsp[-1].minor);
}
// 3062 "parser.c"
        break;
      case 71:
// 1117 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_parameter(yymsp[-3].minor.yy107, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy107, status->scanner_state);
  yy_destructor(35,&yymsp[-1].minor);
}
// 3070 "parser.c"
        break;
      case 72:
// 1121 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_parameter(NULL, yymsp[-3].minor.yy107, yymsp[-2].minor.yy0, yymsp[0].minor.yy107, status->scanner_state);
  yy_destructor(35,&yymsp[-1].minor);
}
// 3078 "parser.c"
        break;
      case 73:
// 1125 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(10,&yymsp[-2].minor);
  yy_destructor(11,&yymsp[0].minor);
}
// 3087 "parser.c"
        break;
      case 74:
// 1129 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_type(XX_TYPE_INTEGER);
  yy_destructor(40,&yymsp[0].minor);
}
// 3095 "parser.c"
        break;
      case 75:
// 1133 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_type(XX_TYPE_DOUBLE);
  yy_destructor(41,&yymsp[0].minor);
}
// 3103 "parser.c"
        break;
      case 76:
// 1137 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_type(XX_TYPE_BOOL);
  yy_destructor(42,&yymsp[0].minor);
}
// 3111 "parser.c"
        break;
      case 77:
// 1141 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_type(XX_TYPE_STRING);
  yy_destructor(43,&yymsp[0].minor);
}
// 3119 "parser.c"
        break;
      case 78:
// 1145 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_type(XX_TYPE_ARRAY);
  yy_destructor(44,&yymsp[0].minor);
}
// 3127 "parser.c"
        break;
      case 79:
// 1149 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_type(XX_TYPE_VAR);
  yy_destructor(45,&yymsp[0].minor);
}
// 3135 "parser.c"
        break;
      case 98:
// 1225 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_break_statement(status->scanner_state);
  yy_destructor(46,&yymsp[-1].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3144 "parser.c"
        break;
      case 99:
// 1229 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_continue_statement(status->scanner_state);
  yy_destructor(47,&yymsp[-1].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3153 "parser.c"
        break;
      case 100:
// 1233 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_if_statement(yymsp[-2].minor.yy107, NULL, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3163 "parser.c"
        break;
      case 101:
// 1237 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_if_statement(yymsp[-5].minor.yy107, NULL, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-6].minor);
  yy_destructor(30,&yymsp[-4].minor);
  yy_destructor(31,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3176 "parser.c"
        break;
      case 102:
// 1241 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_if_statement(yymsp[-3].minor.yy107, yymsp[-1].minor.yy107, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3186 "parser.c"
        break;
      case 103:
// 1245 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_if_statement(yymsp[-7].minor.yy107, yymsp[-5].minor.yy107, yymsp[-1].minor.yy107, status->scanner_state);
  yy_destructor(48,&yymsp[-8].minor);
  yy_destructor(30,&yymsp[-6].minor);
  yy_destructor(31,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3199 "parser.c"
        break;
      case 104:
// 1249 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_if_statement(yymsp[-6].minor.yy107, yymsp[-4].minor.yy107, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-7].minor);
  yy_destructor(30,&yymsp[-5].minor);
  yy_destructor(31,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3212 "parser.c"
        break;
      case 105:
// 1253 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_if_statement(yymsp[-6].minor.yy107, NULL, yymsp[-1].minor.yy107, status->scanner_state);
  yy_destructor(48,&yymsp[-7].minor);
  yy_destructor(30,&yymsp[-5].minor);
  yy_destructor(31,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3225 "parser.c"
        break;
      case 106:
// 1257 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_switch_statement(yymsp[-2].minor.yy107, NULL, status->scanner_state);
  yy_destructor(50,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3235 "parser.c"
        break;
      case 107:
// 1261 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_switch_statement(yymsp[-3].minor.yy107, yymsp[-1].minor.yy107, status->scanner_state);
  yy_destructor(50,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3245 "parser.c"
        break;
      case 110:
// 1273 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_case_clause(yymsp[-2].minor.yy107, yymsp[0].minor.yy107, status->scanner_state);
  yy_destructor(51,&yymsp[-3].minor);
  yy_destructor(52,&yymsp[-1].minor);
}
// 3254 "parser.c"
        break;
      case 111:
// 1277 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_case_clause(NULL, yymsp[0].minor.yy107, status->scanner_state);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(52,&yymsp[-1].minor);
}
// 3263 "parser.c"
        break;
      case 112:
// 1281 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_loop_statement(NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3273 "parser.c"
        break;
      case 113:
// 1285 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_loop_statement(yymsp[-1].minor.yy107, status->scanner_state);
  yy_destructor(54,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3283 "parser.c"
        break;
      case 114:
// 1289 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_while_statement(yymsp[-2].minor.yy107, NULL, status->scanner_state);
  yy_destructor(55,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3293 "parser.c"
        break;
      case 115:
// 1293 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_while_statement(yymsp[-3].minor.yy107, yymsp[-1].minor.yy107, status->scanner_state);
  yy_destructor(55,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3303 "parser.c"
        break;
      case 116:
// 1297 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_for_statement(yymsp[-3].minor.yy107, NULL, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy107, status->scanner_state);
  yy_destructor(56,&yymsp[-6].minor);
  yy_destructor(57,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3314 "parser.c"
        break;
      case 117:
// 1301 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_for_statement(yymsp[-3].minor.yy107, NULL, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy107, status->scanner_state);
  yy_destructor(56,&yymsp[-7].minor);
  yy_destructor(57,&yymsp[-5].minor);
  yy_destructor(58,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3326 "parser.c"
        break;
      case 118:
// 1305 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_for_statement(yymsp[-3].minor.yy107, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy107, status->scanner_state);
  yy_destructor(56,&yymsp[-8].minor);
  yy_destructor(5,&yymsp[-6].minor);
  yy_destructor(57,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3338 "parser.c"
        break;
      case 119:
// 1309 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_for_statement(yymsp[-3].minor.yy107, yymsp[-8].minor.yy0, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy107, status->scanner_state);
  yy_destructor(56,&yymsp[-9].minor);
  yy_destructor(5,&yymsp[-7].minor);
  yy_destructor(57,&yymsp[-5].minor);
  yy_destructor(58,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3351 "parser.c"
        break;
      case 120:
// 1313 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_let_statement(yymsp[-1].minor.yy107, status->scanner_state);
  yy_destructor(59,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3360 "parser.c"
        break;
      case 123:
// 1326 "parser.lemon"
{
	yygotominor.yy107 = json_object_new_string("assign");
  yy_destructor(35,&yymsp[0].minor);
}
// 3368 "parser.c"
        break;
      case 124:
// 1331 "parser.lemon"
{
	yygotominor.yy107 = json_object_new_string("add-assign");
  yy_destructor(60,&yymsp[0].minor);
}
// 3376 "parser.c"
        break;
      case 125:
// 1336 "parser.lemon"
{
	yygotominor.yy107 = json_object_new_string("sub-assign");
  yy_destructor(61,&yymsp[0].minor);
}
// 3384 "parser.c"
        break;
      case 126:
// 1340 "parser.lemon"
{
	yygotominor.yy107 = json_object_new_string("mult-assign");
  yy_destructor(62,&yymsp[0].minor);
}
// 3392 "parser.c"
        break;
      case 127:
// 1344 "parser.lemon"
{
	yygotominor.yy107 = json_object_new_string("div-assign");
  yy_destructor(63,&yymsp[0].minor);
}
// 3400 "parser.c"
        break;
      case 128:
// 1348 "parser.lemon"
{
	yygotominor.yy107 = json_object_new_string("concat-assign");
  yy_destructor(64,&yymsp[0].minor);
}
// 3408 "parser.c"
        break;
      case 129:
// 1353 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_let_assignment("variable", yymsp[-1].minor.yy107, yymsp[-2].minor.yy0, NULL, NULL, yymsp[0].minor.yy107, status->scanner_state);
}
// 3415 "parser.c"
        break;
      case 130:
// 1358 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_let_assignment("object-property", yymsp[-1].minor.yy107, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy107, status->scanner_state);
  yy_destructor(65,&yymsp[-3].minor);
}
// 3423 "parser.c"
        break;
      case 131:
// 1363 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_let_assignment("object-property-append", yymsp[-1].minor.yy107, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy107, status->scanner_state);
  yy_destructor(65,&yymsp[-5].minor);
  yy_destructor(66,&yymsp[-3].minor);
  yy_destructor(67,&yymsp[-2].minor);
}
// 3433 "parser.c"
        break;
      case 132:
// 1368 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_let_assignment("object-property-array-index", yymsp[-1].minor.yy107, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy107, yymsp[0].minor.yy107, status->scanner_state);
  yy_destructor(65,&yymsp[-6].minor);
  yy_destructor(66,&yymsp[-4].minor);
  yy_destructor(67,&yymsp[-2].minor);
}
// 3443 "parser.c"
        break;
      case 133:
// 1373 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_let_assignment("variable-append", yymsp[-1].minor.yy107, yymsp[-4].minor.yy0, NULL, NULL, yymsp[0].minor.yy107, status->scanner_state);
  yy_destructor(66,&yymsp[-3].minor);
  yy_destructor(67,&yymsp[-2].minor);
}
// 3452 "parser.c"
        break;
      case 134:
// 1378 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_let_assignment("array-index", yymsp[-1].minor.yy107, yymsp[-5].minor.yy0, NULL, yymsp[-3].minor.yy107, yymsp[0].minor.yy107, status->scanner_state);
  yy_destructor(66,&yymsp[-4].minor);
  yy_destructor(67,&yymsp[-2].minor);
}
// 3461 "parser.c"
        break;
      case 135:
// 1383 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_let_assignment("incr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(68,&yymsp[0].minor);
}
// 3469 "parser.c"
        break;
      case 136:
// 1388 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_let_assignment("decr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(69,&yymsp[0].minor);
}
// 3477 "parser.c"
        break;
      case 138:
// 1396 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_echo_statement(yymsp[-1].minor.yy107, status->scanner_state);
  yy_destructor(70,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3486 "parser.c"
        break;
      case 141:
// 1408 "parser.lemon"
{
	yygotominor.yy107 = yymsp[0].minor.yy107;;
}
// 3493 "parser.c"
        break;
      case 142:
// 1413 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_mcall_statement(yymsp[-1].minor.yy107, status->scanner_state);
  yy_destructor(28,&yymsp[0].minor);
}
// 3501 "parser.c"
        break;
      case 143:
// 1418 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_scall_statement(yymsp[-1].minor.yy107, status->scanner_state);
  yy_destructor(28,&yymsp[0].minor);
}
// 3509 "parser.c"
        break;
      case 144:
// 1423 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_return_statement(yymsp[-1].minor.yy107, status->scanner_state);
  yy_destructor(71,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3518 "parser.c"
        break;
      case 145:
// 1428 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(72,&yymsp[-4].minor);
  yy_destructor(65,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3532 "parser.c"
        break;
      case 146:
// 1437 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(72,&yymsp[-5].minor);
  yy_destructor(66,&yymsp[-3].minor);
  yy_destructor(67,&yymsp[-1].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3547 "parser.c"
        break;
      case 147:
// 1446 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_INTEGER, yymsp[-2].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(72,&yymsp[-5].minor);
  yy_destructor(66,&yymsp[-3].minor);
  yy_destructor(67,&yymsp[-1].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3562 "parser.c"
        break;
      case 148:
// 1455 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_STRING, yymsp[-2].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(72,&yymsp[-5].minor);
  yy_destructor(66,&yymsp[-3].minor);
  yy_destructor(67,&yymsp[-1].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3577 "parser.c"
        break;
      case 149:
// 1464 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_throw_exception(yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(75,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3586 "parser.c"
        break;
      case 150:
// 1469 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_throw_exception(yymsp[-4].minor.yy0, yymsp[-2].minor.yy107, status->scanner_state);
  yy_destructor(75,&yymsp[-6].minor);
  yy_destructor(23,&yymsp[-5].minor);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(25,&yymsp[-1].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3598 "parser.c"
        break;
      case 151:
// 1473 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_declare_statement(XX_T_TYPE_INTEGER, yymsp[-1].minor.yy107, status->scanner_state);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3607 "parser.c"
        break;
      case 152:
// 1477 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_declare_statement(XX_T_TYPE_UINTEGER, yymsp[-1].minor.yy107, status->scanner_state);
  yy_destructor(76,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3616 "parser.c"
        break;
      case 153:
// 1481 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_declare_statement(XX_T_TYPE_CHAR, yymsp[-1].minor.yy107, status->scanner_state);
  yy_destructor(77,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3625 "parser.c"
        break;
      case 154:
// 1485 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_declare_statement(XX_T_TYPE_UCHAR, yymsp[-1].minor.yy107, status->scanner_state);
  yy_destructor(78,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3634 "parser.c"
        break;
      case 155:
// 1489 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_declare_statement(XX_T_TYPE_LONG, yymsp[-1].minor.yy107, status->scanner_state);
  yy_destructor(79,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3643 "parser.c"
        break;
      case 156:
// 1493 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_declare_statement(XX_T_TYPE_ULONG, yymsp[-1].minor.yy107, status->scanner_state);
  yy_destructor(80,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3652 "parser.c"
        break;
      case 157:
// 1497 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_declare_statement(XX_T_TYPE_DOUBLE, yymsp[-1].minor.yy107, status->scanner_state);
  yy_destructor(41,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3661 "parser.c"
        break;
      case 158:
// 1501 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_declare_statement(XX_T_TYPE_STRING, yymsp[-1].minor.yy107, status->scanner_state);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3670 "parser.c"
        break;
      case 159:
// 1505 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_declare_statement(XX_T_TYPE_BOOL, yymsp[-1].minor.yy107, status->scanner_state);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3679 "parser.c"
        break;
      case 160:
// 1509 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_declare_statement(XX_T_TYPE_VAR, yymsp[-1].minor.yy107, status->scanner_state);
  yy_destructor(45,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3688 "parser.c"
        break;
      case 163:
// 1521 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_declare_variable(yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3695 "parser.c"
        break;
      case 164:
// 1525 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_declare_variable(yymsp[-2].minor.yy0, yymsp[0].minor.yy107, status->scanner_state);
  yy_destructor(35,&yymsp[-1].minor);
}
// 3703 "parser.c"
        break;
      case 166:
// 1533 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_expr("not", yymsp[0].minor.yy107, NULL, NULL, status->scanner_state);
  yy_destructor(24,&yymsp[-1].minor);
}
// 3711 "parser.c"
        break;
      case 167:
// 1537 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_expr("isset", yymsp[0].minor.yy107, NULL, NULL, status->scanner_state);
  yy_destructor(19,&yymsp[-1].minor);
}
// 3719 "parser.c"
        break;
      case 168:
// 1541 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_expr("equals", yymsp[-2].minor.yy107, yymsp[0].minor.yy107, NULL, status->scanner_state);
  yy_destructor(8,&yymsp[-1].minor);
}
// 3727 "parser.c"
        break;
      case 169:
// 1545 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_expr("not-equals", yymsp[-2].minor.yy107, yymsp[0].minor.yy107, NULL, status->scanner_state);
  yy_destructor(15,&yymsp[-1].minor);
}
// 3735 "parser.c"
        break;
      case 170:
// 1549 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_expr("identical", yymsp[-2].minor.yy107, yymsp[0].minor.yy107, NULL, status->scanner_state);
  yy_destructor(9,&yymsp[-1].minor);
}
// 3743 "parser.c"
        break;
      case 171:
// 1553 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_expr("not-identical", yymsp[-2].minor.yy107, yymsp[0].minor.yy107, NULL, status->scanner_state);
  yy_destructor(14,&yymsp[-1].minor);
}
// 3751 "parser.c"
        break;
      case 172:
// 1557 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_expr("less", yymsp[-2].minor.yy107, yymsp[0].minor.yy107, NULL, status->scanner_state);
  yy_destructor(10,&yymsp[-1].minor);
}
// 3759 "parser.c"
        break;
      case 173:
// 1561 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_expr("greater", yymsp[-2].minor.yy107, yymsp[0].minor.yy107, NULL, status->scanner_state);
  yy_destructor(11,&yymsp[-1].minor);
}
// 3767 "parser.c"
        break;
      case 174:
// 1565 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_expr("less-equal", yymsp[-2].minor.yy107, yymsp[0].minor.yy107, NULL, status->scanner_state);
  yy_destructor(12,&yymsp[-1].minor);
}
// 3775 "parser.c"
        break;
      case 175:
// 1569 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_expr("greater-equal", yymsp[-2].minor.yy107, yymsp[0].minor.yy107, NULL, status->scanner_state);
  yy_destructor(13,&yymsp[-1].minor);
}
// 3783 "parser.c"
        break;
      case 176:
// 1573 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_expr("list", yymsp[-1].minor.yy107, NULL, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(25,&yymsp[0].minor);
}
// 3792 "parser.c"
        break;
      case 177:
// 1577 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_expr("cast", yymsp[-2].minor.yy107, yymsp[0].minor.yy107, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(25,&yymsp[-1].minor);
}
// 3801 "parser.c"
        break;
      case 178:
// 1581 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_expr("property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(65,&yymsp[-1].minor);
}
// 3809 "parser.c"
        break;
      case 179:
      case 229:
// 1585 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(66,&yymsp[-2].minor);
  yy_destructor(67,&yymsp[0].minor);
}
// 3819 "parser.c"
        break;
      case 180:
      case 230:
// 1590 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_INTEGER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(66,&yymsp[-2].minor);
  yy_destructor(67,&yymsp[0].minor);
}
// 3829 "parser.c"
        break;
      case 181:
      case 231:
// 1595 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_STRING, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(66,&yymsp[-2].minor);
  yy_destructor(67,&yymsp[0].minor);
}
// 3839 "parser.c"
        break;
      case 182:
// 1600 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_expr("add", yymsp[-2].minor.yy107, yymsp[0].minor.yy107, NULL, status->scanner_state);
  yy_destructor(16,&yymsp[-1].minor);
}
// 3847 "parser.c"
        break;
      case 183:
// 1605 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_expr("sub", yymsp[-2].minor.yy107, yymsp[0].minor.yy107, NULL, status->scanner_state);
  yy_destructor(17,&yymsp[-1].minor);
}
// 3855 "parser.c"
        break;
      case 184:
// 1610 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_expr("concat", yymsp[-2].minor.yy107, yymsp[0].minor.yy107, NULL, status->scanner_state);
  yy_destructor(18,&yymsp[-1].minor);
}
// 3863 "parser.c"
        break;
      case 185:
// 1615 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_expr("and", yymsp[-2].minor.yy107, yymsp[0].minor.yy107, NULL, status->scanner_state);
  yy_destructor(6,&yymsp[-1].minor);
}
// 3871 "parser.c"
        break;
      case 186:
// 1620 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_expr("or", yymsp[-2].minor.yy107, yymsp[0].minor.yy107, NULL, status->scanner_state);
  yy_destructor(7,&yymsp[-1].minor);
}
// 3879 "parser.c"
        break;
      case 187:
// 1625 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_expr("instanceof", yymsp[0].minor.yy107, NULL, NULL, status->scanner_state);
  yy_destructor(22,&yymsp[-1].minor);
}
// 3887 "parser.c"
        break;
      case 188:
// 1630 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_expr("fetch", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy107, NULL, status->scanner_state);
  yy_destructor(20,&yymsp[-3].minor);
  yy_destructor(5,&yymsp[-1].minor);
}
// 3896 "parser.c"
        break;
      case 189:
// 1635 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_expr("typeof", yymsp[0].minor.yy107, NULL, NULL, status->scanner_state);
  yy_destructor(21,&yymsp[-1].minor);
}
// 3904 "parser.c"
        break;
      case 190:
      case 218:
      case 222:
// 1640 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state);
}
// 3913 "parser.c"
        break;
      case 191:
      case 220:
      case 223:
// 1645 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_literal(XX_T_INTEGER, yymsp[0].minor.yy0, status->scanner_state);
}
// 3922 "parser.c"
        break;
      case 192:
      case 219:
      case 224:
// 1650 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_literal(XX_T_STRING, yymsp[0].minor.yy0, status->scanner_state);
}
// 3931 "parser.c"
        break;
      case 193:
// 1655 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_literal(XX_T_SCHAR, yymsp[0].minor.yy0, status->scanner_state);
}
// 3938 "parser.c"
        break;
      case 194:
      case 225:
// 1660 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_literal(XX_T_DOUBLE, yymsp[0].minor.yy0, status->scanner_state);
}
// 3946 "parser.c"
        break;
      case 195:
      case 226:
// 1665 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_literal(XX_T_NULL, NULL, status->scanner_state);
  yy_destructor(83,&yymsp[0].minor);
}
// 3955 "parser.c"
        break;
      case 196:
      case 228:
// 1670 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_literal(XX_T_TRUE, NULL, status->scanner_state);
  yy_destructor(84,&yymsp[0].minor);
}
// 3964 "parser.c"
        break;
      case 197:
      case 227:
// 1675 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_literal(XX_T_FALSE, NULL, status->scanner_state);
  yy_destructor(85,&yymsp[0].minor);
}
// 3973 "parser.c"
        break;
      case 198:
// 1680 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_expr("empty-array", NULL, NULL, NULL, status->scanner_state);
  yy_destructor(66,&yymsp[-1].minor);
  yy_destructor(67,&yymsp[0].minor);
}
// 3982 "parser.c"
        break;
      case 199:
// 1685 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_expr("array", yymsp[-1].minor.yy107, NULL, NULL, status->scanner_state);
  yy_destructor(66,&yymsp[-2].minor);
  yy_destructor(67,&yymsp[0].minor);
}
// 3991 "parser.c"
        break;
      case 200:
// 1690 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_new_instance(yymsp[0].minor.yy0, NULL, status->scanner_state);
  yy_destructor(23,&yymsp[-1].minor);
}
// 3999 "parser.c"
        break;
      case 201:
// 1695 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_new_instance(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(23,&yymsp[-3].minor);
  yy_destructor(38,&yymsp[-1].minor);
  yy_destructor(25,&yymsp[0].minor);
}
// 4009 "parser.c"
        break;
      case 202:
// 1700 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_new_instance(yymsp[-3].minor.yy0, yymsp[-1].minor.yy107, status->scanner_state);
  yy_destructor(23,&yymsp[-4].minor);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(25,&yymsp[0].minor);
}
// 4019 "parser.c"
        break;
      case 203:
// 1705 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_fcall(yymsp[-3].minor.yy0, yymsp[-1].minor.yy107, status->scanner_state);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(25,&yymsp[0].minor);
}
// 4028 "parser.c"
        break;
      case 204:
// 1710 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_fcall(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-1].minor);
  yy_destructor(25,&yymsp[0].minor);
}
// 4037 "parser.c"
        break;
      case 207:
// 1725 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_scall(yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy107, status->scanner_state);
  yy_destructor(86,&yymsp[-4].minor);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(25,&yymsp[0].minor);
}
// 4047 "parser.c"
        break;
      case 208:
// 1730 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_scall(yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(86,&yymsp[-3].minor);
  yy_destructor(38,&yymsp[-1].minor);
  yy_destructor(25,&yymsp[0].minor);
}
// 4057 "parser.c"
        break;
      case 209:
// 1735 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_mcall(yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy107, status->scanner_state);
  yy_destructor(65,&yymsp[-4].minor);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(25,&yymsp[0].minor);
}
// 4067 "parser.c"
        break;
      case 210:
// 1740 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_mcall(yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(65,&yymsp[-3].minor);
  yy_destructor(38,&yymsp[-1].minor);
  yy_destructor(25,&yymsp[0].minor);
}
// 4077 "parser.c"
        break;
      case 216:
// 1764 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_array_item(yymsp[-2].minor.yy107, yymsp[0].minor.yy107, status->scanner_state);
  yy_destructor(52,&yymsp[-1].minor);
}
// 4085 "parser.c"
        break;
      case 217:
// 1768 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_array_item(NULL, yymsp[0].minor.yy107, status->scanner_state);
}
// 4092 "parser.c"
        break;
      case 233:
// 1835 "parser.lemon"
{
	yygotominor.yy107 = xx_ret_comment(yymsp[0].minor.yy0, status->scanner_state);
}
// 4099 "parser.c"
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

// 4175 "parser.c"
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
