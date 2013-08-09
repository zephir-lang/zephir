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

static json_object *xx_ret_let_assignment(char *type, xx_parser_token *V, xx_parser_token *P, json_object *index_expr, json_object *expr, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "assign-type", json_object_new_string(type));
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


// 694 "parser.c"
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
#define YYNOCODE 125
#define YYACTIONTYPE unsigned short int
#define xx_TOKENTYPE xx_parser_token*
typedef union {
  xx_TOKENTYPE yy0;
  json_object* yy175;
  int yy249;
} YYMINORTYPE;
#define YYSTACKDEPTH 100
#define xx_ARG_SDECL xx_parser_status *status;
#define xx_ARG_PDECL ,xx_parser_status *status
#define xx_ARG_FETCH xx_parser_status *status = yypParser->status
#define xx_ARG_STORE yypParser->status = status
#define YYNSTATE 444
#define YYNRULE 201
#define YYERRORSYMBOL 68
#define YYERRSYMDT yy249
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
 /*     0 */   272,  375,   17,  390,  293,  416,  378,  275,  646,    1,
 /*    10 */     2,  443,    4,    5,    6,  251,  260,  266,  263,  223,
 /*    20 */   269,   44,   46,   48,  342,   57,  357,  380,  154,   68,
 /*    30 */    72,   77,  155,  156,   84,  157,  158,  160,  159,  272,
 /*    40 */   215,  224,  227,   27,   15,  242,  275,  391,  404,  376,
 /*    50 */   410,  385,  377,  378,  251,  260,  266,  263,  444,  269,
 /*    60 */    44,   46,   48,  335,   57,  334,  317,  321,   68,   72,
 /*    70 */    77,  155,  156,   84,  157,  158,  160,  159,  272,  215,
 /*    80 */   224,  227,   71,  460,  242,  275,  391,  404,    8,  410,
 /*    90 */   385,   85,  214,  251,  260,  266,  263,  218,  269,   44,
 /*   100 */    46,   48,  308,   57,  334,  317,  321,   68,   72,   77,
 /*   110 */   468,    9,   84,  341,   19,  217,  366,  272,  215,  224,
 /*   120 */   227,   76,  461,  242,  275,  391,  404,  153,  410,  385,
 /*   130 */   148,  282,  251,  260,  266,  263,   11,  269,   44,   46,
 /*   140 */    48,  351,   57,  334,  317,  321,   68,   72,   77,  463,
 /*   150 */   312,   84,  341,   19,  109,  366,  272,  215,  224,  227,
 /*   160 */   306,  173,  242,  275,   18,  146,  153,  165,  150,  161,
 /*   170 */   309,  251,  260,  266,  263,  129,  269,   44,   46,   48,
 /*   180 */    79,   57,  173,  248,  147,   68,   72,   77,  175,   14,
 /*   190 */    84,  153,  116,  118,  120,  272,  215,  224,  227,  296,
 /*   200 */   429,  242,  275,  391,  404,   20,  410,  385,  152,   21,
 /*   210 */   251,  260,  266,  263,  129,  269,   44,   46,   48,   22,
 /*   220 */    57,  173,  176,  121,   68,   72,   77,  175,   45,   84,
 /*   230 */   173,  149,  150,  161,  272,  215,  224,  227,  348,  442,
 /*   240 */   242,  275,  391,  404,   87,  410,  385,   61,  300,  251,
 /*   250 */   260,  266,  263,   47,  269,   44,   46,   48,  392,   57,
 /*   260 */   398,  380,   86,   68,   72,   77,  467,   50,   84,  341,
 /*   270 */    19,   53,  366,  272,  215,  224,  227,   56,  186,  242,
 /*   280 */   275,  418,  180,  421,  422,  399,  377,  378,  251,  260,
 /*   290 */   266,  263,   54,  269,   44,   46,   48,  358,   57,  220,
 /*   300 */   126,  179,   68,   72,   77,  219,  173,   84,    3,    4,
 /*   310 */     5,    6,  272,  215,  224,  227,   52,  436,  242,  275,
 /*   320 */   391,  404,   59,  410,  385,  182,  184,  251,  260,  266,
 /*   330 */   263,   63,  269,   44,   46,   48,   66,   57,  313,  317,
 /*   340 */   321,   68,   72,   77,  462,   69,   84,  341,   19,  405,
 /*   350 */   366,  272,  215,  224,  227,   83,  464,  242,  275,  379,
 /*   360 */    19,   20,  366,  385,  270,  259,  251,  260,  266,  263,
 /*   370 */   254,  269,   44,   46,   48,   74,   57,  111,   62,   65,
 /*   380 */    68,   72,   77,  466,  173,   84,  341,   19,  271,  366,
 /*   390 */   272,  215,  224,  227,  365,  465,  242,  275,  420,  404,
 /*   400 */    64,  410,  297,   78,   60,  251,  260,  266,  263,  400,
 /*   410 */   269,   44,   46,   48,   89,   57,  225,  401,  203,   68,
 /*   420 */    72,   77,  208,  173,   84,  173,   64,  129,  297,  272,
 /*   430 */   215,  224,  227,  288,  173,  242,  275,  419,  390,   88,
 /*   440 */   174,  378,  172,   99,  251,  260,  266,  263,  113,  269,
 /*   450 */    44,   46,   48,  411,   57,  173,  172,  233,   68,   72,
 /*   460 */    77,  172,   98,   84,   12,  367,  424,  100,  272,  215,
 /*   470 */   224,  227,  372,  171,  242,  275,  193,  101,  392,  342,
 /*   480 */   398,  357,  102,  251,  260,  266,  263,  177,  269,   44,
 /*   490 */    46,   48,  188,   57,  267,  259,  406,   68,   72,   77,
 /*   500 */   236,  239,   84,  195,  407,  115,  127,  272,  215,  224,
 /*   510 */   227,  374,  173,  242,  275,  128,   58,  264,  259,  295,
 /*   520 */   103,  105,  251,  260,  266,  263,  173,  269,   44,   46,
 /*   530 */    48,  393,   57,  117,  133,  203,   68,   72,   77,  199,
 /*   540 */   173,   84,  173,  343,  119,  134,  272,  215,  224,  227,
 /*   550 */   294,  173,  242,  275,  245,   73,  104,  243,  295,  151,
 /*   560 */   135,  251,  260,  266,  263,  173,  269,   44,   46,   48,
 /*   570 */   106,   57,  290,  167,  278,   68,   72,   77,  312,  173,
 /*   580 */    84,  173,  629,  131,  181,  272,  215,  224,  227,  307,
 /*   590 */   173,  242,  275,  630,   49,  261,  259,  295,  631,  254,
 /*   600 */   251,  260,  266,  263,  173,  269,   44,   46,   48,  336,
 /*   610 */    57,  123,  183,  137,   68,   72,   77,  268,  173,   84,
 /*   620 */   173,  252,  259,  168,  272,  215,  224,  227,  281,   92,
 /*   630 */   242,  275,   91,  254,  229,  232,  173,  192,  185,  251,
 /*   640 */   260,  266,  263,   80,  269,   44,   46,   48,  197,   57,
 /*   650 */   173,  265,  285,   68,   72,   77,  312,  200,   84,  173,
 /*   660 */   172,  190,  201,  272,  215,  224,  227,   25,  173,  242,
 /*   670 */   275,  433,  254,  437,  206,   95,  210,  254,  251,  260,
 /*   680 */   266,  263,  173,  269,   44,   46,   48,  352,   57,  107,
 /*   690 */   262,  249,   68,   72,   77,  253,  173,   84,  209,  412,
 /*   700 */   228,  230,  272,  215,  224,  227,  340,  413,  242,  275,
 /*   710 */   231,  234,  235,  394,  238,  237,  240,  251,  260,  266,
 /*   720 */   263,  395,  269,   44,   46,   48,  241,   57,  244,  246,
 /*   730 */   247,   68,   72,   77,  250,  256,   84,  255,  258,  257,
 /*   740 */   273,  272,  215,  224,  227,  301,  274,  242,  275,  283,
 /*   750 */   284,  298,  303,  304,  315,  316,  251,  260,  266,  263,
 /*   760 */   318,  269,   44,   46,   48,  319,   57,  320,  322,  324,
 /*   770 */    68,   72,   77,  323,  326,   84,  327,  506,  507,  508,
 /*   780 */   272,  215,  224,  227,  350,  509,  242,  275,  510,  337,
 /*   790 */   511,  512,  344,  343,  358,  251,  260,  266,  263,  347,
 /*   800 */   269,   44,   46,   48,  353,   57,  345,  359,  360,   68,
 /*   810 */    72,   77,  361,  362,   84,  367,  368,  369,  371,  272,
 /*   820 */   215,  224,  227,  354,  381,  242,  275,  370,  383,  382,
 /*   830 */   384,  386,  387,  388,  251,  260,  266,  263,  389,  269,
 /*   840 */    44,   46,   48,  397,   57,  403,  396,  402,   68,   72,
 /*   850 */    77,  409,  415,   84,  425,  426,  432,  408,  272,  215,
 /*   860 */   224,  227,  311,  414,  242,  275,  428,  431,  435,  438,
 /*   870 */   439,  327,  441,  251,  260,  266,  263,  327,  269,   44,
 /*   880 */    46,   48,  327,   57,  327,  327,  327,   68,   72,   77,
 /*   890 */   327,  327,   84,  327,  327,  327,  327,  272,  215,  224,
 /*   900 */   227,  356,  327,  242,  275,  327,  327,  327,  327,  327,
 /*   910 */   327,  327,  251,  260,  266,  263,  327,  269,   44,   46,
 /*   920 */    48,  327,   57,  327,  327,  327,   68,   72,   77,  327,
 /*   930 */   327,   84,  327,  327,  327,  327,  272,  215,  224,  227,
 /*   940 */   363,  327,  242,  275,  327,  327,  327,  327,  327,  327,
 /*   950 */   327,  251,  260,  266,  263,  327,  269,   44,   46,   48,
 /*   960 */   327,   57,  327,  327,  327,   68,   72,   77,  327,  327,
 /*   970 */    84,  327,  327,  327,  327,  272,  215,  224,  227,  338,
 /*   980 */   327,  242,  275,  327,  327,  327,  327,  327,  327,  327,
 /*   990 */   251,  260,  266,  263,  327,  269,   44,   46,   48,  327,
 /*  1000 */    57,  327,  327,  327,   68,   72,   77,  327,  327,   84,
 /*  1010 */   327,  327,  327,  327,  272,  215,  224,  227,  302,  327,
 /*  1020 */   242,  275,  327,  327,  327,  327,  327,  327,  327,  251,
 /*  1030 */   260,  266,  263,  327,  269,   44,   46,   48,  327,   57,
 /*  1040 */   327,  327,  327,   68,   72,   77,  327,  327,   84,  327,
 /*  1050 */   272,  327,  327,  162,  215,  224,  227,  275,  327,  242,
 /*  1060 */   327,  327,  327,  327,  327,  251,  260,  266,  263,  129,
 /*  1070 */   269,   44,   46,   48,  327,   57,  173,  187,  327,   68,
 /*  1080 */    72,   77,  175,  327,   84,  327,  327,  327,  327,  327,
 /*  1090 */   215,  224,  227,   43,  327,  242,  164,  163,  327,  157,
 /*  1100 */   158,  160,  159,  280,  327,  327,  327,  276,   29,   30,
 /*  1110 */    31,   32,   33,   34,   35,   36,   37,   38,   39,   40,
 /*  1120 */    41,   42,  129,  327,   43,  327,  327,   90,  327,  173,
 /*  1130 */   170,  327,  222,  327,  339,  175,  327,  327,  276,   29,
 /*  1140 */    30,   31,   32,   33,   34,   35,   36,   37,   38,   39,
 /*  1150 */    40,   41,   42,  191,  204,   43,  212,  213,  327,    7,
 /*  1160 */   327,   92,   10,  222,  202,  305,  430,  275,  173,  276,
 /*  1170 */    29,   30,   31,   32,   33,   34,   35,   36,   37,   38,
 /*  1180 */    39,   40,   41,   42,  220,   43,  327,  327,  327,  216,
 /*  1190 */   221,  173,  327,  327,  222,   55,  327,  327,  327,  276,
 /*  1200 */    29,   30,   31,   32,   33,   34,   35,   36,   37,   38,
 /*  1210 */    39,   40,   41,   42,  327,   43,   92,  327,  327,  194,
 /*  1220 */   327,  327,  327,  173,  222,  310,  327,  327,  327,  276,
 /*  1230 */    29,   30,   31,   32,   33,   34,   35,   36,   37,   38,
 /*  1240 */    39,   40,   41,   42,  327,   43,   92,  327,  327,  211,
 /*  1250 */   327,  327,  327,  173,  222,   82,  327,  327,  327,  276,
 /*  1260 */    29,   30,   31,   32,   33,   34,   35,   36,   37,   38,
 /*  1270 */    39,   40,   41,   42,   92,   43,   92,  207,  327,  198,
 /*  1280 */   327,  173,  327,  173,  222,   51,  327,  327,  327,  276,
 /*  1290 */    29,   30,   31,   32,   33,   34,   35,   36,   37,   38,
 /*  1300 */    39,   40,   41,   42,  327,   43,  327,  327,  327,  327,
 /*  1310 */   327,  327,  327,  327,  222,   75,  327,  327,  327,  276,
 /*  1320 */    29,   30,   31,   32,   33,   34,   35,   36,   37,   38,
 /*  1330 */    39,   40,   41,   42,  327,   43,  327,  327,  327,  327,
 /*  1340 */   327,  327,  327,  327,  222,   70,  327,  327,  327,  276,
 /*  1350 */    29,   30,   31,   32,   33,   34,   35,   36,   37,   38,
 /*  1360 */    39,   40,   41,   42,  327,   43,  327,  327,  327,  327,
 /*  1370 */   327,  327,  327,  327,  222,  287,  327,  327,  327,  276,
 /*  1380 */    29,   30,   31,   32,   33,   34,   35,   36,   37,   38,
 /*  1390 */    39,   40,   41,   42,  327,   43,  327,  327,  327,  327,
 /*  1400 */   327,  327,  327,  327,  222,  292,  327,  327,  327,  276,
 /*  1410 */    29,   30,   31,   32,   33,   34,   35,   36,   37,   38,
 /*  1420 */    39,   40,   41,   42,  327,   43,  327,  327,  327,  327,
 /*  1430 */   327,  327,  327,  327,  222,   67,  327,  327,  327,  276,
 /*  1440 */    29,   30,   31,   32,   33,   34,   35,   36,   37,   38,
 /*  1450 */    39,   40,   41,   42,  327,   43,  327,  327,  327,  327,
 /*  1460 */   327,  327,  327,  327,  222,   26,  327,  327,  327,  276,
 /*  1470 */    29,   30,   31,   32,   33,   34,   35,   36,   37,   38,
 /*  1480 */    39,   40,   41,   42,  327,   43,  327,  327,  327,  327,
 /*  1490 */   327,  327,  327,  327,  222,  299,  327,  327,  327,  276,
 /*  1500 */    29,   30,   31,   32,   33,   34,   35,   36,   37,   38,
 /*  1510 */    39,   40,   41,   42,  327,   43,  327,  327,  327,  327,
 /*  1520 */   327,  327,  327,  327,  222,  373,  327,  327,  327,  276,
 /*  1530 */    29,   30,   31,   32,   33,   34,   35,   36,   37,   38,
 /*  1540 */    39,   40,   41,   42,  327,   43,  327,  327,  327,  327,
 /*  1550 */   327,  327,  327,  327,  222,  364,  327,  327,  327,  276,
 /*  1560 */    29,   30,   31,   32,   33,   34,   35,   36,   37,   38,
 /*  1570 */    39,   40,   41,   42,  327,   43,  327,  327,  327,  327,
 /*  1580 */   327,  327,  327,  327,  222,  355,  327,  327,  327,  276,
 /*  1590 */    29,   30,   31,   32,   33,   34,   35,   36,   37,   38,
 /*  1600 */    39,   40,   41,   42,  327,   43,  327,  327,  327,  327,
 /*  1610 */   327,  327,  327,  327,  222,  349,  327,  327,  327,  276,
 /*  1620 */    29,   30,   31,   32,   33,   34,   35,   36,   37,   38,
 /*  1630 */    39,   40,   41,   42,  327,   43,  327,  327,  327,  427,
 /*  1640 */    16,  417,  423,  422,  222,  416,  378,  327,  327,   28,
 /*  1650 */    29,   30,   31,   32,   33,   34,   35,   36,   37,   38,
 /*  1660 */    39,   40,   41,   42,   97,  132,  136,  130,  166,   94,
 /*  1670 */   327,  125,  327,  327,  222,  327,  327,  327,  327,  162,
 /*  1680 */   327,  327,  327,  327,  122,  178,   97,  132,  136,  130,
 /*  1690 */   166,   94,  327,  125,   93,  108,  112,  114,  110,   96,
 /*  1700 */   116,  118,  120,  327,  327,  327,  122,  144,  327,  327,
 /*  1710 */   327,  327,  327,  643,  138,  139,  145,  140,  141,  142,
 /*  1720 */   143,  327,  164,  163,  327,  157,  158,  160,  159,  144,
 /*  1730 */   205,  327,  327,  327,  327,  327,  138,  139,  327,  140,
 /*  1740 */   141,  142,  143,  327,  327,  327,  327,  327,   97,  132,
 /*  1750 */   136,  130,  166,   94,  327,  125,   93,  108,  112,  114,
 /*  1760 */   110,   96,  116,  118,  120,  327,  327,  327,  122,  327,
 /*  1770 */    13,   16,  417,  423,  422,  291,  416,  378,  327,  327,
 /*  1780 */    97,  132,  136,  130,  166,   94,  327,  125,  327,  327,
 /*  1790 */   327,  144,  196,  327,  327,  327,  327,  327,  138,  139,
 /*  1800 */   122,  140,  141,  142,  143,   93,  108,  112,  114,  110,
 /*  1810 */    96,  116,  118,  120,  327,  327,  327,  327,  327,  327,
 /*  1820 */   289,  327,  226,  144,  327,  327,  327,  327,  327,  327,
 /*  1830 */   138,  139,  327,  140,  141,  142,  143,  327,  327,  327,
 /*  1840 */   327,  327,   97,  132,  136,  130,  166,   94,  327,  125,
 /*  1850 */    93,  108,  112,  114,  110,   96,  116,  118,  120,  327,
 /*  1860 */   327,  327,  122,  189,  434,   16,  417,  423,  422,  327,
 /*  1870 */   416,  378,  327,  327,   97,  132,  136,  130,  166,   94,
 /*  1880 */   124,  125,  327,  327,  327,  144,  327,  327,  327,  327,
 /*  1890 */   327,  327,  138,  139,  122,  140,  141,  142,  143,   93,
 /*  1900 */   108,  112,  114,  110,   96,  116,  118,  120,  327,  327,
 /*  1910 */   327,  327,  327,  327,  277,  327,  327,  144,  279,  327,
 /*  1920 */   327,  327,  327,  327,  138,  139,  327,  140,  141,  142,
 /*  1930 */   143,  327,  327,  327,  327,  327,   97,  132,  136,  130,
 /*  1940 */   166,   94,  327,  125,   93,  108,  112,  114,  110,   96,
 /*  1950 */   116,  118,  120,  327,  327,  327,  122,  169,  440,   16,
 /*  1960 */   417,  423,  422,   81,  416,  378,  327,  327,   97,  132,
 /*  1970 */   136,  130,  166,   94,  327,  125,  327,  327,  327,  144,
 /*  1980 */   327,  327,  327,  327,  327,  327,  138,  139,  122,  140,
 /*  1990 */   141,  142,  143,   93,  108,  112,  114,  110,   96,  116,
 /*  2000 */   118,  120,   93,  108,  112,  114,  110,   96,  116,  118,
 /*  2010 */   120,  144,  327,  327,  327,  327,  327,  327,  138,  139,
 /*  2020 */   327,  140,  141,  142,  143,   93,  108,  112,  114,  110,
 /*  2030 */    96,  116,  118,  120,  325,  445,  327,  327,  327,  327,
 /*  2040 */   327,  327,  327,  327,  286,  327,  563,  327,  314,  327,
 /*  2050 */   327,  327,    7,  327,  327,   10,  327,  327,  327,  430,
 /*  2060 */   275,  327,   23,  328,  329,  330,  331,  332,  333,  325,
 /*  2070 */   327,  327,  327,  327,  327,  327,  327,  327,  327,  327,
 /*  2080 */   327,  327,  327,  314,  327,  327,   24,  327,  327,  327,
 /*  2090 */   327,  325,  327,  327,  327,  327,  327,  327,  328,  329,
 /*  2100 */   330,  331,  332,  333,  325,  314,  327,  327,  327,  327,
 /*  2110 */   327,  327,  327,  327,  327,  327,  327,  327,  314,  346,
 /*  2120 */   328,  329,  330,  331,  332,  333,  327,  327,  327,  327,
 /*  2130 */   327,  327,  327,  328,  329,  330,  331,  332,  333,
};
static YYCODETYPE yy_lookahead[] = {
 /*     0 */    18,   78,   79,   80,   22,   82,   83,   25,   69,   70,
 /*    10 */    71,   72,   73,   74,   75,   33,   34,   35,   36,   19,
 /*    20 */    38,   39,   40,   41,   26,   43,   28,   29,   18,   47,
 /*    30 */    48,   49,   61,   62,   52,   64,   65,   66,   67,   18,
 /*    40 */    58,   59,   60,   22,   22,   63,   25,   25,   26,   79,
 /*    50 */    28,   29,   82,   83,   33,   34,   35,   36,    0,   38,
 /*    60 */    39,   40,   41,   84,   43,   86,   87,   88,   47,   48,
 /*    70 */    49,   61,   62,   52,   64,   65,   66,   67,   18,   58,
 /*    80 */    59,   60,   22,   22,   63,   25,   25,   26,   18,   28,
 /*    90 */    29,  108,  109,   33,   34,   35,   36,    1,   38,   39,
 /*   100 */    40,   41,   84,   43,   86,   87,   88,   47,   48,   49,
 /*   110 */    22,   19,   52,   25,   26,   19,   28,   18,   58,   59,
 /*   120 */    60,   22,   22,   63,   25,   25,   26,   81,   28,   29,
 /*   130 */     1,    1,   33,   34,   35,   36,   18,   38,   39,   40,
 /*   140 */    41,   84,   43,   86,   87,   88,   47,   48,   49,   22,
 /*   150 */     1,   52,   25,   26,  107,   28,   18,   58,   59,   60,
 /*   160 */    22,  114,   63,   25,   83,  119,   81,  121,  122,  123,
 /*   170 */    21,   33,   34,   35,   36,  107,   38,   39,   40,   41,
 /*   180 */    50,   43,  114,  115,   55,   47,   48,   49,  120,   22,
 /*   190 */    52,   81,    8,    9,   10,   18,   58,   59,   60,   22,
 /*   200 */    22,   63,   25,   25,   26,   30,   28,   29,  123,   18,
 /*   210 */    33,   34,   35,   36,  107,   38,   39,   40,   41,   31,
 /*   220 */    43,  114,  115,  107,   47,   48,   49,  120,   19,   52,
 /*   230 */   114,  121,  122,  123,   18,   58,   59,   60,   22,   22,
 /*   240 */    63,   25,   25,   26,    1,   28,   29,  105,  106,   33,
 /*   250 */    34,   35,   36,   19,   38,   39,   40,   41,   26,   43,
 /*   260 */    28,   29,   19,   47,   48,   49,   22,   21,   52,   25,
 /*   270 */    26,   42,   28,   18,   58,   59,   60,   22,   31,   63,
 /*   280 */    25,   77,   18,   79,   80,   18,   82,   83,   33,   34,
 /*   290 */    35,   36,   21,   38,   39,   40,   41,   30,   43,  107,
 /*   300 */    53,   54,   47,   48,   49,  113,  114,   52,   72,   73,
 /*   310 */    74,   75,   18,   58,   59,   60,   22,   22,   63,   25,
 /*   320 */    25,   26,   21,   28,   29,   61,   62,   33,   34,   35,
 /*   330 */    36,  106,   38,   39,   40,   41,   45,   43,   86,   87,
 /*   340 */    88,   47,   48,   49,   22,   21,   52,   25,   26,   18,
 /*   350 */    28,   18,   58,   59,   60,   22,   22,   63,   25,   25,
 /*   360 */    26,   30,   28,   29,  116,  117,   33,   34,   35,   36,
 /*   370 */     1,   38,   39,   40,   41,   21,   43,  107,   22,   81,
 /*   380 */    47,   48,   49,   22,  114,   52,   25,   26,   19,   28,
 /*   390 */    18,   58,   59,   60,   22,   22,   63,   25,   25,   26,
 /*   400 */    44,   28,   46,   18,   22,   33,   34,   35,   36,   19,
 /*   410 */    38,   39,   40,   41,   18,   43,  107,   27,  107,   47,
 /*   420 */    48,   49,  111,  114,   52,  114,   44,  107,   46,   18,
 /*   430 */    58,   59,   60,   22,  114,   63,   25,   79,   80,  109,
 /*   440 */   120,   83,    1,   18,   33,   34,   35,   36,  107,   38,
 /*   450 */    39,   40,   41,   18,   43,  114,    1,   18,   47,   48,
 /*   460 */    49,    1,  118,   52,   21,   30,   23,   54,   18,   58,
 /*   470 */    59,   60,   22,   32,   63,   25,   27,   18,   26,   26,
 /*   480 */    28,   28,   55,   33,   34,   35,   36,   32,   38,   39,
 /*   490 */    40,   41,   32,   43,  116,  117,   19,   47,   48,   49,
 /*   500 */    61,   62,   52,   54,   27,  107,   18,   18,   58,   59,
 /*   510 */    60,   22,  114,   63,   25,   31,  104,  116,  117,  107,
 /*   520 */    61,   62,   33,   34,   35,   36,  114,   38,   39,   40,
 /*   530 */    41,   18,   43,  107,   18,  107,   47,   48,   49,  111,
 /*   540 */   114,   52,  114,   30,  107,    1,   18,   58,   59,   60,
 /*   550 */    22,  114,   63,   25,   15,  104,   55,   18,  107,   45,
 /*   560 */   118,   33,   34,   35,   36,  114,   38,   39,   40,   41,
 /*   570 */    55,   43,  107,   18,  107,   47,   48,   49,    1,  114,
 /*   580 */    52,  114,   45,  107,   55,   18,   58,   59,   60,   22,
 /*   590 */   114,   63,   25,   45,  104,  116,  117,  107,   45,    1,
 /*   600 */    33,   34,   35,   36,  114,   38,   39,   40,   41,   32,
 /*   610 */    43,  107,   55,  107,   47,   48,   49,   19,  114,   52,
 /*   620 */   114,  116,  117,   31,   18,   58,   59,   60,   22,  107,
 /*   630 */    63,   25,  110,    1,   53,   54,  114,   18,   55,   33,
 /*   640 */    34,   35,   36,  107,   38,   39,   40,   41,   27,   43,
 /*   650 */   114,   19,  107,   47,   48,   49,    1,   55,   52,  114,
 /*   660 */     1,  107,   27,   18,   58,   59,   60,   22,  114,   63,
 /*   670 */    25,   21,    1,   23,   27,  107,   27,    1,   33,   34,
 /*   680 */    35,   36,  114,   38,   39,   40,   41,   32,   43,  107,
 /*   690 */    19,   32,   47,   48,   49,   19,  114,   52,   55,   19,
 /*   700 */    18,   18,   18,   58,   59,   60,   22,   27,   63,   25,
 /*   710 */    19,   55,   19,   19,   19,   55,   55,   33,   34,   35,
 /*   720 */    36,   27,   38,   39,   40,   41,   19,   43,   19,   18,
 /*   730 */    31,   47,   48,   49,   19,   18,   52,  117,   81,   27,
 /*   740 */    53,   18,   58,   59,   60,   22,   18,   63,   25,   18,
 /*   750 */    50,   45,   42,   21,   27,   81,   33,   34,   35,   36,
 /*   760 */    18,   38,   39,   40,   41,   27,   43,   81,   18,   81,
 /*   770 */    47,   48,   49,   27,   18,   52,    5,   18,   18,   18,
 /*   780 */    18,   58,   59,   60,   22,   18,   63,   25,   18,   21,
 /*   790 */    18,   18,   18,   30,   30,   33,   34,   35,   36,   21,
 /*   800 */    38,   39,   40,   41,   21,   43,   31,   18,   31,   47,
 /*   810 */    48,   49,   32,   21,   52,   30,   18,   31,   21,   18,
 /*   820 */    58,   59,   60,   22,   18,   63,   25,   32,   81,   27,
 /*   830 */    19,   18,   27,   81,   33,   34,   35,   36,   19,   38,
 /*   840 */    39,   40,   41,   19,   43,   19,   81,   81,   47,   48,
 /*   850 */    49,   19,   19,   52,   18,   21,   18,   81,   18,   58,
 /*   860 */    59,   60,   22,   81,   63,   25,   22,   20,   22,   18,
 /*   870 */    21,  124,   22,   33,   34,   35,   36,  124,   38,   39,
 /*   880 */    40,   41,  124,   43,  124,  124,  124,   47,   48,   49,
 /*   890 */   124,  124,   52,  124,  124,  124,  124,   18,   58,   59,
 /*   900 */    60,   22,  124,   63,   25,  124,  124,  124,  124,  124,
 /*   910 */   124,  124,   33,   34,   35,   36,  124,   38,   39,   40,
 /*   920 */    41,  124,   43,  124,  124,  124,   47,   48,   49,  124,
 /*   930 */   124,   52,  124,  124,  124,  124,   18,   58,   59,   60,
 /*   940 */    22,  124,   63,   25,  124,  124,  124,  124,  124,  124,
 /*   950 */   124,   33,   34,   35,   36,  124,   38,   39,   40,   41,
 /*   960 */   124,   43,  124,  124,  124,   47,   48,   49,  124,  124,
 /*   970 */    52,  124,  124,  124,  124,   18,   58,   59,   60,   22,
 /*   980 */   124,   63,   25,  124,  124,  124,  124,  124,  124,  124,
 /*   990 */    33,   34,   35,   36,  124,   38,   39,   40,   41,  124,
 /*  1000 */    43,  124,  124,  124,   47,   48,   49,  124,  124,   52,
 /*  1010 */   124,  124,  124,  124,   18,   58,   59,   60,   22,  124,
 /*  1020 */    63,   25,  124,  124,  124,  124,  124,  124,  124,   33,
 /*  1030 */    34,   35,   36,  124,   38,   39,   40,   41,  124,   43,
 /*  1040 */   124,  124,  124,   47,   48,   49,  124,  124,   52,  124,
 /*  1050 */    18,  124,  124,   18,   58,   59,   60,   25,  124,   63,
 /*  1060 */   124,  124,  124,  124,  124,   33,   34,   35,   36,  107,
 /*  1070 */    38,   39,   40,   41,  124,   43,  114,  115,  124,   47,
 /*  1080 */    48,   49,  120,  124,   52,  124,  124,  124,  124,  124,
 /*  1090 */    58,   59,   60,   75,  124,   63,   61,   62,  124,   64,
 /*  1100 */    65,   66,   67,   85,  124,  124,  124,   89,   90,   91,
 /*  1110 */    92,   93,   94,   95,   96,   97,   98,   99,  100,  101,
 /*  1120 */   102,  103,  107,  124,   75,  124,  124,   27,  124,  114,
 /*  1130 */   115,  124,  114,  124,   85,  120,  124,  124,   89,   90,
 /*  1140 */    91,   92,   93,   94,   95,   96,   97,   98,   99,  100,
 /*  1150 */   101,  102,  103,   53,   54,   75,   56,   57,  124,   17,
 /*  1160 */   124,  107,   20,  114,  110,   85,   24,   25,  114,   89,
 /*  1170 */    90,   91,   92,   93,   94,   95,   96,   97,   98,   99,
 /*  1180 */   100,  101,  102,  103,  107,   75,  124,  124,  124,  112,
 /*  1190 */   113,  114,  124,  124,  114,   85,  124,  124,  124,   89,
 /*  1200 */    90,   91,   92,   93,   94,   95,   96,   97,   98,   99,
 /*  1210 */   100,  101,  102,  103,  124,   75,  107,  124,  124,  110,
 /*  1220 */   124,  124,  124,  114,  114,   85,  124,  124,  124,   89,
 /*  1230 */    90,   91,   92,   93,   94,   95,   96,   97,   98,   99,
 /*  1240 */   100,  101,  102,  103,  124,   75,  107,  124,  124,  110,
 /*  1250 */   124,  124,  124,  114,  114,   85,  124,  124,  124,   89,
 /*  1260 */    90,   91,   92,   93,   94,   95,   96,   97,   98,   99,
 /*  1270 */   100,  101,  102,  103,  107,   75,  107,  110,  124,  110,
 /*  1280 */   124,  114,  124,  114,  114,   85,  124,  124,  124,   89,
 /*  1290 */    90,   91,   92,   93,   94,   95,   96,   97,   98,   99,
 /*  1300 */   100,  101,  102,  103,  124,   75,  124,  124,  124,  124,
 /*  1310 */   124,  124,  124,  124,  114,   85,  124,  124,  124,   89,
 /*  1320 */    90,   91,   92,   93,   94,   95,   96,   97,   98,   99,
 /*  1330 */   100,  101,  102,  103,  124,   75,  124,  124,  124,  124,
 /*  1340 */   124,  124,  124,  124,  114,   85,  124,  124,  124,   89,
 /*  1350 */    90,   91,   92,   93,   94,   95,   96,   97,   98,   99,
 /*  1360 */   100,  101,  102,  103,  124,   75,  124,  124,  124,  124,
 /*  1370 */   124,  124,  124,  124,  114,   85,  124,  124,  124,   89,
 /*  1380 */    90,   91,   92,   93,   94,   95,   96,   97,   98,   99,
 /*  1390 */   100,  101,  102,  103,  124,   75,  124,  124,  124,  124,
 /*  1400 */   124,  124,  124,  124,  114,   85,  124,  124,  124,   89,
 /*  1410 */    90,   91,   92,   93,   94,   95,   96,   97,   98,   99,
 /*  1420 */   100,  101,  102,  103,  124,   75,  124,  124,  124,  124,
 /*  1430 */   124,  124,  124,  124,  114,   85,  124,  124,  124,   89,
 /*  1440 */    90,   91,   92,   93,   94,   95,   96,   97,   98,   99,
 /*  1450 */   100,  101,  102,  103,  124,   75,  124,  124,  124,  124,
 /*  1460 */   124,  124,  124,  124,  114,   85,  124,  124,  124,   89,
 /*  1470 */    90,   91,   92,   93,   94,   95,   96,   97,   98,   99,
 /*  1480 */   100,  101,  102,  103,  124,   75,  124,  124,  124,  124,
 /*  1490 */   124,  124,  124,  124,  114,   85,  124,  124,  124,   89,
 /*  1500 */    90,   91,   92,   93,   94,   95,   96,   97,   98,   99,
 /*  1510 */   100,  101,  102,  103,  124,   75,  124,  124,  124,  124,
 /*  1520 */   124,  124,  124,  124,  114,   85,  124,  124,  124,   89,
 /*  1530 */    90,   91,   92,   93,   94,   95,   96,   97,   98,   99,
 /*  1540 */   100,  101,  102,  103,  124,   75,  124,  124,  124,  124,
 /*  1550 */   124,  124,  124,  124,  114,   85,  124,  124,  124,   89,
 /*  1560 */    90,   91,   92,   93,   94,   95,   96,   97,   98,   99,
 /*  1570 */   100,  101,  102,  103,  124,   75,  124,  124,  124,  124,
 /*  1580 */   124,  124,  124,  124,  114,   85,  124,  124,  124,   89,
 /*  1590 */    90,   91,   92,   93,   94,   95,   96,   97,   98,   99,
 /*  1600 */   100,  101,  102,  103,  124,   75,  124,  124,  124,  124,
 /*  1610 */   124,  124,  124,  124,  114,   85,  124,  124,  124,   89,
 /*  1620 */    90,   91,   92,   93,   94,   95,   96,   97,   98,   99,
 /*  1630 */   100,  101,  102,  103,  124,   75,  124,  124,  124,   76,
 /*  1640 */    77,   78,   79,   80,  114,   82,   83,  124,  124,   89,
 /*  1650 */    90,   91,   92,   93,   94,   95,   96,   97,   98,   99,
 /*  1660 */   100,  101,  102,  103,   11,   12,   13,   14,   15,   16,
 /*  1670 */   124,   18,  124,  124,  114,  124,  124,  124,  124,   18,
 /*  1680 */   124,  124,  124,  124,   31,   32,   11,   12,   13,   14,
 /*  1690 */    15,   16,  124,   18,    2,    3,    4,    5,    6,    7,
 /*  1700 */     8,    9,   10,  124,  124,  124,   31,   54,  124,  124,
 /*  1710 */   124,  124,  124,   21,   61,   62,   55,   64,   65,   66,
 /*  1720 */    67,  124,   61,   62,  124,   64,   65,   66,   67,   54,
 /*  1730 */    55,  124,  124,  124,  124,  124,   61,   62,  124,   64,
 /*  1740 */    65,   66,   67,  124,  124,  124,  124,  124,   11,   12,
 /*  1750 */    13,   14,   15,   16,  124,   18,    2,    3,    4,    5,
 /*  1760 */     6,    7,    8,    9,   10,  124,  124,  124,   31,  124,
 /*  1770 */    76,   77,   78,   79,   80,   21,   82,   83,  124,  124,
 /*  1780 */    11,   12,   13,   14,   15,   16,  124,   18,  124,  124,
 /*  1790 */   124,   54,   55,  124,  124,  124,  124,  124,   61,   62,
 /*  1800 */    31,   64,   65,   66,   67,    2,    3,    4,    5,    6,
 /*  1810 */     7,    8,    9,   10,  124,  124,  124,  124,  124,  124,
 /*  1820 */    51,  124,   19,   54,  124,  124,  124,  124,  124,  124,
 /*  1830 */    61,   62,  124,   64,   65,   66,   67,  124,  124,  124,
 /*  1840 */   124,  124,   11,   12,   13,   14,   15,   16,  124,   18,
 /*  1850 */     2,    3,    4,    5,    6,    7,    8,    9,   10,  124,
 /*  1860 */   124,  124,   31,   32,   76,   77,   78,   79,   80,  124,
 /*  1870 */    82,   83,  124,  124,   11,   12,   13,   14,   15,   16,
 /*  1880 */    32,   18,  124,  124,  124,   54,  124,  124,  124,  124,
 /*  1890 */   124,  124,   61,   62,   31,   64,   65,   66,   67,    2,
 /*  1900 */     3,    4,    5,    6,    7,    8,    9,   10,  124,  124,
 /*  1910 */   124,  124,  124,  124,   51,  124,  124,   54,   21,  124,
 /*  1920 */   124,  124,  124,  124,   61,   62,  124,   64,   65,   66,
 /*  1930 */    67,  124,  124,  124,  124,  124,   11,   12,   13,   14,
 /*  1940 */    15,   16,  124,   18,    2,    3,    4,    5,    6,    7,
 /*  1950 */     8,    9,   10,  124,  124,  124,   31,   32,   76,   77,
 /*  1960 */    78,   79,   80,   21,   82,   83,  124,  124,   11,   12,
 /*  1970 */    13,   14,   15,   16,  124,   18,  124,  124,  124,   54,
 /*  1980 */   124,  124,  124,  124,  124,  124,   61,   62,   31,   64,
 /*  1990 */    65,   66,   67,    2,    3,    4,    5,    6,    7,    8,
 /*  2000 */     9,   10,    2,    3,    4,    5,    6,    7,    8,    9,
 /*  2010 */    10,   54,  124,  124,  124,  124,  124,  124,   61,   62,
 /*  2020 */   124,   64,   65,   66,   67,    2,    3,    4,    5,    6,
 /*  2030 */     7,    8,    9,   10,    4,    0,  124,  124,  124,  124,
 /*  2040 */   124,  124,  124,  124,   21,  124,   55,  124,   18,  124,
 /*  2050 */   124,  124,   17,  124,  124,   20,  124,  124,  124,   24,
 /*  2060 */    25,  124,   32,   33,   34,   35,   36,   37,   38,    4,
 /*  2070 */   124,  124,  124,  124,  124,  124,  124,  124,  124,  124,
 /*  2080 */   124,  124,  124,   18,  124,  124,   21,  124,  124,  124,
 /*  2090 */   124,    4,  124,  124,  124,  124,  124,  124,   33,   34,
 /*  2100 */    35,   36,   37,   38,    4,   18,  124,  124,  124,  124,
 /*  2110 */   124,  124,  124,  124,  124,  124,  124,  124,   18,   32,
 /*  2120 */    33,   34,   35,   36,   37,   38,  124,  124,  124,  124,
 /*  2130 */   124,  124,  124,   33,   34,   35,   36,   37,   38,
};
#define YY_SHIFT_USE_DFLT (-30)
static short yy_shift_ofst[] = {
 /*     0 */  1142,   58, 2035,  -30,  -30,  -30,  -30,   70,   92,  -30,
 /*    10 */   118,  443,   22,  167,  -30,  -30,   61,  127,  -30,  175,
 /*    20 */   191,  188, 2030, 2065,  645,  -30,   21,  -30,  -30,  -30,
 /*    30 */   -30,  -30,  -30,  -30,  -30,  -30,  -30,  -30,  -30,  -30,
 /*    40 */   -30,  -30,  -30,  -30,  209,  -30,  234,  -30, 1957,  246,
 /*    50 */   996,  294,  229,  271,  723,  255,  -30, 1957,  301,  382,
 /*    60 */   -30,  356,  -30,  -30,  -29,  291, 1032, 1032,  324,  177,
 /*    70 */    60,  -30, 1957,  354,  528,   99,  -30,  385,  130, 1863,
 /*    80 */  1942, 1032,  333,  -30,  396,  243,  -30,  396,  -30, 1100,
 /*    90 */  1957,  -30, 2000, 1957, 1957,  -30, 1957,  425,  -30,  413,
 /*   100 */   459,  427,  -30,  501,  -30,  515,  -30,  184, 1957,  184,
 /*   110 */  1957,  184, 1957,  184, 1957,  184, 1957,  -30, 1957,  -30,
 /*   120 */  1957,  -30, 1957, 1848,  -30,  247,  488,  484, 1653, 2000,
 /*   130 */  1957,  -30,  516,  544,  425,  -30, 1957,  -30,  -30,  -30,
 /*   140 */   -30,  -30,  -30,  -30, 1661,  -30,  129,  -30, 1035,  -30,
 /*   150 */   514,   10,  -30,  -30,  -30,  -30,  -30,  -30,  -30,  -30,
 /*   160 */   -30,  -30,  537,  548,  553,  -30,  555,  592, 1925,  -30,
 /*   170 */   441,  -30, 1957,  -30,  -30,  -30,  455,  -30,  -30,  264,
 /*   180 */   529,  -30,  557,  -30,  583,  -30, 1831,  460,  -30,  -30,
 /*   190 */   184,  619,  449, 1957,  -30, 1737,  621, 1957,  -30,  602,
 /*   200 */   635, 1957,  -30, 1991, 1675,  647, 1957,  -30,  643,  649,
 /*   210 */  1957,  -30,  -30,  -30,  -30, 1957,   96,  -30, 1957,  -30,
 /*   220 */  2000,  -30,    0,  -30, 1957, 1803,  -30,  682,  581,  683,
 /*   230 */   691,  -30,  439,  656,  693,  -30,  660,  695,  -30,  661,
 /*   240 */   707,  -30,  539,  709,  -30,  711,  699, 1957,  659,  715,
 /*   250 */   -30,  717,  676,  -30,  717,  -30,  712,  -29,  -30,  -30,
 /*   260 */   717,  671,  -30,  717,  632,  -30,  717,  598,  -30,  717,
 /*   270 */   369,  -30,  687,  728,  484,  -30,  -30, 1957, 1897, 1032,
 /*   280 */   606,  -30,  731,  700, 1769, 2023, 1032,  411,  -30, 1957,
 /*   290 */  1754, 1032,  -18,  -30,  -30, 1692,  -30,  706, 1032, 1032,
 /*   300 */   -30,  -30,  710,  732,  567,  138,  -30,  -30,  149, 1032,
 /*   310 */   840,  -30, 2100,  -30,  727,  -29,  -30,  742,  738,  -29,
 /*   320 */   -30,  750,  746,  -29,  -30,  756,  771,  759,  760,  761,
 /*   330 */   767,  770,  772,  773,  -30,  577,  768,  957,  -30,  684,
 /*   340 */   -30,  453,  763,  774,  775, 2087,  778,  216,  -30,  762,
 /*   350 */   -30,  655,  783,  801,  -30,  879,  -30,  764,  789,  777,
 /*   360 */   780,  792,  918,  -30,  372,  -30,  785,  798,  786,  795,
 /*   370 */   797,  450,  -30,  489,  -30,  334,  244,  -30,  -30,   -2,
 /*   380 */   806,  802,  -29,  811,  -30,  813,  805,  -29,  819,  -30,
 /*   390 */   -30,  232,  513,  694,  -30,  -29,  824,  -30,  267,  390,
 /*   400 */   -30,  -29,  826,  -30,  331,  477,  -30,  -29,  832,  -30,
 /*   410 */   435,  680,  -30,  -29,  833,  -30,  -30,  100,  373,   88,
 /*   420 */   452,  361,  -30,  322,  836,  834,  178,  844,  -30,  -30,
 /*   430 */   847,  838,  650,  295,  846,  -30,  -30,  851,  849,  217,
 /*   440 */   850,  -30,  -30,  -30,
};
#define YY_REDUCE_USE_DFLT (-78)
static short yy_reduce_ofst[] = {
 /*     0 */   -61,  -78,  236,  -78,  -78,  -78,  -78,  -78,  -78,  -78,
 /*    10 */   -78,  -78, 1694,  -78,  -78,  -78,  -77,   81,  -78,  -78,
 /*    20 */   -78,  -78,  -21,   18, 1380,  -78, 1560,  -78,  -78,  -78,
 /*    30 */   -78,  -78,  -78,  -78,  -78,  -78,  -78,  -78,  -78,  -78,
 /*    40 */   -78,  -78,  -78,  -78,  -78,  -78,  -78,  -78,  490,  -78,
 /*    50 */  1200, 1560,  -78,  -78, 1110, 1560,  -78,  412,  -78,  142,
 /*    60 */   -78,  225,  -78,  -78,  298,  -78, 1350, 1560,  -78, 1260,
 /*    70 */  1560,  -78,  451,  -78, 1230, 1560,  -78,  -78,  -78,  536,
 /*    80 */   -78, 1170, 1560,  -78,  -17,  -78,  -78,  330,  -78,  -78,
 /*    90 */   522,  -78,  -78,  554,  568,  -78,  582,  344,  -78,  -78,
 /*   100 */   -78,  -78,  -78,  -78,  -78,  -78,  -78,  -78,   47,  -78,
 /*   110 */   270,  -78,  341,  -78,  398,  -78,  426,  -78,  437,  -78,
 /*   120 */   116,  -78,  504,  -78,  -78,  -78,  -78,  -78,  107,  -78,
 /*   130 */   476,  -78,  -78,  -78,  442,  -78,  506,  -78,  -78,  -78,
 /*   140 */   -78,  -78,  -78,  -78,   46,  -78,  -78,  -78,  110,  -78,
 /*   150 */   -78,   85,  -78,  -78,  -78,  -78,  -78,  -78,  -78,  -78,
 /*   160 */   -78,  -78,  -78,  -78,  -78,  -78,  -78,  -78, 1015,  -78,
 /*   170 */   -78,  -78,  320,  -78,  -78,  -78,  -78,  -78,  -78,  -78,
 /*   180 */   -78,  -78,  -78,  -78,  -78,  -78,  962,  -78,  -78,  -78,
 /*   190 */   -78,  -78,  -78, 1109,  -78,  428,  -78, 1169,  -78,  -78,
 /*   200 */   -78, 1054,  -78,  -78,  311,  -78, 1167,  -78,  -78,  -78,
 /*   210 */  1139,  -78,  -78,  -78,  -78, 1077,  -78,  -78,  192,  -78,
 /*   220 */   -78,  -78,  -78,  -78,  309,  -78,  -78,  -78,  -78,  -78,
 /*   230 */   -78,  -78,  -78,  -78,  -78,  -78,  -78,  -78,  -78,  -78,
 /*   240 */   -78,  -78,  -78,  -78,  -78,  -78,  -78,   68,  -78,  -78,
 /*   250 */   -78,  505,  -78,  -78,  620,  -78,  -78,  657,  -78,  -78,
 /*   260 */   479,  -78,  -78,  401,  -78,  -78,  378,  -78,  -78,  248,
 /*   270 */   -78,  -78,  -78,  -78,  -78,  -78,  -78,  467,  -78, 1018,
 /*   280 */  1560,  -78,  -78,  -78,  545,  -78, 1290, 1560,  -78,  465,
 /*   290 */   -78, 1320, 1560,  -78,  -78,  -78,  -78,  -78, 1410, 1560,
 /*   300 */   -78,  -78,  -78,  -78, 1080, 1560,  -78,  -78,  -78, 1140,
 /*   310 */  1560,  -78,  252,  -78,  -78,  674,  -78,  -78,  -78,  686,
 /*   320 */   -78,  -78,  -78,  688,  -78,  -78,  -78,  -78,  -78,  -78,
 /*   330 */   -78,  -78,  -78,  -78,  -78,  -78,  -78, 1049,  -78, 1560,
 /*   340 */   -78,  -78,  -78,  -78,  -78,   57,  -78, 1530,  -78, 1560,
 /*   350 */   -78,  -78,  -78, 1500,  -78, 1560,  -78,  -78,  -78,  -78,
 /*   360 */   -78,  -78, 1470,  -78, 1560,  -78,  -78,  -78,  -78,  -78,
 /*   370 */   -78, 1440,  -78, 1560,  -78,  -30,   81,  -78,  -78,  -78,
 /*   380 */   -78,  -78,  747,  -78,  -78,  -78,  -78,  752,  -78,  -78,
 /*   390 */   -78,  -78,  -78,  -78,  -78,  765,  -78,  -78,  -78,  -78,
 /*   400 */   -78,  766,  -78,  -78,  -78,  -78,  -78,  776,  -78,  -78,
 /*   410 */   -78,  -78,  -78,  782,  -78,  -78,  -78,  204,  358,   81,
 /*   420 */   -78,   81,  -78,   81,  -78,  -78, 1563,  -78,  -78,  -78,
 /*   430 */   -78,  -78,  -78, 1788,  -78,  -78,  -78,  -78,  -78, 1882,
 /*   440 */   -78,  -78,  -78,  -78,
};
static YYACTIONTYPE yy_default[] = {
 /*     0 */   645,  645,  645,  446,  448,  449,  450,  645,  645,  451,
 /*    10 */   645,  645,  645,  645,  452,  456,  645,  645,  481,  645,
 /*    20 */   645,  645,  645,  645,  645,  485,  645,  489,  513,  515,
 /*    30 */   516,  517,  518,  519,  520,  521,  522,  523,  524,  525,
 /*    40 */   526,  527,  528,  529,  645,  530,  645,  531,  645,  645,
 /*    50 */   645,  645,  534,  645,  645,  645,  535,  645,  645,  645,
 /*    60 */   538,  645,  539,  540,  645,  645,  645,  542,  645,  645,
 /*    70 */   645,  545,  645,  645,  645,  645,  547,  645,  645,  645,
 /*    80 */   645,  645,  645,  548,  645,  645,  552,  645,  553,  645,
 /*    90 */   645,  555,  585,  645,  645,  586,  645,  645,  587,  645,
 /*   100 */   645,  645,  640,  645,  641,  645,  642,  589,  645,  590,
 /*   110 */   645,  591,  645,  592,  645,  593,  645,  599,  645,  600,
 /*   120 */   645,  601,  645,  645,  594,  605,  645,  595,  645,  624,
 /*   130 */   645,  602,  645,  645,  645,  603,  645,  604,  606,  607,
 /*   140 */   608,  609,  610,  611,  645,  612,  645,  613,  645,  625,
 /*   150 */   645,  645,  627,  632,  633,  634,  635,  636,  637,  638,
 /*   160 */   639,  628,  633,  635,  634,  626,  645,  614,  645,  615,
 /*   170 */   645,  616,  645,  619,  622,  623,  645,  620,  621,  645,
 /*   180 */   645,  596,  645,  597,  645,  598,  645,  645,  617,  618,
 /*   190 */   588,  645,  645,  645,  556,  645,  645,  645,  557,  645,
 /*   200 */   645,  645,  558,  645,  645,  645,  645,  559,  645,  645,
 /*   210 */   645,  560,  561,  562,  554,  645,  645,  564,  645,  565,
 /*   220 */   567,  566,  645,  568,  645,  645,  569,  645,  645,  645,
 /*   230 */   645,  570,  645,  645,  645,  571,  645,  645,  572,  645,
 /*   240 */   645,  573,  645,  645,  574,  645,  645,  645,  645,  645,
 /*   250 */   575,  645,  645,  576,  645,  581,  583,  645,  584,  582,
 /*   260 */   645,  645,  577,  645,  645,  578,  645,  645,  579,  645,
 /*   270 */   645,  580,  645,  645,  645,  644,  514,  645,  645,  645,
 /*   280 */   645,  549,  645,  645,  645,  645,  645,  645,  550,  645,
 /*   290 */   645,  645,  645,  551,  546,  645,  544,  645,  645,  543,
 /*   300 */   541,  536,  532,  645,  645,  645,  537,  533,  645,  645,
 /*   310 */   645,  490,  645,  498,  500,  645,  503,  645,  501,  645,
 /*   320 */   504,  645,  502,  645,  505,  645,  645,  645,  645,  645,
 /*   330 */   645,  645,  645,  645,  499,  645,  645,  645,  486,  645,
 /*   340 */   493,  645,  645,  645,  645,  645,  645,  645,  487,  645,
 /*   350 */   491,  645,  645,  645,  488,  645,  492,  645,  645,  645,
 /*   360 */   645,  645,  645,  495,  645,  497,  645,  645,  645,  645,
 /*   370 */   645,  645,  494,  645,  496,  645,  645,  479,  482,  645,
 /*   380 */   645,  645,  645,  645,  483,  645,  645,  645,  645,  484,
 /*   390 */   469,  645,  645,  645,  471,  645,  645,  474,  645,  645,
 /*   400 */   476,  645,  645,  478,  645,  645,  472,  645,  645,  473,
 /*   410 */   645,  645,  475,  645,  645,  477,  480,  645,  645,  645,
 /*   420 */   645,  645,  470,  645,  645,  645,  645,  645,  453,  457,
 /*   430 */   645,  645,  645,  645,  645,  454,  458,  645,  645,  645,
 /*   440 */   645,  455,  459,  447,
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
  "LESS",          "GREATER",       "NOTIDENTICAL",  "NOTEQUALS",   
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
  "LET",           "ARROW",         "SBRACKET_OPEN",  "SBRACKET_CLOSE",
  "INCR",          "DECR",          "ECHO",          "RETURN",      
  "UNSET",         "INTEGER",       "STRING",        "THROW",       
  "DOUBLE",        "NULL",          "TRUE",          "FALSE",       
  "error",         "program",       "xx_language",   "xx_top_statement_list",
  "xx_top_statement",  "xx_namespace_def",  "xx_class_def",  "xx_comment",  
  "xx_class_definition",  "xx_class_properties_definition",  "xx_class_consts_definition",  "xx_class_methods_definition",
  "xx_class_property_definition",  "xx_literal_expr",  "xx_class_const_definition",  "xx_class_method_definition",
  "xx_parameter_list",  "xx_statement_list",  "xx_parameter",  "xx_parameter_type",
  "xx_parameter_cast",  "xx_statement",  "xx_let_statement",  "xx_if_statement",
  "xx_loop_statement",  "xx_echo_statement",  "xx_return_statement",  "xx_mcall_statement",
  "xx_unset_statement",  "xx_throw_statement",  "xx_declare_statement",  "xx_break_statement",
  "xx_continue_statement",  "xx_while_statement",  "xx_switch_statement",  "xx_for_statement",
  "xx_eval_expr",  "xx_case_clauses",  "xx_case_clause",  "xx_common_expr",
  "xx_let_assignments",  "xx_let_assignment",  "xx_assign_expr",  "xx_index_expr",
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
 /* 111 */ "xx_let_assignment ::= IDENTIFIER ASSIGN xx_assign_expr",
 /* 112 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER ASSIGN xx_assign_expr",
 /* 113 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE ASSIGN xx_assign_expr",
 /* 114 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER SBRACKET_OPEN xx_index_expr SBRACKET_CLOSE ASSIGN xx_assign_expr",
 /* 115 */ "xx_let_assignment ::= IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE ASSIGN xx_assign_expr",
 /* 116 */ "xx_let_assignment ::= IDENTIFIER SBRACKET_OPEN xx_index_expr SBRACKET_CLOSE ASSIGN xx_assign_expr",
 /* 117 */ "xx_let_assignment ::= IDENTIFIER INCR",
 /* 118 */ "xx_let_assignment ::= IDENTIFIER DECR",
 /* 119 */ "xx_index_expr ::= xx_common_expr",
 /* 120 */ "xx_echo_statement ::= ECHO xx_echo_expressions DOTCOMMA",
 /* 121 */ "xx_echo_expressions ::= xx_echo_expressions COMMA xx_echo_expression",
 /* 122 */ "xx_echo_expressions ::= xx_echo_expression",
 /* 123 */ "xx_echo_expression ::= xx_common_expr",
 /* 124 */ "xx_mcall_statement ::= xx_mcall_expr DOTCOMMA",
 /* 125 */ "xx_return_statement ::= RETURN xx_common_expr DOTCOMMA",
 /* 126 */ "xx_unset_statement ::= UNSET IDENTIFIER ARROW IDENTIFIER DOTCOMMA",
 /* 127 */ "xx_unset_statement ::= UNSET IDENTIFIER SBRACKET_OPEN IDENTIFIER SBRACKET_CLOSE DOTCOMMA",
 /* 128 */ "xx_unset_statement ::= UNSET IDENTIFIER SBRACKET_OPEN INTEGER SBRACKET_CLOSE DOTCOMMA",
 /* 129 */ "xx_unset_statement ::= UNSET IDENTIFIER SBRACKET_OPEN STRING SBRACKET_CLOSE DOTCOMMA",
 /* 130 */ "xx_throw_statement ::= THROW IDENTIFIER DOTCOMMA",
 /* 131 */ "xx_throw_statement ::= THROW NEW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE DOTCOMMA",
 /* 132 */ "xx_declare_statement ::= TYPE_INTEGER xx_declare_variable_list DOTCOMMA",
 /* 133 */ "xx_declare_statement ::= TYPE_DOUBLE xx_declare_variable_list DOTCOMMA",
 /* 134 */ "xx_declare_statement ::= TYPE_STRING xx_declare_variable_list DOTCOMMA",
 /* 135 */ "xx_declare_statement ::= TYPE_BOOL xx_declare_variable_list DOTCOMMA",
 /* 136 */ "xx_declare_statement ::= TYPE_VAR xx_declare_variable_list DOTCOMMA",
 /* 137 */ "xx_declare_variable_list ::= xx_declare_variable_list COMMA xx_declare_variable",
 /* 138 */ "xx_declare_variable_list ::= xx_declare_variable",
 /* 139 */ "xx_declare_variable ::= IDENTIFIER",
 /* 140 */ "xx_declare_variable ::= IDENTIFIER ASSIGN xx_literal_expr",
 /* 141 */ "xx_assign_expr ::= xx_common_expr",
 /* 142 */ "xx_common_expr ::= NOT xx_common_expr",
 /* 143 */ "xx_common_expr ::= ISSET xx_isset_expr",
 /* 144 */ "xx_common_expr ::= xx_common_expr EQUALS xx_common_expr",
 /* 145 */ "xx_common_expr ::= xx_common_expr NOTEQUALS xx_common_expr",
 /* 146 */ "xx_common_expr ::= xx_common_expr IDENTICAL xx_common_expr",
 /* 147 */ "xx_common_expr ::= xx_common_expr NOTIDENTICAL xx_common_expr",
 /* 148 */ "xx_common_expr ::= xx_common_expr LESS xx_common_expr",
 /* 149 */ "xx_common_expr ::= xx_common_expr GREATER xx_common_expr",
 /* 150 */ "xx_common_expr ::= PARENTHESES_OPEN xx_common_expr PARENTHESES_CLOSE",
 /* 151 */ "xx_common_expr ::= IDENTIFIER ARROW IDENTIFIER",
 /* 152 */ "xx_common_expr ::= IDENTIFIER SBRACKET_OPEN IDENTIFIER SBRACKET_CLOSE",
 /* 153 */ "xx_common_expr ::= IDENTIFIER SBRACKET_OPEN INTEGER SBRACKET_CLOSE",
 /* 154 */ "xx_common_expr ::= IDENTIFIER SBRACKET_OPEN STRING SBRACKET_CLOSE",
 /* 155 */ "xx_common_expr ::= xx_common_expr ADD xx_common_expr",
 /* 156 */ "xx_common_expr ::= xx_common_expr SUB xx_common_expr",
 /* 157 */ "xx_common_expr ::= xx_common_expr CONCAT xx_common_expr",
 /* 158 */ "xx_common_expr ::= INSTANCEOF xx_common_expr",
 /* 159 */ "xx_common_expr ::= FETCH IDENTIFIER COMMA xx_isset_expr",
 /* 160 */ "xx_common_expr ::= TYPEOF xx_common_expr",
 /* 161 */ "xx_common_expr ::= IDENTIFIER",
 /* 162 */ "xx_common_expr ::= INTEGER",
 /* 163 */ "xx_common_expr ::= STRING",
 /* 164 */ "xx_common_expr ::= DOUBLE",
 /* 165 */ "xx_common_expr ::= NULL",
 /* 166 */ "xx_common_expr ::= TRUE",
 /* 167 */ "xx_common_expr ::= FALSE",
 /* 168 */ "xx_common_expr ::= SBRACKET_OPEN SBRACKET_CLOSE",
 /* 169 */ "xx_common_expr ::= SBRACKET_OPEN xx_array_list SBRACKET_CLOSE",
 /* 170 */ "xx_common_expr ::= NEW IDENTIFIER",
 /* 171 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 172 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 173 */ "xx_common_expr ::= IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 174 */ "xx_common_expr ::= IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 175 */ "xx_common_expr ::= xx_mcall_expr",
 /* 176 */ "xx_mcall_expr ::= IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 177 */ "xx_mcall_expr ::= IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 178 */ "xx_call_parameters ::= xx_call_parameters COMMA xx_call_parameter",
 /* 179 */ "xx_call_parameters ::= xx_call_parameter",
 /* 180 */ "xx_call_parameter ::= xx_common_expr",
 /* 181 */ "xx_array_list ::= xx_array_list COMMA xx_array_item",
 /* 182 */ "xx_array_list ::= xx_array_item",
 /* 183 */ "xx_array_item ::= xx_array_key COLON xx_array_value",
 /* 184 */ "xx_array_item ::= xx_array_value",
 /* 185 */ "xx_array_key ::= IDENTIFIER",
 /* 186 */ "xx_array_key ::= STRING",
 /* 187 */ "xx_array_key ::= INTEGER",
 /* 188 */ "xx_array_value ::= xx_literal_expr",
 /* 189 */ "xx_array_value ::= IDENTIFIER",
 /* 190 */ "xx_literal_expr ::= INTEGER",
 /* 191 */ "xx_literal_expr ::= STRING",
 /* 192 */ "xx_literal_expr ::= DOUBLE",
 /* 193 */ "xx_literal_expr ::= NULL",
 /* 194 */ "xx_literal_expr ::= FALSE",
 /* 195 */ "xx_literal_expr ::= TRUE",
 /* 196 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN IDENTIFIER SBRACKET_CLOSE",
 /* 197 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN INTEGER SBRACKET_CLOSE",
 /* 198 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN STRING SBRACKET_CLOSE",
 /* 199 */ "xx_eval_expr ::= xx_common_expr",
 /* 200 */ "xx_comment ::= COMMENT",
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
// 760 "parser.lemon"
{
	/*if ((yypminor->yy0)) {
		if ((yypminor->yy0)->free_flag) {
			efree((yypminor->yy0)->token);
		}
		efree((yypminor->yy0));
	}*/
}
// 1843 "parser.c"
      break;
    case 70:
// 773 "parser.lemon"
{ json_object_put((yypminor->yy175)); }
// 1848 "parser.c"
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
  { 69, 1 },
  { 70, 1 },
  { 71, 2 },
  { 71, 1 },
  { 72, 1 },
  { 72, 1 },
  { 72, 1 },
  { 73, 3 },
  { 74, 5 },
  { 74, 7 },
  { 74, 6 },
  { 74, 8 },
  { 74, 4 },
  { 74, 6 },
  { 74, 5 },
  { 74, 7 },
  { 76, 1 },
  { 76, 1 },
  { 76, 1 },
  { 76, 2 },
  { 76, 2 },
  { 76, 2 },
  { 76, 2 },
  { 76, 3 },
  { 76, 3 },
  { 77, 2 },
  { 77, 1 },
  { 80, 4 },
  { 80, 3 },
  { 80, 5 },
  { 80, 6 },
  { 80, 3 },
  { 80, 4 },
  { 80, 5 },
  { 80, 6 },
  { 78, 2 },
  { 78, 1 },
  { 79, 2 },
  { 79, 1 },
  { 82, 6 },
  { 82, 5 },
  { 83, 7 },
  { 83, 8 },
  { 83, 8 },
  { 83, 9 },
  { 83, 8 },
  { 83, 9 },
  { 83, 9 },
  { 83, 10 },
  { 83, 9 },
  { 83, 7 },
  { 83, 8 },
  { 83, 8 },
  { 83, 9 },
  { 84, 3 },
  { 84, 1 },
  { 86, 1 },
  { 86, 2 },
  { 86, 2 },
  { 86, 3 },
  { 86, 4 },
  { 86, 4 },
  { 88, 3 },
  { 87, 1 },
  { 87, 1 },
  { 87, 1 },
  { 87, 1 },
  { 87, 1 },
  { 87, 1 },
  { 85, 2 },
  { 85, 1 },
  { 89, 1 },
  { 89, 1 },
  { 89, 1 },
  { 89, 1 },
  { 89, 1 },
  { 89, 1 },
  { 89, 1 },
  { 89, 1 },
  { 89, 1 },
  { 89, 1 },
  { 89, 1 },
  { 89, 1 },
  { 89, 1 },
  { 89, 1 },
  { 89, 1 },
  { 99, 2 },
  { 100, 2 },
  { 91, 4 },
  { 91, 7 },
  { 91, 5 },
  { 91, 9 },
  { 91, 8 },
  { 91, 8 },
  { 102, 4 },
  { 102, 5 },
  { 105, 2 },
  { 105, 1 },
  { 106, 4 },
  { 106, 3 },
  { 92, 3 },
  { 92, 4 },
  { 101, 4 },
  { 101, 5 },
  { 103, 7 },
  { 103, 8 },
  { 103, 9 },
  { 103, 10 },
  { 90, 3 },
  { 108, 3 },
  { 108, 1 },
  { 109, 3 },
  { 109, 5 },
  { 109, 7 },
  { 109, 8 },
  { 109, 5 },
  { 109, 6 },
  { 109, 2 },
  { 109, 2 },
  { 111, 1 },
  { 93, 3 },
  { 112, 3 },
  { 112, 1 },
  { 113, 1 },
  { 95, 2 },
  { 94, 3 },
  { 96, 5 },
  { 96, 6 },
  { 96, 6 },
  { 96, 6 },
  { 97, 3 },
  { 97, 7 },
  { 98, 3 },
  { 98, 3 },
  { 98, 3 },
  { 98, 3 },
  { 98, 3 },
  { 116, 3 },
  { 116, 1 },
  { 117, 1 },
  { 117, 3 },
  { 110, 1 },
  { 107, 2 },
  { 107, 2 },
  { 107, 3 },
  { 107, 3 },
  { 107, 3 },
  { 107, 3 },
  { 107, 3 },
  { 107, 3 },
  { 107, 3 },
  { 107, 3 },
  { 107, 4 },
  { 107, 4 },
  { 107, 4 },
  { 107, 3 },
  { 107, 3 },
  { 107, 3 },
  { 107, 2 },
  { 107, 4 },
  { 107, 2 },
  { 107, 1 },
  { 107, 1 },
  { 107, 1 },
  { 107, 1 },
  { 107, 1 },
  { 107, 1 },
  { 107, 1 },
  { 107, 2 },
  { 107, 3 },
  { 107, 2 },
  { 107, 4 },
  { 107, 5 },
  { 107, 4 },
  { 107, 3 },
  { 107, 1 },
  { 114, 6 },
  { 114, 5 },
  { 115, 3 },
  { 115, 1 },
  { 120, 1 },
  { 119, 3 },
  { 119, 1 },
  { 121, 3 },
  { 121, 1 },
  { 122, 1 },
  { 122, 1 },
  { 122, 1 },
  { 123, 1 },
  { 123, 1 },
  { 81, 1 },
  { 81, 1 },
  { 81, 1 },
  { 81, 1 },
  { 81, 1 },
  { 81, 1 },
  { 118, 4 },
  { 118, 4 },
  { 118, 4 },
  { 104, 1 },
  { 75, 1 },
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
	status->ret = yymsp[0].minor.yy175;
}
// 2266 "parser.c"
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
      case 119:
      case 141:
      case 175:
      case 180:
      case 188:
      case 199:
// 775 "parser.lemon"
{
	yygotominor.yy175 = yymsp[0].minor.yy175;
}
// 2297 "parser.c"
        break;
      case 2:
      case 25:
      case 35:
      case 37:
      case 69:
      case 96:
// 779 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_list(yymsp[-1].minor.yy175, yymsp[0].minor.yy175);
}
// 2309 "parser.c"
        break;
      case 3:
      case 26:
      case 36:
      case 38:
      case 55:
      case 70:
      case 97:
      case 110:
      case 122:
      case 138:
      case 179:
      case 182:
// 783 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_list(NULL, yymsp[0].minor.yy175);
}
// 2327 "parser.c"
        break;
      case 7:
// 799 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_namespace(yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(17,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2336 "parser.c"
        break;
      case 8:
// 803 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, 0, NULL, status->scanner_state);
  yy_destructor(20,&yymsp[-4].minor);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2346 "parser.c"
        break;
      case 9:
// 807 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-5].minor.yy0, yymsp[-1].minor.yy175, 0, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(20,&yymsp[-6].minor);
  yy_destructor(23,&yymsp[-4].minor);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2357 "parser.c"
        break;
      case 10:
// 811 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, 1, NULL, status->scanner_state);
  yy_destructor(24,&yymsp[-5].minor);
  yy_destructor(20,&yymsp[-4].minor);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2368 "parser.c"
        break;
      case 11:
// 815 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-5].minor.yy0, yymsp[-1].minor.yy175, 1, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(24,&yymsp[-7].minor);
  yy_destructor(20,&yymsp[-6].minor);
  yy_destructor(23,&yymsp[-4].minor);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2380 "parser.c"
        break;
      case 12:
// 819 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-2].minor.yy0, NULL, 0, NULL, status->scanner_state);
  yy_destructor(20,&yymsp[-3].minor);
  yy_destructor(21,&yymsp[-1].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2390 "parser.c"
        break;
      case 13:
// 823 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-4].minor.yy0, NULL, 0, yymsp[-2].minor.yy0, status->scanner_state);
  yy_destructor(20,&yymsp[-5].minor);
  yy_destructor(23,&yymsp[-3].minor);
  yy_destructor(21,&yymsp[-1].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2401 "parser.c"
        break;
      case 14:
// 827 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-2].minor.yy0, NULL, 1, NULL, status->scanner_state);
  yy_destructor(24,&yymsp[-4].minor);
  yy_destructor(20,&yymsp[-3].minor);
  yy_destructor(21,&yymsp[-1].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2412 "parser.c"
        break;
      case 15:
// 831 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-4].minor.yy0, NULL, 1, yymsp[-2].minor.yy0, status->scanner_state);
  yy_destructor(24,&yymsp[-6].minor);
  yy_destructor(20,&yymsp[-5].minor);
  yy_destructor(23,&yymsp[-3].minor);
  yy_destructor(21,&yymsp[-1].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2424 "parser.c"
        break;
      case 16:
// 835 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(yymsp[0].minor.yy175, NULL, NULL, status->scanner_state);
}
// 2431 "parser.c"
        break;
      case 17:
// 839 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(NULL, NULL, yymsp[0].minor.yy175, status->scanner_state);
}
// 2438 "parser.c"
        break;
      case 18:
// 843 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(NULL, yymsp[0].minor.yy175, NULL, status->scanner_state);
}
// 2445 "parser.c"
        break;
      case 19:
// 847 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(yymsp[-1].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
}
// 2452 "parser.c"
        break;
      case 20:
// 851 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(yymsp[-1].minor.yy175, NULL, yymsp[0].minor.yy175, status->scanner_state);
}
// 2459 "parser.c"
        break;
      case 21:
// 855 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(yymsp[0].minor.yy175, NULL, yymsp[-1].minor.yy175, status->scanner_state);
}
// 2466 "parser.c"
        break;
      case 22:
// 859 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(NULL, yymsp[0].minor.yy175, yymsp[-1].minor.yy175, status->scanner_state);
}
// 2473 "parser.c"
        break;
      case 23:
// 863 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(yymsp[-2].minor.yy175, yymsp[0].minor.yy175, yymsp[-1].minor.yy175, status->scanner_state);
}
// 2480 "parser.c"
        break;
      case 24:
// 867 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(yymsp[-1].minor.yy175, yymsp[0].minor.yy175, yymsp[-2].minor.yy175, status->scanner_state);
}
// 2487 "parser.c"
        break;
      case 27:
// 879 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_property(XX_T_PUBLIC, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(26,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2496 "parser.c"
        break;
      case 28:
// 883 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_property(XX_T_PUBLIC, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(26,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2505 "parser.c"
        break;
      case 29:
// 887 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_property(XX_T_PUBLIC, yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, NULL, status->scanner_state);
  yy_destructor(26,&yymsp[-4].minor);
  yy_destructor(27,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2515 "parser.c"
        break;
      case 30:
// 891 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_property(XX_T_PUBLIC, yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(26,&yymsp[-4].minor);
  yy_destructor(27,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2525 "parser.c"
        break;
      case 31:
// 895 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_property(XX_T_PROTECTED, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2534 "parser.c"
        break;
      case 32:
// 899 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_property(XX_T_PROTECTED, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(28,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2543 "parser.c"
        break;
      case 33:
// 903 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_property(XX_T_PROTECTED, yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-4].minor);
  yy_destructor(27,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2553 "parser.c"
        break;
      case 34:
// 907 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_property(XX_T_PROTECTED, yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(28,&yymsp[-4].minor);
  yy_destructor(27,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2563 "parser.c"
        break;
      case 39:
// 927 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(29,&yymsp[-4].minor);
  yy_destructor(27,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2573 "parser.c"
        break;
      case 40:
// 931 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, NULL, status->scanner_state);
  yy_destructor(29,&yymsp[-4].minor);
  yy_destructor(27,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2583 "parser.c"
        break;
      case 41:
// 935 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-4].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(26,&yymsp[-6].minor);
  yy_destructor(30,&yymsp[-5].minor);
  yy_destructor(31,&yymsp[-3].minor);
  yy_destructor(32,&yymsp[-2].minor);
  yy_destructor(21,&yymsp[-1].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2596 "parser.c"
        break;
      case 42:
// 939 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-5].minor.yy0, yymsp[-3].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(26,&yymsp[-7].minor);
  yy_destructor(30,&yymsp[-6].minor);
  yy_destructor(31,&yymsp[-4].minor);
  yy_destructor(32,&yymsp[-2].minor);
  yy_destructor(21,&yymsp[-1].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2609 "parser.c"
        break;
      case 43:
// 943 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-4].minor.yy0, NULL, NULL, yymsp[-7].minor.yy0, status->scanner_state);
  yy_destructor(26,&yymsp[-6].minor);
  yy_destructor(30,&yymsp[-5].minor);
  yy_destructor(31,&yymsp[-3].minor);
  yy_destructor(32,&yymsp[-2].minor);
  yy_destructor(21,&yymsp[-1].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2622 "parser.c"
        break;
      case 44:
// 947 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-5].minor.yy0, yymsp[-3].minor.yy175, NULL, yymsp[-8].minor.yy0, status->scanner_state);
  yy_destructor(26,&yymsp[-7].minor);
  yy_destructor(30,&yymsp[-6].minor);
  yy_destructor(31,&yymsp[-4].minor);
  yy_destructor(32,&yymsp[-2].minor);
  yy_destructor(21,&yymsp[-1].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2635 "parser.c"
        break;
      case 45:
// 951 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy175, NULL, status->scanner_state);
  yy_destructor(26,&yymsp[-7].minor);
  yy_destructor(30,&yymsp[-6].minor);
  yy_destructor(31,&yymsp[-4].minor);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2648 "parser.c"
        break;
      case 46:
// 955 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-6].minor.yy0, yymsp[-3].minor.yy175, yymsp[-1].minor.yy175, NULL, status->scanner_state);
  yy_destructor(26,&yymsp[-8].minor);
  yy_destructor(30,&yymsp[-7].minor);
  yy_destructor(31,&yymsp[-5].minor);
  yy_destructor(32,&yymsp[-4].minor);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2661 "parser.c"
        break;
      case 47:
// 959 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy175, yymsp[-8].minor.yy0, status->scanner_state);
  yy_destructor(26,&yymsp[-7].minor);
  yy_destructor(30,&yymsp[-6].minor);
  yy_destructor(31,&yymsp[-4].minor);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2674 "parser.c"
        break;
      case 48:
// 963 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-6].minor.yy0, yymsp[-4].minor.yy175, yymsp[-1].minor.yy175, yymsp[-9].minor.yy0, status->scanner_state);
  yy_destructor(26,&yymsp[-8].minor);
  yy_destructor(30,&yymsp[-7].minor);
  yy_destructor(31,&yymsp[-5].minor);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2687 "parser.c"
        break;
      case 49:
// 967 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-6].minor.yy0, yymsp[-4].minor.yy175, yymsp[-1].minor.yy175, NULL, status->scanner_state);
  yy_destructor(26,&yymsp[-8].minor);
  yy_destructor(30,&yymsp[-7].minor);
  yy_destructor(31,&yymsp[-5].minor);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2700 "parser.c"
        break;
      case 50:
// 971 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(XX_T_PROTECTED, yymsp[-4].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-6].minor);
  yy_destructor(30,&yymsp[-5].minor);
  yy_destructor(31,&yymsp[-3].minor);
  yy_destructor(32,&yymsp[-2].minor);
  yy_destructor(21,&yymsp[-1].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2713 "parser.c"
        break;
      case 51:
// 975 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(XX_T_PROTECTED, yymsp[-4].minor.yy0, NULL, NULL, yymsp[-7].minor.yy0, status->scanner_state);
  yy_destructor(28,&yymsp[-6].minor);
  yy_destructor(30,&yymsp[-5].minor);
  yy_destructor(31,&yymsp[-3].minor);
  yy_destructor(32,&yymsp[-2].minor);
  yy_destructor(21,&yymsp[-1].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2726 "parser.c"
        break;
      case 52:
// 979 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(XX_T_PROTECTED, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy175, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-7].minor);
  yy_destructor(30,&yymsp[-6].minor);
  yy_destructor(31,&yymsp[-4].minor);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2739 "parser.c"
        break;
      case 53:
// 983 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(XX_T_PROTECTED, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy175, yymsp[-8].minor.yy0, status->scanner_state);
  yy_destructor(28,&yymsp[-7].minor);
  yy_destructor(30,&yymsp[-6].minor);
  yy_destructor(31,&yymsp[-4].minor);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2752 "parser.c"
        break;
      case 54:
      case 109:
      case 121:
      case 137:
      case 178:
      case 181:
// 987 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_list(yymsp[-2].minor.yy175, yymsp[0].minor.yy175);
  yy_destructor(1,&yymsp[-1].minor);
}
// 2765 "parser.c"
        break;
      case 56:
// 995 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_parameter(NULL, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 2772 "parser.c"
        break;
      case 57:
// 999 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_parameter(yymsp[-1].minor.yy175, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 2779 "parser.c"
        break;
      case 58:
// 1003 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_parameter(NULL, yymsp[-1].minor.yy175, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 2786 "parser.c"
        break;
      case 59:
// 1007 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_parameter(NULL, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(27,&yymsp[-1].minor);
}
// 2794 "parser.c"
        break;
      case 60:
// 1011 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_parameter(yymsp[-3].minor.yy175, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(27,&yymsp[-1].minor);
}
// 2802 "parser.c"
        break;
      case 61:
// 1015 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_parameter(NULL, yymsp[-3].minor.yy175, yymsp[-2].minor.yy0, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(27,&yymsp[-1].minor);
}
// 2810 "parser.c"
        break;
      case 62:
// 1019 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(4,&yymsp[-2].minor);
  yy_destructor(5,&yymsp[0].minor);
}
// 2819 "parser.c"
        break;
      case 63:
// 1023 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_INTEGER);
  yy_destructor(33,&yymsp[0].minor);
}
// 2827 "parser.c"
        break;
      case 64:
// 1027 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_DOUBLE);
  yy_destructor(34,&yymsp[0].minor);
}
// 2835 "parser.c"
        break;
      case 65:
// 1031 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_BOOL);
  yy_destructor(35,&yymsp[0].minor);
}
// 2843 "parser.c"
        break;
      case 66:
// 1035 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_STRING);
  yy_destructor(36,&yymsp[0].minor);
}
// 2851 "parser.c"
        break;
      case 67:
// 1039 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_ARRAY);
  yy_destructor(37,&yymsp[0].minor);
}
// 2859 "parser.c"
        break;
      case 68:
// 1043 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_VAR);
  yy_destructor(38,&yymsp[0].minor);
}
// 2867 "parser.c"
        break;
      case 86:
// 1115 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_break_statement(status->scanner_state);
  yy_destructor(39,&yymsp[-1].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2876 "parser.c"
        break;
      case 87:
// 1119 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_continue_statement(status->scanner_state);
  yy_destructor(40,&yymsp[-1].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2885 "parser.c"
        break;
      case 88:
// 1123 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_if_statement(yymsp[-2].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(41,&yymsp[-3].minor);
  yy_destructor(21,&yymsp[-1].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2895 "parser.c"
        break;
      case 89:
// 1127 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_if_statement(yymsp[-5].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(41,&yymsp[-6].minor);
  yy_destructor(21,&yymsp[-4].minor);
  yy_destructor(22,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(21,&yymsp[-1].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2908 "parser.c"
        break;
      case 90:
// 1131 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_if_statement(yymsp[-3].minor.yy175, yymsp[-1].minor.yy175, NULL, status->scanner_state);
  yy_destructor(41,&yymsp[-4].minor);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2918 "parser.c"
        break;
      case 91:
// 1135 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_if_statement(yymsp[-7].minor.yy175, yymsp[-5].minor.yy175, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(41,&yymsp[-8].minor);
  yy_destructor(21,&yymsp[-6].minor);
  yy_destructor(22,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-3].minor);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2931 "parser.c"
        break;
      case 92:
// 1139 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_if_statement(yymsp[-6].minor.yy175, yymsp[-4].minor.yy175, NULL, status->scanner_state);
  yy_destructor(41,&yymsp[-7].minor);
  yy_destructor(21,&yymsp[-5].minor);
  yy_destructor(22,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(21,&yymsp[-1].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2944 "parser.c"
        break;
      case 93:
// 1143 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_if_statement(yymsp[-6].minor.yy175, NULL, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(41,&yymsp[-7].minor);
  yy_destructor(21,&yymsp[-5].minor);
  yy_destructor(22,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-3].minor);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2957 "parser.c"
        break;
      case 94:
// 1147 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_switch_statement(yymsp[-2].minor.yy175, NULL, status->scanner_state);
  yy_destructor(43,&yymsp[-3].minor);
  yy_destructor(21,&yymsp[-1].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2967 "parser.c"
        break;
      case 95:
// 1151 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_switch_statement(yymsp[-3].minor.yy175, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(43,&yymsp[-4].minor);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2977 "parser.c"
        break;
      case 98:
// 1163 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_case_clause(yymsp[-2].minor.yy175, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(44,&yymsp[-3].minor);
  yy_destructor(45,&yymsp[-1].minor);
}
// 2986 "parser.c"
        break;
      case 99:
// 1167 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_case_clause(NULL, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[-1].minor);
}
// 2995 "parser.c"
        break;
      case 100:
// 1171 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_loop_statement(NULL, status->scanner_state);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(21,&yymsp[-1].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 3005 "parser.c"
        break;
      case 101:
// 1175 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_loop_statement(yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(47,&yymsp[-3].minor);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 3015 "parser.c"
        break;
      case 102:
// 1179 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_while_statement(yymsp[-2].minor.yy175, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-3].minor);
  yy_destructor(21,&yymsp[-1].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 3025 "parser.c"
        break;
      case 103:
// 1183 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_while_statement(yymsp[-3].minor.yy175, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(48,&yymsp[-4].minor);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 3035 "parser.c"
        break;
      case 104:
// 1187 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_for_statement(yymsp[-3].minor.yy175, NULL, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(49,&yymsp[-6].minor);
  yy_destructor(50,&yymsp[-4].minor);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 3046 "parser.c"
        break;
      case 105:
// 1191 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_for_statement(yymsp[-3].minor.yy175, NULL, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(49,&yymsp[-7].minor);
  yy_destructor(50,&yymsp[-5].minor);
  yy_destructor(51,&yymsp[-4].minor);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 3058 "parser.c"
        break;
      case 106:
// 1195 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_for_statement(yymsp[-3].minor.yy175, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(49,&yymsp[-8].minor);
  yy_destructor(1,&yymsp[-6].minor);
  yy_destructor(50,&yymsp[-4].minor);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 3070 "parser.c"
        break;
      case 107:
// 1199 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_for_statement(yymsp[-3].minor.yy175, yymsp[-8].minor.yy0, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(49,&yymsp[-9].minor);
  yy_destructor(1,&yymsp[-7].minor);
  yy_destructor(50,&yymsp[-5].minor);
  yy_destructor(51,&yymsp[-4].minor);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 3083 "parser.c"
        break;
      case 108:
// 1203 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_statement(yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 3092 "parser.c"
        break;
      case 111:
// 1216 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("variable", yymsp[-2].minor.yy0, NULL, NULL, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(27,&yymsp[-1].minor);
}
// 3100 "parser.c"
        break;
      case 112:
// 1221 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("object-property", yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(53,&yymsp[-3].minor);
  yy_destructor(27,&yymsp[-1].minor);
}
// 3109 "parser.c"
        break;
      case 113:
// 1226 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("object-property-append", yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(53,&yymsp[-5].minor);
  yy_destructor(54,&yymsp[-3].minor);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(27,&yymsp[-1].minor);
}
// 3120 "parser.c"
        break;
      case 114:
// 1231 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("object-property-array-index", yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy175, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(53,&yymsp[-6].minor);
  yy_destructor(54,&yymsp[-4].minor);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(27,&yymsp[-1].minor);
}
// 3131 "parser.c"
        break;
      case 115:
// 1236 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("variable-append", yymsp[-4].minor.yy0, NULL, NULL, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(54,&yymsp[-3].minor);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(27,&yymsp[-1].minor);
}
// 3141 "parser.c"
        break;
      case 116:
// 1241 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("array-index", yymsp[-5].minor.yy0, NULL, yymsp[-3].minor.yy175, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(54,&yymsp[-4].minor);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(27,&yymsp[-1].minor);
}
// 3151 "parser.c"
        break;
      case 117:
// 1246 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("incr", yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(56,&yymsp[0].minor);
}
// 3159 "parser.c"
        break;
      case 118:
// 1251 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("decr", yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(57,&yymsp[0].minor);
}
// 3167 "parser.c"
        break;
      case 120:
// 1259 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_echo_statement(yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(58,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 3176 "parser.c"
        break;
      case 123:
// 1271 "parser.lemon"
{
	yygotominor.yy175 = yymsp[0].minor.yy175;;
}
// 3183 "parser.c"
        break;
      case 124:
// 1276 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_mcall_statement(yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(19,&yymsp[0].minor);
}
// 3191 "parser.c"
        break;
      case 125:
// 1281 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_return_statement(yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(59,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 3200 "parser.c"
        break;
      case 126:
// 1286 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(60,&yymsp[-4].minor);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 3214 "parser.c"
        break;
      case 127:
// 1295 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(60,&yymsp[-5].minor);
  yy_destructor(54,&yymsp[-3].minor);
  yy_destructor(55,&yymsp[-1].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 3229 "parser.c"
        break;
      case 128:
// 1304 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_INTEGER, yymsp[-2].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(60,&yymsp[-5].minor);
  yy_destructor(54,&yymsp[-3].minor);
  yy_destructor(55,&yymsp[-1].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 3244 "parser.c"
        break;
      case 129:
// 1313 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_STRING, yymsp[-2].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(60,&yymsp[-5].minor);
  yy_destructor(54,&yymsp[-3].minor);
  yy_destructor(55,&yymsp[-1].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 3259 "parser.c"
        break;
      case 130:
// 1322 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_throw_exception(yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(63,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 3268 "parser.c"
        break;
      case 131:
// 1327 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_throw_exception(yymsp[-4].minor.yy0, yymsp[-2].minor.yy175, status->scanner_state);
  yy_destructor(63,&yymsp[-6].minor);
  yy_destructor(15,&yymsp[-5].minor);
  yy_destructor(31,&yymsp[-3].minor);
  yy_destructor(32,&yymsp[-1].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 3280 "parser.c"
        break;
      case 132:
// 1331 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_INTEGER, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(33,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 3289 "parser.c"
        break;
      case 133:
// 1335 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_DOUBLE, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(34,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 3298 "parser.c"
        break;
      case 134:
// 1339 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_STRING, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(36,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 3307 "parser.c"
        break;
      case 135:
// 1343 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_BOOL, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(35,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 3316 "parser.c"
        break;
      case 136:
// 1347 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_VAR, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 3325 "parser.c"
        break;
      case 139:
// 1359 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_variable(yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3332 "parser.c"
        break;
      case 140:
// 1363 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_variable(yymsp[-2].minor.yy0, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(27,&yymsp[-1].minor);
}
// 3340 "parser.c"
        break;
      case 142:
// 1371 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("not", yymsp[0].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(16,&yymsp[-1].minor);
}
// 3348 "parser.c"
        break;
      case 143:
// 1375 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("isset", yymsp[0].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(11,&yymsp[-1].minor);
}
// 3356 "parser.c"
        break;
      case 144:
// 1379 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("equals", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(2,&yymsp[-1].minor);
}
// 3364 "parser.c"
        break;
      case 145:
// 1383 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("not-equals", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(7,&yymsp[-1].minor);
}
// 3372 "parser.c"
        break;
      case 146:
// 1387 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("identical", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(3,&yymsp[-1].minor);
}
// 3380 "parser.c"
        break;
      case 147:
// 1391 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("not-identical", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(6,&yymsp[-1].minor);
}
// 3388 "parser.c"
        break;
      case 148:
// 1395 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("less", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(4,&yymsp[-1].minor);
}
// 3396 "parser.c"
        break;
      case 149:
// 1399 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("greater", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(5,&yymsp[-1].minor);
}
// 3404 "parser.c"
        break;
      case 150:
// 1403 "parser.lemon"
{
	yygotominor.yy175 = yymsp[-1].minor.yy175;
  yy_destructor(31,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3413 "parser.c"
        break;
      case 151:
// 1407 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(53,&yymsp[-1].minor);
}
// 3421 "parser.c"
        break;
      case 152:
      case 196:
// 1411 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(55,&yymsp[0].minor);
}
// 3431 "parser.c"
        break;
      case 153:
      case 197:
// 1416 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_INTEGER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(55,&yymsp[0].minor);
}
// 3441 "parser.c"
        break;
      case 154:
      case 198:
// 1421 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_STRING, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(55,&yymsp[0].minor);
}
// 3451 "parser.c"
        break;
      case 155:
// 1426 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("add", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(8,&yymsp[-1].minor);
}
// 3459 "parser.c"
        break;
      case 156:
// 1431 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("sub", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(9,&yymsp[-1].minor);
}
// 3467 "parser.c"
        break;
      case 157:
// 1436 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("concat", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(10,&yymsp[-1].minor);
}
// 3475 "parser.c"
        break;
      case 158:
// 1441 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("instanceof", yymsp[0].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(14,&yymsp[-1].minor);
}
// 3483 "parser.c"
        break;
      case 159:
// 1446 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("fetch", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(12,&yymsp[-3].minor);
  yy_destructor(1,&yymsp[-1].minor);
}
// 3492 "parser.c"
        break;
      case 160:
// 1451 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("typeof", yymsp[0].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(13,&yymsp[-1].minor);
}
// 3500 "parser.c"
        break;
      case 161:
      case 185:
      case 189:
// 1456 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state);
}
// 3509 "parser.c"
        break;
      case 162:
      case 187:
      case 190:
// 1461 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_INTEGER, yymsp[0].minor.yy0, status->scanner_state);
}
// 3518 "parser.c"
        break;
      case 163:
      case 186:
      case 191:
// 1466 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_STRING, yymsp[0].minor.yy0, status->scanner_state);
}
// 3527 "parser.c"
        break;
      case 164:
      case 192:
// 1471 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_DOUBLE, yymsp[0].minor.yy0, status->scanner_state);
}
// 3535 "parser.c"
        break;
      case 165:
      case 193:
// 1476 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_NULL, NULL, status->scanner_state);
  yy_destructor(65,&yymsp[0].minor);
}
// 3544 "parser.c"
        break;
      case 166:
      case 195:
// 1481 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_TRUE, NULL, status->scanner_state);
  yy_destructor(66,&yymsp[0].minor);
}
// 3553 "parser.c"
        break;
      case 167:
      case 194:
// 1486 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_FALSE, NULL, status->scanner_state);
  yy_destructor(67,&yymsp[0].minor);
}
// 3562 "parser.c"
        break;
      case 168:
// 1491 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("empty-array", NULL, NULL, NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-1].minor);
  yy_destructor(55,&yymsp[0].minor);
}
// 3571 "parser.c"
        break;
      case 169:
// 1496 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("array", yymsp[-1].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(55,&yymsp[0].minor);
}
// 3580 "parser.c"
        break;
      case 170:
// 1501 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_new_instance(yymsp[0].minor.yy0, NULL, status->scanner_state);
  yy_destructor(15,&yymsp[-1].minor);
}
// 3588 "parser.c"
        break;
      case 171:
// 1506 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_new_instance(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(15,&yymsp[-3].minor);
  yy_destructor(31,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3598 "parser.c"
        break;
      case 172:
// 1511 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_new_instance(yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(15,&yymsp[-4].minor);
  yy_destructor(31,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3608 "parser.c"
        break;
      case 173:
// 1516 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_fcall(yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(31,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3617 "parser.c"
        break;
      case 174:
// 1521 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_fcall(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(31,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3626 "parser.c"
        break;
      case 176:
// 1531 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_mcall(yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(53,&yymsp[-4].minor);
  yy_destructor(31,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3636 "parser.c"
        break;
      case 177:
// 1536 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_mcall(yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(53,&yymsp[-3].minor);
  yy_destructor(31,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3646 "parser.c"
        break;
      case 183:
// 1560 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_array_item(yymsp[-2].minor.yy175, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(45,&yymsp[-1].minor);
}
// 3654 "parser.c"
        break;
      case 184:
// 1564 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_array_item(NULL, yymsp[0].minor.yy175, status->scanner_state);
}
// 3661 "parser.c"
        break;
      case 200:
// 1631 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_comment(yymsp[0].minor.yy0, status->scanner_state);
}
// 3668 "parser.c"
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

// 3744 "parser.c"
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
