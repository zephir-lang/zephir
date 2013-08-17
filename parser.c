/* Driver template for the LEMON parser generator.
** The author disclaims copyright to this source code.
*/
/* First off, code is include which follows the "include" declaration
** in the input file. */
#include <stdio.h>
// 43 "parser.lemon"


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
		case XX_TYPE_DOUBLE:
			return json_object_new_string("double");
		case XX_TYPE_BOOL:
			return json_object_new_string("bool");
		case XX_TYPE_STRING:
			return json_object_new_string("string");
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


// 704 "parser.c"
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
#define YYNOCODE 147
#define YYACTIONTYPE unsigned short int
#define xx_TOKENTYPE xx_parser_token*
typedef union {
  xx_TOKENTYPE yy0;
  json_object* yy279;
  int yy293;
} YYMINORTYPE;
#define YYSTACKDEPTH 100
#define xx_ARG_SDECL xx_parser_status *status;
#define xx_ARG_PDECL ,xx_parser_status *status
#define xx_ARG_FETCH xx_parser_status *status = yypParser->status
#define xx_ARG_STORE yypParser->status = status
#define YYNSTATE 465
#define YYNRULE 228
#define YYERRORSYMBOL 85
#define YYERRSYMDT yy293
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
 /*     0 */   301,   18,   19,  200,  335,  382,  433,  304,   11,  427,
 /*    10 */   428,  429,  430,  434,  265,  289,  295,  292,  254,  298,
 /*    20 */    44,   46,   48,   12,   57,  445,  169,  170,   68,   72,
 /*    30 */    77,  140,  193,   84,  171,  172,  174,  173,  482,  255,
 /*    40 */    60,  414,  194,  408,  229,  238,  241,  395,  115,  256,
 /*    50 */   274,  277,  280,  283,  286,  301,  187,  389,  390,   27,
 /*    60 */   391,   64,  304,  326,  427,  428,  429,  430,  258,  265,
 /*    70 */   289,  295,  292,  143,  298,   44,   46,   48,  133,   57,
 /*    80 */   393,  187,  190,   68,   72,   77,  187,  189,   84,  196,
 /*    90 */   198,  167,  519,  463,  367,  186,  414,  382,  408,  229,
 /*   100 */   238,  241,  395,  131,  256,  274,  277,  280,  283,  286,
 /*   110 */   301,  187,   49,  520,  330,  324,  394,  304,  147,  427,
 /*   120 */   428,  429,  430,  187,  265,  289,  295,  292,  202,  298,
 /*   130 */    44,   46,   48,  160,   57,  179,  164,  175,   68,   72,
 /*   140 */    77,  337,    8,   84,  365,  348,  352,   73,   15,  520,
 /*   150 */   324,  414,  394,  408,  229,  238,  241,  395,  187,  256,
 /*   160 */   274,  277,  280,  283,  286,  301,  344,  348,  352,  317,
 /*   170 */   518,  386,  304,  387,  427,  428,  429,  430,  107,  265,
 /*   180 */   289,  295,  292,  234,  298,   44,   46,   48,   98,   57,
 /*   190 */   233,  187,  225,   68,   72,   77,  187,  415,   84,  426,
 /*   200 */    62,  260,  367,  450,  518,  382,  414,  387,  408,  229,
 /*   210 */   238,  241,  395,  148,  256,  274,  277,  280,  283,  286,
 /*   220 */   301,   64,  261,  326,  373,  109,  111,  304,  105,  427,
 /*   230 */   428,  429,  430,  247,  265,  289,  295,  292,  143,  298,
 /*   240 */    44,   46,   48,  121,   57,  264,  187,  262,   68,   72,
 /*   250 */    77,  187,  189,   84,    3,    4,    5,    6,  481,  176,
 /*   260 */   314,  414,  149,  408,  229,  238,  241,  395,  187,  256,
 /*   270 */   274,  277,  280,  283,  286,  301,  296,  273,  399,   71,
 /*   280 */   250,  253,  304,  400,  401,   19,  417,  167,  382,  265,
 /*   290 */   289,  295,  292,  418,  298,   44,   46,   48,   78,   57,
 /*   300 */   159,  268,  343,   68,   72,   77,  178,  177,   84,  176,
 /*   310 */   427,  428,  429,  430,  171,  172,  174,  173,   45,  229,
 /*   320 */   238,  241,  269,  297,  256,  274,  277,  280,  283,  286,
 /*   330 */   301,  163,  164,  175,  340,  377,  454,  304,  458,  383,
 /*   340 */   385,  388,  392,  272,  265,  289,  295,  292,  395,  298,
 /*   350 */    44,   46,   48,  129,   57,  270,  178,  177,   68,   72,
 /*   360 */    77,  187,  117,   84,  171,  172,  174,  173,  485,  232,
 /*   370 */   187,  402,   20,  408,  229,  238,  241,  395,  127,  256,
 /*   380 */   274,  277,  280,  283,  286,  301,  187,   58,  268,  342,
 /*   390 */   324,  231,  304,   21,  427,  428,  429,  430,  187,  265,
 /*   400 */   289,  295,  292,  143,  298,   44,   46,   48,  384,   57,
 /*   410 */   300,  187,  184,   68,   72,   77,  376,  189,   84,  365,
 /*   420 */   348,  352,  271,  457,  299,  273,  414,  465,  408,  229,
 /*   430 */   238,  241,  395,  167,  256,  274,  277,  280,  283,  286,
 /*   440 */   301,  517,   85,  228,  310,  125,   47,  304,   89,  383,
 /*   450 */   385,  388,  392,  187,  265,  289,  295,  292,  143,  298,
 /*   460 */    44,   46,   48,  143,   57,   22,  187,  201,   68,   72,
 /*   470 */    77,  187,  189,   84,  126,  128,  130,  188,  489,  166,
 /*   480 */   119,  366,  243,  246,  229,  238,  241,  395,  187,  256,
 /*   490 */   274,  277,  280,  283,  286,  301,  396,  165,  397,  331,
 /*   500 */   307,  162,  304,   88,  427,  428,  429,  430,  187,  265,
 /*   510 */   289,  295,  292,  123,  298,   44,   46,   48,   98,   57,
 /*   520 */   343,  187,   97,   68,   72,   77,  187,  113,   84,  259,
 /*   530 */    98,   96,  257,  486,  221,  187,  441,   50,  187,  229,
 /*   540 */   238,  241,  395,  302,  256,  274,  277,  280,  283,  286,
 /*   550 */   301,  281,  273,  338,  379,  268,  142,  304,  303,  383,
 /*   560 */   385,  388,  392,  161,  265,  289,  295,  292,  217,  298,
 /*   570 */    44,   46,   48,  222,   57,  186,  187,  282,   68,   72,
 /*   580 */    77,   98,   53,   84,  313,  208,   61,  329,  484,  187,
 /*   590 */   145,  366,  278,  273,  229,  238,  241,  395,  187,  256,
 /*   600 */   274,  277,  280,  283,  286,  301,   54,  186,  185,   52,
 /*   610 */   101,   14,  304,   24,  383,  385,  388,  392,  187,  265,
 /*   620 */   289,  295,  292,  268,  298,   44,   46,   48,   98,   57,
 /*   630 */   186,   87,  216,   68,   72,   77,  187,  312,   84,  217,
 /*   640 */   263,  677,  311,  488,  213,  279,  366,  187,  678,  229,
 /*   650 */   238,  241,  395,   86,  256,  274,  277,  280,  283,  286,
 /*   660 */   301,  284,  273,  191,  325,  234,  679,  304,  266,  273,
 /*   670 */   168,  230,  235,  187,  265,  289,  295,  292,  181,  298,
 /*   680 */    44,   46,   48,   98,   57,  268,  182,  212,   68,   72,
 /*   690 */    77,  187,  327,   84,   79,  204,  694,    1,    2,  464,
 /*   700 */     4,    5,    6,  187,  229,  238,  241,  285,  137,  256,
 /*   710 */   274,  277,  280,  283,  286,  301,  187,  169,  170,   56,
 /*   720 */   287,  273,  304,  332,   90,  171,  172,  174,  173,  265,
 /*   730 */   289,  295,  292,  319,  298,   44,   46,   48,  333,   57,
 /*   740 */   104,  187,   59,   68,   72,   77,  275,  273,   84,  135,
 /*   750 */    91,   92,   93,   94,   95,    7,  209,  187,   10,  229,
 /*   760 */   238,  241,  451,  304,  256,  274,  277,  280,  283,  286,
 /*   770 */   301,  268,  339,  268,  375,  268,  106,  304,  108,  383,
 /*   780 */   385,  388,  392,  346,  265,  289,  295,  292,  268,  298,
 /*   790 */    44,   46,   48,  288,   57,  276,   80,  267,   68,   72,
 /*   800 */    77,  290,  273,   84,  187,  151,  268,  347,  487,    9,
 /*   810 */   291,  366,  349,  187,  229,  238,  241,  395,  350,  256,
 /*   820 */   274,  277,  280,  283,  286,  301,  293,  273,  294,   25,
 /*   830 */   239,  110,  304,  351,  383,  385,  388,  392,  187,  265,
 /*   840 */   289,  295,  292,   63,  298,   44,   46,   48,  353,   57,
 /*   850 */   354,  355,  195,   68,   72,   77,  112,  357,   84,  358,
 /*   860 */   539,  197,  540,  483,  541,  542,  366,  543,  544,  229,
 /*   870 */   238,  241,  395,  199,  256,  274,  277,  280,  283,  286,
 /*   880 */   301,  368,  545,  369,  336,  207,  370,  304,  372,   90,
 /*   890 */    65,  378,   66,  206,  265,  289,  295,  292,  252,  298,
 /*   900 */    44,   46,   48,  211,   57,  516,  522,  215,   68,   72,
 /*   910 */    77,  524,  528,   84,  521,   91,   92,   93,   94,   95,
 /*   920 */   383,  385,  388,  392,  229,  238,  241,  523,  525,  256,
 /*   930 */   274,  277,  280,  283,  286,  301,  526,  530,  527,   83,
 /*   940 */   214,  529,  304,  404,  383,  385,  388,  392,  405,  265,
 /*   950 */   289,  295,  292,  407,  298,   44,   46,   48,  395,   57,
 /*   960 */   406,  220,  409,   68,   72,   77,  410,  411,   84,  412,
 /*   970 */   422,  423,  424,  425,  223,  224,  420,  421,  403,  229,
 /*   980 */   238,  241,  395,  419,  256,  274,  277,  280,  283,  286,
 /*   990 */   301,   69,  517,  519,  322,  416,  435,  304,  436,  427,
 /*  1000 */   428,  429,  430,  237,  265,  289,  295,  292,  446,  298,
 /*  1010 */    44,   46,   48,  447,   57,  449,  242,  452,   68,   72,
 /*  1020 */    77,  244,  453,   84,  245,  422,  423,  424,  425,  456,
 /*  1030 */   459,  460,  248,  403,  229,  238,  241,  395,  462,  256,
 /*  1040 */   274,  277,  280,  283,  286,  301,   74,  249,  141,  323,
 /*  1050 */   432,  466,  304,  251,  376,  376,  376,  376,  376,  265,
 /*  1060 */   289,  295,  292,  376,  298,   44,   46,   48,  376,   57,
 /*  1070 */   376,  376,  376,   68,   72,   77,    7,  376,   84,   10,
 /*  1080 */   376,  376,  376,  451,  304,  376,  376,  376,  376,  229,
 /*  1090 */   238,  241,  376,  376,  256,  274,  277,  280,  283,  286,
 /*  1100 */   301,  376,  376,  376,   76,  376,  376,  304,  376,  376,
 /*  1110 */   376,  376,  376,  376,  265,  289,  295,  292,  376,  298,
 /*  1120 */    44,   46,   48,  376,   57,  376,  376,  376,   68,   72,
 /*  1130 */    77,  376,  376,   84,  376,  376,  376,  376,  376,  376,
 /*  1140 */   376,  376,  376,  376,  229,  238,  241,  376,  376,  256,
 /*  1150 */   274,  277,  280,  283,  286,  301,  376,  376,  376,  381,
 /*  1160 */   376,  376,  304,  376,  376,  376,  376,  376,  376,  265,
 /*  1170 */   289,  295,  292,  376,  298,   44,   46,   48,  376,   57,
 /*  1180 */   376,  376,  376,   68,   72,   77,  376,  376,   84,  376,
 /*  1190 */   376,  376,  376,  376,  376,  376,  301,  376,  376,  229,
 /*  1200 */   238,  241,  376,  304,  256,  274,  277,  280,  283,  286,
 /*  1210 */   265,  289,  295,  292,  376,  298,   44,   46,   48,  376,
 /*  1220 */    57,  376,  376,  376,   68,   72,   77,  376,  376,   84,
 /*  1230 */   376,  376,  376,  376,  376,  376,  376,  376,   43,  376,
 /*  1240 */   229,  238,  241,  376,  376,  256,  274,  277,  280,  283,
 /*  1250 */   286,   70,  376,  376,  376,  376,  305,   29,   30,   31,
 /*  1260 */    32,   33,   34,   35,   36,   37,   38,   39,   40,   41,
 /*  1270 */    42,  439,   43,  442,  443,  431,  376,  426,  400,  401,
 /*  1280 */    19,  376,  236,  382,  376,   51,  376,  376,  376,  376,
 /*  1290 */   305,   29,   30,   31,   32,   33,   34,   35,   36,   37,
 /*  1300 */    38,   39,   40,   41,   42,   43,  398,   17,  413,  431,
 /*  1310 */   376,  426,  437,  401,   19,  376,  236,  382,  321,  376,
 /*  1320 */   376,  376,  376,  305,   29,   30,   31,   32,   33,   34,
 /*  1330 */    35,   36,   37,   38,   39,   40,   41,   42,   43,  376,
 /*  1340 */   376,  376,  376,  376,  376,  376,  376,  376,  376,  236,
 /*  1350 */   376,  328,  376,  376,  376,  376,  305,   29,   30,   31,
 /*  1360 */    32,   33,   34,   35,   36,   37,   38,   39,   40,   41,
 /*  1370 */    42,  376,  376,  376,  376,   43,  376,  440,  413,  431,
 /*  1380 */   376,  426,  236,  401,   19,  376,   90,  382,   67,  376,
 /*  1390 */   376,  376,  376,  305,   29,   30,   31,   32,   33,   34,
 /*  1400 */    35,   36,   37,   38,   39,   40,   41,   42,  376,   43,
 /*  1410 */   376,  376,   91,   92,   93,   94,   95,  205,  218,  236,
 /*  1420 */   226,  227,  309,  376,  376,  376,  376,  305,   29,   30,
 /*  1430 */    31,   32,   33,   34,   35,   36,   37,   38,   39,   40,
 /*  1440 */    41,   42,   43,  376,  376,  376,  376,  376,  376,  376,
 /*  1450 */   376,  376,  376,  236,  376,  374,  376,  376,  376,  376,
 /*  1460 */   305,   29,   30,   31,   32,   33,   34,   35,   36,   37,
 /*  1470 */    38,   39,   40,   41,   42,   43,  376,  376,  376,  376,
 /*  1480 */   376,  376,  376,  376,  376,  376,  236,  376,   26,  376,
 /*  1490 */   376,  376,  376,  305,   29,   30,   31,   32,   33,   34,
 /*  1500 */    35,   36,   37,   38,   39,   40,   41,   42,  376,  376,
 /*  1510 */   376,  376,   43,  376,  376,  376,  376,  376,  376,  236,
 /*  1520 */   376,  376,  376,  376,  376,  341,  376,  376,  376,  376,
 /*  1530 */   305,   29,   30,   31,   32,   33,   34,   35,   36,   37,
 /*  1540 */    38,   39,   40,   41,   42,  376,   43,  376,  376,  376,
 /*  1550 */   376,  376,  376,  376,  376,  376,  236,  376,  376,   82,
 /*  1560 */   376,  376,  376,  376,  305,   29,   30,   31,   32,   33,
 /*  1570 */    34,   35,   36,   37,   38,   39,   40,   41,   42,   43,
 /*  1580 */   376,  376,  376,  376,  376,  376,  376,  376,  376,  376,
 /*  1590 */   236,  376,  334,  376,  376,  376,  376,  305,   29,   30,
 /*  1600 */    31,   32,   33,   34,   35,   36,   37,   38,   39,   40,
 /*  1610 */    41,   42,   43,  376,  376,  376,  376,  376,  376,  376,
 /*  1620 */   376,  376,  376,  236,  376,   55,  376,  376,  376,  376,
 /*  1630 */   305,   29,   30,   31,   32,   33,   34,   35,   36,   37,
 /*  1640 */    38,   39,   40,   41,   42,  376,  376,  376,  376,   43,
 /*  1650 */   376,  376,  376,  376,  376,  376,  236,  376,  376,  376,
 /*  1660 */   376,  376,  380,  376,  376,  376,  376,  305,   29,   30,
 /*  1670 */    31,   32,   33,   34,   35,   36,   37,   38,   39,   40,
 /*  1680 */    41,   42,  376,   43,  376,  376,  376,  376,  376,  376,
 /*  1690 */   376,  376,  376,  236,  376,  376,   75,  376,  376,  376,
 /*  1700 */   376,  305,   29,   30,   31,   32,   33,   34,   35,   36,
 /*  1710 */    37,   38,   39,   40,   41,   42,   43,  376,  376,  376,
 /*  1720 */   376,  376,  376,  376,  376,  376,  376,  236,  376,  316,
 /*  1730 */   376,  376,  376,  376,  305,   29,   30,   31,   32,   33,
 /*  1740 */    34,   35,   36,   37,   38,   39,   40,   41,   42,   43,
 /*  1750 */   376,  376,  376,  376,  376,  376,  376,  376,  376,  376,
 /*  1760 */   236,  376,  376,  376,  376,  376,  376,   28,   29,   30,
 /*  1770 */    31,   32,   33,   34,   35,   36,   37,   38,   39,   40,
 /*  1780 */    41,   42,  376,  376,  376,  376,  376,  103,  146,  150,
 /*  1790 */   144,  180,  100,  236,  139,  376,  376,  376,  356,  103,
 /*  1800 */   146,  150,  144,  180,  100,  136,  139,  376,  376,  376,
 /*  1810 */   376,  376,  376,  376,  345,  376,  376,  136,  376,  103,
 /*  1820 */   146,  150,  144,  180,  100,  376,  139,  376,  359,  360,
 /*  1830 */   361,  362,  363,  364,  158,  210,  376,  136,  318,  376,
 /*  1840 */   376,  152,  153,  376,  376,  376,  158,  376,  376,  154,
 /*  1850 */   155,  156,  157,  152,  153,  376,  376,  376,  306,  376,
 /*  1860 */   376,  154,  155,  156,  157,  376,  158,  376,  376,  376,
 /*  1870 */   376,  376,  376,  152,  153,  376,  376,  376,  376,  376,
 /*  1880 */   376,  154,  155,  156,  157,  376,  103,  146,  150,  144,
 /*  1890 */   180,  100,  376,  139,  376,  376,  376,  376,  376,  376,
 /*  1900 */   376,  376,  376,  376,  136,  183,  103,  146,  150,  144,
 /*  1910 */   180,  100,  376,  139,  376,  376,  376,  376,  376,  376,
 /*  1920 */   376,  376,  376,  376,  136,  192,  376,  103,  146,  150,
 /*  1930 */   144,  180,  100,  158,  139,  376,  376,  376,  376,  376,
 /*  1940 */   152,  153,  376,  376,  376,  136,  203,  376,  154,  155,
 /*  1950 */   156,  157,  376,  158,  376,  376,  376,  376,  376,  376,
 /*  1960 */   152,  153,  376,  376,  376,  376,  376,  376,  154,  155,
 /*  1970 */   156,  157,  356,  376,  158,  376,  376,  376,  376,  376,
 /*  1980 */   376,  152,  153,  376,  376,  376,  376,  376,  345,  154,
 /*  1990 */   155,  156,  157,  376,  103,  146,  150,  144,  180,  100,
 /*  2000 */    23,  139,  359,  360,  361,  362,  363,  364,  376,  376,
 /*  2010 */   376,  376,  136,  376,  103,  146,  150,  144,  180,  100,
 /*  2020 */   376,  139,  376,  376,  376,  376,  448,   16,  438,  444,
 /*  2030 */   443,  431,  136,  426,  437,  401,   19,  376,  376,  382,
 /*  2040 */   376,  158,  219,  376,  376,  376,  376,  376,  152,  153,
 /*  2050 */   376,  376,  376,  376,  376,  376,  154,  155,  156,  157,
 /*  2060 */   376,  158,  376,  376,  376,  376,  376,  376,  152,  153,
 /*  2070 */   376,  376,  376,  376,  376,  376,  154,  155,  156,  157,
 /*  2080 */   376,  376,  376,  132,  134,   99,  114,  118,  120,  122,
 /*  2090 */   124,  116,  102,  126,  128,  130,  376,  376,  376,  376,
 /*  2100 */   376,  376,  376,  376,  376,  376,  308,  376,  376,  376,
 /*  2110 */   376,  376,  376,  132,  134,   99,  114,  118,  120,  122,
 /*  2120 */   124,  116,  102,  126,  128,  130,  376,  376,  376,  376,
 /*  2130 */   376,  376,  376,  376,  376,  376,  315,  376,  376,  376,
 /*  2140 */   376,  376,  376,  132,  134,   99,  114,  118,  120,  122,
 /*  2150 */   124,  116,  102,  126,  128,  130,  376,  376,  376,  376,
 /*  2160 */   376,  376,  376,  376,  240,  376,  376,  376,  376,  376,
 /*  2170 */   376,  132,  134,   99,  114,  118,  120,  122,  124,  116,
 /*  2180 */   102,  126,  128,  130,  376,  376,  376,  376,  376,  376,
 /*  2190 */   376,  376,  376,  376,  320,  376,  376,  376,  376,  376,
 /*  2200 */   376,  132,  134,   99,  114,  118,  120,  122,  124,  116,
 /*  2210 */   102,  126,  128,  130,  132,  134,   99,  114,  118,  120,
 /*  2220 */   122,  124,  116,  102,  126,  128,  130,  132,  134,   99,
 /*  2230 */   114,  118,  120,  122,  124,  116,  102,  126,  128,  130,
 /*  2240 */   376,  376,  376,  376,  376,  376,  138,  376,  376,  376,
 /*  2250 */   691,  376,  376,  376,  376,  376,  376,  376,  376,  376,
 /*  2260 */   376,  376,  602,  376,  376,  376,  376,  376,  376,  132,
 /*  2270 */   134,   99,  114,  118,  120,  122,  124,  116,  102,  126,
 /*  2280 */   128,  130,  376,  376,  376,  376,  376,  376,  376,  376,
 /*  2290 */   376,  376,   81,  376,  376,  376,  376,  376,  376,  132,
 /*  2300 */   134,   99,  114,  118,  120,  122,  124,  116,  102,  126,
 /*  2310 */   128,  130,   99,  114,  118,  120,  122,  124,  116,  102,
 /*  2320 */   126,  128,  130,  455,   16,  438,  444,  443,  431,  376,
 /*  2330 */   426,  437,  401,   19,  376,  376,  382,  376,  376,  376,
 /*  2340 */   461,   16,  438,  444,  443,  431,  376,  426,  437,  401,
 /*  2350 */    19,  376,  376,  382,  376,  376,  376,  376,  356,  376,
 /*  2360 */   376,  376,  376,   13,   16,  438,  444,  443,  431,  376,
 /*  2370 */   426,  437,  401,   19,  345,  376,  382,  376,  376,  376,
 /*  2380 */   376,  376,  376,  376,  376,  376,  371,  376,  359,  360,
 /*  2390 */   361,  362,  363,  364,
};
static YYCODETYPE yy_lookahead[] = {
 /*     0 */    26,  102,  103,   37,   30,  106,   27,   33,   26,    1,
 /*    10 */     2,    3,    4,   34,   40,   41,   42,   43,   67,   45,
 /*    20 */    46,   47,   48,   29,   50,   31,   73,   74,   54,   55,
 /*    30 */    56,   65,   66,   59,   81,   82,   83,   84,   30,   27,
 /*    40 */    30,   33,   26,   35,   70,   71,   72,   39,  128,   75,
 /*    50 */    76,   77,   78,   79,   80,   26,  136,    1,    2,   30,
 /*    60 */     4,   51,   33,   53,    1,    2,    3,    4,   27,   40,
 /*    70 */    41,   42,   43,  128,   45,   46,   47,   48,  128,   50,
 /*    80 */     3,  136,  137,   54,   55,   56,  136,  142,   59,   73,
 /*    90 */    74,   99,   36,   30,  103,    5,   33,  106,   35,   70,
 /*   100 */    71,   72,   39,  128,   75,   76,   77,   78,   79,   80,
 /*   110 */    26,  136,  125,   36,   30,  128,   39,   33,   26,    1,
 /*   120 */     2,    3,    4,  136,   40,   41,   42,   43,   38,   45,
 /*   130 */    46,   47,   48,  141,   50,  143,  144,  145,   54,   55,
 /*   140 */    56,  104,   26,   59,  107,  108,  109,  125,   30,   36,
 /*   150 */   128,   33,   39,   35,   70,   71,   72,   39,  136,   75,
 /*   160 */    76,   77,   78,   79,   80,   26,  107,  108,  109,   30,
 /*   170 */    36,    3,   33,   39,    1,    2,    3,    4,   26,   40,
 /*   180 */    41,   42,   43,  128,   45,   46,   47,   48,  128,   50,
 /*   190 */   135,  136,  132,   54,   55,   56,  136,   98,   59,  100,
 /*   200 */    30,   26,  103,   30,   36,  106,   33,   39,   35,   70,
 /*   210 */    71,   72,   39,    5,   75,   76,   77,   78,   79,   80,
 /*   220 */    26,   51,   37,   53,   30,   73,   74,   33,   26,    1,
 /*   230 */     2,    3,    4,   26,   40,   41,   42,   43,  128,   45,
 /*   240 */    46,   47,   48,  128,   50,   27,  136,  137,   54,   55,
 /*   250 */    56,  136,  142,   59,   89,   90,   91,   92,   30,   26,
 /*   260 */   128,   33,  140,   35,   70,   71,   72,   39,  136,   75,
 /*   270 */    76,   77,   78,   79,   80,   26,  138,  139,   96,   30,
 /*   280 */    73,   74,   33,  101,  102,  103,   27,   99,  106,   40,
 /*   290 */    41,   42,   43,   34,   45,   46,   47,   48,   26,   50,
 /*   300 */    67,    5,    5,   54,   55,   56,   73,   74,   59,   26,
 /*   310 */     1,    2,    3,    4,   81,   82,   83,   84,   27,   70,
 /*   320 */    71,   72,  139,   27,   75,   76,   77,   78,   79,   80,
 /*   330 */    26,  143,  144,  145,   30,   38,   29,   33,   31,    1,
 /*   340 */     2,    3,    4,   99,   40,   41,   42,   43,   39,   45,
 /*   350 */    46,   47,   48,  128,   50,   26,   73,   74,   54,   55,
 /*   360 */    56,  136,  128,   59,   81,   82,   83,   84,   30,    5,
 /*   370 */   136,   33,   36,   35,   70,   71,   72,   39,  128,   75,
 /*   380 */    76,   77,   78,   79,   80,   26,  136,  125,    5,   30,
 /*   390 */   128,   27,   33,   26,    1,    2,    3,    4,  136,   40,
 /*   400 */    41,   42,   43,  128,   45,   46,   47,   48,    3,   50,
 /*   410 */    27,  136,  137,   54,   55,   56,  104,  142,   59,  107,
 /*   420 */   108,  109,   34,   30,  138,  139,   33,    0,   35,   70,
 /*   430 */    71,   72,   39,   99,   75,   76,   77,   78,   79,   80,
 /*   440 */    26,   36,  129,  130,   30,  128,   27,   33,   26,    1,
 /*   450 */     2,    3,    4,  136,   40,   41,   42,   43,  128,   45,
 /*   460 */    46,   47,   48,  128,   50,   37,  136,  137,   54,   55,
 /*   470 */    56,  136,  142,   59,   16,   17,   18,  142,   30,  145,
 /*   480 */   128,   33,   65,   66,   70,   71,   72,   39,  136,   75,
 /*   490 */    76,   77,   78,   79,   80,   26,    2,   52,    4,   30,
 /*   500 */   128,    5,   33,  130,    1,    2,    3,    4,  136,   40,
 /*   510 */    41,   42,   43,  128,   45,   46,   47,   48,  128,   50,
 /*   520 */     5,  136,  132,   54,   55,   56,  136,  128,   59,   23,
 /*   530 */   128,  131,   26,   30,  132,  136,   33,   29,  136,   70,
 /*   540 */    71,   72,   39,   65,   75,   76,   77,   78,   79,   80,
 /*   550 */    26,  138,  139,   38,   30,    5,   37,   33,   26,    1,
 /*   560 */     2,    3,    4,   67,   40,   41,   42,   43,  128,   45,
 /*   570 */    46,   47,   48,  133,   50,    5,  136,   27,   54,   55,
 /*   580 */    56,  128,   49,   59,   57,  132,  126,  127,   30,  136,
 /*   590 */   128,   33,  138,  139,   70,   71,   72,   39,  136,   75,
 /*   600 */    76,   77,   78,   79,   80,   26,   29,    5,   38,   30,
 /*   610 */   128,   30,   33,   29,    1,    2,    3,    4,  136,   40,
 /*   620 */    41,   42,   43,    5,   45,   46,   47,   48,  128,   50,
 /*   630 */     5,    5,  132,   54,   55,   56,  136,   26,   59,  128,
 /*   640 */    38,   52,    5,   30,  133,   27,   33,  136,   52,   70,
 /*   650 */    71,   72,   39,   27,   75,   76,   77,   78,   79,   80,
 /*   660 */    26,  138,  139,   38,   30,  128,   52,   33,  138,  139,
 /*   670 */    26,  134,  135,  136,   40,   41,   42,   43,   26,   45,
 /*   680 */    46,   47,   48,  128,   50,    5,   37,  132,   54,   55,
 /*   690 */    56,  136,   52,   59,   57,  128,   86,   87,   88,   89,
 /*   700 */    90,   91,   92,  136,   70,   71,   72,   27,  128,   75,
 /*   710 */    76,   77,   78,   79,   80,   26,  136,   73,   74,   30,
 /*   720 */   138,  139,   33,   49,   34,   81,   82,   83,   84,   40,
 /*   730 */    41,   42,   43,  128,   45,   46,   47,   48,   29,   50,
 /*   740 */   140,  136,   29,   54,   55,   56,  138,  139,   59,  128,
 /*   750 */    60,   61,   62,   63,   64,   25,   66,  136,   28,   70,
 /*   760 */    71,   72,   32,   33,   75,   76,   77,   78,   79,   80,
 /*   770 */    26,    5,   29,    5,   30,    5,   66,   33,   67,    1,
 /*   780 */     2,    3,    4,   34,   40,   41,   42,   43,    5,   45,
 /*   790 */    46,   47,   48,   27,   50,   27,  128,   27,   54,   55,
 /*   800 */    56,  138,  139,   59,  136,  128,    5,   99,   30,   27,
 /*   810 */    27,   33,   26,  136,   70,   71,   72,   39,   34,   75,
 /*   820 */    76,   77,   78,   79,   80,   26,  138,  139,   27,   30,
 /*   830 */   128,   67,   33,   99,    1,    2,    3,    4,  136,   40,
 /*   840 */    41,   42,   43,  127,   45,   46,   47,   48,   26,   50,
 /*   850 */    34,   99,   67,   54,   55,   56,   67,   26,   59,   11,
 /*   860 */    26,   67,   26,   30,   26,   26,   33,   26,   26,   70,
 /*   870 */    71,   72,   39,   67,   75,   76,   77,   78,   79,   80,
 /*   880 */    26,   36,   26,   26,   30,  131,   37,   33,   29,   34,
 /*   890 */    99,   29,   52,   26,   40,   41,   42,   43,   27,   45,
 /*   900 */    46,   47,   48,  131,   50,   36,   36,  131,   54,   55,
 /*   910 */    56,   36,   36,   59,   36,   60,   61,   62,   63,   64,
 /*   920 */     1,    2,    3,    4,   70,   71,   72,   36,   36,   75,
 /*   930 */    76,   77,   78,   79,   80,   26,   36,   36,   36,   30,
 /*   940 */    67,   36,   33,   26,    1,    2,    3,    4,   34,   40,
 /*   950 */    41,   42,   43,   27,   45,   46,   47,   48,   39,   50,
 /*   960 */    99,  131,   26,   54,   55,   56,   34,   99,   59,   27,
 /*   970 */     1,    2,    3,    4,   67,  131,   27,  100,   35,   70,
 /*   980 */    71,   72,   39,   99,   75,   76,   77,   78,   79,   80,
 /*   990 */    26,   29,   36,   36,   30,   26,   99,   33,   27,    1,
 /*  1000 */     2,    3,    4,   27,   40,   41,   42,   43,   26,   45,
 /*  1010 */    46,   47,   48,   29,   50,   30,   26,   28,   54,   55,
 /*  1020 */    56,   26,   26,   59,   27,    1,    2,    3,    4,   30,
 /*  1030 */    26,   29,   67,   35,   70,   71,   72,   39,   30,   75,
 /*  1040 */    76,   77,   78,   79,   80,   26,   29,   27,   26,   30,
 /*  1050 */    26,    0,   33,   67,  146,  146,  146,  146,  146,   40,
 /*  1060 */    41,   42,   43,  146,   45,   46,   47,   48,  146,   50,
 /*  1070 */   146,  146,  146,   54,   55,   56,   25,  146,   59,   28,
 /*  1080 */   146,  146,  146,   32,   33,  146,  146,  146,  146,   70,
 /*  1090 */    71,   72,  146,  146,   75,   76,   77,   78,   79,   80,
 /*  1100 */    26,  146,  146,  146,   30,  146,  146,   33,  146,  146,
 /*  1110 */   146,  146,  146,  146,   40,   41,   42,   43,  146,   45,
 /*  1120 */    46,   47,   48,  146,   50,  146,  146,  146,   54,   55,
 /*  1130 */    56,  146,  146,   59,  146,  146,  146,  146,  146,  146,
 /*  1140 */   146,  146,  146,  146,   70,   71,   72,  146,  146,   75,
 /*  1150 */    76,   77,   78,   79,   80,   26,  146,  146,  146,   30,
 /*  1160 */   146,  146,   33,  146,  146,  146,  146,  146,  146,   40,
 /*  1170 */    41,   42,   43,  146,   45,   46,   47,   48,  146,   50,
 /*  1180 */   146,  146,  146,   54,   55,   56,  146,  146,   59,  146,
 /*  1190 */   146,  146,  146,  146,  146,  146,   26,  146,  146,   70,
 /*  1200 */    71,   72,  146,   33,   75,   76,   77,   78,   79,   80,
 /*  1210 */    40,   41,   42,   43,  146,   45,   46,   47,   48,  146,
 /*  1220 */    50,  146,  146,  146,   54,   55,   56,  146,  146,   59,
 /*  1230 */   146,  146,  146,  146,  146,  146,  146,  146,   92,  146,
 /*  1240 */    70,   71,   72,  146,  146,   75,   76,   77,   78,   79,
 /*  1250 */    80,  105,  146,  146,  146,  146,  110,  111,  112,  113,
 /*  1260 */   114,  115,  116,  117,  118,  119,  120,  121,  122,  123,
 /*  1270 */   124,   94,   92,   96,   97,   98,  146,  100,  101,  102,
 /*  1280 */   103,  146,  136,  106,  146,  105,  146,  146,  146,  146,
 /*  1290 */   110,  111,  112,  113,  114,  115,  116,  117,  118,  119,
 /*  1300 */   120,  121,  122,  123,  124,   92,   95,   96,   97,   98,
 /*  1310 */   146,  100,  101,  102,  103,  146,  136,  106,  105,  146,
 /*  1320 */   146,  146,  146,  110,  111,  112,  113,  114,  115,  116,
 /*  1330 */   117,  118,  119,  120,  121,  122,  123,  124,   92,  146,
 /*  1340 */   146,  146,  146,  146,  146,  146,  146,  146,  146,  136,
 /*  1350 */   146,  105,  146,  146,  146,  146,  110,  111,  112,  113,
 /*  1360 */   114,  115,  116,  117,  118,  119,  120,  121,  122,  123,
 /*  1370 */   124,  146,  146,  146,  146,   92,  146,   96,   97,   98,
 /*  1380 */   146,  100,  136,  102,  103,  146,   34,  106,  105,  146,
 /*  1390 */   146,  146,  146,  110,  111,  112,  113,  114,  115,  116,
 /*  1400 */   117,  118,  119,  120,  121,  122,  123,  124,  146,   92,
 /*  1410 */   146,  146,   60,   61,   62,   63,   64,   65,   66,  136,
 /*  1420 */    68,   69,  105,  146,  146,  146,  146,  110,  111,  112,
 /*  1430 */   113,  114,  115,  116,  117,  118,  119,  120,  121,  122,
 /*  1440 */   123,  124,   92,  146,  146,  146,  146,  146,  146,  146,
 /*  1450 */   146,  146,  146,  136,  146,  105,  146,  146,  146,  146,
 /*  1460 */   110,  111,  112,  113,  114,  115,  116,  117,  118,  119,
 /*  1470 */   120,  121,  122,  123,  124,   92,  146,  146,  146,  146,
 /*  1480 */   146,  146,  146,  146,  146,  146,  136,  146,  105,  146,
 /*  1490 */   146,  146,  146,  110,  111,  112,  113,  114,  115,  116,
 /*  1500 */   117,  118,  119,  120,  121,  122,  123,  124,  146,  146,
 /*  1510 */   146,  146,   92,  146,  146,  146,  146,  146,  146,  136,
 /*  1520 */   146,  146,  146,  146,  146,  105,  146,  146,  146,  146,
 /*  1530 */   110,  111,  112,  113,  114,  115,  116,  117,  118,  119,
 /*  1540 */   120,  121,  122,  123,  124,  146,   92,  146,  146,  146,
 /*  1550 */   146,  146,  146,  146,  146,  146,  136,  146,  146,  105,
 /*  1560 */   146,  146,  146,  146,  110,  111,  112,  113,  114,  115,
 /*  1570 */   116,  117,  118,  119,  120,  121,  122,  123,  124,   92,
 /*  1580 */   146,  146,  146,  146,  146,  146,  146,  146,  146,  146,
 /*  1590 */   136,  146,  105,  146,  146,  146,  146,  110,  111,  112,
 /*  1600 */   113,  114,  115,  116,  117,  118,  119,  120,  121,  122,
 /*  1610 */   123,  124,   92,  146,  146,  146,  146,  146,  146,  146,
 /*  1620 */   146,  146,  146,  136,  146,  105,  146,  146,  146,  146,
 /*  1630 */   110,  111,  112,  113,  114,  115,  116,  117,  118,  119,
 /*  1640 */   120,  121,  122,  123,  124,  146,  146,  146,  146,   92,
 /*  1650 */   146,  146,  146,  146,  146,  146,  136,  146,  146,  146,
 /*  1660 */   146,  146,  105,  146,  146,  146,  146,  110,  111,  112,
 /*  1670 */   113,  114,  115,  116,  117,  118,  119,  120,  121,  122,
 /*  1680 */   123,  124,  146,   92,  146,  146,  146,  146,  146,  146,
 /*  1690 */   146,  146,  146,  136,  146,  146,  105,  146,  146,  146,
 /*  1700 */   146,  110,  111,  112,  113,  114,  115,  116,  117,  118,
 /*  1710 */   119,  120,  121,  122,  123,  124,   92,  146,  146,  146,
 /*  1720 */   146,  146,  146,  146,  146,  146,  146,  136,  146,  105,
 /*  1730 */   146,  146,  146,  146,  110,  111,  112,  113,  114,  115,
 /*  1740 */   116,  117,  118,  119,  120,  121,  122,  123,  124,   92,
 /*  1750 */   146,  146,  146,  146,  146,  146,  146,  146,  146,  146,
 /*  1760 */   136,  146,  146,  146,  146,  146,  146,  110,  111,  112,
 /*  1770 */   113,  114,  115,  116,  117,  118,  119,  120,  121,  122,
 /*  1780 */   123,  124,  146,  146,  146,  146,  146,   19,   20,   21,
 /*  1790 */    22,   23,   24,  136,   26,  146,  146,  146,   10,   19,
 /*  1800 */    20,   21,   22,   23,   24,   37,   26,  146,  146,  146,
 /*  1810 */   146,  146,  146,  146,   26,  146,  146,   37,  146,   19,
 /*  1820 */    20,   21,   22,   23,   24,  146,   26,  146,   40,   41,
 /*  1830 */    42,   43,   44,   45,   66,   67,  146,   37,   58,  146,
 /*  1840 */   146,   73,   74,  146,  146,  146,   66,  146,  146,   81,
 /*  1850 */    82,   83,   84,   73,   74,  146,  146,  146,   58,  146,
 /*  1860 */   146,   81,   82,   83,   84,  146,   66,  146,  146,  146,
 /*  1870 */   146,  146,  146,   73,   74,  146,  146,  146,  146,  146,
 /*  1880 */   146,   81,   82,   83,   84,  146,   19,   20,   21,   22,
 /*  1890 */    23,   24,  146,   26,  146,  146,  146,  146,  146,  146,
 /*  1900 */   146,  146,  146,  146,   37,   38,   19,   20,   21,   22,
 /*  1910 */    23,   24,  146,   26,  146,  146,  146,  146,  146,  146,
 /*  1920 */   146,  146,  146,  146,   37,   38,  146,   19,   20,   21,
 /*  1930 */    22,   23,   24,   66,   26,  146,  146,  146,  146,  146,
 /*  1940 */    73,   74,  146,  146,  146,   37,   38,  146,   81,   82,
 /*  1950 */    83,   84,  146,   66,  146,  146,  146,  146,  146,  146,
 /*  1960 */    73,   74,  146,  146,  146,  146,  146,  146,   81,   82,
 /*  1970 */    83,   84,   10,  146,   66,  146,  146,  146,  146,  146,
 /*  1980 */   146,   73,   74,  146,  146,  146,  146,  146,   26,   81,
 /*  1990 */    82,   83,   84,  146,   19,   20,   21,   22,   23,   24,
 /*  2000 */    38,   26,   40,   41,   42,   43,   44,   45,  146,  146,
 /*  2010 */   146,  146,   37,  146,   19,   20,   21,   22,   23,   24,
 /*  2020 */   146,   26,  146,  146,  146,  146,   93,   94,   95,   96,
 /*  2030 */    97,   98,   37,  100,  101,  102,  103,  146,  146,  106,
 /*  2040 */   146,   66,   67,  146,  146,  146,  146,  146,   73,   74,
 /*  2050 */   146,  146,  146,  146,  146,  146,   81,   82,   83,   84,
 /*  2060 */   146,   66,  146,  146,  146,  146,  146,  146,   73,   74,
 /*  2070 */   146,  146,  146,  146,  146,  146,   81,   82,   83,   84,
 /*  2080 */   146,  146,  146,    6,    7,    8,    9,   10,   11,   12,
 /*  2090 */    13,   14,   15,   16,   17,   18,  146,  146,  146,  146,
 /*  2100 */   146,  146,  146,  146,  146,  146,   29,  146,  146,  146,
 /*  2110 */   146,  146,  146,    6,    7,    8,    9,   10,   11,   12,
 /*  2120 */    13,   14,   15,   16,   17,   18,  146,  146,  146,  146,
 /*  2130 */   146,  146,  146,  146,  146,  146,   29,  146,  146,  146,
 /*  2140 */   146,  146,  146,    6,    7,    8,    9,   10,   11,   12,
 /*  2150 */    13,   14,   15,   16,   17,   18,  146,  146,  146,  146,
 /*  2160 */   146,  146,  146,  146,   27,  146,  146,  146,  146,  146,
 /*  2170 */   146,    6,    7,    8,    9,   10,   11,   12,   13,   14,
 /*  2180 */    15,   16,   17,   18,  146,  146,  146,  146,  146,  146,
 /*  2190 */   146,  146,  146,  146,   29,  146,  146,  146,  146,  146,
 /*  2200 */   146,    6,    7,    8,    9,   10,   11,   12,   13,   14,
 /*  2210 */    15,   16,   17,   18,    6,    7,    8,    9,   10,   11,
 /*  2220 */    12,   13,   14,   15,   16,   17,   18,    6,    7,    8,
 /*  2230 */     9,   10,   11,   12,   13,   14,   15,   16,   17,   18,
 /*  2240 */   146,  146,  146,  146,  146,  146,   38,  146,  146,  146,
 /*  2250 */    29,  146,  146,  146,  146,  146,  146,  146,  146,  146,
 /*  2260 */   146,  146,   67,  146,  146,  146,  146,  146,  146,    6,
 /*  2270 */     7,    8,    9,   10,   11,   12,   13,   14,   15,   16,
 /*  2280 */    17,   18,  146,  146,  146,  146,  146,  146,  146,  146,
 /*  2290 */   146,  146,   29,  146,  146,  146,  146,  146,  146,    6,
 /*  2300 */     7,    8,    9,   10,   11,   12,   13,   14,   15,   16,
 /*  2310 */    17,   18,    8,    9,   10,   11,   12,   13,   14,   15,
 /*  2320 */    16,   17,   18,   93,   94,   95,   96,   97,   98,  146,
 /*  2330 */   100,  101,  102,  103,  146,  146,  106,  146,  146,  146,
 /*  2340 */    93,   94,   95,   96,   97,   98,  146,  100,  101,  102,
 /*  2350 */   103,  146,  146,  106,  146,  146,  146,  146,   10,  146,
 /*  2360 */   146,  146,  146,   93,   94,   95,   96,   97,   98,  146,
 /*  2370 */   100,  101,  102,  103,   26,  146,  106,  146,  146,  146,
 /*  2380 */   146,  146,  146,  146,  146,  146,   38,  146,   40,   41,
 /*  2390 */    42,   43,   44,   45,
};
#define YY_SHIFT_USE_DFLT (-50)
static short yy_shift_ofst[] = {
 /*     0 */   730,  427, 1051,  -50,  -50,  -50,  -50,  116,  782,  -50,
 /*    10 */   -18,   -6,  118,  581,  -50,  -50,  228,  558,  -50,  336,
 /*    20 */   367,  428, 1962,  584,  799,  -50,   29,  -50,  -50,  -50,
 /*    30 */   -50,  -50,  -50,  -50,  -50,  -50,  -50,  -50,  -50,  -50,
 /*    40 */   -50,  -50,  -50,  -50,  291,  -50,  419,  -50, 1995,  508,
 /*    50 */   469,  579,  533,  577,   84,  689,  -50, 1995,  713,   10,
 /*    60 */   -50,  170,  -50,  -50,  -47,  840, 1170, 1170,  962,  634,
 /*    70 */   249,  -50, 1995, 1017, 1019, 1074,  -50,  272,  637, 1800,
 /*    80 */  2263, 1170,  909,  -50,  422,  626,  -50,  422,  -50, 1352,
 /*    90 */   -50,  -50,  -50,  -50,  -50,  -50, 1995,  -50, 2293, 1995,
 /*   100 */  1995,  -50, 1995,  202,  -50,  710,  152,  711,  -50,  764,
 /*   110 */   -50,  789,  -50,  458, 1995,  458, 1995,  458, 1995,  458,
 /*   120 */  1995,  458, 1995,  458, 1995,  458, 1995,  -50, 1995,  -50,
 /*   130 */  1995,  -50, 1995, 2304, 1995, 2304, 1995, 2208,  -50,  -34,
 /*   140 */  1022,  519, 1887, 2293, 1995,  -50,   92,  208,  202,  -50,
 /*   150 */  1995,  -50,  -50,  -50,  -50,  -50,  -50,  -50,  233,  -50,
 /*   160 */   496,  -50,  283,  -50,  445,  644,  -50,  -50,  -50,  -50,
 /*   170 */   -50,  -50,  -50,  -50,  -50,  -50,  589,  596,  614,  -50,
 /*   180 */   652,  649, 1867,  -50,  570,  -50, 1995,  -50,  -50,  -50,
 /*   190 */   625,  -50,  -50,   16,  785,  -50,  794,  -50,  806,  -50,
 /*   200 */  1908,   90,  -50,  -50,  458,  867,  690, 1995,  -50, 1768,
 /*   210 */   855, 1995,  -50,  873,  855, 1995,  -50, 2195, 1975,  855,
 /*   220 */  1995,  -50,  907,  855, 1995,  -50,  -50,  -50,  -50, 1995,
 /*   230 */   364,  -50, 1995,  -50, 2293,  -50,  976,  -50, 1995, 2137,
 /*   240 */   -50,  990,  417,  995,  997,  -50,  207,  965, 1020,  -50,
 /*   250 */   986,  871,  -50,  -49,   12,  -50,  506,   41,  -50,  175,
 /*   260 */   185, 1995,  602,  218,  -50,  329,  770,  -50,  329,  -50,
 /*   270 */   388,  -47,  -50,  -50,  329,  768,  -50,  329,  618,  -50,
 /*   280 */   329,  550,  -50,  329,  680,  -50,  329,  766,  -50,  329,
 /*   290 */   783,  -50,  329,  801,  -50,  329,  296,  -50,  329,  383,
 /*   300 */   -50,  478,  532,  519,  -50,  -50, 1995, 2077, 1170,  414,
 /*   310 */   -50,  611,  527, 1780, 2107, 1170,  139,  -50, 1995, 2165,
 /*   320 */  1170,  964,  -50,  -50, 2221,  -50,  640, 1170, 1170,  -50,
 /*   330 */   -50,  674,  709,  854,  -26,  -50,  -50,  515,  743,  304,
 /*   340 */   -50,  359,  -50, 1788,  -50,  749,  -47,  -50,  786,  784,
 /*   350 */   -47,  -50,  822,  816,  -47,  -50,  831,  848,  834,  836,
 /*   360 */   838,  839,  841,  842,  856,  -50,  919,  845,  857,  849,
 /*   370 */  2348,  859,  194,  -50,  744,  -50,  297,  862,  524,  -50,
 /*   380 */  1129,  -50,  869,  405,  870,  168,  875,  876,   56,  878,
 /*   390 */   891,  892,   77,  900,  901,  494,  902,  905,  338,  613,
 /*   400 */   -50,  -50,  943,  917,  914,  -47,  926,  -50,  936,  932,
 /*   410 */   -47,  942,  -50,  -50,  998,  969,  259,  -50,  -47,  949,
 /*   420 */   -50,  -50,  -50,  -50,  -50,  -50,  -50,  956,  134,  957,
 /*   430 */   113, 1024,  -21,  -50,  -47,  971,  -50,  -50,    8,  503,
 /*   440 */   448,  309,  778,  -50,  833,  982,  984,  173,  985,  -50,
 /*   450 */   -50,  989,  996,  307,  393,  999,  -50,  -50, 1004, 1002,
 /*   460 */    63, 1008,  -50,  -50,  -50,
};
#define YY_REDUCE_USE_DFLT (-102)
static short yy_reduce_ofst[] = {
 /*     0 */   610, -102,  165, -102, -102, -102, -102, -102, -102, -102,
 /*    10 */  -102, -102, 2270, -102, -102, -102, 1211, -101, -102, -102,
 /*    20 */  -102, -102,   37, -102, 1383, -102, 1657, -102, -102, -102,
 /*    30 */  -102, -102, -102, -102, -102, -102, -102, -102, -102, -102,
 /*    40 */  -102, -102, -102, -102, -102, -102, -102, -102,  -13, -102,
 /*    50 */  1180, 1657, -102, -102, 1520, 1657, -102,  262, -102,  460,
 /*    60 */  -102,  716, -102, -102,  791, -102, 1283, 1657, -102, 1146,
 /*    70 */  1657, -102,   22, -102, 1591, 1657, -102, -102, -102,  668,
 /*    80 */  -102, 1454, 1657, -102,  313, -102, -102,  373, -102,  400,
 /*    90 */  -102, -102, -102, -102, -102, -102,  390, -102, -102,  567,
 /*   100 */   482, -102,  399,  600, -102, -102, -102, -102, -102, -102,
 /*   110 */  -102, -102, -102, -102,  -80, -102,  234, -102,  352, -102,
 /*   120 */   115, -102,  385, -102,  317, -102,  250, -102,  225, -102,
 /*   130 */   -25, -102,  -50, -102,  621, -102,  580, -102, -102, -102,
 /*   140 */  -102, -102,  -55, -102,  462, -102, -102, -102,  122, -102,
 /*   150 */   677, -102, -102, -102, -102, -102, -102, -102,   -8, -102,
 /*   160 */  -102, -102,  188, -102, -102,  334, -102, -102, -102, -102,
 /*   170 */  -102, -102, -102, -102, -102, -102, -102, -102, -102, -102,
 /*   180 */  -102, -102,  275, -102, -102, -102,  335, -102, -102, -102,
 /*   190 */  -102, -102, -102, -102, -102, -102, -102, -102, -102, -102,
 /*   200 */   330, -102, -102, -102, -102, -102,  754,  453, -102,  511,
 /*   210 */   772,  555, -102, -102,  776,  500, -102, -102,  440,  830,
 /*   220 */   402, -102, -102,  844,   60, -102, -102, -102, -102,  537,
 /*   230 */  -102, -102,   55, -102, -102, -102, -102, -102,  702, -102,
 /*   240 */  -102, -102, -102, -102, -102, -102, -102, -102, -102, -102,
 /*   250 */  -102, -102, -102, -102, -102, -102, -102, -102, -102, -102,
 /*   260 */  -102,  110, -102, -102, -102,  530, -102, -102,  183, -102,
 /*   270 */  -102,  244, -102, -102,  608, -102, -102,  454, -102, -102,
 /*   280 */   413, -102, -102,  523, -102, -102,  582, -102, -102,  663,
 /*   290 */  -102, -102,  688, -102, -102,  138, -102, -102,  286, -102,
 /*   300 */  -102, -102, -102, -102, -102, -102,  372, -102, 1317, 1657,
 /*   310 */  -102, -102, -102,  132, -102, 1624, 1657, -102,  605, -102,
 /*   320 */  1213, 1657, -102, -102, -102, -102, -102, 1246, 1657, -102,
 /*   330 */  -102, -102, -102, 1487, 1657, -102, -102, -102, -102, 1420,
 /*   340 */  -102, 1657, -102,   59, -102, -102,  708, -102, -102, -102,
 /*   350 */   734, -102, -102, -102,  752, -102, -102, -102, -102, -102,
 /*   360 */  -102, -102, -102, -102, -102, -102,   -9, -102, -102, -102,
 /*   370 */   312, -102, 1350, -102, 1657, -102, -102, -102, 1557, -102,
 /*   380 */  1657, -102, -102, -102, -102, -102, -102, -102, -102, -102,
 /*   390 */  -102, -102, -102, -102, -102, -102, -102, -102,  182, -101,
 /*   400 */  -102, -102,   -9, -102, -102,  861, -102, -102, -102, -102,
 /*   410 */   868, -102, -102, -102,   99,  877, -102, -102,  884, -102,
 /*   420 */  -102, -102, -102, -102, -102, -102, -102, -102, -102, -102,
 /*   430 */  -102,  877, -102, -102,  897, -102, -102, -102, 1177, 1281,
 /*   440 */  -101,   99, -101, -102, -101, -102, -102, 1933, -102, -102,
 /*   450 */  -102, -102, -102, -102, 2230, -102, -102, -102, -102, -102,
 /*   460 */  2247, -102, -102, -102, -102,
};
static YYACTIONTYPE yy_default[] = {
 /*     0 */   693,  693,  693,  467,  469,  470,  471,  693,  693,  472,
 /*    10 */   693,  693,  693,  693,  473,  477,  693,  693,  504,  693,
 /*    20 */   693,  693,  693,  693,  693,  508,  693,  510,  546,  548,
 /*    30 */   549,  550,  551,  552,  553,  554,  555,  556,  557,  558,
 /*    40 */   559,  560,  561,  562,  693,  563,  693,  564,  693,  693,
 /*    50 */   693,  693,  567,  693,  693,  693,  568,  693,  693,  693,
 /*    60 */   571,  693,  572,  573,  693,  693,  693,  575,  693,  693,
 /*    70 */   693,  578,  693,  693,  693,  693,  580,  693,  693,  693,
 /*    80 */   693,  693,  693,  581,  693,  693,  585,  693,  586,  693,
 /*    90 */   588,  589,  590,  591,  592,  593,  693,  594,  629,  693,
 /*   100 */   693,  630,  693,  693,  631,  693,  693,  693,  688,  693,
 /*   110 */   689,  693,  690,  633,  693,  634,  693,  635,  693,  636,
 /*   120 */   693,  637,  693,  638,  693,  639,  693,  645,  693,  646,
 /*   130 */   693,  647,  693,  648,  693,  649,  693,  693,  640,  653,
 /*   140 */   693,  641,  693,  672,  693,  650,  693,  693,  693,  651,
 /*   150 */   693,  652,  654,  655,  656,  657,  658,  659,  693,  660,
 /*   160 */   693,  661,  693,  673,  693,  693,  675,  680,  681,  682,
 /*   170 */   683,  684,  685,  686,  687,  676,  681,  683,  682,  674,
 /*   180 */   693,  662,  693,  663,  693,  664,  693,  667,  670,  671,
 /*   190 */   693,  668,  669,  693,  693,  642,  693,  643,  693,  644,
 /*   200 */   693,  693,  665,  666,  632,  693,  693,  693,  595,  693,
 /*   210 */   693,  693,  596,  693,  693,  693,  597,  693,  693,  693,
 /*   220 */   693,  598,  693,  693,  693,  599,  600,  601,  587,  693,
 /*   230 */   693,  603,  693,  604,  606,  605,  693,  607,  693,  693,
 /*   240 */   608,  693,  693,  693,  693,  609,  693,  693,  693,  610,
 /*   250 */   693,  693,  611,  693,  693,  612,  693,  693,  613,  693,
 /*   260 */   693,  693,  693,  693,  614,  693,  693,  615,  693,  625,
 /*   270 */   627,  693,  628,  626,  693,  693,  616,  693,  693,  617,
 /*   280 */   693,  693,  618,  693,  693,  619,  693,  693,  620,  693,
 /*   290 */   693,  621,  693,  693,  622,  693,  693,  623,  693,  693,
 /*   300 */   624,  693,  693,  693,  692,  547,  693,  693,  693,  693,
 /*   310 */   582,  693,  693,  693,  693,  693,  693,  583,  693,  693,
 /*   320 */   693,  693,  584,  579,  693,  577,  693,  693,  576,  574,
 /*   330 */   569,  565,  693,  693,  693,  570,  566,  693,  693,  693,
 /*   340 */   509,  693,  511,  693,  531,  533,  693,  536,  693,  534,
 /*   350 */   693,  537,  693,  535,  693,  538,  693,  693,  693,  693,
 /*   360 */   693,  693,  693,  693,  693,  532,  693,  693,  693,  693,
 /*   370 */   693,  693,  693,  512,  693,  514,  693,  693,  693,  513,
 /*   380 */   693,  515,  693,  693,  693,  693,  693,  693,  693,  693,
 /*   390 */   693,  693,  693,  693,  693,  693,  693,  693,  693,  693,
 /*   400 */   502,  505,  693,  693,  693,  693,  693,  506,  693,  693,
 /*   410 */   693,  693,  507,  490,  693,  693,  693,  492,  693,  693,
 /*   420 */   494,  496,  498,  499,  500,  501,  497,  498,  499,  500,
 /*   430 */   501,  693,  693,  493,  693,  693,  495,  503,  693,  693,
 /*   440 */   693,  693,  693,  491,  693,  693,  693,  693,  693,  474,
 /*   450 */   478,  693,  693,  693,  693,  693,  475,  479,  693,  693,
 /*   460 */   693,  693,  476,  480,  468,
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
  "NOT",           "NAMESPACE",     "IDENTIFIER",    "DOTCOMMA",    
  "CLASS",         "BRACKET_OPEN",  "BRACKET_CLOSE",  "EXTENDS",     
  "ABSTRACT",      "COMMENT",       "ASSIGN",        "CONST",       
  "FUNCTION",      "PARENTHESES_OPEN",  "PARENTHESES_CLOSE",  "INLINE",      
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
  "TYPE_ULONG",    "DOUBLE",        "NULL",          "TRUE",        
  "FALSE",         "error",         "program",       "xx_language", 
  "xx_top_statement_list",  "xx_top_statement",  "xx_namespace_def",  "xx_class_def",
  "xx_comment",    "xx_class_definition",  "xx_class_properties_definition",  "xx_class_consts_definition",
  "xx_class_methods_definition",  "xx_class_property_definition",  "xx_property_visibility_list",  "xx_literal_expr",
  "xx_property_visibility",  "xx_class_const_definition",  "xx_class_method_definition",  "xx_method_visibility_list",
  "xx_parameter_list",  "xx_statement_list",  "xx_method_visibility",  "xx_parameter",
  "xx_parameter_type",  "xx_parameter_cast",  "xx_statement",  "xx_let_statement",
  "xx_if_statement",  "xx_loop_statement",  "xx_echo_statement",  "xx_return_statement",
  "xx_mcall_statement",  "xx_unset_statement",  "xx_throw_statement",  "xx_declare_statement",
  "xx_break_statement",  "xx_continue_statement",  "xx_while_statement",  "xx_switch_statement",
  "xx_for_statement",  "xx_eval_expr",  "xx_case_clauses",  "xx_case_clause",
  "xx_common_expr",  "xx_let_assignments",  "xx_let_assignment",  "xx_assignment_operator",
  "xx_assign_expr",  "xx_index_expr",  "xx_echo_expressions",  "xx_echo_expression",
  "xx_mcall_expr",  "xx_call_parameters",  "xx_declare_variable_list",  "xx_declare_variable",
  "xx_isset_expr",  "xx_array_list",  "xx_call_parameter",  "xx_array_item",
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
 /*  51 */ "xx_method_visibility_list ::= xx_method_visibility",
 /*  52 */ "xx_method_visibility ::= PUBLIC",
 /*  53 */ "xx_method_visibility ::= PROTECTED",
 /*  54 */ "xx_method_visibility ::= STATIC",
 /*  55 */ "xx_method_visibility ::= PRIVATE",
 /*  56 */ "xx_method_visibility ::= STATIC PUBLIC",
 /*  57 */ "xx_method_visibility ::= PUBLIC STATIC",
 /*  58 */ "xx_method_visibility ::= STATIC PROTECTED",
 /*  59 */ "xx_method_visibility ::= PROTECTED STATIC",
 /*  60 */ "xx_method_visibility ::= STATIC PRIVATE",
 /*  61 */ "xx_method_visibility ::= PRIVATE STATIC",
 /*  62 */ "xx_method_visibility ::= INLINE PROTECTED",
 /*  63 */ "xx_method_visibility ::= PROTECTED INLINE",
 /*  64 */ "xx_method_visibility ::= INLINE PRIVATE",
 /*  65 */ "xx_method_visibility ::= PRIVATE INLINE",
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
 /*  76 */ "xx_parameter_type ::= TYPE_DOUBLE",
 /*  77 */ "xx_parameter_type ::= TYPE_BOOL",
 /*  78 */ "xx_parameter_type ::= TYPE_STRING",
 /*  79 */ "xx_parameter_type ::= TYPE_ARRAY",
 /*  80 */ "xx_parameter_type ::= TYPE_VAR",
 /*  81 */ "xx_statement_list ::= xx_statement_list xx_statement",
 /*  82 */ "xx_statement_list ::= xx_statement",
 /*  83 */ "xx_statement ::= xx_let_statement",
 /*  84 */ "xx_statement ::= xx_if_statement",
 /*  85 */ "xx_statement ::= xx_loop_statement",
 /*  86 */ "xx_statement ::= xx_echo_statement",
 /*  87 */ "xx_statement ::= xx_return_statement",
 /*  88 */ "xx_statement ::= xx_mcall_statement",
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
 /* 143 */ "xx_return_statement ::= RETURN xx_common_expr DOTCOMMA",
 /* 144 */ "xx_unset_statement ::= UNSET IDENTIFIER ARROW IDENTIFIER DOTCOMMA",
 /* 145 */ "xx_unset_statement ::= UNSET IDENTIFIER SBRACKET_OPEN IDENTIFIER SBRACKET_CLOSE DOTCOMMA",
 /* 146 */ "xx_unset_statement ::= UNSET IDENTIFIER SBRACKET_OPEN INTEGER SBRACKET_CLOSE DOTCOMMA",
 /* 147 */ "xx_unset_statement ::= UNSET IDENTIFIER SBRACKET_OPEN STRING SBRACKET_CLOSE DOTCOMMA",
 /* 148 */ "xx_throw_statement ::= THROW IDENTIFIER DOTCOMMA",
 /* 149 */ "xx_throw_statement ::= THROW NEW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE DOTCOMMA",
 /* 150 */ "xx_declare_statement ::= TYPE_INTEGER xx_declare_variable_list DOTCOMMA",
 /* 151 */ "xx_declare_statement ::= TYPE_UINTEGER xx_declare_variable_list DOTCOMMA",
 /* 152 */ "xx_declare_statement ::= TYPE_CHAR xx_declare_variable_list DOTCOMMA",
 /* 153 */ "xx_declare_statement ::= TYPE_UCHAR xx_declare_variable_list DOTCOMMA",
 /* 154 */ "xx_declare_statement ::= TYPE_LONG xx_declare_variable_list DOTCOMMA",
 /* 155 */ "xx_declare_statement ::= TYPE_ULONG xx_declare_variable_list DOTCOMMA",
 /* 156 */ "xx_declare_statement ::= TYPE_DOUBLE xx_declare_variable_list DOTCOMMA",
 /* 157 */ "xx_declare_statement ::= TYPE_STRING xx_declare_variable_list DOTCOMMA",
 /* 158 */ "xx_declare_statement ::= TYPE_BOOL xx_declare_variable_list DOTCOMMA",
 /* 159 */ "xx_declare_statement ::= TYPE_VAR xx_declare_variable_list DOTCOMMA",
 /* 160 */ "xx_declare_variable_list ::= xx_declare_variable_list COMMA xx_declare_variable",
 /* 161 */ "xx_declare_variable_list ::= xx_declare_variable",
 /* 162 */ "xx_declare_variable ::= IDENTIFIER",
 /* 163 */ "xx_declare_variable ::= IDENTIFIER ASSIGN xx_literal_expr",
 /* 164 */ "xx_assign_expr ::= xx_common_expr",
 /* 165 */ "xx_common_expr ::= NOT xx_common_expr",
 /* 166 */ "xx_common_expr ::= ISSET xx_isset_expr",
 /* 167 */ "xx_common_expr ::= xx_common_expr EQUALS xx_common_expr",
 /* 168 */ "xx_common_expr ::= xx_common_expr NOTEQUALS xx_common_expr",
 /* 169 */ "xx_common_expr ::= xx_common_expr IDENTICAL xx_common_expr",
 /* 170 */ "xx_common_expr ::= xx_common_expr NOTIDENTICAL xx_common_expr",
 /* 171 */ "xx_common_expr ::= xx_common_expr LESS xx_common_expr",
 /* 172 */ "xx_common_expr ::= xx_common_expr GREATER xx_common_expr",
 /* 173 */ "xx_common_expr ::= xx_common_expr LESSEQUAL xx_common_expr",
 /* 174 */ "xx_common_expr ::= xx_common_expr GREATEREQUAL xx_common_expr",
 /* 175 */ "xx_common_expr ::= PARENTHESES_OPEN xx_common_expr PARENTHESES_CLOSE",
 /* 176 */ "xx_common_expr ::= IDENTIFIER ARROW IDENTIFIER",
 /* 177 */ "xx_common_expr ::= IDENTIFIER SBRACKET_OPEN IDENTIFIER SBRACKET_CLOSE",
 /* 178 */ "xx_common_expr ::= IDENTIFIER SBRACKET_OPEN INTEGER SBRACKET_CLOSE",
 /* 179 */ "xx_common_expr ::= IDENTIFIER SBRACKET_OPEN STRING SBRACKET_CLOSE",
 /* 180 */ "xx_common_expr ::= xx_common_expr ADD xx_common_expr",
 /* 181 */ "xx_common_expr ::= xx_common_expr SUB xx_common_expr",
 /* 182 */ "xx_common_expr ::= xx_common_expr CONCAT xx_common_expr",
 /* 183 */ "xx_common_expr ::= xx_common_expr AND xx_common_expr",
 /* 184 */ "xx_common_expr ::= xx_common_expr OR xx_common_expr",
 /* 185 */ "xx_common_expr ::= INSTANCEOF xx_common_expr",
 /* 186 */ "xx_common_expr ::= FETCH IDENTIFIER COMMA xx_isset_expr",
 /* 187 */ "xx_common_expr ::= TYPEOF xx_common_expr",
 /* 188 */ "xx_common_expr ::= IDENTIFIER",
 /* 189 */ "xx_common_expr ::= INTEGER",
 /* 190 */ "xx_common_expr ::= STRING",
 /* 191 */ "xx_common_expr ::= DOUBLE",
 /* 192 */ "xx_common_expr ::= NULL",
 /* 193 */ "xx_common_expr ::= TRUE",
 /* 194 */ "xx_common_expr ::= FALSE",
 /* 195 */ "xx_common_expr ::= SBRACKET_OPEN SBRACKET_CLOSE",
 /* 196 */ "xx_common_expr ::= SBRACKET_OPEN xx_array_list SBRACKET_CLOSE",
 /* 197 */ "xx_common_expr ::= NEW IDENTIFIER",
 /* 198 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 199 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 200 */ "xx_common_expr ::= IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 201 */ "xx_common_expr ::= IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 202 */ "xx_common_expr ::= xx_mcall_expr",
 /* 203 */ "xx_mcall_expr ::= IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 204 */ "xx_mcall_expr ::= IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 205 */ "xx_call_parameters ::= xx_call_parameters COMMA xx_call_parameter",
 /* 206 */ "xx_call_parameters ::= xx_call_parameter",
 /* 207 */ "xx_call_parameter ::= xx_common_expr",
 /* 208 */ "xx_array_list ::= xx_array_list COMMA xx_array_item",
 /* 209 */ "xx_array_list ::= xx_array_item",
 /* 210 */ "xx_array_item ::= xx_array_key COLON xx_array_value",
 /* 211 */ "xx_array_item ::= xx_array_value",
 /* 212 */ "xx_array_key ::= IDENTIFIER",
 /* 213 */ "xx_array_key ::= STRING",
 /* 214 */ "xx_array_key ::= INTEGER",
 /* 215 */ "xx_array_value ::= xx_literal_expr",
 /* 216 */ "xx_array_value ::= IDENTIFIER",
 /* 217 */ "xx_literal_expr ::= INTEGER",
 /* 218 */ "xx_literal_expr ::= STRING",
 /* 219 */ "xx_literal_expr ::= DOUBLE",
 /* 220 */ "xx_literal_expr ::= NULL",
 /* 221 */ "xx_literal_expr ::= FALSE",
 /* 222 */ "xx_literal_expr ::= TRUE",
 /* 223 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN IDENTIFIER SBRACKET_CLOSE",
 /* 224 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN INTEGER SBRACKET_CLOSE",
 /* 225 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN STRING SBRACKET_CLOSE",
 /* 226 */ "xx_eval_expr ::= xx_common_expr",
 /* 227 */ "xx_comment ::= COMMENT",
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
// 773 "parser.lemon"
{
	/*if ((yypminor->yy0)) {
		if ((yypminor->yy0)->free_flag) {
			efree((yypminor->yy0)->token);
		}
		efree((yypminor->yy0));
	}*/
}
// 1961 "parser.c"
      break;
    case 87:
// 786 "parser.lemon"
{ json_object_put((yypminor->yy279)); }
// 1966 "parser.c"
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
  { 86, 1 },
  { 87, 1 },
  { 88, 2 },
  { 88, 1 },
  { 89, 1 },
  { 89, 1 },
  { 89, 1 },
  { 90, 3 },
  { 91, 5 },
  { 91, 7 },
  { 91, 6 },
  { 91, 8 },
  { 91, 4 },
  { 91, 6 },
  { 91, 5 },
  { 91, 7 },
  { 93, 1 },
  { 93, 1 },
  { 93, 1 },
  { 93, 2 },
  { 93, 2 },
  { 93, 2 },
  { 93, 2 },
  { 93, 3 },
  { 93, 3 },
  { 94, 2 },
  { 94, 1 },
  { 97, 4 },
  { 97, 3 },
  { 97, 6 },
  { 97, 5 },
  { 98, 2 },
  { 98, 1 },
  { 100, 1 },
  { 100, 1 },
  { 100, 1 },
  { 100, 1 },
  { 95, 2 },
  { 95, 1 },
  { 96, 2 },
  { 96, 1 },
  { 101, 6 },
  { 101, 5 },
  { 102, 7 },
  { 102, 8 },
  { 102, 8 },
  { 102, 9 },
  { 102, 8 },
  { 102, 9 },
  { 102, 9 },
  { 102, 10 },
  { 103, 1 },
  { 106, 1 },
  { 106, 1 },
  { 106, 1 },
  { 106, 1 },
  { 106, 2 },
  { 106, 2 },
  { 106, 2 },
  { 106, 2 },
  { 106, 2 },
  { 106, 2 },
  { 106, 2 },
  { 106, 2 },
  { 106, 2 },
  { 106, 2 },
  { 104, 3 },
  { 104, 1 },
  { 107, 1 },
  { 107, 2 },
  { 107, 2 },
  { 107, 3 },
  { 107, 4 },
  { 107, 4 },
  { 109, 3 },
  { 108, 1 },
  { 108, 1 },
  { 108, 1 },
  { 108, 1 },
  { 108, 1 },
  { 108, 1 },
  { 105, 2 },
  { 105, 1 },
  { 110, 1 },
  { 110, 1 },
  { 110, 1 },
  { 110, 1 },
  { 110, 1 },
  { 110, 1 },
  { 110, 1 },
  { 110, 1 },
  { 110, 1 },
  { 110, 1 },
  { 110, 1 },
  { 110, 1 },
  { 110, 1 },
  { 110, 1 },
  { 110, 1 },
  { 120, 2 },
  { 121, 2 },
  { 112, 4 },
  { 112, 7 },
  { 112, 5 },
  { 112, 9 },
  { 112, 8 },
  { 112, 8 },
  { 123, 4 },
  { 123, 5 },
  { 126, 2 },
  { 126, 1 },
  { 127, 4 },
  { 127, 3 },
  { 113, 3 },
  { 113, 4 },
  { 122, 4 },
  { 122, 5 },
  { 124, 7 },
  { 124, 8 },
  { 124, 9 },
  { 124, 10 },
  { 111, 3 },
  { 129, 3 },
  { 129, 1 },
  { 131, 1 },
  { 131, 1 },
  { 131, 1 },
  { 131, 1 },
  { 131, 1 },
  { 131, 1 },
  { 130, 3 },
  { 130, 5 },
  { 130, 7 },
  { 130, 8 },
  { 130, 5 },
  { 130, 6 },
  { 130, 2 },
  { 130, 2 },
  { 133, 1 },
  { 114, 3 },
  { 134, 3 },
  { 134, 1 },
  { 135, 1 },
  { 116, 2 },
  { 115, 3 },
  { 117, 5 },
  { 117, 6 },
  { 117, 6 },
  { 117, 6 },
  { 118, 3 },
  { 118, 7 },
  { 119, 3 },
  { 119, 3 },
  { 119, 3 },
  { 119, 3 },
  { 119, 3 },
  { 119, 3 },
  { 119, 3 },
  { 119, 3 },
  { 119, 3 },
  { 119, 3 },
  { 138, 3 },
  { 138, 1 },
  { 139, 1 },
  { 139, 3 },
  { 132, 1 },
  { 128, 2 },
  { 128, 2 },
  { 128, 3 },
  { 128, 3 },
  { 128, 3 },
  { 128, 3 },
  { 128, 3 },
  { 128, 3 },
  { 128, 3 },
  { 128, 3 },
  { 128, 3 },
  { 128, 3 },
  { 128, 4 },
  { 128, 4 },
  { 128, 4 },
  { 128, 3 },
  { 128, 3 },
  { 128, 3 },
  { 128, 3 },
  { 128, 3 },
  { 128, 2 },
  { 128, 4 },
  { 128, 2 },
  { 128, 1 },
  { 128, 1 },
  { 128, 1 },
  { 128, 1 },
  { 128, 1 },
  { 128, 1 },
  { 128, 1 },
  { 128, 2 },
  { 128, 3 },
  { 128, 2 },
  { 128, 4 },
  { 128, 5 },
  { 128, 4 },
  { 128, 3 },
  { 128, 1 },
  { 136, 6 },
  { 136, 5 },
  { 137, 3 },
  { 137, 1 },
  { 142, 1 },
  { 141, 3 },
  { 141, 1 },
  { 143, 3 },
  { 143, 1 },
  { 144, 1 },
  { 144, 1 },
  { 144, 1 },
  { 145, 1 },
  { 145, 1 },
  { 99, 1 },
  { 99, 1 },
  { 99, 1 },
  { 99, 1 },
  { 99, 1 },
  { 99, 1 },
  { 140, 4 },
  { 140, 4 },
  { 140, 4 },
  { 125, 1 },
  { 92, 1 },
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
// 782 "parser.lemon"
{
	status->ret = yymsp[0].minor.yy279;
}
// 2411 "parser.c"
        break;
      case 1:
      case 4:
      case 5:
      case 6:
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
      case 164:
      case 202:
      case 207:
      case 215:
      case 226:
// 788 "parser.lemon"
{
	yygotominor.yy279 = yymsp[0].minor.yy279;
}
// 2442 "parser.c"
        break;
      case 2:
      case 25:
      case 31:
      case 37:
      case 39:
      case 81:
      case 108:
// 792 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_list(yymsp[-1].minor.yy279, yymsp[0].minor.yy279);
}
// 2455 "parser.c"
        break;
      case 3:
      case 26:
      case 32:
      case 38:
      case 40:
      case 51:
      case 67:
      case 82:
      case 109:
      case 122:
      case 140:
      case 161:
      case 206:
      case 209:
// 796 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_list(NULL, yymsp[0].minor.yy279);
}
// 2475 "parser.c"
        break;
      case 7:
// 812 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_namespace(yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(27,&yymsp[0].minor);
}
// 2484 "parser.c"
        break;
      case 8:
// 816 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[-1].minor.yy279, 0, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-4].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 2494 "parser.c"
        break;
      case 9:
// 820 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_class(yymsp[-5].minor.yy0, yymsp[-1].minor.yy279, 0, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(28,&yymsp[-6].minor);
  yy_destructor(31,&yymsp[-4].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 2505 "parser.c"
        break;
      case 10:
// 824 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[-1].minor.yy279, 1, NULL, status->scanner_state);
  yy_destructor(32,&yymsp[-5].minor);
  yy_destructor(28,&yymsp[-4].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 2516 "parser.c"
        break;
      case 11:
// 828 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_class(yymsp[-5].minor.yy0, yymsp[-1].minor.yy279, 1, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(32,&yymsp[-7].minor);
  yy_destructor(28,&yymsp[-6].minor);
  yy_destructor(31,&yymsp[-4].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 2528 "parser.c"
        break;
      case 12:
// 832 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_class(yymsp[-2].minor.yy0, NULL, 0, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-3].minor);
  yy_destructor(29,&yymsp[-1].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 2538 "parser.c"
        break;
      case 13:
// 836 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_class(yymsp[-4].minor.yy0, NULL, 0, yymsp[-2].minor.yy0, status->scanner_state);
  yy_destructor(28,&yymsp[-5].minor);
  yy_destructor(31,&yymsp[-3].minor);
  yy_destructor(29,&yymsp[-1].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 2549 "parser.c"
        break;
      case 14:
// 840 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_class(yymsp[-2].minor.yy0, NULL, 1, NULL, status->scanner_state);
  yy_destructor(32,&yymsp[-4].minor);
  yy_destructor(28,&yymsp[-3].minor);
  yy_destructor(29,&yymsp[-1].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 2560 "parser.c"
        break;
      case 15:
// 844 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_class(yymsp[-4].minor.yy0, NULL, 1, yymsp[-2].minor.yy0, status->scanner_state);
  yy_destructor(32,&yymsp[-6].minor);
  yy_destructor(28,&yymsp[-5].minor);
  yy_destructor(31,&yymsp[-3].minor);
  yy_destructor(29,&yymsp[-1].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 2572 "parser.c"
        break;
      case 16:
// 848 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_class_definition(yymsp[0].minor.yy279, NULL, NULL, status->scanner_state);
}
// 2579 "parser.c"
        break;
      case 17:
// 852 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_class_definition(NULL, NULL, yymsp[0].minor.yy279, status->scanner_state);
}
// 2586 "parser.c"
        break;
      case 18:
// 856 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_class_definition(NULL, yymsp[0].minor.yy279, NULL, status->scanner_state);
}
// 2593 "parser.c"
        break;
      case 19:
// 860 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_class_definition(yymsp[-1].minor.yy279, yymsp[0].minor.yy279, NULL, status->scanner_state);
}
// 2600 "parser.c"
        break;
      case 20:
// 864 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_class_definition(yymsp[-1].minor.yy279, NULL, yymsp[0].minor.yy279, status->scanner_state);
}
// 2607 "parser.c"
        break;
      case 21:
// 868 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_class_definition(yymsp[0].minor.yy279, NULL, yymsp[-1].minor.yy279, status->scanner_state);
}
// 2614 "parser.c"
        break;
      case 22:
// 872 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_class_definition(NULL, yymsp[0].minor.yy279, yymsp[-1].minor.yy279, status->scanner_state);
}
// 2621 "parser.c"
        break;
      case 23:
// 876 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_class_definition(yymsp[-2].minor.yy279, yymsp[0].minor.yy279, yymsp[-1].minor.yy279, status->scanner_state);
}
// 2628 "parser.c"
        break;
      case 24:
// 880 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_class_definition(yymsp[-1].minor.yy279, yymsp[0].minor.yy279, yymsp[-2].minor.yy279, status->scanner_state);
}
// 2635 "parser.c"
        break;
      case 27:
// 892 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_class_property(yymsp[-2].minor.yy279, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(27,&yymsp[0].minor);
}
// 2643 "parser.c"
        break;
      case 28:
// 896 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_class_property(yymsp[-2].minor.yy279, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(27,&yymsp[0].minor);
}
// 2651 "parser.c"
        break;
      case 29:
// 900 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_class_property(yymsp[-4].minor.yy279, yymsp[-3].minor.yy0, yymsp[-1].minor.yy279, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(34,&yymsp[-2].minor);
  yy_destructor(27,&yymsp[0].minor);
}
// 2660 "parser.c"
        break;
      case 30:
// 904 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_class_property(yymsp[-4].minor.yy279, yymsp[-3].minor.yy0, yymsp[-1].minor.yy279, NULL, status->scanner_state);
  yy_destructor(34,&yymsp[-2].minor);
  yy_destructor(27,&yymsp[0].minor);
}
// 2669 "parser.c"
        break;
      case 33:
// 916 "parser.lemon"
{
	yygotominor.yy279 = json_object_new_string("public");
  yy_destructor(1,&yymsp[0].minor);
}
// 2677 "parser.c"
        break;
      case 34:
// 920 "parser.lemon"
{
	yygotominor.yy279 = json_object_new_string("protected");
  yy_destructor(2,&yymsp[0].minor);
}
// 2685 "parser.c"
        break;
      case 35:
// 924 "parser.lemon"
{
	yygotominor.yy279 = json_object_new_string("static");
  yy_destructor(3,&yymsp[0].minor);
}
// 2693 "parser.c"
        break;
      case 36:
// 928 "parser.lemon"
{
	yygotominor.yy279 = json_object_new_string("private");
  yy_destructor(4,&yymsp[0].minor);
}
// 2701 "parser.c"
        break;
      case 41:
// 948 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy279, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(35,&yymsp[-4].minor);
  yy_destructor(34,&yymsp[-2].minor);
  yy_destructor(27,&yymsp[0].minor);
}
// 2711 "parser.c"
        break;
      case 42:
// 952 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy279, NULL, status->scanner_state);
  yy_destructor(35,&yymsp[-4].minor);
  yy_destructor(34,&yymsp[-2].minor);
  yy_destructor(27,&yymsp[0].minor);
}
// 2721 "parser.c"
        break;
      case 43:
// 956 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_class_method(yymsp[-6].minor.yy279, yymsp[-4].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(36,&yymsp[-5].minor);
  yy_destructor(37,&yymsp[-3].minor);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(29,&yymsp[-1].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 2733 "parser.c"
        break;
      case 44:
// 960 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_class_method(yymsp[-7].minor.yy279, yymsp[-5].minor.yy0, yymsp[-3].minor.yy279, NULL, NULL, status->scanner_state);
  yy_destructor(36,&yymsp[-6].minor);
  yy_destructor(37,&yymsp[-4].minor);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(29,&yymsp[-1].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 2745 "parser.c"
        break;
      case 45:
// 964 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_class_method(yymsp[-7].minor.yy279, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy279, NULL, status->scanner_state);
  yy_destructor(36,&yymsp[-6].minor);
  yy_destructor(37,&yymsp[-4].minor);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 2757 "parser.c"
        break;
      case 46:
// 968 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_class_method(yymsp[-8].minor.yy279, yymsp[-6].minor.yy0, yymsp[-4].minor.yy279, yymsp[-1].minor.yy279, NULL, status->scanner_state);
  yy_destructor(36,&yymsp[-7].minor);
  yy_destructor(37,&yymsp[-5].minor);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 2769 "parser.c"
        break;
      case 47:
// 972 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_class_method(yymsp[-6].minor.yy279, yymsp[-4].minor.yy0, NULL, NULL, yymsp[-7].minor.yy0, status->scanner_state);
  yy_destructor(36,&yymsp[-5].minor);
  yy_destructor(37,&yymsp[-3].minor);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(29,&yymsp[-1].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 2781 "parser.c"
        break;
      case 48:
// 976 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_class_method(yymsp[-7].minor.yy279, yymsp[-5].minor.yy0, yymsp[-3].minor.yy279, NULL, yymsp[-8].minor.yy0, status->scanner_state);
  yy_destructor(36,&yymsp[-6].minor);
  yy_destructor(37,&yymsp[-4].minor);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(29,&yymsp[-1].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 2793 "parser.c"
        break;
      case 49:
// 980 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_class_method(yymsp[-7].minor.yy279, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy279, yymsp[-8].minor.yy0, status->scanner_state);
  yy_destructor(36,&yymsp[-6].minor);
  yy_destructor(37,&yymsp[-4].minor);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 2805 "parser.c"
        break;
      case 50:
// 984 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_class_method(yymsp[-8].minor.yy279, yymsp[-6].minor.yy0, yymsp[-4].minor.yy279, yymsp[-1].minor.yy279, yymsp[-9].minor.yy0, status->scanner_state);
  yy_destructor(36,&yymsp[-7].minor);
  yy_destructor(37,&yymsp[-5].minor);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 2817 "parser.c"
        break;
      case 52:
// 996 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_list(NULL, json_object_new_string("public"));
  yy_destructor(1,&yymsp[0].minor);
}
// 2825 "parser.c"
        break;
      case 53:
// 1000 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_list(NULL, json_object_new_string("protected"));
  yy_destructor(2,&yymsp[0].minor);
}
// 2833 "parser.c"
        break;
      case 54:
// 1004 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_list(NULL, json_object_new_string("static"));
  yy_destructor(3,&yymsp[0].minor);
}
// 2841 "parser.c"
        break;
      case 55:
// 1008 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_list(NULL, json_object_new_string("private"));
  yy_destructor(4,&yymsp[0].minor);
}
// 2849 "parser.c"
        break;
      case 56:
// 1012 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_list(json_object_new_string("static"), json_object_new_string("public"));
  yy_destructor(3,&yymsp[-1].minor);
  yy_destructor(1,&yymsp[0].minor);
}
// 2858 "parser.c"
        break;
      case 57:
// 1016 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_list(json_object_new_string("static"), json_object_new_string("public"));
  yy_destructor(1,&yymsp[-1].minor);
  yy_destructor(3,&yymsp[0].minor);
}
// 2867 "parser.c"
        break;
      case 58:
// 1020 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_list(json_object_new_string("static"), json_object_new_string("protected"));
  yy_destructor(3,&yymsp[-1].minor);
  yy_destructor(2,&yymsp[0].minor);
}
// 2876 "parser.c"
        break;
      case 59:
// 1024 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_list(json_object_new_string("static"), json_object_new_string("protected"));
  yy_destructor(2,&yymsp[-1].minor);
  yy_destructor(3,&yymsp[0].minor);
}
// 2885 "parser.c"
        break;
      case 60:
// 1028 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_list(json_object_new_string("static"), json_object_new_string("private"));
  yy_destructor(3,&yymsp[-1].minor);
  yy_destructor(4,&yymsp[0].minor);
}
// 2894 "parser.c"
        break;
      case 61:
// 1032 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_list(json_object_new_string("static"), json_object_new_string("private"));
  yy_destructor(4,&yymsp[-1].minor);
  yy_destructor(3,&yymsp[0].minor);
}
// 2903 "parser.c"
        break;
      case 62:
// 1036 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_list(json_object_new_string("inline"), json_object_new_string("protected"));
  yy_destructor(39,&yymsp[-1].minor);
  yy_destructor(2,&yymsp[0].minor);
}
// 2912 "parser.c"
        break;
      case 63:
// 1040 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_list(json_object_new_string("inline"), json_object_new_string("protected"));
  yy_destructor(2,&yymsp[-1].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 2921 "parser.c"
        break;
      case 64:
// 1044 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_list(json_object_new_string("inline"), json_object_new_string("private"));
  yy_destructor(39,&yymsp[-1].minor);
  yy_destructor(4,&yymsp[0].minor);
}
// 2930 "parser.c"
        break;
      case 65:
// 1048 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_list(json_object_new_string("inline"), json_object_new_string("private"));
  yy_destructor(4,&yymsp[-1].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 2939 "parser.c"
        break;
      case 66:
      case 121:
      case 139:
      case 160:
      case 205:
      case 208:
// 1052 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_list(yymsp[-2].minor.yy279, yymsp[0].minor.yy279);
  yy_destructor(5,&yymsp[-1].minor);
}
// 2952 "parser.c"
        break;
      case 68:
// 1060 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_parameter(NULL, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 2959 "parser.c"
        break;
      case 69:
// 1064 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_parameter(yymsp[-1].minor.yy279, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 2966 "parser.c"
        break;
      case 70:
// 1068 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_parameter(NULL, yymsp[-1].minor.yy279, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 2973 "parser.c"
        break;
      case 71:
// 1072 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_parameter(NULL, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy279, status->scanner_state);
  yy_destructor(34,&yymsp[-1].minor);
}
// 2981 "parser.c"
        break;
      case 72:
// 1076 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_parameter(yymsp[-3].minor.yy279, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy279, status->scanner_state);
  yy_destructor(34,&yymsp[-1].minor);
}
// 2989 "parser.c"
        break;
      case 73:
// 1080 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_parameter(NULL, yymsp[-3].minor.yy279, yymsp[-2].minor.yy0, yymsp[0].minor.yy279, status->scanner_state);
  yy_destructor(34,&yymsp[-1].minor);
}
// 2997 "parser.c"
        break;
      case 74:
// 1084 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(10,&yymsp[-2].minor);
  yy_destructor(11,&yymsp[0].minor);
}
// 3006 "parser.c"
        break;
      case 75:
// 1088 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_type(XX_TYPE_INTEGER);
  yy_destructor(40,&yymsp[0].minor);
}
// 3014 "parser.c"
        break;
      case 76:
// 1092 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_type(XX_TYPE_DOUBLE);
  yy_destructor(41,&yymsp[0].minor);
}
// 3022 "parser.c"
        break;
      case 77:
// 1096 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_type(XX_TYPE_BOOL);
  yy_destructor(42,&yymsp[0].minor);
}
// 3030 "parser.c"
        break;
      case 78:
// 1100 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_type(XX_TYPE_STRING);
  yy_destructor(43,&yymsp[0].minor);
}
// 3038 "parser.c"
        break;
      case 79:
// 1104 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_type(XX_TYPE_ARRAY);
  yy_destructor(44,&yymsp[0].minor);
}
// 3046 "parser.c"
        break;
      case 80:
// 1108 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_type(XX_TYPE_VAR);
  yy_destructor(45,&yymsp[0].minor);
}
// 3054 "parser.c"
        break;
      case 98:
// 1180 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_break_statement(status->scanner_state);
  yy_destructor(46,&yymsp[-1].minor);
  yy_destructor(27,&yymsp[0].minor);
}
// 3063 "parser.c"
        break;
      case 99:
// 1184 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_continue_statement(status->scanner_state);
  yy_destructor(47,&yymsp[-1].minor);
  yy_destructor(27,&yymsp[0].minor);
}
// 3072 "parser.c"
        break;
      case 100:
// 1188 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_if_statement(yymsp[-2].minor.yy279, NULL, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-3].minor);
  yy_destructor(29,&yymsp[-1].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 3082 "parser.c"
        break;
      case 101:
// 1192 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_if_statement(yymsp[-5].minor.yy279, NULL, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-6].minor);
  yy_destructor(29,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(29,&yymsp[-1].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 3095 "parser.c"
        break;
      case 102:
// 1196 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_if_statement(yymsp[-3].minor.yy279, yymsp[-1].minor.yy279, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-4].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 3105 "parser.c"
        break;
      case 103:
// 1200 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_if_statement(yymsp[-7].minor.yy279, yymsp[-5].minor.yy279, yymsp[-1].minor.yy279, status->scanner_state);
  yy_destructor(48,&yymsp[-8].minor);
  yy_destructor(29,&yymsp[-6].minor);
  yy_destructor(30,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 3118 "parser.c"
        break;
      case 104:
// 1204 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_if_statement(yymsp[-6].minor.yy279, yymsp[-4].minor.yy279, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-7].minor);
  yy_destructor(29,&yymsp[-5].minor);
  yy_destructor(30,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(29,&yymsp[-1].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 3131 "parser.c"
        break;
      case 105:
// 1208 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_if_statement(yymsp[-6].minor.yy279, NULL, yymsp[-1].minor.yy279, status->scanner_state);
  yy_destructor(48,&yymsp[-7].minor);
  yy_destructor(29,&yymsp[-5].minor);
  yy_destructor(30,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 3144 "parser.c"
        break;
      case 106:
// 1212 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_switch_statement(yymsp[-2].minor.yy279, NULL, status->scanner_state);
  yy_destructor(50,&yymsp[-3].minor);
  yy_destructor(29,&yymsp[-1].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 3154 "parser.c"
        break;
      case 107:
// 1216 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_switch_statement(yymsp[-3].minor.yy279, yymsp[-1].minor.yy279, status->scanner_state);
  yy_destructor(50,&yymsp[-4].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 3164 "parser.c"
        break;
      case 110:
// 1228 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_case_clause(yymsp[-2].minor.yy279, yymsp[0].minor.yy279, status->scanner_state);
  yy_destructor(51,&yymsp[-3].minor);
  yy_destructor(52,&yymsp[-1].minor);
}
// 3173 "parser.c"
        break;
      case 111:
// 1232 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_case_clause(NULL, yymsp[0].minor.yy279, status->scanner_state);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(52,&yymsp[-1].minor);
}
// 3182 "parser.c"
        break;
      case 112:
// 1236 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_loop_statement(NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(29,&yymsp[-1].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 3192 "parser.c"
        break;
      case 113:
// 1240 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_loop_statement(yymsp[-1].minor.yy279, status->scanner_state);
  yy_destructor(54,&yymsp[-3].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 3202 "parser.c"
        break;
      case 114:
// 1244 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_while_statement(yymsp[-2].minor.yy279, NULL, status->scanner_state);
  yy_destructor(55,&yymsp[-3].minor);
  yy_destructor(29,&yymsp[-1].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 3212 "parser.c"
        break;
      case 115:
// 1248 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_while_statement(yymsp[-3].minor.yy279, yymsp[-1].minor.yy279, status->scanner_state);
  yy_destructor(55,&yymsp[-4].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 3222 "parser.c"
        break;
      case 116:
// 1252 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_for_statement(yymsp[-3].minor.yy279, NULL, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy279, status->scanner_state);
  yy_destructor(56,&yymsp[-6].minor);
  yy_destructor(57,&yymsp[-4].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 3233 "parser.c"
        break;
      case 117:
// 1256 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_for_statement(yymsp[-3].minor.yy279, NULL, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy279, status->scanner_state);
  yy_destructor(56,&yymsp[-7].minor);
  yy_destructor(57,&yymsp[-5].minor);
  yy_destructor(58,&yymsp[-4].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 3245 "parser.c"
        break;
      case 118:
// 1260 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_for_statement(yymsp[-3].minor.yy279, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy279, status->scanner_state);
  yy_destructor(56,&yymsp[-8].minor);
  yy_destructor(5,&yymsp[-6].minor);
  yy_destructor(57,&yymsp[-4].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 3257 "parser.c"
        break;
      case 119:
// 1264 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_for_statement(yymsp[-3].minor.yy279, yymsp[-8].minor.yy0, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy279, status->scanner_state);
  yy_destructor(56,&yymsp[-9].minor);
  yy_destructor(5,&yymsp[-7].minor);
  yy_destructor(57,&yymsp[-5].minor);
  yy_destructor(58,&yymsp[-4].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 3270 "parser.c"
        break;
      case 120:
// 1268 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_let_statement(yymsp[-1].minor.yy279, status->scanner_state);
  yy_destructor(59,&yymsp[-2].minor);
  yy_destructor(27,&yymsp[0].minor);
}
// 3279 "parser.c"
        break;
      case 123:
// 1281 "parser.lemon"
{
	yygotominor.yy279 = json_object_new_string("assign");
  yy_destructor(34,&yymsp[0].minor);
}
// 3287 "parser.c"
        break;
      case 124:
// 1286 "parser.lemon"
{
	yygotominor.yy279 = json_object_new_string("add-assign");
  yy_destructor(60,&yymsp[0].minor);
}
// 3295 "parser.c"
        break;
      case 125:
// 1291 "parser.lemon"
{
	yygotominor.yy279 = json_object_new_string("sub-assign");
  yy_destructor(61,&yymsp[0].minor);
}
// 3303 "parser.c"
        break;
      case 126:
// 1295 "parser.lemon"
{
	yygotominor.yy279 = json_object_new_string("mult-assign");
  yy_destructor(62,&yymsp[0].minor);
}
// 3311 "parser.c"
        break;
      case 127:
// 1299 "parser.lemon"
{
	yygotominor.yy279 = json_object_new_string("div-assign");
  yy_destructor(63,&yymsp[0].minor);
}
// 3319 "parser.c"
        break;
      case 128:
// 1303 "parser.lemon"
{
	yygotominor.yy279 = json_object_new_string("concat-assign");
  yy_destructor(64,&yymsp[0].minor);
}
// 3327 "parser.c"
        break;
      case 129:
// 1308 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_let_assignment("variable", yymsp[-1].minor.yy279, yymsp[-2].minor.yy0, NULL, NULL, yymsp[0].minor.yy279, status->scanner_state);
}
// 3334 "parser.c"
        break;
      case 130:
// 1313 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_let_assignment("object-property", yymsp[-1].minor.yy279, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy279, status->scanner_state);
  yy_destructor(65,&yymsp[-3].minor);
}
// 3342 "parser.c"
        break;
      case 131:
// 1318 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_let_assignment("object-property-append", yymsp[-1].minor.yy279, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy279, status->scanner_state);
  yy_destructor(65,&yymsp[-5].minor);
  yy_destructor(66,&yymsp[-3].minor);
  yy_destructor(67,&yymsp[-2].minor);
}
// 3352 "parser.c"
        break;
      case 132:
// 1323 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_let_assignment("object-property-array-index", yymsp[-1].minor.yy279, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy279, yymsp[0].minor.yy279, status->scanner_state);
  yy_destructor(65,&yymsp[-6].minor);
  yy_destructor(66,&yymsp[-4].minor);
  yy_destructor(67,&yymsp[-2].minor);
}
// 3362 "parser.c"
        break;
      case 133:
// 1328 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_let_assignment("variable-append", yymsp[-1].minor.yy279, yymsp[-4].minor.yy0, NULL, NULL, yymsp[0].minor.yy279, status->scanner_state);
  yy_destructor(66,&yymsp[-3].minor);
  yy_destructor(67,&yymsp[-2].minor);
}
// 3371 "parser.c"
        break;
      case 134:
// 1333 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_let_assignment("array-index", yymsp[-1].minor.yy279, yymsp[-5].minor.yy0, NULL, yymsp[-3].minor.yy279, yymsp[0].minor.yy279, status->scanner_state);
  yy_destructor(66,&yymsp[-4].minor);
  yy_destructor(67,&yymsp[-2].minor);
}
// 3380 "parser.c"
        break;
      case 135:
// 1338 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_let_assignment("incr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(68,&yymsp[0].minor);
}
// 3388 "parser.c"
        break;
      case 136:
// 1343 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_let_assignment("decr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(69,&yymsp[0].minor);
}
// 3396 "parser.c"
        break;
      case 138:
// 1351 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_echo_statement(yymsp[-1].minor.yy279, status->scanner_state);
  yy_destructor(70,&yymsp[-2].minor);
  yy_destructor(27,&yymsp[0].minor);
}
// 3405 "parser.c"
        break;
      case 141:
// 1363 "parser.lemon"
{
	yygotominor.yy279 = yymsp[0].minor.yy279;;
}
// 3412 "parser.c"
        break;
      case 142:
// 1368 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_mcall_statement(yymsp[-1].minor.yy279, status->scanner_state);
  yy_destructor(27,&yymsp[0].minor);
}
// 3420 "parser.c"
        break;
      case 143:
// 1373 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_return_statement(yymsp[-1].minor.yy279, status->scanner_state);
  yy_destructor(71,&yymsp[-2].minor);
  yy_destructor(27,&yymsp[0].minor);
}
// 3429 "parser.c"
        break;
      case 144:
// 1378 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(72,&yymsp[-4].minor);
  yy_destructor(65,&yymsp[-2].minor);
  yy_destructor(27,&yymsp[0].minor);
}
// 3443 "parser.c"
        break;
      case 145:
// 1387 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(72,&yymsp[-5].minor);
  yy_destructor(66,&yymsp[-3].minor);
  yy_destructor(67,&yymsp[-1].minor);
  yy_destructor(27,&yymsp[0].minor);
}
// 3458 "parser.c"
        break;
      case 146:
// 1396 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_INTEGER, yymsp[-2].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(72,&yymsp[-5].minor);
  yy_destructor(66,&yymsp[-3].minor);
  yy_destructor(67,&yymsp[-1].minor);
  yy_destructor(27,&yymsp[0].minor);
}
// 3473 "parser.c"
        break;
      case 147:
// 1405 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_STRING, yymsp[-2].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(72,&yymsp[-5].minor);
  yy_destructor(66,&yymsp[-3].minor);
  yy_destructor(67,&yymsp[-1].minor);
  yy_destructor(27,&yymsp[0].minor);
}
// 3488 "parser.c"
        break;
      case 148:
// 1414 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_throw_exception(yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(75,&yymsp[-2].minor);
  yy_destructor(27,&yymsp[0].minor);
}
// 3497 "parser.c"
        break;
      case 149:
// 1419 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_throw_exception(yymsp[-4].minor.yy0, yymsp[-2].minor.yy279, status->scanner_state);
  yy_destructor(75,&yymsp[-6].minor);
  yy_destructor(23,&yymsp[-5].minor);
  yy_destructor(37,&yymsp[-3].minor);
  yy_destructor(38,&yymsp[-1].minor);
  yy_destructor(27,&yymsp[0].minor);
}
// 3509 "parser.c"
        break;
      case 150:
// 1423 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_declare_statement(XX_T_TYPE_INTEGER, yymsp[-1].minor.yy279, status->scanner_state);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(27,&yymsp[0].minor);
}
// 3518 "parser.c"
        break;
      case 151:
// 1427 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_declare_statement(XX_T_TYPE_UINTEGER, yymsp[-1].minor.yy279, status->scanner_state);
  yy_destructor(76,&yymsp[-2].minor);
  yy_destructor(27,&yymsp[0].minor);
}
// 3527 "parser.c"
        break;
      case 152:
// 1431 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_declare_statement(XX_T_TYPE_CHAR, yymsp[-1].minor.yy279, status->scanner_state);
  yy_destructor(77,&yymsp[-2].minor);
  yy_destructor(27,&yymsp[0].minor);
}
// 3536 "parser.c"
        break;
      case 153:
// 1435 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_declare_statement(XX_T_TYPE_UCHAR, yymsp[-1].minor.yy279, status->scanner_state);
  yy_destructor(78,&yymsp[-2].minor);
  yy_destructor(27,&yymsp[0].minor);
}
// 3545 "parser.c"
        break;
      case 154:
// 1439 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_declare_statement(XX_T_TYPE_LONG, yymsp[-1].minor.yy279, status->scanner_state);
  yy_destructor(79,&yymsp[-2].minor);
  yy_destructor(27,&yymsp[0].minor);
}
// 3554 "parser.c"
        break;
      case 155:
// 1443 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_declare_statement(XX_T_TYPE_ULONG, yymsp[-1].minor.yy279, status->scanner_state);
  yy_destructor(80,&yymsp[-2].minor);
  yy_destructor(27,&yymsp[0].minor);
}
// 3563 "parser.c"
        break;
      case 156:
// 1447 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_declare_statement(XX_T_TYPE_DOUBLE, yymsp[-1].minor.yy279, status->scanner_state);
  yy_destructor(41,&yymsp[-2].minor);
  yy_destructor(27,&yymsp[0].minor);
}
// 3572 "parser.c"
        break;
      case 157:
// 1451 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_declare_statement(XX_T_TYPE_STRING, yymsp[-1].minor.yy279, status->scanner_state);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(27,&yymsp[0].minor);
}
// 3581 "parser.c"
        break;
      case 158:
// 1455 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_declare_statement(XX_T_TYPE_BOOL, yymsp[-1].minor.yy279, status->scanner_state);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(27,&yymsp[0].minor);
}
// 3590 "parser.c"
        break;
      case 159:
// 1459 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_declare_statement(XX_T_TYPE_VAR, yymsp[-1].minor.yy279, status->scanner_state);
  yy_destructor(45,&yymsp[-2].minor);
  yy_destructor(27,&yymsp[0].minor);
}
// 3599 "parser.c"
        break;
      case 162:
// 1471 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_declare_variable(yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3606 "parser.c"
        break;
      case 163:
// 1475 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_declare_variable(yymsp[-2].minor.yy0, yymsp[0].minor.yy279, status->scanner_state);
  yy_destructor(34,&yymsp[-1].minor);
}
// 3614 "parser.c"
        break;
      case 165:
// 1483 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_expr("not", yymsp[0].minor.yy279, NULL, NULL, status->scanner_state);
  yy_destructor(24,&yymsp[-1].minor);
}
// 3622 "parser.c"
        break;
      case 166:
// 1487 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_expr("isset", yymsp[0].minor.yy279, NULL, NULL, status->scanner_state);
  yy_destructor(19,&yymsp[-1].minor);
}
// 3630 "parser.c"
        break;
      case 167:
// 1491 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_expr("equals", yymsp[-2].minor.yy279, yymsp[0].minor.yy279, NULL, status->scanner_state);
  yy_destructor(8,&yymsp[-1].minor);
}
// 3638 "parser.c"
        break;
      case 168:
// 1495 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_expr("not-equals", yymsp[-2].minor.yy279, yymsp[0].minor.yy279, NULL, status->scanner_state);
  yy_destructor(15,&yymsp[-1].minor);
}
// 3646 "parser.c"
        break;
      case 169:
// 1499 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_expr("identical", yymsp[-2].minor.yy279, yymsp[0].minor.yy279, NULL, status->scanner_state);
  yy_destructor(9,&yymsp[-1].minor);
}
// 3654 "parser.c"
        break;
      case 170:
// 1503 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_expr("not-identical", yymsp[-2].minor.yy279, yymsp[0].minor.yy279, NULL, status->scanner_state);
  yy_destructor(14,&yymsp[-1].minor);
}
// 3662 "parser.c"
        break;
      case 171:
// 1507 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_expr("less", yymsp[-2].minor.yy279, yymsp[0].minor.yy279, NULL, status->scanner_state);
  yy_destructor(10,&yymsp[-1].minor);
}
// 3670 "parser.c"
        break;
      case 172:
// 1511 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_expr("greater", yymsp[-2].minor.yy279, yymsp[0].minor.yy279, NULL, status->scanner_state);
  yy_destructor(11,&yymsp[-1].minor);
}
// 3678 "parser.c"
        break;
      case 173:
// 1515 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_expr("less-equal", yymsp[-2].minor.yy279, yymsp[0].minor.yy279, NULL, status->scanner_state);
  yy_destructor(12,&yymsp[-1].minor);
}
// 3686 "parser.c"
        break;
      case 174:
// 1519 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_expr("greater-equal", yymsp[-2].minor.yy279, yymsp[0].minor.yy279, NULL, status->scanner_state);
  yy_destructor(13,&yymsp[-1].minor);
}
// 3694 "parser.c"
        break;
      case 175:
// 1523 "parser.lemon"
{
	yygotominor.yy279 = yymsp[-1].minor.yy279;
  yy_destructor(37,&yymsp[-2].minor);
  yy_destructor(38,&yymsp[0].minor);
}
// 3703 "parser.c"
        break;
      case 176:
// 1527 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_expr("property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(65,&yymsp[-1].minor);
}
// 3711 "parser.c"
        break;
      case 177:
      case 223:
// 1531 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(66,&yymsp[-2].minor);
  yy_destructor(67,&yymsp[0].minor);
}
// 3721 "parser.c"
        break;
      case 178:
      case 224:
// 1536 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_INTEGER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(66,&yymsp[-2].minor);
  yy_destructor(67,&yymsp[0].minor);
}
// 3731 "parser.c"
        break;
      case 179:
      case 225:
// 1541 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_STRING, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(66,&yymsp[-2].minor);
  yy_destructor(67,&yymsp[0].minor);
}
// 3741 "parser.c"
        break;
      case 180:
// 1546 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_expr("add", yymsp[-2].minor.yy279, yymsp[0].minor.yy279, NULL, status->scanner_state);
  yy_destructor(16,&yymsp[-1].minor);
}
// 3749 "parser.c"
        break;
      case 181:
// 1551 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_expr("sub", yymsp[-2].minor.yy279, yymsp[0].minor.yy279, NULL, status->scanner_state);
  yy_destructor(17,&yymsp[-1].minor);
}
// 3757 "parser.c"
        break;
      case 182:
// 1556 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_expr("concat", yymsp[-2].minor.yy279, yymsp[0].minor.yy279, NULL, status->scanner_state);
  yy_destructor(18,&yymsp[-1].minor);
}
// 3765 "parser.c"
        break;
      case 183:
// 1561 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_expr("and", yymsp[-2].minor.yy279, yymsp[0].minor.yy279, NULL, status->scanner_state);
  yy_destructor(6,&yymsp[-1].minor);
}
// 3773 "parser.c"
        break;
      case 184:
// 1566 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_expr("or", yymsp[-2].minor.yy279, yymsp[0].minor.yy279, NULL, status->scanner_state);
  yy_destructor(7,&yymsp[-1].minor);
}
// 3781 "parser.c"
        break;
      case 185:
// 1571 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_expr("instanceof", yymsp[0].minor.yy279, NULL, NULL, status->scanner_state);
  yy_destructor(22,&yymsp[-1].minor);
}
// 3789 "parser.c"
        break;
      case 186:
// 1576 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_expr("fetch", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy279, NULL, status->scanner_state);
  yy_destructor(20,&yymsp[-3].minor);
  yy_destructor(5,&yymsp[-1].minor);
}
// 3798 "parser.c"
        break;
      case 187:
// 1581 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_expr("typeof", yymsp[0].minor.yy279, NULL, NULL, status->scanner_state);
  yy_destructor(21,&yymsp[-1].minor);
}
// 3806 "parser.c"
        break;
      case 188:
      case 212:
      case 216:
// 1586 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state);
}
// 3815 "parser.c"
        break;
      case 189:
      case 214:
      case 217:
// 1591 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_literal(XX_T_INTEGER, yymsp[0].minor.yy0, status->scanner_state);
}
// 3824 "parser.c"
        break;
      case 190:
      case 213:
      case 218:
// 1596 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_literal(XX_T_STRING, yymsp[0].minor.yy0, status->scanner_state);
}
// 3833 "parser.c"
        break;
      case 191:
      case 219:
// 1601 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_literal(XX_T_DOUBLE, yymsp[0].minor.yy0, status->scanner_state);
}
// 3841 "parser.c"
        break;
      case 192:
      case 220:
// 1606 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_literal(XX_T_NULL, NULL, status->scanner_state);
  yy_destructor(82,&yymsp[0].minor);
}
// 3850 "parser.c"
        break;
      case 193:
      case 222:
// 1611 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_literal(XX_T_TRUE, NULL, status->scanner_state);
  yy_destructor(83,&yymsp[0].minor);
}
// 3859 "parser.c"
        break;
      case 194:
      case 221:
// 1616 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_literal(XX_T_FALSE, NULL, status->scanner_state);
  yy_destructor(84,&yymsp[0].minor);
}
// 3868 "parser.c"
        break;
      case 195:
// 1621 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_expr("empty-array", NULL, NULL, NULL, status->scanner_state);
  yy_destructor(66,&yymsp[-1].minor);
  yy_destructor(67,&yymsp[0].minor);
}
// 3877 "parser.c"
        break;
      case 196:
// 1626 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_expr("array", yymsp[-1].minor.yy279, NULL, NULL, status->scanner_state);
  yy_destructor(66,&yymsp[-2].minor);
  yy_destructor(67,&yymsp[0].minor);
}
// 3886 "parser.c"
        break;
      case 197:
// 1631 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_new_instance(yymsp[0].minor.yy0, NULL, status->scanner_state);
  yy_destructor(23,&yymsp[-1].minor);
}
// 3894 "parser.c"
        break;
      case 198:
// 1636 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_new_instance(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(23,&yymsp[-3].minor);
  yy_destructor(37,&yymsp[-1].minor);
  yy_destructor(38,&yymsp[0].minor);
}
// 3904 "parser.c"
        break;
      case 199:
// 1641 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_new_instance(yymsp[-3].minor.yy0, yymsp[-1].minor.yy279, status->scanner_state);
  yy_destructor(23,&yymsp[-4].minor);
  yy_destructor(37,&yymsp[-2].minor);
  yy_destructor(38,&yymsp[0].minor);
}
// 3914 "parser.c"
        break;
      case 200:
// 1646 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_fcall(yymsp[-3].minor.yy0, yymsp[-1].minor.yy279, status->scanner_state);
  yy_destructor(37,&yymsp[-2].minor);
  yy_destructor(38,&yymsp[0].minor);
}
// 3923 "parser.c"
        break;
      case 201:
// 1651 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_fcall(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(37,&yymsp[-1].minor);
  yy_destructor(38,&yymsp[0].minor);
}
// 3932 "parser.c"
        break;
      case 203:
// 1661 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_mcall(yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy279, status->scanner_state);
  yy_destructor(65,&yymsp[-4].minor);
  yy_destructor(37,&yymsp[-2].minor);
  yy_destructor(38,&yymsp[0].minor);
}
// 3942 "parser.c"
        break;
      case 204:
// 1666 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_mcall(yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(65,&yymsp[-3].minor);
  yy_destructor(37,&yymsp[-1].minor);
  yy_destructor(38,&yymsp[0].minor);
}
// 3952 "parser.c"
        break;
      case 210:
// 1690 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_array_item(yymsp[-2].minor.yy279, yymsp[0].minor.yy279, status->scanner_state);
  yy_destructor(52,&yymsp[-1].minor);
}
// 3960 "parser.c"
        break;
      case 211:
// 1694 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_array_item(NULL, yymsp[0].minor.yy279, status->scanner_state);
}
// 3967 "parser.c"
        break;
      case 227:
// 1761 "parser.lemon"
{
	yygotominor.yy279 = xx_ret_comment(yymsp[0].minor.yy0, status->scanner_state);
}
// 3974 "parser.c"
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
// 739 "parser.lemon"


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

// 4050 "parser.c"
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
