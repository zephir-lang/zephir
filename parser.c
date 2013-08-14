/* Driver template for the LEMON parser generator.
** The author disclaims copyright to this source code.
*/
/* First off, code is include which follows the "include" declaration
** in the input file. */
#include <stdio.h>
// 41 "parser.lemon"


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

static json_object *xx_ret_class_property(int visibility, xx_parser_token *T, json_object *default_value, xx_parser_token *D, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	if (visibility == XX_T_PUBLIC) {
		json_object_object_add(ret, "visibility", json_object_new_string("public"));
	} else {
		json_object_object_add(ret, "visibility", json_object_new_string("protected"));
	}
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

static json_object *xx_ret_class_method(int visibility, xx_parser_token *T, json_object *parameters, json_object *statements, xx_parser_token *D, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	if (visibility == XX_T_PUBLIC) {
		json_object_object_add(ret, "visibility", json_object_new_string("public"));
	} else {
		json_object_object_add(ret, "visibility", json_object_new_string("protected"));
	}
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


// 697 "parser.c"
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
#define YYNOCODE 135
#define YYACTIONTYPE unsigned short int
#define xx_TOKENTYPE xx_parser_token*
typedef union {
  xx_TOKENTYPE yy0;
  json_object* yy95;
  int yy269;
} YYMINORTYPE;
#define YYSTACKDEPTH 100
#define xx_ARG_SDECL xx_parser_status *status;
#define xx_ARG_PDECL ,xx_parser_status *status
#define xx_ARG_FETCH xx_parser_status *status = yypParser->status
#define xx_ARG_STORE yypParser->status = status
#define YYNSTATE 458
#define YYNRULE 211
#define YYERRORSYMBOL 77
#define YYERRSYMDT yy269
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
 /*     0 */   286,  389,   17,  404,   25,  430,  392,  289,  670,    1,
 /*    10 */     2,  457,    4,    5,    6,  265,  274,  280,  277,  143,
 /*    20 */   283,   44,   46,   48,  296,   57,   90,  187,  190,   68,
 /*    30 */    72,   77,  176,  189,   84,   13,   16,  431,  437,  436,
 /*    40 */   259,  430,  392,  257,  286,  229,  238,  241,   27,  143,
 /*    50 */   256,  289,   91,   92,   93,   94,   95,  187,  209,  265,
 /*    60 */   274,  280,  277,  188,  283,   44,   46,   48,  349,   57,
 /*    70 */   348,  331,  335,   68,   72,   77,   49,   79,   84,  309,
 /*    80 */   178,  177,    8,  171,  172,  174,  173,  187,  286,  229,
 /*    90 */   238,  241,  316,  458,  256,  289,  169,  170,    9,  171,
 /*   100 */   172,  174,  173,  265,  274,  280,  277,  326,  283,   44,
 /*   110 */    46,   48,   98,   57,   14,   90,   97,   68,   72,   77,
 /*   120 */   187,   11,   84,  441,   16,  431,  437,  436,   18,  430,
 /*   130 */   392,  323,  286,  229,  238,  241,   52,  200,  256,  289,
 /*   140 */   186,   91,   92,   93,   94,   95,   20,  265,  274,  280,
 /*   150 */   277,   21,  283,   44,   46,   48,  322,   57,  348,  331,
 /*   160 */   335,   68,   72,   77,  140,  193,   84,  448,   16,  431,
 /*   170 */   437,  436,   22,  430,  392,  185,  286,  229,  238,  241,
 /*   180 */   315,   15,  256,  289,  405,  418,  390,  424,  399,  391,
 /*   190 */   392,  265,  274,  280,  277,  143,  283,   44,   46,   48,
 /*   200 */    12,   57,  438,  187,  184,   68,   72,   77,   80,  189,
 /*   210 */    84,  454,   16,  431,  437,  436,  187,  430,  392,   45,
 /*   220 */   286,  229,  238,  241,   56,  474,  256,  289,  405,  418,
 /*   230 */   167,  424,  399,  167,   50,  265,  274,  280,  277,  143,
 /*   240 */   283,   44,   46,   48,  356,   57,  371,  187,  201,   68,
 /*   250 */    72,   77,  478,  189,   84,  393,   19,  162,  380,  399,
 /*   260 */     3,    4,    5,    6,  286,  229,  238,  241,  310,  143,
 /*   270 */   256,  289,  160,  166,  179,  164,  175,  187,  262,  265,
 /*   280 */   274,  280,  277,  189,  283,   44,   46,   48,  365,   57,
 /*   290 */   348,  331,  335,   68,   72,   77,   47,  475,   84,  234,
 /*   300 */   405,  418,   54,  424,  399,  230,  235,  187,  286,  229,
 /*   310 */   238,  241,   71,   53,  256,  289,  432,  107,  435,  436,
 /*   320 */   161,  391,  392,  265,  274,  280,  277,  234,  283,   44,
 /*   330 */    46,   48,   98,   57,  233,  187,  208,   68,   72,   77,
 /*   340 */   187,  443,   84,    7,  405,  418,   10,  424,  399,   59,
 /*   350 */   444,  289,  286,  229,  238,  241,  308,  450,  256,  289,
 /*   360 */   405,  418,   63,  424,  399,  109,  111,  265,  274,  280,
 /*   370 */   277,   60,  283,   44,   46,   48,   66,   57,  126,  128,
 /*   380 */   130,   68,   72,   77,  456,   69,   84,  405,  418,   65,
 /*   390 */   424,  399,  167,   64,   74,  311,  286,  229,  238,  241,
 /*   400 */    76,  477,  256,  289,  355,   19,   87,  380,  327,  331,
 /*   410 */   335,  265,  274,  280,  277,   62,  283,   44,   46,   48,
 /*   420 */    58,   57,   78,  309,  217,   68,   72,   77,   86,  213,
 /*   430 */    84,  187,  187,  163,  164,  175,  406,   64,  412,  311,
 /*   440 */   286,  229,  238,  241,   83,  481,  256,  289,  355,   19,
 /*   450 */   356,  380,  371,  394,  407,  265,  274,  280,  277,  217,
 /*   460 */   283,   44,   46,   48,  222,   57,  357,  187,   88,   68,
 /*   470 */    72,   77,  479,   98,   84,  434,  418,  212,  424,   89,
 /*   480 */   406,  187,  412,  394,  286,  229,  238,  241,  295,  482,
 /*   490 */   256,  289,  355,   19,  105,  380,   61,  314,   96,  265,
 /*   500 */   274,  280,  277,  186,  283,   44,   46,   48,  104,   57,
 /*   510 */    85,  228,  106,   68,   72,   77,  480,   98,   84,  355,
 /*   520 */    19,  216,  380,  433,  404,  187,  141,  392,  286,  229,
 /*   530 */   238,  241,  302,  476,  256,  289,  355,   19,  191,  380,
 /*   540 */   243,  246,  268,  265,  274,  280,  277,  204,  283,   44,
 /*   550 */    46,   48,  108,   57,  142,  187,  194,   68,   72,   77,
 /*   560 */    73,   98,   84,  309,  267,  221,  247,  266,  273,  187,
 /*   570 */   147,  187,  286,  229,  238,  241,  307,   98,  256,  289,
 /*   580 */   110,  225,  275,  273,  413,  187,  419,  265,  274,  280,
 /*   590 */   277,  101,  283,   44,   46,   48,  372,   57,   20,  187,
 /*   600 */   113,   68,   72,   77,  196,  198,   84,  186,  187,  278,
 /*   610 */   273,  281,  273,  115,  250,  253,  286,  229,  238,  241,
 /*   620 */   321,  187,  256,  289,  284,  273,  447,  117,  451,  112,
 /*   630 */   148,  265,  274,  280,  277,  187,  283,   44,   46,   48,
 /*   640 */   119,   57,  202,  425,  121,   68,   72,   77,  187,  123,
 /*   650 */    84,  125,  187,  149,  165,  381,  181,  187,  653,  187,
 /*   660 */   286,  229,  238,  241,  320,  127,  256,  289,  654,  655,
 /*   670 */   182,  206,  195,  187,  197,  265,  274,  280,  277,  129,
 /*   680 */   283,   44,   46,   48,  237,   57,  199,  187,  131,   68,
 /*   690 */    72,   77,  133,  135,   84,  137,  187,  207,  242,  211,
 /*   700 */   187,  187,  186,  187,  286,  229,  238,  241,  325,  145,
 /*   710 */   256,  289,  326,  244,  214,  215,  220,  187,  223,  265,
 /*   720 */   274,  280,  277,  151,  283,   44,   46,   48,  245,   57,
 /*   730 */   224,  187,  239,   68,   72,   77,  232,  263,   84,  268,
 /*   740 */   187,  248,  249,  251,  252,  255,  258,  350,  286,  229,
 /*   750 */   238,  241,  352,  268,  256,  289,  254,  264,  231,  260,
 /*   760 */   261,  276,  270,  265,  274,  280,  277,  268,  283,   44,
 /*   770 */    46,   48,  269,   57,  271,  279,  292,   68,   72,   77,
 /*   780 */   268,  326,   84,  299,  187,  272,  287,  288,  312,  282,
 /*   790 */   297,  187,  286,  229,  238,  241,  354,  304,  256,  289,
 /*   800 */   298,  317,  285,  318,  329,  187,  330,  265,  274,  280,
 /*   810 */   277,  332,  283,   44,   46,   48,  366,   57,  333,  408,
 /*   820 */   414,   68,   72,   77,  336,  337,   84,  409,  415,  340,
 /*   830 */   341,  334,  338,  420,  520,  521,  286,  229,  238,  241,
 /*   840 */   362,  421,  256,  289,  522,  351,  359,  426,  523,  524,
 /*   850 */   525,  265,  274,  280,  277,  427,  283,   44,   46,   48,
 /*   860 */   526,   57,  358,  357,  361,   68,   72,   77,  367,  372,
 /*   870 */    84,  373,  374,  376,  382,  385,  381,  375,  395,  383,
 /*   880 */   286,  229,  238,  241,  364,  396,  256,  289,  397,  384,
 /*   890 */   398,  400,  401,  402,  403,  265,  274,  280,  277,  410,
 /*   900 */   283,   44,   46,   48,  411,   57,  416,  417,  422,   68,
 /*   910 */    72,   77,  423,  429,   84,  428,  439,  440,  347,  442,
 /*   920 */   445,  446,  452,  449,  286,  229,  238,  241,  368,  453,
 /*   930 */   256,  289,  455,  347,  347,  347,  347,  347,  347,  265,
 /*   940 */   274,  280,  277,  347,  283,   44,   46,   48,  347,   57,
 /*   950 */   347,  347,  347,   68,   72,   77,  347,  347,   84,  347,
 /*   960 */   347,  347,  347,  347,  347,  347,  347,  347,  286,  229,
 /*   970 */   238,  241,  370,  347,  256,  289,  347,  347,  347,  347,
 /*   980 */   347,  347,  347,  265,  274,  280,  277,  347,  283,   44,
 /*   990 */    46,   48,  347,   57,  347,  347,  347,   68,   72,   77,
 /*  1000 */   347,  347,   84,  347,  347,  347,  347,  347,  347,  347,
 /*  1010 */   347,  347,  286,  229,  238,  241,  377,  347,  256,  289,
 /*  1020 */   347,  347,  347,  347,  347,  347,  347,  265,  274,  280,
 /*  1030 */   277,  347,  283,   44,   46,   48,  347,   57,  347,  347,
 /*  1040 */   347,   68,   72,   77,  347,  347,   84,  347,  347,  347,
 /*  1050 */   347,  347,  347,  347,  347,  347,  286,  229,  238,  241,
 /*  1060 */   379,  347,  256,  289,  347,  347,  347,  347,  347,  347,
 /*  1070 */   347,  265,  274,  280,  277,  347,  283,   44,   46,   48,
 /*  1080 */   347,   57,  347,  347,  347,   68,   72,   77,  347,  347,
 /*  1090 */    84,  347,  347,  347,  347,  347,  347,  347,  347,  347,
 /*  1100 */   286,  229,  238,  241,  386,  347,  256,  289,  347,  347,
 /*  1110 */   347,  347,  347,  347,  347,  265,  274,  280,  277,  347,
 /*  1120 */   283,   44,   46,   48,  347,   57,  347,  347,  347,   68,
 /*  1130 */    72,   77,  347,  347,   84,  347,  347,  347,  347,  347,
 /*  1140 */   347,  347,  347,  347,  286,  229,  238,  241,  388,  347,
 /*  1150 */   256,  289,  347,  347,  347,  347,  347,  347,  347,  265,
 /*  1160 */   274,  280,  277,  347,  283,   44,   46,   48,  347,   57,
 /*  1170 */   347,  347,  347,   68,   72,   77,  347,  347,   84,  347,
 /*  1180 */   347,  347,  347,  347,  347,  286,  347,  347,  347,  229,
 /*  1190 */   238,  241,  289,  347,  256,  347,  347,  347,  347,  347,
 /*  1200 */   265,  274,  280,  277,  347,  283,   44,   46,   48,  347,
 /*  1210 */    57,  347,  168,  459,   68,   72,   77,  347,  347,   84,
 /*  1220 */   347,  347,  347,  347,  347,  347,  347,  347,  347,   43,
 /*  1230 */   229,  238,  241,  347,    7,  256,  347,   10,  347,   26,
 /*  1240 */   347,  444,  289,  290,   29,   30,   31,   32,   33,   34,
 /*  1250 */    35,   36,   37,   38,   39,   40,   41,   42,   43,  347,
 /*  1260 */   169,  170,  347,  171,  172,  174,  173,  347,   51,  236,
 /*  1270 */   347,  347,  290,   29,   30,   31,   32,   33,   34,   35,
 /*  1280 */    36,   37,   38,   39,   40,   41,   42,   43,  347,  347,
 /*  1290 */   347,  347,  347,  347,  347,  347,  347,   55,  236,  347,
 /*  1300 */   347,  290,   29,   30,   31,   32,   33,   34,   35,   36,
 /*  1310 */    37,   38,   39,   40,   41,   42,   43,  347,  347,  347,
 /*  1320 */   347,  347,  347,  347,  347,  347,   67,  236,  347,  347,
 /*  1330 */   290,   29,   30,   31,   32,   33,   34,   35,   36,   37,
 /*  1340 */    38,   39,   40,   41,   42,  347,  347,  347,  347,  347,
 /*  1350 */   347,  347,  347,   43,  347,  347,  236,  347,  347,  347,
 /*  1360 */   347,  347,  347,   70,  347,  347,  347,  290,   29,   30,
 /*  1370 */    31,   32,   33,   34,   35,   36,   37,   38,   39,   40,
 /*  1380 */    41,   42,   43,  347,  347,  347,  347,  347,  347,  347,
 /*  1390 */   347,  347,   75,  236,  347,  347,  290,   29,   30,   31,
 /*  1400 */    32,   33,   34,   35,   36,   37,   38,   39,   40,   41,
 /*  1410 */    42,   43,  347,  347,  347,  347,  347,  347,  347,  347,
 /*  1420 */   347,   82,  236,  347,  347,  290,   29,   30,   31,   32,
 /*  1430 */    33,   34,   35,   36,   37,   38,   39,   40,   41,   42,
 /*  1440 */   347,  347,  347,  347,  347,  347,  347,  347,   43,  347,
 /*  1450 */   347,  236,  347,  347,  347,  347,  347,  347,  294,  347,
 /*  1460 */   347,  347,  290,   29,   30,   31,   32,   33,   34,   35,
 /*  1470 */    36,   37,   38,   39,   40,   41,   42,   43,  347,  347,
 /*  1480 */   347,  347,  347,  347,  347,  347,  347,  301,  236,  347,
 /*  1490 */   347,  290,   29,   30,   31,   32,   33,   34,   35,   36,
 /*  1500 */    37,   38,   39,   40,   41,   42,   43,  347,  347,  347,
 /*  1510 */   347,  347,  347,  347,  347,  347,  306,  236,  347,  347,
 /*  1520 */   290,   29,   30,   31,   32,   33,   34,   35,   36,   37,
 /*  1530 */    38,   39,   40,   41,   42,  347,  347,  347,  347,  347,
 /*  1540 */   347,  347,  347,   43,  347,  347,  236,  347,  347,  347,
 /*  1550 */   347,  347,  347,  313,  347,  347,  347,  290,   29,   30,
 /*  1560 */    31,   32,   33,   34,   35,   36,   37,   38,   39,   40,
 /*  1570 */    41,   42,   43,  347,  347,  347,  347,  347,  347,  347,
 /*  1580 */   347,  347,  319,  236,  347,  347,  290,   29,   30,   31,
 /*  1590 */    32,   33,   34,   35,   36,   37,   38,   39,   40,   41,
 /*  1600 */    42,   43,  347,  347,  347,  347,  347,  347,  347,  347,
 /*  1610 */   347,  324,  236,  347,  347,  290,   29,   30,   31,   32,
 /*  1620 */    33,   34,   35,   36,   37,   38,   39,   40,   41,   42,
 /*  1630 */   347,  347,  347,  347,  347,  347,  347,  347,   43,  347,
 /*  1640 */   347,  236,  347,  347,  347,  347,  347,  347,  353,  347,
 /*  1650 */   347,  347,  290,   29,   30,   31,   32,   33,   34,   35,
 /*  1660 */    36,   37,   38,   39,   40,   41,   42,   43,  347,  347,
 /*  1670 */   347,  347,  347,  347,  347,  347,  347,  363,  236,  347,
 /*  1680 */   347,  290,   29,   30,   31,   32,   33,   34,   35,   36,
 /*  1690 */    37,   38,   39,   40,   41,   42,   43,  347,  347,  347,
 /*  1700 */   347,  347,  347,  347,  347,  347,  369,  236,  347,  347,
 /*  1710 */   290,   29,   30,   31,   32,   33,   34,   35,   36,   37,
 /*  1720 */    38,   39,   40,   41,   42,  347,  347,  347,  347,  347,
 /*  1730 */   347,  347,  347,   43,  347,  347,  236,  347,  347,  347,
 /*  1740 */   347,  347,  347,  378,  347,  347,  347,  290,   29,   30,
 /*  1750 */    31,   32,   33,   34,   35,   36,   37,   38,   39,   40,
 /*  1760 */    41,   42,   43,  347,  347,  347,  347,  347,  347,  347,
 /*  1770 */   347,  347,  387,  236,  347,  347,  290,   29,   30,   31,
 /*  1780 */    32,   33,   34,   35,   36,   37,   38,   39,   40,   41,
 /*  1790 */    42,   43,  347,  347,  347,  347,  347,  347,  347,  347,
 /*  1800 */   347,  347,  236,  347,  347,   28,   29,   30,   31,   32,
 /*  1810 */    33,   34,   35,   36,   37,   38,   39,   40,   41,   42,
 /*  1820 */   347,  103,  146,  150,  144,  180,  100,  347,  139,  347,
 /*  1830 */   347,  236,  347,  347,  347,  103,  146,  150,  144,  180,
 /*  1840 */   100,  136,  139,  347,  103,  146,  150,  144,  180,  100,
 /*  1850 */   347,  139,  347,  347,  347,  136,  192,  347,  347,  347,
 /*  1860 */   347,  291,  347,  347,  136,  183,  347,  347,  347,  158,
 /*  1870 */   347,  347,  347,  347,  347,  347,  152,  153,  347,  154,
 /*  1880 */   155,  156,  157,  158,  347,  347,  347,  347,  347,  347,
 /*  1890 */   152,  153,  158,  154,  155,  156,  157,  347,  347,  152,
 /*  1900 */   153,  347,  154,  155,  156,  157,  103,  146,  150,  144,
 /*  1910 */   180,  100,  347,  139,   99,  114,  118,  120,  122,  124,
 /*  1920 */   116,  102,  126,  128,  130,  347,  136,  203,  347,  103,
 /*  1930 */   146,  150,  144,  180,  100,  347,  139,  347,  347,  339,
 /*  1940 */   347,  347,  347,  103,  146,  150,  144,  180,  100,  136,
 /*  1950 */   139,  347,  347,  347,  158,  328,  347,  347,  347,  347,
 /*  1960 */   347,  152,  153,  136,  154,  155,  156,  157,  347,   23,
 /*  1970 */   342,  343,  344,  345,  346,  347,  347,  158,  210,  347,
 /*  1980 */   347,  347,  347,  347,  152,  153,  347,  154,  155,  156,
 /*  1990 */   157,  158,  219,  347,  347,  347,  347,  339,  152,  153,
 /*  2000 */   347,  154,  155,  156,  157,  103,  146,  150,  144,  180,
 /*  2010 */   100,  347,  139,  328,  103,  146,  150,  144,  180,  100,
 /*  2020 */    90,  139,  347,  347,  347,  136,  347,  360,  342,  343,
 /*  2030 */   344,  345,  346,  347,  136,  347,  347,  347,  347,  347,
 /*  2040 */   347,  347,  347,  347,  347,  303,   91,   92,   93,   94,
 /*  2050 */    95,  205,  218,  158,  226,  227,  347,  347,  347,  347,
 /*  2060 */   152,  153,  158,  154,  155,  156,  157,  347,  347,  152,
 /*  2070 */   153,  347,  154,  155,  156,  157,  132,  134,   99,  114,
 /*  2080 */   118,  120,  122,  124,  116,  102,  126,  128,  130,  347,
 /*  2090 */   347,  347,  347,  347,  347,  347,  347,  347,  347,   81,
 /*  2100 */   347,  347,  132,  134,   99,  114,  118,  120,  122,  124,
 /*  2110 */   116,  102,  126,  128,  130,  132,  134,   99,  114,  118,
 /*  2120 */   120,  122,  124,  116,  102,  126,  128,  130,  347,  347,
 /*  2130 */   347,  347,  347,  347,  347,  347,  138,  347,  347,  132,
 /*  2140 */   134,   99,  114,  118,  120,  122,  124,  116,  102,  126,
 /*  2150 */   128,  130,  347,  347,  347,  347,  347,  347,  347,  347,
 /*  2160 */   240,  347,  347,  132,  134,   99,  114,  118,  120,  122,
 /*  2170 */   124,  116,  102,  126,  128,  130,  347,  583,  347,  347,
 /*  2180 */   347,  347,  347,  347,  347,  347,  293,  347,  347,  132,
 /*  2190 */   134,   99,  114,  118,  120,  122,  124,  116,  102,  126,
 /*  2200 */   128,  130,  347,  347,  347,  347,  347,  347,  347,  347,
 /*  2210 */   347,  347,  300,  347,  347,  132,  134,   99,  114,  118,
 /*  2220 */   120,  122,  124,  116,  102,  126,  128,  130,  347,  347,
 /*  2230 */   347,  347,  347,  347,  347,  347,  347,  347,  305,  176,
 /*  2240 */   347,  132,  134,   99,  114,  118,  120,  122,  124,  116,
 /*  2250 */   102,  126,  128,  130,  347,  347,  347,  347,  347,  347,
 /*  2260 */   347,  347,  347,  347,  667,  347,  347,  132,  134,   99,
 /*  2270 */   114,  118,  120,  122,  124,  116,  102,  126,  128,  130,
 /*  2280 */   339,  159,  347,  347,  347,  347,  339,  178,  177,  347,
 /*  2290 */   171,  172,  174,  173,  347,  347,  328,  347,  347,   24,
 /*  2300 */   347,  347,  328,  347,  347,  347,  347,  347,  347,  347,
 /*  2310 */   347,  342,  343,  344,  345,  346,  347,  342,  343,  344,
 /*  2320 */   345,  346,  347,
};
static YYCODETYPE yy_lookahead[] = {
 /*     0 */    22,   87,   88,   89,   26,   91,   92,   29,   78,   79,
 /*    10 */    80,   81,   82,   83,   84,   37,   38,   39,   40,  116,
 /*    20 */    42,   43,   44,   45,    1,   47,   31,  124,  125,   51,
 /*    30 */    52,   53,   22,  130,   56,   85,   86,   87,   88,   89,
 /*    40 */    19,   91,   92,   22,   22,   67,   68,   69,   26,  116,
 /*    50 */    72,   29,   57,   58,   59,   60,   61,  124,   63,   37,
 /*    60 */    38,   39,   40,  130,   42,   43,   44,   45,   93,   47,
 /*    70 */    95,   96,   97,   51,   52,   53,  113,   54,   56,  116,
 /*    80 */    70,   71,   22,   73,   74,   75,   76,  124,   22,   67,
 /*    90 */    68,   69,   26,    0,   72,   29,   70,   71,   23,   73,
 /*   100 */    74,   75,   76,   37,   38,   39,   40,    1,   42,   43,
 /*   110 */    44,   45,  116,   47,   26,   31,  120,   51,   52,   53,
 /*   120 */   124,   22,   56,   85,   86,   87,   88,   89,   92,   91,
 /*   130 */    92,   25,   22,   67,   68,   69,   26,   35,   72,   29,
 /*   140 */     1,   57,   58,   59,   60,   61,   34,   37,   38,   39,
 /*   150 */    40,   22,   42,   43,   44,   45,   93,   47,   95,   96,
 /*   160 */    97,   51,   52,   53,   62,   63,   56,   85,   86,   87,
 /*   170 */    88,   89,   35,   91,   92,   36,   22,   67,   68,   69,
 /*   180 */    26,   26,   72,   29,   29,   30,   88,   32,   33,   91,
 /*   190 */    92,   37,   38,   39,   40,  116,   42,   43,   44,   45,
 /*   200 */    25,   47,   27,  124,  125,   51,   52,   53,  116,  130,
 /*   210 */    56,   85,   86,   87,   88,   89,  124,   91,   92,   23,
 /*   220 */    22,   67,   68,   69,   26,   26,   72,   29,   29,   30,
 /*   230 */    90,   32,   33,   90,   25,   37,   38,   39,   40,  116,
 /*   240 */    42,   43,   44,   45,   30,   47,   32,  124,  125,   51,
 /*   250 */    52,   53,   26,  130,   56,   29,   30,    1,   32,   33,
 /*   260 */    81,   82,   83,   84,   22,   67,   68,   69,   26,  116,
 /*   270 */    72,   29,  129,  133,  131,  132,  133,  124,  125,   37,
 /*   280 */    38,   39,   40,  130,   42,   43,   44,   45,   93,   47,
 /*   290 */    95,   96,   97,   51,   52,   53,   23,   26,   56,  116,
 /*   300 */    29,   30,   25,   32,   33,  122,  123,  124,   22,   67,
 /*   310 */    68,   69,   26,   46,   72,   29,   86,   22,   88,   89,
 /*   320 */    64,   91,   92,   37,   38,   39,   40,  116,   42,   43,
 /*   330 */    44,   45,  116,   47,  123,  124,  120,   51,   52,   53,
 /*   340 */   124,   26,   56,   21,   29,   30,   24,   32,   33,   25,
 /*   350 */    28,   29,   22,   67,   68,   69,   26,   26,   72,   29,
 /*   360 */    29,   30,  115,   32,   33,   70,   71,   37,   38,   39,
 /*   370 */    40,   26,   42,   43,   44,   45,   49,   47,   12,   13,
 /*   380 */    14,   51,   52,   53,   26,   25,   56,   29,   30,   90,
 /*   390 */    32,   33,   90,   48,   25,   50,   22,   67,   68,   69,
 /*   400 */    26,   26,   72,   29,   29,   30,    1,   32,   95,   96,
 /*   410 */    97,   37,   38,   39,   40,   26,   42,   43,   44,   45,
 /*   420 */   113,   47,   22,  116,  116,   51,   52,   53,   23,  121,
 /*   430 */    56,  124,  124,  131,  132,  133,   30,   48,   32,   50,
 /*   440 */    22,   67,   68,   69,   26,   26,   72,   29,   29,   30,
 /*   450 */    30,   32,   32,   33,   22,   37,   38,   39,   40,  116,
 /*   460 */    42,   43,   44,   45,  121,   47,   34,  124,  118,   51,
 /*   470 */    52,   53,   26,  116,   56,   29,   30,  120,   32,   22,
 /*   480 */    30,  124,   32,   33,   22,   67,   68,   69,   26,   26,
 /*   490 */    72,   29,   29,   30,   22,   32,  114,  115,  119,   37,
 /*   500 */    38,   39,   40,    1,   42,   43,   44,   45,  128,   47,
 /*   510 */   117,  118,   63,   51,   52,   53,   26,  116,   56,   29,
 /*   520 */    30,  120,   32,   88,   89,  124,   22,   92,   22,   67,
 /*   530 */    68,   69,   26,   26,   72,   29,   29,   30,   36,   32,
 /*   540 */    62,   63,    1,   37,   38,   39,   40,  116,   42,   43,
 /*   550 */    44,   45,   64,   47,   35,  124,   22,   51,   52,   53,
 /*   560 */   113,  116,   56,  116,   23,  120,   22,  126,  127,  124,
 /*   570 */    22,  124,   22,   67,   68,   69,   26,  116,   72,   29,
 /*   580 */    64,  120,  126,  127,   22,  124,   22,   37,   38,   39,
 /*   590 */    40,  116,   42,   43,   44,   45,   34,   47,   34,  124,
 /*   600 */   116,   51,   52,   53,   70,   71,   56,    1,  124,  126,
 /*   610 */   127,  126,  127,  116,   70,   71,   22,   67,   68,   69,
 /*   620 */    26,  124,   72,   29,  126,  127,   25,  116,   27,   64,
 /*   630 */     1,   37,   38,   39,   40,  124,   42,   43,   44,   45,
 /*   640 */   116,   47,   36,   22,  116,   51,   52,   53,  124,  116,
 /*   650 */    56,  116,  124,  128,   49,   34,   22,  124,   49,  124,
 /*   660 */    22,   67,   68,   69,   26,  116,   72,   29,   49,   49,
 /*   670 */    35,   22,   64,  124,   64,   37,   38,   39,   40,  116,
 /*   680 */    42,   43,   44,   45,   23,   47,   64,  124,  116,   51,
 /*   690 */    52,   53,  116,  116,   56,  116,  124,  119,   22,  119,
 /*   700 */   124,  124,    1,  124,   22,   67,   68,   69,   26,  116,
 /*   710 */    72,   29,    1,   22,   64,  119,  119,  124,   64,   37,
 /*   720 */    38,   39,   40,  116,   42,   43,   44,   45,   23,   47,
 /*   730 */   119,  124,  116,   51,   52,   53,    1,   36,   56,    1,
 /*   740 */   124,   64,   23,   64,   23,   23,   23,   36,   22,   67,
 /*   750 */    68,   69,   26,    1,   72,   29,   64,   23,   23,   22,
 /*   760 */    35,   23,   22,   37,   38,   39,   40,    1,   42,   43,
 /*   770 */    44,   45,  127,   47,   31,   23,  116,   51,   52,   53,
 /*   780 */     1,    1,   56,  116,  124,   90,   62,   22,   49,   23,
 /*   790 */    22,  124,   22,   67,   68,   69,   26,  116,   72,   29,
 /*   800 */    54,   46,   23,   25,   31,  124,   90,   37,   38,   39,
 /*   810 */    40,   22,   42,   43,   44,   45,   36,   47,   31,   23,
 /*   820 */    23,   51,   52,   53,   22,   31,   56,   31,   31,   22,
 /*   830 */     7,   90,   90,   23,   22,   22,   22,   67,   68,   69,
 /*   840 */    26,   31,   72,   29,   22,   25,   35,   23,   22,   22,
 /*   850 */    22,   37,   38,   39,   40,   31,   42,   43,   44,   45,
 /*   860 */    22,   47,   22,   34,   25,   51,   52,   53,   25,   34,
 /*   870 */    56,   22,   35,   25,   22,   25,   34,   36,   22,   35,
 /*   880 */    22,   67,   68,   69,   26,   31,   72,   29,   90,   36,
 /*   890 */    23,   22,   31,   90,   23,   37,   38,   39,   40,   90,
 /*   900 */    42,   43,   44,   45,   23,   47,   90,   23,   90,   51,
 /*   910 */    52,   53,   23,   23,   56,   90,   22,   25,  134,   26,
 /*   920 */    24,   22,   22,   26,   22,   67,   68,   69,   26,   25,
 /*   930 */    72,   29,   26,  134,  134,  134,  134,  134,  134,   37,
 /*   940 */    38,   39,   40,  134,   42,   43,   44,   45,  134,   47,
 /*   950 */   134,  134,  134,   51,   52,   53,  134,  134,   56,  134,
 /*   960 */   134,  134,  134,  134,  134,  134,  134,  134,   22,   67,
 /*   970 */    68,   69,   26,  134,   72,   29,  134,  134,  134,  134,
 /*   980 */   134,  134,  134,   37,   38,   39,   40,  134,   42,   43,
 /*   990 */    44,   45,  134,   47,  134,  134,  134,   51,   52,   53,
 /*  1000 */   134,  134,   56,  134,  134,  134,  134,  134,  134,  134,
 /*  1010 */   134,  134,   22,   67,   68,   69,   26,  134,   72,   29,
 /*  1020 */   134,  134,  134,  134,  134,  134,  134,   37,   38,   39,
 /*  1030 */    40,  134,   42,   43,   44,   45,  134,   47,  134,  134,
 /*  1040 */   134,   51,   52,   53,  134,  134,   56,  134,  134,  134,
 /*  1050 */   134,  134,  134,  134,  134,  134,   22,   67,   68,   69,
 /*  1060 */    26,  134,   72,   29,  134,  134,  134,  134,  134,  134,
 /*  1070 */   134,   37,   38,   39,   40,  134,   42,   43,   44,   45,
 /*  1080 */   134,   47,  134,  134,  134,   51,   52,   53,  134,  134,
 /*  1090 */    56,  134,  134,  134,  134,  134,  134,  134,  134,  134,
 /*  1100 */    22,   67,   68,   69,   26,  134,   72,   29,  134,  134,
 /*  1110 */   134,  134,  134,  134,  134,   37,   38,   39,   40,  134,
 /*  1120 */    42,   43,   44,   45,  134,   47,  134,  134,  134,   51,
 /*  1130 */    52,   53,  134,  134,   56,  134,  134,  134,  134,  134,
 /*  1140 */   134,  134,  134,  134,   22,   67,   68,   69,   26,  134,
 /*  1150 */    72,   29,  134,  134,  134,  134,  134,  134,  134,   37,
 /*  1160 */    38,   39,   40,  134,   42,   43,   44,   45,  134,   47,
 /*  1170 */   134,  134,  134,   51,   52,   53,  134,  134,   56,  134,
 /*  1180 */   134,  134,  134,  134,  134,   22,  134,  134,  134,   67,
 /*  1190 */    68,   69,   29,  134,   72,  134,  134,  134,  134,  134,
 /*  1200 */    37,   38,   39,   40,  134,   42,   43,   44,   45,  134,
 /*  1210 */    47,  134,   22,    0,   51,   52,   53,  134,  134,   56,
 /*  1220 */   134,  134,  134,  134,  134,  134,  134,  134,  134,   84,
 /*  1230 */    67,   68,   69,  134,   21,   72,  134,   24,  134,   94,
 /*  1240 */   134,   28,   29,   98,   99,  100,  101,  102,  103,  104,
 /*  1250 */   105,  106,  107,  108,  109,  110,  111,  112,   84,  134,
 /*  1260 */    70,   71,  134,   73,   74,   75,   76,  134,   94,  124,
 /*  1270 */   134,  134,   98,   99,  100,  101,  102,  103,  104,  105,
 /*  1280 */   106,  107,  108,  109,  110,  111,  112,   84,  134,  134,
 /*  1290 */   134,  134,  134,  134,  134,  134,  134,   94,  124,  134,
 /*  1300 */   134,   98,   99,  100,  101,  102,  103,  104,  105,  106,
 /*  1310 */   107,  108,  109,  110,  111,  112,   84,  134,  134,  134,
 /*  1320 */   134,  134,  134,  134,  134,  134,   94,  124,  134,  134,
 /*  1330 */    98,   99,  100,  101,  102,  103,  104,  105,  106,  107,
 /*  1340 */   108,  109,  110,  111,  112,  134,  134,  134,  134,  134,
 /*  1350 */   134,  134,  134,   84,  134,  134,  124,  134,  134,  134,
 /*  1360 */   134,  134,  134,   94,  134,  134,  134,   98,   99,  100,
 /*  1370 */   101,  102,  103,  104,  105,  106,  107,  108,  109,  110,
 /*  1380 */   111,  112,   84,  134,  134,  134,  134,  134,  134,  134,
 /*  1390 */   134,  134,   94,  124,  134,  134,   98,   99,  100,  101,
 /*  1400 */   102,  103,  104,  105,  106,  107,  108,  109,  110,  111,
 /*  1410 */   112,   84,  134,  134,  134,  134,  134,  134,  134,  134,
 /*  1420 */   134,   94,  124,  134,  134,   98,   99,  100,  101,  102,
 /*  1430 */   103,  104,  105,  106,  107,  108,  109,  110,  111,  112,
 /*  1440 */   134,  134,  134,  134,  134,  134,  134,  134,   84,  134,
 /*  1450 */   134,  124,  134,  134,  134,  134,  134,  134,   94,  134,
 /*  1460 */   134,  134,   98,   99,  100,  101,  102,  103,  104,  105,
 /*  1470 */   106,  107,  108,  109,  110,  111,  112,   84,  134,  134,
 /*  1480 */   134,  134,  134,  134,  134,  134,  134,   94,  124,  134,
 /*  1490 */   134,   98,   99,  100,  101,  102,  103,  104,  105,  106,
 /*  1500 */   107,  108,  109,  110,  111,  112,   84,  134,  134,  134,
 /*  1510 */   134,  134,  134,  134,  134,  134,   94,  124,  134,  134,
 /*  1520 */    98,   99,  100,  101,  102,  103,  104,  105,  106,  107,
 /*  1530 */   108,  109,  110,  111,  112,  134,  134,  134,  134,  134,
 /*  1540 */   134,  134,  134,   84,  134,  134,  124,  134,  134,  134,
 /*  1550 */   134,  134,  134,   94,  134,  134,  134,   98,   99,  100,
 /*  1560 */   101,  102,  103,  104,  105,  106,  107,  108,  109,  110,
 /*  1570 */   111,  112,   84,  134,  134,  134,  134,  134,  134,  134,
 /*  1580 */   134,  134,   94,  124,  134,  134,   98,   99,  100,  101,
 /*  1590 */   102,  103,  104,  105,  106,  107,  108,  109,  110,  111,
 /*  1600 */   112,   84,  134,  134,  134,  134,  134,  134,  134,  134,
 /*  1610 */   134,   94,  124,  134,  134,   98,   99,  100,  101,  102,
 /*  1620 */   103,  104,  105,  106,  107,  108,  109,  110,  111,  112,
 /*  1630 */   134,  134,  134,  134,  134,  134,  134,  134,   84,  134,
 /*  1640 */   134,  124,  134,  134,  134,  134,  134,  134,   94,  134,
 /*  1650 */   134,  134,   98,   99,  100,  101,  102,  103,  104,  105,
 /*  1660 */   106,  107,  108,  109,  110,  111,  112,   84,  134,  134,
 /*  1670 */   134,  134,  134,  134,  134,  134,  134,   94,  124,  134,
 /*  1680 */   134,   98,   99,  100,  101,  102,  103,  104,  105,  106,
 /*  1690 */   107,  108,  109,  110,  111,  112,   84,  134,  134,  134,
 /*  1700 */   134,  134,  134,  134,  134,  134,   94,  124,  134,  134,
 /*  1710 */    98,   99,  100,  101,  102,  103,  104,  105,  106,  107,
 /*  1720 */   108,  109,  110,  111,  112,  134,  134,  134,  134,  134,
 /*  1730 */   134,  134,  134,   84,  134,  134,  124,  134,  134,  134,
 /*  1740 */   134,  134,  134,   94,  134,  134,  134,   98,   99,  100,
 /*  1750 */   101,  102,  103,  104,  105,  106,  107,  108,  109,  110,
 /*  1760 */   111,  112,   84,  134,  134,  134,  134,  134,  134,  134,
 /*  1770 */   134,  134,   94,  124,  134,  134,   98,   99,  100,  101,
 /*  1780 */   102,  103,  104,  105,  106,  107,  108,  109,  110,  111,
 /*  1790 */   112,   84,  134,  134,  134,  134,  134,  134,  134,  134,
 /*  1800 */   134,  134,  124,  134,  134,   98,   99,  100,  101,  102,
 /*  1810 */   103,  104,  105,  106,  107,  108,  109,  110,  111,  112,
 /*  1820 */   134,   15,   16,   17,   18,   19,   20,  134,   22,  134,
 /*  1830 */   134,  124,  134,  134,  134,   15,   16,   17,   18,   19,
 /*  1840 */    20,   35,   22,  134,   15,   16,   17,   18,   19,   20,
 /*  1850 */   134,   22,  134,  134,  134,   35,   36,  134,  134,  134,
 /*  1860 */   134,   55,  134,  134,   35,   36,  134,  134,  134,   63,
 /*  1870 */   134,  134,  134,  134,  134,  134,   70,   71,  134,   73,
 /*  1880 */    74,   75,   76,   63,  134,  134,  134,  134,  134,  134,
 /*  1890 */    70,   71,   63,   73,   74,   75,   76,  134,  134,   70,
 /*  1900 */    71,  134,   73,   74,   75,   76,   15,   16,   17,   18,
 /*  1910 */    19,   20,  134,   22,    4,    5,    6,    7,    8,    9,
 /*  1920 */    10,   11,   12,   13,   14,  134,   35,   36,  134,   15,
 /*  1930 */    16,   17,   18,   19,   20,  134,   22,  134,  134,    6,
 /*  1940 */   134,  134,  134,   15,   16,   17,   18,   19,   20,   35,
 /*  1950 */    22,  134,  134,  134,   63,   22,  134,  134,  134,  134,
 /*  1960 */   134,   70,   71,   35,   73,   74,   75,   76,  134,   36,
 /*  1970 */    37,   38,   39,   40,   41,   42,  134,   63,   64,  134,
 /*  1980 */   134,  134,  134,  134,   70,   71,  134,   73,   74,   75,
 /*  1990 */    76,   63,   64,  134,  134,  134,  134,    6,   70,   71,
 /*  2000 */   134,   73,   74,   75,   76,   15,   16,   17,   18,   19,
 /*  2010 */    20,  134,   22,   22,   15,   16,   17,   18,   19,   20,
 /*  2020 */    31,   22,  134,  134,  134,   35,  134,   36,   37,   38,
 /*  2030 */    39,   40,   41,   42,   35,  134,  134,  134,  134,  134,
 /*  2040 */   134,  134,  134,  134,  134,   55,   57,   58,   59,   60,
 /*  2050 */    61,   62,   63,   63,   65,   66,  134,  134,  134,  134,
 /*  2060 */    70,   71,   63,   73,   74,   75,   76,  134,  134,   70,
 /*  2070 */    71,  134,   73,   74,   75,   76,    2,    3,    4,    5,
 /*  2080 */     6,    7,    8,    9,   10,   11,   12,   13,   14,  134,
 /*  2090 */   134,  134,  134,  134,  134,  134,  134,  134,  134,   25,
 /*  2100 */   134,  134,    2,    3,    4,    5,    6,    7,    8,    9,
 /*  2110 */    10,   11,   12,   13,   14,    2,    3,    4,    5,    6,
 /*  2120 */     7,    8,    9,   10,   11,   12,   13,   14,  134,  134,
 /*  2130 */   134,  134,  134,  134,  134,  134,   36,  134,  134,    2,
 /*  2140 */     3,    4,    5,    6,    7,    8,    9,   10,   11,   12,
 /*  2150 */    13,   14,  134,  134,  134,  134,  134,  134,  134,  134,
 /*  2160 */    23,  134,  134,    2,    3,    4,    5,    6,    7,    8,
 /*  2170 */     9,   10,   11,   12,   13,   14,  134,   64,  134,  134,
 /*  2180 */   134,  134,  134,  134,  134,  134,   25,  134,  134,    2,
 /*  2190 */     3,    4,    5,    6,    7,    8,    9,   10,   11,   12,
 /*  2200 */    13,   14,  134,  134,  134,  134,  134,  134,  134,  134,
 /*  2210 */   134,  134,   25,  134,  134,    2,    3,    4,    5,    6,
 /*  2220 */     7,    8,    9,   10,   11,   12,   13,   14,  134,  134,
 /*  2230 */   134,  134,  134,  134,  134,  134,  134,  134,   25,   22,
 /*  2240 */   134,    2,    3,    4,    5,    6,    7,    8,    9,   10,
 /*  2250 */    11,   12,   13,   14,  134,  134,  134,  134,  134,  134,
 /*  2260 */   134,  134,  134,  134,   25,  134,  134,    2,    3,    4,
 /*  2270 */     5,    6,    7,    8,    9,   10,   11,   12,   13,   14,
 /*  2280 */     6,   64,  134,  134,  134,  134,    6,   70,   71,  134,
 /*  2290 */    73,   74,   75,   76,  134,  134,   22,  134,  134,   25,
 /*  2300 */   134,  134,   22,  134,  134,  134,  134,  134,  134,  134,
 /*  2310 */   134,   37,   38,   39,   40,   41,   42,   37,   38,   39,
 /*  2320 */    40,   41,   42,
};
#define YY_SHIFT_USE_DFLT (-23)
static short yy_shift_ofst[] = {
 /*     0 */   322,   93, 1213,  -23,  -23,  -23,  -23,   60,   75,  -23,
 /*    10 */    99,  175,  155,   88,  -23,  -23,  199,  375,  -23,  112,
 /*    20 */   129,  137, 1933, 2274,  -22,  -23,   22,  -23,  -23,  -23,
 /*    30 */   -23,  -23,  -23,  -23,  -23,  -23,  -23,  -23,  -23,  -23,
 /*    40 */   -23,  -23,  -23,  -23,  196,  -23,  273,  -23, 1999,  209,
 /*    50 */    66,  110,  267,  277,  154,  198,  -23, 1999,  324,  345,
 /*    60 */   -23,  389,  -23,  -23,   26,  327, 1163, 1163,  360,  242,
 /*    70 */   286,  -23, 1999,  369,  330,  374,  -23,  400,   23, 1806,
 /*    80 */  2074, 1163,  418,  -23,  457,  405,  -23,  457,  -23, 1989,
 /*    90 */   -23,  -23,  -23,  -23,  -23,  -23, 1999,  -23, 2265, 1999,
 /*   100 */  1999,  -23, 1999,  472,  -23,  449,  295,  488,  -23,  516,
 /*   110 */   -23,  565,  -23,  366, 1999,  366, 1999,  366, 1999,  366,
 /*   120 */  1999,  366, 1999,  366, 1999,  366, 1999,  -23, 1999,  -23,
 /*   130 */  1999,  -23, 1999, 1910, 1999, 1910, 1999, 2100,  -23,  102,
 /*   140 */   504,  519, 1820, 2265, 1999,  -23,  548,  629,  472,  -23,
 /*   150 */  1999,  -23,  -23,  -23,  -23,  -23,  -23,  -23, 2217,  -23,
 /*   160 */   256,  -23,   10,  -23,  605, 1190,  -23,  -23,  -23,  -23,
 /*   170 */   -23,  -23,  -23,  -23,  -23,  -23,  609,  619,  620,  -23,
 /*   180 */   634,  635, 1829,  -23,  139,  -23, 1999,  -23,  -23,  -23,
 /*   190 */   502,  -23,  -23,  534,  608,  -23,  610,  -23,  622,  -23,
 /*   200 */  1891,  606,  -23,  -23,  366,  649,   -5, 1999,  -23, 1914,
 /*   210 */    84, 1999,  -23,  650,   84, 1999,  -23, 2113, 1928,   84,
 /*   220 */  1999,  -23,  654,   84, 1999,  -23,  -23,  -23,  -23, 1999,
 /*   230 */   735,  -23, 1999,  -23, 2265,  -23,  661,  -23, 1999, 2137,
 /*   240 */   -23,  676,  478,  691,  705,  -23,  544,  677,  719,  -23,
 /*   250 */   679,  721,  -23,  692,  722,  -23,   21,  723,  -23,  737,
 /*   260 */   725, 1999,  701,  734,  -23,  740,  541,  -23,  740,  -23,
 /*   270 */   743,   26,  -23,  -23,  740,  738,  -23,  740,  752,  -23,
 /*   280 */   740,  766,  -23,  740,  779,  -23,  724,  765,  519,  -23,
 /*   290 */   -23, 1999, 2161, 1163,  462,  -23,  768,  746, 1990, 2187,
 /*   300 */  1163,  506,  -23, 1999, 2213, 1163,  550,  -23,  -23, 2239,
 /*   310 */   -23,  739, 1163, 1163,  -23,  -23,  755,  778,  594,  638,
 /*   320 */   -23,  -23,  106, 1163,  682,  -23, 2280,  -23,  773,   26,
 /*   330 */   -23,  789,  787,   26,  -23,  802,  794,   26,  -23,  807,
 /*   340 */   823,  812,  813,  822,  826,  827,  828,  838,  -23,  711,
 /*   350 */   820,  726,  -23,  770,  -23,  214,  829,  840,  811, 1991,
 /*   360 */   839,  814,  -23,  858,  -23,  780,  843,  902,  -23,  946,
 /*   370 */   -23,  835,  849,  837,  841,  848,  990,  -23, 1034,  -23,
 /*   380 */   842,  852,  844,  853,  850, 1078,  -23, 1122,  -23,  226,
 /*   390 */   419,  -23,  -23,  420,  856,  854,   26,  867,  -23,  869,
 /*   400 */   861,   26,  871,  -23,  -23,  450,  432,  796,  -23,   26,
 /*   410 */   881,  -23,  562,  797,  -23,   26,  884,  -23,  564,  810,
 /*   420 */   -23,   26,  889,  -23,  621,  824,  -23,   26,  890,  -23,
 /*   430 */   -23,  271,  446,  463,  406,  490,  -23,  507,  894,  892,
 /*   440 */   315,  893,  -23,  -23,  896,  899,  601,  331,  897,  -23,
 /*   450 */   -23,  900,  904,  358,  906,  -23,  -23,  -23,
};
#define YY_REDUCE_USE_DFLT (-98)
static short yy_reduce_ofst[] = {
 /*     0 */   -70,  -98,  179,  -98,  -98,  -98,  -98,  -98,  -98,  -98,
 /*    10 */   -98,  -98,  -50,  -98,  -98,  -98,  -86,   36,  -98,  -98,
 /*    20 */   -98,  -98,  -25,   63, 1145,  -98, 1707,  -98,  -98,  -98,
 /*    30 */   -98,  -98,  -98,  -98,  -98,  -98,  -98,  -98,  -98,  -98,
 /*    40 */   -98,  -98,  -98,  -98,  -98,  -98,  -98,  -98,  -37,  -98,
 /*    50 */  1174, 1707,  -98,  -98, 1203, 1707,  -98,  307,  -98,  382,
 /*    60 */   -98,  247,  -98,  -98,  299,  -98, 1232, 1707,  -98, 1269,
 /*    70 */  1707,  -98,  447,  -98, 1298, 1707,  -98,  -98,  -98,   92,
 /*    80 */   -98, 1327, 1707,  -98,  393,  -98,  -98,  350,  -98,  379,
 /*    90 */   -98,  -98,  -98,  -98,  -98,  -98,   -4,  -98,  -98,  431,
 /*   100 */   475,  -98,  484,  380,  -98,  -98,  -98,  -98,  -98,  -98,
 /*   110 */   -98,  -98,  -98,  -98,  497,  -98,  511,  -98,  524,  -98,
 /*   120 */   528,  -98,  533,  -98,  535,  -98,  549,  -98,  563,  -98,
 /*   130 */   572,  -98,  576,  -98,  577,  -98,  579,  -98,  -98,  -98,
 /*   140 */   -98,  -98,  -97,  -98,  593,  -98,  -98,  -98,  525,  -98,
 /*   150 */   607,  -98,  -98,  -98,  -98,  -98,  -98,  -98,  143,  -98,
 /*   160 */   -98,  -98,  302,  -98,  -98,  140,  -98,  -98,  -98,  -98,
 /*   170 */   -98,  -98,  -98,  -98,  -98,  -98,  -98,  -98,  -98,  -98,
 /*   180 */   -98,  -98,   79,  -98,  -98,  -98,  -67,  -98,  -98,  -98,
 /*   190 */   -98,  -98,  -98,  -98,  -98,  -98,  -98,  -98,  -98,  -98,
 /*   200 */   123,  -98,  -98,  -98,  -98,  -98,  578,  216,  -98,  308,
 /*   210 */   580,  357,  -98,  -98,  596,  401,  -98,  -98,  343,  597,
 /*   220 */   445,  -98,  -98,  611,  461,  -98,  -98,  -98,  -98,  183,
 /*   230 */   -98,  -98,  211,  -98,  -98,  -98,  -98,  -98,  616,  -98,
 /*   240 */   -98,  -98,  -98,  -98,  -98,  -98,  -98,  -98,  -98,  -98,
 /*   250 */   -98,  -98,  -98,  -98,  -98,  -98,  -98,  -98,  -98,  -98,
 /*   260 */   -98,  153,  -98,  -98,  -98,  441,  -98,  -98,  645,  -98,
 /*   270 */   -98,  695,  -98,  -98,  456,  -98,  -98,  483,  -98,  -98,
 /*   280 */   485,  -98,  -98,  498,  -98,  -98,  -98,  -98,  -98,  -98,
 /*   290 */   -98,  660,  -98, 1364, 1707,  -98,  -98,  -98,  667,  -98,
 /*   300 */  1393, 1707,  -98,  681,  -98, 1422, 1707,  -98,  -98,  -98,
 /*   310 */   -98,  -98, 1459, 1707,  -98,  -98,  -98,  -98, 1488, 1707,
 /*   320 */   -98,  -98,  -98, 1517, 1707,  -98,  313,  -98,  -98,  716,
 /*   330 */   -98,  -98,  -98,  741,  -98,  -98,  -98,  742,  -98,  -98,
 /*   340 */   -98,  -98,  -98,  -98,  -98,  -98,  -98,  -98,  -98,  -98,
 /*   350 */   -98, 1554,  -98, 1707,  -98,  -98,  -98,  -98,  -98,  195,
 /*   360 */   -98, 1583,  -98, 1707,  -98,  -98,  -98, 1612,  -98, 1707,
 /*   370 */   -98,  -98,  -98,  -98,  -98,  -98, 1649,  -98, 1707,  -98,
 /*   380 */   -98,  -98,  -98,  -98,  -98, 1678,  -98, 1707,  -98,   98,
 /*   390 */    36,  -98,  -98,  -98,  -98,  -98,  798,  -98,  -98,  -98,
 /*   400 */   -98,  803,  -98,  -98,  -98,  -98,  -98,  -98,  -98,  809,
 /*   410 */   -98,  -98,  -98,  -98,  -98,  816,  -98,  -98,  -98,  -98,
 /*   420 */   -98,  818,  -98,  -98,  -98,  -98,  -98,  825,  -98,  -98,
 /*   430 */   -98,  230,  435,   36,  -98,   36,  -98,   36,  -98,  -98,
 /*   440 */    38,  -98,  -98,  -98,  -98,  -98,  -98,   82,  -98,  -98,
 /*   450 */   -98,  -98,  -98,  126,  -98,  -98,  -98,  -98,
};
static YYACTIONTYPE yy_default[] = {
 /*     0 */   669,  669,  669,  460,  462,  463,  464,  669,  669,  465,
 /*    10 */   669,  669,  669,  669,  466,  470,  669,  669,  495,  669,
 /*    20 */   669,  669,  669,  669,  669,  499,  669,  503,  527,  529,
 /*    30 */   530,  531,  532,  533,  534,  535,  536,  537,  538,  539,
 /*    40 */   540,  541,  542,  543,  669,  544,  669,  545,  669,  669,
 /*    50 */   669,  669,  548,  669,  669,  669,  549,  669,  669,  669,
 /*    60 */   552,  669,  553,  554,  669,  669,  669,  556,  669,  669,
 /*    70 */   669,  559,  669,  669,  669,  669,  561,  669,  669,  669,
 /*    80 */   669,  669,  669,  562,  669,  669,  566,  669,  567,  669,
 /*    90 */   569,  570,  571,  572,  573,  574,  669,  575,  605,  669,
 /*   100 */   669,  606,  669,  669,  607,  669,  669,  669,  664,  669,
 /*   110 */   665,  669,  666,  609,  669,  610,  669,  611,  669,  612,
 /*   120 */   669,  613,  669,  614,  669,  615,  669,  621,  669,  622,
 /*   130 */   669,  623,  669,  624,  669,  625,  669,  669,  616,  629,
 /*   140 */   669,  617,  669,  648,  669,  626,  669,  669,  669,  627,
 /*   150 */   669,  628,  630,  631,  632,  633,  634,  635,  669,  636,
 /*   160 */   669,  637,  669,  649,  669,  669,  651,  656,  657,  658,
 /*   170 */   659,  660,  661,  662,  663,  652,  657,  659,  658,  650,
 /*   180 */   669,  638,  669,  639,  669,  640,  669,  643,  646,  647,
 /*   190 */   669,  644,  645,  669,  669,  618,  669,  619,  669,  620,
 /*   200 */   669,  669,  641,  642,  608,  669,  669,  669,  576,  669,
 /*   210 */   669,  669,  577,  669,  669,  669,  578,  669,  669,  669,
 /*   220 */   669,  579,  669,  669,  669,  580,  581,  582,  568,  669,
 /*   230 */   669,  584,  669,  585,  587,  586,  669,  588,  669,  669,
 /*   240 */   589,  669,  669,  669,  669,  590,  669,  669,  669,  591,
 /*   250 */   669,  669,  592,  669,  669,  593,  669,  669,  594,  669,
 /*   260 */   669,  669,  669,  669,  595,  669,  669,  596,  669,  601,
 /*   270 */   603,  669,  604,  602,  669,  669,  597,  669,  669,  598,
 /*   280 */   669,  669,  599,  669,  669,  600,  669,  669,  669,  668,
 /*   290 */   528,  669,  669,  669,  669,  563,  669,  669,  669,  669,
 /*   300 */   669,  669,  564,  669,  669,  669,  669,  565,  560,  669,
 /*   310 */   558,  669,  669,  557,  555,  550,  546,  669,  669,  669,
 /*   320 */   551,  547,  669,  669,  669,  504,  669,  512,  514,  669,
 /*   330 */   517,  669,  515,  669,  518,  669,  516,  669,  519,  669,
 /*   340 */   669,  669,  669,  669,  669,  669,  669,  669,  513,  669,
 /*   350 */   669,  669,  500,  669,  507,  669,  669,  669,  669,  669,
 /*   360 */   669,  669,  501,  669,  505,  669,  669,  669,  502,  669,
 /*   370 */   506,  669,  669,  669,  669,  669,  669,  509,  669,  511,
 /*   380 */   669,  669,  669,  669,  669,  669,  508,  669,  510,  669,
 /*   390 */   669,  493,  496,  669,  669,  669,  669,  669,  497,  669,
 /*   400 */   669,  669,  669,  498,  483,  669,  669,  669,  485,  669,
 /*   410 */   669,  488,  669,  669,  490,  669,  669,  492,  669,  669,
 /*   420 */   486,  669,  669,  487,  669,  669,  489,  669,  669,  491,
 /*   430 */   494,  669,  669,  669,  669,  669,  484,  669,  669,  669,
 /*   440 */   669,  669,  467,  471,  669,  669,  669,  669,  669,  468,
 /*   450 */   472,  669,  669,  669,  669,  469,  473,  461,
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
  "$",             "COMMA",         "AND",           "OR",          
  "EQUALS",        "IDENTICAL",     "LESS",          "GREATER",     
  "LESSEQUAL",     "GREATEREQUAL",  "NOTIDENTICAL",  "NOTEQUALS",   
  "ADD",           "SUB",           "CONCAT",        "ISSET",       
  "FETCH",         "TYPEOF",        "INSTANCEOF",    "NEW",         
  "NOT",           "NAMESPACE",     "IDENTIFIER",    "DOTCOMMA",    
  "CLASS",         "BRACKET_OPEN",  "BRACKET_CLOSE",  "EXTENDS",     
  "ABSTRACT",      "COMMENT",       "PUBLIC",        "ASSIGN",      
  "PROTECTED",     "CONST",         "FUNCTION",      "PARENTHESES_OPEN",
  "PARENTHESES_CLOSE",  "TYPE_INTEGER",  "TYPE_DOUBLE",   "TYPE_BOOL",   
  "TYPE_STRING",   "TYPE_ARRAY",    "TYPE_VAR",      "BREAK",       
  "CONTINUE",      "IF",            "ELSE",          "SWITCH",      
  "CASE",          "COLON",         "DEFAULT",       "LOOP",        
  "WHILE",         "FOR",           "IN",            "REVERSE",     
  "LET",           "ADDASSIGN",     "SUBASSIGN",     "MULASSIGN",   
  "DIVASSIGN",     "CONCATASSIGN",  "ARROW",         "SBRACKET_OPEN",
  "SBRACKET_CLOSE",  "INCR",          "DECR",          "ECHO",        
  "RETURN",        "UNSET",         "INTEGER",       "STRING",      
  "THROW",         "DOUBLE",        "NULL",          "TRUE",        
  "FALSE",         "error",         "program",       "xx_language", 
  "xx_top_statement_list",  "xx_top_statement",  "xx_namespace_def",  "xx_class_def",
  "xx_comment",    "xx_class_definition",  "xx_class_properties_definition",  "xx_class_consts_definition",
  "xx_class_methods_definition",  "xx_class_property_definition",  "xx_literal_expr",  "xx_class_const_definition",
  "xx_class_method_definition",  "xx_parameter_list",  "xx_statement_list",  "xx_parameter",
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
 /*  27 */ "xx_class_property_definition ::= COMMENT PUBLIC IDENTIFIER DOTCOMMA",
 /*  28 */ "xx_class_property_definition ::= PUBLIC IDENTIFIER DOTCOMMA",
 /*  29 */ "xx_class_property_definition ::= PUBLIC IDENTIFIER ASSIGN xx_literal_expr DOTCOMMA",
 /*  30 */ "xx_class_property_definition ::= COMMENT PUBLIC IDENTIFIER ASSIGN xx_literal_expr DOTCOMMA",
 /*  31 */ "xx_class_property_definition ::= PROTECTED IDENTIFIER DOTCOMMA",
 /*  32 */ "xx_class_property_definition ::= COMMENT PROTECTED IDENTIFIER DOTCOMMA",
 /*  33 */ "xx_class_property_definition ::= PROTECTED IDENTIFIER ASSIGN xx_literal_expr DOTCOMMA",
 /*  34 */ "xx_class_property_definition ::= COMMENT PROTECTED IDENTIFIER ASSIGN xx_literal_expr DOTCOMMA",
 /*  35 */ "xx_class_consts_definition ::= xx_class_consts_definition xx_class_const_definition",
 /*  36 */ "xx_class_consts_definition ::= xx_class_const_definition",
 /*  37 */ "xx_class_methods_definition ::= xx_class_methods_definition xx_class_method_definition",
 /*  38 */ "xx_class_methods_definition ::= xx_class_method_definition",
 /*  39 */ "xx_class_const_definition ::= COMMENT CONST IDENTIFIER ASSIGN xx_literal_expr DOTCOMMA",
 /*  40 */ "xx_class_const_definition ::= CONST IDENTIFIER ASSIGN xx_literal_expr DOTCOMMA",
 /*  41 */ "xx_class_method_definition ::= PUBLIC FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  42 */ "xx_class_method_definition ::= PUBLIC FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  43 */ "xx_class_method_definition ::= COMMENT PUBLIC FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  44 */ "xx_class_method_definition ::= COMMENT PUBLIC FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  45 */ "xx_class_method_definition ::= PUBLIC FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  46 */ "xx_class_method_definition ::= PUBLIC FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE xx_parameter_list BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  47 */ "xx_class_method_definition ::= COMMENT PUBLIC FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  48 */ "xx_class_method_definition ::= COMMENT PUBLIC FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  49 */ "xx_class_method_definition ::= PUBLIC FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  50 */ "xx_class_method_definition ::= PROTECTED FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  51 */ "xx_class_method_definition ::= COMMENT PROTECTED FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  52 */ "xx_class_method_definition ::= PROTECTED FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  53 */ "xx_class_method_definition ::= COMMENT PROTECTED FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  54 */ "xx_parameter_list ::= xx_parameter_list COMMA xx_parameter",
 /*  55 */ "xx_parameter_list ::= xx_parameter",
 /*  56 */ "xx_parameter ::= IDENTIFIER",
 /*  57 */ "xx_parameter ::= xx_parameter_type IDENTIFIER",
 /*  58 */ "xx_parameter ::= xx_parameter_cast IDENTIFIER",
 /*  59 */ "xx_parameter ::= IDENTIFIER ASSIGN xx_literal_expr",
 /*  60 */ "xx_parameter ::= xx_parameter_type IDENTIFIER ASSIGN xx_literal_expr",
 /*  61 */ "xx_parameter ::= xx_parameter_cast IDENTIFIER ASSIGN xx_literal_expr",
 /*  62 */ "xx_parameter_cast ::= LESS IDENTIFIER GREATER",
 /*  63 */ "xx_parameter_type ::= TYPE_INTEGER",
 /*  64 */ "xx_parameter_type ::= TYPE_DOUBLE",
 /*  65 */ "xx_parameter_type ::= TYPE_BOOL",
 /*  66 */ "xx_parameter_type ::= TYPE_STRING",
 /*  67 */ "xx_parameter_type ::= TYPE_ARRAY",
 /*  68 */ "xx_parameter_type ::= TYPE_VAR",
 /*  69 */ "xx_statement_list ::= xx_statement_list xx_statement",
 /*  70 */ "xx_statement_list ::= xx_statement",
 /*  71 */ "xx_statement ::= xx_let_statement",
 /*  72 */ "xx_statement ::= xx_if_statement",
 /*  73 */ "xx_statement ::= xx_loop_statement",
 /*  74 */ "xx_statement ::= xx_echo_statement",
 /*  75 */ "xx_statement ::= xx_return_statement",
 /*  76 */ "xx_statement ::= xx_mcall_statement",
 /*  77 */ "xx_statement ::= xx_unset_statement",
 /*  78 */ "xx_statement ::= xx_throw_statement",
 /*  79 */ "xx_statement ::= xx_declare_statement",
 /*  80 */ "xx_statement ::= xx_break_statement",
 /*  81 */ "xx_statement ::= xx_continue_statement",
 /*  82 */ "xx_statement ::= xx_while_statement",
 /*  83 */ "xx_statement ::= xx_switch_statement",
 /*  84 */ "xx_statement ::= xx_for_statement",
 /*  85 */ "xx_statement ::= xx_comment",
 /*  86 */ "xx_break_statement ::= BREAK DOTCOMMA",
 /*  87 */ "xx_continue_statement ::= CONTINUE DOTCOMMA",
 /*  88 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /*  89 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE ELSE BRACKET_OPEN BRACKET_CLOSE",
 /*  90 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  91 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE ELSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  92 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE ELSE BRACKET_OPEN BRACKET_CLOSE",
 /*  93 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE ELSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  94 */ "xx_switch_statement ::= SWITCH xx_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /*  95 */ "xx_switch_statement ::= SWITCH xx_eval_expr BRACKET_OPEN xx_case_clauses BRACKET_CLOSE",
 /*  96 */ "xx_case_clauses ::= xx_case_clauses xx_case_clause",
 /*  97 */ "xx_case_clauses ::= xx_case_clause",
 /*  98 */ "xx_case_clause ::= CASE xx_literal_expr COLON xx_statement_list",
 /*  99 */ "xx_case_clause ::= DEFAULT COLON xx_statement_list",
 /* 100 */ "xx_loop_statement ::= LOOP BRACKET_OPEN BRACKET_CLOSE",
 /* 101 */ "xx_loop_statement ::= LOOP BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 102 */ "xx_while_statement ::= WHILE xx_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /* 103 */ "xx_while_statement ::= WHILE xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 104 */ "xx_for_statement ::= FOR IDENTIFIER IN xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 105 */ "xx_for_statement ::= FOR IDENTIFIER IN REVERSE xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 106 */ "xx_for_statement ::= FOR IDENTIFIER COMMA IDENTIFIER IN xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 107 */ "xx_for_statement ::= FOR IDENTIFIER COMMA IDENTIFIER IN REVERSE xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 108 */ "xx_let_statement ::= LET xx_let_assignments DOTCOMMA",
 /* 109 */ "xx_let_assignments ::= xx_let_assignments COMMA xx_let_assignment",
 /* 110 */ "xx_let_assignments ::= xx_let_assignment",
 /* 111 */ "xx_assignment_operator ::= ASSIGN",
 /* 112 */ "xx_assignment_operator ::= ADDASSIGN",
 /* 113 */ "xx_assignment_operator ::= SUBASSIGN",
 /* 114 */ "xx_assignment_operator ::= MULASSIGN",
 /* 115 */ "xx_assignment_operator ::= DIVASSIGN",
 /* 116 */ "xx_assignment_operator ::= CONCATASSIGN",
 /* 117 */ "xx_let_assignment ::= IDENTIFIER xx_assignment_operator xx_assign_expr",
 /* 118 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER xx_assignment_operator xx_assign_expr",
 /* 119 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 120 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER SBRACKET_OPEN xx_index_expr SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 121 */ "xx_let_assignment ::= IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 122 */ "xx_let_assignment ::= IDENTIFIER SBRACKET_OPEN xx_index_expr SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 123 */ "xx_let_assignment ::= IDENTIFIER INCR",
 /* 124 */ "xx_let_assignment ::= IDENTIFIER DECR",
 /* 125 */ "xx_index_expr ::= xx_common_expr",
 /* 126 */ "xx_echo_statement ::= ECHO xx_echo_expressions DOTCOMMA",
 /* 127 */ "xx_echo_expressions ::= xx_echo_expressions COMMA xx_echo_expression",
 /* 128 */ "xx_echo_expressions ::= xx_echo_expression",
 /* 129 */ "xx_echo_expression ::= xx_common_expr",
 /* 130 */ "xx_mcall_statement ::= xx_mcall_expr DOTCOMMA",
 /* 131 */ "xx_return_statement ::= RETURN xx_common_expr DOTCOMMA",
 /* 132 */ "xx_unset_statement ::= UNSET IDENTIFIER ARROW IDENTIFIER DOTCOMMA",
 /* 133 */ "xx_unset_statement ::= UNSET IDENTIFIER SBRACKET_OPEN IDENTIFIER SBRACKET_CLOSE DOTCOMMA",
 /* 134 */ "xx_unset_statement ::= UNSET IDENTIFIER SBRACKET_OPEN INTEGER SBRACKET_CLOSE DOTCOMMA",
 /* 135 */ "xx_unset_statement ::= UNSET IDENTIFIER SBRACKET_OPEN STRING SBRACKET_CLOSE DOTCOMMA",
 /* 136 */ "xx_throw_statement ::= THROW IDENTIFIER DOTCOMMA",
 /* 137 */ "xx_throw_statement ::= THROW NEW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE DOTCOMMA",
 /* 138 */ "xx_declare_statement ::= TYPE_INTEGER xx_declare_variable_list DOTCOMMA",
 /* 139 */ "xx_declare_statement ::= TYPE_DOUBLE xx_declare_variable_list DOTCOMMA",
 /* 140 */ "xx_declare_statement ::= TYPE_STRING xx_declare_variable_list DOTCOMMA",
 /* 141 */ "xx_declare_statement ::= TYPE_BOOL xx_declare_variable_list DOTCOMMA",
 /* 142 */ "xx_declare_statement ::= TYPE_VAR xx_declare_variable_list DOTCOMMA",
 /* 143 */ "xx_declare_variable_list ::= xx_declare_variable_list COMMA xx_declare_variable",
 /* 144 */ "xx_declare_variable_list ::= xx_declare_variable",
 /* 145 */ "xx_declare_variable ::= IDENTIFIER",
 /* 146 */ "xx_declare_variable ::= IDENTIFIER ASSIGN xx_literal_expr",
 /* 147 */ "xx_assign_expr ::= xx_common_expr",
 /* 148 */ "xx_common_expr ::= NOT xx_common_expr",
 /* 149 */ "xx_common_expr ::= ISSET xx_isset_expr",
 /* 150 */ "xx_common_expr ::= xx_common_expr EQUALS xx_common_expr",
 /* 151 */ "xx_common_expr ::= xx_common_expr NOTEQUALS xx_common_expr",
 /* 152 */ "xx_common_expr ::= xx_common_expr IDENTICAL xx_common_expr",
 /* 153 */ "xx_common_expr ::= xx_common_expr NOTIDENTICAL xx_common_expr",
 /* 154 */ "xx_common_expr ::= xx_common_expr LESS xx_common_expr",
 /* 155 */ "xx_common_expr ::= xx_common_expr GREATER xx_common_expr",
 /* 156 */ "xx_common_expr ::= xx_common_expr LESSEQUAL xx_common_expr",
 /* 157 */ "xx_common_expr ::= xx_common_expr GREATEREQUAL xx_common_expr",
 /* 158 */ "xx_common_expr ::= PARENTHESES_OPEN xx_common_expr PARENTHESES_CLOSE",
 /* 159 */ "xx_common_expr ::= IDENTIFIER ARROW IDENTIFIER",
 /* 160 */ "xx_common_expr ::= IDENTIFIER SBRACKET_OPEN IDENTIFIER SBRACKET_CLOSE",
 /* 161 */ "xx_common_expr ::= IDENTIFIER SBRACKET_OPEN INTEGER SBRACKET_CLOSE",
 /* 162 */ "xx_common_expr ::= IDENTIFIER SBRACKET_OPEN STRING SBRACKET_CLOSE",
 /* 163 */ "xx_common_expr ::= xx_common_expr ADD xx_common_expr",
 /* 164 */ "xx_common_expr ::= xx_common_expr SUB xx_common_expr",
 /* 165 */ "xx_common_expr ::= xx_common_expr CONCAT xx_common_expr",
 /* 166 */ "xx_common_expr ::= xx_common_expr AND xx_common_expr",
 /* 167 */ "xx_common_expr ::= xx_common_expr OR xx_common_expr",
 /* 168 */ "xx_common_expr ::= INSTANCEOF xx_common_expr",
 /* 169 */ "xx_common_expr ::= FETCH IDENTIFIER COMMA xx_isset_expr",
 /* 170 */ "xx_common_expr ::= TYPEOF xx_common_expr",
 /* 171 */ "xx_common_expr ::= IDENTIFIER",
 /* 172 */ "xx_common_expr ::= INTEGER",
 /* 173 */ "xx_common_expr ::= STRING",
 /* 174 */ "xx_common_expr ::= DOUBLE",
 /* 175 */ "xx_common_expr ::= NULL",
 /* 176 */ "xx_common_expr ::= TRUE",
 /* 177 */ "xx_common_expr ::= FALSE",
 /* 178 */ "xx_common_expr ::= SBRACKET_OPEN SBRACKET_CLOSE",
 /* 179 */ "xx_common_expr ::= SBRACKET_OPEN xx_array_list SBRACKET_CLOSE",
 /* 180 */ "xx_common_expr ::= NEW IDENTIFIER",
 /* 181 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 182 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 183 */ "xx_common_expr ::= IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 184 */ "xx_common_expr ::= IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 185 */ "xx_common_expr ::= xx_mcall_expr",
 /* 186 */ "xx_mcall_expr ::= IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 187 */ "xx_mcall_expr ::= IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 188 */ "xx_call_parameters ::= xx_call_parameters COMMA xx_call_parameter",
 /* 189 */ "xx_call_parameters ::= xx_call_parameter",
 /* 190 */ "xx_call_parameter ::= xx_common_expr",
 /* 191 */ "xx_array_list ::= xx_array_list COMMA xx_array_item",
 /* 192 */ "xx_array_list ::= xx_array_item",
 /* 193 */ "xx_array_item ::= xx_array_key COLON xx_array_value",
 /* 194 */ "xx_array_item ::= xx_array_value",
 /* 195 */ "xx_array_key ::= IDENTIFIER",
 /* 196 */ "xx_array_key ::= STRING",
 /* 197 */ "xx_array_key ::= INTEGER",
 /* 198 */ "xx_array_value ::= xx_literal_expr",
 /* 199 */ "xx_array_value ::= IDENTIFIER",
 /* 200 */ "xx_literal_expr ::= INTEGER",
 /* 201 */ "xx_literal_expr ::= STRING",
 /* 202 */ "xx_literal_expr ::= DOUBLE",
 /* 203 */ "xx_literal_expr ::= NULL",
 /* 204 */ "xx_literal_expr ::= FALSE",
 /* 205 */ "xx_literal_expr ::= TRUE",
 /* 206 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN IDENTIFIER SBRACKET_CLOSE",
 /* 207 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN INTEGER SBRACKET_CLOSE",
 /* 208 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN STRING SBRACKET_CLOSE",
 /* 209 */ "xx_eval_expr ::= xx_common_expr",
 /* 210 */ "xx_comment ::= COMMENT",
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
// 764 "parser.lemon"
{
	/*if ((yypminor->yy0)) {
		if ((yypminor->yy0)->free_flag) {
			efree((yypminor->yy0)->token);
		}
		efree((yypminor->yy0));
	}*/
}
// 1909 "parser.c"
      break;
    case 79:
// 777 "parser.lemon"
{ json_object_put((yypminor->yy95)); }
// 1914 "parser.c"
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
  { 78, 1 },
  { 79, 1 },
  { 80, 2 },
  { 80, 1 },
  { 81, 1 },
  { 81, 1 },
  { 81, 1 },
  { 82, 3 },
  { 83, 5 },
  { 83, 7 },
  { 83, 6 },
  { 83, 8 },
  { 83, 4 },
  { 83, 6 },
  { 83, 5 },
  { 83, 7 },
  { 85, 1 },
  { 85, 1 },
  { 85, 1 },
  { 85, 2 },
  { 85, 2 },
  { 85, 2 },
  { 85, 2 },
  { 85, 3 },
  { 85, 3 },
  { 86, 2 },
  { 86, 1 },
  { 89, 4 },
  { 89, 3 },
  { 89, 5 },
  { 89, 6 },
  { 89, 3 },
  { 89, 4 },
  { 89, 5 },
  { 89, 6 },
  { 87, 2 },
  { 87, 1 },
  { 88, 2 },
  { 88, 1 },
  { 91, 6 },
  { 91, 5 },
  { 92, 7 },
  { 92, 8 },
  { 92, 8 },
  { 92, 9 },
  { 92, 8 },
  { 92, 9 },
  { 92, 9 },
  { 92, 10 },
  { 92, 9 },
  { 92, 7 },
  { 92, 8 },
  { 92, 8 },
  { 92, 9 },
  { 93, 3 },
  { 93, 1 },
  { 95, 1 },
  { 95, 2 },
  { 95, 2 },
  { 95, 3 },
  { 95, 4 },
  { 95, 4 },
  { 97, 3 },
  { 96, 1 },
  { 96, 1 },
  { 96, 1 },
  { 96, 1 },
  { 96, 1 },
  { 96, 1 },
  { 94, 2 },
  { 94, 1 },
  { 98, 1 },
  { 98, 1 },
  { 98, 1 },
  { 98, 1 },
  { 98, 1 },
  { 98, 1 },
  { 98, 1 },
  { 98, 1 },
  { 98, 1 },
  { 98, 1 },
  { 98, 1 },
  { 98, 1 },
  { 98, 1 },
  { 98, 1 },
  { 98, 1 },
  { 108, 2 },
  { 109, 2 },
  { 100, 4 },
  { 100, 7 },
  { 100, 5 },
  { 100, 9 },
  { 100, 8 },
  { 100, 8 },
  { 111, 4 },
  { 111, 5 },
  { 114, 2 },
  { 114, 1 },
  { 115, 4 },
  { 115, 3 },
  { 101, 3 },
  { 101, 4 },
  { 110, 4 },
  { 110, 5 },
  { 112, 7 },
  { 112, 8 },
  { 112, 9 },
  { 112, 10 },
  { 99, 3 },
  { 117, 3 },
  { 117, 1 },
  { 119, 1 },
  { 119, 1 },
  { 119, 1 },
  { 119, 1 },
  { 119, 1 },
  { 119, 1 },
  { 118, 3 },
  { 118, 5 },
  { 118, 7 },
  { 118, 8 },
  { 118, 5 },
  { 118, 6 },
  { 118, 2 },
  { 118, 2 },
  { 121, 1 },
  { 102, 3 },
  { 122, 3 },
  { 122, 1 },
  { 123, 1 },
  { 104, 2 },
  { 103, 3 },
  { 105, 5 },
  { 105, 6 },
  { 105, 6 },
  { 105, 6 },
  { 106, 3 },
  { 106, 7 },
  { 107, 3 },
  { 107, 3 },
  { 107, 3 },
  { 107, 3 },
  { 107, 3 },
  { 126, 3 },
  { 126, 1 },
  { 127, 1 },
  { 127, 3 },
  { 120, 1 },
  { 116, 2 },
  { 116, 2 },
  { 116, 3 },
  { 116, 3 },
  { 116, 3 },
  { 116, 3 },
  { 116, 3 },
  { 116, 3 },
  { 116, 3 },
  { 116, 3 },
  { 116, 3 },
  { 116, 3 },
  { 116, 4 },
  { 116, 4 },
  { 116, 4 },
  { 116, 3 },
  { 116, 3 },
  { 116, 3 },
  { 116, 3 },
  { 116, 3 },
  { 116, 2 },
  { 116, 4 },
  { 116, 2 },
  { 116, 1 },
  { 116, 1 },
  { 116, 1 },
  { 116, 1 },
  { 116, 1 },
  { 116, 1 },
  { 116, 1 },
  { 116, 2 },
  { 116, 3 },
  { 116, 2 },
  { 116, 4 },
  { 116, 5 },
  { 116, 4 },
  { 116, 3 },
  { 116, 1 },
  { 124, 6 },
  { 124, 5 },
  { 125, 3 },
  { 125, 1 },
  { 130, 1 },
  { 129, 3 },
  { 129, 1 },
  { 131, 3 },
  { 131, 1 },
  { 132, 1 },
  { 132, 1 },
  { 132, 1 },
  { 133, 1 },
  { 133, 1 },
  { 90, 1 },
  { 90, 1 },
  { 90, 1 },
  { 90, 1 },
  { 90, 1 },
  { 90, 1 },
  { 128, 4 },
  { 128, 4 },
  { 128, 4 },
  { 113, 1 },
  { 84, 1 },
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
// 773 "parser.lemon"
{
	status->ret = yymsp[0].minor.yy95;
}
// 2342 "parser.c"
        break;
      case 1:
      case 4:
      case 5:
      case 6:
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
      case 125:
      case 147:
      case 185:
      case 190:
      case 198:
      case 209:
// 779 "parser.lemon"
{
	yygotominor.yy95 = yymsp[0].minor.yy95;
}
// 2373 "parser.c"
        break;
      case 2:
      case 25:
      case 35:
      case 37:
      case 69:
      case 96:
// 783 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_list(yymsp[-1].minor.yy95, yymsp[0].minor.yy95);
}
// 2385 "parser.c"
        break;
      case 3:
      case 26:
      case 36:
      case 38:
      case 55:
      case 70:
      case 97:
      case 110:
      case 128:
      case 144:
      case 189:
      case 192:
// 787 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_list(NULL, yymsp[0].minor.yy95);
}
// 2403 "parser.c"
        break;
      case 7:
// 803 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_namespace(yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(23,&yymsp[0].minor);
}
// 2412 "parser.c"
        break;
      case 8:
// 807 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[-1].minor.yy95, 0, NULL, status->scanner_state);
  yy_destructor(24,&yymsp[-4].minor);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 2422 "parser.c"
        break;
      case 9:
// 811 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class(yymsp[-5].minor.yy0, yymsp[-1].minor.yy95, 0, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(24,&yymsp[-6].minor);
  yy_destructor(27,&yymsp[-4].minor);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 2433 "parser.c"
        break;
      case 10:
// 815 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[-1].minor.yy95, 1, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-5].minor);
  yy_destructor(24,&yymsp[-4].minor);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 2444 "parser.c"
        break;
      case 11:
// 819 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class(yymsp[-5].minor.yy0, yymsp[-1].minor.yy95, 1, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(28,&yymsp[-7].minor);
  yy_destructor(24,&yymsp[-6].minor);
  yy_destructor(27,&yymsp[-4].minor);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 2456 "parser.c"
        break;
      case 12:
// 823 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class(yymsp[-2].minor.yy0, NULL, 0, NULL, status->scanner_state);
  yy_destructor(24,&yymsp[-3].minor);
  yy_destructor(25,&yymsp[-1].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 2466 "parser.c"
        break;
      case 13:
// 827 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class(yymsp[-4].minor.yy0, NULL, 0, yymsp[-2].minor.yy0, status->scanner_state);
  yy_destructor(24,&yymsp[-5].minor);
  yy_destructor(27,&yymsp[-3].minor);
  yy_destructor(25,&yymsp[-1].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 2477 "parser.c"
        break;
      case 14:
// 831 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class(yymsp[-2].minor.yy0, NULL, 1, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-4].minor);
  yy_destructor(24,&yymsp[-3].minor);
  yy_destructor(25,&yymsp[-1].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 2488 "parser.c"
        break;
      case 15:
// 835 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class(yymsp[-4].minor.yy0, NULL, 1, yymsp[-2].minor.yy0, status->scanner_state);
  yy_destructor(28,&yymsp[-6].minor);
  yy_destructor(24,&yymsp[-5].minor);
  yy_destructor(27,&yymsp[-3].minor);
  yy_destructor(25,&yymsp[-1].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 2500 "parser.c"
        break;
      case 16:
// 839 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_definition(yymsp[0].minor.yy95, NULL, NULL, status->scanner_state);
}
// 2507 "parser.c"
        break;
      case 17:
// 843 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_definition(NULL, NULL, yymsp[0].minor.yy95, status->scanner_state);
}
// 2514 "parser.c"
        break;
      case 18:
// 847 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_definition(NULL, yymsp[0].minor.yy95, NULL, status->scanner_state);
}
// 2521 "parser.c"
        break;
      case 19:
// 851 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_definition(yymsp[-1].minor.yy95, yymsp[0].minor.yy95, NULL, status->scanner_state);
}
// 2528 "parser.c"
        break;
      case 20:
// 855 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_definition(yymsp[-1].minor.yy95, NULL, yymsp[0].minor.yy95, status->scanner_state);
}
// 2535 "parser.c"
        break;
      case 21:
// 859 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_definition(yymsp[0].minor.yy95, NULL, yymsp[-1].minor.yy95, status->scanner_state);
}
// 2542 "parser.c"
        break;
      case 22:
// 863 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_definition(NULL, yymsp[0].minor.yy95, yymsp[-1].minor.yy95, status->scanner_state);
}
// 2549 "parser.c"
        break;
      case 23:
// 867 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_definition(yymsp[-2].minor.yy95, yymsp[0].minor.yy95, yymsp[-1].minor.yy95, status->scanner_state);
}
// 2556 "parser.c"
        break;
      case 24:
// 871 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_definition(yymsp[-1].minor.yy95, yymsp[0].minor.yy95, yymsp[-2].minor.yy95, status->scanner_state);
}
// 2563 "parser.c"
        break;
      case 27:
// 883 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_property(XX_T_PUBLIC, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(23,&yymsp[0].minor);
}
// 2572 "parser.c"
        break;
      case 28:
// 887 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_property(XX_T_PUBLIC, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(23,&yymsp[0].minor);
}
// 2581 "parser.c"
        break;
      case 29:
// 891 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_property(XX_T_PUBLIC, yymsp[-3].minor.yy0, yymsp[-1].minor.yy95, NULL, status->scanner_state);
  yy_destructor(30,&yymsp[-4].minor);
  yy_destructor(31,&yymsp[-2].minor);
  yy_destructor(23,&yymsp[0].minor);
}
// 2591 "parser.c"
        break;
      case 30:
// 895 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_property(XX_T_PUBLIC, yymsp[-3].minor.yy0, yymsp[-1].minor.yy95, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(30,&yymsp[-4].minor);
  yy_destructor(31,&yymsp[-2].minor);
  yy_destructor(23,&yymsp[0].minor);
}
// 2601 "parser.c"
        break;
      case 31:
// 899 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_property(XX_T_PROTECTED, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(32,&yymsp[-2].minor);
  yy_destructor(23,&yymsp[0].minor);
}
// 2610 "parser.c"
        break;
      case 32:
// 903 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_property(XX_T_PROTECTED, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(32,&yymsp[-2].minor);
  yy_destructor(23,&yymsp[0].minor);
}
// 2619 "parser.c"
        break;
      case 33:
// 907 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_property(XX_T_PROTECTED, yymsp[-3].minor.yy0, yymsp[-1].minor.yy95, NULL, status->scanner_state);
  yy_destructor(32,&yymsp[-4].minor);
  yy_destructor(31,&yymsp[-2].minor);
  yy_destructor(23,&yymsp[0].minor);
}
// 2629 "parser.c"
        break;
      case 34:
// 911 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_property(XX_T_PROTECTED, yymsp[-3].minor.yy0, yymsp[-1].minor.yy95, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(32,&yymsp[-4].minor);
  yy_destructor(31,&yymsp[-2].minor);
  yy_destructor(23,&yymsp[0].minor);
}
// 2639 "parser.c"
        break;
      case 39:
// 931 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy95, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(33,&yymsp[-4].minor);
  yy_destructor(31,&yymsp[-2].minor);
  yy_destructor(23,&yymsp[0].minor);
}
// 2649 "parser.c"
        break;
      case 40:
// 935 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy95, NULL, status->scanner_state);
  yy_destructor(33,&yymsp[-4].minor);
  yy_destructor(31,&yymsp[-2].minor);
  yy_destructor(23,&yymsp[0].minor);
}
// 2659 "parser.c"
        break;
      case 41:
// 939 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-4].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(30,&yymsp[-6].minor);
  yy_destructor(34,&yymsp[-5].minor);
  yy_destructor(35,&yymsp[-3].minor);
  yy_destructor(36,&yymsp[-2].minor);
  yy_destructor(25,&yymsp[-1].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 2672 "parser.c"
        break;
      case 42:
// 943 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-5].minor.yy0, yymsp[-3].minor.yy95, NULL, NULL, status->scanner_state);
  yy_destructor(30,&yymsp[-7].minor);
  yy_destructor(34,&yymsp[-6].minor);
  yy_destructor(35,&yymsp[-4].minor);
  yy_destructor(36,&yymsp[-2].minor);
  yy_destructor(25,&yymsp[-1].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 2685 "parser.c"
        break;
      case 43:
// 947 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-4].minor.yy0, NULL, NULL, yymsp[-7].minor.yy0, status->scanner_state);
  yy_destructor(30,&yymsp[-6].minor);
  yy_destructor(34,&yymsp[-5].minor);
  yy_destructor(35,&yymsp[-3].minor);
  yy_destructor(36,&yymsp[-2].minor);
  yy_destructor(25,&yymsp[-1].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 2698 "parser.c"
        break;
      case 44:
// 951 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-5].minor.yy0, yymsp[-3].minor.yy95, NULL, yymsp[-8].minor.yy0, status->scanner_state);
  yy_destructor(30,&yymsp[-7].minor);
  yy_destructor(34,&yymsp[-6].minor);
  yy_destructor(35,&yymsp[-4].minor);
  yy_destructor(36,&yymsp[-2].minor);
  yy_destructor(25,&yymsp[-1].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 2711 "parser.c"
        break;
      case 45:
// 955 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy95, NULL, status->scanner_state);
  yy_destructor(30,&yymsp[-7].minor);
  yy_destructor(34,&yymsp[-6].minor);
  yy_destructor(35,&yymsp[-4].minor);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 2724 "parser.c"
        break;
      case 46:
// 959 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-6].minor.yy0, yymsp[-3].minor.yy95, yymsp[-1].minor.yy95, NULL, status->scanner_state);
  yy_destructor(30,&yymsp[-8].minor);
  yy_destructor(34,&yymsp[-7].minor);
  yy_destructor(35,&yymsp[-5].minor);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 2737 "parser.c"
        break;
      case 47:
// 963 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy95, yymsp[-8].minor.yy0, status->scanner_state);
  yy_destructor(30,&yymsp[-7].minor);
  yy_destructor(34,&yymsp[-6].minor);
  yy_destructor(35,&yymsp[-4].minor);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 2750 "parser.c"
        break;
      case 48:
// 967 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-6].minor.yy0, yymsp[-4].minor.yy95, yymsp[-1].minor.yy95, yymsp[-9].minor.yy0, status->scanner_state);
  yy_destructor(30,&yymsp[-8].minor);
  yy_destructor(34,&yymsp[-7].minor);
  yy_destructor(35,&yymsp[-5].minor);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 2763 "parser.c"
        break;
      case 49:
// 971 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-6].minor.yy0, yymsp[-4].minor.yy95, yymsp[-1].minor.yy95, NULL, status->scanner_state);
  yy_destructor(30,&yymsp[-8].minor);
  yy_destructor(34,&yymsp[-7].minor);
  yy_destructor(35,&yymsp[-5].minor);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 2776 "parser.c"
        break;
      case 50:
// 975 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_method(XX_T_PROTECTED, yymsp[-4].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(32,&yymsp[-6].minor);
  yy_destructor(34,&yymsp[-5].minor);
  yy_destructor(35,&yymsp[-3].minor);
  yy_destructor(36,&yymsp[-2].minor);
  yy_destructor(25,&yymsp[-1].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 2789 "parser.c"
        break;
      case 51:
// 979 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_method(XX_T_PROTECTED, yymsp[-4].minor.yy0, NULL, NULL, yymsp[-7].minor.yy0, status->scanner_state);
  yy_destructor(32,&yymsp[-6].minor);
  yy_destructor(34,&yymsp[-5].minor);
  yy_destructor(35,&yymsp[-3].minor);
  yy_destructor(36,&yymsp[-2].minor);
  yy_destructor(25,&yymsp[-1].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 2802 "parser.c"
        break;
      case 52:
// 983 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_method(XX_T_PROTECTED, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy95, NULL, status->scanner_state);
  yy_destructor(32,&yymsp[-7].minor);
  yy_destructor(34,&yymsp[-6].minor);
  yy_destructor(35,&yymsp[-4].minor);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 2815 "parser.c"
        break;
      case 53:
// 987 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_method(XX_T_PROTECTED, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy95, yymsp[-8].minor.yy0, status->scanner_state);
  yy_destructor(32,&yymsp[-7].minor);
  yy_destructor(34,&yymsp[-6].minor);
  yy_destructor(35,&yymsp[-4].minor);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 2828 "parser.c"
        break;
      case 54:
      case 109:
      case 127:
      case 143:
      case 188:
      case 191:
// 991 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_list(yymsp[-2].minor.yy95, yymsp[0].minor.yy95);
  yy_destructor(1,&yymsp[-1].minor);
}
// 2841 "parser.c"
        break;
      case 56:
// 999 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_parameter(NULL, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 2848 "parser.c"
        break;
      case 57:
// 1003 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_parameter(yymsp[-1].minor.yy95, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 2855 "parser.c"
        break;
      case 58:
// 1007 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_parameter(NULL, yymsp[-1].minor.yy95, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 2862 "parser.c"
        break;
      case 59:
// 1011 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_parameter(NULL, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy95, status->scanner_state);
  yy_destructor(31,&yymsp[-1].minor);
}
// 2870 "parser.c"
        break;
      case 60:
// 1015 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_parameter(yymsp[-3].minor.yy95, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy95, status->scanner_state);
  yy_destructor(31,&yymsp[-1].minor);
}
// 2878 "parser.c"
        break;
      case 61:
// 1019 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_parameter(NULL, yymsp[-3].minor.yy95, yymsp[-2].minor.yy0, yymsp[0].minor.yy95, status->scanner_state);
  yy_destructor(31,&yymsp[-1].minor);
}
// 2886 "parser.c"
        break;
      case 62:
// 1023 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(6,&yymsp[-2].minor);
  yy_destructor(7,&yymsp[0].minor);
}
// 2895 "parser.c"
        break;
      case 63:
// 1027 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_type(XX_TYPE_INTEGER);
  yy_destructor(37,&yymsp[0].minor);
}
// 2903 "parser.c"
        break;
      case 64:
// 1031 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_type(XX_TYPE_DOUBLE);
  yy_destructor(38,&yymsp[0].minor);
}
// 2911 "parser.c"
        break;
      case 65:
// 1035 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_type(XX_TYPE_BOOL);
  yy_destructor(39,&yymsp[0].minor);
}
// 2919 "parser.c"
        break;
      case 66:
// 1039 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_type(XX_TYPE_STRING);
  yy_destructor(40,&yymsp[0].minor);
}
// 2927 "parser.c"
        break;
      case 67:
// 1043 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_type(XX_TYPE_ARRAY);
  yy_destructor(41,&yymsp[0].minor);
}
// 2935 "parser.c"
        break;
      case 68:
// 1047 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_type(XX_TYPE_VAR);
  yy_destructor(42,&yymsp[0].minor);
}
// 2943 "parser.c"
        break;
      case 86:
// 1119 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_break_statement(status->scanner_state);
  yy_destructor(43,&yymsp[-1].minor);
  yy_destructor(23,&yymsp[0].minor);
}
// 2952 "parser.c"
        break;
      case 87:
// 1123 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_continue_statement(status->scanner_state);
  yy_destructor(44,&yymsp[-1].minor);
  yy_destructor(23,&yymsp[0].minor);
}
// 2961 "parser.c"
        break;
      case 88:
// 1127 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_if_statement(yymsp[-2].minor.yy95, NULL, NULL, status->scanner_state);
  yy_destructor(45,&yymsp[-3].minor);
  yy_destructor(25,&yymsp[-1].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 2971 "parser.c"
        break;
      case 89:
// 1131 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_if_statement(yymsp[-5].minor.yy95, NULL, NULL, status->scanner_state);
  yy_destructor(45,&yymsp[-6].minor);
  yy_destructor(25,&yymsp[-4].minor);
  yy_destructor(26,&yymsp[-3].minor);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(25,&yymsp[-1].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 2984 "parser.c"
        break;
      case 90:
// 1135 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_if_statement(yymsp[-3].minor.yy95, yymsp[-1].minor.yy95, NULL, status->scanner_state);
  yy_destructor(45,&yymsp[-4].minor);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 2994 "parser.c"
        break;
      case 91:
// 1139 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_if_statement(yymsp[-7].minor.yy95, yymsp[-5].minor.yy95, yymsp[-1].minor.yy95, status->scanner_state);
  yy_destructor(45,&yymsp[-8].minor);
  yy_destructor(25,&yymsp[-6].minor);
  yy_destructor(26,&yymsp[-4].minor);
  yy_destructor(46,&yymsp[-3].minor);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 3007 "parser.c"
        break;
      case 92:
// 1143 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_if_statement(yymsp[-6].minor.yy95, yymsp[-4].minor.yy95, NULL, status->scanner_state);
  yy_destructor(45,&yymsp[-7].minor);
  yy_destructor(25,&yymsp[-5].minor);
  yy_destructor(26,&yymsp[-3].minor);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(25,&yymsp[-1].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 3020 "parser.c"
        break;
      case 93:
// 1147 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_if_statement(yymsp[-6].minor.yy95, NULL, yymsp[-1].minor.yy95, status->scanner_state);
  yy_destructor(45,&yymsp[-7].minor);
  yy_destructor(25,&yymsp[-5].minor);
  yy_destructor(26,&yymsp[-4].minor);
  yy_destructor(46,&yymsp[-3].minor);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 3033 "parser.c"
        break;
      case 94:
// 1151 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_switch_statement(yymsp[-2].minor.yy95, NULL, status->scanner_state);
  yy_destructor(47,&yymsp[-3].minor);
  yy_destructor(25,&yymsp[-1].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 3043 "parser.c"
        break;
      case 95:
// 1155 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_switch_statement(yymsp[-3].minor.yy95, yymsp[-1].minor.yy95, status->scanner_state);
  yy_destructor(47,&yymsp[-4].minor);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 3053 "parser.c"
        break;
      case 98:
// 1167 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_case_clause(yymsp[-2].minor.yy95, yymsp[0].minor.yy95, status->scanner_state);
  yy_destructor(48,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-1].minor);
}
// 3062 "parser.c"
        break;
      case 99:
// 1171 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_case_clause(NULL, yymsp[0].minor.yy95, status->scanner_state);
  yy_destructor(50,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[-1].minor);
}
// 3071 "parser.c"
        break;
      case 100:
// 1175 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_loop_statement(NULL, status->scanner_state);
  yy_destructor(51,&yymsp[-2].minor);
  yy_destructor(25,&yymsp[-1].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 3081 "parser.c"
        break;
      case 101:
// 1179 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_loop_statement(yymsp[-1].minor.yy95, status->scanner_state);
  yy_destructor(51,&yymsp[-3].minor);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 3091 "parser.c"
        break;
      case 102:
// 1183 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_while_statement(yymsp[-2].minor.yy95, NULL, status->scanner_state);
  yy_destructor(52,&yymsp[-3].minor);
  yy_destructor(25,&yymsp[-1].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 3101 "parser.c"
        break;
      case 103:
// 1187 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_while_statement(yymsp[-3].minor.yy95, yymsp[-1].minor.yy95, status->scanner_state);
  yy_destructor(52,&yymsp[-4].minor);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 3111 "parser.c"
        break;
      case 104:
// 1191 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_for_statement(yymsp[-3].minor.yy95, NULL, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy95, status->scanner_state);
  yy_destructor(53,&yymsp[-6].minor);
  yy_destructor(54,&yymsp[-4].minor);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 3122 "parser.c"
        break;
      case 105:
// 1195 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_for_statement(yymsp[-3].minor.yy95, NULL, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy95, status->scanner_state);
  yy_destructor(53,&yymsp[-7].minor);
  yy_destructor(54,&yymsp[-5].minor);
  yy_destructor(55,&yymsp[-4].minor);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 3134 "parser.c"
        break;
      case 106:
// 1199 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_for_statement(yymsp[-3].minor.yy95, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy95, status->scanner_state);
  yy_destructor(53,&yymsp[-8].minor);
  yy_destructor(1,&yymsp[-6].minor);
  yy_destructor(54,&yymsp[-4].minor);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 3146 "parser.c"
        break;
      case 107:
// 1203 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_for_statement(yymsp[-3].minor.yy95, yymsp[-8].minor.yy0, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy95, status->scanner_state);
  yy_destructor(53,&yymsp[-9].minor);
  yy_destructor(1,&yymsp[-7].minor);
  yy_destructor(54,&yymsp[-5].minor);
  yy_destructor(55,&yymsp[-4].minor);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 3159 "parser.c"
        break;
      case 108:
// 1207 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_let_statement(yymsp[-1].minor.yy95, status->scanner_state);
  yy_destructor(56,&yymsp[-2].minor);
  yy_destructor(23,&yymsp[0].minor);
}
// 3168 "parser.c"
        break;
      case 111:
// 1220 "parser.lemon"
{
	yygotominor.yy95 = json_object_new_string("assign");
  yy_destructor(31,&yymsp[0].minor);
}
// 3176 "parser.c"
        break;
      case 112:
// 1225 "parser.lemon"
{
	yygotominor.yy95 = json_object_new_string("add-assign");
  yy_destructor(57,&yymsp[0].minor);
}
// 3184 "parser.c"
        break;
      case 113:
// 1230 "parser.lemon"
{
	yygotominor.yy95 = json_object_new_string("sub-assign");
  yy_destructor(58,&yymsp[0].minor);
}
// 3192 "parser.c"
        break;
      case 114:
// 1234 "parser.lemon"
{
	yygotominor.yy95 = json_object_new_string("mult-assign");
  yy_destructor(59,&yymsp[0].minor);
}
// 3200 "parser.c"
        break;
      case 115:
// 1238 "parser.lemon"
{
	yygotominor.yy95 = json_object_new_string("div-assign");
  yy_destructor(60,&yymsp[0].minor);
}
// 3208 "parser.c"
        break;
      case 116:
// 1242 "parser.lemon"
{
	yygotominor.yy95 = json_object_new_string("concat-assign");
  yy_destructor(61,&yymsp[0].minor);
}
// 3216 "parser.c"
        break;
      case 117:
// 1247 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_let_assignment("variable", yymsp[-1].minor.yy95, yymsp[-2].minor.yy0, NULL, NULL, yymsp[0].minor.yy95, status->scanner_state);
}
// 3223 "parser.c"
        break;
      case 118:
// 1252 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_let_assignment("object-property", yymsp[-1].minor.yy95, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy95, status->scanner_state);
  yy_destructor(62,&yymsp[-3].minor);
}
// 3231 "parser.c"
        break;
      case 119:
// 1257 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_let_assignment("object-property-append", yymsp[-1].minor.yy95, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy95, status->scanner_state);
  yy_destructor(62,&yymsp[-5].minor);
  yy_destructor(63,&yymsp[-3].minor);
  yy_destructor(64,&yymsp[-2].minor);
}
// 3241 "parser.c"
        break;
      case 120:
// 1262 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_let_assignment("object-property-array-index", yymsp[-1].minor.yy95, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy95, yymsp[0].minor.yy95, status->scanner_state);
  yy_destructor(62,&yymsp[-6].minor);
  yy_destructor(63,&yymsp[-4].minor);
  yy_destructor(64,&yymsp[-2].minor);
}
// 3251 "parser.c"
        break;
      case 121:
// 1267 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_let_assignment("variable-append", yymsp[-1].minor.yy95, yymsp[-4].minor.yy0, NULL, NULL, yymsp[0].minor.yy95, status->scanner_state);
  yy_destructor(63,&yymsp[-3].minor);
  yy_destructor(64,&yymsp[-2].minor);
}
// 3260 "parser.c"
        break;
      case 122:
// 1272 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_let_assignment("array-index", yymsp[-1].minor.yy95, yymsp[-5].minor.yy0, NULL, yymsp[-3].minor.yy95, yymsp[0].minor.yy95, status->scanner_state);
  yy_destructor(63,&yymsp[-4].minor);
  yy_destructor(64,&yymsp[-2].minor);
}
// 3269 "parser.c"
        break;
      case 123:
// 1277 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_let_assignment("incr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(65,&yymsp[0].minor);
}
// 3277 "parser.c"
        break;
      case 124:
// 1282 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_let_assignment("decr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(66,&yymsp[0].minor);
}
// 3285 "parser.c"
        break;
      case 126:
// 1290 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_echo_statement(yymsp[-1].minor.yy95, status->scanner_state);
  yy_destructor(67,&yymsp[-2].minor);
  yy_destructor(23,&yymsp[0].minor);
}
// 3294 "parser.c"
        break;
      case 129:
// 1302 "parser.lemon"
{
	yygotominor.yy95 = yymsp[0].minor.yy95;;
}
// 3301 "parser.c"
        break;
      case 130:
// 1307 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_mcall_statement(yymsp[-1].minor.yy95, status->scanner_state);
  yy_destructor(23,&yymsp[0].minor);
}
// 3309 "parser.c"
        break;
      case 131:
// 1312 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_return_statement(yymsp[-1].minor.yy95, status->scanner_state);
  yy_destructor(68,&yymsp[-2].minor);
  yy_destructor(23,&yymsp[0].minor);
}
// 3318 "parser.c"
        break;
      case 132:
// 1317 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(69,&yymsp[-4].minor);
  yy_destructor(62,&yymsp[-2].minor);
  yy_destructor(23,&yymsp[0].minor);
}
// 3332 "parser.c"
        break;
      case 133:
// 1326 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(69,&yymsp[-5].minor);
  yy_destructor(63,&yymsp[-3].minor);
  yy_destructor(64,&yymsp[-1].minor);
  yy_destructor(23,&yymsp[0].minor);
}
// 3347 "parser.c"
        break;
      case 134:
// 1335 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_INTEGER, yymsp[-2].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(69,&yymsp[-5].minor);
  yy_destructor(63,&yymsp[-3].minor);
  yy_destructor(64,&yymsp[-1].minor);
  yy_destructor(23,&yymsp[0].minor);
}
// 3362 "parser.c"
        break;
      case 135:
// 1344 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_STRING, yymsp[-2].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(69,&yymsp[-5].minor);
  yy_destructor(63,&yymsp[-3].minor);
  yy_destructor(64,&yymsp[-1].minor);
  yy_destructor(23,&yymsp[0].minor);
}
// 3377 "parser.c"
        break;
      case 136:
// 1353 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_throw_exception(yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(72,&yymsp[-2].minor);
  yy_destructor(23,&yymsp[0].minor);
}
// 3386 "parser.c"
        break;
      case 137:
// 1358 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_throw_exception(yymsp[-4].minor.yy0, yymsp[-2].minor.yy95, status->scanner_state);
  yy_destructor(72,&yymsp[-6].minor);
  yy_destructor(19,&yymsp[-5].minor);
  yy_destructor(35,&yymsp[-3].minor);
  yy_destructor(36,&yymsp[-1].minor);
  yy_destructor(23,&yymsp[0].minor);
}
// 3398 "parser.c"
        break;
      case 138:
// 1362 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_declare_statement(XX_T_TYPE_INTEGER, yymsp[-1].minor.yy95, status->scanner_state);
  yy_destructor(37,&yymsp[-2].minor);
  yy_destructor(23,&yymsp[0].minor);
}
// 3407 "parser.c"
        break;
      case 139:
// 1366 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_declare_statement(XX_T_TYPE_DOUBLE, yymsp[-1].minor.yy95, status->scanner_state);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(23,&yymsp[0].minor);
}
// 3416 "parser.c"
        break;
      case 140:
// 1370 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_declare_statement(XX_T_TYPE_STRING, yymsp[-1].minor.yy95, status->scanner_state);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(23,&yymsp[0].minor);
}
// 3425 "parser.c"
        break;
      case 141:
// 1374 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_declare_statement(XX_T_TYPE_BOOL, yymsp[-1].minor.yy95, status->scanner_state);
  yy_destructor(39,&yymsp[-2].minor);
  yy_destructor(23,&yymsp[0].minor);
}
// 3434 "parser.c"
        break;
      case 142:
// 1378 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_declare_statement(XX_T_TYPE_VAR, yymsp[-1].minor.yy95, status->scanner_state);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(23,&yymsp[0].minor);
}
// 3443 "parser.c"
        break;
      case 145:
// 1390 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_declare_variable(yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3450 "parser.c"
        break;
      case 146:
// 1394 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_declare_variable(yymsp[-2].minor.yy0, yymsp[0].minor.yy95, status->scanner_state);
  yy_destructor(31,&yymsp[-1].minor);
}
// 3458 "parser.c"
        break;
      case 148:
// 1402 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_expr("not", yymsp[0].minor.yy95, NULL, NULL, status->scanner_state);
  yy_destructor(20,&yymsp[-1].minor);
}
// 3466 "parser.c"
        break;
      case 149:
// 1406 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_expr("isset", yymsp[0].minor.yy95, NULL, NULL, status->scanner_state);
  yy_destructor(15,&yymsp[-1].minor);
}
// 3474 "parser.c"
        break;
      case 150:
// 1410 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_expr("equals", yymsp[-2].minor.yy95, yymsp[0].minor.yy95, NULL, status->scanner_state);
  yy_destructor(4,&yymsp[-1].minor);
}
// 3482 "parser.c"
        break;
      case 151:
// 1414 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_expr("not-equals", yymsp[-2].minor.yy95, yymsp[0].minor.yy95, NULL, status->scanner_state);
  yy_destructor(11,&yymsp[-1].minor);
}
// 3490 "parser.c"
        break;
      case 152:
// 1418 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_expr("identical", yymsp[-2].minor.yy95, yymsp[0].minor.yy95, NULL, status->scanner_state);
  yy_destructor(5,&yymsp[-1].minor);
}
// 3498 "parser.c"
        break;
      case 153:
// 1422 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_expr("not-identical", yymsp[-2].minor.yy95, yymsp[0].minor.yy95, NULL, status->scanner_state);
  yy_destructor(10,&yymsp[-1].minor);
}
// 3506 "parser.c"
        break;
      case 154:
// 1426 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_expr("less", yymsp[-2].minor.yy95, yymsp[0].minor.yy95, NULL, status->scanner_state);
  yy_destructor(6,&yymsp[-1].minor);
}
// 3514 "parser.c"
        break;
      case 155:
// 1430 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_expr("greater", yymsp[-2].minor.yy95, yymsp[0].minor.yy95, NULL, status->scanner_state);
  yy_destructor(7,&yymsp[-1].minor);
}
// 3522 "parser.c"
        break;
      case 156:
// 1434 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_expr("less-equal", yymsp[-2].minor.yy95, yymsp[0].minor.yy95, NULL, status->scanner_state);
  yy_destructor(8,&yymsp[-1].minor);
}
// 3530 "parser.c"
        break;
      case 157:
// 1438 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_expr("greater-equal", yymsp[-2].minor.yy95, yymsp[0].minor.yy95, NULL, status->scanner_state);
  yy_destructor(9,&yymsp[-1].minor);
}
// 3538 "parser.c"
        break;
      case 158:
// 1442 "parser.lemon"
{
	yygotominor.yy95 = yymsp[-1].minor.yy95;
  yy_destructor(35,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 3547 "parser.c"
        break;
      case 159:
// 1446 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_expr("property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(62,&yymsp[-1].minor);
}
// 3555 "parser.c"
        break;
      case 160:
      case 206:
// 1450 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(63,&yymsp[-2].minor);
  yy_destructor(64,&yymsp[0].minor);
}
// 3565 "parser.c"
        break;
      case 161:
      case 207:
// 1455 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_INTEGER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(63,&yymsp[-2].minor);
  yy_destructor(64,&yymsp[0].minor);
}
// 3575 "parser.c"
        break;
      case 162:
      case 208:
// 1460 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_STRING, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(63,&yymsp[-2].minor);
  yy_destructor(64,&yymsp[0].minor);
}
// 3585 "parser.c"
        break;
      case 163:
// 1465 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_expr("add", yymsp[-2].minor.yy95, yymsp[0].minor.yy95, NULL, status->scanner_state);
  yy_destructor(12,&yymsp[-1].minor);
}
// 3593 "parser.c"
        break;
      case 164:
// 1470 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_expr("sub", yymsp[-2].minor.yy95, yymsp[0].minor.yy95, NULL, status->scanner_state);
  yy_destructor(13,&yymsp[-1].minor);
}
// 3601 "parser.c"
        break;
      case 165:
// 1475 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_expr("concat", yymsp[-2].minor.yy95, yymsp[0].minor.yy95, NULL, status->scanner_state);
  yy_destructor(14,&yymsp[-1].minor);
}
// 3609 "parser.c"
        break;
      case 166:
// 1480 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_expr("and", yymsp[-2].minor.yy95, yymsp[0].minor.yy95, NULL, status->scanner_state);
  yy_destructor(2,&yymsp[-1].minor);
}
// 3617 "parser.c"
        break;
      case 167:
// 1485 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_expr("or", yymsp[-2].minor.yy95, yymsp[0].minor.yy95, NULL, status->scanner_state);
  yy_destructor(3,&yymsp[-1].minor);
}
// 3625 "parser.c"
        break;
      case 168:
// 1490 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_expr("instanceof", yymsp[0].minor.yy95, NULL, NULL, status->scanner_state);
  yy_destructor(18,&yymsp[-1].minor);
}
// 3633 "parser.c"
        break;
      case 169:
// 1495 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_expr("fetch", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy95, NULL, status->scanner_state);
  yy_destructor(16,&yymsp[-3].minor);
  yy_destructor(1,&yymsp[-1].minor);
}
// 3642 "parser.c"
        break;
      case 170:
// 1500 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_expr("typeof", yymsp[0].minor.yy95, NULL, NULL, status->scanner_state);
  yy_destructor(17,&yymsp[-1].minor);
}
// 3650 "parser.c"
        break;
      case 171:
      case 195:
      case 199:
// 1505 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state);
}
// 3659 "parser.c"
        break;
      case 172:
      case 197:
      case 200:
// 1510 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_literal(XX_T_INTEGER, yymsp[0].minor.yy0, status->scanner_state);
}
// 3668 "parser.c"
        break;
      case 173:
      case 196:
      case 201:
// 1515 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_literal(XX_T_STRING, yymsp[0].minor.yy0, status->scanner_state);
}
// 3677 "parser.c"
        break;
      case 174:
      case 202:
// 1520 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_literal(XX_T_DOUBLE, yymsp[0].minor.yy0, status->scanner_state);
}
// 3685 "parser.c"
        break;
      case 175:
      case 203:
// 1525 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_literal(XX_T_NULL, NULL, status->scanner_state);
  yy_destructor(74,&yymsp[0].minor);
}
// 3694 "parser.c"
        break;
      case 176:
      case 205:
// 1530 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_literal(XX_T_TRUE, NULL, status->scanner_state);
  yy_destructor(75,&yymsp[0].minor);
}
// 3703 "parser.c"
        break;
      case 177:
      case 204:
// 1535 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_literal(XX_T_FALSE, NULL, status->scanner_state);
  yy_destructor(76,&yymsp[0].minor);
}
// 3712 "parser.c"
        break;
      case 178:
// 1540 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_expr("empty-array", NULL, NULL, NULL, status->scanner_state);
  yy_destructor(63,&yymsp[-1].minor);
  yy_destructor(64,&yymsp[0].minor);
}
// 3721 "parser.c"
        break;
      case 179:
// 1545 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_expr("array", yymsp[-1].minor.yy95, NULL, NULL, status->scanner_state);
  yy_destructor(63,&yymsp[-2].minor);
  yy_destructor(64,&yymsp[0].minor);
}
// 3730 "parser.c"
        break;
      case 180:
// 1550 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_new_instance(yymsp[0].minor.yy0, NULL, status->scanner_state);
  yy_destructor(19,&yymsp[-1].minor);
}
// 3738 "parser.c"
        break;
      case 181:
// 1555 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_new_instance(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(19,&yymsp[-3].minor);
  yy_destructor(35,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 3748 "parser.c"
        break;
      case 182:
// 1560 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_new_instance(yymsp[-3].minor.yy0, yymsp[-1].minor.yy95, status->scanner_state);
  yy_destructor(19,&yymsp[-4].minor);
  yy_destructor(35,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 3758 "parser.c"
        break;
      case 183:
// 1565 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_fcall(yymsp[-3].minor.yy0, yymsp[-1].minor.yy95, status->scanner_state);
  yy_destructor(35,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 3767 "parser.c"
        break;
      case 184:
// 1570 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_fcall(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(35,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 3776 "parser.c"
        break;
      case 186:
// 1580 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_mcall(yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy95, status->scanner_state);
  yy_destructor(62,&yymsp[-4].minor);
  yy_destructor(35,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 3786 "parser.c"
        break;
      case 187:
// 1585 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_mcall(yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(62,&yymsp[-3].minor);
  yy_destructor(35,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 3796 "parser.c"
        break;
      case 193:
// 1609 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_array_item(yymsp[-2].minor.yy95, yymsp[0].minor.yy95, status->scanner_state);
  yy_destructor(49,&yymsp[-1].minor);
}
// 3804 "parser.c"
        break;
      case 194:
// 1613 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_array_item(NULL, yymsp[0].minor.yy95, status->scanner_state);
}
// 3811 "parser.c"
        break;
      case 210:
// 1680 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_comment(yymsp[0].minor.yy0, status->scanner_state);
}
// 3818 "parser.c"
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
// 730 "parser.lemon"


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

// 3894 "parser.c"
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
