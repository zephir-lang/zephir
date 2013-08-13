/* Driver template for the LEMON parser generator.
** The author disclaims copyright to this source code.
*/
/* First off, code is include which follows the "include" declaration
** in the input file. */
#include <stdio.h>
// 40 "parser.lemon"


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
#define YYNOCODE 133
#define YYACTIONTYPE unsigned short int
#define xx_TOKENTYPE xx_parser_token*
typedef union {
  xx_TOKENTYPE yy0;
  json_object* yy247;
  int yy265;
} YYMINORTYPE;
#define YYSTACKDEPTH 100
#define xx_ARG_SDECL xx_parser_status *status;
#define xx_ARG_PDECL ,xx_parser_status *status
#define xx_ARG_FETCH xx_parser_status *status = yypParser->status
#define xx_ARG_STORE yypParser->status = status
#define YYNSTATE 454
#define YYNRULE 209
#define YYERRORSYMBOL 75
#define YYERRSYMDT yy265
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
 /*     0 */   282,  385,   17,  400,   25,  426,  388,  285,  664,    1,
 /*    10 */     2,  453,    4,    5,    6,  261,  270,  276,  273,  454,
 /*    20 */   279,   44,   46,   48,  345,   57,  344,  327,  331,   68,
 /*    30 */    72,   77,   61,  310,   84,   15,  282,  163,  401,  414,
 /*    40 */    27,  420,  395,  285,    8,  225,  234,  237,  239,  242,
 /*    50 */   252,  261,  270,  276,  273,  158,  279,   44,   46,   48,
 /*    60 */   318,   57,  344,  327,  331,   68,   72,   77,   11,  292,
 /*    70 */    84,  470,  282,   14,  401,  414,  312,  420,  395,  285,
 /*    80 */   162,  225,  234,  237,  262,  269,  252,  261,  270,  276,
 /*    90 */   273,  139,  279,   44,   46,   48,   12,   57,  434,  183,
 /*   100 */   186,   68,   72,   77,   18,  185,   84,  474,  282,  403,
 /*   110 */   389,   19,   52,  376,  395,  285,  157,  225,  234,  237,
 /*   120 */    79,  353,  252,  261,  270,  276,  273,  139,  279,   44,
 /*   130 */    46,   48,   80,   57,    9,  183,  180,   68,   72,   77,
 /*   140 */   183,  185,   84,  471,  282,   21,  401,  414,  311,  420,
 /*   150 */   395,  285,   20,  225,  234,  237,   85,  224,  252,  261,
 /*   160 */   270,  276,  273,   22,  279,   44,   46,   48,  361,   57,
 /*   170 */   344,  327,  331,   68,   72,   77,  409,  322,   84,  439,
 /*   180 */   282,   50,  401,  414,   56,  420,  395,  285,  368,  225,
 /*   190 */   234,  237,   45,   87,  252,  261,  270,  276,  273,  319,
 /*   200 */   279,   44,   46,   48,   98,   57,  271,  269,   97,   68,
 /*   210 */    72,   77,  183,   86,   84,  446,  282,   53,  401,  414,
 /*   220 */   306,  420,  395,  285,   47,  225,  234,  237,   54,  228,
 /*   230 */   252,  261,  270,  276,  273,  200,  279,   44,   46,   48,
 /*   240 */    98,   57,   59,  183,  204,   68,   72,   77,  183,  227,
 /*   250 */    84,  452,  282,   65,  401,  414,   71,  420,  395,  285,
 /*   260 */    63,  225,  234,  237,  274,  269,  252,  261,  270,  276,
 /*   270 */   273,  264,  279,   44,   46,   48,  139,   57,  323,  327,
 /*   280 */   331,   68,   72,   77,  183,  197,   84,  264,  282,   66,
 /*   290 */   185,  263,  304,    7,  264,  285,   10,  225,  234,  237,
 /*   300 */   440,  285,  252,  261,  270,  276,  273,  272,  279,   44,
 /*   310 */    46,   48,   98,   57,  275,   69,  208,   68,   72,   77,
 /*   320 */   183,   96,   84,  473,  282,   74,  351,   19,   76,  376,
 /*   330 */   182,  285,  264,  225,  234,  237,  277,  269,  252,  261,
 /*   340 */   270,  276,  273,   78,  279,   44,   46,   48,  139,   57,
 /*   350 */    89,  101,  278,   68,   72,   77,  183,  258,   84,  183,
 /*   360 */   282,   88,  185,  181,   83,  196,  264,  285,  105,  225,
 /*   370 */   234,  237,  280,  269,  252,  261,  270,  276,  273,  104,
 /*   380 */   279,   44,   46,   48,  386,   57,  281,  387,  388,   68,
 /*   390 */    72,   77,  136,  189,   84,  477,  282,  415,  351,   19,
 /*   400 */   291,  376,  352,  285,  367,  225,  234,  237,  106,   20,
 /*   410 */   252,  261,  270,  276,  273,  139,  279,   44,   46,   48,
 /*   420 */    98,   57,  108,  183,  212,   68,   72,   77,  183,  184,
 /*   430 */    84,  475,  282,  421,  430,  414,  298,  420,  402,  285,
 /*   440 */   408,  225,  234,  237,  110,  377,  252,  261,  270,  276,
 /*   450 */   273,  288,  279,   44,   46,   48,   98,   57,  112,  183,
 /*   460 */   217,   68,   72,   77,  183,  137,   84,  478,  282,  138,
 /*   470 */   351,   19,  303,  376,  443,  285,  447,  225,  234,  237,
 /*   480 */   143,  144,  252,  261,  270,  276,  273,  295,  279,   44,
 /*   490 */    46,   48,   98,   57,  145,  183,  221,   68,   72,   77,
 /*   500 */   183,  161,   84,  476,  282,  647,  351,   19,  317,  376,
 /*   510 */   648,  285,  649,  225,  234,  237,  177,  191,  252,  261,
 /*   520 */   270,  276,  273,  178,  279,   44,   46,   48,  352,   57,
 /*   530 */   367,  390,  193,   68,   72,   77,  195,  202,   84,  472,
 /*   540 */   282,  203,  351,   19,  316,  376,  182,  285,  207,  225,
 /*   550 */   234,  237,  211,  210,  252,  261,  270,  276,  273,  216,
 /*   560 */   279,   44,   46,   48,   49,   57,  219,  305,  233,   68,
 /*   570 */    72,   77,  238,  220,   84,  183,  282,  240,   58,  187,
 /*   580 */   321,  305,  182,  285,  241,  225,  234,  237,  244,  183,
 /*   590 */   252,  261,  270,  276,  273,  245,  279,   44,   46,   48,
 /*   600 */    73,   57,  107,  305,  247,   68,   72,   77,  248,  251,
 /*   610 */    84,  183,  282,  250,   60,  198,  348,  230,  256,  285,
 /*   620 */   254,  225,  234,  237,  229,  183,  252,  261,  270,  276,
 /*   630 */   273,  260,  279,   44,   46,   48,   64,   57,  307,  213,
 /*   640 */   257,   68,   72,   77,  209,  113,   84,  183,  282,  266,
 /*   650 */   109,  111,  350,  183,  265,  285,  267,  225,  234,  237,
 /*   660 */   268,   62,  252,  261,  270,  276,  273,  284,  279,   44,
 /*   670 */    46,   48,  402,   57,  408,  390,  283,   68,   72,   77,
 /*   680 */   293,  308,   84,   64,  282,  307,  429,  400,  358,  115,
 /*   690 */   388,  285,  294,  225,  234,  237,  313,  183,  252,  261,
 /*   700 */   270,  276,  273,  314,  279,   44,   46,   48,  325,   57,
 /*   710 */   326,  213,  328,   68,   72,   77,  218,  117,   84,  183,
 /*   720 */   282,  330,  182,  255,  360,  183,  253,  285,  329,  225,
 /*   730 */   234,  237,  332,  333,  252,  261,  270,  276,  273,  119,
 /*   740 */   279,   44,   46,   48,  121,   57,  334,  183,  336,   68,
 /*   750 */    72,   77,  183,  123,   84,  259,  282,  337,  347,  516,
 /*   760 */   364,  183,  322,  285,  517,  225,  234,  237,  518,  519,
 /*   770 */   252,  261,  270,  276,  273,  353,  279,   44,   46,   48,
 /*   780 */   125,   57,  520,  127,  521,   68,   72,   77,  183,  129,
 /*   790 */    84,  183,  282,  522,  354,  346,  366,  183,  322,  285,
 /*   800 */   355,  225,  234,  237,  357,  363,  252,  261,  270,  276,
 /*   810 */   273,  368,  279,   44,   46,   48,  131,   57,  369,  133,
 /*   820 */   370,   68,   72,   77,  183,  371,   84,  183,  282,  372,
 /*   830 */   378,  362,  373,  141,  377,  285,  379,  225,  234,  237,
 /*   840 */   381,  183,  252,  261,  270,  276,  273,  380,  279,   44,
 /*   850 */    46,   48,  147,   57,  391,  235,  392,   68,   72,   77,
 /*   860 */   183,  394,   84,  183,  282,  393,  396,  397,  375,  300,
 /*   870 */   399,  285,  398,  225,  234,  237,  406,  183,  252,  261,
 /*   880 */   270,  276,  273,  407,  279,   44,   46,   48,  412,   57,
 /*   890 */   413,  404,  418,   68,   72,   77,  419,  410,   84,  405,
 /*   900 */   282,  436,  424,  425,  382,  411,  435,  285,  438,  225,
 /*   910 */   234,  237,  441,  442,  252,  261,  270,  276,  273,  416,
 /*   920 */   279,   44,   46,   48,  445,   57,  448,  417,  449,   68,
 /*   930 */    72,   77,  451,  422,   84,  343,  282,  343,  343,  343,
 /*   940 */   384,  423,  343,  285,  343,  225,  234,  237,   90,  343,
 /*   950 */   252,  261,  270,  276,  273,  343,  279,   44,   46,   48,
 /*   960 */   343,   57,  343,  343,  343,   68,   72,   77,  343,  163,
 /*   970 */    84,  343,  282,  163,   91,   92,   93,   94,   95,  285,
 /*   980 */   205,  225,  234,  237,  343,  343,  252,  261,  270,  276,
 /*   990 */   273,  343,  279,   44,   46,   48,  343,   57,  126,  128,
 /*  1000 */   130,   68,   72,   77,  343,  343,   84,  343,  156,  343,
 /*  1010 */   175,  160,  171,   43,  159,  160,  171,  225,  234,  237,
 /*  1020 */   343,  343,  252,   26,  343,  343,  343,  286,   29,   30,
 /*  1030 */    31,   32,   33,   34,   35,   36,   37,   38,   39,   40,
 /*  1040 */    41,   42,   43,  165,  166,  343,  167,  168,  170,  169,
 /*  1050 */   343,  343,   51,  232,  172,  343,  286,   29,   30,   31,
 /*  1060 */    32,   33,   34,   35,   36,   37,   38,   39,   40,   41,
 /*  1070 */    42,   43,  343,   13,   16,  427,  433,  432,  343,  426,
 /*  1080 */   388,   55,  232,  343,  343,  286,   29,   30,   31,   32,
 /*  1090 */    33,   34,   35,   36,   37,   38,   39,   40,   41,   42,
 /*  1100 */    43,  343,  174,  173,   90,  167,  168,  170,  169,  343,
 /*  1110 */    67,  232,  343,  343,  286,   29,   30,   31,   32,   33,
 /*  1120 */    34,   35,   36,   37,   38,   39,   40,   41,   42,  343,
 /*  1130 */    91,   92,   93,   94,   95,   43,    3,    4,    5,    6,
 /*  1140 */   232,  343,  343,  343,  343,   70,  164,  343,  343,  286,
 /*  1150 */    29,   30,   31,   32,   33,   34,   35,   36,   37,   38,
 /*  1160 */    39,   40,   41,   42,   43,  343,  437,   16,  427,  433,
 /*  1170 */   432,  343,  426,  388,   75,  232,  190,  343,  286,   29,
 /*  1180 */    30,   31,   32,   33,   34,   35,   36,   37,   38,   39,
 /*  1190 */    40,   41,   42,   43,  165,  166,  343,  167,  168,  170,
 /*  1200 */   169,  343,  343,   82,  232,  343,  343,  286,   29,   30,
 /*  1210 */    31,   32,   33,   34,   35,   36,   37,   38,   39,   40,
 /*  1220 */    41,   42,  343,  230,  192,  194,  343,  343,   43,  226,
 /*  1230 */   231,  183,  343,  232,  428,  343,  431,  432,  290,  387,
 /*  1240 */   388,  343,  286,   29,   30,   31,   32,   33,   34,   35,
 /*  1250 */    36,   37,   38,   39,   40,   41,   42,   43,  343,  444,
 /*  1260 */    16,  427,  433,  432,  343,  426,  388,  297,  232,  343,
 /*  1270 */   243,  286,   29,   30,   31,   32,   33,   34,   35,   36,
 /*  1280 */    37,   38,   39,   40,   41,   42,   43,  343,  450,   16,
 /*  1290 */   427,  433,  432,  343,  426,  388,  302,  232,  455,  343,
 /*  1300 */   286,   29,   30,   31,   32,   33,   34,   35,   36,   37,
 /*  1310 */    38,   39,   40,   41,   42,  343,  343,    7,  246,  249,
 /*  1320 */    10,   43,  343,  343,  440,  285,  232,  343,  343,  343,
 /*  1330 */   343,  309,  343,  343,  343,  286,   29,   30,   31,   32,
 /*  1340 */    33,   34,   35,   36,   37,   38,   39,   40,   41,   42,
 /*  1350 */    43,  343,  343,  343,  343,  343,  343,  343,  343,  343,
 /*  1360 */   315,  232,  343,  343,  286,   29,   30,   31,   32,   33,
 /*  1370 */    34,   35,   36,   37,   38,   39,   40,   41,   42,   43,
 /*  1380 */   343,  343,  343,  343,  343,  343,  343,  343,  343,  320,
 /*  1390 */   232,  343,  343,  286,   29,   30,   31,   32,   33,   34,
 /*  1400 */    35,   36,   37,   38,   39,   40,   41,   42,  343,  343,
 /*  1410 */   343,  343,  343,  343,   43,  343,  343,  343,  343,  232,
 /*  1420 */   343,  343,  343,  343,  349,  343,  343,  343,  286,   29,
 /*  1430 */    30,   31,   32,   33,   34,   35,   36,   37,   38,   39,
 /*  1440 */    40,   41,   42,   43,  343,  343,  343,  343,  343,  343,
 /*  1450 */   343,  343,  343,  359,  232,  343,  343,  286,   29,   30,
 /*  1460 */    31,   32,   33,   34,   35,   36,   37,   38,   39,   40,
 /*  1470 */    41,   42,   43,  343,  343,  343,  343,  343,  343,  343,
 /*  1480 */   343,  343,  365,  232,  343,  343,  286,   29,   30,   31,
 /*  1490 */    32,   33,   34,   35,   36,   37,   38,   39,   40,   41,
 /*  1500 */    42,  343,  343,  343,  343,  343,  343,   43,  343,  343,
 /*  1510 */   343,  343,  232,  343,  343,  343,  343,  374,  343,  343,
 /*  1520 */   343,  286,   29,   30,   31,   32,   33,   34,   35,   36,
 /*  1530 */    37,   38,   39,   40,   41,   42,   43,  343,  343,  343,
 /*  1540 */   343,  343,  343,  343,  343,  343,  383,  232,  343,  343,
 /*  1550 */   286,   29,   30,   31,   32,   33,   34,   35,   36,   37,
 /*  1560 */    38,   39,   40,   41,   42,   43,  343,  343,  343,  343,
 /*  1570 */   343,  343,  343,  343,  343,  343,  232,  343,  343,   28,
 /*  1580 */    29,   30,   31,   32,   33,   34,   35,   36,   37,   38,
 /*  1590 */    39,   40,   41,   42,  103,  142,  146,  140,  176,  100,
 /*  1600 */   343,  135,  343,  343,  343,  232,  343,  343,  103,  142,
 /*  1610 */   146,  140,  176,  100,  132,  135,   99,  114,  118,  120,
 /*  1620 */   122,  124,  116,  102,  126,  128,  130,  343,  132,  188,
 /*  1630 */   343,  343,  343,  343,  287,  343,  343,   81,  343,  343,
 /*  1640 */   343,  343,  154,  343,  343,  343,  343,  343,  343,  148,
 /*  1650 */   149,  343,  150,  151,  152,  153,  154,  343,  343,  343,
 /*  1660 */   343,  343,  343,  148,  149,  343,  150,  151,  152,  153,
 /*  1670 */   103,  142,  146,  140,  176,  100,  343,  135,   99,  114,
 /*  1680 */   118,  120,  122,  124,  116,  102,  126,  128,  130,  343,
 /*  1690 */   132,  179,  343,  343,   99,  114,  118,  120,  122,  124,
 /*  1700 */   116,  102,  126,  128,  130,  343,  343,  343,  343,  172,
 /*  1710 */   134,  343,  343,  343,  343,  343,  343,  343,  154,  343,
 /*  1720 */   343,  343,  343,  343,  343,  148,  149,  343,  150,  151,
 /*  1730 */   152,  153,  103,  142,  146,  140,  176,  100,  343,  135,
 /*  1740 */    99,  114,  118,  120,  122,  124,  116,  102,  126,  128,
 /*  1750 */   130,  155,  132,  199,  579,  343,  343,  174,  173,  236,
 /*  1760 */   167,  168,  170,  169,  343,  343,  103,  142,  146,  140,
 /*  1770 */   176,  100,  343,  135,  343,  343,  343,  343,  343,  343,
 /*  1780 */   154,  343,  343,  343,  343,  343,  132,  148,  149,  343,
 /*  1790 */   150,  151,  152,  153,  343,  343,  343,  335,  343,  343,
 /*  1800 */   103,  142,  146,  140,  176,  100,  343,  135,  343,  343,
 /*  1810 */   343,  343,  343,  324,  154,  206,   24,  343,  343,  343,
 /*  1820 */   132,  148,  149,  343,  150,  151,  152,  153,  338,  339,
 /*  1830 */   340,  341,  342,  343,  103,  142,  146,  140,  176,  100,
 /*  1840 */   343,  135,  343,  343,  343,  343,  343,  343,  154,  215,
 /*  1850 */   343,  343,  343,  343,  132,  148,  149,  343,  150,  151,
 /*  1860 */   152,  153,  343,  103,  142,  146,  140,  176,  100,  343,
 /*  1870 */   135,  343,  343,  343,  299,  343,  343,  343,  343,  343,
 /*  1880 */   343,  343,  154,  132,  343,  343,  343,  343,  343,  148,
 /*  1890 */   149,  343,  150,  151,  152,  153,   99,  114,  118,  120,
 /*  1900 */   122,  124,  116,  102,  126,  128,  130,  343,  343,  343,
 /*  1910 */   343,  154,  343,   90,  343,  343,  343,  289,  148,  149,
 /*  1920 */   343,  150,  151,  152,  153,   99,  114,  118,  120,  122,
 /*  1930 */   124,  116,  102,  126,  128,  130,  343,  343,  343,   91,
 /*  1940 */    92,   93,   94,   95,  201,  214,  296,  222,  223,   99,
 /*  1950 */   114,  118,  120,  122,  124,  116,  102,  126,  128,  130,
 /*  1960 */   343,  343,  343,  343,  343,  343,  343,  343,  343,  343,
 /*  1970 */   301,  343,  343,   99,  114,  118,  120,  122,  124,  116,
 /*  1980 */   102,  126,  128,  130,  343,  343,  343,  343,  343,  343,
 /*  1990 */   343,  343,  343,  343,  661,  343,  343,   99,  114,  118,
 /*  2000 */   120,  122,  124,  116,  102,  126,  128,  130,  335,  343,
 /*  2010 */   343,  343,  343,  343,  343,  343,  343,  343,  343,  343,
 /*  2020 */   343,  343,  343,  343,  324,  343,  343,  343,  343,  335,
 /*  2030 */   343,  343,  343,  343,  343,  343,  343,  343,   23,  338,
 /*  2040 */   339,  340,  341,  342,  343,  324,  343,  343,  343,  343,
 /*  2050 */   335,  343,  343,  343,  343,  343,  343,  343,  343,  356,
 /*  2060 */   338,  339,  340,  341,  342,  343,  324,  343,  343,  343,
 /*  2070 */   343,  343,  343,  343,  343,  343,  343,  343,  343,  343,
 /*  2080 */   343,  338,  339,  340,  341,  342,  343,
};
static YYCODETYPE yy_lookahead[] = {
 /*     0 */    20,   85,   86,   87,   24,   89,   90,   27,   76,   77,
 /*    10 */    78,   79,   80,   81,   82,   35,   36,   37,   38,    0,
 /*    20 */    40,   41,   42,   43,   91,   45,   93,   94,   95,   49,
 /*    30 */    50,   51,  112,  113,   54,   24,   20,   88,   27,   28,
 /*    40 */    24,   30,   31,   27,   20,   65,   66,   67,   60,   61,
 /*    50 */    70,   35,   36,   37,   38,    1,   40,   41,   42,   43,
 /*    60 */    91,   45,   93,   94,   95,   49,   50,   51,   20,    1,
 /*    70 */    54,   24,   20,   24,   27,   28,   24,   30,   31,   27,
 /*    80 */   131,   65,   66,   67,  124,  125,   70,   35,   36,   37,
 /*    90 */    38,  114,   40,   41,   42,   43,   23,   45,   25,  122,
 /*   100 */   123,   49,   50,   51,   90,  128,   54,   24,   20,   20,
 /*   110 */    27,   28,   24,   30,   31,   27,   62,   65,   66,   67,
 /*   120 */    52,   32,   70,   35,   36,   37,   38,  114,   40,   41,
 /*   130 */    42,   43,  114,   45,   21,  122,  123,   49,   50,   51,
 /*   140 */   122,  128,   54,   24,   20,   20,   27,   28,   24,   30,
 /*   150 */    31,   27,   32,   65,   66,   67,  115,  116,   70,   35,
 /*   160 */    36,   37,   38,   33,   40,   41,   42,   43,   91,   45,
 /*   170 */    93,   94,   95,   49,   50,   51,   20,    1,   54,   24,
 /*   180 */    20,   23,   27,   28,   24,   30,   31,   27,   32,   65,
 /*   190 */    66,   67,   21,    1,   70,   35,   36,   37,   38,   23,
 /*   200 */    40,   41,   42,   43,  114,   45,  124,  125,  118,   49,
 /*   210 */    50,   51,  122,   21,   54,   24,   20,   44,   27,   28,
 /*   220 */    24,   30,   31,   27,   21,   65,   66,   67,   23,    1,
 /*   230 */    70,   35,   36,   37,   38,  114,   40,   41,   42,   43,
 /*   240 */   114,   45,   23,  122,  118,   49,   50,   51,  122,   21,
 /*   250 */    54,   24,   20,   88,   27,   28,   24,   30,   31,   27,
 /*   260 */   113,   65,   66,   67,  124,  125,   70,   35,   36,   37,
 /*   270 */    38,    1,   40,   41,   42,   43,  114,   45,   93,   94,
 /*   280 */    95,   49,   50,   51,  122,  123,   54,    1,   20,   47,
 /*   290 */   128,   21,   24,   19,    1,   27,   22,   65,   66,   67,
 /*   300 */    26,   27,   70,   35,   36,   37,   38,   21,   40,   41,
 /*   310 */    42,   43,  114,   45,   21,   23,  118,   49,   50,   51,
 /*   320 */   122,  117,   54,   24,   20,   23,   27,   28,   24,   30,
 /*   330 */     1,   27,    1,   65,   66,   67,  124,  125,   70,   35,
 /*   340 */    36,   37,   38,   20,   40,   41,   42,   43,  114,   45,
 /*   350 */    20,  114,   21,   49,   50,   51,  122,  123,   54,  122,
 /*   360 */    20,  116,  128,   34,   24,   33,    1,   27,   20,   65,
 /*   370 */    66,   67,  124,  125,   70,   35,   36,   37,   38,  126,
 /*   380 */    40,   41,   42,   43,   86,   45,   21,   89,   90,   49,
 /*   390 */    50,   51,   60,   61,   54,   24,   20,   20,   27,   28,
 /*   400 */    24,   30,   28,   27,   30,   65,   66,   67,   61,   32,
 /*   410 */    70,   35,   36,   37,   38,  114,   40,   41,   42,   43,
 /*   420 */   114,   45,   62,  122,  118,   49,   50,   51,  122,  128,
 /*   430 */    54,   24,   20,   20,   27,   28,   24,   30,   28,   27,
 /*   440 */    30,   65,   66,   67,   62,   32,   70,   35,   36,   37,
 /*   450 */    38,  114,   40,   41,   42,   43,  114,   45,   62,  122,
 /*   460 */   118,   49,   50,   51,  122,   20,   54,   24,   20,   33,
 /*   470 */    27,   28,   24,   30,   23,   27,   25,   65,   66,   67,
 /*   480 */    20,    1,   70,   35,   36,   37,   38,  114,   40,   41,
 /*   490 */    42,   43,  114,   45,  126,  122,  118,   49,   50,   51,
 /*   500 */   122,   47,   54,   24,   20,   47,   27,   28,   24,   30,
 /*   510 */    47,   27,   47,   65,   66,   67,   20,   62,   70,   35,
 /*   520 */    36,   37,   38,   33,   40,   41,   42,   43,   28,   45,
 /*   530 */    30,   31,   62,   49,   50,   51,   62,   20,   54,   24,
 /*   540 */    20,  117,   27,   28,   24,   30,    1,   27,  117,   65,
 /*   550 */    66,   67,  117,   62,   70,   35,   36,   37,   38,  117,
 /*   560 */    40,   41,   42,   43,  111,   45,   62,  114,   21,   49,
 /*   570 */    50,   51,   20,  117,   54,  122,   20,   20,  111,   34,
 /*   580 */    24,  114,    1,   27,   21,   65,   66,   67,   62,  122,
 /*   590 */    70,   35,   36,   37,   38,   21,   40,   41,   42,   43,
 /*   600 */   111,   45,   20,  114,   62,   49,   50,   51,   21,   21,
 /*   610 */    54,  122,   20,   62,   24,   34,   24,  114,   20,   27,
 /*   620 */    21,   65,   66,   67,  121,  122,   70,   35,   36,   37,
 /*   630 */    38,   21,   40,   41,   42,   43,   46,   45,   48,  114,
 /*   640 */    33,   49,   50,   51,  119,  114,   54,  122,   20,   20,
 /*   650 */    68,   69,   24,  122,  125,   27,   29,   65,   66,   67,
 /*   660 */    88,   24,   70,   35,   36,   37,   38,   20,   40,   41,
 /*   670 */    42,   43,   28,   45,   30,   31,   60,   49,   50,   51,
 /*   680 */    20,   47,   54,   46,   20,   48,   86,   87,   24,  114,
 /*   690 */    90,   27,   52,   65,   66,   67,   44,  122,   70,   35,
 /*   700 */    36,   37,   38,   23,   40,   41,   42,   43,   29,   45,
 /*   710 */    88,  114,   20,   49,   50,   51,  119,  114,   54,  122,
 /*   720 */    20,   88,    1,   17,   24,  122,   20,   27,   29,   65,
 /*   730 */    66,   67,   20,   29,   70,   35,   36,   37,   38,  114,
 /*   740 */    40,   41,   42,   43,  114,   45,   88,  122,   20,   49,
 /*   750 */    50,   51,  122,  114,   54,   34,   20,    5,   23,   20,
 /*   760 */    24,  122,    1,   27,   20,   65,   66,   67,   20,   20,
 /*   770 */    70,   35,   36,   37,   38,   32,   40,   41,   42,   43,
 /*   780 */   114,   45,   20,  114,   20,   49,   50,   51,  122,  114,
 /*   790 */    54,  122,   20,   20,   20,   34,   24,  122,    1,   27,
 /*   800 */    33,   65,   66,   67,   23,   23,   70,   35,   36,   37,
 /*   810 */    38,   32,   40,   41,   42,   43,  114,   45,   20,  114,
 /*   820 */    33,   49,   50,   51,  122,   34,   54,  122,   20,   23,
 /*   830 */    20,   34,   24,  114,   32,   27,   33,   65,   66,   67,
 /*   840 */    23,  122,   70,   35,   36,   37,   38,   34,   40,   41,
 /*   850 */    42,   43,  114,   45,   20,  114,   29,   49,   50,   51,
 /*   860 */   122,   21,   54,  122,   20,   88,   20,   29,   24,  114,
 /*   870 */    21,   27,   88,   65,   66,   67,   88,  122,   70,   35,
 /*   880 */    36,   37,   38,   21,   40,   41,   42,   43,   88,   45,
 /*   890 */    21,   21,   88,   49,   50,   51,   21,   21,   54,   29,
 /*   900 */    20,   23,   88,   21,   24,   29,   20,   27,   24,   65,
 /*   910 */    66,   67,   22,   20,   70,   35,   36,   37,   38,   21,
 /*   920 */    40,   41,   42,   43,   24,   45,   20,   29,   23,   49,
 /*   930 */    50,   51,   24,   21,   54,  132,   20,  132,  132,  132,
 /*   940 */    24,   29,  132,   27,  132,   65,   66,   67,   29,  132,
 /*   950 */    70,   35,   36,   37,   38,  132,   40,   41,   42,   43,
 /*   960 */   132,   45,  132,  132,  132,   49,   50,   51,  132,   88,
 /*   970 */    54,  132,   20,   88,   55,   56,   57,   58,   59,   27,
 /*   980 */    61,   65,   66,   67,  132,  132,   70,   35,   36,   37,
 /*   990 */    38,  132,   40,   41,   42,   43,  132,   45,   10,   11,
 /*  1000 */    12,   49,   50,   51,  132,  132,   54,  132,  127,  132,
 /*  1010 */   129,  130,  131,   82,  129,  130,  131,   65,   66,   67,
 /*  1020 */   132,  132,   70,   92,  132,  132,  132,   96,   97,   98,
 /*  1030 */    99,  100,  101,  102,  103,  104,  105,  106,  107,  108,
 /*  1040 */   109,  110,   82,   68,   69,  132,   71,   72,   73,   74,
 /*  1050 */   132,  132,   92,  122,   20,  132,   96,   97,   98,   99,
 /*  1060 */   100,  101,  102,  103,  104,  105,  106,  107,  108,  109,
 /*  1070 */   110,   82,  132,   83,   84,   85,   86,   87,  132,   89,
 /*  1080 */    90,   92,  122,  132,  132,   96,   97,   98,   99,  100,
 /*  1090 */   101,  102,  103,  104,  105,  106,  107,  108,  109,  110,
 /*  1100 */    82,  132,   68,   69,   29,   71,   72,   73,   74,  132,
 /*  1110 */    92,  122,  132,  132,   96,   97,   98,   99,  100,  101,
 /*  1120 */   102,  103,  104,  105,  106,  107,  108,  109,  110,  132,
 /*  1130 */    55,   56,   57,   58,   59,   82,   79,   80,   81,   82,
 /*  1140 */   122,  132,  132,  132,  132,   92,   20,  132,  132,   96,
 /*  1150 */    97,   98,   99,  100,  101,  102,  103,  104,  105,  106,
 /*  1160 */   107,  108,  109,  110,   82,  132,   83,   84,   85,   86,
 /*  1170 */    87,  132,   89,   90,   92,  122,   20,  132,   96,   97,
 /*  1180 */    98,   99,  100,  101,  102,  103,  104,  105,  106,  107,
 /*  1190 */   108,  109,  110,   82,   68,   69,  132,   71,   72,   73,
 /*  1200 */    74,  132,  132,   92,  122,  132,  132,   96,   97,   98,
 /*  1210 */    99,  100,  101,  102,  103,  104,  105,  106,  107,  108,
 /*  1220 */   109,  110,  132,  114,   68,   69,  132,  132,   82,  120,
 /*  1230 */   121,  122,  132,  122,   84,  132,   86,   87,   92,   89,
 /*  1240 */    90,  132,   96,   97,   98,   99,  100,  101,  102,  103,
 /*  1250 */   104,  105,  106,  107,  108,  109,  110,   82,  132,   83,
 /*  1260 */    84,   85,   86,   87,  132,   89,   90,   92,  122,  132,
 /*  1270 */    20,   96,   97,   98,   99,  100,  101,  102,  103,  104,
 /*  1280 */   105,  106,  107,  108,  109,  110,   82,  132,   83,   84,
 /*  1290 */    85,   86,   87,  132,   89,   90,   92,  122,    0,  132,
 /*  1300 */    96,   97,   98,   99,  100,  101,  102,  103,  104,  105,
 /*  1310 */   106,  107,  108,  109,  110,  132,  132,   19,   68,   69,
 /*  1320 */    22,   82,  132,  132,   26,   27,  122,  132,  132,  132,
 /*  1330 */   132,   92,  132,  132,  132,   96,   97,   98,   99,  100,
 /*  1340 */   101,  102,  103,  104,  105,  106,  107,  108,  109,  110,
 /*  1350 */    82,  132,  132,  132,  132,  132,  132,  132,  132,  132,
 /*  1360 */    92,  122,  132,  132,   96,   97,   98,   99,  100,  101,
 /*  1370 */   102,  103,  104,  105,  106,  107,  108,  109,  110,   82,
 /*  1380 */   132,  132,  132,  132,  132,  132,  132,  132,  132,   92,
 /*  1390 */   122,  132,  132,   96,   97,   98,   99,  100,  101,  102,
 /*  1400 */   103,  104,  105,  106,  107,  108,  109,  110,  132,  132,
 /*  1410 */   132,  132,  132,  132,   82,  132,  132,  132,  132,  122,
 /*  1420 */   132,  132,  132,  132,   92,  132,  132,  132,   96,   97,
 /*  1430 */    98,   99,  100,  101,  102,  103,  104,  105,  106,  107,
 /*  1440 */   108,  109,  110,   82,  132,  132,  132,  132,  132,  132,
 /*  1450 */   132,  132,  132,   92,  122,  132,  132,   96,   97,   98,
 /*  1460 */    99,  100,  101,  102,  103,  104,  105,  106,  107,  108,
 /*  1470 */   109,  110,   82,  132,  132,  132,  132,  132,  132,  132,
 /*  1480 */   132,  132,   92,  122,  132,  132,   96,   97,   98,   99,
 /*  1490 */   100,  101,  102,  103,  104,  105,  106,  107,  108,  109,
 /*  1500 */   110,  132,  132,  132,  132,  132,  132,   82,  132,  132,
 /*  1510 */   132,  132,  122,  132,  132,  132,  132,   92,  132,  132,
 /*  1520 */   132,   96,   97,   98,   99,  100,  101,  102,  103,  104,
 /*  1530 */   105,  106,  107,  108,  109,  110,   82,  132,  132,  132,
 /*  1540 */   132,  132,  132,  132,  132,  132,   92,  122,  132,  132,
 /*  1550 */    96,   97,   98,   99,  100,  101,  102,  103,  104,  105,
 /*  1560 */   106,  107,  108,  109,  110,   82,  132,  132,  132,  132,
 /*  1570 */   132,  132,  132,  132,  132,  132,  122,  132,  132,   96,
 /*  1580 */    97,   98,   99,  100,  101,  102,  103,  104,  105,  106,
 /*  1590 */   107,  108,  109,  110,   13,   14,   15,   16,   17,   18,
 /*  1600 */   132,   20,  132,  132,  132,  122,  132,  132,   13,   14,
 /*  1610 */    15,   16,   17,   18,   33,   20,    2,    3,    4,    5,
 /*  1620 */     6,    7,    8,    9,   10,   11,   12,  132,   33,   34,
 /*  1630 */   132,  132,  132,  132,   53,  132,  132,   23,  132,  132,
 /*  1640 */   132,  132,   61,  132,  132,  132,  132,  132,  132,   68,
 /*  1650 */    69,  132,   71,   72,   73,   74,   61,  132,  132,  132,
 /*  1660 */   132,  132,  132,   68,   69,  132,   71,   72,   73,   74,
 /*  1670 */    13,   14,   15,   16,   17,   18,  132,   20,    2,    3,
 /*  1680 */     4,    5,    6,    7,    8,    9,   10,   11,   12,  132,
 /*  1690 */    33,   34,  132,  132,    2,    3,    4,    5,    6,    7,
 /*  1700 */     8,    9,   10,   11,   12,  132,  132,  132,  132,   20,
 /*  1710 */    34,  132,  132,  132,  132,  132,  132,  132,   61,  132,
 /*  1720 */   132,  132,  132,  132,  132,   68,   69,  132,   71,   72,
 /*  1730 */    73,   74,   13,   14,   15,   16,   17,   18,  132,   20,
 /*  1740 */     2,    3,    4,    5,    6,    7,    8,    9,   10,   11,
 /*  1750 */    12,   62,   33,   34,   62,  132,  132,   68,   69,   21,
 /*  1760 */    71,   72,   73,   74,  132,  132,   13,   14,   15,   16,
 /*  1770 */    17,   18,  132,   20,  132,  132,  132,  132,  132,  132,
 /*  1780 */    61,  132,  132,  132,  132,  132,   33,   68,   69,  132,
 /*  1790 */    71,   72,   73,   74,  132,  132,  132,    4,  132,  132,
 /*  1800 */    13,   14,   15,   16,   17,   18,  132,   20,  132,  132,
 /*  1810 */   132,  132,  132,   20,   61,   62,   23,  132,  132,  132,
 /*  1820 */    33,   68,   69,  132,   71,   72,   73,   74,   35,   36,
 /*  1830 */    37,   38,   39,   40,   13,   14,   15,   16,   17,   18,
 /*  1840 */   132,   20,  132,  132,  132,  132,  132,  132,   61,   62,
 /*  1850 */   132,  132,  132,  132,   33,   68,   69,  132,   71,   72,
 /*  1860 */    73,   74,  132,   13,   14,   15,   16,   17,   18,  132,
 /*  1870 */    20,  132,  132,  132,   53,  132,  132,  132,  132,  132,
 /*  1880 */   132,  132,   61,   33,  132,  132,  132,  132,  132,   68,
 /*  1890 */    69,  132,   71,   72,   73,   74,    2,    3,    4,    5,
 /*  1900 */     6,    7,    8,    9,   10,   11,   12,  132,  132,  132,
 /*  1910 */   132,   61,  132,   29,  132,  132,  132,   23,   68,   69,
 /*  1920 */   132,   71,   72,   73,   74,    2,    3,    4,    5,    6,
 /*  1930 */     7,    8,    9,   10,   11,   12,  132,  132,  132,   55,
 /*  1940 */    56,   57,   58,   59,   60,   61,   23,   63,   64,    2,
 /*  1950 */     3,    4,    5,    6,    7,    8,    9,   10,   11,   12,
 /*  1960 */   132,  132,  132,  132,  132,  132,  132,  132,  132,  132,
 /*  1970 */    23,  132,  132,    2,    3,    4,    5,    6,    7,    8,
 /*  1980 */     9,   10,   11,   12,  132,  132,  132,  132,  132,  132,
 /*  1990 */   132,  132,  132,  132,   23,  132,  132,    2,    3,    4,
 /*  2000 */     5,    6,    7,    8,    9,   10,   11,   12,    4,  132,
 /*  2010 */   132,  132,  132,  132,  132,  132,  132,  132,  132,  132,
 /*  2020 */   132,  132,  132,  132,   20,  132,  132,  132,  132,    4,
 /*  2030 */   132,  132,  132,  132,  132,  132,  132,  132,   34,   35,
 /*  2040 */    36,   37,   38,   39,   40,   20,  132,  132,  132,  132,
 /*  2050 */     4,  132,  132,  132,  132,  132,  132,  132,  132,   34,
 /*  2060 */    35,   36,   37,   38,   39,   40,   20,  132,  132,  132,
 /*  2070 */   132,  132,  132,  132,  132,  132,  132,  132,  132,  132,
 /*  2080 */   132,   35,   36,   37,   38,   39,   40,
};
#define YY_SHIFT_USE_DFLT (-21)
static short yy_shift_ofst[] = {
 /*     0 */   274,   19, 1298,  -21,  -21,  -21,  -21,   24,  113,  -21,
 /*    10 */    48,   73,   11,   49,  -21,  -21,   47,  299,  -21,  120,
 /*    20 */   125,  130, 2004, 1793,  -20,  -21,   16,  -21,  -21,  -21,
 /*    30 */   -21,  -21,  -21,  -21,  -21,  -21,  -21,  -21,  -21,  -21,
 /*    40 */   -21,  -21,  -21,  -21,  171,  -21,  203,  -21, 1850,  158,
 /*    50 */    52,   88,  173,  205,  124,  160,  -21, 1850,  219,  590,
 /*    60 */   -21,  637,  -21,  -21,  975,  242,  952,  952,  292,  196,
 /*    70 */   232,  -21, 1850,  302,  268,  304,  -21,  323,   68, 1581,
 /*    80 */  1614,  952,  340,  -21,  330,  192,  -21,  330,  -21, 1884,
 /*    90 */   -21,  -21,  -21,  -21,  -21,  -21, 1850,  -21, 1995, 1850,
 /*   100 */  1850,  -21, 1850,  348,  -21,  347,  582,  360,  -21,  382,
 /*   110 */   -21,  396,  -21,  988, 1850,  988, 1850,  988, 1850,  988,
 /*   120 */  1850,  988, 1850,  988, 1850,  988, 1850,  -21, 1850,  -21,
 /*   130 */  1850,  -21, 1850, 1676,  -21,  332,  445,  436, 1595, 1995,
 /*   140 */  1850,  -21,  460,  480,  348,  -21, 1850,  -21,  -21,  -21,
 /*   150 */   -21,  -21,  -21,  -21, 1689,  -21,   54,  -21, 1034,  -21,
 /*   160 */   454, 1126,  -21,  -21,  -21,  -21,  -21,  -21,  -21,  -21,
 /*   170 */   -21,  -21,  458,  463,  465,  -21,  496,  490, 1657,  -21,
 /*   180 */   329,  -21, 1850,  -21,  -21,  -21,  545,  -21,  -21, 1156,
 /*   190 */   455,  -21,  470,  -21,  474,  -21, 1719,  581,  -21,  -21,
 /*   200 */   988,  517,  919, 1850,  -21, 1753, 1075, 1850,  -21,  491,
 /*   210 */  1075, 1850,  -21, 1692, 1787, 1075, 1850,  -21,  504, 1075,
 /*   220 */  1850,  -21,  -21,  -21,  -21, 1850,  228,  -21, 1850,  -21,
 /*   230 */  1995,  -21,  547,  -21, 1850, 1738,  -21,  552,  -12,  557,
 /*   240 */   563,  -21, 1250,  526,  574,  -21,  542,  587,  -21,  551,
 /*   250 */   588,  -21,  706,  599,  -21,  598,  607, 1850,  721,  610,
 /*   260 */   -21,  629,  270,  -21,  629,  -21,  627,  975,  -21,  -21,
 /*   270 */   629,  286,  -21,  629,  293,  -21,  629,  331,  -21,  629,
 /*   280 */   365,  -21,  616,  647,  436,  -21,  -21, 1850, 1894,  952,
 /*   290 */   376,  -21,  660,  640, 1821, 1923,  952,  412,  -21, 1850,
 /*   300 */  1947,  952,  448,  -21,  -21, 1971,  -21,  634,  952,  952,
 /*   310 */   -21,  -21,  652,  680,  484,  520,  -21,  -21,  176,  952,
 /*   320 */   556,  -21, 2046,  -21,  679,  975,  -21,  692,  699,  975,
 /*   330 */   -21,  712,  704,  975,  -21,  728,  752,  739,  744,  748,
 /*   340 */   749,  762,  764,  773,  -21,  761,  735,  592,  -21,  628,
 /*   350 */   -21,  374,  743,  774,  767, 2025,  781,  664,  -21,  700,
 /*   360 */   -21,  797,  782,  736,  -21,  772,  -21,  779,  798,  787,
 /*   370 */   791,  806,  808,  -21,  844,  -21,  802,  810,  803,  813,
 /*   380 */   817,  880,  -21,  916,  -21,   83,  371,  -21,  -21,  500,
 /*   390 */   834,  827,  975,  840,  -21,  846,  838,  975,  849,  -21,
 /*   400 */   -21,  644,   89,  870,  -21,  975,  862,  -21,  156,  876,
 /*   410 */   -21,  975,  869,  -21,  377,  898,  -21,  975,  875,  -21,
 /*   420 */   413,  912,  -21,  975,  882,  -21,  -21,  119,  407,  443,
 /*   430 */   410,  479,  -21,  515,  886,  878,  155,  884,  -21,  -21,
 /*   440 */   890,  893,  451,  191,  900,  -21,  -21,  906,  905,  227,
 /*   450 */   908,  -21,  -21,  -21,
};
#define YY_REDUCE_USE_DFLT (-85)
static short yy_reduce_ofst[] = {
 /*     0 */   -68,  -85, 1057,  -85,  -85,  -85,  -85,  -85,  -85,  -85,
 /*    10 */   -85,  -85,  990,  -85,  -85,  -85,  -84,   14,  -85,  -85,
 /*    20 */   -85,  -85,  -67,  -31,  931,  -85, 1483,  -85,  -85,  -85,
 /*    30 */   -85,  -85,  -85,  -85,  -85,  -85,  -85,  -85,  -85,  -85,
 /*    40 */   -85,  -85,  -85,  -85,  -85,  -85,  -85,  -85,  453,  -85,
 /*    50 */   960, 1483,  -85,  -85,  989, 1483,  -85,  467,  -85,  -80,
 /*    60 */   -85,  147,  -85,  -85,  165,  -85, 1018, 1483,  -85, 1053,
 /*    70 */  1483,  -85,  489,  -85, 1082, 1483,  -85,  -85,  -85,   18,
 /*    80 */   -85, 1111, 1483,  -85,   41,  -85,  -85,  245,  -85,  204,
 /*    90 */   -85,  -85,  -85,  -85,  -85,  -85,   90,  -85,  -85,  121,
 /*   100 */   237,  -85,  531,  253,  -85,  -85,  -85,  -85,  -85,  -85,
 /*   110 */   -85,  -85,  -85,  -85,  575,  -85,  603,  -85,  625,  -85,
 /*   120 */   630,  -85,  639,  -85,  666,  -85,  669,  -85,  675,  -85,
 /*   130 */   702,  -85,  705,  -85,  -85,  -85,  -85,  -85,  -23,  -85,
 /*   140 */   719,  -85,  -85,  -85,  368,  -85,  738,  -85,  -85,  -85,
 /*   150 */   -85,  -85,  -85,  -85,  881,  -85,  -85,  -85,  885,  -85,
 /*   160 */   -85,  -51,  -85,  -85,  -85,  -85,  -85,  -85,  -85,  -85,
 /*   170 */   -85,  -85,  -85,  -85,  -85,  -85,  -85,  -85,   13,  -85,
 /*   180 */   -85,  -85,  301,  -85,  -85,  -85,  -85,  -85,  -85,  -85,
 /*   190 */   -85,  -85,  -85,  -85,  -85,  -85,  162,  -85,  -85,  -85,
 /*   200 */   -85,  -85,  424,  126,  -85,  525,  431,  198,  -85,  -85,
 /*   210 */   435,  306,  -85,  -85,  597,  442,  342,  -85,  -85,  456,
 /*   220 */   378,  -85,  -85,  -85,  -85, 1109,  -85,  -85,  503,  -85,
 /*   230 */   -85,  -85,  -85,  -85,  741,  -85,  -85,  -85,  -85,  -85,
 /*   240 */   -85,  -85,  -85,  -85,  -85,  -85,  -85,  -85,  -85,  -85,
 /*   250 */   -85,  -85,  -85,  -85,  -85,  -85,  -85,  234,  -85,  -85,
 /*   260 */   -85,  -40,  -85,  -85,  529,  -85,  -85,  572,  -85,  -85,
 /*   270 */    82,  -85,  -85,  140,  -85,  -85,  212,  -85,  -85,  248,
 /*   280 */   -85,  -85,  -85,  -85,  -85,  -85,  -85,  337,  -85, 1146,
 /*   290 */  1483,  -85,  -85,  -85,  373,  -85, 1175, 1483,  -85,  755,
 /*   300 */   -85, 1204, 1483,  -85,  -85,  -85,  -85,  -85, 1239, 1483,
 /*   310 */   -85,  -85,  -85,  -85, 1268, 1483,  -85,  -85,  -85, 1297,
 /*   320 */  1483,  -85,  185,  -85,  -85,  622,  -85,  -85,  -85,  633,
 /*   330 */   -85,  -85,  -85,  658,  -85,  -85,  -85,  -85,  -85,  -85,
 /*   340 */   -85,  -85,  -85,  -85,  -85,  -85,  -85, 1332,  -85, 1483,
 /*   350 */   -85,  -85,  -85,  -85,  -85,   77,  -85, 1361,  -85, 1483,
 /*   360 */   -85,  -85,  -85, 1390,  -85, 1483,  -85,  -85,  -85,  -85,
 /*   370 */   -85,  -85, 1425,  -85, 1483,  -85,  -85,  -85,  -85,  -85,
 /*   380 */   -85, 1454,  -85, 1483,  -85,  298,   14,  -85,  -85,  -85,
 /*   390 */   -85,  -85,  777,  -85,  -85,  -85,  -85,  784,  -85,  -85,
 /*   400 */   -85,  -85,  -85,  -85,  -85,  788,  -85,  -85,  -85,  -85,
 /*   410 */   -85,  800,  -85,  -85,  -85,  -85,  -85,  804,  -85,  -85,
 /*   420 */   -85,  -85,  -85,  814,  -85,  -85,  -85, 1150,  600,   14,
 /*   430 */   -85,   14,  -85,   14,  -85,  -85, 1083,  -85,  -85,  -85,
 /*   440 */   -85,  -85,  -85, 1176,  -85,  -85,  -85,  -85,  -85, 1205,
 /*   450 */   -85,  -85,  -85,  -85,
};
static YYACTIONTYPE yy_default[] = {
 /*     0 */   663,  663,  663,  456,  458,  459,  460,  663,  663,  461,
 /*    10 */   663,  663,  663,  663,  462,  466,  663,  663,  491,  663,
 /*    20 */   663,  663,  663,  663,  663,  495,  663,  499,  523,  525,
 /*    30 */   526,  527,  528,  529,  530,  531,  532,  533,  534,  535,
 /*    40 */   536,  537,  538,  539,  663,  540,  663,  541,  663,  663,
 /*    50 */   663,  663,  544,  663,  663,  663,  545,  663,  663,  663,
 /*    60 */   548,  663,  549,  550,  663,  663,  663,  552,  663,  663,
 /*    70 */   663,  555,  663,  663,  663,  663,  557,  663,  663,  663,
 /*    80 */   663,  663,  663,  558,  663,  663,  562,  663,  563,  663,
 /*    90 */   565,  566,  567,  568,  569,  570,  663,  571,  601,  663,
 /*   100 */   663,  602,  663,  663,  603,  663,  663,  663,  658,  663,
 /*   110 */   659,  663,  660,  605,  663,  606,  663,  607,  663,  608,
 /*   120 */   663,  609,  663,  610,  663,  611,  663,  617,  663,  618,
 /*   130 */   663,  619,  663,  663,  612,  623,  663,  613,  663,  642,
 /*   140 */   663,  620,  663,  663,  663,  621,  663,  622,  624,  625,
 /*   150 */   626,  627,  628,  629,  663,  630,  663,  631,  663,  643,
 /*   160 */   663,  663,  645,  650,  651,  652,  653,  654,  655,  656,
 /*   170 */   657,  646,  651,  653,  652,  644,  663,  632,  663,  633,
 /*   180 */   663,  634,  663,  637,  640,  641,  663,  638,  639,  663,
 /*   190 */   663,  614,  663,  615,  663,  616,  663,  663,  635,  636,
 /*   200 */   604,  663,  663,  663,  572,  663,  663,  663,  573,  663,
 /*   210 */   663,  663,  574,  663,  663,  663,  663,  575,  663,  663,
 /*   220 */   663,  576,  577,  578,  564,  663,  663,  580,  663,  581,
 /*   230 */   583,  582,  663,  584,  663,  663,  585,  663,  663,  663,
 /*   240 */   663,  586,  663,  663,  663,  587,  663,  663,  588,  663,
 /*   250 */   663,  589,  663,  663,  590,  663,  663,  663,  663,  663,
 /*   260 */   591,  663,  663,  592,  663,  597,  599,  663,  600,  598,
 /*   270 */   663,  663,  593,  663,  663,  594,  663,  663,  595,  663,
 /*   280 */   663,  596,  663,  663,  663,  662,  524,  663,  663,  663,
 /*   290 */   663,  559,  663,  663,  663,  663,  663,  663,  560,  663,
 /*   300 */   663,  663,  663,  561,  556,  663,  554,  663,  663,  553,
 /*   310 */   551,  546,  542,  663,  663,  663,  547,  543,  663,  663,
 /*   320 */   663,  500,  663,  508,  510,  663,  513,  663,  511,  663,
 /*   330 */   514,  663,  512,  663,  515,  663,  663,  663,  663,  663,
 /*   340 */   663,  663,  663,  663,  509,  663,  663,  663,  496,  663,
 /*   350 */   503,  663,  663,  663,  663,  663,  663,  663,  497,  663,
 /*   360 */   501,  663,  663,  663,  498,  663,  502,  663,  663,  663,
 /*   370 */   663,  663,  663,  505,  663,  507,  663,  663,  663,  663,
 /*   380 */   663,  663,  504,  663,  506,  663,  663,  489,  492,  663,
 /*   390 */   663,  663,  663,  663,  493,  663,  663,  663,  663,  494,
 /*   400 */   479,  663,  663,  663,  481,  663,  663,  484,  663,  663,
 /*   410 */   486,  663,  663,  488,  663,  663,  482,  663,  663,  483,
 /*   420 */   663,  663,  485,  663,  663,  487,  490,  663,  663,  663,
 /*   430 */   663,  663,  480,  663,  663,  663,  663,  663,  463,  467,
 /*   440 */   663,  663,  663,  663,  663,  464,  468,  663,  663,  663,
 /*   450 */   663,  465,  469,  457,
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
  "$",             "COMMA",         "EQUALS",        "IDENTICAL",   
  "LESS",          "GREATER",       "LESSEQUAL",     "GREATEREQUAL",
  "NOTIDENTICAL",  "NOTEQUALS",     "ADD",           "SUB",         
  "CONCAT",        "ISSET",         "FETCH",         "TYPEOF",      
  "INSTANCEOF",    "NEW",           "NOT",           "NAMESPACE",   
  "IDENTIFIER",    "DOTCOMMA",      "CLASS",         "BRACKET_OPEN",
  "BRACKET_CLOSE",  "EXTENDS",       "ABSTRACT",      "COMMENT",     
  "PUBLIC",        "ASSIGN",        "PROTECTED",     "CONST",       
  "FUNCTION",      "PARENTHESES_OPEN",  "PARENTHESES_CLOSE",  "TYPE_INTEGER",
  "TYPE_DOUBLE",   "TYPE_BOOL",     "TYPE_STRING",   "TYPE_ARRAY",  
  "TYPE_VAR",      "BREAK",         "CONTINUE",      "IF",          
  "ELSE",          "SWITCH",        "CASE",          "COLON",       
  "DEFAULT",       "LOOP",          "WHILE",         "FOR",         
  "IN",            "REVERSE",       "LET",           "ADDASSIGN",   
  "SUBASSIGN",     "MULASSIGN",     "DIVASSIGN",     "CONCATASSIGN",
  "ARROW",         "SBRACKET_OPEN",  "SBRACKET_CLOSE",  "INCR",        
  "DECR",          "ECHO",          "RETURN",        "UNSET",       
  "INTEGER",       "STRING",        "THROW",         "DOUBLE",      
  "NULL",          "TRUE",          "FALSE",         "error",       
  "program",       "xx_language",   "xx_top_statement_list",  "xx_top_statement",
  "xx_namespace_def",  "xx_class_def",  "xx_comment",    "xx_class_definition",
  "xx_class_properties_definition",  "xx_class_consts_definition",  "xx_class_methods_definition",  "xx_class_property_definition",
  "xx_literal_expr",  "xx_class_const_definition",  "xx_class_method_definition",  "xx_parameter_list",
  "xx_statement_list",  "xx_parameter",  "xx_parameter_type",  "xx_parameter_cast",
  "xx_statement",  "xx_let_statement",  "xx_if_statement",  "xx_loop_statement",
  "xx_echo_statement",  "xx_return_statement",  "xx_mcall_statement",  "xx_unset_statement",
  "xx_throw_statement",  "xx_declare_statement",  "xx_break_statement",  "xx_continue_statement",
  "xx_while_statement",  "xx_switch_statement",  "xx_for_statement",  "xx_eval_expr",
  "xx_case_clauses",  "xx_case_clause",  "xx_common_expr",  "xx_let_assignments",
  "xx_let_assignment",  "xx_assignment_operator",  "xx_assign_expr",  "xx_index_expr",
  "xx_echo_expressions",  "xx_echo_expression",  "xx_mcall_expr",  "xx_call_parameters",
  "xx_declare_variable_list",  "xx_declare_variable",  "xx_isset_expr",  "xx_array_list",
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
 /* 166 */ "xx_common_expr ::= INSTANCEOF xx_common_expr",
 /* 167 */ "xx_common_expr ::= FETCH IDENTIFIER COMMA xx_isset_expr",
 /* 168 */ "xx_common_expr ::= TYPEOF xx_common_expr",
 /* 169 */ "xx_common_expr ::= IDENTIFIER",
 /* 170 */ "xx_common_expr ::= INTEGER",
 /* 171 */ "xx_common_expr ::= STRING",
 /* 172 */ "xx_common_expr ::= DOUBLE",
 /* 173 */ "xx_common_expr ::= NULL",
 /* 174 */ "xx_common_expr ::= TRUE",
 /* 175 */ "xx_common_expr ::= FALSE",
 /* 176 */ "xx_common_expr ::= SBRACKET_OPEN SBRACKET_CLOSE",
 /* 177 */ "xx_common_expr ::= SBRACKET_OPEN xx_array_list SBRACKET_CLOSE",
 /* 178 */ "xx_common_expr ::= NEW IDENTIFIER",
 /* 179 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 180 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 181 */ "xx_common_expr ::= IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 182 */ "xx_common_expr ::= IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 183 */ "xx_common_expr ::= xx_mcall_expr",
 /* 184 */ "xx_mcall_expr ::= IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 185 */ "xx_mcall_expr ::= IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 186 */ "xx_call_parameters ::= xx_call_parameters COMMA xx_call_parameter",
 /* 187 */ "xx_call_parameters ::= xx_call_parameter",
 /* 188 */ "xx_call_parameter ::= xx_common_expr",
 /* 189 */ "xx_array_list ::= xx_array_list COMMA xx_array_item",
 /* 190 */ "xx_array_list ::= xx_array_item",
 /* 191 */ "xx_array_item ::= xx_array_key COLON xx_array_value",
 /* 192 */ "xx_array_item ::= xx_array_value",
 /* 193 */ "xx_array_key ::= IDENTIFIER",
 /* 194 */ "xx_array_key ::= STRING",
 /* 195 */ "xx_array_key ::= INTEGER",
 /* 196 */ "xx_array_value ::= xx_literal_expr",
 /* 197 */ "xx_array_value ::= IDENTIFIER",
 /* 198 */ "xx_literal_expr ::= INTEGER",
 /* 199 */ "xx_literal_expr ::= STRING",
 /* 200 */ "xx_literal_expr ::= DOUBLE",
 /* 201 */ "xx_literal_expr ::= NULL",
 /* 202 */ "xx_literal_expr ::= FALSE",
 /* 203 */ "xx_literal_expr ::= TRUE",
 /* 204 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN IDENTIFIER SBRACKET_CLOSE",
 /* 205 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN INTEGER SBRACKET_CLOSE",
 /* 206 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN STRING SBRACKET_CLOSE",
 /* 207 */ "xx_eval_expr ::= xx_common_expr",
 /* 208 */ "xx_comment ::= COMMENT",
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
// 763 "parser.lemon"
{
	/*if ((yypminor->yy0)) {
		if ((yypminor->yy0)->free_flag) {
			efree((yypminor->yy0)->token);
		}
		efree((yypminor->yy0));
	}*/
}
// 1856 "parser.c"
      break;
    case 77:
// 776 "parser.lemon"
{ json_object_put((yypminor->yy247)); }
// 1861 "parser.c"
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
  { 76, 1 },
  { 77, 1 },
  { 78, 2 },
  { 78, 1 },
  { 79, 1 },
  { 79, 1 },
  { 79, 1 },
  { 80, 3 },
  { 81, 5 },
  { 81, 7 },
  { 81, 6 },
  { 81, 8 },
  { 81, 4 },
  { 81, 6 },
  { 81, 5 },
  { 81, 7 },
  { 83, 1 },
  { 83, 1 },
  { 83, 1 },
  { 83, 2 },
  { 83, 2 },
  { 83, 2 },
  { 83, 2 },
  { 83, 3 },
  { 83, 3 },
  { 84, 2 },
  { 84, 1 },
  { 87, 4 },
  { 87, 3 },
  { 87, 5 },
  { 87, 6 },
  { 87, 3 },
  { 87, 4 },
  { 87, 5 },
  { 87, 6 },
  { 85, 2 },
  { 85, 1 },
  { 86, 2 },
  { 86, 1 },
  { 89, 6 },
  { 89, 5 },
  { 90, 7 },
  { 90, 8 },
  { 90, 8 },
  { 90, 9 },
  { 90, 8 },
  { 90, 9 },
  { 90, 9 },
  { 90, 10 },
  { 90, 9 },
  { 90, 7 },
  { 90, 8 },
  { 90, 8 },
  { 90, 9 },
  { 91, 3 },
  { 91, 1 },
  { 93, 1 },
  { 93, 2 },
  { 93, 2 },
  { 93, 3 },
  { 93, 4 },
  { 93, 4 },
  { 95, 3 },
  { 94, 1 },
  { 94, 1 },
  { 94, 1 },
  { 94, 1 },
  { 94, 1 },
  { 94, 1 },
  { 92, 2 },
  { 92, 1 },
  { 96, 1 },
  { 96, 1 },
  { 96, 1 },
  { 96, 1 },
  { 96, 1 },
  { 96, 1 },
  { 96, 1 },
  { 96, 1 },
  { 96, 1 },
  { 96, 1 },
  { 96, 1 },
  { 96, 1 },
  { 96, 1 },
  { 96, 1 },
  { 96, 1 },
  { 106, 2 },
  { 107, 2 },
  { 98, 4 },
  { 98, 7 },
  { 98, 5 },
  { 98, 9 },
  { 98, 8 },
  { 98, 8 },
  { 109, 4 },
  { 109, 5 },
  { 112, 2 },
  { 112, 1 },
  { 113, 4 },
  { 113, 3 },
  { 99, 3 },
  { 99, 4 },
  { 108, 4 },
  { 108, 5 },
  { 110, 7 },
  { 110, 8 },
  { 110, 9 },
  { 110, 10 },
  { 97, 3 },
  { 115, 3 },
  { 115, 1 },
  { 117, 1 },
  { 117, 1 },
  { 117, 1 },
  { 117, 1 },
  { 117, 1 },
  { 117, 1 },
  { 116, 3 },
  { 116, 5 },
  { 116, 7 },
  { 116, 8 },
  { 116, 5 },
  { 116, 6 },
  { 116, 2 },
  { 116, 2 },
  { 119, 1 },
  { 100, 3 },
  { 120, 3 },
  { 120, 1 },
  { 121, 1 },
  { 102, 2 },
  { 101, 3 },
  { 103, 5 },
  { 103, 6 },
  { 103, 6 },
  { 103, 6 },
  { 104, 3 },
  { 104, 7 },
  { 105, 3 },
  { 105, 3 },
  { 105, 3 },
  { 105, 3 },
  { 105, 3 },
  { 124, 3 },
  { 124, 1 },
  { 125, 1 },
  { 125, 3 },
  { 118, 1 },
  { 114, 2 },
  { 114, 2 },
  { 114, 3 },
  { 114, 3 },
  { 114, 3 },
  { 114, 3 },
  { 114, 3 },
  { 114, 3 },
  { 114, 3 },
  { 114, 3 },
  { 114, 3 },
  { 114, 3 },
  { 114, 4 },
  { 114, 4 },
  { 114, 4 },
  { 114, 3 },
  { 114, 3 },
  { 114, 3 },
  { 114, 2 },
  { 114, 4 },
  { 114, 2 },
  { 114, 1 },
  { 114, 1 },
  { 114, 1 },
  { 114, 1 },
  { 114, 1 },
  { 114, 1 },
  { 114, 1 },
  { 114, 2 },
  { 114, 3 },
  { 114, 2 },
  { 114, 4 },
  { 114, 5 },
  { 114, 4 },
  { 114, 3 },
  { 114, 1 },
  { 122, 6 },
  { 122, 5 },
  { 123, 3 },
  { 123, 1 },
  { 128, 1 },
  { 127, 3 },
  { 127, 1 },
  { 129, 3 },
  { 129, 1 },
  { 130, 1 },
  { 130, 1 },
  { 130, 1 },
  { 131, 1 },
  { 131, 1 },
  { 88, 1 },
  { 88, 1 },
  { 88, 1 },
  { 88, 1 },
  { 88, 1 },
  { 88, 1 },
  { 126, 4 },
  { 126, 4 },
  { 126, 4 },
  { 111, 1 },
  { 82, 1 },
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
// 772 "parser.lemon"
{
	status->ret = yymsp[0].minor.yy247;
}
// 2287 "parser.c"
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
      case 183:
      case 188:
      case 196:
      case 207:
// 778 "parser.lemon"
{
	yygotominor.yy247 = yymsp[0].minor.yy247;
}
// 2318 "parser.c"
        break;
      case 2:
      case 25:
      case 35:
      case 37:
      case 69:
      case 96:
// 782 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_list(yymsp[-1].minor.yy247, yymsp[0].minor.yy247);
}
// 2330 "parser.c"
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
      case 187:
      case 190:
// 786 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_list(NULL, yymsp[0].minor.yy247);
}
// 2348 "parser.c"
        break;
      case 7:
// 802 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_namespace(yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(19,&yymsp[-2].minor);
  yy_destructor(21,&yymsp[0].minor);
}
// 2357 "parser.c"
        break;
      case 8:
// 806 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[-1].minor.yy247, 0, NULL, status->scanner_state);
  yy_destructor(22,&yymsp[-4].minor);
  yy_destructor(23,&yymsp[-2].minor);
  yy_destructor(24,&yymsp[0].minor);
}
// 2367 "parser.c"
        break;
      case 9:
// 810 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_class(yymsp[-5].minor.yy0, yymsp[-1].minor.yy247, 0, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(22,&yymsp[-6].minor);
  yy_destructor(25,&yymsp[-4].minor);
  yy_destructor(23,&yymsp[-2].minor);
  yy_destructor(24,&yymsp[0].minor);
}
// 2378 "parser.c"
        break;
      case 10:
// 814 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[-1].minor.yy247, 1, NULL, status->scanner_state);
  yy_destructor(26,&yymsp[-5].minor);
  yy_destructor(22,&yymsp[-4].minor);
  yy_destructor(23,&yymsp[-2].minor);
  yy_destructor(24,&yymsp[0].minor);
}
// 2389 "parser.c"
        break;
      case 11:
// 818 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_class(yymsp[-5].minor.yy0, yymsp[-1].minor.yy247, 1, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(26,&yymsp[-7].minor);
  yy_destructor(22,&yymsp[-6].minor);
  yy_destructor(25,&yymsp[-4].minor);
  yy_destructor(23,&yymsp[-2].minor);
  yy_destructor(24,&yymsp[0].minor);
}
// 2401 "parser.c"
        break;
      case 12:
// 822 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_class(yymsp[-2].minor.yy0, NULL, 0, NULL, status->scanner_state);
  yy_destructor(22,&yymsp[-3].minor);
  yy_destructor(23,&yymsp[-1].minor);
  yy_destructor(24,&yymsp[0].minor);
}
// 2411 "parser.c"
        break;
      case 13:
// 826 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_class(yymsp[-4].minor.yy0, NULL, 0, yymsp[-2].minor.yy0, status->scanner_state);
  yy_destructor(22,&yymsp[-5].minor);
  yy_destructor(25,&yymsp[-3].minor);
  yy_destructor(23,&yymsp[-1].minor);
  yy_destructor(24,&yymsp[0].minor);
}
// 2422 "parser.c"
        break;
      case 14:
// 830 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_class(yymsp[-2].minor.yy0, NULL, 1, NULL, status->scanner_state);
  yy_destructor(26,&yymsp[-4].minor);
  yy_destructor(22,&yymsp[-3].minor);
  yy_destructor(23,&yymsp[-1].minor);
  yy_destructor(24,&yymsp[0].minor);
}
// 2433 "parser.c"
        break;
      case 15:
// 834 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_class(yymsp[-4].minor.yy0, NULL, 1, yymsp[-2].minor.yy0, status->scanner_state);
  yy_destructor(26,&yymsp[-6].minor);
  yy_destructor(22,&yymsp[-5].minor);
  yy_destructor(25,&yymsp[-3].minor);
  yy_destructor(23,&yymsp[-1].minor);
  yy_destructor(24,&yymsp[0].minor);
}
// 2445 "parser.c"
        break;
      case 16:
// 838 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_class_definition(yymsp[0].minor.yy247, NULL, NULL, status->scanner_state);
}
// 2452 "parser.c"
        break;
      case 17:
// 842 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_class_definition(NULL, NULL, yymsp[0].minor.yy247, status->scanner_state);
}
// 2459 "parser.c"
        break;
      case 18:
// 846 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_class_definition(NULL, yymsp[0].minor.yy247, NULL, status->scanner_state);
}
// 2466 "parser.c"
        break;
      case 19:
// 850 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_class_definition(yymsp[-1].minor.yy247, yymsp[0].minor.yy247, NULL, status->scanner_state);
}
// 2473 "parser.c"
        break;
      case 20:
// 854 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_class_definition(yymsp[-1].minor.yy247, NULL, yymsp[0].minor.yy247, status->scanner_state);
}
// 2480 "parser.c"
        break;
      case 21:
// 858 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_class_definition(yymsp[0].minor.yy247, NULL, yymsp[-1].minor.yy247, status->scanner_state);
}
// 2487 "parser.c"
        break;
      case 22:
// 862 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_class_definition(NULL, yymsp[0].minor.yy247, yymsp[-1].minor.yy247, status->scanner_state);
}
// 2494 "parser.c"
        break;
      case 23:
// 866 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_class_definition(yymsp[-2].minor.yy247, yymsp[0].minor.yy247, yymsp[-1].minor.yy247, status->scanner_state);
}
// 2501 "parser.c"
        break;
      case 24:
// 870 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_class_definition(yymsp[-1].minor.yy247, yymsp[0].minor.yy247, yymsp[-2].minor.yy247, status->scanner_state);
}
// 2508 "parser.c"
        break;
      case 27:
// 882 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_class_property(XX_T_PUBLIC, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(28,&yymsp[-2].minor);
  yy_destructor(21,&yymsp[0].minor);
}
// 2517 "parser.c"
        break;
      case 28:
// 886 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_class_property(XX_T_PUBLIC, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-2].minor);
  yy_destructor(21,&yymsp[0].minor);
}
// 2526 "parser.c"
        break;
      case 29:
// 890 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_class_property(XX_T_PUBLIC, yymsp[-3].minor.yy0, yymsp[-1].minor.yy247, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-4].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(21,&yymsp[0].minor);
}
// 2536 "parser.c"
        break;
      case 30:
// 894 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_class_property(XX_T_PUBLIC, yymsp[-3].minor.yy0, yymsp[-1].minor.yy247, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(28,&yymsp[-4].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(21,&yymsp[0].minor);
}
// 2546 "parser.c"
        break;
      case 31:
// 898 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_class_property(XX_T_PROTECTED, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(21,&yymsp[0].minor);
}
// 2555 "parser.c"
        break;
      case 32:
// 902 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_class_property(XX_T_PROTECTED, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(21,&yymsp[0].minor);
}
// 2564 "parser.c"
        break;
      case 33:
// 906 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_class_property(XX_T_PROTECTED, yymsp[-3].minor.yy0, yymsp[-1].minor.yy247, NULL, status->scanner_state);
  yy_destructor(30,&yymsp[-4].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(21,&yymsp[0].minor);
}
// 2574 "parser.c"
        break;
      case 34:
// 910 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_class_property(XX_T_PROTECTED, yymsp[-3].minor.yy0, yymsp[-1].minor.yy247, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(30,&yymsp[-4].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(21,&yymsp[0].minor);
}
// 2584 "parser.c"
        break;
      case 39:
// 930 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy247, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(31,&yymsp[-4].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(21,&yymsp[0].minor);
}
// 2594 "parser.c"
        break;
      case 40:
// 934 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy247, NULL, status->scanner_state);
  yy_destructor(31,&yymsp[-4].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(21,&yymsp[0].minor);
}
// 2604 "parser.c"
        break;
      case 41:
// 938 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-4].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-6].minor);
  yy_destructor(32,&yymsp[-5].minor);
  yy_destructor(33,&yymsp[-3].minor);
  yy_destructor(34,&yymsp[-2].minor);
  yy_destructor(23,&yymsp[-1].minor);
  yy_destructor(24,&yymsp[0].minor);
}
// 2617 "parser.c"
        break;
      case 42:
// 942 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-5].minor.yy0, yymsp[-3].minor.yy247, NULL, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-7].minor);
  yy_destructor(32,&yymsp[-6].minor);
  yy_destructor(33,&yymsp[-4].minor);
  yy_destructor(34,&yymsp[-2].minor);
  yy_destructor(23,&yymsp[-1].minor);
  yy_destructor(24,&yymsp[0].minor);
}
// 2630 "parser.c"
        break;
      case 43:
// 946 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-4].minor.yy0, NULL, NULL, yymsp[-7].minor.yy0, status->scanner_state);
  yy_destructor(28,&yymsp[-6].minor);
  yy_destructor(32,&yymsp[-5].minor);
  yy_destructor(33,&yymsp[-3].minor);
  yy_destructor(34,&yymsp[-2].minor);
  yy_destructor(23,&yymsp[-1].minor);
  yy_destructor(24,&yymsp[0].minor);
}
// 2643 "parser.c"
        break;
      case 44:
// 950 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-5].minor.yy0, yymsp[-3].minor.yy247, NULL, yymsp[-8].minor.yy0, status->scanner_state);
  yy_destructor(28,&yymsp[-7].minor);
  yy_destructor(32,&yymsp[-6].minor);
  yy_destructor(33,&yymsp[-4].minor);
  yy_destructor(34,&yymsp[-2].minor);
  yy_destructor(23,&yymsp[-1].minor);
  yy_destructor(24,&yymsp[0].minor);
}
// 2656 "parser.c"
        break;
      case 45:
// 954 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy247, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-7].minor);
  yy_destructor(32,&yymsp[-6].minor);
  yy_destructor(33,&yymsp[-4].minor);
  yy_destructor(34,&yymsp[-3].minor);
  yy_destructor(23,&yymsp[-2].minor);
  yy_destructor(24,&yymsp[0].minor);
}
// 2669 "parser.c"
        break;
      case 46:
// 958 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-6].minor.yy0, yymsp[-3].minor.yy247, yymsp[-1].minor.yy247, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-8].minor);
  yy_destructor(32,&yymsp[-7].minor);
  yy_destructor(33,&yymsp[-5].minor);
  yy_destructor(34,&yymsp[-4].minor);
  yy_destructor(23,&yymsp[-2].minor);
  yy_destructor(24,&yymsp[0].minor);
}
// 2682 "parser.c"
        break;
      case 47:
// 962 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy247, yymsp[-8].minor.yy0, status->scanner_state);
  yy_destructor(28,&yymsp[-7].minor);
  yy_destructor(32,&yymsp[-6].minor);
  yy_destructor(33,&yymsp[-4].minor);
  yy_destructor(34,&yymsp[-3].minor);
  yy_destructor(23,&yymsp[-2].minor);
  yy_destructor(24,&yymsp[0].minor);
}
// 2695 "parser.c"
        break;
      case 48:
// 966 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-6].minor.yy0, yymsp[-4].minor.yy247, yymsp[-1].minor.yy247, yymsp[-9].minor.yy0, status->scanner_state);
  yy_destructor(28,&yymsp[-8].minor);
  yy_destructor(32,&yymsp[-7].minor);
  yy_destructor(33,&yymsp[-5].minor);
  yy_destructor(34,&yymsp[-3].minor);
  yy_destructor(23,&yymsp[-2].minor);
  yy_destructor(24,&yymsp[0].minor);
}
// 2708 "parser.c"
        break;
      case 49:
// 970 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-6].minor.yy0, yymsp[-4].minor.yy247, yymsp[-1].minor.yy247, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-8].minor);
  yy_destructor(32,&yymsp[-7].minor);
  yy_destructor(33,&yymsp[-5].minor);
  yy_destructor(34,&yymsp[-3].minor);
  yy_destructor(23,&yymsp[-2].minor);
  yy_destructor(24,&yymsp[0].minor);
}
// 2721 "parser.c"
        break;
      case 50:
// 974 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_class_method(XX_T_PROTECTED, yymsp[-4].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(30,&yymsp[-6].minor);
  yy_destructor(32,&yymsp[-5].minor);
  yy_destructor(33,&yymsp[-3].minor);
  yy_destructor(34,&yymsp[-2].minor);
  yy_destructor(23,&yymsp[-1].minor);
  yy_destructor(24,&yymsp[0].minor);
}
// 2734 "parser.c"
        break;
      case 51:
// 978 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_class_method(XX_T_PROTECTED, yymsp[-4].minor.yy0, NULL, NULL, yymsp[-7].minor.yy0, status->scanner_state);
  yy_destructor(30,&yymsp[-6].minor);
  yy_destructor(32,&yymsp[-5].minor);
  yy_destructor(33,&yymsp[-3].minor);
  yy_destructor(34,&yymsp[-2].minor);
  yy_destructor(23,&yymsp[-1].minor);
  yy_destructor(24,&yymsp[0].minor);
}
// 2747 "parser.c"
        break;
      case 52:
// 982 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_class_method(XX_T_PROTECTED, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy247, NULL, status->scanner_state);
  yy_destructor(30,&yymsp[-7].minor);
  yy_destructor(32,&yymsp[-6].minor);
  yy_destructor(33,&yymsp[-4].minor);
  yy_destructor(34,&yymsp[-3].minor);
  yy_destructor(23,&yymsp[-2].minor);
  yy_destructor(24,&yymsp[0].minor);
}
// 2760 "parser.c"
        break;
      case 53:
// 986 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_class_method(XX_T_PROTECTED, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy247, yymsp[-8].minor.yy0, status->scanner_state);
  yy_destructor(30,&yymsp[-7].minor);
  yy_destructor(32,&yymsp[-6].minor);
  yy_destructor(33,&yymsp[-4].minor);
  yy_destructor(34,&yymsp[-3].minor);
  yy_destructor(23,&yymsp[-2].minor);
  yy_destructor(24,&yymsp[0].minor);
}
// 2773 "parser.c"
        break;
      case 54:
      case 109:
      case 127:
      case 143:
      case 186:
      case 189:
// 990 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_list(yymsp[-2].minor.yy247, yymsp[0].minor.yy247);
  yy_destructor(1,&yymsp[-1].minor);
}
// 2786 "parser.c"
        break;
      case 56:
// 998 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_parameter(NULL, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 2793 "parser.c"
        break;
      case 57:
// 1002 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_parameter(yymsp[-1].minor.yy247, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 2800 "parser.c"
        break;
      case 58:
// 1006 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_parameter(NULL, yymsp[-1].minor.yy247, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 2807 "parser.c"
        break;
      case 59:
// 1010 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_parameter(NULL, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy247, status->scanner_state);
  yy_destructor(29,&yymsp[-1].minor);
}
// 2815 "parser.c"
        break;
      case 60:
// 1014 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_parameter(yymsp[-3].minor.yy247, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy247, status->scanner_state);
  yy_destructor(29,&yymsp[-1].minor);
}
// 2823 "parser.c"
        break;
      case 61:
// 1018 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_parameter(NULL, yymsp[-3].minor.yy247, yymsp[-2].minor.yy0, yymsp[0].minor.yy247, status->scanner_state);
  yy_destructor(29,&yymsp[-1].minor);
}
// 2831 "parser.c"
        break;
      case 62:
// 1022 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(4,&yymsp[-2].minor);
  yy_destructor(5,&yymsp[0].minor);
}
// 2840 "parser.c"
        break;
      case 63:
// 1026 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_type(XX_TYPE_INTEGER);
  yy_destructor(35,&yymsp[0].minor);
}
// 2848 "parser.c"
        break;
      case 64:
// 1030 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_type(XX_TYPE_DOUBLE);
  yy_destructor(36,&yymsp[0].minor);
}
// 2856 "parser.c"
        break;
      case 65:
// 1034 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_type(XX_TYPE_BOOL);
  yy_destructor(37,&yymsp[0].minor);
}
// 2864 "parser.c"
        break;
      case 66:
// 1038 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_type(XX_TYPE_STRING);
  yy_destructor(38,&yymsp[0].minor);
}
// 2872 "parser.c"
        break;
      case 67:
// 1042 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_type(XX_TYPE_ARRAY);
  yy_destructor(39,&yymsp[0].minor);
}
// 2880 "parser.c"
        break;
      case 68:
// 1046 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_type(XX_TYPE_VAR);
  yy_destructor(40,&yymsp[0].minor);
}
// 2888 "parser.c"
        break;
      case 86:
// 1118 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_break_statement(status->scanner_state);
  yy_destructor(41,&yymsp[-1].minor);
  yy_destructor(21,&yymsp[0].minor);
}
// 2897 "parser.c"
        break;
      case 87:
// 1122 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_continue_statement(status->scanner_state);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(21,&yymsp[0].minor);
}
// 2906 "parser.c"
        break;
      case 88:
// 1126 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_if_statement(yymsp[-2].minor.yy247, NULL, NULL, status->scanner_state);
  yy_destructor(43,&yymsp[-3].minor);
  yy_destructor(23,&yymsp[-1].minor);
  yy_destructor(24,&yymsp[0].minor);
}
// 2916 "parser.c"
        break;
      case 89:
// 1130 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_if_statement(yymsp[-5].minor.yy247, NULL, NULL, status->scanner_state);
  yy_destructor(43,&yymsp[-6].minor);
  yy_destructor(23,&yymsp[-4].minor);
  yy_destructor(24,&yymsp[-3].minor);
  yy_destructor(44,&yymsp[-2].minor);
  yy_destructor(23,&yymsp[-1].minor);
  yy_destructor(24,&yymsp[0].minor);
}
// 2929 "parser.c"
        break;
      case 90:
// 1134 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_if_statement(yymsp[-3].minor.yy247, yymsp[-1].minor.yy247, NULL, status->scanner_state);
  yy_destructor(43,&yymsp[-4].minor);
  yy_destructor(23,&yymsp[-2].minor);
  yy_destructor(24,&yymsp[0].minor);
}
// 2939 "parser.c"
        break;
      case 91:
// 1138 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_if_statement(yymsp[-7].minor.yy247, yymsp[-5].minor.yy247, yymsp[-1].minor.yy247, status->scanner_state);
  yy_destructor(43,&yymsp[-8].minor);
  yy_destructor(23,&yymsp[-6].minor);
  yy_destructor(24,&yymsp[-4].minor);
  yy_destructor(44,&yymsp[-3].minor);
  yy_destructor(23,&yymsp[-2].minor);
  yy_destructor(24,&yymsp[0].minor);
}
// 2952 "parser.c"
        break;
      case 92:
// 1142 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_if_statement(yymsp[-6].minor.yy247, yymsp[-4].minor.yy247, NULL, status->scanner_state);
  yy_destructor(43,&yymsp[-7].minor);
  yy_destructor(23,&yymsp[-5].minor);
  yy_destructor(24,&yymsp[-3].minor);
  yy_destructor(44,&yymsp[-2].minor);
  yy_destructor(23,&yymsp[-1].minor);
  yy_destructor(24,&yymsp[0].minor);
}
// 2965 "parser.c"
        break;
      case 93:
// 1146 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_if_statement(yymsp[-6].minor.yy247, NULL, yymsp[-1].minor.yy247, status->scanner_state);
  yy_destructor(43,&yymsp[-7].minor);
  yy_destructor(23,&yymsp[-5].minor);
  yy_destructor(24,&yymsp[-4].minor);
  yy_destructor(44,&yymsp[-3].minor);
  yy_destructor(23,&yymsp[-2].minor);
  yy_destructor(24,&yymsp[0].minor);
}
// 2978 "parser.c"
        break;
      case 94:
// 1150 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_switch_statement(yymsp[-2].minor.yy247, NULL, status->scanner_state);
  yy_destructor(45,&yymsp[-3].minor);
  yy_destructor(23,&yymsp[-1].minor);
  yy_destructor(24,&yymsp[0].minor);
}
// 2988 "parser.c"
        break;
      case 95:
// 1154 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_switch_statement(yymsp[-3].minor.yy247, yymsp[-1].minor.yy247, status->scanner_state);
  yy_destructor(45,&yymsp[-4].minor);
  yy_destructor(23,&yymsp[-2].minor);
  yy_destructor(24,&yymsp[0].minor);
}
// 2998 "parser.c"
        break;
      case 98:
// 1166 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_case_clause(yymsp[-2].minor.yy247, yymsp[0].minor.yy247, status->scanner_state);
  yy_destructor(46,&yymsp[-3].minor);
  yy_destructor(47,&yymsp[-1].minor);
}
// 3007 "parser.c"
        break;
      case 99:
// 1170 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_case_clause(NULL, yymsp[0].minor.yy247, status->scanner_state);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[-1].minor);
}
// 3016 "parser.c"
        break;
      case 100:
// 1174 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_loop_statement(NULL, status->scanner_state);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(23,&yymsp[-1].minor);
  yy_destructor(24,&yymsp[0].minor);
}
// 3026 "parser.c"
        break;
      case 101:
// 1178 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_loop_statement(yymsp[-1].minor.yy247, status->scanner_state);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(23,&yymsp[-2].minor);
  yy_destructor(24,&yymsp[0].minor);
}
// 3036 "parser.c"
        break;
      case 102:
// 1182 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_while_statement(yymsp[-2].minor.yy247, NULL, status->scanner_state);
  yy_destructor(50,&yymsp[-3].minor);
  yy_destructor(23,&yymsp[-1].minor);
  yy_destructor(24,&yymsp[0].minor);
}
// 3046 "parser.c"
        break;
      case 103:
// 1186 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_while_statement(yymsp[-3].minor.yy247, yymsp[-1].minor.yy247, status->scanner_state);
  yy_destructor(50,&yymsp[-4].minor);
  yy_destructor(23,&yymsp[-2].minor);
  yy_destructor(24,&yymsp[0].minor);
}
// 3056 "parser.c"
        break;
      case 104:
// 1190 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_for_statement(yymsp[-3].minor.yy247, NULL, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy247, status->scanner_state);
  yy_destructor(51,&yymsp[-6].minor);
  yy_destructor(52,&yymsp[-4].minor);
  yy_destructor(23,&yymsp[-2].minor);
  yy_destructor(24,&yymsp[0].minor);
}
// 3067 "parser.c"
        break;
      case 105:
// 1194 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_for_statement(yymsp[-3].minor.yy247, NULL, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy247, status->scanner_state);
  yy_destructor(51,&yymsp[-7].minor);
  yy_destructor(52,&yymsp[-5].minor);
  yy_destructor(53,&yymsp[-4].minor);
  yy_destructor(23,&yymsp[-2].minor);
  yy_destructor(24,&yymsp[0].minor);
}
// 3079 "parser.c"
        break;
      case 106:
// 1198 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_for_statement(yymsp[-3].minor.yy247, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy247, status->scanner_state);
  yy_destructor(51,&yymsp[-8].minor);
  yy_destructor(1,&yymsp[-6].minor);
  yy_destructor(52,&yymsp[-4].minor);
  yy_destructor(23,&yymsp[-2].minor);
  yy_destructor(24,&yymsp[0].minor);
}
// 3091 "parser.c"
        break;
      case 107:
// 1202 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_for_statement(yymsp[-3].minor.yy247, yymsp[-8].minor.yy0, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy247, status->scanner_state);
  yy_destructor(51,&yymsp[-9].minor);
  yy_destructor(1,&yymsp[-7].minor);
  yy_destructor(52,&yymsp[-5].minor);
  yy_destructor(53,&yymsp[-4].minor);
  yy_destructor(23,&yymsp[-2].minor);
  yy_destructor(24,&yymsp[0].minor);
}
// 3104 "parser.c"
        break;
      case 108:
// 1206 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_let_statement(yymsp[-1].minor.yy247, status->scanner_state);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(21,&yymsp[0].minor);
}
// 3113 "parser.c"
        break;
      case 111:
// 1219 "parser.lemon"
{
	yygotominor.yy247 = json_object_new_string("assign");
  yy_destructor(29,&yymsp[0].minor);
}
// 3121 "parser.c"
        break;
      case 112:
// 1224 "parser.lemon"
{
	yygotominor.yy247 = json_object_new_string("add-assign");
  yy_destructor(55,&yymsp[0].minor);
}
// 3129 "parser.c"
        break;
      case 113:
// 1229 "parser.lemon"
{
	yygotominor.yy247 = json_object_new_string("sub-assign");
  yy_destructor(56,&yymsp[0].minor);
}
// 3137 "parser.c"
        break;
      case 114:
// 1233 "parser.lemon"
{
	yygotominor.yy247 = json_object_new_string("mult-assign");
  yy_destructor(57,&yymsp[0].minor);
}
// 3145 "parser.c"
        break;
      case 115:
// 1237 "parser.lemon"
{
	yygotominor.yy247 = json_object_new_string("div-assign");
  yy_destructor(58,&yymsp[0].minor);
}
// 3153 "parser.c"
        break;
      case 116:
// 1241 "parser.lemon"
{
	yygotominor.yy247 = json_object_new_string("concat-assign");
  yy_destructor(59,&yymsp[0].minor);
}
// 3161 "parser.c"
        break;
      case 117:
// 1246 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_let_assignment("variable", yymsp[-1].minor.yy247, yymsp[-2].minor.yy0, NULL, NULL, yymsp[0].minor.yy247, status->scanner_state);
}
// 3168 "parser.c"
        break;
      case 118:
// 1251 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_let_assignment("object-property", yymsp[-1].minor.yy247, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy247, status->scanner_state);
  yy_destructor(60,&yymsp[-3].minor);
}
// 3176 "parser.c"
        break;
      case 119:
// 1256 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_let_assignment("object-property-append", yymsp[-1].minor.yy247, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy247, status->scanner_state);
  yy_destructor(60,&yymsp[-5].minor);
  yy_destructor(61,&yymsp[-3].minor);
  yy_destructor(62,&yymsp[-2].minor);
}
// 3186 "parser.c"
        break;
      case 120:
// 1261 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_let_assignment("object-property-array-index", yymsp[-1].minor.yy247, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy247, yymsp[0].minor.yy247, status->scanner_state);
  yy_destructor(60,&yymsp[-6].minor);
  yy_destructor(61,&yymsp[-4].minor);
  yy_destructor(62,&yymsp[-2].minor);
}
// 3196 "parser.c"
        break;
      case 121:
// 1266 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_let_assignment("variable-append", yymsp[-1].minor.yy247, yymsp[-4].minor.yy0, NULL, NULL, yymsp[0].minor.yy247, status->scanner_state);
  yy_destructor(61,&yymsp[-3].minor);
  yy_destructor(62,&yymsp[-2].minor);
}
// 3205 "parser.c"
        break;
      case 122:
// 1271 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_let_assignment("array-index", yymsp[-1].minor.yy247, yymsp[-5].minor.yy0, NULL, yymsp[-3].minor.yy247, yymsp[0].minor.yy247, status->scanner_state);
  yy_destructor(61,&yymsp[-4].minor);
  yy_destructor(62,&yymsp[-2].minor);
}
// 3214 "parser.c"
        break;
      case 123:
// 1276 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_let_assignment("incr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(63,&yymsp[0].minor);
}
// 3222 "parser.c"
        break;
      case 124:
// 1281 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_let_assignment("decr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(64,&yymsp[0].minor);
}
// 3230 "parser.c"
        break;
      case 126:
// 1289 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_echo_statement(yymsp[-1].minor.yy247, status->scanner_state);
  yy_destructor(65,&yymsp[-2].minor);
  yy_destructor(21,&yymsp[0].minor);
}
// 3239 "parser.c"
        break;
      case 129:
// 1301 "parser.lemon"
{
	yygotominor.yy247 = yymsp[0].minor.yy247;;
}
// 3246 "parser.c"
        break;
      case 130:
// 1306 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_mcall_statement(yymsp[-1].minor.yy247, status->scanner_state);
  yy_destructor(21,&yymsp[0].minor);
}
// 3254 "parser.c"
        break;
      case 131:
// 1311 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_return_statement(yymsp[-1].minor.yy247, status->scanner_state);
  yy_destructor(66,&yymsp[-2].minor);
  yy_destructor(21,&yymsp[0].minor);
}
// 3263 "parser.c"
        break;
      case 132:
// 1316 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(67,&yymsp[-4].minor);
  yy_destructor(60,&yymsp[-2].minor);
  yy_destructor(21,&yymsp[0].minor);
}
// 3277 "parser.c"
        break;
      case 133:
// 1325 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(67,&yymsp[-5].minor);
  yy_destructor(61,&yymsp[-3].minor);
  yy_destructor(62,&yymsp[-1].minor);
  yy_destructor(21,&yymsp[0].minor);
}
// 3292 "parser.c"
        break;
      case 134:
// 1334 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_INTEGER, yymsp[-2].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(67,&yymsp[-5].minor);
  yy_destructor(61,&yymsp[-3].minor);
  yy_destructor(62,&yymsp[-1].minor);
  yy_destructor(21,&yymsp[0].minor);
}
// 3307 "parser.c"
        break;
      case 135:
// 1343 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_STRING, yymsp[-2].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(67,&yymsp[-5].minor);
  yy_destructor(61,&yymsp[-3].minor);
  yy_destructor(62,&yymsp[-1].minor);
  yy_destructor(21,&yymsp[0].minor);
}
// 3322 "parser.c"
        break;
      case 136:
// 1352 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_throw_exception(yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(70,&yymsp[-2].minor);
  yy_destructor(21,&yymsp[0].minor);
}
// 3331 "parser.c"
        break;
      case 137:
// 1357 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_throw_exception(yymsp[-4].minor.yy0, yymsp[-2].minor.yy247, status->scanner_state);
  yy_destructor(70,&yymsp[-6].minor);
  yy_destructor(17,&yymsp[-5].minor);
  yy_destructor(33,&yymsp[-3].minor);
  yy_destructor(34,&yymsp[-1].minor);
  yy_destructor(21,&yymsp[0].minor);
}
// 3343 "parser.c"
        break;
      case 138:
// 1361 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_declare_statement(XX_T_TYPE_INTEGER, yymsp[-1].minor.yy247, status->scanner_state);
  yy_destructor(35,&yymsp[-2].minor);
  yy_destructor(21,&yymsp[0].minor);
}
// 3352 "parser.c"
        break;
      case 139:
// 1365 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_declare_statement(XX_T_TYPE_DOUBLE, yymsp[-1].minor.yy247, status->scanner_state);
  yy_destructor(36,&yymsp[-2].minor);
  yy_destructor(21,&yymsp[0].minor);
}
// 3361 "parser.c"
        break;
      case 140:
// 1369 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_declare_statement(XX_T_TYPE_STRING, yymsp[-1].minor.yy247, status->scanner_state);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(21,&yymsp[0].minor);
}
// 3370 "parser.c"
        break;
      case 141:
// 1373 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_declare_statement(XX_T_TYPE_BOOL, yymsp[-1].minor.yy247, status->scanner_state);
  yy_destructor(37,&yymsp[-2].minor);
  yy_destructor(21,&yymsp[0].minor);
}
// 3379 "parser.c"
        break;
      case 142:
// 1377 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_declare_statement(XX_T_TYPE_VAR, yymsp[-1].minor.yy247, status->scanner_state);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(21,&yymsp[0].minor);
}
// 3388 "parser.c"
        break;
      case 145:
// 1389 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_declare_variable(yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3395 "parser.c"
        break;
      case 146:
// 1393 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_declare_variable(yymsp[-2].minor.yy0, yymsp[0].minor.yy247, status->scanner_state);
  yy_destructor(29,&yymsp[-1].minor);
}
// 3403 "parser.c"
        break;
      case 148:
// 1401 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_expr("not", yymsp[0].minor.yy247, NULL, NULL, status->scanner_state);
  yy_destructor(18,&yymsp[-1].minor);
}
// 3411 "parser.c"
        break;
      case 149:
// 1405 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_expr("isset", yymsp[0].minor.yy247, NULL, NULL, status->scanner_state);
  yy_destructor(13,&yymsp[-1].minor);
}
// 3419 "parser.c"
        break;
      case 150:
// 1409 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_expr("equals", yymsp[-2].minor.yy247, yymsp[0].minor.yy247, NULL, status->scanner_state);
  yy_destructor(2,&yymsp[-1].minor);
}
// 3427 "parser.c"
        break;
      case 151:
// 1413 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_expr("not-equals", yymsp[-2].minor.yy247, yymsp[0].minor.yy247, NULL, status->scanner_state);
  yy_destructor(9,&yymsp[-1].minor);
}
// 3435 "parser.c"
        break;
      case 152:
// 1417 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_expr("identical", yymsp[-2].minor.yy247, yymsp[0].minor.yy247, NULL, status->scanner_state);
  yy_destructor(3,&yymsp[-1].minor);
}
// 3443 "parser.c"
        break;
      case 153:
// 1421 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_expr("not-identical", yymsp[-2].minor.yy247, yymsp[0].minor.yy247, NULL, status->scanner_state);
  yy_destructor(8,&yymsp[-1].minor);
}
// 3451 "parser.c"
        break;
      case 154:
// 1425 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_expr("less", yymsp[-2].minor.yy247, yymsp[0].minor.yy247, NULL, status->scanner_state);
  yy_destructor(4,&yymsp[-1].minor);
}
// 3459 "parser.c"
        break;
      case 155:
// 1429 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_expr("greater", yymsp[-2].minor.yy247, yymsp[0].minor.yy247, NULL, status->scanner_state);
  yy_destructor(5,&yymsp[-1].minor);
}
// 3467 "parser.c"
        break;
      case 156:
// 1433 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_expr("less-equal", yymsp[-2].minor.yy247, yymsp[0].minor.yy247, NULL, status->scanner_state);
  yy_destructor(6,&yymsp[-1].minor);
}
// 3475 "parser.c"
        break;
      case 157:
// 1437 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_expr("greater-equal", yymsp[-2].minor.yy247, yymsp[0].minor.yy247, NULL, status->scanner_state);
  yy_destructor(7,&yymsp[-1].minor);
}
// 3483 "parser.c"
        break;
      case 158:
// 1441 "parser.lemon"
{
	yygotominor.yy247 = yymsp[-1].minor.yy247;
  yy_destructor(33,&yymsp[-2].minor);
  yy_destructor(34,&yymsp[0].minor);
}
// 3492 "parser.c"
        break;
      case 159:
// 1445 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_expr("property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(60,&yymsp[-1].minor);
}
// 3500 "parser.c"
        break;
      case 160:
      case 204:
// 1449 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(61,&yymsp[-2].minor);
  yy_destructor(62,&yymsp[0].minor);
}
// 3510 "parser.c"
        break;
      case 161:
      case 205:
// 1454 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_INTEGER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(61,&yymsp[-2].minor);
  yy_destructor(62,&yymsp[0].minor);
}
// 3520 "parser.c"
        break;
      case 162:
      case 206:
// 1459 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_STRING, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(61,&yymsp[-2].minor);
  yy_destructor(62,&yymsp[0].minor);
}
// 3530 "parser.c"
        break;
      case 163:
// 1464 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_expr("add", yymsp[-2].minor.yy247, yymsp[0].minor.yy247, NULL, status->scanner_state);
  yy_destructor(10,&yymsp[-1].minor);
}
// 3538 "parser.c"
        break;
      case 164:
// 1469 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_expr("sub", yymsp[-2].minor.yy247, yymsp[0].minor.yy247, NULL, status->scanner_state);
  yy_destructor(11,&yymsp[-1].minor);
}
// 3546 "parser.c"
        break;
      case 165:
// 1474 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_expr("concat", yymsp[-2].minor.yy247, yymsp[0].minor.yy247, NULL, status->scanner_state);
  yy_destructor(12,&yymsp[-1].minor);
}
// 3554 "parser.c"
        break;
      case 166:
// 1479 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_expr("instanceof", yymsp[0].minor.yy247, NULL, NULL, status->scanner_state);
  yy_destructor(16,&yymsp[-1].minor);
}
// 3562 "parser.c"
        break;
      case 167:
// 1484 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_expr("fetch", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy247, NULL, status->scanner_state);
  yy_destructor(14,&yymsp[-3].minor);
  yy_destructor(1,&yymsp[-1].minor);
}
// 3571 "parser.c"
        break;
      case 168:
// 1489 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_expr("typeof", yymsp[0].minor.yy247, NULL, NULL, status->scanner_state);
  yy_destructor(15,&yymsp[-1].minor);
}
// 3579 "parser.c"
        break;
      case 169:
      case 193:
      case 197:
// 1494 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state);
}
// 3588 "parser.c"
        break;
      case 170:
      case 195:
      case 198:
// 1499 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_literal(XX_T_INTEGER, yymsp[0].minor.yy0, status->scanner_state);
}
// 3597 "parser.c"
        break;
      case 171:
      case 194:
      case 199:
// 1504 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_literal(XX_T_STRING, yymsp[0].minor.yy0, status->scanner_state);
}
// 3606 "parser.c"
        break;
      case 172:
      case 200:
// 1509 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_literal(XX_T_DOUBLE, yymsp[0].minor.yy0, status->scanner_state);
}
// 3614 "parser.c"
        break;
      case 173:
      case 201:
// 1514 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_literal(XX_T_NULL, NULL, status->scanner_state);
  yy_destructor(72,&yymsp[0].minor);
}
// 3623 "parser.c"
        break;
      case 174:
      case 203:
// 1519 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_literal(XX_T_TRUE, NULL, status->scanner_state);
  yy_destructor(73,&yymsp[0].minor);
}
// 3632 "parser.c"
        break;
      case 175:
      case 202:
// 1524 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_literal(XX_T_FALSE, NULL, status->scanner_state);
  yy_destructor(74,&yymsp[0].minor);
}
// 3641 "parser.c"
        break;
      case 176:
// 1529 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_expr("empty-array", NULL, NULL, NULL, status->scanner_state);
  yy_destructor(61,&yymsp[-1].minor);
  yy_destructor(62,&yymsp[0].minor);
}
// 3650 "parser.c"
        break;
      case 177:
// 1534 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_expr("array", yymsp[-1].minor.yy247, NULL, NULL, status->scanner_state);
  yy_destructor(61,&yymsp[-2].minor);
  yy_destructor(62,&yymsp[0].minor);
}
// 3659 "parser.c"
        break;
      case 178:
// 1539 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_new_instance(yymsp[0].minor.yy0, NULL, status->scanner_state);
  yy_destructor(17,&yymsp[-1].minor);
}
// 3667 "parser.c"
        break;
      case 179:
// 1544 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_new_instance(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(17,&yymsp[-3].minor);
  yy_destructor(33,&yymsp[-1].minor);
  yy_destructor(34,&yymsp[0].minor);
}
// 3677 "parser.c"
        break;
      case 180:
// 1549 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_new_instance(yymsp[-3].minor.yy0, yymsp[-1].minor.yy247, status->scanner_state);
  yy_destructor(17,&yymsp[-4].minor);
  yy_destructor(33,&yymsp[-2].minor);
  yy_destructor(34,&yymsp[0].minor);
}
// 3687 "parser.c"
        break;
      case 181:
// 1554 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_fcall(yymsp[-3].minor.yy0, yymsp[-1].minor.yy247, status->scanner_state);
  yy_destructor(33,&yymsp[-2].minor);
  yy_destructor(34,&yymsp[0].minor);
}
// 3696 "parser.c"
        break;
      case 182:
// 1559 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_fcall(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(33,&yymsp[-1].minor);
  yy_destructor(34,&yymsp[0].minor);
}
// 3705 "parser.c"
        break;
      case 184:
// 1569 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_mcall(yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy247, status->scanner_state);
  yy_destructor(60,&yymsp[-4].minor);
  yy_destructor(33,&yymsp[-2].minor);
  yy_destructor(34,&yymsp[0].minor);
}
// 3715 "parser.c"
        break;
      case 185:
// 1574 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_mcall(yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(60,&yymsp[-3].minor);
  yy_destructor(33,&yymsp[-1].minor);
  yy_destructor(34,&yymsp[0].minor);
}
// 3725 "parser.c"
        break;
      case 191:
// 1598 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_array_item(yymsp[-2].minor.yy247, yymsp[0].minor.yy247, status->scanner_state);
  yy_destructor(47,&yymsp[-1].minor);
}
// 3733 "parser.c"
        break;
      case 192:
// 1602 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_array_item(NULL, yymsp[0].minor.yy247, status->scanner_state);
}
// 3740 "parser.c"
        break;
      case 208:
// 1669 "parser.lemon"
{
	yygotominor.yy247 = xx_ret_comment(yymsp[0].minor.yy0, status->scanner_state);
}
// 3747 "parser.c"
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
// 729 "parser.lemon"


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

// 3823 "parser.c"
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
