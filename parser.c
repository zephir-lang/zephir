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


// 693 "parser.c"
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
#define YYNOCODE 139
#define YYACTIONTYPE unsigned short int
#define xx_TOKENTYPE xx_parser_token*
typedef union {
  xx_TOKENTYPE yy0;
  json_object* yy55;
  int yy277;
} YYMINORTYPE;
#define YYSTACKDEPTH 100
#define xx_ARG_SDECL xx_parser_status *status;
#define xx_ARG_PDECL ,xx_parser_status *status
#define xx_ARG_FETCH xx_parser_status *status = yypParser->status
#define xx_ARG_STORE yypParser->status = status
#define YYNSTATE 456
#define YYNRULE 213
#define YYERRORSYMBOL 79
#define YYERRSYMDT yy277
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
 /*     0 */   286,  456,   60,  259,   25,  339,  257,  289,  670,    1,
 /*    10 */     2,  455,    4,    5,    6,   61,  314,  265,  274,  280,
 /*    20 */   277,  328,  283,   44,   46,   48,   64,   57,  311,    8,
 /*    30 */   176,   68,   72,   77,  200,    9,   84,   23,  342,  343,
 /*    40 */   344,  345,  346,  347,   85,  228,  286,  229,  238,  241,
 /*    50 */    27,   15,  256,  289,  405,   11,  426,  427,  415,  416,
 /*    60 */   399,  140,  193,  265,  274,  280,  277,  217,  283,   44,
 /*    70 */    46,   48,  213,   57,  159,  187,  107,   68,   72,   77,
 /*    80 */   178,  177,   84,  171,  172,  174,  173,   14,    3,    4,
 /*    90 */     5,    6,  286,  229,  238,  241,  316,  472,  256,  289,
 /*   100 */   405,   20,  426,  427,  415,  416,  399,  243,  246,  265,
 /*   110 */   274,  280,  277,  217,  283,   44,   46,   48,  222,   57,
 /*   120 */   167,  187,  176,   68,   72,   77,  109,  111,   84,  389,
 /*   130 */    17,  404,  420,   18,  417,  428,  392,   21,  286,  229,
 /*   140 */   238,  241,   52,  473,  256,  289,  405,   22,  426,  427,
 /*   150 */   415,  416,  399,  266,  273,  265,  274,  280,  277,  234,
 /*   160 */   283,   44,   46,   48,  166,   57,  233,  187,  168,   68,
 /*   170 */    72,   77,  178,  177,   84,  171,  172,  174,  173,  349,
 /*   180 */   296,  348,  331,  335,  286,  229,  238,  241,  315,   98,
 /*   190 */   256,  289,  430,   97,  433,  434,  420,  187,  417,  391,
 /*   200 */   392,  265,  274,  280,  277,   45,  283,   44,   46,   48,
 /*   210 */   322,   57,  348,  331,  335,   68,   72,   77,  169,  170,
 /*   220 */    84,  171,  172,  174,  173,   47,  418,  419,  415,  416,
 /*   230 */   286,  229,  238,  241,   56,   79,  256,  289,  477,   90,
 /*   240 */    12,  432,  436,  426,  427,  415,  416,  265,  274,  280,
 /*   250 */   277,  143,  283,   44,   46,   48,  406,   57,  417,  187,
 /*   260 */   190,   68,   72,   77,   49,  189,   84,  309,   91,   92,
 /*   270 */    93,   94,   95,  162,  209,  187,  286,  229,  238,  241,
 /*   280 */   310,  441,  256,  289,  405,   50,  426,  427,  415,  416,
 /*   290 */   399,   53,   87,  265,  274,  280,  277,  143,  283,   44,
 /*   300 */    46,   48,  445,   57,  449,  187,  184,   68,   72,   77,
 /*   310 */   448,  189,   84,  405,   86,  426,  427,  415,  416,  399,
 /*   320 */   275,  273,  286,  229,  238,  241,   71,  454,  256,  289,
 /*   330 */   405,   54,  426,  427,  415,  416,  399,   59,  161,  265,
 /*   340 */   274,  280,  277,  143,  283,   44,   46,   48,   90,   57,
 /*   350 */    63,  187,  201,   68,   72,   77,   65,  189,   84,  169,
 /*   360 */   170,  390,  171,  172,  174,  173,  391,  392,  286,  229,
 /*   370 */   238,  241,  308,   58,  256,  289,  309,   91,   92,   93,
 /*   380 */    94,   95,  457,  167,  187,  265,  274,  280,  277,   66,
 /*   390 */   283,   44,   46,   48,  365,   57,  348,  331,  335,   68,
 /*   400 */    72,   77,  232,    7,   84,    7,   10,   78,   10,   89,
 /*   410 */   442,  289,  442,  289,  286,  229,  238,  241,   76,   69,
 /*   420 */   256,  289,  234,  160,  231,  179,  164,  175,  230,  235,
 /*   430 */   187,  265,  274,  280,  277,   88,  283,   44,   46,   48,
 /*   440 */    74,   57,  126,  128,  130,   68,   72,   77,  476,   62,
 /*   450 */    84,  393,   96,   19,  380,  356,  371,  399,  105,  394,
 /*   460 */   286,  229,  238,  241,   83,  186,  256,  289,  418,  419,
 /*   470 */   415,  416,  394,   64,  186,  311,  167,  265,  274,  280,
 /*   480 */   277,  408,  283,   44,   46,   48,   98,   57,  409,  407,
 /*   490 */   208,   68,   72,   77,  187,   80,   84,  186,  413,  414,
 /*   500 */   415,  416,  185,  187,  104,  421,  286,  229,  238,  241,
 /*   510 */   295,  191,  256,  289,  413,  414,  415,  416,  163,  164,
 /*   520 */   175,  278,  273,  265,  274,  280,  277,  143,  283,   44,
 /*   530 */    46,   48,  204,   57,  202,  187,  262,   68,   72,   77,
 /*   540 */   187,  189,   84,  186,  431,  404,  420,  106,  417,  141,
 /*   550 */   392,  108,  286,  229,  238,  241,  302,  475,  256,  289,
 /*   560 */   355,  194,   19,  380,  327,  331,  335,  356,  371,  265,
 /*   570 */   274,  280,  277,  326,  283,   44,   46,   48,  110,   57,
 /*   580 */   263,  281,  273,   68,   72,   77,  479,  326,   84,  355,
 /*   590 */   480,   19,  380,  355,  112,   19,  380,  323,  286,  229,
 /*   600 */   238,  241,  307,  478,  256,  289,  355,  247,   19,  380,
 /*   610 */   142,  196,  198,  148,  147,  265,  274,  280,  277,  143,
 /*   620 */   283,   44,   46,   48,  350,   57,  149,  187,  101,   68,
 /*   630 */    72,   77,  474,  188,   84,  355,  187,   19,  380,  284,
 /*   640 */   273,  165,  181,  207,  286,  229,  238,  241,  321,   73,
 /*   650 */   256,  289,  309,  653,  654,  655,  182,  250,  253,  195,
 /*   660 */   187,  265,  274,  280,  277,  113,  283,   44,   46,   48,
 /*   670 */   206,   57,  211,  187,   98,   68,   72,   77,  212,   98,
 /*   680 */    84,   98,  187,  216,  197,  221,  215,  187,  220,  187,
 /*   690 */   286,  229,  238,  241,  320,   98,  256,  289,  115,  225,
 /*   700 */   199,  214,  223,  187,  224,  237,  187,  265,  274,  280,
 /*   710 */   277,  117,  283,   44,   46,   48,  119,   57,  242,  187,
 /*   720 */   121,   68,   72,   77,  187,  123,   84,  125,  187,  268,
 /*   730 */   244,  248,  245,  187,  249,  187,  286,  229,  238,  241,
 /*   740 */   325,  127,  256,  289,  129,  251,  252,  254,  255,  187,
 /*   750 */   258,  267,  187,  265,  274,  280,  277,  131,  283,   44,
 /*   760 */    46,   48,  260,   57,  264,  187,  133,   68,   72,   77,
 /*   770 */   135,  137,   84,  145,  187,  268,  261,  270,  187,  187,
 /*   780 */   326,  187,  286,  229,  238,  241,  352,  151,  256,  289,
 /*   790 */   239,  269,  271,  272,  287,  187,  312,  276,  187,  265,
 /*   800 */   274,  280,  277,  268,  283,   44,   46,   48,  288,   57,
 /*   810 */   297,  268,  292,   68,   72,   77,  268,  366,   84,  299,
 /*   820 */   187,  298,  318,  317,  329,  279,  330,  187,  286,  229,
 /*   830 */   238,  241,  354,  282,  256,  289,  304,  333,  285,  332,
 /*   840 */   334,  337,  422,  336,  187,  265,  274,  280,  277,  423,
 /*   850 */   283,   44,   46,   48,  338,   57,  340,  341,  520,   68,
 /*   860 */    72,   77,  521,  522,   84,  523,  524,  525,  526,  357,
 /*   870 */   358,  372,  351,  373,  286,  229,  238,  241,  362,  359,
 /*   880 */   256,  289,  361,  367,  376,  382,  374,  398,  397,  381,
 /*   890 */   395,  265,  274,  280,  277,  385,  283,   44,   46,   48,
 /*   900 */   375,   57,  383,  396,  384,   68,   72,   77,  400,  401,
 /*   910 */    84,  402,  403,  411,  412,  410,  424,  425,  437,  440,
 /*   920 */   286,  229,  238,  241,  364,  438,  256,  289,  443,  444,
 /*   930 */   450,  447,  453,  353,  451,  353,  353,  265,  274,  280,
 /*   940 */   277,  353,  283,   44,   46,   48,  353,   57,  353,  353,
 /*   950 */   353,   68,   72,   77,  353,  353,   84,  353,  353,  353,
 /*   960 */   353,  353,  353,  353,  353,  353,  286,  229,  238,  241,
 /*   970 */   368,  353,  256,  289,  353,  353,  353,  353,  353,  353,
 /*   980 */   353,  353,  353,  265,  274,  280,  277,  353,  283,   44,
 /*   990 */    46,   48,  353,   57,  353,  353,  353,   68,   72,   77,
 /*  1000 */   353,  353,   84,  353,  353,  353,  353,  353,  353,  353,
 /*  1010 */   353,  353,  286,  229,  238,  241,  370,  353,  256,  289,
 /*  1020 */   353,  353,  353,  353,  353,  353,  353,  353,  353,  265,
 /*  1030 */   274,  280,  277,  353,  283,   44,   46,   48,  353,   57,
 /*  1040 */   353,  353,  353,   68,   72,   77,  353,  353,   84,  353,
 /*  1050 */   353,  353,  353,  353,  353,  353,  353,  353,  286,  229,
 /*  1060 */   238,  241,  377,  353,  256,  289,  353,  353,  353,  353,
 /*  1070 */   353,  353,  353,  353,  353,  265,  274,  280,  277,  353,
 /*  1080 */   283,   44,   46,   48,  353,   57,  353,  353,  353,   68,
 /*  1090 */    72,   77,  353,  353,   84,  353,  353,  353,  353,  353,
 /*  1100 */   353,  353,  353,  353,  286,  229,  238,  241,  379,  353,
 /*  1110 */   256,  289,  353,  353,  353,  353,  353,  353,  353,  353,
 /*  1120 */   353,  265,  274,  280,  277,  353,  283,   44,   46,   48,
 /*  1130 */   353,   57,  353,  353,  353,   68,   72,   77,  353,  353,
 /*  1140 */    84,  353,  353,  353,  353,  353,  353,  353,  353,  353,
 /*  1150 */   286,  229,  238,  241,  386,  353,  256,  289,  353,  353,
 /*  1160 */   353,  353,  353,  353,  353,  353,  353,  265,  274,  280,
 /*  1170 */   277,  353,  283,   44,   46,   48,  353,   57,  353,  353,
 /*  1180 */   353,   68,   72,   77,  353,  353,   84,  353,  353,  353,
 /*  1190 */   353,  353,  353,  353,  353,  353,  286,  229,  238,  241,
 /*  1200 */   388,  353,  256,  289,  353,  353,  353,  353,  353,  353,
 /*  1210 */   353,  353,  353,  265,  274,  280,  277,  353,  283,   44,
 /*  1220 */    46,   48,  353,   57,  353,  353,  353,   68,   72,   77,
 /*  1230 */   353,  353,   84,  353,  353,  353,  353,  353,  353,  286,
 /*  1240 */   353,  353,  353,  229,  238,  241,  289,  353,  256,  353,
 /*  1250 */   353,  353,  353,  353,  353,  353,  265,  274,  280,  277,
 /*  1260 */   353,  283,   44,   46,   48,  353,   57,  353,  353,  353,
 /*  1270 */    68,   72,   77,  353,  353,   84,  353,  353,  353,  353,
 /*  1280 */   353,  353,  353,   43,  353,  353,  229,  238,  241,  353,
 /*  1290 */   353,  256,  353,  353,  353,   26,  353,  353,  353,  290,
 /*  1300 */    29,   30,   31,   32,   33,   34,   35,   36,   37,   38,
 /*  1310 */    39,   40,   41,   42,   43,  353,  353,   90,  353,  353,
 /*  1320 */   353,  353,  353,  353,  353,  236,   51,  353,  353,  353,
 /*  1330 */   290,   29,   30,   31,   32,   33,   34,   35,   36,   37,
 /*  1340 */    38,   39,   40,   41,   42,   43,   91,   92,   93,   94,
 /*  1350 */    95,  205,  218,  353,  226,  227,  236,   55,  353,  353,
 /*  1360 */   353,  290,   29,   30,   31,   32,   33,   34,   35,   36,
 /*  1370 */    37,   38,   39,   40,   41,   42,  353,  353,   43,  353,
 /*  1380 */   353,  353,  353,  353,  353,  353,  353,  236,  353,  353,
 /*  1390 */    67,  353,  353,  353,  290,   29,   30,   31,   32,   33,
 /*  1400 */    34,   35,   36,   37,   38,   39,   40,   41,   42,  353,
 /*  1410 */   353,  339,   43,  353,   13,   16,  429,  435,  434,  420,
 /*  1420 */   236,  417,  428,  392,   70,  353,  353,  328,  290,   29,
 /*  1430 */    30,   31,   32,   33,   34,   35,   36,   37,   38,   39,
 /*  1440 */    40,   41,   42,   43,  342,  343,  344,  345,  346,  347,
 /*  1450 */   353,  353,  353,  353,  236,   75,  353,  353,  353,  290,
 /*  1460 */    29,   30,   31,   32,   33,   34,   35,   36,   37,   38,
 /*  1470 */    39,   40,   41,   42,   43,  353,  353,  353,  353,  353,
 /*  1480 */   353,  353,  353,  353,  353,  236,   82,  353,  353,  353,
 /*  1490 */   290,   29,   30,   31,   32,   33,   34,   35,   36,   37,
 /*  1500 */    38,   39,   40,   41,   42,  353,  353,   43,  353,  353,
 /*  1510 */   353,  353,  353,  353,  353,  353,  236,  353,  353,  294,
 /*  1520 */   353,  353,  353,  290,   29,   30,   31,   32,   33,   34,
 /*  1530 */    35,   36,   37,   38,   39,   40,   41,   42,  353,  353,
 /*  1540 */   353,   43,  353,  439,   16,  429,  435,  434,  420,  236,
 /*  1550 */   417,  428,  392,  301,  353,  353,  353,  290,   29,   30,
 /*  1560 */    31,   32,   33,   34,   35,   36,   37,   38,   39,   40,
 /*  1570 */    41,   42,   43,  353,  353,  353,  353,  353,  353,  353,
 /*  1580 */   353,  353,  353,  236,  306,  353,  353,  353,  290,   29,
 /*  1590 */    30,   31,   32,   33,   34,   35,   36,   37,   38,   39,
 /*  1600 */    40,   41,   42,   43,  353,  353,  353,  353,  353,  353,
 /*  1610 */   353,  353,  353,  353,  236,  313,  353,  353,  353,  290,
 /*  1620 */    29,   30,   31,   32,   33,   34,   35,   36,   37,   38,
 /*  1630 */    39,   40,   41,   42,  353,  353,   43,  353,  353,  353,
 /*  1640 */   353,  353,  353,  353,  353,  236,  353,  353,  319,  353,
 /*  1650 */   353,  353,  290,   29,   30,   31,   32,   33,   34,   35,
 /*  1660 */    36,   37,   38,   39,   40,   41,   42,  353,  353,  353,
 /*  1670 */    43,  353,  446,   16,  429,  435,  434,  420,  236,  417,
 /*  1680 */   428,  392,  324,  353,  353,  353,  290,   29,   30,   31,
 /*  1690 */    32,   33,   34,   35,   36,   37,   38,   39,   40,   41,
 /*  1700 */    42,   43,  353,  353,  353,  353,  353,  353,  353,  353,
 /*  1710 */   353,  353,  236,  353,  353,  353,  353,  290,   29,   30,
 /*  1720 */    31,   32,   33,   34,   35,   36,   37,   38,   39,   40,
 /*  1730 */    41,   42,   43,  353,  353,  353,  353,  353,  353,  353,
 /*  1740 */   353,  353,  353,  236,  363,  353,  353,  353,  290,   29,
 /*  1750 */    30,   31,   32,   33,   34,   35,   36,   37,   38,   39,
 /*  1760 */    40,   41,   42,  353,  353,   43,  353,  353,  353,  353,
 /*  1770 */   353,  353,  353,  353,  236,  353,  353,  369,  353,  353,
 /*  1780 */   353,  290,   29,   30,   31,   32,   33,   34,   35,   36,
 /*  1790 */    37,   38,   39,   40,   41,   42,  353,  353,  353,   43,
 /*  1800 */   353,  452,   16,  429,  435,  434,  420,  236,  417,  428,
 /*  1810 */   392,  378,  353,  353,  353,  290,   29,   30,   31,   32,
 /*  1820 */    33,   34,   35,   36,   37,   38,   39,   40,   41,   42,
 /*  1830 */    43,  353,  353,  353,  353,  353,  353,  353,  353,  353,
 /*  1840 */   353,  236,  387,  353,  353,  353,  290,   29,   30,   31,
 /*  1850 */    32,   33,   34,   35,   36,   37,   38,   39,   40,   41,
 /*  1860 */    42,   43,  353,  353,  353,  353,  353,  353,  353,  353,
 /*  1870 */   353,  353,  236,  353,  353,  353,  353,   28,   29,   30,
 /*  1880 */    31,   32,   33,   34,   35,   36,   37,   38,   39,   40,
 /*  1890 */    41,   42,  353,  103,  146,  150,  144,  180,  100,  353,
 /*  1900 */   139,  353,  353,  236,  353,  353,  353,  103,  146,  150,
 /*  1910 */   144,  180,  100,  353,  139,  136,  103,  146,  150,  144,
 /*  1920 */   180,  100,  353,  139,  353,  339,  353,  353,  353,  136,
 /*  1930 */   192,  353,  353,  353,  353,  291,  353,  353,  136,  183,
 /*  1940 */   353,  328,  353,  158,   24,  353,  353,  353,  353,  353,
 /*  1950 */   152,  153,  353,  154,  155,  156,  157,  158,  342,  343,
 /*  1960 */   344,  345,  346,  347,  152,  153,  158,  154,  155,  156,
 /*  1970 */   157,  353,  353,  152,  153,  353,  154,  155,  156,  157,
 /*  1980 */   103,  146,  150,  144,  180,  100,  353,  139,  353,  353,
 /*  1990 */   353,  353,  353,  353,  103,  146,  150,  144,  180,  100,
 /*  2000 */   353,  139,  136,  203,  353,  353,  353,  353,  353,  353,
 /*  2010 */   103,  146,  150,  144,  180,  100,  136,  139,   99,  114,
 /*  2020 */   118,  120,  122,  124,  116,  102,  126,  128,  130,  353,
 /*  2030 */   158,  353,  136,  353,  353,  353,  353,  152,  153,  353,
 /*  2040 */   154,  155,  156,  157,  158,  210,  353,  353,  353,  353,
 /*  2050 */   353,  152,  153,  353,  154,  155,  156,  157,  353,  353,
 /*  2060 */   158,  219,  353,  353,  353,  353,  353,  152,  153,  353,
 /*  2070 */   154,  155,  156,  157,  353,  353,  103,  146,  150,  144,
 /*  2080 */   180,  100,  353,  139,  353,  353,  353,  353,  353,  353,
 /*  2090 */   353,  353,  103,  146,  150,  144,  180,  100,  136,  139,
 /*  2100 */   132,  134,   99,  114,  118,  120,  122,  124,  116,  102,
 /*  2110 */   126,  128,  130,  353,  136,  353,  353,  353,  303,  353,
 /*  2120 */   353,  353,  353,   81,  353,  353,  158,  353,  353,  353,
 /*  2130 */   353,  353,  353,  152,  153,  353,  154,  155,  156,  157,
 /*  2140 */   353,  353,  158,  353,  353,  353,  353,  353,  353,  152,
 /*  2150 */   153,  353,  154,  155,  156,  157,  132,  134,   99,  114,
 /*  2160 */   118,  120,  122,  124,  116,  102,  126,  128,  130,  132,
 /*  2170 */   134,   99,  114,  118,  120,  122,  124,  116,  102,  126,
 /*  2180 */   128,  130,  353,  353,  353,  353,  353,  353,  353,  353,
 /*  2190 */   353,  353,  138,  353,  353,  132,  134,   99,  114,  118,
 /*  2200 */   120,  122,  124,  116,  102,  126,  128,  130,  353,  353,
 /*  2210 */   353,  353,  353,  353,  353,  353,  240,  353,  353,  132,
 /*  2220 */   134,   99,  114,  118,  120,  122,  124,  116,  102,  126,
 /*  2230 */   128,  130,  353,  583,  353,  353,  353,  353,  353,  353,
 /*  2240 */   353,  353,  293,  353,  353,  132,  134,   99,  114,  118,
 /*  2250 */   120,  122,  124,  116,  102,  126,  128,  130,  353,  353,
 /*  2260 */   353,  353,  353,  353,  353,  353,  353,  353,  300,  353,
 /*  2270 */   353,  132,  134,   99,  114,  118,  120,  122,  124,  116,
 /*  2280 */   102,  126,  128,  130,  353,  353,  353,  353,  353,  353,
 /*  2290 */   353,  353,  353,  353,  305,  353,  353,  132,  134,   99,
 /*  2300 */   114,  118,  120,  122,  124,  116,  102,  126,  128,  130,
 /*  2310 */   353,  353,  353,  353,  353,  353,  353,  353,  353,  353,
 /*  2320 */   667,  353,  353,  132,  134,   99,  114,  118,  120,  122,
 /*  2330 */   124,  116,  102,  126,  128,  130,  339,  353,  353,  353,
 /*  2340 */   353,  353,  353,  353,  353,  353,  353,  353,  353,  353,
 /*  2350 */   353,  353,  328,  353,  353,  353,  353,  353,  353,  353,
 /*  2360 */   353,  353,  353,  353,  353,  353,  353,  353,  360,  342,
 /*  2370 */   343,  344,  345,  346,  347,
};
static YYCODETYPE yy_lookahead[] = {
 /*     0 */    22,    0,   26,   19,   26,    6,   22,   29,   80,   81,
 /*    10 */    82,   83,   84,   85,   86,  118,  119,   39,   40,   41,
 /*    20 */    42,   22,   44,   45,   46,   47,   50,   49,   52,   22,
 /*    30 */    22,   53,   54,   55,   37,   23,   58,   38,   39,   40,
 /*    40 */    41,   42,   43,   44,  121,  122,   22,   69,   70,   71,
 /*    50 */    26,   26,   74,   29,   29,   22,   31,   32,   33,   34,
 /*    60 */    35,   64,   65,   39,   40,   41,   42,  120,   44,   45,
 /*    70 */    46,   47,  125,   49,   66,  128,   22,   53,   54,   55,
 /*    80 */    72,   73,   58,   75,   76,   77,   78,   26,   83,   84,
 /*    90 */    85,   86,   22,   69,   70,   71,   26,   26,   74,   29,
 /*   100 */    29,   36,   31,   32,   33,   34,   35,   64,   65,   39,
 /*   110 */    40,   41,   42,  120,   44,   45,   46,   47,  125,   49,
 /*   120 */    93,  128,   22,   53,   54,   55,   72,   73,   58,   89,
 /*   130 */    90,   91,   92,   96,   94,   95,   96,   22,   22,   69,
 /*   140 */    70,   71,   26,   26,   74,   29,   29,   37,   31,   32,
 /*   150 */    33,   34,   35,  130,  131,   39,   40,   41,   42,  120,
 /*   160 */    44,   45,   46,   47,  137,   49,  127,  128,   22,   53,
 /*   170 */    54,   55,   72,   73,   58,   75,   76,   77,   78,   97,
 /*   180 */     1,   99,  100,  101,   22,   69,   70,   71,   26,  120,
 /*   190 */    74,   29,   88,  124,   90,   91,   92,  128,   94,   95,
 /*   200 */    96,   39,   40,   41,   42,   23,   44,   45,   46,   47,
 /*   210 */    97,   49,   99,  100,  101,   53,   54,   55,   72,   73,
 /*   220 */    58,   75,   76,   77,   78,   23,   31,   32,   33,   34,
 /*   230 */    22,   69,   70,   71,   26,   56,   74,   29,   26,   30,
 /*   240 */    25,   29,   27,   31,   32,   33,   34,   39,   40,   41,
 /*   250 */    42,  120,   44,   45,   46,   47,   92,   49,   94,  128,
 /*   260 */   129,   53,   54,   55,  117,  134,   58,  120,   59,   60,
 /*   270 */    61,   62,   63,    1,   65,  128,   22,   69,   70,   71,
 /*   280 */    26,   26,   74,   29,   29,   25,   31,   32,   33,   34,
 /*   290 */    35,   48,    1,   39,   40,   41,   42,  120,   44,   45,
 /*   300 */    46,   47,   25,   49,   27,  128,  129,   53,   54,   55,
 /*   310 */    26,  134,   58,   29,   23,   31,   32,   33,   34,   35,
 /*   320 */   130,  131,   22,   69,   70,   71,   26,   26,   74,   29,
 /*   330 */    29,   25,   31,   32,   33,   34,   35,   25,   66,   39,
 /*   340 */    40,   41,   42,  120,   44,   45,   46,   47,   30,   49,
 /*   350 */   119,  128,  129,   53,   54,   55,   93,  134,   58,   72,
 /*   360 */    73,   90,   75,   76,   77,   78,   95,   96,   22,   69,
 /*   370 */    70,   71,   26,  117,   74,   29,  120,   59,   60,   61,
 /*   380 */    62,   63,    0,   93,  128,   39,   40,   41,   42,   51,
 /*   390 */    44,   45,   46,   47,   97,   49,   99,  100,  101,   53,
 /*   400 */    54,   55,    1,   21,   58,   21,   24,   22,   24,   22,
 /*   410 */    28,   29,   28,   29,   22,   69,   70,   71,   26,   25,
 /*   420 */    74,   29,  120,  133,   23,  135,  136,  137,  126,  127,
 /*   430 */   128,   39,   40,   41,   42,  122,   44,   45,   46,   47,
 /*   440 */    25,   49,   12,   13,   14,   53,   54,   55,   26,   26,
 /*   450 */    58,   29,  123,   31,   32,   31,   32,   35,   22,   35,
 /*   460 */    22,   69,   70,   71,   26,    1,   74,   29,   31,   32,
 /*   470 */    33,   34,   35,   50,    1,   52,   93,   39,   40,   41,
 /*   480 */    42,   23,   44,   45,   46,   47,  120,   49,   30,   22,
 /*   490 */   124,   53,   54,   55,  128,  120,   58,    1,   31,   32,
 /*   500 */    33,   34,   38,  128,  132,   22,   22,   69,   70,   71,
 /*   510 */    26,   38,   74,   29,   31,   32,   33,   34,  135,  136,
 /*   520 */   137,  130,  131,   39,   40,   41,   42,  120,   44,   45,
 /*   530 */    46,   47,  120,   49,   38,  128,  129,   53,   54,   55,
 /*   540 */   128,  134,   58,    1,   90,   91,   92,   65,   94,   22,
 /*   550 */    96,   66,   22,   69,   70,   71,   26,   26,   74,   29,
 /*   560 */    29,   22,   31,   32,   99,  100,  101,   31,   32,   39,
 /*   570 */    40,   41,   42,    1,   44,   45,   46,   47,   66,   49,
 /*   580 */    38,  130,  131,   53,   54,   55,   26,    1,   58,   29,
 /*   590 */    26,   31,   32,   29,   66,   31,   32,   25,   22,   69,
 /*   600 */    70,   71,   26,   26,   74,   29,   29,   22,   31,   32,
 /*   610 */    37,   72,   73,    1,   22,   39,   40,   41,   42,  120,
 /*   620 */    44,   45,   46,   47,   38,   49,  132,  128,  120,   53,
 /*   630 */    54,   55,   26,  134,   58,   29,  128,   31,   32,  130,
 /*   640 */   131,   51,   22,  123,   22,   69,   70,   71,   26,  117,
 /*   650 */    74,   29,  120,   51,   51,   51,   37,   72,   73,   66,
 /*   660 */   128,   39,   40,   41,   42,  120,   44,   45,   46,   47,
 /*   670 */    22,   49,  123,  128,  120,   53,   54,   55,  124,  120,
 /*   680 */    58,  120,  128,  124,   66,  124,  123,  128,  123,  128,
 /*   690 */    22,   69,   70,   71,   26,  120,   74,   29,  120,  124,
 /*   700 */    66,   66,   66,  128,  123,   23,  128,   39,   40,   41,
 /*   710 */    42,  120,   44,   45,   46,   47,  120,   49,   22,  128,
 /*   720 */   120,   53,   54,   55,  128,  120,   58,  120,  128,    1,
 /*   730 */    22,   66,   23,  128,   23,  128,   22,   69,   70,   71,
 /*   740 */    26,  120,   74,   29,  120,   66,   23,   66,   23,  128,
 /*   750 */    23,   23,  128,   39,   40,   41,   42,  120,   44,   45,
 /*   760 */    46,   47,   22,   49,   23,  128,  120,   53,   54,   55,
 /*   770 */   120,  120,   58,  120,  128,    1,   37,   22,  128,  128,
 /*   780 */     1,  128,   22,   69,   70,   71,   26,  120,   74,   29,
 /*   790 */   120,  131,   30,   93,   64,  128,   51,   23,  128,   39,
 /*   800 */    40,   41,   42,    1,   44,   45,   46,   47,   22,   49,
 /*   810 */    22,    1,  120,   53,   54,   55,    1,   38,   58,  120,
 /*   820 */   128,   56,   25,   48,   30,   23,   93,  128,   22,   69,
 /*   830 */    70,   71,   26,   23,   74,   29,  120,   30,   23,   22,
 /*   840 */    93,   30,   23,   22,  128,   39,   40,   41,   42,   30,
 /*   850 */    44,   45,   46,   47,   93,   49,   22,    7,   22,   53,
 /*   860 */    54,   55,   22,   22,   58,   22,   22,   22,   22,   36,
 /*   870 */    22,   36,   25,   22,   22,   69,   70,   71,   26,   37,
 /*   880 */    74,   29,   25,   25,   25,   22,   37,   23,   93,   36,
 /*   890 */    22,   39,   40,   41,   42,   25,   44,   45,   46,   47,
 /*   900 */    38,   49,   37,   30,   38,   53,   54,   55,   22,   30,
 /*   910 */    58,   93,   23,   23,   94,   93,   93,   23,   22,   26,
 /*   920 */    22,   69,   70,   71,   26,   25,   74,   29,   24,   22,
 /*   930 */    22,   26,   26,  138,   25,  138,  138,   39,   40,   41,
 /*   940 */    42,  138,   44,   45,   46,   47,  138,   49,  138,  138,
 /*   950 */   138,   53,   54,   55,  138,  138,   58,  138,  138,  138,
 /*   960 */   138,  138,  138,  138,  138,  138,   22,   69,   70,   71,
 /*   970 */    26,  138,   74,   29,  138,  138,  138,  138,  138,  138,
 /*   980 */   138,  138,  138,   39,   40,   41,   42,  138,   44,   45,
 /*   990 */    46,   47,  138,   49,  138,  138,  138,   53,   54,   55,
 /*  1000 */   138,  138,   58,  138,  138,  138,  138,  138,  138,  138,
 /*  1010 */   138,  138,   22,   69,   70,   71,   26,  138,   74,   29,
 /*  1020 */   138,  138,  138,  138,  138,  138,  138,  138,  138,   39,
 /*  1030 */    40,   41,   42,  138,   44,   45,   46,   47,  138,   49,
 /*  1040 */   138,  138,  138,   53,   54,   55,  138,  138,   58,  138,
 /*  1050 */   138,  138,  138,  138,  138,  138,  138,  138,   22,   69,
 /*  1060 */    70,   71,   26,  138,   74,   29,  138,  138,  138,  138,
 /*  1070 */   138,  138,  138,  138,  138,   39,   40,   41,   42,  138,
 /*  1080 */    44,   45,   46,   47,  138,   49,  138,  138,  138,   53,
 /*  1090 */    54,   55,  138,  138,   58,  138,  138,  138,  138,  138,
 /*  1100 */   138,  138,  138,  138,   22,   69,   70,   71,   26,  138,
 /*  1110 */    74,   29,  138,  138,  138,  138,  138,  138,  138,  138,
 /*  1120 */   138,   39,   40,   41,   42,  138,   44,   45,   46,   47,
 /*  1130 */   138,   49,  138,  138,  138,   53,   54,   55,  138,  138,
 /*  1140 */    58,  138,  138,  138,  138,  138,  138,  138,  138,  138,
 /*  1150 */    22,   69,   70,   71,   26,  138,   74,   29,  138,  138,
 /*  1160 */   138,  138,  138,  138,  138,  138,  138,   39,   40,   41,
 /*  1170 */    42,  138,   44,   45,   46,   47,  138,   49,  138,  138,
 /*  1180 */   138,   53,   54,   55,  138,  138,   58,  138,  138,  138,
 /*  1190 */   138,  138,  138,  138,  138,  138,   22,   69,   70,   71,
 /*  1200 */    26,  138,   74,   29,  138,  138,  138,  138,  138,  138,
 /*  1210 */   138,  138,  138,   39,   40,   41,   42,  138,   44,   45,
 /*  1220 */    46,   47,  138,   49,  138,  138,  138,   53,   54,   55,
 /*  1230 */   138,  138,   58,  138,  138,  138,  138,  138,  138,   22,
 /*  1240 */   138,  138,  138,   69,   70,   71,   29,  138,   74,  138,
 /*  1250 */   138,  138,  138,  138,  138,  138,   39,   40,   41,   42,
 /*  1260 */   138,   44,   45,   46,   47,  138,   49,  138,  138,  138,
 /*  1270 */    53,   54,   55,  138,  138,   58,  138,  138,  138,  138,
 /*  1280 */   138,  138,  138,   86,  138,  138,   69,   70,   71,  138,
 /*  1290 */   138,   74,  138,  138,  138,   98,  138,  138,  138,  102,
 /*  1300 */   103,  104,  105,  106,  107,  108,  109,  110,  111,  112,
 /*  1310 */   113,  114,  115,  116,   86,  138,  138,   30,  138,  138,
 /*  1320 */   138,  138,  138,  138,  138,  128,   98,  138,  138,  138,
 /*  1330 */   102,  103,  104,  105,  106,  107,  108,  109,  110,  111,
 /*  1340 */   112,  113,  114,  115,  116,   86,   59,   60,   61,   62,
 /*  1350 */    63,   64,   65,  138,   67,   68,  128,   98,  138,  138,
 /*  1360 */   138,  102,  103,  104,  105,  106,  107,  108,  109,  110,
 /*  1370 */   111,  112,  113,  114,  115,  116,  138,  138,   86,  138,
 /*  1380 */   138,  138,  138,  138,  138,  138,  138,  128,  138,  138,
 /*  1390 */    98,  138,  138,  138,  102,  103,  104,  105,  106,  107,
 /*  1400 */   108,  109,  110,  111,  112,  113,  114,  115,  116,  138,
 /*  1410 */   138,    6,   86,  138,   87,   88,   89,   90,   91,   92,
 /*  1420 */   128,   94,   95,   96,   98,  138,  138,   22,  102,  103,
 /*  1430 */   104,  105,  106,  107,  108,  109,  110,  111,  112,  113,
 /*  1440 */   114,  115,  116,   86,   39,   40,   41,   42,   43,   44,
 /*  1450 */   138,  138,  138,  138,  128,   98,  138,  138,  138,  102,
 /*  1460 */   103,  104,  105,  106,  107,  108,  109,  110,  111,  112,
 /*  1470 */   113,  114,  115,  116,   86,  138,  138,  138,  138,  138,
 /*  1480 */   138,  138,  138,  138,  138,  128,   98,  138,  138,  138,
 /*  1490 */   102,  103,  104,  105,  106,  107,  108,  109,  110,  111,
 /*  1500 */   112,  113,  114,  115,  116,  138,  138,   86,  138,  138,
 /*  1510 */   138,  138,  138,  138,  138,  138,  128,  138,  138,   98,
 /*  1520 */   138,  138,  138,  102,  103,  104,  105,  106,  107,  108,
 /*  1530 */   109,  110,  111,  112,  113,  114,  115,  116,  138,  138,
 /*  1540 */   138,   86,  138,   87,   88,   89,   90,   91,   92,  128,
 /*  1550 */    94,   95,   96,   98,  138,  138,  138,  102,  103,  104,
 /*  1560 */   105,  106,  107,  108,  109,  110,  111,  112,  113,  114,
 /*  1570 */   115,  116,   86,  138,  138,  138,  138,  138,  138,  138,
 /*  1580 */   138,  138,  138,  128,   98,  138,  138,  138,  102,  103,
 /*  1590 */   104,  105,  106,  107,  108,  109,  110,  111,  112,  113,
 /*  1600 */   114,  115,  116,   86,  138,  138,  138,  138,  138,  138,
 /*  1610 */   138,  138,  138,  138,  128,   98,  138,  138,  138,  102,
 /*  1620 */   103,  104,  105,  106,  107,  108,  109,  110,  111,  112,
 /*  1630 */   113,  114,  115,  116,  138,  138,   86,  138,  138,  138,
 /*  1640 */   138,  138,  138,  138,  138,  128,  138,  138,   98,  138,
 /*  1650 */   138,  138,  102,  103,  104,  105,  106,  107,  108,  109,
 /*  1660 */   110,  111,  112,  113,  114,  115,  116,  138,  138,  138,
 /*  1670 */    86,  138,   87,   88,   89,   90,   91,   92,  128,   94,
 /*  1680 */    95,   96,   98,  138,  138,  138,  102,  103,  104,  105,
 /*  1690 */   106,  107,  108,  109,  110,  111,  112,  113,  114,  115,
 /*  1700 */   116,   86,  138,  138,  138,  138,  138,  138,  138,  138,
 /*  1710 */   138,  138,  128,   98,  138,  138,  138,  102,  103,  104,
 /*  1720 */   105,  106,  107,  108,  109,  110,  111,  112,  113,  114,
 /*  1730 */   115,  116,   86,  138,  138,  138,  138,  138,  138,  138,
 /*  1740 */   138,  138,  138,  128,   98,  138,  138,  138,  102,  103,
 /*  1750 */   104,  105,  106,  107,  108,  109,  110,  111,  112,  113,
 /*  1760 */   114,  115,  116,  138,  138,   86,  138,  138,  138,  138,
 /*  1770 */   138,  138,  138,  138,  128,  138,  138,   98,  138,  138,
 /*  1780 */   138,  102,  103,  104,  105,  106,  107,  108,  109,  110,
 /*  1790 */   111,  112,  113,  114,  115,  116,  138,  138,  138,   86,
 /*  1800 */   138,   87,   88,   89,   90,   91,   92,  128,   94,   95,
 /*  1810 */    96,   98,  138,  138,  138,  102,  103,  104,  105,  106,
 /*  1820 */   107,  108,  109,  110,  111,  112,  113,  114,  115,  116,
 /*  1830 */    86,  138,  138,  138,  138,  138,  138,  138,  138,  138,
 /*  1840 */   138,  128,   98,  138,  138,  138,  102,  103,  104,  105,
 /*  1850 */   106,  107,  108,  109,  110,  111,  112,  113,  114,  115,
 /*  1860 */   116,   86,  138,  138,  138,  138,  138,  138,  138,  138,
 /*  1870 */   138,  138,  128,  138,  138,  138,  138,  102,  103,  104,
 /*  1880 */   105,  106,  107,  108,  109,  110,  111,  112,  113,  114,
 /*  1890 */   115,  116,  138,   15,   16,   17,   18,   19,   20,  138,
 /*  1900 */    22,  138,  138,  128,  138,  138,  138,   15,   16,   17,
 /*  1910 */    18,   19,   20,  138,   22,   37,   15,   16,   17,   18,
 /*  1920 */    19,   20,  138,   22,  138,    6,  138,  138,  138,   37,
 /*  1930 */    38,  138,  138,  138,  138,   57,  138,  138,   37,   38,
 /*  1940 */   138,   22,  138,   65,   25,  138,  138,  138,  138,  138,
 /*  1950 */    72,   73,  138,   75,   76,   77,   78,   65,   39,   40,
 /*  1960 */    41,   42,   43,   44,   72,   73,   65,   75,   76,   77,
 /*  1970 */    78,  138,  138,   72,   73,  138,   75,   76,   77,   78,
 /*  1980 */    15,   16,   17,   18,   19,   20,  138,   22,  138,  138,
 /*  1990 */   138,  138,  138,  138,   15,   16,   17,   18,   19,   20,
 /*  2000 */   138,   22,   37,   38,  138,  138,  138,  138,  138,  138,
 /*  2010 */    15,   16,   17,   18,   19,   20,   37,   22,    4,    5,
 /*  2020 */     6,    7,    8,    9,   10,   11,   12,   13,   14,  138,
 /*  2030 */    65,  138,   37,  138,  138,  138,  138,   72,   73,  138,
 /*  2040 */    75,   76,   77,   78,   65,   66,  138,  138,  138,  138,
 /*  2050 */   138,   72,   73,  138,   75,   76,   77,   78,  138,  138,
 /*  2060 */    65,   66,  138,  138,  138,  138,  138,   72,   73,  138,
 /*  2070 */    75,   76,   77,   78,  138,  138,   15,   16,   17,   18,
 /*  2080 */    19,   20,  138,   22,  138,  138,  138,  138,  138,  138,
 /*  2090 */   138,  138,   15,   16,   17,   18,   19,   20,   37,   22,
 /*  2100 */     2,    3,    4,    5,    6,    7,    8,    9,   10,   11,
 /*  2110 */    12,   13,   14,  138,   37,  138,  138,  138,   57,  138,
 /*  2120 */   138,  138,  138,   25,  138,  138,   65,  138,  138,  138,
 /*  2130 */   138,  138,  138,   72,   73,  138,   75,   76,   77,   78,
 /*  2140 */   138,  138,   65,  138,  138,  138,  138,  138,  138,   72,
 /*  2150 */    73,  138,   75,   76,   77,   78,    2,    3,    4,    5,
 /*  2160 */     6,    7,    8,    9,   10,   11,   12,   13,   14,    2,
 /*  2170 */     3,    4,    5,    6,    7,    8,    9,   10,   11,   12,
 /*  2180 */    13,   14,  138,  138,  138,  138,  138,  138,  138,  138,
 /*  2190 */   138,  138,   38,  138,  138,    2,    3,    4,    5,    6,
 /*  2200 */     7,    8,    9,   10,   11,   12,   13,   14,  138,  138,
 /*  2210 */   138,  138,  138,  138,  138,  138,   23,  138,  138,    2,
 /*  2220 */     3,    4,    5,    6,    7,    8,    9,   10,   11,   12,
 /*  2230 */    13,   14,  138,   66,  138,  138,  138,  138,  138,  138,
 /*  2240 */   138,  138,   25,  138,  138,    2,    3,    4,    5,    6,
 /*  2250 */     7,    8,    9,   10,   11,   12,   13,   14,  138,  138,
 /*  2260 */   138,  138,  138,  138,  138,  138,  138,  138,   25,  138,
 /*  2270 */   138,    2,    3,    4,    5,    6,    7,    8,    9,   10,
 /*  2280 */    11,   12,   13,   14,  138,  138,  138,  138,  138,  138,
 /*  2290 */   138,  138,  138,  138,   25,  138,  138,    2,    3,    4,
 /*  2300 */     5,    6,    7,    8,    9,   10,   11,   12,   13,   14,
 /*  2310 */   138,  138,  138,  138,  138,  138,  138,  138,  138,  138,
 /*  2320 */    25,  138,  138,    2,    3,    4,    5,    6,    7,    8,
 /*  2330 */     9,   10,   11,   12,   13,   14,    6,  138,  138,  138,
 /*  2340 */   138,  138,  138,  138,  138,  138,  138,  138,  138,  138,
 /*  2350 */   138,  138,   22,  138,  138,  138,  138,  138,  138,  138,
 /*  2360 */   138,  138,  138,  138,  138,  138,  138,  138,   38,   39,
 /*  2370 */    40,   41,   42,   43,   44,
};
#define YY_SHIFT_USE_DFLT (-25)
static short yy_shift_ofst[] = {
 /*     0 */   384,    1,  382,  -25,  -25,  -25,  -25,    7,   12,  -25,
 /*    10 */    33,  215,   25,   61,  -25,  -25,   71,  531,  -25,   65,
 /*    20 */   115,  110,   -1, 1919,  -22,  -25,   24,  -25,  -25,  -25,
 /*    30 */   -25,  -25,  -25,  -25,  -25,  -25,  -25,  -25,  -25,  -25,
 /*    40 */   -25,  -25,  -25,  -25,  182,  -25,  202,  -25, 2077,  260,
 /*    50 */    70,  116,  243,  306,  162,  208,  -25, 2077,  312,  -24,
 /*    60 */   -25,  423,  -25,  -25,  287,  338, 1217, 1217,  394,  254,
 /*    70 */   300,  -25, 2077,  415,  346,  392,  -25,  385,  179, 1878,
 /*    80 */  2098, 1217,  438,  -25,  387,  291,  -25,  387,  -25, 1287,
 /*    90 */   -25,  -25,  -25,  -25,  -25,  -25, 2077,  -25, 2321, 2077,
 /*   100 */  2077,  -25, 2077,  436,  -25,  482,   54,  485,  -25,  512,
 /*   110 */   -25,  528,  -25,  430, 2077,  430, 2077,  430, 2077,  430,
 /*   120 */  2077,  430, 2077,  430, 2077,  430, 2077,  -25, 2077,  -25,
 /*   130 */  2077,  -25, 2077, 2014, 2077, 2014, 2077, 2154,  -25,   -3,
 /*   140 */   527,  573, 1892, 2321, 2077,  -25,  592,  612,  436,  -25,
 /*   150 */  2077,  -25,  -25,  -25,  -25,  -25,  -25,  -25,    8,  -25,
 /*   160 */   272,  -25,  100,  -25,  590,  146,  -25,  -25,  -25,  -25,
 /*   170 */   -25,  -25,  -25,  -25,  -25,  -25,  602,  603,  604,  -25,
 /*   180 */   620,  619, 1901,  -25,  464,  -25, 2077,  -25,  -25,  -25,
 /*   190 */   473,  -25,  -25,  539,  593,  -25,  618,  -25,  634,  -25,
 /*   200 */  1965,  496,  -25,  -25,  430,  648,  209, 2077,  -25, 1979,
 /*   210 */   318, 2077,  -25,  635,  318, 2077,  -25, 2167, 1995,  318,
 /*   220 */  2077,  -25,  636,  318, 2077,  -25,  -25,  -25,  -25, 2077,
 /*   230 */   401,  -25, 2077,  -25, 2321,  -25,  682,  -25, 2077, 2193,
 /*   240 */   -25,  696,   43,  708,  709,  -25,  585,  665,  711,  -25,
 /*   250 */   679,  723,  -25,  681,  725,  -25,  -16,  727,  -25,  740,
 /*   260 */   739, 2077,  542,  741,  -25,  755,  728,  -25,  755,  -25,
 /*   270 */   762,  287,  -25,  -25,  755,  774,  -25,  755,  802,  -25,
 /*   280 */   755,  810,  -25,  755,  815,  -25,  730,  786,  573,  -25,
 /*   290 */   -25, 2077, 2217, 1217,  484,  -25,  788,  765, 2061, 2243,
 /*   300 */  1217,  530,  -25, 2077, 2269, 1217,  576,  -25,  -25, 2295,
 /*   310 */   -25,  745, 1217, 1217,  -25,  -25,  775,  797,  622,  668,
 /*   320 */   -25,  -25,  572, 1217,  714,  -25, 1405,  -25,  794,  287,
 /*   330 */   -25,  817,  807,  287,  -25,  821,  811,  287,  -25,  834,
 /*   340 */   850,  836,  840,  841,  843,  844,  845,  846,  -25,  586,
 /*   350 */   847,  760,  -25,  806,  -25,  536,  833,  848,  842, 2330,
 /*   360 */   857,  852,  -25,  898,  -25,  779,  858,  944,  -25,  990,
 /*   370 */   -25,  835,  851,  849,  862,  859, 1036,  -25, 1082,  -25,
 /*   380 */   853,  863,  865,  866,  870, 1128,  -25, 1174,  -25,  422,
 /*   390 */   560,  -25,  -25,  424,  868,  873,  287,  864,  -25,  886,
 /*   400 */   879,  287,  889,  -25,  -25,  437,  467,  458,  -25,  287,
 /*   410 */   890,  -25,  -25,  -25,  -25,  -25,  -25,  -25,  833,  835,
 /*   420 */   483,  819,  -25,  287,  894,  -25,   65,  853,  -25,  117,
 /*   430 */   212,  564,  195,  577,  -25,  606,  896,  900,  255,  893,
 /*   440 */   -25,  -25,  904,  907,  277,  284,  905,  -25,  -25,  908,
 /*   450 */   909,  301,  906,  -25,  -25,  -25,
};
#define YY_REDUCE_USE_DFLT (-104)
static short yy_reduce_ofst[] = {
 /*     0 */   -72, -104,    5, -104, -104, -104, -104, -104, -104, -104,
 /*    10 */  -104, -104, 1327, -104, -104, -104,   40,   37, -104, -104,
 /*    20 */  -104, -104,   82,  113, 1197, -104, 1775, -104, -104, -104,
 /*    30 */  -104, -104, -104, -104, -104, -104, -104, -104, -104, -104,
 /*    40 */  -104, -104, -104, -104, -104, -104, -104, -104,  147, -104,
 /*    50 */  1228, 1775, -104, -104, 1259, 1775, -104,  256, -104, -103,
 /*    60 */  -104,  231, -104, -104,  263, -104, 1292, 1775, -104, 1326,
 /*    70 */  1775, -104,  532, -104, 1357, 1775, -104, -104, -104,  375,
 /*    80 */  -104, 1388, 1775, -104,  -77, -104, -104,  313, -104,  329,
 /*    90 */  -104, -104, -104, -104, -104, -104,   69, -104, -104,  412,
 /*   100 */   508, -104,  545,  372, -104, -104, -104, -104, -104, -104,
 /*   110 */  -104, -104, -104, -104,  578, -104,  591, -104,  596, -104,
 /*   120 */   600, -104,  605, -104,  607, -104,  621, -104,  624, -104,
 /*   130 */   637, -104,  646, -104,  650, -104,  651, -104, -104, -104,
 /*   140 */  -104, -104,  131, -104,  653, -104, -104, -104,  494, -104,
 /*   150 */   667, -104, -104, -104, -104, -104, -104, -104,  290, -104,
 /*   160 */  -104, -104,  383, -104, -104,   27, -104, -104, -104, -104,
 /*   170 */  -104, -104, -104, -104, -104, -104, -104, -104, -104, -104,
 /*   180 */  -104, -104,  177, -104, -104, -104,  499, -104, -104, -104,
 /*   190 */  -104, -104, -104, -104, -104, -104, -104, -104, -104, -104,
 /*   200 */   223, -104, -104, -104, -104, -104,  520,  366, -104,  -53,
 /*   210 */   549,  554, -104, -104,  563,  559, -104, -104,   -7,  565,
 /*   220 */   561, -104, -104,  581,  575, -104, -104, -104, -104,  302,
 /*   230 */  -104, -104,   39, -104, -104, -104, -104, -104,  670, -104,
 /*   240 */  -104, -104, -104, -104, -104, -104, -104, -104, -104, -104,
 /*   250 */  -104, -104, -104, -104, -104, -104, -104, -104, -104, -104,
 /*   260 */  -104,  407, -104, -104, -104,   23, -104, -104,  660, -104,
 /*   270 */  -104,  700, -104, -104,  190, -104, -104,  391, -104, -104,
 /*   280 */   451, -104, -104,  509, -104, -104, -104, -104, -104, -104,
 /*   290 */  -104,  692, -104, 1421, 1775, -104, -104, -104,  699, -104,
 /*   300 */  1455, 1775, -104,  716, -104, 1486, 1775, -104, -104, -104,
 /*   310 */  -104, -104, 1517, 1775, -104, -104, -104, -104, 1550, 1775,
 /*   320 */  -104, -104, -104, 1584, 1775, -104,  465, -104, -104,  733,
 /*   330 */  -104, -104, -104,  747, -104, -104, -104,  761, -104, -104,
 /*   340 */  -104, -104, -104, -104, -104, -104, -104, -104, -104, -104,
 /*   350 */  -104, 1615, -104, 1775, -104, -104, -104, -104, -104,  297,
 /*   360 */  -104, 1646, -104, 1775, -104, -104, -104, 1679, -104, 1775,
 /*   370 */  -104, -104, -104, -104, -104, -104, 1713, -104, 1775, -104,
 /*   380 */  -104, -104, -104, -104, -104, 1744, -104, 1775, -104,  271,
 /*   390 */    37, -104, -104, -104, -104, -104,  795, -104, -104, -104,
 /*   400 */  -104,  818, -104, -104, -104,  164,  820, -104, -104,  822,
 /*   410 */  -104, -104, -104, -104, -104, -104, -104, -104, -104, -104,
 /*   420 */   820, -104, -104,  823, -104, -104, -104, -104, -104,  104,
 /*   430 */   454,   37,  164,   37, -104,   37, -104, -104, 1456, -104,
 /*   440 */  -104, -104, -104, -104, -104, 1585, -104, -104, -104, -104,
 /*   450 */  -104, 1714, -104, -104, -104, -104,
};
static YYACTIONTYPE yy_default[] = {
 /*     0 */   669,  669,  669,  458,  460,  461,  462,  669,  669,  463,
 /*    10 */   669,  669,  669,  669,  464,  468,  669,  669,  495,  669,
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
 /*   400 */   669,  669,  669,  498,  481,  669,  669,  669,  483,  669,
 /*   410 */   669,  485,  487,  489,  490,  491,  492,  488,  489,  490,
 /*   420 */   669,  669,  484,  669,  669,  486,  489,  490,  494,  669,
 /*   430 */   669,  669,  669,  669,  482,  669,  669,  669,  669,  669,
 /*   440 */   465,  469,  669,  669,  669,  669,  669,  466,  470,  669,
 /*   450 */   669,  669,  669,  467,  471,  459,
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
  "ABSTRACT",      "COMMENT",       "ASSIGN",        "PUBLIC",      
  "PROTECTED",     "STATIC",        "PRIVATE",       "CONST",       
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
  "xx_property_visibility_list",  "xx_literal_expr",  "xx_property_visibility",  "xx_class_const_definition",
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
 /*  43 */ "xx_class_method_definition ::= PUBLIC FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  44 */ "xx_class_method_definition ::= PUBLIC FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  45 */ "xx_class_method_definition ::= COMMENT PUBLIC FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  46 */ "xx_class_method_definition ::= COMMENT PUBLIC FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  47 */ "xx_class_method_definition ::= PUBLIC FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  48 */ "xx_class_method_definition ::= PUBLIC FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE xx_parameter_list BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  49 */ "xx_class_method_definition ::= COMMENT PUBLIC FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  50 */ "xx_class_method_definition ::= COMMENT PUBLIC FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  51 */ "xx_class_method_definition ::= PUBLIC FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  52 */ "xx_class_method_definition ::= PROTECTED FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  53 */ "xx_class_method_definition ::= COMMENT PROTECTED FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  54 */ "xx_class_method_definition ::= PROTECTED FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  55 */ "xx_class_method_definition ::= COMMENT PROTECTED FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  56 */ "xx_parameter_list ::= xx_parameter_list COMMA xx_parameter",
 /*  57 */ "xx_parameter_list ::= xx_parameter",
 /*  58 */ "xx_parameter ::= IDENTIFIER",
 /*  59 */ "xx_parameter ::= xx_parameter_type IDENTIFIER",
 /*  60 */ "xx_parameter ::= xx_parameter_cast IDENTIFIER",
 /*  61 */ "xx_parameter ::= IDENTIFIER ASSIGN xx_literal_expr",
 /*  62 */ "xx_parameter ::= xx_parameter_type IDENTIFIER ASSIGN xx_literal_expr",
 /*  63 */ "xx_parameter ::= xx_parameter_cast IDENTIFIER ASSIGN xx_literal_expr",
 /*  64 */ "xx_parameter_cast ::= LESS IDENTIFIER GREATER",
 /*  65 */ "xx_parameter_type ::= TYPE_INTEGER",
 /*  66 */ "xx_parameter_type ::= TYPE_DOUBLE",
 /*  67 */ "xx_parameter_type ::= TYPE_BOOL",
 /*  68 */ "xx_parameter_type ::= TYPE_STRING",
 /*  69 */ "xx_parameter_type ::= TYPE_ARRAY",
 /*  70 */ "xx_parameter_type ::= TYPE_VAR",
 /*  71 */ "xx_statement_list ::= xx_statement_list xx_statement",
 /*  72 */ "xx_statement_list ::= xx_statement",
 /*  73 */ "xx_statement ::= xx_let_statement",
 /*  74 */ "xx_statement ::= xx_if_statement",
 /*  75 */ "xx_statement ::= xx_loop_statement",
 /*  76 */ "xx_statement ::= xx_echo_statement",
 /*  77 */ "xx_statement ::= xx_return_statement",
 /*  78 */ "xx_statement ::= xx_mcall_statement",
 /*  79 */ "xx_statement ::= xx_unset_statement",
 /*  80 */ "xx_statement ::= xx_throw_statement",
 /*  81 */ "xx_statement ::= xx_declare_statement",
 /*  82 */ "xx_statement ::= xx_break_statement",
 /*  83 */ "xx_statement ::= xx_continue_statement",
 /*  84 */ "xx_statement ::= xx_while_statement",
 /*  85 */ "xx_statement ::= xx_switch_statement",
 /*  86 */ "xx_statement ::= xx_for_statement",
 /*  87 */ "xx_statement ::= xx_comment",
 /*  88 */ "xx_break_statement ::= BREAK DOTCOMMA",
 /*  89 */ "xx_continue_statement ::= CONTINUE DOTCOMMA",
 /*  90 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /*  91 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE ELSE BRACKET_OPEN BRACKET_CLOSE",
 /*  92 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  93 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE ELSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  94 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE ELSE BRACKET_OPEN BRACKET_CLOSE",
 /*  95 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE ELSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  96 */ "xx_switch_statement ::= SWITCH xx_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /*  97 */ "xx_switch_statement ::= SWITCH xx_eval_expr BRACKET_OPEN xx_case_clauses BRACKET_CLOSE",
 /*  98 */ "xx_case_clauses ::= xx_case_clauses xx_case_clause",
 /*  99 */ "xx_case_clauses ::= xx_case_clause",
 /* 100 */ "xx_case_clause ::= CASE xx_literal_expr COLON xx_statement_list",
 /* 101 */ "xx_case_clause ::= DEFAULT COLON xx_statement_list",
 /* 102 */ "xx_loop_statement ::= LOOP BRACKET_OPEN BRACKET_CLOSE",
 /* 103 */ "xx_loop_statement ::= LOOP BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 104 */ "xx_while_statement ::= WHILE xx_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /* 105 */ "xx_while_statement ::= WHILE xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 106 */ "xx_for_statement ::= FOR IDENTIFIER IN xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 107 */ "xx_for_statement ::= FOR IDENTIFIER IN REVERSE xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 108 */ "xx_for_statement ::= FOR IDENTIFIER COMMA IDENTIFIER IN xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 109 */ "xx_for_statement ::= FOR IDENTIFIER COMMA IDENTIFIER IN REVERSE xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 110 */ "xx_let_statement ::= LET xx_let_assignments DOTCOMMA",
 /* 111 */ "xx_let_assignments ::= xx_let_assignments COMMA xx_let_assignment",
 /* 112 */ "xx_let_assignments ::= xx_let_assignment",
 /* 113 */ "xx_assignment_operator ::= ASSIGN",
 /* 114 */ "xx_assignment_operator ::= ADDASSIGN",
 /* 115 */ "xx_assignment_operator ::= SUBASSIGN",
 /* 116 */ "xx_assignment_operator ::= MULASSIGN",
 /* 117 */ "xx_assignment_operator ::= DIVASSIGN",
 /* 118 */ "xx_assignment_operator ::= CONCATASSIGN",
 /* 119 */ "xx_let_assignment ::= IDENTIFIER xx_assignment_operator xx_assign_expr",
 /* 120 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER xx_assignment_operator xx_assign_expr",
 /* 121 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 122 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER SBRACKET_OPEN xx_index_expr SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 123 */ "xx_let_assignment ::= IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 124 */ "xx_let_assignment ::= IDENTIFIER SBRACKET_OPEN xx_index_expr SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 125 */ "xx_let_assignment ::= IDENTIFIER INCR",
 /* 126 */ "xx_let_assignment ::= IDENTIFIER DECR",
 /* 127 */ "xx_index_expr ::= xx_common_expr",
 /* 128 */ "xx_echo_statement ::= ECHO xx_echo_expressions DOTCOMMA",
 /* 129 */ "xx_echo_expressions ::= xx_echo_expressions COMMA xx_echo_expression",
 /* 130 */ "xx_echo_expressions ::= xx_echo_expression",
 /* 131 */ "xx_echo_expression ::= xx_common_expr",
 /* 132 */ "xx_mcall_statement ::= xx_mcall_expr DOTCOMMA",
 /* 133 */ "xx_return_statement ::= RETURN xx_common_expr DOTCOMMA",
 /* 134 */ "xx_unset_statement ::= UNSET IDENTIFIER ARROW IDENTIFIER DOTCOMMA",
 /* 135 */ "xx_unset_statement ::= UNSET IDENTIFIER SBRACKET_OPEN IDENTIFIER SBRACKET_CLOSE DOTCOMMA",
 /* 136 */ "xx_unset_statement ::= UNSET IDENTIFIER SBRACKET_OPEN INTEGER SBRACKET_CLOSE DOTCOMMA",
 /* 137 */ "xx_unset_statement ::= UNSET IDENTIFIER SBRACKET_OPEN STRING SBRACKET_CLOSE DOTCOMMA",
 /* 138 */ "xx_throw_statement ::= THROW IDENTIFIER DOTCOMMA",
 /* 139 */ "xx_throw_statement ::= THROW NEW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE DOTCOMMA",
 /* 140 */ "xx_declare_statement ::= TYPE_INTEGER xx_declare_variable_list DOTCOMMA",
 /* 141 */ "xx_declare_statement ::= TYPE_DOUBLE xx_declare_variable_list DOTCOMMA",
 /* 142 */ "xx_declare_statement ::= TYPE_STRING xx_declare_variable_list DOTCOMMA",
 /* 143 */ "xx_declare_statement ::= TYPE_BOOL xx_declare_variable_list DOTCOMMA",
 /* 144 */ "xx_declare_statement ::= TYPE_VAR xx_declare_variable_list DOTCOMMA",
 /* 145 */ "xx_declare_variable_list ::= xx_declare_variable_list COMMA xx_declare_variable",
 /* 146 */ "xx_declare_variable_list ::= xx_declare_variable",
 /* 147 */ "xx_declare_variable ::= IDENTIFIER",
 /* 148 */ "xx_declare_variable ::= IDENTIFIER ASSIGN xx_literal_expr",
 /* 149 */ "xx_assign_expr ::= xx_common_expr",
 /* 150 */ "xx_common_expr ::= NOT xx_common_expr",
 /* 151 */ "xx_common_expr ::= ISSET xx_isset_expr",
 /* 152 */ "xx_common_expr ::= xx_common_expr EQUALS xx_common_expr",
 /* 153 */ "xx_common_expr ::= xx_common_expr NOTEQUALS xx_common_expr",
 /* 154 */ "xx_common_expr ::= xx_common_expr IDENTICAL xx_common_expr",
 /* 155 */ "xx_common_expr ::= xx_common_expr NOTIDENTICAL xx_common_expr",
 /* 156 */ "xx_common_expr ::= xx_common_expr LESS xx_common_expr",
 /* 157 */ "xx_common_expr ::= xx_common_expr GREATER xx_common_expr",
 /* 158 */ "xx_common_expr ::= xx_common_expr LESSEQUAL xx_common_expr",
 /* 159 */ "xx_common_expr ::= xx_common_expr GREATEREQUAL xx_common_expr",
 /* 160 */ "xx_common_expr ::= PARENTHESES_OPEN xx_common_expr PARENTHESES_CLOSE",
 /* 161 */ "xx_common_expr ::= IDENTIFIER ARROW IDENTIFIER",
 /* 162 */ "xx_common_expr ::= IDENTIFIER SBRACKET_OPEN IDENTIFIER SBRACKET_CLOSE",
 /* 163 */ "xx_common_expr ::= IDENTIFIER SBRACKET_OPEN INTEGER SBRACKET_CLOSE",
 /* 164 */ "xx_common_expr ::= IDENTIFIER SBRACKET_OPEN STRING SBRACKET_CLOSE",
 /* 165 */ "xx_common_expr ::= xx_common_expr ADD xx_common_expr",
 /* 166 */ "xx_common_expr ::= xx_common_expr SUB xx_common_expr",
 /* 167 */ "xx_common_expr ::= xx_common_expr CONCAT xx_common_expr",
 /* 168 */ "xx_common_expr ::= xx_common_expr AND xx_common_expr",
 /* 169 */ "xx_common_expr ::= xx_common_expr OR xx_common_expr",
 /* 170 */ "xx_common_expr ::= INSTANCEOF xx_common_expr",
 /* 171 */ "xx_common_expr ::= FETCH IDENTIFIER COMMA xx_isset_expr",
 /* 172 */ "xx_common_expr ::= TYPEOF xx_common_expr",
 /* 173 */ "xx_common_expr ::= IDENTIFIER",
 /* 174 */ "xx_common_expr ::= INTEGER",
 /* 175 */ "xx_common_expr ::= STRING",
 /* 176 */ "xx_common_expr ::= DOUBLE",
 /* 177 */ "xx_common_expr ::= NULL",
 /* 178 */ "xx_common_expr ::= TRUE",
 /* 179 */ "xx_common_expr ::= FALSE",
 /* 180 */ "xx_common_expr ::= SBRACKET_OPEN SBRACKET_CLOSE",
 /* 181 */ "xx_common_expr ::= SBRACKET_OPEN xx_array_list SBRACKET_CLOSE",
 /* 182 */ "xx_common_expr ::= NEW IDENTIFIER",
 /* 183 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 184 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 185 */ "xx_common_expr ::= IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 186 */ "xx_common_expr ::= IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 187 */ "xx_common_expr ::= xx_mcall_expr",
 /* 188 */ "xx_mcall_expr ::= IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 189 */ "xx_mcall_expr ::= IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 190 */ "xx_call_parameters ::= xx_call_parameters COMMA xx_call_parameter",
 /* 191 */ "xx_call_parameters ::= xx_call_parameter",
 /* 192 */ "xx_call_parameter ::= xx_common_expr",
 /* 193 */ "xx_array_list ::= xx_array_list COMMA xx_array_item",
 /* 194 */ "xx_array_list ::= xx_array_item",
 /* 195 */ "xx_array_item ::= xx_array_key COLON xx_array_value",
 /* 196 */ "xx_array_item ::= xx_array_value",
 /* 197 */ "xx_array_key ::= IDENTIFIER",
 /* 198 */ "xx_array_key ::= STRING",
 /* 199 */ "xx_array_key ::= INTEGER",
 /* 200 */ "xx_array_value ::= xx_literal_expr",
 /* 201 */ "xx_array_value ::= IDENTIFIER",
 /* 202 */ "xx_literal_expr ::= INTEGER",
 /* 203 */ "xx_literal_expr ::= STRING",
 /* 204 */ "xx_literal_expr ::= DOUBLE",
 /* 205 */ "xx_literal_expr ::= NULL",
 /* 206 */ "xx_literal_expr ::= FALSE",
 /* 207 */ "xx_literal_expr ::= TRUE",
 /* 208 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN IDENTIFIER SBRACKET_CLOSE",
 /* 209 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN INTEGER SBRACKET_CLOSE",
 /* 210 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN STRING SBRACKET_CLOSE",
 /* 211 */ "xx_eval_expr ::= xx_common_expr",
 /* 212 */ "xx_comment ::= COMMENT",
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
// 760 "parser.lemon"
{
	/*if ((yypminor->yy0)) {
		if ((yypminor->yy0)->free_flag) {
			efree((yypminor->yy0)->token);
		}
		efree((yypminor->yy0));
	}*/
}
// 1920 "parser.c"
      break;
    case 81:
// 773 "parser.lemon"
{ json_object_put((yypminor->yy55)); }
// 1925 "parser.c"
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
  { 80, 1 },
  { 81, 1 },
  { 82, 2 },
  { 82, 1 },
  { 83, 1 },
  { 83, 1 },
  { 83, 1 },
  { 84, 3 },
  { 85, 5 },
  { 85, 7 },
  { 85, 6 },
  { 85, 8 },
  { 85, 4 },
  { 85, 6 },
  { 85, 5 },
  { 85, 7 },
  { 87, 1 },
  { 87, 1 },
  { 87, 1 },
  { 87, 2 },
  { 87, 2 },
  { 87, 2 },
  { 87, 2 },
  { 87, 3 },
  { 87, 3 },
  { 88, 2 },
  { 88, 1 },
  { 91, 4 },
  { 91, 3 },
  { 91, 6 },
  { 91, 5 },
  { 92, 2 },
  { 92, 1 },
  { 94, 1 },
  { 94, 1 },
  { 94, 1 },
  { 94, 1 },
  { 89, 2 },
  { 89, 1 },
  { 90, 2 },
  { 90, 1 },
  { 95, 6 },
  { 95, 5 },
  { 96, 7 },
  { 96, 8 },
  { 96, 8 },
  { 96, 9 },
  { 96, 8 },
  { 96, 9 },
  { 96, 9 },
  { 96, 10 },
  { 96, 9 },
  { 96, 7 },
  { 96, 8 },
  { 96, 8 },
  { 96, 9 },
  { 97, 3 },
  { 97, 1 },
  { 99, 1 },
  { 99, 2 },
  { 99, 2 },
  { 99, 3 },
  { 99, 4 },
  { 99, 4 },
  { 101, 3 },
  { 100, 1 },
  { 100, 1 },
  { 100, 1 },
  { 100, 1 },
  { 100, 1 },
  { 100, 1 },
  { 98, 2 },
  { 98, 1 },
  { 102, 1 },
  { 102, 1 },
  { 102, 1 },
  { 102, 1 },
  { 102, 1 },
  { 102, 1 },
  { 102, 1 },
  { 102, 1 },
  { 102, 1 },
  { 102, 1 },
  { 102, 1 },
  { 102, 1 },
  { 102, 1 },
  { 102, 1 },
  { 102, 1 },
  { 112, 2 },
  { 113, 2 },
  { 104, 4 },
  { 104, 7 },
  { 104, 5 },
  { 104, 9 },
  { 104, 8 },
  { 104, 8 },
  { 115, 4 },
  { 115, 5 },
  { 118, 2 },
  { 118, 1 },
  { 119, 4 },
  { 119, 3 },
  { 105, 3 },
  { 105, 4 },
  { 114, 4 },
  { 114, 5 },
  { 116, 7 },
  { 116, 8 },
  { 116, 9 },
  { 116, 10 },
  { 103, 3 },
  { 121, 3 },
  { 121, 1 },
  { 123, 1 },
  { 123, 1 },
  { 123, 1 },
  { 123, 1 },
  { 123, 1 },
  { 123, 1 },
  { 122, 3 },
  { 122, 5 },
  { 122, 7 },
  { 122, 8 },
  { 122, 5 },
  { 122, 6 },
  { 122, 2 },
  { 122, 2 },
  { 125, 1 },
  { 106, 3 },
  { 126, 3 },
  { 126, 1 },
  { 127, 1 },
  { 108, 2 },
  { 107, 3 },
  { 109, 5 },
  { 109, 6 },
  { 109, 6 },
  { 109, 6 },
  { 110, 3 },
  { 110, 7 },
  { 111, 3 },
  { 111, 3 },
  { 111, 3 },
  { 111, 3 },
  { 111, 3 },
  { 130, 3 },
  { 130, 1 },
  { 131, 1 },
  { 131, 3 },
  { 124, 1 },
  { 120, 2 },
  { 120, 2 },
  { 120, 3 },
  { 120, 3 },
  { 120, 3 },
  { 120, 3 },
  { 120, 3 },
  { 120, 3 },
  { 120, 3 },
  { 120, 3 },
  { 120, 3 },
  { 120, 3 },
  { 120, 4 },
  { 120, 4 },
  { 120, 4 },
  { 120, 3 },
  { 120, 3 },
  { 120, 3 },
  { 120, 3 },
  { 120, 3 },
  { 120, 2 },
  { 120, 4 },
  { 120, 2 },
  { 120, 1 },
  { 120, 1 },
  { 120, 1 },
  { 120, 1 },
  { 120, 1 },
  { 120, 1 },
  { 120, 1 },
  { 120, 2 },
  { 120, 3 },
  { 120, 2 },
  { 120, 4 },
  { 120, 5 },
  { 120, 4 },
  { 120, 3 },
  { 120, 1 },
  { 128, 6 },
  { 128, 5 },
  { 129, 3 },
  { 129, 1 },
  { 134, 1 },
  { 133, 3 },
  { 133, 1 },
  { 135, 3 },
  { 135, 1 },
  { 136, 1 },
  { 136, 1 },
  { 136, 1 },
  { 137, 1 },
  { 137, 1 },
  { 93, 1 },
  { 93, 1 },
  { 93, 1 },
  { 93, 1 },
  { 93, 1 },
  { 93, 1 },
  { 132, 4 },
  { 132, 4 },
  { 132, 4 },
  { 117, 1 },
  { 86, 1 },
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
// 769 "parser.lemon"
{
	status->ret = yymsp[0].minor.yy55;
}
// 2355 "parser.c"
        break;
      case 1:
      case 4:
      case 5:
      case 6:
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
      case 127:
      case 149:
      case 187:
      case 192:
      case 200:
      case 211:
// 775 "parser.lemon"
{
	yygotominor.yy55 = yymsp[0].minor.yy55;
}
// 2386 "parser.c"
        break;
      case 2:
      case 25:
      case 31:
      case 37:
      case 39:
      case 71:
      case 98:
// 779 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_list(yymsp[-1].minor.yy55, yymsp[0].minor.yy55);
}
// 2399 "parser.c"
        break;
      case 3:
      case 26:
      case 32:
      case 38:
      case 40:
      case 57:
      case 72:
      case 99:
      case 112:
      case 130:
      case 146:
      case 191:
      case 194:
// 783 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_list(NULL, yymsp[0].minor.yy55);
}
// 2418 "parser.c"
        break;
      case 7:
// 799 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_namespace(yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(23,&yymsp[0].minor);
}
// 2427 "parser.c"
        break;
      case 8:
// 803 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[-1].minor.yy55, 0, NULL, status->scanner_state);
  yy_destructor(24,&yymsp[-4].minor);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 2437 "parser.c"
        break;
      case 9:
// 807 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class(yymsp[-5].minor.yy0, yymsp[-1].minor.yy55, 0, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(24,&yymsp[-6].minor);
  yy_destructor(27,&yymsp[-4].minor);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 2448 "parser.c"
        break;
      case 10:
// 811 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[-1].minor.yy55, 1, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-5].minor);
  yy_destructor(24,&yymsp[-4].minor);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 2459 "parser.c"
        break;
      case 11:
// 815 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class(yymsp[-5].minor.yy0, yymsp[-1].minor.yy55, 1, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(28,&yymsp[-7].minor);
  yy_destructor(24,&yymsp[-6].minor);
  yy_destructor(27,&yymsp[-4].minor);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 2471 "parser.c"
        break;
      case 12:
// 819 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class(yymsp[-2].minor.yy0, NULL, 0, NULL, status->scanner_state);
  yy_destructor(24,&yymsp[-3].minor);
  yy_destructor(25,&yymsp[-1].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 2481 "parser.c"
        break;
      case 13:
// 823 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class(yymsp[-4].minor.yy0, NULL, 0, yymsp[-2].minor.yy0, status->scanner_state);
  yy_destructor(24,&yymsp[-5].minor);
  yy_destructor(27,&yymsp[-3].minor);
  yy_destructor(25,&yymsp[-1].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 2492 "parser.c"
        break;
      case 14:
// 827 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class(yymsp[-2].minor.yy0, NULL, 1, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-4].minor);
  yy_destructor(24,&yymsp[-3].minor);
  yy_destructor(25,&yymsp[-1].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 2503 "parser.c"
        break;
      case 15:
// 831 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class(yymsp[-4].minor.yy0, NULL, 1, yymsp[-2].minor.yy0, status->scanner_state);
  yy_destructor(28,&yymsp[-6].minor);
  yy_destructor(24,&yymsp[-5].minor);
  yy_destructor(27,&yymsp[-3].minor);
  yy_destructor(25,&yymsp[-1].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 2515 "parser.c"
        break;
      case 16:
// 835 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_definition(yymsp[0].minor.yy55, NULL, NULL, status->scanner_state);
}
// 2522 "parser.c"
        break;
      case 17:
// 839 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_definition(NULL, NULL, yymsp[0].minor.yy55, status->scanner_state);
}
// 2529 "parser.c"
        break;
      case 18:
// 843 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_definition(NULL, yymsp[0].minor.yy55, NULL, status->scanner_state);
}
// 2536 "parser.c"
        break;
      case 19:
// 847 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_definition(yymsp[-1].minor.yy55, yymsp[0].minor.yy55, NULL, status->scanner_state);
}
// 2543 "parser.c"
        break;
      case 20:
// 851 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_definition(yymsp[-1].minor.yy55, NULL, yymsp[0].minor.yy55, status->scanner_state);
}
// 2550 "parser.c"
        break;
      case 21:
// 855 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_definition(yymsp[0].minor.yy55, NULL, yymsp[-1].minor.yy55, status->scanner_state);
}
// 2557 "parser.c"
        break;
      case 22:
// 859 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_definition(NULL, yymsp[0].minor.yy55, yymsp[-1].minor.yy55, status->scanner_state);
}
// 2564 "parser.c"
        break;
      case 23:
// 863 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_definition(yymsp[-2].minor.yy55, yymsp[0].minor.yy55, yymsp[-1].minor.yy55, status->scanner_state);
}
// 2571 "parser.c"
        break;
      case 24:
// 867 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_definition(yymsp[-1].minor.yy55, yymsp[0].minor.yy55, yymsp[-2].minor.yy55, status->scanner_state);
}
// 2578 "parser.c"
        break;
      case 27:
// 879 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_property(yymsp[-2].minor.yy55, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(23,&yymsp[0].minor);
}
// 2586 "parser.c"
        break;
      case 28:
// 883 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_property(yymsp[-2].minor.yy55, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(23,&yymsp[0].minor);
}
// 2594 "parser.c"
        break;
      case 29:
// 887 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_property(yymsp[-4].minor.yy55, yymsp[-3].minor.yy0, yymsp[-1].minor.yy55, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(23,&yymsp[0].minor);
}
// 2603 "parser.c"
        break;
      case 30:
// 891 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_property(yymsp[-4].minor.yy55, yymsp[-3].minor.yy0, yymsp[-1].minor.yy55, NULL, status->scanner_state);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(23,&yymsp[0].minor);
}
// 2612 "parser.c"
        break;
      case 33:
// 903 "parser.lemon"
{
	yygotominor.yy55 = json_object_new_string("public");
  yy_destructor(31,&yymsp[0].minor);
}
// 2620 "parser.c"
        break;
      case 34:
// 907 "parser.lemon"
{
	yygotominor.yy55 = json_object_new_string("protected");
  yy_destructor(32,&yymsp[0].minor);
}
// 2628 "parser.c"
        break;
      case 35:
// 911 "parser.lemon"
{
	yygotominor.yy55 = json_object_new_string("static");
  yy_destructor(33,&yymsp[0].minor);
}
// 2636 "parser.c"
        break;
      case 36:
// 915 "parser.lemon"
{
	yygotominor.yy55 = json_object_new_string("private");
  yy_destructor(34,&yymsp[0].minor);
}
// 2644 "parser.c"
        break;
      case 41:
// 935 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy55, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(35,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(23,&yymsp[0].minor);
}
// 2654 "parser.c"
        break;
      case 42:
// 939 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy55, NULL, status->scanner_state);
  yy_destructor(35,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(23,&yymsp[0].minor);
}
// 2664 "parser.c"
        break;
      case 43:
// 943 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-4].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(31,&yymsp[-6].minor);
  yy_destructor(36,&yymsp[-5].minor);
  yy_destructor(37,&yymsp[-3].minor);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(25,&yymsp[-1].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 2677 "parser.c"
        break;
      case 44:
// 947 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-5].minor.yy0, yymsp[-3].minor.yy55, NULL, NULL, status->scanner_state);
  yy_destructor(31,&yymsp[-7].minor);
  yy_destructor(36,&yymsp[-6].minor);
  yy_destructor(37,&yymsp[-4].minor);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(25,&yymsp[-1].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 2690 "parser.c"
        break;
      case 45:
// 951 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-4].minor.yy0, NULL, NULL, yymsp[-7].minor.yy0, status->scanner_state);
  yy_destructor(31,&yymsp[-6].minor);
  yy_destructor(36,&yymsp[-5].minor);
  yy_destructor(37,&yymsp[-3].minor);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(25,&yymsp[-1].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 2703 "parser.c"
        break;
      case 46:
// 955 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-5].minor.yy0, yymsp[-3].minor.yy55, NULL, yymsp[-8].minor.yy0, status->scanner_state);
  yy_destructor(31,&yymsp[-7].minor);
  yy_destructor(36,&yymsp[-6].minor);
  yy_destructor(37,&yymsp[-4].minor);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(25,&yymsp[-1].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 2716 "parser.c"
        break;
      case 47:
// 959 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy55, NULL, status->scanner_state);
  yy_destructor(31,&yymsp[-7].minor);
  yy_destructor(36,&yymsp[-6].minor);
  yy_destructor(37,&yymsp[-4].minor);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 2729 "parser.c"
        break;
      case 48:
// 963 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-6].minor.yy0, yymsp[-3].minor.yy55, yymsp[-1].minor.yy55, NULL, status->scanner_state);
  yy_destructor(31,&yymsp[-8].minor);
  yy_destructor(36,&yymsp[-7].minor);
  yy_destructor(37,&yymsp[-5].minor);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 2742 "parser.c"
        break;
      case 49:
// 967 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy55, yymsp[-8].minor.yy0, status->scanner_state);
  yy_destructor(31,&yymsp[-7].minor);
  yy_destructor(36,&yymsp[-6].minor);
  yy_destructor(37,&yymsp[-4].minor);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 2755 "parser.c"
        break;
      case 50:
// 971 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-6].minor.yy0, yymsp[-4].minor.yy55, yymsp[-1].minor.yy55, yymsp[-9].minor.yy0, status->scanner_state);
  yy_destructor(31,&yymsp[-8].minor);
  yy_destructor(36,&yymsp[-7].minor);
  yy_destructor(37,&yymsp[-5].minor);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 2768 "parser.c"
        break;
      case 51:
// 975 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-6].minor.yy0, yymsp[-4].minor.yy55, yymsp[-1].minor.yy55, NULL, status->scanner_state);
  yy_destructor(31,&yymsp[-8].minor);
  yy_destructor(36,&yymsp[-7].minor);
  yy_destructor(37,&yymsp[-5].minor);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 2781 "parser.c"
        break;
      case 52:
// 979 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_method(XX_T_PROTECTED, yymsp[-4].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(32,&yymsp[-6].minor);
  yy_destructor(36,&yymsp[-5].minor);
  yy_destructor(37,&yymsp[-3].minor);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(25,&yymsp[-1].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 2794 "parser.c"
        break;
      case 53:
// 983 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_method(XX_T_PROTECTED, yymsp[-4].minor.yy0, NULL, NULL, yymsp[-7].minor.yy0, status->scanner_state);
  yy_destructor(32,&yymsp[-6].minor);
  yy_destructor(36,&yymsp[-5].minor);
  yy_destructor(37,&yymsp[-3].minor);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(25,&yymsp[-1].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 2807 "parser.c"
        break;
      case 54:
// 987 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_method(XX_T_PROTECTED, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy55, NULL, status->scanner_state);
  yy_destructor(32,&yymsp[-7].minor);
  yy_destructor(36,&yymsp[-6].minor);
  yy_destructor(37,&yymsp[-4].minor);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 2820 "parser.c"
        break;
      case 55:
// 991 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_method(XX_T_PROTECTED, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy55, yymsp[-8].minor.yy0, status->scanner_state);
  yy_destructor(32,&yymsp[-7].minor);
  yy_destructor(36,&yymsp[-6].minor);
  yy_destructor(37,&yymsp[-4].minor);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 2833 "parser.c"
        break;
      case 56:
      case 111:
      case 129:
      case 145:
      case 190:
      case 193:
// 995 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_list(yymsp[-2].minor.yy55, yymsp[0].minor.yy55);
  yy_destructor(1,&yymsp[-1].minor);
}
// 2846 "parser.c"
        break;
      case 58:
// 1003 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_parameter(NULL, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 2853 "parser.c"
        break;
      case 59:
// 1007 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_parameter(yymsp[-1].minor.yy55, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 2860 "parser.c"
        break;
      case 60:
// 1011 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_parameter(NULL, yymsp[-1].minor.yy55, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 2867 "parser.c"
        break;
      case 61:
// 1015 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_parameter(NULL, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy55, status->scanner_state);
  yy_destructor(30,&yymsp[-1].minor);
}
// 2875 "parser.c"
        break;
      case 62:
// 1019 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_parameter(yymsp[-3].minor.yy55, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy55, status->scanner_state);
  yy_destructor(30,&yymsp[-1].minor);
}
// 2883 "parser.c"
        break;
      case 63:
// 1023 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_parameter(NULL, yymsp[-3].minor.yy55, yymsp[-2].minor.yy0, yymsp[0].minor.yy55, status->scanner_state);
  yy_destructor(30,&yymsp[-1].minor);
}
// 2891 "parser.c"
        break;
      case 64:
// 1027 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(6,&yymsp[-2].minor);
  yy_destructor(7,&yymsp[0].minor);
}
// 2900 "parser.c"
        break;
      case 65:
// 1031 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_type(XX_TYPE_INTEGER);
  yy_destructor(39,&yymsp[0].minor);
}
// 2908 "parser.c"
        break;
      case 66:
// 1035 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_type(XX_TYPE_DOUBLE);
  yy_destructor(40,&yymsp[0].minor);
}
// 2916 "parser.c"
        break;
      case 67:
// 1039 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_type(XX_TYPE_BOOL);
  yy_destructor(41,&yymsp[0].minor);
}
// 2924 "parser.c"
        break;
      case 68:
// 1043 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_type(XX_TYPE_STRING);
  yy_destructor(42,&yymsp[0].minor);
}
// 2932 "parser.c"
        break;
      case 69:
// 1047 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_type(XX_TYPE_ARRAY);
  yy_destructor(43,&yymsp[0].minor);
}
// 2940 "parser.c"
        break;
      case 70:
// 1051 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_type(XX_TYPE_VAR);
  yy_destructor(44,&yymsp[0].minor);
}
// 2948 "parser.c"
        break;
      case 88:
// 1123 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_break_statement(status->scanner_state);
  yy_destructor(45,&yymsp[-1].minor);
  yy_destructor(23,&yymsp[0].minor);
}
// 2957 "parser.c"
        break;
      case 89:
// 1127 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_continue_statement(status->scanner_state);
  yy_destructor(46,&yymsp[-1].minor);
  yy_destructor(23,&yymsp[0].minor);
}
// 2966 "parser.c"
        break;
      case 90:
// 1131 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_if_statement(yymsp[-2].minor.yy55, NULL, NULL, status->scanner_state);
  yy_destructor(47,&yymsp[-3].minor);
  yy_destructor(25,&yymsp[-1].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 2976 "parser.c"
        break;
      case 91:
// 1135 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_if_statement(yymsp[-5].minor.yy55, NULL, NULL, status->scanner_state);
  yy_destructor(47,&yymsp[-6].minor);
  yy_destructor(25,&yymsp[-4].minor);
  yy_destructor(26,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(25,&yymsp[-1].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 2989 "parser.c"
        break;
      case 92:
// 1139 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_if_statement(yymsp[-3].minor.yy55, yymsp[-1].minor.yy55, NULL, status->scanner_state);
  yy_destructor(47,&yymsp[-4].minor);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 2999 "parser.c"
        break;
      case 93:
// 1143 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_if_statement(yymsp[-7].minor.yy55, yymsp[-5].minor.yy55, yymsp[-1].minor.yy55, status->scanner_state);
  yy_destructor(47,&yymsp[-8].minor);
  yy_destructor(25,&yymsp[-6].minor);
  yy_destructor(26,&yymsp[-4].minor);
  yy_destructor(48,&yymsp[-3].minor);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 3012 "parser.c"
        break;
      case 94:
// 1147 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_if_statement(yymsp[-6].minor.yy55, yymsp[-4].minor.yy55, NULL, status->scanner_state);
  yy_destructor(47,&yymsp[-7].minor);
  yy_destructor(25,&yymsp[-5].minor);
  yy_destructor(26,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(25,&yymsp[-1].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 3025 "parser.c"
        break;
      case 95:
// 1151 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_if_statement(yymsp[-6].minor.yy55, NULL, yymsp[-1].minor.yy55, status->scanner_state);
  yy_destructor(47,&yymsp[-7].minor);
  yy_destructor(25,&yymsp[-5].minor);
  yy_destructor(26,&yymsp[-4].minor);
  yy_destructor(48,&yymsp[-3].minor);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 3038 "parser.c"
        break;
      case 96:
// 1155 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_switch_statement(yymsp[-2].minor.yy55, NULL, status->scanner_state);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(25,&yymsp[-1].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 3048 "parser.c"
        break;
      case 97:
// 1159 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_switch_statement(yymsp[-3].minor.yy55, yymsp[-1].minor.yy55, status->scanner_state);
  yy_destructor(49,&yymsp[-4].minor);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 3058 "parser.c"
        break;
      case 100:
// 1171 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_case_clause(yymsp[-2].minor.yy55, yymsp[0].minor.yy55, status->scanner_state);
  yy_destructor(50,&yymsp[-3].minor);
  yy_destructor(51,&yymsp[-1].minor);
}
// 3067 "parser.c"
        break;
      case 101:
// 1175 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_case_clause(NULL, yymsp[0].minor.yy55, status->scanner_state);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(51,&yymsp[-1].minor);
}
// 3076 "parser.c"
        break;
      case 102:
// 1179 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_loop_statement(NULL, status->scanner_state);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(25,&yymsp[-1].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 3086 "parser.c"
        break;
      case 103:
// 1183 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_loop_statement(yymsp[-1].minor.yy55, status->scanner_state);
  yy_destructor(53,&yymsp[-3].minor);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 3096 "parser.c"
        break;
      case 104:
// 1187 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_while_statement(yymsp[-2].minor.yy55, NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-3].minor);
  yy_destructor(25,&yymsp[-1].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 3106 "parser.c"
        break;
      case 105:
// 1191 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_while_statement(yymsp[-3].minor.yy55, yymsp[-1].minor.yy55, status->scanner_state);
  yy_destructor(54,&yymsp[-4].minor);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 3116 "parser.c"
        break;
      case 106:
// 1195 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_for_statement(yymsp[-3].minor.yy55, NULL, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy55, status->scanner_state);
  yy_destructor(55,&yymsp[-6].minor);
  yy_destructor(56,&yymsp[-4].minor);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 3127 "parser.c"
        break;
      case 107:
// 1199 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_for_statement(yymsp[-3].minor.yy55, NULL, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy55, status->scanner_state);
  yy_destructor(55,&yymsp[-7].minor);
  yy_destructor(56,&yymsp[-5].minor);
  yy_destructor(57,&yymsp[-4].minor);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 3139 "parser.c"
        break;
      case 108:
// 1203 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_for_statement(yymsp[-3].minor.yy55, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy55, status->scanner_state);
  yy_destructor(55,&yymsp[-8].minor);
  yy_destructor(1,&yymsp[-6].minor);
  yy_destructor(56,&yymsp[-4].minor);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 3151 "parser.c"
        break;
      case 109:
// 1207 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_for_statement(yymsp[-3].minor.yy55, yymsp[-8].minor.yy0, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy55, status->scanner_state);
  yy_destructor(55,&yymsp[-9].minor);
  yy_destructor(1,&yymsp[-7].minor);
  yy_destructor(56,&yymsp[-5].minor);
  yy_destructor(57,&yymsp[-4].minor);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 3164 "parser.c"
        break;
      case 110:
// 1211 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_let_statement(yymsp[-1].minor.yy55, status->scanner_state);
  yy_destructor(58,&yymsp[-2].minor);
  yy_destructor(23,&yymsp[0].minor);
}
// 3173 "parser.c"
        break;
      case 113:
// 1224 "parser.lemon"
{
	yygotominor.yy55 = json_object_new_string("assign");
  yy_destructor(30,&yymsp[0].minor);
}
// 3181 "parser.c"
        break;
      case 114:
// 1229 "parser.lemon"
{
	yygotominor.yy55 = json_object_new_string("add-assign");
  yy_destructor(59,&yymsp[0].minor);
}
// 3189 "parser.c"
        break;
      case 115:
// 1234 "parser.lemon"
{
	yygotominor.yy55 = json_object_new_string("sub-assign");
  yy_destructor(60,&yymsp[0].minor);
}
// 3197 "parser.c"
        break;
      case 116:
// 1238 "parser.lemon"
{
	yygotominor.yy55 = json_object_new_string("mult-assign");
  yy_destructor(61,&yymsp[0].minor);
}
// 3205 "parser.c"
        break;
      case 117:
// 1242 "parser.lemon"
{
	yygotominor.yy55 = json_object_new_string("div-assign");
  yy_destructor(62,&yymsp[0].minor);
}
// 3213 "parser.c"
        break;
      case 118:
// 1246 "parser.lemon"
{
	yygotominor.yy55 = json_object_new_string("concat-assign");
  yy_destructor(63,&yymsp[0].minor);
}
// 3221 "parser.c"
        break;
      case 119:
// 1251 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_let_assignment("variable", yymsp[-1].minor.yy55, yymsp[-2].minor.yy0, NULL, NULL, yymsp[0].minor.yy55, status->scanner_state);
}
// 3228 "parser.c"
        break;
      case 120:
// 1256 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_let_assignment("object-property", yymsp[-1].minor.yy55, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy55, status->scanner_state);
  yy_destructor(64,&yymsp[-3].minor);
}
// 3236 "parser.c"
        break;
      case 121:
// 1261 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_let_assignment("object-property-append", yymsp[-1].minor.yy55, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy55, status->scanner_state);
  yy_destructor(64,&yymsp[-5].minor);
  yy_destructor(65,&yymsp[-3].minor);
  yy_destructor(66,&yymsp[-2].minor);
}
// 3246 "parser.c"
        break;
      case 122:
// 1266 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_let_assignment("object-property-array-index", yymsp[-1].minor.yy55, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy55, yymsp[0].minor.yy55, status->scanner_state);
  yy_destructor(64,&yymsp[-6].minor);
  yy_destructor(65,&yymsp[-4].minor);
  yy_destructor(66,&yymsp[-2].minor);
}
// 3256 "parser.c"
        break;
      case 123:
// 1271 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_let_assignment("variable-append", yymsp[-1].minor.yy55, yymsp[-4].minor.yy0, NULL, NULL, yymsp[0].minor.yy55, status->scanner_state);
  yy_destructor(65,&yymsp[-3].minor);
  yy_destructor(66,&yymsp[-2].minor);
}
// 3265 "parser.c"
        break;
      case 124:
// 1276 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_let_assignment("array-index", yymsp[-1].minor.yy55, yymsp[-5].minor.yy0, NULL, yymsp[-3].minor.yy55, yymsp[0].minor.yy55, status->scanner_state);
  yy_destructor(65,&yymsp[-4].minor);
  yy_destructor(66,&yymsp[-2].minor);
}
// 3274 "parser.c"
        break;
      case 125:
// 1281 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_let_assignment("incr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(67,&yymsp[0].minor);
}
// 3282 "parser.c"
        break;
      case 126:
// 1286 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_let_assignment("decr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(68,&yymsp[0].minor);
}
// 3290 "parser.c"
        break;
      case 128:
// 1294 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_echo_statement(yymsp[-1].minor.yy55, status->scanner_state);
  yy_destructor(69,&yymsp[-2].minor);
  yy_destructor(23,&yymsp[0].minor);
}
// 3299 "parser.c"
        break;
      case 131:
// 1306 "parser.lemon"
{
	yygotominor.yy55 = yymsp[0].minor.yy55;;
}
// 3306 "parser.c"
        break;
      case 132:
// 1311 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_mcall_statement(yymsp[-1].minor.yy55, status->scanner_state);
  yy_destructor(23,&yymsp[0].minor);
}
// 3314 "parser.c"
        break;
      case 133:
// 1316 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_return_statement(yymsp[-1].minor.yy55, status->scanner_state);
  yy_destructor(70,&yymsp[-2].minor);
  yy_destructor(23,&yymsp[0].minor);
}
// 3323 "parser.c"
        break;
      case 134:
// 1321 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(71,&yymsp[-4].minor);
  yy_destructor(64,&yymsp[-2].minor);
  yy_destructor(23,&yymsp[0].minor);
}
// 3337 "parser.c"
        break;
      case 135:
// 1330 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(71,&yymsp[-5].minor);
  yy_destructor(65,&yymsp[-3].minor);
  yy_destructor(66,&yymsp[-1].minor);
  yy_destructor(23,&yymsp[0].minor);
}
// 3352 "parser.c"
        break;
      case 136:
// 1339 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_INTEGER, yymsp[-2].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(71,&yymsp[-5].minor);
  yy_destructor(65,&yymsp[-3].minor);
  yy_destructor(66,&yymsp[-1].minor);
  yy_destructor(23,&yymsp[0].minor);
}
// 3367 "parser.c"
        break;
      case 137:
// 1348 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_STRING, yymsp[-2].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(71,&yymsp[-5].minor);
  yy_destructor(65,&yymsp[-3].minor);
  yy_destructor(66,&yymsp[-1].minor);
  yy_destructor(23,&yymsp[0].minor);
}
// 3382 "parser.c"
        break;
      case 138:
// 1357 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_throw_exception(yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(74,&yymsp[-2].minor);
  yy_destructor(23,&yymsp[0].minor);
}
// 3391 "parser.c"
        break;
      case 139:
// 1362 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_throw_exception(yymsp[-4].minor.yy0, yymsp[-2].minor.yy55, status->scanner_state);
  yy_destructor(74,&yymsp[-6].minor);
  yy_destructor(19,&yymsp[-5].minor);
  yy_destructor(37,&yymsp[-3].minor);
  yy_destructor(38,&yymsp[-1].minor);
  yy_destructor(23,&yymsp[0].minor);
}
// 3403 "parser.c"
        break;
      case 140:
// 1366 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_declare_statement(XX_T_TYPE_INTEGER, yymsp[-1].minor.yy55, status->scanner_state);
  yy_destructor(39,&yymsp[-2].minor);
  yy_destructor(23,&yymsp[0].minor);
}
// 3412 "parser.c"
        break;
      case 141:
// 1370 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_declare_statement(XX_T_TYPE_DOUBLE, yymsp[-1].minor.yy55, status->scanner_state);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(23,&yymsp[0].minor);
}
// 3421 "parser.c"
        break;
      case 142:
// 1374 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_declare_statement(XX_T_TYPE_STRING, yymsp[-1].minor.yy55, status->scanner_state);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(23,&yymsp[0].minor);
}
// 3430 "parser.c"
        break;
      case 143:
// 1378 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_declare_statement(XX_T_TYPE_BOOL, yymsp[-1].minor.yy55, status->scanner_state);
  yy_destructor(41,&yymsp[-2].minor);
  yy_destructor(23,&yymsp[0].minor);
}
// 3439 "parser.c"
        break;
      case 144:
// 1382 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_declare_statement(XX_T_TYPE_VAR, yymsp[-1].minor.yy55, status->scanner_state);
  yy_destructor(44,&yymsp[-2].minor);
  yy_destructor(23,&yymsp[0].minor);
}
// 3448 "parser.c"
        break;
      case 147:
// 1394 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_declare_variable(yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3455 "parser.c"
        break;
      case 148:
// 1398 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_declare_variable(yymsp[-2].minor.yy0, yymsp[0].minor.yy55, status->scanner_state);
  yy_destructor(30,&yymsp[-1].minor);
}
// 3463 "parser.c"
        break;
      case 150:
// 1406 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("not", yymsp[0].minor.yy55, NULL, NULL, status->scanner_state);
  yy_destructor(20,&yymsp[-1].minor);
}
// 3471 "parser.c"
        break;
      case 151:
// 1410 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("isset", yymsp[0].minor.yy55, NULL, NULL, status->scanner_state);
  yy_destructor(15,&yymsp[-1].minor);
}
// 3479 "parser.c"
        break;
      case 152:
// 1414 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("equals", yymsp[-2].minor.yy55, yymsp[0].minor.yy55, NULL, status->scanner_state);
  yy_destructor(4,&yymsp[-1].minor);
}
// 3487 "parser.c"
        break;
      case 153:
// 1418 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("not-equals", yymsp[-2].minor.yy55, yymsp[0].minor.yy55, NULL, status->scanner_state);
  yy_destructor(11,&yymsp[-1].minor);
}
// 3495 "parser.c"
        break;
      case 154:
// 1422 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("identical", yymsp[-2].minor.yy55, yymsp[0].minor.yy55, NULL, status->scanner_state);
  yy_destructor(5,&yymsp[-1].minor);
}
// 3503 "parser.c"
        break;
      case 155:
// 1426 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("not-identical", yymsp[-2].minor.yy55, yymsp[0].minor.yy55, NULL, status->scanner_state);
  yy_destructor(10,&yymsp[-1].minor);
}
// 3511 "parser.c"
        break;
      case 156:
// 1430 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("less", yymsp[-2].minor.yy55, yymsp[0].minor.yy55, NULL, status->scanner_state);
  yy_destructor(6,&yymsp[-1].minor);
}
// 3519 "parser.c"
        break;
      case 157:
// 1434 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("greater", yymsp[-2].minor.yy55, yymsp[0].minor.yy55, NULL, status->scanner_state);
  yy_destructor(7,&yymsp[-1].minor);
}
// 3527 "parser.c"
        break;
      case 158:
// 1438 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("less-equal", yymsp[-2].minor.yy55, yymsp[0].minor.yy55, NULL, status->scanner_state);
  yy_destructor(8,&yymsp[-1].minor);
}
// 3535 "parser.c"
        break;
      case 159:
// 1442 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("greater-equal", yymsp[-2].minor.yy55, yymsp[0].minor.yy55, NULL, status->scanner_state);
  yy_destructor(9,&yymsp[-1].minor);
}
// 3543 "parser.c"
        break;
      case 160:
// 1446 "parser.lemon"
{
	yygotominor.yy55 = yymsp[-1].minor.yy55;
  yy_destructor(37,&yymsp[-2].minor);
  yy_destructor(38,&yymsp[0].minor);
}
// 3552 "parser.c"
        break;
      case 161:
// 1450 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(64,&yymsp[-1].minor);
}
// 3560 "parser.c"
        break;
      case 162:
      case 208:
// 1454 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(65,&yymsp[-2].minor);
  yy_destructor(66,&yymsp[0].minor);
}
// 3570 "parser.c"
        break;
      case 163:
      case 209:
// 1459 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_INTEGER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(65,&yymsp[-2].minor);
  yy_destructor(66,&yymsp[0].minor);
}
// 3580 "parser.c"
        break;
      case 164:
      case 210:
// 1464 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_STRING, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(65,&yymsp[-2].minor);
  yy_destructor(66,&yymsp[0].minor);
}
// 3590 "parser.c"
        break;
      case 165:
// 1469 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("add", yymsp[-2].minor.yy55, yymsp[0].minor.yy55, NULL, status->scanner_state);
  yy_destructor(12,&yymsp[-1].minor);
}
// 3598 "parser.c"
        break;
      case 166:
// 1474 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("sub", yymsp[-2].minor.yy55, yymsp[0].minor.yy55, NULL, status->scanner_state);
  yy_destructor(13,&yymsp[-1].minor);
}
// 3606 "parser.c"
        break;
      case 167:
// 1479 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("concat", yymsp[-2].minor.yy55, yymsp[0].minor.yy55, NULL, status->scanner_state);
  yy_destructor(14,&yymsp[-1].minor);
}
// 3614 "parser.c"
        break;
      case 168:
// 1484 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("and", yymsp[-2].minor.yy55, yymsp[0].minor.yy55, NULL, status->scanner_state);
  yy_destructor(2,&yymsp[-1].minor);
}
// 3622 "parser.c"
        break;
      case 169:
// 1489 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("or", yymsp[-2].minor.yy55, yymsp[0].minor.yy55, NULL, status->scanner_state);
  yy_destructor(3,&yymsp[-1].minor);
}
// 3630 "parser.c"
        break;
      case 170:
// 1494 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("instanceof", yymsp[0].minor.yy55, NULL, NULL, status->scanner_state);
  yy_destructor(18,&yymsp[-1].minor);
}
// 3638 "parser.c"
        break;
      case 171:
// 1499 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("fetch", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy55, NULL, status->scanner_state);
  yy_destructor(16,&yymsp[-3].minor);
  yy_destructor(1,&yymsp[-1].minor);
}
// 3647 "parser.c"
        break;
      case 172:
// 1504 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("typeof", yymsp[0].minor.yy55, NULL, NULL, status->scanner_state);
  yy_destructor(17,&yymsp[-1].minor);
}
// 3655 "parser.c"
        break;
      case 173:
      case 197:
      case 201:
// 1509 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state);
}
// 3664 "parser.c"
        break;
      case 174:
      case 199:
      case 202:
// 1514 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_literal(XX_T_INTEGER, yymsp[0].minor.yy0, status->scanner_state);
}
// 3673 "parser.c"
        break;
      case 175:
      case 198:
      case 203:
// 1519 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_literal(XX_T_STRING, yymsp[0].minor.yy0, status->scanner_state);
}
// 3682 "parser.c"
        break;
      case 176:
      case 204:
// 1524 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_literal(XX_T_DOUBLE, yymsp[0].minor.yy0, status->scanner_state);
}
// 3690 "parser.c"
        break;
      case 177:
      case 205:
// 1529 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_literal(XX_T_NULL, NULL, status->scanner_state);
  yy_destructor(76,&yymsp[0].minor);
}
// 3699 "parser.c"
        break;
      case 178:
      case 207:
// 1534 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_literal(XX_T_TRUE, NULL, status->scanner_state);
  yy_destructor(77,&yymsp[0].minor);
}
// 3708 "parser.c"
        break;
      case 179:
      case 206:
// 1539 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_literal(XX_T_FALSE, NULL, status->scanner_state);
  yy_destructor(78,&yymsp[0].minor);
}
// 3717 "parser.c"
        break;
      case 180:
// 1544 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("empty-array", NULL, NULL, NULL, status->scanner_state);
  yy_destructor(65,&yymsp[-1].minor);
  yy_destructor(66,&yymsp[0].minor);
}
// 3726 "parser.c"
        break;
      case 181:
// 1549 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("array", yymsp[-1].minor.yy55, NULL, NULL, status->scanner_state);
  yy_destructor(65,&yymsp[-2].minor);
  yy_destructor(66,&yymsp[0].minor);
}
// 3735 "parser.c"
        break;
      case 182:
// 1554 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_new_instance(yymsp[0].minor.yy0, NULL, status->scanner_state);
  yy_destructor(19,&yymsp[-1].minor);
}
// 3743 "parser.c"
        break;
      case 183:
// 1559 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_new_instance(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(19,&yymsp[-3].minor);
  yy_destructor(37,&yymsp[-1].minor);
  yy_destructor(38,&yymsp[0].minor);
}
// 3753 "parser.c"
        break;
      case 184:
// 1564 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_new_instance(yymsp[-3].minor.yy0, yymsp[-1].minor.yy55, status->scanner_state);
  yy_destructor(19,&yymsp[-4].minor);
  yy_destructor(37,&yymsp[-2].minor);
  yy_destructor(38,&yymsp[0].minor);
}
// 3763 "parser.c"
        break;
      case 185:
// 1569 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_fcall(yymsp[-3].minor.yy0, yymsp[-1].minor.yy55, status->scanner_state);
  yy_destructor(37,&yymsp[-2].minor);
  yy_destructor(38,&yymsp[0].minor);
}
// 3772 "parser.c"
        break;
      case 186:
// 1574 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_fcall(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(37,&yymsp[-1].minor);
  yy_destructor(38,&yymsp[0].minor);
}
// 3781 "parser.c"
        break;
      case 188:
// 1584 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_mcall(yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy55, status->scanner_state);
  yy_destructor(64,&yymsp[-4].minor);
  yy_destructor(37,&yymsp[-2].minor);
  yy_destructor(38,&yymsp[0].minor);
}
// 3791 "parser.c"
        break;
      case 189:
// 1589 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_mcall(yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(64,&yymsp[-3].minor);
  yy_destructor(37,&yymsp[-1].minor);
  yy_destructor(38,&yymsp[0].minor);
}
// 3801 "parser.c"
        break;
      case 195:
// 1613 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_array_item(yymsp[-2].minor.yy55, yymsp[0].minor.yy55, status->scanner_state);
  yy_destructor(51,&yymsp[-1].minor);
}
// 3809 "parser.c"
        break;
      case 196:
// 1617 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_array_item(NULL, yymsp[0].minor.yy55, status->scanner_state);
}
// 3816 "parser.c"
        break;
      case 212:
// 1684 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_comment(yymsp[0].minor.yy0, status->scanner_state);
}
// 3823 "parser.c"
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
// 726 "parser.lemon"


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

// 3899 "parser.c"
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
