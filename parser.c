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


// 689 "parser.c"
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
#define YYNOCODE 142
#define YYACTIONTYPE unsigned short int
#define xx_TOKENTYPE xx_parser_token*
typedef union {
  xx_TOKENTYPE yy0;
  json_object* yy25;
  int yy283;
} YYMINORTYPE;
#define YYSTACKDEPTH 100
#define xx_ARG_SDECL xx_parser_status *status;
#define xx_ARG_PDECL ,xx_parser_status *status
#define xx_ARG_FETCH xx_parser_status *status = yypParser->status
#define xx_ARG_STORE yypParser->status = status
#define YYNSTATE 450
#define YYNRULE 223
#define YYERRORSYMBOL 80
#define YYERRSYMDT yy283
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
 /*     0 */   286,  126,  128,  130,   25,  169,  170,  289,  171,  172,
 /*    10 */   174,  173,  450,  200,  265,  274,  280,  277,  143,  283,
 /*    20 */    44,   46,   48,   80,   57,   90,  187,  176,   68,   72,
 /*    30 */    77,  187,  188,   84,  674,    1,    2,  449,    4,    5,
 /*    40 */     6,  140,  193,  286,  229,  238,  241,   27,  107,  256,
 /*    50 */   289,   91,   92,   93,   94,   95,    9,  265,  274,  280,
 /*    60 */   277,  204,  283,   44,   46,   48,  451,   57,  159,  187,
 /*    70 */   176,   68,   72,   77,  178,  177,   84,  171,  172,  174,
 /*    80 */   173,   12,  167,  430,   61,  314,  286,  229,  238,  241,
 /*    90 */   316,    7,  256,  289,   10,  109,  111,    8,  436,  289,
 /*   100 */   265,  274,  280,  277,  217,  283,   44,   46,   48,  213,
 /*   110 */    57,   11,  187,  168,   68,   72,   77,  178,  177,   84,
 /*   120 */   171,  172,  174,  173,  160,  296,  179,  164,  175,  286,
 /*   130 */   229,  238,  241,   52,    7,  256,  289,   10,  329,  333,
 /*   140 */   337,  436,  289,  265,  274,  280,  277,   14,  283,   44,
 /*   150 */    46,   48,   20,   57,   85,  228,   21,   68,   72,   77,
 /*   160 */   169,  170,   84,  171,  172,  174,  173,   24,  368,  370,
 /*   170 */   373,  377,  286,  229,  238,  241,  315,   79,  256,  289,
 /*   180 */   322,   90,  167,  350,  333,  337,  265,  274,  280,  277,
 /*   190 */   143,  283,   44,   46,   48,  101,   57,   22,  187,  190,
 /*   200 */    68,   72,   77,  187,  189,   84,  380,   91,   92,   93,
 /*   210 */    94,   95,  113,  209,   45,  286,  229,  238,  241,   56,
 /*   220 */   187,  256,  289,  194,  407,  408,  409,  410,  166,  265,
 /*   230 */   274,  280,  277,  217,  283,   44,   46,   48,  222,   57,
 /*   240 */   381,  187,  382,   68,   72,   77,   18,   19,   84,  401,
 /*   250 */   367,  407,  408,  409,  410,  115,  243,  246,  286,  229,
 /*   260 */   238,  241,  310,  187,  256,  289,    3,    4,    5,    6,
 /*   270 */   196,  198,  265,  274,  280,  277,  417,  283,   44,   46,
 /*   280 */    48,  143,   57,  266,  273,   47,   68,   72,   77,  187,
 /*   290 */   184,   84,  234,  167,  259,  189,   50,  257,  230,  235,
 /*   300 */   187,  286,  229,  238,  241,   71,  361,  256,  289,  350,
 /*   310 */   333,  337,  275,  273,   87,  265,  274,  280,  277,   60,
 /*   320 */   283,   44,   46,   48,   63,   57,  278,  273,   53,   68,
 /*   330 */    72,   77,   49,   62,   84,  309,   86,  163,  164,  175,
 /*   340 */    64,   54,  311,  187,  286,  229,  238,  241,  308,   98,
 /*   350 */   256,  289,  186,   97,   64,   59,  311,  187,  265,  274,
 /*   360 */   280,  277,  234,  283,   44,   46,   48,  143,   57,  233,
 /*   370 */   187,  117,   68,   72,   77,  187,  201,   84,  247,  187,
 /*   380 */   162,  189,   65,  374,  375,  185,  376,  286,  229,  238,
 /*   390 */   241,   76,   98,  256,  289,   66,  208,  281,  273,  232,
 /*   400 */   187,  265,  274,  280,  277,  143,  283,   44,   46,   48,
 /*   410 */   186,   57,   69,  187,  262,   68,   72,   77,  504,  189,
 /*   420 */    84,  231,  186,  186,   58,  250,  253,  309,  284,  273,
 /*   430 */   286,  229,  238,  241,   83,  187,  256,  289,  328,  328,
 /*   440 */    74,   78,  161,  191,  265,  274,  280,  277,  371,  283,
 /*   450 */    44,   46,   48,   98,   57,  202,  263,  212,   68,   72,
 /*   460 */    77,  187,  400,   84,  411,  352,  503,  352,  367,  372,
 /*   470 */   367,  323,  362,  286,  229,  238,  241,  295,   98,  256,
 /*   480 */   289,  503,  216,  268,  372,   88,  187,  265,  274,  280,
 /*   490 */   277,  378,  283,   44,   46,   48,   73,   57,   89,  309,
 /*   500 */    98,   68,   72,   77,  221,  267,   84,  187,  187,  505,
 /*   510 */    96,  105,  379,  119,  104,  106,  286,  229,  238,  241,
 /*   520 */   302,  187,  256,  289,  505,  369,  121,  379,  108,  268,
 /*   530 */   265,  274,  280,  277,  187,  283,   44,   46,   48,   98,
 /*   540 */    57,  110,  123,  225,   68,   72,   77,  187,  125,   84,
 /*   550 */   187,  276,  439,  112,  443,  141,  187,  402,  502,  286,
 /*   560 */   229,  238,  241,  307,  403,  256,  289,  147,  142,  127,
 /*   570 */   148,  149,  165,  265,  274,  280,  277,  187,  283,   44,
 /*   580 */    46,   48,  129,   57,  657,  131,  133,   68,   72,   77,
 /*   590 */   187,  135,   84,  187,  187,  658,  659,  181,  182,  187,
 /*   600 */   206,  195,  286,  229,  238,  241,  321,  418,  256,  289,
 /*   610 */   207,  237,  137,  197,  419,  268,  265,  274,  280,  277,
 /*   620 */   187,  283,   44,   46,   48,  145,   57,  199,  151,  211,
 /*   630 */    68,   72,   77,  187,  268,   84,  187,  279,  215,  214,
 /*   640 */   220,  268,  239,  224,  223,  286,  229,  238,  241,  320,
 /*   650 */   187,  256,  289,  242,  244,  248,  282,  245,  249,  265,
 /*   660 */   274,  280,  277,  285,  283,   44,   46,   48,  292,   57,
 /*   670 */   251,  299,  304,   68,   72,   77,  187,  252,   84,  187,
 /*   680 */   187,  254,  255,  258,  260,  261,  264,  270,  286,  229,
 /*   690 */   238,  241,  325,  269,  256,  289,  272,  271,  287,  288,
 /*   700 */   297,  298,  265,  274,  280,  277,  312,  283,   44,   46,
 /*   710 */    48,  318,   57,  317,  324,  331,   68,   72,   77,  332,
 /*   720 */   334,   84,  335,  338,  339,  342,  336,  343,  340,  524,
 /*   730 */   525,  286,  229,  238,  241,  327,  526,  256,  289,  527,
 /*   740 */   528,  529,  353,  530,  354,  265,  274,  280,  277,  355,
 /*   750 */   283,   44,   46,   48,  357,   57,  363,  501,  507,   68,
 /*   760 */    72,   77,  509,  389,   84,  513,  506,  508,  510,  511,
 /*   770 */   515,  512,  514,  390,  286,  229,  238,  241,  358,  392,
 /*   780 */   256,  289,  394,  391,  395,  396,  397,  406,  265,  274,
 /*   790 */   280,  277,  404,  283,   44,   46,   48,  405,   57,  502,
 /*   800 */   504,  420,   68,   72,   77,  421,  431,   84,  432,  438,
 /*   810 */   437,  434,  444,  366,  441,  447,  445,  286,  229,  238,
 /*   820 */   241,  360,  366,  256,  289,  366,  366,  366,  366,  366,
 /*   830 */   366,  265,  274,  280,  277,  366,  283,   44,   46,   48,
 /*   840 */   366,   57,  366,  366,  366,   68,   72,   77,  366,  366,
 /*   850 */    84,  366,  366,  366,  366,  366,  366,  366,  366,  366,
 /*   860 */   286,  229,  238,  241,  364,  366,  256,  289,  366,  366,
 /*   870 */   366,  366,  366,  366,  265,  274,  280,  277,  366,  283,
 /*   880 */    44,   46,   48,  366,   57,  366,  366,  366,   68,   72,
 /*   890 */    77,  366,  366,   84,  366,  366,  366,  366,  366,  366,
 /*   900 */   366,  366,  366,  286,  229,  238,  241,  366,  366,  256,
 /*   910 */   289,  366,  368,  370,  373,  377,  366,  265,  274,  280,
 /*   920 */   277,  366,  283,   44,   46,   48,  366,   57,  366,  366,
 /*   930 */   366,   68,   72,   77,  366,  366,   84,  366,  368,  370,
 /*   940 */   373,  377,  366,  366,  286,  366,  388,  229,  238,  241,
 /*   950 */   380,  289,  256,  366,  366,  366,  366,  366,  265,  274,
 /*   960 */   280,  277,  366,  283,   44,   46,   48,  469,   57,  366,
 /*   970 */   351,  366,   68,   72,   77,  366,  380,   84,   43,  366,
 /*   980 */   366,  366,  366,  366,  366,  366,   90,  366,  229,  238,
 /*   990 */   241,   26,  366,  256,  366,  366,  290,   29,   30,   31,
 /*  1000 */    32,   33,   34,   35,   36,   37,   38,   39,   40,   41,
 /*  1010 */    42,   43,   91,   92,   93,   94,   95,  205,  218,  366,
 /*  1020 */   226,  227,  236,  366,   51,  366,  366,  366,  366,  290,
 /*  1030 */    29,   30,   31,   32,   33,   34,   35,   36,   37,   38,
 /*  1040 */    39,   40,   41,   42,  384,  366,   43,  366,  366,  385,
 /*  1050 */   386,   19,  366,  366,  367,  236,  366,  366,  366,   55,
 /*  1060 */   366,  366,  366,  366,  290,   29,   30,   31,   32,   33,
 /*  1070 */    34,   35,   36,   37,   38,   39,   40,   41,   42,   43,
 /*  1080 */   383,   17,  398,  416,  366,  411,  422,  386,   19,  366,
 /*  1090 */   236,  367,   67,  366,  366,  366,  366,  290,   29,   30,
 /*  1100 */    31,   32,   33,   34,   35,   36,   37,   38,   39,   40,
 /*  1110 */    41,   42,   43,  366,  425,  398,  416,  366,  411,  366,
 /*  1120 */   386,   19,  366,  236,  367,   70,  368,  370,  373,  377,
 /*  1130 */   290,   29,   30,   31,   32,   33,   34,   35,   36,   37,
 /*  1140 */    38,   39,   40,   41,   42,  366,  366,   43,  366,  412,
 /*  1150 */   413,  414,  415,  366,  366,  474,  236,  366,  351,  366,
 /*  1160 */    75,  366,  366,  366,  380,  290,   29,   30,   31,   32,
 /*  1170 */    33,   34,   35,   36,   37,   38,   39,   40,   41,   42,
 /*  1180 */    43,  366,  366,  388,  366,  366,  366,  380,  366,  366,
 /*  1190 */   366,  236,  366,   82,  366,  366,  366,  366,  290,   29,
 /*  1200 */    30,   31,   32,   33,   34,   35,   36,   37,   38,   39,
 /*  1210 */    40,   41,   42,   43,  412,  413,  414,  415,  366,  366,
 /*  1220 */   366,  366,  366,  366,  236,  366,  294,  366,  366,  366,
 /*  1230 */   366,  290,   29,   30,   31,   32,   33,   34,   35,   36,
 /*  1240 */    37,   38,   39,   40,   41,   42,  366,  366,   43,  366,
 /*  1250 */   366,  366,  380,  366,  366,  366,  366,  236,  366,  366,
 /*  1260 */   366,  301,  366,  366,  366,  366,  290,   29,   30,   31,
 /*  1270 */    32,   33,   34,   35,   36,   37,   38,   39,   40,   41,
 /*  1280 */    42,   43,  366,  366,  366,  366,  366,  366,  366,  366,
 /*  1290 */   366,  366,  236,  366,  306,  366,  366,  366,  366,  290,
 /*  1300 */    29,   30,   31,   32,   33,   34,   35,   36,   37,   38,
 /*  1310 */    39,   40,   41,   42,   43,  366,  366,  366,  366,  366,
 /*  1320 */   366,  366,  366,  366,  366,  236,  366,  313,  366,  366,
 /*  1330 */   366,  366,  290,   29,   30,   31,   32,   33,   34,   35,
 /*  1340 */    36,   37,   38,   39,   40,   41,   42,  366,  366,   43,
 /*  1350 */   366,  366,  366,  366,  366,  366,  366,  366,  236,  366,
 /*  1360 */   366,  366,  319,  366,  366,  366,  366,  290,   29,   30,
 /*  1370 */    31,   32,   33,   34,   35,   36,   37,   38,   39,   40,
 /*  1380 */    41,   42,   43,  366,  366,  366,  366,  366,  366,  366,
 /*  1390 */   366,  366,  366,  236,  366,  326,  366,  366,  366,  366,
 /*  1400 */   290,   29,   30,   31,   32,   33,   34,   35,   36,   37,
 /*  1410 */    38,   39,   40,   41,   42,   43,  366,  366,  366,  366,
 /*  1420 */   366,  366,  366,  366,  366,  366,  236,  366,  359,  366,
 /*  1430 */   366,  366,  366,  290,   29,   30,   31,   32,   33,   34,
 /*  1440 */    35,   36,   37,   38,   39,   40,   41,   42,  366,  366,
 /*  1450 */    43,  366,  366,  366,  366,  366,  366,  366,  366,  236,
 /*  1460 */   366,  366,  366,  365,  366,  366,  366,  366,  290,   29,
 /*  1470 */    30,   31,   32,   33,   34,   35,   36,   37,   38,   39,
 /*  1480 */    40,   41,   42,   43,  366,  366,  366,  366,  366,  366,
 /*  1490 */   366,  366,  366,  366,  236,  366,  366,  366,  366,  366,
 /*  1500 */   366,   28,   29,   30,   31,   32,   33,   34,   35,   36,
 /*  1510 */    37,   38,   39,   40,   41,   42,  366,  366,  366,  366,
 /*  1520 */   366,  103,  146,  150,  144,  180,  100,  236,  139,  366,
 /*  1530 */   103,  146,  150,  144,  180,  100,  366,  139,  366,  136,
 /*  1540 */   366,  366,  366,  366,  366,  366,  366,  366,  136,  192,
 /*  1550 */   366,  103,  146,  150,  144,  180,  100,  366,  139,  366,
 /*  1560 */   291,  366,  368,  370,  373,  377,  366,  366,  158,  136,
 /*  1570 */   183,  366,  366,  366,  366,  152,  153,  158,  154,  155,
 /*  1580 */   156,  157,  366,  366,  152,  153,  366,  154,  155,  156,
 /*  1590 */   157,  473,  366,  366,  351,  366,  341,  366,  158,  366,
 /*  1600 */   380,  366,  366,  366,  366,  152,  153,  366,  154,  155,
 /*  1610 */   156,  157,  330,  366,  366,  366,  341,  366,  103,  146,
 /*  1620 */   150,  144,  180,  100,   23,  139,  344,  345,  346,  347,
 /*  1630 */   348,  349,  330,  366,  366,  366,  136,  203,  103,  146,
 /*  1640 */   150,  144,  180,  100,  356,  139,  344,  345,  346,  347,
 /*  1650 */   348,  349,  412,  413,  414,  415,  136,  366,  366,  366,
 /*  1660 */   366,  366,  366,  366,  366,  158,  366,  366,  366,  366,
 /*  1670 */   366,  366,  152,  153,  366,  154,  155,  156,  157,  366,
 /*  1680 */   366,  471,  366,  366,  426,  158,  210,  366,  366,  366,
 /*  1690 */   380,  366,  152,  153,  366,  154,  155,  156,  157,  366,
 /*  1700 */   366,  366,  366,  366,  366,  103,  146,  150,  144,  180,
 /*  1710 */   100,  366,  139,  424,  366,  427,  428,  416,  366,  411,
 /*  1720 */   385,  386,   19,  136,  366,  367,  366,  103,  146,  150,
 /*  1730 */   144,  180,  100,  366,  139,  366,  366,  366,  341,  366,
 /*  1740 */   366,  366,  366,  366,  366,  136,  366,  366,  366,  366,
 /*  1750 */   366,  366,  158,  219,  330,  366,  366,  366,  366,  152,
 /*  1760 */   153,  366,  154,  155,  156,  157,  303,  366,  344,  345,
 /*  1770 */   346,  347,  348,  349,  158,  368,  370,  373,  377,  366,
 /*  1780 */   366,  152,  153,  366,  154,  155,  156,  157,  366,  366,
 /*  1790 */   366,  366,  366,  366,  103,  146,  150,  144,  180,  100,
 /*  1800 */   366,  139,  366,  366,  470,  366,  366,  387,  366,  393,
 /*  1810 */   366,  366,  136,  380,  366,  366,  366,  366,  366,  132,
 /*  1820 */   134,   99,  114,  118,  120,  122,  124,  116,  102,  126,
 /*  1830 */   128,  130,  366,  366,  366,  366,  366,  366,  366,  366,
 /*  1840 */   366,  158,   81,  366,  366,  366,  366,  366,  152,  153,
 /*  1850 */   366,  154,  155,  156,  157,  132,  134,   99,  114,  118,
 /*  1860 */   120,  122,  124,  116,  102,  126,  128,  130,  132,  134,
 /*  1870 */    99,  114,  118,  120,  122,  124,  116,  102,  126,  128,
 /*  1880 */   130,  366,  366,  366,  366,  366,  366,  138,  366,  366,
 /*  1890 */   366,  366,  366,  366,  132,  134,   99,  114,  118,  120,
 /*  1900 */   122,  124,  116,  102,  126,  128,  130,  366,  366,   13,
 /*  1910 */    16,  423,  429,  428,  416,  240,  411,  422,  386,   19,
 /*  1920 */   366,  366,  367,  366,  412,  413,  414,  415,  366,  587,
 /*  1930 */   366,  366,  366,  366,  366,  366,  132,  134,   99,  114,
 /*  1940 */   118,  120,  122,  124,  116,  102,  126,  128,  130,  366,
 /*  1950 */   366,  366,  366,   15,  366,  366,  399,  366,  393,  293,
 /*  1960 */   366,  366,  380,  366,  366,  366,  132,  134,   99,  114,
 /*  1970 */   118,  120,  122,  124,  116,  102,  126,  128,  130,  366,
 /*  1980 */   412,  413,  414,  415,  368,  370,  373,  377,  366,  300,
 /*  1990 */   366,  366,  366,  366,  366,  366,  132,  134,   99,  114,
 /*  2000 */   118,  120,  122,  124,  116,  102,  126,  128,  130,  466,
 /*  2010 */   366,  366,  399,  472,  393,  366,  351,  366,  380,  305,
 /*  2020 */   366,  366,  380,  366,  366,  366,  132,  134,   99,  114,
 /*  2030 */   118,  120,  122,  124,  116,  102,  126,  128,  130,  366,
 /*  2040 */   366,  366,  366,  366,  366,  366,  366,  366,  366,  671,
 /*  2050 */   366,  366,  366,  366,  366,  366,  132,  134,   99,  114,
 /*  2060 */   118,  120,  122,  124,  116,  102,  126,  128,  130,   99,
 /*  2070 */   114,  118,  120,  122,  124,  116,  102,  126,  128,  130,
 /*  2080 */   366,  366,  366,  366,  366,  433,   16,  423,  429,  428,
 /*  2090 */   416,  366,  411,  422,  386,   19,  366,  366,  367,  366,
 /*  2100 */   412,  413,  414,  415,  366,  366,  366,  366,  366,  366,
 /*  2110 */   366,  366,  366,  366,  366,  440,   16,  423,  429,  428,
 /*  2120 */   416,  366,  411,  422,  386,   19,  366,  366,  367,  467,
 /*  2130 */   366,  366,  399,  366,  393,  366,  366,  366,  380,  366,
 /*  2140 */   412,  413,  414,  415,  366,  446,   16,  423,  429,  428,
 /*  2150 */   416,  366,  411,  422,  386,   19,  366,  366,  367,  366,
 /*  2160 */   412,  413,  414,  415,  368,  370,  373,  377,  366,  435,
 /*  2170 */   366,  366,  399,  366,  393,  366,  366,  366,  380,  366,
 /*  2180 */   412,  413,  414,  415,  366,  366,  366,  366,  366,  442,
 /*  2190 */   366,  366,  399,  468,  393,  366,  351,  366,  380,  366,
 /*  2200 */   366,  366,  380,  366,  366,  366,  366,  366,  366,  448,
 /*  2210 */   366,  366,  399,  366,  393,  366,  366,  366,  380,
};
static YYCODETYPE yy_lookahead[] = {
 /*     0 */    26,   16,   17,   18,   30,   73,   74,   33,   76,   77,
 /*    10 */    78,   79,    0,   37,   40,   41,   42,   43,  123,   45,
 /*    20 */    46,   47,   48,  123,   50,   34,  131,   26,   54,   55,
 /*    30 */    56,  131,  137,   59,   81,   82,   83,   84,   85,   86,
 /*    40 */    87,   65,   66,   26,   70,   71,   72,   30,   26,   75,
 /*    50 */    33,   60,   61,   62,   63,   64,   27,   40,   41,   42,
 /*    60 */    43,  123,   45,   46,   47,   48,    0,   50,   67,  131,
 /*    70 */    26,   54,   55,   56,   73,   74,   59,   76,   77,   78,
 /*    80 */    79,   29,   94,   31,  121,  122,   26,   70,   71,   72,
 /*    90 */    30,   25,   75,   33,   28,   73,   74,   26,   32,   33,
 /*   100 */    40,   41,   42,   43,  123,   45,   46,   47,   48,  128,
 /*   110 */    50,   26,  131,   26,   54,   55,   56,   73,   74,   59,
 /*   120 */    76,   77,   78,   79,  136,    5,  138,  139,  140,   26,
 /*   130 */    70,   71,   72,   30,   25,   75,   33,   28,  102,  103,
 /*   140 */   104,   32,   33,   40,   41,   42,   43,   30,   45,   46,
 /*   150 */    47,   48,   36,   50,  124,  125,   26,   54,   55,   56,
 /*   160 */    73,   74,   59,   76,   77,   78,   79,   29,    1,    2,
 /*   170 */     3,    4,   26,   70,   71,   72,   30,   57,   75,   33,
 /*   180 */    99,   34,   94,  102,  103,  104,   40,   41,   42,   43,
 /*   190 */   123,   45,   46,   47,   48,  123,   50,   37,  131,  132,
 /*   200 */    54,   55,   56,  131,  137,   59,   39,   60,   61,   62,
 /*   210 */    63,   64,  123,   66,   27,   26,   70,   71,   72,   30,
 /*   220 */   131,   75,   33,   26,    1,    2,    3,    4,  140,   40,
 /*   230 */    41,   42,   43,  123,   45,   46,   47,   48,  128,   50,
 /*   240 */     2,  131,    4,   54,   55,   56,   97,   98,   59,   26,
 /*   250 */   101,    1,    2,    3,    4,  123,   65,   66,   26,   70,
 /*   260 */    71,   72,   30,  131,   75,   33,   84,   85,   86,   87,
 /*   270 */    73,   74,   40,   41,   42,   43,   26,   45,   46,   47,
 /*   280 */    48,  123,   50,  133,  134,   27,   54,   55,   56,  131,
 /*   290 */   132,   59,  123,   94,   23,  137,   29,   26,  129,  130,
 /*   300 */   131,   26,   70,   71,   72,   30,   99,   75,   33,  102,
 /*   310 */   103,  104,  133,  134,    5,   40,   41,   42,   43,   30,
 /*   320 */    45,   46,   47,   48,  122,   50,  133,  134,   49,   54,
 /*   330 */    55,   56,  120,   30,   59,  123,   27,  138,  139,  140,
 /*   340 */    51,   29,   53,  131,   26,   70,   71,   72,   30,  123,
 /*   350 */    75,   33,    5,  127,   51,   29,   53,  131,   40,   41,
 /*   360 */    42,   43,  123,   45,   46,   47,   48,  123,   50,  130,
 /*   370 */   131,  123,   54,   55,   56,  131,  132,   59,   26,  131,
 /*   380 */     5,  137,   94,    1,    2,   38,    4,   26,   70,   71,
 /*   390 */    72,   30,  123,   75,   33,   52,  127,  133,  134,    5,
 /*   400 */   131,   40,   41,   42,   43,  123,   45,   46,   47,   48,
 /*   410 */     5,   50,   29,  131,  132,   54,   55,   56,   36,  137,
 /*   420 */    59,   27,    5,    5,  120,   73,   74,  123,  133,  134,
 /*   430 */    26,   70,   71,   72,   30,  131,   75,   33,    5,    5,
 /*   440 */    29,   26,   67,   38,   40,   41,   42,   43,    3,   45,
 /*   450 */    46,   47,   48,  123,   50,   38,   38,  127,   54,   55,
 /*   460 */    56,  131,   93,   59,   95,   98,   36,   98,  101,   39,
 /*   470 */   101,   38,   38,   26,   70,   71,   72,   30,  123,   75,
 /*   480 */    33,   36,  127,    5,   39,  125,  131,   40,   41,   42,
 /*   490 */    43,    3,   45,   46,   47,   48,  120,   50,   26,  123,
 /*   500 */   123,   54,   55,   56,  127,   27,   59,  131,  131,   36,
 /*   510 */   126,   26,   39,  123,  135,   66,   26,   70,   71,   72,
 /*   520 */    30,  131,   75,   33,   36,    3,  123,   39,   67,    5,
 /*   530 */    40,   41,   42,   43,  131,   45,   46,   47,   48,  123,
 /*   540 */    50,   67,  123,  127,   54,   55,   56,  131,  123,   59,
 /*   550 */   131,   27,   29,   67,   31,   26,  131,   27,   36,   26,
 /*   560 */    70,   71,   72,   30,   34,   75,   33,   26,   37,  123,
 /*   570 */     5,  135,   52,   40,   41,   42,   43,  131,   45,   46,
 /*   580 */    47,   48,  123,   50,   52,  123,  123,   54,   55,   56,
 /*   590 */   131,  123,   59,  131,  131,   52,   52,   26,   37,  131,
 /*   600 */    26,   67,   26,   70,   71,   72,   30,   27,   75,   33,
 /*   610 */   126,   27,  123,   67,   34,    5,   40,   41,   42,   43,
 /*   620 */   131,   45,   46,   47,   48,  123,   50,   67,  123,  126,
 /*   630 */    54,   55,   56,  131,    5,   59,  131,   27,  126,   67,
 /*   640 */   126,    5,  123,  126,   67,   26,   70,   71,   72,   30,
 /*   650 */   131,   75,   33,   26,   26,   67,   27,   27,   27,   40,
 /*   660 */    41,   42,   43,   27,   45,   46,   47,   48,  123,   50,
 /*   670 */    67,  123,  123,   54,   55,   56,  131,   27,   59,  131,
 /*   680 */   131,   67,   27,   27,   26,   37,   27,   26,   26,   70,
 /*   690 */    71,   72,   30,  134,   75,   33,   94,   34,   65,   26,
 /*   700 */    26,   57,   40,   41,   42,   43,   52,   45,   46,   47,
 /*   710 */    48,   29,   50,   49,   29,   34,   54,   55,   56,   94,
 /*   720 */    26,   59,   34,   26,   34,   26,   94,   11,   94,   26,
 /*   730 */    26,   26,   70,   71,   72,   30,   26,   75,   33,   26,
 /*   740 */    26,   26,   36,   26,   26,   40,   41,   42,   43,   37,
 /*   750 */    45,   46,   47,   48,   29,   50,   29,   36,   36,   54,
 /*   760 */    55,   56,   36,   26,   59,   36,   36,   36,   36,   36,
 /*   770 */    36,   36,   36,   34,   26,   70,   71,   72,   30,   27,
 /*   780 */    75,   33,   26,   94,   34,   94,   27,   95,   40,   41,
 /*   790 */    42,   43,   94,   45,   46,   47,   48,   27,   50,   36,
 /*   800 */    36,   94,   54,   55,   56,   27,   26,   59,   29,   26,
 /*   810 */    28,   30,   26,  141,   30,   30,   29,   26,   70,   71,
 /*   820 */    72,   30,  141,   75,   33,  141,  141,  141,  141,  141,
 /*   830 */   141,   40,   41,   42,   43,  141,   45,   46,   47,   48,
 /*   840 */   141,   50,  141,  141,  141,   54,   55,   56,  141,  141,
 /*   850 */    59,  141,  141,  141,  141,  141,  141,  141,  141,  141,
 /*   860 */    26,   70,   71,   72,   30,  141,   75,   33,  141,  141,
 /*   870 */   141,  141,  141,  141,   40,   41,   42,   43,  141,   45,
 /*   880 */    46,   47,   48,  141,   50,  141,  141,  141,   54,   55,
 /*   890 */    56,  141,  141,   59,  141,  141,  141,  141,  141,  141,
 /*   900 */   141,  141,  141,   26,   70,   71,   72,   30,  141,   75,
 /*   910 */    33,  141,    1,    2,    3,    4,  141,   40,   41,   42,
 /*   920 */    43,  141,   45,   46,   47,   48,  141,   50,  141,  141,
 /*   930 */   141,   54,   55,   56,  141,  141,   59,  141,    1,    2,
 /*   940 */     3,    4,  141,  141,   26,  141,   35,   70,   71,   72,
 /*   950 */    39,   33,   75,  141,  141,  141,  141,  141,   40,   41,
 /*   960 */    42,   43,  141,   45,   46,   47,   48,   30,   50,  141,
 /*   970 */    33,  141,   54,   55,   56,  141,   39,   59,   87,  141,
 /*   980 */   141,  141,  141,  141,  141,  141,   34,  141,   70,   71,
 /*   990 */    72,  100,  141,   75,  141,  141,  105,  106,  107,  108,
 /*  1000 */   109,  110,  111,  112,  113,  114,  115,  116,  117,  118,
 /*  1010 */   119,   87,   60,   61,   62,   63,   64,   65,   66,  141,
 /*  1020 */    68,   69,  131,  141,  100,  141,  141,  141,  141,  105,
 /*  1030 */   106,  107,  108,  109,  110,  111,  112,  113,  114,  115,
 /*  1040 */   116,  117,  118,  119,   91,  141,   87,  141,  141,   96,
 /*  1050 */    97,   98,  141,  141,  101,  131,  141,  141,  141,  100,
 /*  1060 */   141,  141,  141,  141,  105,  106,  107,  108,  109,  110,
 /*  1070 */   111,  112,  113,  114,  115,  116,  117,  118,  119,   87,
 /*  1080 */    90,   91,   92,   93,  141,   95,   96,   97,   98,  141,
 /*  1090 */   131,  101,  100,  141,  141,  141,  141,  105,  106,  107,
 /*  1100 */   108,  109,  110,  111,  112,  113,  114,  115,  116,  117,
 /*  1110 */   118,  119,   87,  141,   91,   92,   93,  141,   95,  141,
 /*  1120 */    97,   98,  141,  131,  101,  100,    1,    2,    3,    4,
 /*  1130 */   105,  106,  107,  108,  109,  110,  111,  112,  113,  114,
 /*  1140 */   115,  116,  117,  118,  119,  141,  141,   87,  141,    1,
 /*  1150 */     2,    3,    4,  141,  141,   30,  131,  141,   33,  141,
 /*  1160 */   100,  141,  141,  141,   39,  105,  106,  107,  108,  109,
 /*  1170 */   110,  111,  112,  113,  114,  115,  116,  117,  118,  119,
 /*  1180 */    87,  141,  141,   35,  141,  141,  141,   39,  141,  141,
 /*  1190 */   141,  131,  141,  100,  141,  141,  141,  141,  105,  106,
 /*  1200 */   107,  108,  109,  110,  111,  112,  113,  114,  115,  116,
 /*  1210 */   117,  118,  119,   87,    1,    2,    3,    4,  141,  141,
 /*  1220 */   141,  141,  141,  141,  131,  141,  100,  141,  141,  141,
 /*  1230 */   141,  105,  106,  107,  108,  109,  110,  111,  112,  113,
 /*  1240 */   114,  115,  116,  117,  118,  119,  141,  141,   87,  141,
 /*  1250 */   141,  141,   39,  141,  141,  141,  141,  131,  141,  141,
 /*  1260 */   141,  100,  141,  141,  141,  141,  105,  106,  107,  108,
 /*  1270 */   109,  110,  111,  112,  113,  114,  115,  116,  117,  118,
 /*  1280 */   119,   87,  141,  141,  141,  141,  141,  141,  141,  141,
 /*  1290 */   141,  141,  131,  141,  100,  141,  141,  141,  141,  105,
 /*  1300 */   106,  107,  108,  109,  110,  111,  112,  113,  114,  115,
 /*  1310 */   116,  117,  118,  119,   87,  141,  141,  141,  141,  141,
 /*  1320 */   141,  141,  141,  141,  141,  131,  141,  100,  141,  141,
 /*  1330 */   141,  141,  105,  106,  107,  108,  109,  110,  111,  112,
 /*  1340 */   113,  114,  115,  116,  117,  118,  119,  141,  141,   87,
 /*  1350 */   141,  141,  141,  141,  141,  141,  141,  141,  131,  141,
 /*  1360 */   141,  141,  100,  141,  141,  141,  141,  105,  106,  107,
 /*  1370 */   108,  109,  110,  111,  112,  113,  114,  115,  116,  117,
 /*  1380 */   118,  119,   87,  141,  141,  141,  141,  141,  141,  141,
 /*  1390 */   141,  141,  141,  131,  141,  100,  141,  141,  141,  141,
 /*  1400 */   105,  106,  107,  108,  109,  110,  111,  112,  113,  114,
 /*  1410 */   115,  116,  117,  118,  119,   87,  141,  141,  141,  141,
 /*  1420 */   141,  141,  141,  141,  141,  141,  131,  141,  100,  141,
 /*  1430 */   141,  141,  141,  105,  106,  107,  108,  109,  110,  111,
 /*  1440 */   112,  113,  114,  115,  116,  117,  118,  119,  141,  141,
 /*  1450 */    87,  141,  141,  141,  141,  141,  141,  141,  141,  131,
 /*  1460 */   141,  141,  141,  100,  141,  141,  141,  141,  105,  106,
 /*  1470 */   107,  108,  109,  110,  111,  112,  113,  114,  115,  116,
 /*  1480 */   117,  118,  119,   87,  141,  141,  141,  141,  141,  141,
 /*  1490 */   141,  141,  141,  141,  131,  141,  141,  141,  141,  141,
 /*  1500 */   141,  105,  106,  107,  108,  109,  110,  111,  112,  113,
 /*  1510 */   114,  115,  116,  117,  118,  119,  141,  141,  141,  141,
 /*  1520 */   141,   19,   20,   21,   22,   23,   24,  131,   26,  141,
 /*  1530 */    19,   20,   21,   22,   23,   24,  141,   26,  141,   37,
 /*  1540 */   141,  141,  141,  141,  141,  141,  141,  141,   37,   38,
 /*  1550 */   141,   19,   20,   21,   22,   23,   24,  141,   26,  141,
 /*  1560 */    58,  141,    1,    2,    3,    4,  141,  141,   66,   37,
 /*  1570 */    38,  141,  141,  141,  141,   73,   74,   66,   76,   77,
 /*  1580 */    78,   79,  141,  141,   73,   74,  141,   76,   77,   78,
 /*  1590 */    79,   30,  141,  141,   33,  141,   10,  141,   66,  141,
 /*  1600 */    39,  141,  141,  141,  141,   73,   74,  141,   76,   77,
 /*  1610 */    78,   79,   26,  141,  141,  141,   10,  141,   19,   20,
 /*  1620 */    21,   22,   23,   24,   38,   26,   40,   41,   42,   43,
 /*  1630 */    44,   45,   26,  141,  141,  141,   37,   38,   19,   20,
 /*  1640 */    21,   22,   23,   24,   38,   26,   40,   41,   42,   43,
 /*  1650 */    44,   45,    1,    2,    3,    4,   37,  141,  141,  141,
 /*  1660 */   141,  141,  141,  141,  141,   66,  141,  141,  141,  141,
 /*  1670 */   141,  141,   73,   74,  141,   76,   77,   78,   79,  141,
 /*  1680 */   141,   30,  141,  141,   33,   66,   67,  141,  141,  141,
 /*  1690 */    39,  141,   73,   74,  141,   76,   77,   78,   79,  141,
 /*  1700 */   141,  141,  141,  141,  141,   19,   20,   21,   22,   23,
 /*  1710 */    24,  141,   26,   89,  141,   91,   92,   93,  141,   95,
 /*  1720 */    96,   97,   98,   37,  141,  101,  141,   19,   20,   21,
 /*  1730 */    22,   23,   24,  141,   26,  141,  141,  141,   10,  141,
 /*  1740 */   141,  141,  141,  141,  141,   37,  141,  141,  141,  141,
 /*  1750 */   141,  141,   66,   67,   26,  141,  141,  141,  141,   73,
 /*  1760 */    74,  141,   76,   77,   78,   79,   58,  141,   40,   41,
 /*  1770 */    42,   43,   44,   45,   66,    1,    2,    3,    4,  141,
 /*  1780 */   141,   73,   74,  141,   76,   77,   78,   79,  141,  141,
 /*  1790 */   141,  141,  141,  141,   19,   20,   21,   22,   23,   24,
 /*  1800 */   141,   26,  141,  141,   30,  141,  141,   33,  141,   35,
 /*  1810 */   141,  141,   37,   39,  141,  141,  141,  141,  141,    6,
 /*  1820 */     7,    8,    9,   10,   11,   12,   13,   14,   15,   16,
 /*  1830 */    17,   18,  141,  141,  141,  141,  141,  141,  141,  141,
 /*  1840 */   141,   66,   29,  141,  141,  141,  141,  141,   73,   74,
 /*  1850 */   141,   76,   77,   78,   79,    6,    7,    8,    9,   10,
 /*  1860 */    11,   12,   13,   14,   15,   16,   17,   18,    6,    7,
 /*  1870 */     8,    9,   10,   11,   12,   13,   14,   15,   16,   17,
 /*  1880 */    18,  141,  141,  141,  141,  141,  141,   38,  141,  141,
 /*  1890 */   141,  141,  141,  141,    6,    7,    8,    9,   10,   11,
 /*  1900 */    12,   13,   14,   15,   16,   17,   18,  141,  141,   88,
 /*  1910 */    89,   90,   91,   92,   93,   27,   95,   96,   97,   98,
 /*  1920 */   141,  141,  101,  141,    1,    2,    3,    4,  141,   67,
 /*  1930 */   141,  141,  141,  141,  141,  141,    6,    7,    8,    9,
 /*  1940 */    10,   11,   12,   13,   14,   15,   16,   17,   18,  141,
 /*  1950 */   141,  141,  141,   30,  141,  141,   33,  141,   35,   29,
 /*  1960 */   141,  141,   39,  141,  141,  141,    6,    7,    8,    9,
 /*  1970 */    10,   11,   12,   13,   14,   15,   16,   17,   18,  141,
 /*  1980 */     1,    2,    3,    4,    1,    2,    3,    4,  141,   29,
 /*  1990 */   141,  141,  141,  141,  141,  141,    6,    7,    8,    9,
 /*  2000 */    10,   11,   12,   13,   14,   15,   16,   17,   18,   30,
 /*  2010 */   141,  141,   33,   30,   35,  141,   33,  141,   39,   29,
 /*  2020 */   141,  141,   39,  141,  141,  141,    6,    7,    8,    9,
 /*  2030 */    10,   11,   12,   13,   14,   15,   16,   17,   18,  141,
 /*  2040 */   141,  141,  141,  141,  141,  141,  141,  141,  141,   29,
 /*  2050 */   141,  141,  141,  141,  141,  141,    6,    7,    8,    9,
 /*  2060 */    10,   11,   12,   13,   14,   15,   16,   17,   18,    8,
 /*  2070 */     9,   10,   11,   12,   13,   14,   15,   16,   17,   18,
 /*  2080 */   141,  141,  141,  141,  141,   88,   89,   90,   91,   92,
 /*  2090 */    93,  141,   95,   96,   97,   98,  141,  141,  101,  141,
 /*  2100 */     1,    2,    3,    4,  141,  141,  141,  141,  141,  141,
 /*  2110 */   141,  141,  141,  141,  141,   88,   89,   90,   91,   92,
 /*  2120 */    93,  141,   95,   96,   97,   98,  141,  141,  101,   30,
 /*  2130 */   141,  141,   33,  141,   35,  141,  141,  141,   39,  141,
 /*  2140 */     1,    2,    3,    4,  141,   88,   89,   90,   91,   92,
 /*  2150 */    93,  141,   95,   96,   97,   98,  141,  141,  101,  141,
 /*  2160 */     1,    2,    3,    4,    1,    2,    3,    4,  141,   30,
 /*  2170 */   141,  141,   33,  141,   35,  141,  141,  141,   39,  141,
 /*  2180 */     1,    2,    3,    4,  141,  141,  141,  141,  141,   30,
 /*  2190 */   141,  141,   33,   30,   35,  141,   33,  141,   39,  141,
 /*  2200 */   141,  141,   39,  141,  141,  141,  141,  141,  141,   30,
 /*  2210 */   141,  141,   33,  141,   35,  141,  141,  141,   39,
};
#define YY_SHIFT_USE_DFLT (-69)
static short yy_shift_ofst[] = {
 /*     0 */   109,   12,   66,  -69,  -69,  -69,  -69,   71,   29,  -69,
 /*    10 */    85,   52, 1923,  117,  -69,  -69, 1979,  937,  -69,  116,
 /*    20 */   130,  160, 1586,  138,  -26,  -69,   17,  -69,  -69,  -69,
 /*    30 */   -69,  -69,  -69,  -69,  -69,  -69,  -69,  -69,  -69,  -69,
 /*    40 */   -69,  -69,  -69,  -69,  187,  -69,  258,  -69, 1775,  267,
 /*    50 */    60,  103,  279,  312,  146,  189,  -69, 1775,  326,  289,
 /*    60 */   -69,  303,  -69,  -69,  -68,  343,  918,  918,  383,  232,
 /*    70 */   275,  -69, 1775,  411,  318,  361,  -69,  415,  120, 1502,
 /*    80 */  1813,  918,  404,  -69,  472,  309,  -69,  472,  -69,  952,
 /*    90 */   -69,  -69,  -69,  -69,  -69,  -69, 1775,  -69, 2050, 1775,
 /*   100 */  1775,  -69, 1775,  485,  -69,  449,   22,  461,  -69,  474,
 /*   110 */   -69,  486,  -69,  -15, 1775,  -15, 1775,  -15, 1775,  -15,
 /*   120 */  1775,  -15, 1775,  -15, 1775,  -15, 1775,  -69, 1775,  -69,
 /*   130 */  1775,  -69, 1775, 2061, 1775, 2061, 1775, 1849,  -69,  -24,
 /*   140 */   529,  531, 1511, 2050, 1775,  -69,  541,  565,  485,  -69,
 /*   150 */  1775,  -69,  -69,  -69,  -69,  -69,  -69,  -69,    1,  -69,
 /*   160 */   375,  -69,   44,  -69,  520,   87,  -69,  -69,  -69,  -69,
 /*   170 */   -69,  -69,  -69,  -69,  -69,  -69,  532,  543,  544,  -69,
 /*   180 */   571,  561, 1532,  -69,  347,  -69, 1775,  -69,  -69,  -69,
 /*   190 */   405,  -69,  -69,  197,  534,  -69,  546,  -69,  560,  -69,
 /*   200 */  1599,  417,  -69,  -69,  -15,  574,  147, 1775,  -69, 1619,
 /*   210 */    -9, 1775,  -69,  572,   -9, 1775,  -69, 1862, 1686,   -9,
 /*   220 */  1775,  -69,  577,   -9, 1775,  -69,  -69,  -69,  -69, 1775,
 /*   230 */   394,  -69, 1775,  -69, 2050,  -69,  584,  -69, 1775, 1888,
 /*   240 */   -69,  627,  191,  628,  630,  -69,  352,  588,  631,  -69,
 /*   250 */   603,  650,  -69,  614,  655,  -69,  271,  656,  -69,  658,
 /*   260 */   648, 1775,  418,  659,  -69,  661,  478,  -69,  661,  -69,
 /*   270 */   663,  -68,  -69,  -69,  661,  524,  -69,  661,  610,  -69,
 /*   280 */   661,  629,  -69,  661,  636,  -69,  633,  673,  531,  -69,
 /*   290 */   -69, 1775, 1930,  918,  447,  -69,  674,  644, 1708, 1960,
 /*   300 */   918,  490,  -69, 1775, 1990,  918,  533,  -69,  -69, 2020,
 /*   310 */   -69,  654,  918,  918,  -69,  -69,  664,  682,  576,  619,
 /*   320 */   -69,  -69,  433,  685,  662,  -69,  705,  -69, 1728,  -69,
 /*   330 */   681,  -68,  -69,  694,  688,  -68,  -69,  697,  690,  -68,
 /*   340 */   -69,  699,  716,  703,  704,  710,  713,  714,  715,  717,
 /*   350 */   -69,  167,  706,  718,  712, 1606,  725,  748,  -69,  791,
 /*   360 */   -69,  434,  727,  834,  -69,  877,  -69,  721,  522,  722,
 /*   370 */   445,  726,  729,  382,  730,  731,  732,  488,  733,  734,
 /*   380 */   238,  735,  736, 1774, 1561,  -69,  -69,  911,  737,  739,
 /*   390 */   -68,  752,  -69,  756,  750,  -68,  759,  -69,  -69, 1148,
 /*   400 */   223,  530,  -69,  -68,  770,  -69,  -69,  -69,  -69,  -69,
 /*   410 */   -69,  -69,  763,  430,  764,  473,  250,  580,  -69,  -68,
 /*   420 */   778,  -69,  -69, 2099, 1651, 1125, 1213, 1983,  -69, 2163,
 /*   430 */   780,  779, 2139,  781,  -69,  -69,  782,  783,  523, 2159,
 /*   440 */   784,  -69,  -69,  786,  787, 2179,  785,  -69,  -69,  -69,
};
#define YY_REDUCE_USE_DFLT (-106)
static short yy_reduce_ofst[] = {
 /*     0 */   -47, -106,  182, -106, -106, -106, -106, -106, -106, -106,
 /*    10 */  -106, -106, 1821, -106, -106, -106,  990,  149, -106, -106,
 /*    20 */  -106, -106,   81, -106,  891, -106, 1396, -106, -106, -106,
 /*    30 */  -106, -106, -106, -106, -106, -106, -106, -106, -106, -106,
 /*    40 */  -106, -106, -106, -106, -106, -106, -106, -106,  212, -106,
 /*    50 */   924, 1396, -106, -106,  959, 1396, -106,  304, -106,  -37,
 /*    60 */  -106,  202, -106, -106,  288, -106,  992, 1396, -106, 1025,
 /*    70 */  1396, -106,  376, -106, 1060, 1396, -106, -106, -106, -100,
 /*    80 */  -106, 1093, 1396, -106,   30, -106, -106,  360, -106,  384,
 /*    90 */  -106, -106, -106, -106, -106, -106,  226, -106, -106,  -62,
 /*   100 */    72, -106,   89,  379, -106, -106, -106, -106, -106, -106,
 /*   110 */  -106, -106, -106, -106,  132, -106,  248, -106,  390, -106,
 /*   120 */   403, -106,  419, -106,  425, -106,  446, -106,  459, -106,
 /*   130 */   462, -106,  463, -106,  468, -106,  489, -106, -106, -106,
 /*   140 */  -106, -106,   67, -106,  502, -106, -106, -106,  436, -106,
 /*   150 */   505, -106, -106, -106, -106, -106, -106, -106,  -12, -106,
 /*   160 */  -106, -106,  199, -106, -106,   88, -106, -106, -106, -106,
 /*   170 */  -106, -106, -106, -106, -106, -106, -106, -106, -106, -106,
 /*   180 */  -106, -106,  158, -106, -106, -106, -105, -106, -106, -106,
 /*   190 */  -106, -106, -106, -106, -106, -106, -106, -106, -106, -106,
 /*   200 */   244, -106, -106, -106, -106, -106,  484,  269, -106,  -19,
 /*   210 */   503,  330, -106, -106,  512,  355, -106, -106,  110,  514,
 /*   220 */   377, -106, -106,  517,  416, -106, -106, -106, -106,  169,
 /*   230 */  -106, -106,  239, -106, -106, -106, -106, -106,  519, -106,
 /*   240 */  -106, -106, -106, -106, -106, -106, -106, -106, -106, -106,
 /*   250 */  -106, -106, -106, -106, -106, -106, -106, -106, -106, -106,
 /*   260 */  -106,  282, -106, -106, -106,  150, -106, -106,  559, -106,
 /*   270 */  -106,  602, -106, -106,  179, -106, -106,  193, -106, -106,
 /*   280 */   264, -106, -106,  295, -106, -106, -106, -106, -106, -106,
 /*   290 */  -106,  545, -106, 1126, 1396, -106, -106, -106,  548, -106,
 /*   300 */  1161, 1396, -106,  549, -106, 1194, 1396, -106, -106, -106,
 /*   310 */  -106, -106, 1227, 1396, -106, -106, -106, -106, 1262, 1396,
 /*   320 */  -106, -106, -106, -106, 1295, -106, 1396, -106,   36, -106,
 /*   330 */  -106,  625, -106, -106, -106,  632, -106, -106, -106,  634,
 /*   340 */  -106, -106, -106, -106, -106, -106, -106, -106, -106, -106,
 /*   350 */  -106,  367, -106, -106, -106,  207, -106, 1328, -106, 1396,
 /*   360 */  -106, -106, -106, 1363, -106, 1396, -106, -106, -106, -106,
 /*   370 */  -106, -106, -106, -106, -106, -106, -106, -106, -106, -106,
 /*   380 */  -106, -106, -106,  953,  149, -106, -106,  367, -106, -106,
 /*   390 */   689, -106, -106, -106, -106,  691, -106, -106, -106,  369,
 /*   400 */   692, -106, -106,  698, -106, -106, -106, -106, -106, -106,
 /*   410 */  -106, -106, -106, -106, -106, -106,  692, -106, -106,  707,
 /*   420 */  -106, -106, -106, 1624, 1023,  149,  369,  149, -106,  149,
 /*   430 */  -106, -106, 1997, -106, -106, -106, -106, -106, -106, 2027,
 /*   440 */  -106, -106, -106, -106, -106, 2057, -106, -106, -106, -106,
};
static YYACTIONTYPE yy_default[] = {
 /*     0 */   673,  673,  673,  452,  454,  455,  456,  673,  673,  457,
 /*    10 */   673,  673,  673,  673,  458,  462,  673,  673,  489,  673,
 /*    20 */   673,  673,  673,  673,  673,  493,  673,  495,  531,  533,
 /*    30 */   534,  535,  536,  537,  538,  539,  540,  541,  542,  543,
 /*    40 */   544,  545,  546,  547,  673,  548,  673,  549,  673,  673,
 /*    50 */   673,  673,  552,  673,  673,  673,  553,  673,  673,  673,
 /*    60 */   556,  673,  557,  558,  673,  673,  673,  560,  673,  673,
 /*    70 */   673,  563,  673,  673,  673,  673,  565,  673,  673,  673,
 /*    80 */   673,  673,  673,  566,  673,  673,  570,  673,  571,  673,
 /*    90 */   573,  574,  575,  576,  577,  578,  673,  579,  609,  673,
 /*   100 */   673,  610,  673,  673,  611,  673,  673,  673,  668,  673,
 /*   110 */   669,  673,  670,  613,  673,  614,  673,  615,  673,  616,
 /*   120 */   673,  617,  673,  618,  673,  619,  673,  625,  673,  626,
 /*   130 */   673,  627,  673,  628,  673,  629,  673,  673,  620,  633,
 /*   140 */   673,  621,  673,  652,  673,  630,  673,  673,  673,  631,
 /*   150 */   673,  632,  634,  635,  636,  637,  638,  639,  673,  640,
 /*   160 */   673,  641,  673,  653,  673,  673,  655,  660,  661,  662,
 /*   170 */   663,  664,  665,  666,  667,  656,  661,  663,  662,  654,
 /*   180 */   673,  642,  673,  643,  673,  644,  673,  647,  650,  651,
 /*   190 */   673,  648,  649,  673,  673,  622,  673,  623,  673,  624,
 /*   200 */   673,  673,  645,  646,  612,  673,  673,  673,  580,  673,
 /*   210 */   673,  673,  581,  673,  673,  673,  582,  673,  673,  673,
 /*   220 */   673,  583,  673,  673,  673,  584,  585,  586,  572,  673,
 /*   230 */   673,  588,  673,  589,  591,  590,  673,  592,  673,  673,
 /*   240 */   593,  673,  673,  673,  673,  594,  673,  673,  673,  595,
 /*   250 */   673,  673,  596,  673,  673,  597,  673,  673,  598,  673,
 /*   260 */   673,  673,  673,  673,  599,  673,  673,  600,  673,  605,
 /*   270 */   607,  673,  608,  606,  673,  673,  601,  673,  673,  602,
 /*   280 */   673,  673,  603,  673,  673,  604,  673,  673,  673,  672,
 /*   290 */   532,  673,  673,  673,  673,  567,  673,  673,  673,  673,
 /*   300 */   673,  673,  568,  673,  673,  673,  673,  569,  564,  673,
 /*   310 */   562,  673,  673,  561,  559,  554,  550,  673,  673,  673,
 /*   320 */   555,  551,  673,  673,  673,  494,  673,  496,  673,  516,
 /*   330 */   518,  673,  521,  673,  519,  673,  522,  673,  520,  673,
 /*   340 */   523,  673,  673,  673,  673,  673,  673,  673,  673,  673,
 /*   350 */   517,  673,  673,  673,  673,  673,  673,  673,  497,  673,
 /*   360 */   499,  673,  673,  673,  498,  673,  500,  673,  673,  673,
 /*   370 */   673,  673,  673,  673,  673,  673,  673,  673,  673,  673,
 /*   380 */   673,  673,  673,  673,  673,  487,  490,  673,  673,  673,
 /*   390 */   673,  673,  491,  673,  673,  673,  673,  492,  475,  673,
 /*   400 */   673,  673,  477,  673,  673,  479,  481,  483,  484,  485,
 /*   410 */   486,  482,  483,  484,  485,  486,  673,  673,  478,  673,
 /*   420 */   673,  480,  488,  673,  673,  673,  673,  673,  476,  673,
 /*   430 */   673,  673,  673,  673,  459,  463,  673,  673,  673,  673,
 /*   440 */   673,  460,  464,  673,  673,  673,  673,  461,  465,  453,
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
  "DOUBLE",        "NULL",          "TRUE",          "FALSE",       
  "error",         "program",       "xx_language",   "xx_top_statement_list",
  "xx_top_statement",  "xx_namespace_def",  "xx_class_def",  "xx_comment",  
  "xx_class_definition",  "xx_class_properties_definition",  "xx_class_consts_definition",  "xx_class_methods_definition",
  "xx_class_property_definition",  "xx_property_visibility_list",  "xx_literal_expr",  "xx_property_visibility",
  "xx_class_const_definition",  "xx_class_method_definition",  "xx_method_visibility_list",  "xx_parameter_list",
  "xx_statement_list",  "xx_method_visibility",  "xx_parameter",  "xx_parameter_type",
  "xx_parameter_cast",  "xx_statement",  "xx_let_statement",  "xx_if_statement",
  "xx_loop_statement",  "xx_echo_statement",  "xx_return_statement",  "xx_mcall_statement",
  "xx_unset_statement",  "xx_throw_statement",  "xx_declare_statement",  "xx_break_statement",
  "xx_continue_statement",  "xx_while_statement",  "xx_switch_statement",  "xx_for_statement",
  "xx_eval_expr",  "xx_case_clauses",  "xx_case_clause",  "xx_common_expr",
  "xx_let_assignments",  "xx_let_assignment",  "xx_assignment_operator",  "xx_assign_expr",
  "xx_index_expr",  "xx_echo_expressions",  "xx_echo_expression",  "xx_mcall_expr",
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
 /* 151 */ "xx_declare_statement ::= TYPE_DOUBLE xx_declare_variable_list DOTCOMMA",
 /* 152 */ "xx_declare_statement ::= TYPE_STRING xx_declare_variable_list DOTCOMMA",
 /* 153 */ "xx_declare_statement ::= TYPE_BOOL xx_declare_variable_list DOTCOMMA",
 /* 154 */ "xx_declare_statement ::= TYPE_VAR xx_declare_variable_list DOTCOMMA",
 /* 155 */ "xx_declare_variable_list ::= xx_declare_variable_list COMMA xx_declare_variable",
 /* 156 */ "xx_declare_variable_list ::= xx_declare_variable",
 /* 157 */ "xx_declare_variable ::= IDENTIFIER",
 /* 158 */ "xx_declare_variable ::= IDENTIFIER ASSIGN xx_literal_expr",
 /* 159 */ "xx_assign_expr ::= xx_common_expr",
 /* 160 */ "xx_common_expr ::= NOT xx_common_expr",
 /* 161 */ "xx_common_expr ::= ISSET xx_isset_expr",
 /* 162 */ "xx_common_expr ::= xx_common_expr EQUALS xx_common_expr",
 /* 163 */ "xx_common_expr ::= xx_common_expr NOTEQUALS xx_common_expr",
 /* 164 */ "xx_common_expr ::= xx_common_expr IDENTICAL xx_common_expr",
 /* 165 */ "xx_common_expr ::= xx_common_expr NOTIDENTICAL xx_common_expr",
 /* 166 */ "xx_common_expr ::= xx_common_expr LESS xx_common_expr",
 /* 167 */ "xx_common_expr ::= xx_common_expr GREATER xx_common_expr",
 /* 168 */ "xx_common_expr ::= xx_common_expr LESSEQUAL xx_common_expr",
 /* 169 */ "xx_common_expr ::= xx_common_expr GREATEREQUAL xx_common_expr",
 /* 170 */ "xx_common_expr ::= PARENTHESES_OPEN xx_common_expr PARENTHESES_CLOSE",
 /* 171 */ "xx_common_expr ::= IDENTIFIER ARROW IDENTIFIER",
 /* 172 */ "xx_common_expr ::= IDENTIFIER SBRACKET_OPEN IDENTIFIER SBRACKET_CLOSE",
 /* 173 */ "xx_common_expr ::= IDENTIFIER SBRACKET_OPEN INTEGER SBRACKET_CLOSE",
 /* 174 */ "xx_common_expr ::= IDENTIFIER SBRACKET_OPEN STRING SBRACKET_CLOSE",
 /* 175 */ "xx_common_expr ::= xx_common_expr ADD xx_common_expr",
 /* 176 */ "xx_common_expr ::= xx_common_expr SUB xx_common_expr",
 /* 177 */ "xx_common_expr ::= xx_common_expr CONCAT xx_common_expr",
 /* 178 */ "xx_common_expr ::= xx_common_expr AND xx_common_expr",
 /* 179 */ "xx_common_expr ::= xx_common_expr OR xx_common_expr",
 /* 180 */ "xx_common_expr ::= INSTANCEOF xx_common_expr",
 /* 181 */ "xx_common_expr ::= FETCH IDENTIFIER COMMA xx_isset_expr",
 /* 182 */ "xx_common_expr ::= TYPEOF xx_common_expr",
 /* 183 */ "xx_common_expr ::= IDENTIFIER",
 /* 184 */ "xx_common_expr ::= INTEGER",
 /* 185 */ "xx_common_expr ::= STRING",
 /* 186 */ "xx_common_expr ::= DOUBLE",
 /* 187 */ "xx_common_expr ::= NULL",
 /* 188 */ "xx_common_expr ::= TRUE",
 /* 189 */ "xx_common_expr ::= FALSE",
 /* 190 */ "xx_common_expr ::= SBRACKET_OPEN SBRACKET_CLOSE",
 /* 191 */ "xx_common_expr ::= SBRACKET_OPEN xx_array_list SBRACKET_CLOSE",
 /* 192 */ "xx_common_expr ::= NEW IDENTIFIER",
 /* 193 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 194 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 195 */ "xx_common_expr ::= IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 196 */ "xx_common_expr ::= IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 197 */ "xx_common_expr ::= xx_mcall_expr",
 /* 198 */ "xx_mcall_expr ::= IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 199 */ "xx_mcall_expr ::= IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 200 */ "xx_call_parameters ::= xx_call_parameters COMMA xx_call_parameter",
 /* 201 */ "xx_call_parameters ::= xx_call_parameter",
 /* 202 */ "xx_call_parameter ::= xx_common_expr",
 /* 203 */ "xx_array_list ::= xx_array_list COMMA xx_array_item",
 /* 204 */ "xx_array_list ::= xx_array_item",
 /* 205 */ "xx_array_item ::= xx_array_key COLON xx_array_value",
 /* 206 */ "xx_array_item ::= xx_array_value",
 /* 207 */ "xx_array_key ::= IDENTIFIER",
 /* 208 */ "xx_array_key ::= STRING",
 /* 209 */ "xx_array_key ::= INTEGER",
 /* 210 */ "xx_array_value ::= xx_literal_expr",
 /* 211 */ "xx_array_value ::= IDENTIFIER",
 /* 212 */ "xx_literal_expr ::= INTEGER",
 /* 213 */ "xx_literal_expr ::= STRING",
 /* 214 */ "xx_literal_expr ::= DOUBLE",
 /* 215 */ "xx_literal_expr ::= NULL",
 /* 216 */ "xx_literal_expr ::= FALSE",
 /* 217 */ "xx_literal_expr ::= TRUE",
 /* 218 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN IDENTIFIER SBRACKET_CLOSE",
 /* 219 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN INTEGER SBRACKET_CLOSE",
 /* 220 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN STRING SBRACKET_CLOSE",
 /* 221 */ "xx_eval_expr ::= xx_common_expr",
 /* 222 */ "xx_comment ::= COMMENT",
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
// 758 "parser.lemon"
{
	/*if ((yypminor->yy0)) {
		if ((yypminor->yy0)->free_flag) {
			efree((yypminor->yy0)->token);
		}
		efree((yypminor->yy0));
	}*/
}
// 1893 "parser.c"
      break;
    case 82:
// 771 "parser.lemon"
{ json_object_put((yypminor->yy25)); }
// 1898 "parser.c"
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
  { 81, 1 },
  { 82, 1 },
  { 83, 2 },
  { 83, 1 },
  { 84, 1 },
  { 84, 1 },
  { 84, 1 },
  { 85, 3 },
  { 86, 5 },
  { 86, 7 },
  { 86, 6 },
  { 86, 8 },
  { 86, 4 },
  { 86, 6 },
  { 86, 5 },
  { 86, 7 },
  { 88, 1 },
  { 88, 1 },
  { 88, 1 },
  { 88, 2 },
  { 88, 2 },
  { 88, 2 },
  { 88, 2 },
  { 88, 3 },
  { 88, 3 },
  { 89, 2 },
  { 89, 1 },
  { 92, 4 },
  { 92, 3 },
  { 92, 6 },
  { 92, 5 },
  { 93, 2 },
  { 93, 1 },
  { 95, 1 },
  { 95, 1 },
  { 95, 1 },
  { 95, 1 },
  { 90, 2 },
  { 90, 1 },
  { 91, 2 },
  { 91, 1 },
  { 96, 6 },
  { 96, 5 },
  { 97, 7 },
  { 97, 8 },
  { 97, 8 },
  { 97, 9 },
  { 97, 8 },
  { 97, 9 },
  { 97, 9 },
  { 97, 10 },
  { 98, 1 },
  { 101, 1 },
  { 101, 1 },
  { 101, 1 },
  { 101, 1 },
  { 101, 2 },
  { 101, 2 },
  { 101, 2 },
  { 101, 2 },
  { 101, 2 },
  { 101, 2 },
  { 101, 2 },
  { 101, 2 },
  { 101, 2 },
  { 101, 2 },
  { 99, 3 },
  { 99, 1 },
  { 102, 1 },
  { 102, 2 },
  { 102, 2 },
  { 102, 3 },
  { 102, 4 },
  { 102, 4 },
  { 104, 3 },
  { 103, 1 },
  { 103, 1 },
  { 103, 1 },
  { 103, 1 },
  { 103, 1 },
  { 103, 1 },
  { 100, 2 },
  { 100, 1 },
  { 105, 1 },
  { 105, 1 },
  { 105, 1 },
  { 105, 1 },
  { 105, 1 },
  { 105, 1 },
  { 105, 1 },
  { 105, 1 },
  { 105, 1 },
  { 105, 1 },
  { 105, 1 },
  { 105, 1 },
  { 105, 1 },
  { 105, 1 },
  { 105, 1 },
  { 115, 2 },
  { 116, 2 },
  { 107, 4 },
  { 107, 7 },
  { 107, 5 },
  { 107, 9 },
  { 107, 8 },
  { 107, 8 },
  { 118, 4 },
  { 118, 5 },
  { 121, 2 },
  { 121, 1 },
  { 122, 4 },
  { 122, 3 },
  { 108, 3 },
  { 108, 4 },
  { 117, 4 },
  { 117, 5 },
  { 119, 7 },
  { 119, 8 },
  { 119, 9 },
  { 119, 10 },
  { 106, 3 },
  { 124, 3 },
  { 124, 1 },
  { 126, 1 },
  { 126, 1 },
  { 126, 1 },
  { 126, 1 },
  { 126, 1 },
  { 126, 1 },
  { 125, 3 },
  { 125, 5 },
  { 125, 7 },
  { 125, 8 },
  { 125, 5 },
  { 125, 6 },
  { 125, 2 },
  { 125, 2 },
  { 128, 1 },
  { 109, 3 },
  { 129, 3 },
  { 129, 1 },
  { 130, 1 },
  { 111, 2 },
  { 110, 3 },
  { 112, 5 },
  { 112, 6 },
  { 112, 6 },
  { 112, 6 },
  { 113, 3 },
  { 113, 7 },
  { 114, 3 },
  { 114, 3 },
  { 114, 3 },
  { 114, 3 },
  { 114, 3 },
  { 133, 3 },
  { 133, 1 },
  { 134, 1 },
  { 134, 3 },
  { 127, 1 },
  { 123, 2 },
  { 123, 2 },
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
  { 123, 4 },
  { 123, 4 },
  { 123, 4 },
  { 123, 3 },
  { 123, 3 },
  { 123, 3 },
  { 123, 3 },
  { 123, 3 },
  { 123, 2 },
  { 123, 4 },
  { 123, 2 },
  { 123, 1 },
  { 123, 1 },
  { 123, 1 },
  { 123, 1 },
  { 123, 1 },
  { 123, 1 },
  { 123, 1 },
  { 123, 2 },
  { 123, 3 },
  { 123, 2 },
  { 123, 4 },
  { 123, 5 },
  { 123, 4 },
  { 123, 3 },
  { 123, 1 },
  { 131, 6 },
  { 131, 5 },
  { 132, 3 },
  { 132, 1 },
  { 137, 1 },
  { 136, 3 },
  { 136, 1 },
  { 138, 3 },
  { 138, 1 },
  { 139, 1 },
  { 139, 1 },
  { 139, 1 },
  { 140, 1 },
  { 140, 1 },
  { 94, 1 },
  { 94, 1 },
  { 94, 1 },
  { 94, 1 },
  { 94, 1 },
  { 94, 1 },
  { 135, 4 },
  { 135, 4 },
  { 135, 4 },
  { 120, 1 },
  { 87, 1 },
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
// 767 "parser.lemon"
{
	status->ret = yymsp[0].minor.yy25;
}
// 2338 "parser.c"
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
      case 159:
      case 197:
      case 202:
      case 210:
      case 221:
// 773 "parser.lemon"
{
	yygotominor.yy25 = yymsp[0].minor.yy25;
}
// 2369 "parser.c"
        break;
      case 2:
      case 25:
      case 31:
      case 37:
      case 39:
      case 81:
      case 108:
// 777 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_list(yymsp[-1].minor.yy25, yymsp[0].minor.yy25);
}
// 2382 "parser.c"
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
      case 156:
      case 201:
      case 204:
// 781 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_list(NULL, yymsp[0].minor.yy25);
}
// 2402 "parser.c"
        break;
      case 7:
// 797 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_namespace(yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(27,&yymsp[0].minor);
}
// 2411 "parser.c"
        break;
      case 8:
// 801 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[-1].minor.yy25, 0, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-4].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 2421 "parser.c"
        break;
      case 9:
// 805 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class(yymsp[-5].minor.yy0, yymsp[-1].minor.yy25, 0, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(28,&yymsp[-6].minor);
  yy_destructor(31,&yymsp[-4].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 2432 "parser.c"
        break;
      case 10:
// 809 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[-1].minor.yy25, 1, NULL, status->scanner_state);
  yy_destructor(32,&yymsp[-5].minor);
  yy_destructor(28,&yymsp[-4].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 2443 "parser.c"
        break;
      case 11:
// 813 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class(yymsp[-5].minor.yy0, yymsp[-1].minor.yy25, 1, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(32,&yymsp[-7].minor);
  yy_destructor(28,&yymsp[-6].minor);
  yy_destructor(31,&yymsp[-4].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 2455 "parser.c"
        break;
      case 12:
// 817 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class(yymsp[-2].minor.yy0, NULL, 0, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-3].minor);
  yy_destructor(29,&yymsp[-1].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 2465 "parser.c"
        break;
      case 13:
// 821 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class(yymsp[-4].minor.yy0, NULL, 0, yymsp[-2].minor.yy0, status->scanner_state);
  yy_destructor(28,&yymsp[-5].minor);
  yy_destructor(31,&yymsp[-3].minor);
  yy_destructor(29,&yymsp[-1].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 2476 "parser.c"
        break;
      case 14:
// 825 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class(yymsp[-2].minor.yy0, NULL, 1, NULL, status->scanner_state);
  yy_destructor(32,&yymsp[-4].minor);
  yy_destructor(28,&yymsp[-3].minor);
  yy_destructor(29,&yymsp[-1].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 2487 "parser.c"
        break;
      case 15:
// 829 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class(yymsp[-4].minor.yy0, NULL, 1, yymsp[-2].minor.yy0, status->scanner_state);
  yy_destructor(32,&yymsp[-6].minor);
  yy_destructor(28,&yymsp[-5].minor);
  yy_destructor(31,&yymsp[-3].minor);
  yy_destructor(29,&yymsp[-1].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 2499 "parser.c"
        break;
      case 16:
// 833 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_definition(yymsp[0].minor.yy25, NULL, NULL, status->scanner_state);
}
// 2506 "parser.c"
        break;
      case 17:
// 837 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_definition(NULL, NULL, yymsp[0].minor.yy25, status->scanner_state);
}
// 2513 "parser.c"
        break;
      case 18:
// 841 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_definition(NULL, yymsp[0].minor.yy25, NULL, status->scanner_state);
}
// 2520 "parser.c"
        break;
      case 19:
// 845 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_definition(yymsp[-1].minor.yy25, yymsp[0].minor.yy25, NULL, status->scanner_state);
}
// 2527 "parser.c"
        break;
      case 20:
// 849 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_definition(yymsp[-1].minor.yy25, NULL, yymsp[0].minor.yy25, status->scanner_state);
}
// 2534 "parser.c"
        break;
      case 21:
// 853 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_definition(yymsp[0].minor.yy25, NULL, yymsp[-1].minor.yy25, status->scanner_state);
}
// 2541 "parser.c"
        break;
      case 22:
// 857 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_definition(NULL, yymsp[0].minor.yy25, yymsp[-1].minor.yy25, status->scanner_state);
}
// 2548 "parser.c"
        break;
      case 23:
// 861 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_definition(yymsp[-2].minor.yy25, yymsp[0].minor.yy25, yymsp[-1].minor.yy25, status->scanner_state);
}
// 2555 "parser.c"
        break;
      case 24:
// 865 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_definition(yymsp[-1].minor.yy25, yymsp[0].minor.yy25, yymsp[-2].minor.yy25, status->scanner_state);
}
// 2562 "parser.c"
        break;
      case 27:
// 877 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_property(yymsp[-2].minor.yy25, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(27,&yymsp[0].minor);
}
// 2570 "parser.c"
        break;
      case 28:
// 881 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_property(yymsp[-2].minor.yy25, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(27,&yymsp[0].minor);
}
// 2578 "parser.c"
        break;
      case 29:
// 885 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_property(yymsp[-4].minor.yy25, yymsp[-3].minor.yy0, yymsp[-1].minor.yy25, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(34,&yymsp[-2].minor);
  yy_destructor(27,&yymsp[0].minor);
}
// 2587 "parser.c"
        break;
      case 30:
// 889 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_property(yymsp[-4].minor.yy25, yymsp[-3].minor.yy0, yymsp[-1].minor.yy25, NULL, status->scanner_state);
  yy_destructor(34,&yymsp[-2].minor);
  yy_destructor(27,&yymsp[0].minor);
}
// 2596 "parser.c"
        break;
      case 33:
// 901 "parser.lemon"
{
	yygotominor.yy25 = json_object_new_string("public");
  yy_destructor(1,&yymsp[0].minor);
}
// 2604 "parser.c"
        break;
      case 34:
// 905 "parser.lemon"
{
	yygotominor.yy25 = json_object_new_string("protected");
  yy_destructor(2,&yymsp[0].minor);
}
// 2612 "parser.c"
        break;
      case 35:
// 909 "parser.lemon"
{
	yygotominor.yy25 = json_object_new_string("static");
  yy_destructor(3,&yymsp[0].minor);
}
// 2620 "parser.c"
        break;
      case 36:
// 913 "parser.lemon"
{
	yygotominor.yy25 = json_object_new_string("private");
  yy_destructor(4,&yymsp[0].minor);
}
// 2628 "parser.c"
        break;
      case 41:
// 933 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy25, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(35,&yymsp[-4].minor);
  yy_destructor(34,&yymsp[-2].minor);
  yy_destructor(27,&yymsp[0].minor);
}
// 2638 "parser.c"
        break;
      case 42:
// 937 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy25, NULL, status->scanner_state);
  yy_destructor(35,&yymsp[-4].minor);
  yy_destructor(34,&yymsp[-2].minor);
  yy_destructor(27,&yymsp[0].minor);
}
// 2648 "parser.c"
        break;
      case 43:
// 941 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_method(yymsp[-6].minor.yy25, yymsp[-4].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(36,&yymsp[-5].minor);
  yy_destructor(37,&yymsp[-3].minor);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(29,&yymsp[-1].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 2660 "parser.c"
        break;
      case 44:
// 945 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_method(yymsp[-7].minor.yy25, yymsp[-5].minor.yy0, yymsp[-3].minor.yy25, NULL, NULL, status->scanner_state);
  yy_destructor(36,&yymsp[-6].minor);
  yy_destructor(37,&yymsp[-4].minor);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(29,&yymsp[-1].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 2672 "parser.c"
        break;
      case 45:
// 949 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_method(yymsp[-7].minor.yy25, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy25, NULL, status->scanner_state);
  yy_destructor(36,&yymsp[-6].minor);
  yy_destructor(37,&yymsp[-4].minor);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 2684 "parser.c"
        break;
      case 46:
// 953 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_method(yymsp[-8].minor.yy25, yymsp[-6].minor.yy0, yymsp[-4].minor.yy25, yymsp[-1].minor.yy25, NULL, status->scanner_state);
  yy_destructor(36,&yymsp[-7].minor);
  yy_destructor(37,&yymsp[-5].minor);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 2696 "parser.c"
        break;
      case 47:
// 957 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_method(yymsp[-6].minor.yy25, yymsp[-4].minor.yy0, NULL, NULL, yymsp[-7].minor.yy0, status->scanner_state);
  yy_destructor(36,&yymsp[-5].minor);
  yy_destructor(37,&yymsp[-3].minor);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(29,&yymsp[-1].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 2708 "parser.c"
        break;
      case 48:
// 961 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_method(yymsp[-7].minor.yy25, yymsp[-5].minor.yy0, yymsp[-3].minor.yy25, NULL, yymsp[-8].minor.yy0, status->scanner_state);
  yy_destructor(36,&yymsp[-6].minor);
  yy_destructor(37,&yymsp[-4].minor);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(29,&yymsp[-1].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 2720 "parser.c"
        break;
      case 49:
// 965 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_method(yymsp[-7].minor.yy25, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy25, yymsp[-8].minor.yy0, status->scanner_state);
  yy_destructor(36,&yymsp[-6].minor);
  yy_destructor(37,&yymsp[-4].minor);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 2732 "parser.c"
        break;
      case 50:
// 969 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_method(yymsp[-8].minor.yy25, yymsp[-6].minor.yy0, yymsp[-4].minor.yy25, yymsp[-1].minor.yy25, yymsp[-9].minor.yy0, status->scanner_state);
  yy_destructor(36,&yymsp[-7].minor);
  yy_destructor(37,&yymsp[-5].minor);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 2744 "parser.c"
        break;
      case 52:
// 981 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_list(NULL, json_object_new_string("public"));
  yy_destructor(1,&yymsp[0].minor);
}
// 2752 "parser.c"
        break;
      case 53:
// 985 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_list(NULL, json_object_new_string("protected"));
  yy_destructor(2,&yymsp[0].minor);
}
// 2760 "parser.c"
        break;
      case 54:
// 989 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_list(NULL, json_object_new_string("static"));
  yy_destructor(3,&yymsp[0].minor);
}
// 2768 "parser.c"
        break;
      case 55:
// 993 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_list(NULL, json_object_new_string("private"));
  yy_destructor(4,&yymsp[0].minor);
}
// 2776 "parser.c"
        break;
      case 56:
// 997 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_list(json_object_new_string("static"), json_object_new_string("public"));
  yy_destructor(3,&yymsp[-1].minor);
  yy_destructor(1,&yymsp[0].minor);
}
// 2785 "parser.c"
        break;
      case 57:
// 1001 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_list(json_object_new_string("static"), json_object_new_string("public"));
  yy_destructor(1,&yymsp[-1].minor);
  yy_destructor(3,&yymsp[0].minor);
}
// 2794 "parser.c"
        break;
      case 58:
// 1005 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_list(json_object_new_string("static"), json_object_new_string("protected"));
  yy_destructor(3,&yymsp[-1].minor);
  yy_destructor(2,&yymsp[0].minor);
}
// 2803 "parser.c"
        break;
      case 59:
// 1009 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_list(json_object_new_string("static"), json_object_new_string("protected"));
  yy_destructor(2,&yymsp[-1].minor);
  yy_destructor(3,&yymsp[0].minor);
}
// 2812 "parser.c"
        break;
      case 60:
// 1013 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_list(json_object_new_string("static"), json_object_new_string("private"));
  yy_destructor(3,&yymsp[-1].minor);
  yy_destructor(4,&yymsp[0].minor);
}
// 2821 "parser.c"
        break;
      case 61:
// 1017 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_list(json_object_new_string("static"), json_object_new_string("private"));
  yy_destructor(4,&yymsp[-1].minor);
  yy_destructor(3,&yymsp[0].minor);
}
// 2830 "parser.c"
        break;
      case 62:
// 1021 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_list(json_object_new_string("inline"), json_object_new_string("protected"));
  yy_destructor(39,&yymsp[-1].minor);
  yy_destructor(2,&yymsp[0].minor);
}
// 2839 "parser.c"
        break;
      case 63:
// 1025 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_list(json_object_new_string("inline"), json_object_new_string("protected"));
  yy_destructor(2,&yymsp[-1].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 2848 "parser.c"
        break;
      case 64:
// 1029 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_list(json_object_new_string("inline"), json_object_new_string("private"));
  yy_destructor(39,&yymsp[-1].minor);
  yy_destructor(4,&yymsp[0].minor);
}
// 2857 "parser.c"
        break;
      case 65:
// 1033 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_list(json_object_new_string("inline"), json_object_new_string("private"));
  yy_destructor(4,&yymsp[-1].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 2866 "parser.c"
        break;
      case 66:
      case 121:
      case 139:
      case 155:
      case 200:
      case 203:
// 1037 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_list(yymsp[-2].minor.yy25, yymsp[0].minor.yy25);
  yy_destructor(5,&yymsp[-1].minor);
}
// 2879 "parser.c"
        break;
      case 68:
// 1045 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_parameter(NULL, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 2886 "parser.c"
        break;
      case 69:
// 1049 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_parameter(yymsp[-1].minor.yy25, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 2893 "parser.c"
        break;
      case 70:
// 1053 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_parameter(NULL, yymsp[-1].minor.yy25, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 2900 "parser.c"
        break;
      case 71:
// 1057 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_parameter(NULL, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy25, status->scanner_state);
  yy_destructor(34,&yymsp[-1].minor);
}
// 2908 "parser.c"
        break;
      case 72:
// 1061 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_parameter(yymsp[-3].minor.yy25, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy25, status->scanner_state);
  yy_destructor(34,&yymsp[-1].minor);
}
// 2916 "parser.c"
        break;
      case 73:
// 1065 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_parameter(NULL, yymsp[-3].minor.yy25, yymsp[-2].minor.yy0, yymsp[0].minor.yy25, status->scanner_state);
  yy_destructor(34,&yymsp[-1].minor);
}
// 2924 "parser.c"
        break;
      case 74:
// 1069 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(10,&yymsp[-2].minor);
  yy_destructor(11,&yymsp[0].minor);
}
// 2933 "parser.c"
        break;
      case 75:
// 1073 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_type(XX_TYPE_INTEGER);
  yy_destructor(40,&yymsp[0].minor);
}
// 2941 "parser.c"
        break;
      case 76:
// 1077 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_type(XX_TYPE_DOUBLE);
  yy_destructor(41,&yymsp[0].minor);
}
// 2949 "parser.c"
        break;
      case 77:
// 1081 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_type(XX_TYPE_BOOL);
  yy_destructor(42,&yymsp[0].minor);
}
// 2957 "parser.c"
        break;
      case 78:
// 1085 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_type(XX_TYPE_STRING);
  yy_destructor(43,&yymsp[0].minor);
}
// 2965 "parser.c"
        break;
      case 79:
// 1089 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_type(XX_TYPE_ARRAY);
  yy_destructor(44,&yymsp[0].minor);
}
// 2973 "parser.c"
        break;
      case 80:
// 1093 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_type(XX_TYPE_VAR);
  yy_destructor(45,&yymsp[0].minor);
}
// 2981 "parser.c"
        break;
      case 98:
// 1165 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_break_statement(status->scanner_state);
  yy_destructor(46,&yymsp[-1].minor);
  yy_destructor(27,&yymsp[0].minor);
}
// 2990 "parser.c"
        break;
      case 99:
// 1169 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_continue_statement(status->scanner_state);
  yy_destructor(47,&yymsp[-1].minor);
  yy_destructor(27,&yymsp[0].minor);
}
// 2999 "parser.c"
        break;
      case 100:
// 1173 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_if_statement(yymsp[-2].minor.yy25, NULL, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-3].minor);
  yy_destructor(29,&yymsp[-1].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 3009 "parser.c"
        break;
      case 101:
// 1177 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_if_statement(yymsp[-5].minor.yy25, NULL, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-6].minor);
  yy_destructor(29,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(29,&yymsp[-1].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 3022 "parser.c"
        break;
      case 102:
// 1181 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_if_statement(yymsp[-3].minor.yy25, yymsp[-1].minor.yy25, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-4].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 3032 "parser.c"
        break;
      case 103:
// 1185 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_if_statement(yymsp[-7].minor.yy25, yymsp[-5].minor.yy25, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(48,&yymsp[-8].minor);
  yy_destructor(29,&yymsp[-6].minor);
  yy_destructor(30,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 3045 "parser.c"
        break;
      case 104:
// 1189 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_if_statement(yymsp[-6].minor.yy25, yymsp[-4].minor.yy25, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-7].minor);
  yy_destructor(29,&yymsp[-5].minor);
  yy_destructor(30,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(29,&yymsp[-1].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 3058 "parser.c"
        break;
      case 105:
// 1193 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_if_statement(yymsp[-6].minor.yy25, NULL, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(48,&yymsp[-7].minor);
  yy_destructor(29,&yymsp[-5].minor);
  yy_destructor(30,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 3071 "parser.c"
        break;
      case 106:
// 1197 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_switch_statement(yymsp[-2].minor.yy25, NULL, status->scanner_state);
  yy_destructor(50,&yymsp[-3].minor);
  yy_destructor(29,&yymsp[-1].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 3081 "parser.c"
        break;
      case 107:
// 1201 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_switch_statement(yymsp[-3].minor.yy25, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(50,&yymsp[-4].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 3091 "parser.c"
        break;
      case 110:
// 1213 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_case_clause(yymsp[-2].minor.yy25, yymsp[0].minor.yy25, status->scanner_state);
  yy_destructor(51,&yymsp[-3].minor);
  yy_destructor(52,&yymsp[-1].minor);
}
// 3100 "parser.c"
        break;
      case 111:
// 1217 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_case_clause(NULL, yymsp[0].minor.yy25, status->scanner_state);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(52,&yymsp[-1].minor);
}
// 3109 "parser.c"
        break;
      case 112:
// 1221 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_loop_statement(NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(29,&yymsp[-1].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 3119 "parser.c"
        break;
      case 113:
// 1225 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_loop_statement(yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(54,&yymsp[-3].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 3129 "parser.c"
        break;
      case 114:
// 1229 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_while_statement(yymsp[-2].minor.yy25, NULL, status->scanner_state);
  yy_destructor(55,&yymsp[-3].minor);
  yy_destructor(29,&yymsp[-1].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 3139 "parser.c"
        break;
      case 115:
// 1233 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_while_statement(yymsp[-3].minor.yy25, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(55,&yymsp[-4].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 3149 "parser.c"
        break;
      case 116:
// 1237 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_for_statement(yymsp[-3].minor.yy25, NULL, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(56,&yymsp[-6].minor);
  yy_destructor(57,&yymsp[-4].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 3160 "parser.c"
        break;
      case 117:
// 1241 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_for_statement(yymsp[-3].minor.yy25, NULL, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(56,&yymsp[-7].minor);
  yy_destructor(57,&yymsp[-5].minor);
  yy_destructor(58,&yymsp[-4].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 3172 "parser.c"
        break;
      case 118:
// 1245 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_for_statement(yymsp[-3].minor.yy25, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(56,&yymsp[-8].minor);
  yy_destructor(5,&yymsp[-6].minor);
  yy_destructor(57,&yymsp[-4].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 3184 "parser.c"
        break;
      case 119:
// 1249 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_for_statement(yymsp[-3].minor.yy25, yymsp[-8].minor.yy0, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(56,&yymsp[-9].minor);
  yy_destructor(5,&yymsp[-7].minor);
  yy_destructor(57,&yymsp[-5].minor);
  yy_destructor(58,&yymsp[-4].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 3197 "parser.c"
        break;
      case 120:
// 1253 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_let_statement(yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(59,&yymsp[-2].minor);
  yy_destructor(27,&yymsp[0].minor);
}
// 3206 "parser.c"
        break;
      case 123:
// 1266 "parser.lemon"
{
	yygotominor.yy25 = json_object_new_string("assign");
  yy_destructor(34,&yymsp[0].minor);
}
// 3214 "parser.c"
        break;
      case 124:
// 1271 "parser.lemon"
{
	yygotominor.yy25 = json_object_new_string("add-assign");
  yy_destructor(60,&yymsp[0].minor);
}
// 3222 "parser.c"
        break;
      case 125:
// 1276 "parser.lemon"
{
	yygotominor.yy25 = json_object_new_string("sub-assign");
  yy_destructor(61,&yymsp[0].minor);
}
// 3230 "parser.c"
        break;
      case 126:
// 1280 "parser.lemon"
{
	yygotominor.yy25 = json_object_new_string("mult-assign");
  yy_destructor(62,&yymsp[0].minor);
}
// 3238 "parser.c"
        break;
      case 127:
// 1284 "parser.lemon"
{
	yygotominor.yy25 = json_object_new_string("div-assign");
  yy_destructor(63,&yymsp[0].minor);
}
// 3246 "parser.c"
        break;
      case 128:
// 1288 "parser.lemon"
{
	yygotominor.yy25 = json_object_new_string("concat-assign");
  yy_destructor(64,&yymsp[0].minor);
}
// 3254 "parser.c"
        break;
      case 129:
// 1293 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_let_assignment("variable", yymsp[-1].minor.yy25, yymsp[-2].minor.yy0, NULL, NULL, yymsp[0].minor.yy25, status->scanner_state);
}
// 3261 "parser.c"
        break;
      case 130:
// 1298 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_let_assignment("object-property", yymsp[-1].minor.yy25, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy25, status->scanner_state);
  yy_destructor(65,&yymsp[-3].minor);
}
// 3269 "parser.c"
        break;
      case 131:
// 1303 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_let_assignment("object-property-append", yymsp[-1].minor.yy25, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy25, status->scanner_state);
  yy_destructor(65,&yymsp[-5].minor);
  yy_destructor(66,&yymsp[-3].minor);
  yy_destructor(67,&yymsp[-2].minor);
}
// 3279 "parser.c"
        break;
      case 132:
// 1308 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_let_assignment("object-property-array-index", yymsp[-1].minor.yy25, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy25, yymsp[0].minor.yy25, status->scanner_state);
  yy_destructor(65,&yymsp[-6].minor);
  yy_destructor(66,&yymsp[-4].minor);
  yy_destructor(67,&yymsp[-2].minor);
}
// 3289 "parser.c"
        break;
      case 133:
// 1313 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_let_assignment("variable-append", yymsp[-1].minor.yy25, yymsp[-4].minor.yy0, NULL, NULL, yymsp[0].minor.yy25, status->scanner_state);
  yy_destructor(66,&yymsp[-3].minor);
  yy_destructor(67,&yymsp[-2].minor);
}
// 3298 "parser.c"
        break;
      case 134:
// 1318 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_let_assignment("array-index", yymsp[-1].minor.yy25, yymsp[-5].minor.yy0, NULL, yymsp[-3].minor.yy25, yymsp[0].minor.yy25, status->scanner_state);
  yy_destructor(66,&yymsp[-4].minor);
  yy_destructor(67,&yymsp[-2].minor);
}
// 3307 "parser.c"
        break;
      case 135:
// 1323 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_let_assignment("incr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(68,&yymsp[0].minor);
}
// 3315 "parser.c"
        break;
      case 136:
// 1328 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_let_assignment("decr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(69,&yymsp[0].minor);
}
// 3323 "parser.c"
        break;
      case 138:
// 1336 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_echo_statement(yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(70,&yymsp[-2].minor);
  yy_destructor(27,&yymsp[0].minor);
}
// 3332 "parser.c"
        break;
      case 141:
// 1348 "parser.lemon"
{
	yygotominor.yy25 = yymsp[0].minor.yy25;;
}
// 3339 "parser.c"
        break;
      case 142:
// 1353 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_mcall_statement(yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(27,&yymsp[0].minor);
}
// 3347 "parser.c"
        break;
      case 143:
// 1358 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_return_statement(yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(71,&yymsp[-2].minor);
  yy_destructor(27,&yymsp[0].minor);
}
// 3356 "parser.c"
        break;
      case 144:
// 1363 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(72,&yymsp[-4].minor);
  yy_destructor(65,&yymsp[-2].minor);
  yy_destructor(27,&yymsp[0].minor);
}
// 3370 "parser.c"
        break;
      case 145:
// 1372 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(72,&yymsp[-5].minor);
  yy_destructor(66,&yymsp[-3].minor);
  yy_destructor(67,&yymsp[-1].minor);
  yy_destructor(27,&yymsp[0].minor);
}
// 3385 "parser.c"
        break;
      case 146:
// 1381 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_INTEGER, yymsp[-2].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(72,&yymsp[-5].minor);
  yy_destructor(66,&yymsp[-3].minor);
  yy_destructor(67,&yymsp[-1].minor);
  yy_destructor(27,&yymsp[0].minor);
}
// 3400 "parser.c"
        break;
      case 147:
// 1390 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_STRING, yymsp[-2].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(72,&yymsp[-5].minor);
  yy_destructor(66,&yymsp[-3].minor);
  yy_destructor(67,&yymsp[-1].minor);
  yy_destructor(27,&yymsp[0].minor);
}
// 3415 "parser.c"
        break;
      case 148:
// 1399 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_throw_exception(yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(75,&yymsp[-2].minor);
  yy_destructor(27,&yymsp[0].minor);
}
// 3424 "parser.c"
        break;
      case 149:
// 1404 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_throw_exception(yymsp[-4].minor.yy0, yymsp[-2].minor.yy25, status->scanner_state);
  yy_destructor(75,&yymsp[-6].minor);
  yy_destructor(23,&yymsp[-5].minor);
  yy_destructor(37,&yymsp[-3].minor);
  yy_destructor(38,&yymsp[-1].minor);
  yy_destructor(27,&yymsp[0].minor);
}
// 3436 "parser.c"
        break;
      case 150:
// 1408 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_declare_statement(XX_T_TYPE_INTEGER, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(27,&yymsp[0].minor);
}
// 3445 "parser.c"
        break;
      case 151:
// 1412 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_declare_statement(XX_T_TYPE_DOUBLE, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(41,&yymsp[-2].minor);
  yy_destructor(27,&yymsp[0].minor);
}
// 3454 "parser.c"
        break;
      case 152:
// 1416 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_declare_statement(XX_T_TYPE_STRING, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(27,&yymsp[0].minor);
}
// 3463 "parser.c"
        break;
      case 153:
// 1420 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_declare_statement(XX_T_TYPE_BOOL, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(27,&yymsp[0].minor);
}
// 3472 "parser.c"
        break;
      case 154:
// 1424 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_declare_statement(XX_T_TYPE_VAR, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(45,&yymsp[-2].minor);
  yy_destructor(27,&yymsp[0].minor);
}
// 3481 "parser.c"
        break;
      case 157:
// 1436 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_declare_variable(yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3488 "parser.c"
        break;
      case 158:
// 1440 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_declare_variable(yymsp[-2].minor.yy0, yymsp[0].minor.yy25, status->scanner_state);
  yy_destructor(34,&yymsp[-1].minor);
}
// 3496 "parser.c"
        break;
      case 160:
// 1448 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("not", yymsp[0].minor.yy25, NULL, NULL, status->scanner_state);
  yy_destructor(24,&yymsp[-1].minor);
}
// 3504 "parser.c"
        break;
      case 161:
// 1452 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("isset", yymsp[0].minor.yy25, NULL, NULL, status->scanner_state);
  yy_destructor(19,&yymsp[-1].minor);
}
// 3512 "parser.c"
        break;
      case 162:
// 1456 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("equals", yymsp[-2].minor.yy25, yymsp[0].minor.yy25, NULL, status->scanner_state);
  yy_destructor(8,&yymsp[-1].minor);
}
// 3520 "parser.c"
        break;
      case 163:
// 1460 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("not-equals", yymsp[-2].minor.yy25, yymsp[0].minor.yy25, NULL, status->scanner_state);
  yy_destructor(15,&yymsp[-1].minor);
}
// 3528 "parser.c"
        break;
      case 164:
// 1464 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("identical", yymsp[-2].minor.yy25, yymsp[0].minor.yy25, NULL, status->scanner_state);
  yy_destructor(9,&yymsp[-1].minor);
}
// 3536 "parser.c"
        break;
      case 165:
// 1468 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("not-identical", yymsp[-2].minor.yy25, yymsp[0].minor.yy25, NULL, status->scanner_state);
  yy_destructor(14,&yymsp[-1].minor);
}
// 3544 "parser.c"
        break;
      case 166:
// 1472 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("less", yymsp[-2].minor.yy25, yymsp[0].minor.yy25, NULL, status->scanner_state);
  yy_destructor(10,&yymsp[-1].minor);
}
// 3552 "parser.c"
        break;
      case 167:
// 1476 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("greater", yymsp[-2].minor.yy25, yymsp[0].minor.yy25, NULL, status->scanner_state);
  yy_destructor(11,&yymsp[-1].minor);
}
// 3560 "parser.c"
        break;
      case 168:
// 1480 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("less-equal", yymsp[-2].minor.yy25, yymsp[0].minor.yy25, NULL, status->scanner_state);
  yy_destructor(12,&yymsp[-1].minor);
}
// 3568 "parser.c"
        break;
      case 169:
// 1484 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("greater-equal", yymsp[-2].minor.yy25, yymsp[0].minor.yy25, NULL, status->scanner_state);
  yy_destructor(13,&yymsp[-1].minor);
}
// 3576 "parser.c"
        break;
      case 170:
// 1488 "parser.lemon"
{
	yygotominor.yy25 = yymsp[-1].minor.yy25;
  yy_destructor(37,&yymsp[-2].minor);
  yy_destructor(38,&yymsp[0].minor);
}
// 3585 "parser.c"
        break;
      case 171:
// 1492 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(65,&yymsp[-1].minor);
}
// 3593 "parser.c"
        break;
      case 172:
      case 218:
// 1496 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(66,&yymsp[-2].minor);
  yy_destructor(67,&yymsp[0].minor);
}
// 3603 "parser.c"
        break;
      case 173:
      case 219:
// 1501 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_INTEGER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(66,&yymsp[-2].minor);
  yy_destructor(67,&yymsp[0].minor);
}
// 3613 "parser.c"
        break;
      case 174:
      case 220:
// 1506 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_STRING, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(66,&yymsp[-2].minor);
  yy_destructor(67,&yymsp[0].minor);
}
// 3623 "parser.c"
        break;
      case 175:
// 1511 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("add", yymsp[-2].minor.yy25, yymsp[0].minor.yy25, NULL, status->scanner_state);
  yy_destructor(16,&yymsp[-1].minor);
}
// 3631 "parser.c"
        break;
      case 176:
// 1516 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("sub", yymsp[-2].minor.yy25, yymsp[0].minor.yy25, NULL, status->scanner_state);
  yy_destructor(17,&yymsp[-1].minor);
}
// 3639 "parser.c"
        break;
      case 177:
// 1521 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("concat", yymsp[-2].minor.yy25, yymsp[0].minor.yy25, NULL, status->scanner_state);
  yy_destructor(18,&yymsp[-1].minor);
}
// 3647 "parser.c"
        break;
      case 178:
// 1526 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("and", yymsp[-2].minor.yy25, yymsp[0].minor.yy25, NULL, status->scanner_state);
  yy_destructor(6,&yymsp[-1].minor);
}
// 3655 "parser.c"
        break;
      case 179:
// 1531 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("or", yymsp[-2].minor.yy25, yymsp[0].minor.yy25, NULL, status->scanner_state);
  yy_destructor(7,&yymsp[-1].minor);
}
// 3663 "parser.c"
        break;
      case 180:
// 1536 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("instanceof", yymsp[0].minor.yy25, NULL, NULL, status->scanner_state);
  yy_destructor(22,&yymsp[-1].minor);
}
// 3671 "parser.c"
        break;
      case 181:
// 1541 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("fetch", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy25, NULL, status->scanner_state);
  yy_destructor(20,&yymsp[-3].minor);
  yy_destructor(5,&yymsp[-1].minor);
}
// 3680 "parser.c"
        break;
      case 182:
// 1546 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("typeof", yymsp[0].minor.yy25, NULL, NULL, status->scanner_state);
  yy_destructor(21,&yymsp[-1].minor);
}
// 3688 "parser.c"
        break;
      case 183:
      case 207:
      case 211:
// 1551 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state);
}
// 3697 "parser.c"
        break;
      case 184:
      case 209:
      case 212:
// 1556 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_literal(XX_T_INTEGER, yymsp[0].minor.yy0, status->scanner_state);
}
// 3706 "parser.c"
        break;
      case 185:
      case 208:
      case 213:
// 1561 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_literal(XX_T_STRING, yymsp[0].minor.yy0, status->scanner_state);
}
// 3715 "parser.c"
        break;
      case 186:
      case 214:
// 1566 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_literal(XX_T_DOUBLE, yymsp[0].minor.yy0, status->scanner_state);
}
// 3723 "parser.c"
        break;
      case 187:
      case 215:
// 1571 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_literal(XX_T_NULL, NULL, status->scanner_state);
  yy_destructor(77,&yymsp[0].minor);
}
// 3732 "parser.c"
        break;
      case 188:
      case 217:
// 1576 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_literal(XX_T_TRUE, NULL, status->scanner_state);
  yy_destructor(78,&yymsp[0].minor);
}
// 3741 "parser.c"
        break;
      case 189:
      case 216:
// 1581 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_literal(XX_T_FALSE, NULL, status->scanner_state);
  yy_destructor(79,&yymsp[0].minor);
}
// 3750 "parser.c"
        break;
      case 190:
// 1586 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("empty-array", NULL, NULL, NULL, status->scanner_state);
  yy_destructor(66,&yymsp[-1].minor);
  yy_destructor(67,&yymsp[0].minor);
}
// 3759 "parser.c"
        break;
      case 191:
// 1591 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("array", yymsp[-1].minor.yy25, NULL, NULL, status->scanner_state);
  yy_destructor(66,&yymsp[-2].minor);
  yy_destructor(67,&yymsp[0].minor);
}
// 3768 "parser.c"
        break;
      case 192:
// 1596 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_new_instance(yymsp[0].minor.yy0, NULL, status->scanner_state);
  yy_destructor(23,&yymsp[-1].minor);
}
// 3776 "parser.c"
        break;
      case 193:
// 1601 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_new_instance(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(23,&yymsp[-3].minor);
  yy_destructor(37,&yymsp[-1].minor);
  yy_destructor(38,&yymsp[0].minor);
}
// 3786 "parser.c"
        break;
      case 194:
// 1606 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_new_instance(yymsp[-3].minor.yy0, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(23,&yymsp[-4].minor);
  yy_destructor(37,&yymsp[-2].minor);
  yy_destructor(38,&yymsp[0].minor);
}
// 3796 "parser.c"
        break;
      case 195:
// 1611 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_fcall(yymsp[-3].minor.yy0, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(37,&yymsp[-2].minor);
  yy_destructor(38,&yymsp[0].minor);
}
// 3805 "parser.c"
        break;
      case 196:
// 1616 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_fcall(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(37,&yymsp[-1].minor);
  yy_destructor(38,&yymsp[0].minor);
}
// 3814 "parser.c"
        break;
      case 198:
// 1626 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_mcall(yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(65,&yymsp[-4].minor);
  yy_destructor(37,&yymsp[-2].minor);
  yy_destructor(38,&yymsp[0].minor);
}
// 3824 "parser.c"
        break;
      case 199:
// 1631 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_mcall(yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(65,&yymsp[-3].minor);
  yy_destructor(37,&yymsp[-1].minor);
  yy_destructor(38,&yymsp[0].minor);
}
// 3834 "parser.c"
        break;
      case 205:
// 1655 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_array_item(yymsp[-2].minor.yy25, yymsp[0].minor.yy25, status->scanner_state);
  yy_destructor(52,&yymsp[-1].minor);
}
// 3842 "parser.c"
        break;
      case 206:
// 1659 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_array_item(NULL, yymsp[0].minor.yy25, status->scanner_state);
}
// 3849 "parser.c"
        break;
      case 222:
// 1726 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_comment(yymsp[0].minor.yy0, status->scanner_state);
}
// 3856 "parser.c"
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
// 724 "parser.lemon"


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

// 3932 "parser.c"
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
