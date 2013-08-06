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
#define YYNSTATE 442
#define YYNRULE 200
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
 /*     0 */   272,  373,   17,  388,  306,  414,  387,  275,  643,    1,
 /*    10 */     2,  441,    4,    5,    6,  251,  260,  266,  263,  341,
 /*    20 */   269,   44,   46,   48,  335,   57,  334,  317,  321,   68,
 /*    30 */    72,   77,  155,  156,   84,  157,  158,  160,  159,  272,
 /*    40 */   215,  224,  227,  307,   80,  242,  275,  416,  153,  419,
 /*    50 */   420,  173,  375,  387,  251,  260,  266,  263,  129,  269,
 /*    60 */    44,   46,   48,  312,   57,  173,  170,  282,   68,   72,
 /*    70 */    77,  175,  338,   84,    3,    4,    5,    6,  272,  215,
 /*    80 */   224,  227,  346,  309,  242,  275,  146,  233,  165,  150,
 /*    90 */   161,  252,  259,  251,  260,  266,  263,  129,  269,   44,
 /*   100 */    46,   48,  337,   57,  173,  176,  509,   68,   72,   77,
 /*   110 */   175,  349,   84,  334,  317,  321,   79,  272,  215,  224,
 /*   120 */   227,  348,  440,  242,  275,  389,  402,  185,  408,  382,
 /*   130 */   236,  239,  251,  260,  266,  263,  508,  269,   44,   46,
 /*   140 */    48,  308,   57,  334,  317,  321,   68,   72,   77,  463,
 /*   150 */   507,   84,  418,  402,  148,  408,  272,  215,  224,  227,
 /*   160 */    25,  458,  242,  275,  389,  402,  374,  408,  382,  375,
 /*   170 */   387,  251,  260,  266,  263,  106,  269,   44,   46,   48,
 /*   180 */   506,   57,  116,  118,  120,   68,   72,   77,  466,  505,
 /*   190 */    84,  339,   19,  391,  364,  272,  215,  224,  227,  352,
 /*   200 */   434,  242,  275,  389,  402,  341,  408,  382,  147,  183,
 /*   210 */   251,  260,  266,  263,  254,  269,   44,   46,   48,   18,
 /*   220 */    57,  313,  317,  321,   68,   72,   77,  464,  206,   84,
 /*   230 */   339,   19,  268,  364,  272,  215,  224,  227,  372,  462,
 /*   240 */   242,  275,  376,   19,  254,  364,  382,  261,  259,  251,
 /*   250 */   260,  266,  263,  254,  269,   44,   46,   48,  340,   57,
 /*   260 */   355,  377,  253,   68,   72,   77,  461,  255,   84,  339,
 /*   270 */    19,  262,  364,  272,  215,  224,  227,  354,   15,  242,
 /*   280 */   275,  389,  402,  254,  408,  382,  264,  259,  251,  260,
 /*   290 */   266,  263,  254,  269,   44,   46,   48,  390,   57,  396,
 /*   300 */   377,  265,   68,   72,   77,  460,  504,   84,  339,   19,
 /*   310 */   271,  364,  272,  215,  224,  227,  363,  427,  242,  275,
 /*   320 */   389,  402,   87,  408,  382,  267,  259,  251,  260,  266,
 /*   330 */   263,  218,  269,   44,   46,   48,   12,   57,  422,  172,
 /*   340 */    86,   68,   72,   77,  465,  153,   84,  339,   19,  217,
 /*   350 */   364,  272,  215,  224,  227,  361,  459,  242,  275,  389,
 /*   360 */   402,  153,  408,  382,   60,  104,  251,  260,  266,  263,
 /*   370 */   249,  269,   44,   46,   48,  245,   57,  220,  243,  503,
 /*   380 */    68,   72,   77,  219,  173,   84,   64,  152,  297,  378,
 /*   390 */   272,  215,  224,  227,   56,  180,  242,  275,  327,  101,
 /*   400 */   380,  149,  150,  161,  326,  251,  260,  266,  263,  379,
 /*   410 */   269,   44,   46,   48,  193,   57,  270,  259,  278,   68,
 /*   420 */    72,   77,  417,  388,   84,  173,  387,  129,  381,  272,
 /*   430 */   215,  224,  227,   76,  173,  242,  275,  324,  182,  184,
 /*   440 */   174,  195,  103,  105,  251,  260,  266,  263,  383,  269,
 /*   450 */    44,   46,   48,  386,   57,   85,  214,   62,   68,   72,
 /*   460 */    77,  172,  340,   84,  355,  390,  285,  396,  272,  215,
 /*   470 */   224,  227,  370,  173,  242,  275,  181,   49,  186,   64,
 /*   480 */   295,  297,  397,  251,  260,  266,  263,  173,  269,   44,
 /*   490 */    46,   48,  171,   57,  356,  209,  290,   68,   72,   77,
 /*   500 */   126,  179,   84,  173,  210,  190,  403,  272,  215,  224,
 /*   510 */   227,   83,  173,  242,  275,  431,   58,  435,   20,  295,
 /*   520 */    61,  300,  251,  260,  266,  263,  173,  269,   44,   46,
 /*   530 */    48,  322,   57,   95,  384,  107,   68,   72,   77,  172,
 /*   540 */   173,   84,  173,  312,  409,  237,  272,  215,  224,  227,
 /*   550 */   301,  172,  242,  275,  323,   73,  365,  102,  295,  385,
 /*   560 */    63,  251,  260,  266,  263,  173,  269,   44,   46,   48,
 /*   570 */   177,   57,  109,  312,  336,   68,   72,   77,   14,  173,
 /*   580 */    84,  320,  188,  111,  256,  272,  215,  224,  227,   27,
 /*   590 */   173,  242,  275,  229,  232,    9,   54,  319,  238,  318,
 /*   600 */   251,  260,  266,  263,  350,  269,   44,   46,   48,  392,
 /*   610 */    57,  113,  436,  115,   68,   72,   77,  393,  173,   84,
 /*   620 */   173,  395,  117,  100,  272,  215,  224,  227,  294,  173,
 /*   630 */   242,  275,  316,  394,  315,  244,  119,   53,   98,  251,
 /*   640 */   260,  266,  263,  173,  269,   44,   46,   48,  398,   57,
 /*   650 */   121,  404,  437,   68,   72,   77,  399,  173,   84,  405,
 /*   660 */    99,  123,   78,  272,  215,  224,  227,  281,  173,  242,
 /*   670 */   275,  304,  303,   65,  168,  225,  401,  400,  251,  260,
 /*   680 */   266,  263,  173,  269,   44,   46,   48,  410,   57,  167,
 /*   690 */   127,  131,   68,   72,   77,  411,  298,   84,  173,   66,
 /*   700 */   137,  250,  272,  215,  224,  227,  311,  173,  242,  275,
 /*   710 */   628,  223,  128,  407,   22,  406,   50,  251,  260,  266,
 /*   720 */   263,  627,  269,   44,   46,   48,  439,   57,  240,  247,
 /*   730 */   626,   68,   72,   77,  283,   88,   84,  284,   89,  412,
 /*   740 */   413,  272,  215,  224,  227,  288,   21,  242,  275,  228,
 /*   750 */   442,  241,  246,  273,   47,  274,  251,  260,  266,  263,
 /*   760 */    11,  269,   44,   46,   48,  230,   57,  231,   20,  423,
 /*   770 */    68,   72,   77,  424,  133,   84,    8,  426,  151,  134,
 /*   780 */   272,  215,  224,  227,  293,   69,  242,  275,  258,  429,
 /*   790 */   235,  234,  430,   74,   45,  251,  260,  266,  263,  359,
 /*   800 */   269,   44,   46,   48,  135,   57,  360,  358,  357,   68,
 /*   810 */    72,   77,  200,  197,   84,  257,  351,  356,  192,  272,
 /*   820 */   215,  224,  227,  296,  201,  242,  275,  433,  365,  366,
 /*   830 */   345,  368,  342,  367,  251,  260,  266,  263,  343,  269,
 /*   840 */    44,   46,   48,  369,   57,   59,  326,  326,   68,   72,
 /*   850 */    77,  326,  326,   84,  326,  326,  326,  326,  272,  215,
 /*   860 */   224,  227,  302,  326,  242,  275,  326,  326,  326,  326,
 /*   870 */   326,  326,  326,  251,  260,  266,  263,  326,  269,   44,
 /*   880 */    46,   48,  326,   57,  326,  326,  326,   68,   72,   77,
 /*   890 */   326,  326,   84,  326,  326,  326,  326,  272,  215,  224,
 /*   900 */   227,   71,  326,  242,  275,  326,  326,  326,  326,  326,
 /*   910 */   326,  326,  251,  260,  266,  263,  326,  269,   44,   46,
 /*   920 */    48,  326,   57,  326,  326,  326,   68,   72,   77,  326,
 /*   930 */   326,   84,  326,  326,  326,  326,  272,  215,  224,  227,
 /*   940 */    52,  326,  242,  275,  326,  326,  326,  326,  326,  326,
 /*   950 */   326,  251,  260,  266,  263,  326,  269,   44,   46,   48,
 /*   960 */   326,   57,  326,  326,  326,   68,   72,   77,  220,  326,
 /*   970 */    84,  326,  272,  216,  221,  173,  215,  224,  227,  275,
 /*   980 */   326,  242,  326,  326,  326,  326,  326,  251,  260,  266,
 /*   990 */   263,  129,  269,   44,   46,   48,  326,   57,  173,  248,
 /*  1000 */   326,   68,   72,   77,  175,  326,   84,  326,  326,  326,
 /*  1010 */   326,  326,  215,  224,  227,   43,  326,  242,   90,  326,
 /*  1020 */    13,   16,  415,  421,  420,  299,  414,  387,  326,  276,
 /*  1030 */    29,   30,   31,   32,   33,   34,   35,   36,   37,   38,
 /*  1040 */    39,   40,   41,   42,  191,  204,   43,  212,  213,  326,
 /*  1050 */     7,  326,  326,   10,  222,  326,  347,  428,  275,  326,
 /*  1060 */   276,   29,   30,   31,   32,   33,   34,   35,   36,   37,
 /*  1070 */    38,   39,   40,   41,   42,  129,  326,   43,  326,  326,
 /*  1080 */   326,  326,  173,  187,  326,  222,  326,  310,  175,  326,
 /*  1090 */   326,  276,   29,   30,   31,   32,   33,   34,   35,   36,
 /*  1100 */    37,   38,   39,   40,   41,   42,   92,   43,  326,  207,
 /*  1110 */   326,   92,   92,  173,  198,   91,  222,  362,  173,  173,
 /*  1120 */   326,  276,   29,   30,   31,   32,   33,   34,   35,   36,
 /*  1130 */    37,   38,   39,   40,   41,   42,  326,   43,  203,  326,
 /*  1140 */   326,   92,  208,  326,  202,  173,  222,   26,  173,  326,
 /*  1150 */   326,  276,   29,   30,   31,   32,   33,   34,   35,   36,
 /*  1160 */    37,   38,   39,   40,   41,   42,  203,   43,   92,  326,
 /*  1170 */   199,  194,  326,  173,  326,  173,  222,   55,  326,  326,
 /*  1180 */   326,  276,   29,   30,   31,   32,   33,   34,   35,   36,
 /*  1190 */    37,   38,   39,   40,   41,   42,   92,   43,  326,  211,
 /*  1200 */   326,  326,  326,  173,  326,  326,  222,  353,  326,  326,
 /*  1210 */   326,  276,   29,   30,   31,   32,   33,   34,   35,   36,
 /*  1220 */    37,   38,   39,   40,   41,   42,  326,   43,  326,  326,
 /*  1230 */   326,  326,  326,  326,  326,  326,  222,   82,  326,  326,
 /*  1240 */   326,  276,   29,   30,   31,   32,   33,   34,   35,   36,
 /*  1250 */    37,   38,   39,   40,   41,   42,  326,   43,  326,  326,
 /*  1260 */   326,  326,  326,  326,  326,  326,  222,   70,  326,  326,
 /*  1270 */   326,  276,   29,   30,   31,   32,   33,   34,   35,   36,
 /*  1280 */    37,   38,   39,   40,   41,   42,  326,   43,  326,  326,
 /*  1290 */   326,  326,  326,  326,  326,  326,  222,  292,  326,  326,
 /*  1300 */   326,  276,   29,   30,   31,   32,   33,   34,   35,   36,
 /*  1310 */    37,   38,   39,   40,   41,   42,  326,   43,  326,  326,
 /*  1320 */   326,  326,  326,  326,  326,  326,  222,  371,  326,  326,
 /*  1330 */   326,  276,   29,   30,   31,   32,   33,   34,   35,   36,
 /*  1340 */    37,   38,   39,   40,   41,   42,  326,   43,  326,  326,
 /*  1350 */   326,  326,  326,  326,  326,  326,  222,  305,  326,  326,
 /*  1360 */   326,  276,   29,   30,   31,   32,   33,   34,   35,   36,
 /*  1370 */    37,   38,   39,   40,   41,   42,  326,   43,  326,  326,
 /*  1380 */   326,  326,  326,  326,  326,  326,  222,   51,  326,  326,
 /*  1390 */   326,  276,   29,   30,   31,   32,   33,   34,   35,   36,
 /*  1400 */    37,   38,   39,   40,   41,   42,  326,   43,  326,  326,
 /*  1410 */   326,  326,  326,  326,  326,  326,  222,  280,  326,  326,
 /*  1420 */   326,  276,   29,   30,   31,   32,   33,   34,   35,   36,
 /*  1430 */    37,   38,   39,   40,   41,   42,  326,   43,  326,  326,
 /*  1440 */   326,  326,  326,  326,  326,  326,  222,   67,  326,  326,
 /*  1450 */   326,  276,   29,   30,   31,   32,   33,   34,   35,   36,
 /*  1460 */    37,   38,   39,   40,   41,   42,  326,   43,  326,  326,
 /*  1470 */   326,  326,  326,  326,  326,  326,  222,   75,  326,  326,
 /*  1480 */   326,  276,   29,   30,   31,   32,   33,   34,   35,   36,
 /*  1490 */    37,   38,   39,   40,   41,   42,  326,   43,  326,  326,
 /*  1500 */   326,  326,  326,  326,  326,  326,  222,  287,  326,  326,
 /*  1510 */   326,  276,   29,   30,   31,   32,   33,   34,   35,   36,
 /*  1520 */    37,   38,   39,   40,   41,   42,  162,   43,  326,  326,
 /*  1530 */   326,  425,   16,  415,  421,  420,  222,  414,  387,  326,
 /*  1540 */   326,   28,   29,   30,   31,   32,   33,   34,   35,   36,
 /*  1550 */    37,   38,   39,   40,   41,   42,   97,  132,  136,  130,
 /*  1560 */   166,   94,  326,  125,  326,  326,  222,  326,  326,  164,
 /*  1570 */   163,  326,  157,  158,  160,  159,  122,  189,  326,   93,
 /*  1580 */   108,  112,  114,  110,   96,  116,  118,  120,  326,   93,
 /*  1590 */   108,  112,  114,  110,   96,  116,  118,  120,   81,  144,
 /*  1600 */   326,  326,  326,  326,  326,  326,  138,  139,  640,  140,
 /*  1610 */   141,  142,  143,  326,  326,  326,  326,  326,   97,  132,
 /*  1620 */   136,  130,  166,   94,  326,  125,   93,  108,  112,  114,
 /*  1630 */   110,   96,  116,  118,  120,  326,  326,  326,  122,  178,
 /*  1640 */   326,   93,  108,  112,  114,  110,   96,  116,  118,  120,
 /*  1650 */   326,  438,   16,  415,  421,  420,  124,  414,  387,  326,
 /*  1660 */   286,  144,  326,  326,  326,  326,  326,  326,  138,  139,
 /*  1670 */   326,  140,  141,  142,  143,  326,  326,  326,  326,  326,
 /*  1680 */    97,  132,  136,  130,  166,   94,  326,  125,   93,  108,
 /*  1690 */   112,  114,  110,   96,  116,  118,  120,  326,  326,  326,
 /*  1700 */   122,  169,  326,  326,  326,  226,  326,  326,  326,  326,
 /*  1710 */   326,  326,   97,  132,  136,  130,  166,   94,  326,  125,
 /*  1720 */   326,  326,  326,  144,  326,  326,  326,  326,  326,  326,
 /*  1730 */   138,  139,  122,  140,  141,  142,  143,  326,  326,  326,
 /*  1740 */   325,  326,  154,  326,   97,  132,  136,  130,  166,   94,
 /*  1750 */   326,  125,  326,  326,  314,  144,  205,   24,  326,  326,
 /*  1760 */   326,  326,  138,  139,  122,  140,  141,  142,  143,  328,
 /*  1770 */   329,  330,  331,  332,  333,  326,   97,  132,  136,  130,
 /*  1780 */   166,   94,  326,  125,  277,  155,  156,  144,  157,  158,
 /*  1790 */   160,  159,  326,  326,  138,  139,  122,  140,  141,  142,
 /*  1800 */   143,  326,  326,  326,  325,  326,  326,  326,   97,  132,
 /*  1810 */   136,  130,  166,   94,  326,  125,  289,  326,  314,  144,
 /*  1820 */   326,  326,  326,  326,  326,  326,  138,  139,  122,  140,
 /*  1830 */   141,  142,  143,  328,  329,  330,  331,  332,  333,  326,
 /*  1840 */    97,  132,  136,  130,  166,   94,  326,  125,  326,  326,
 /*  1850 */   326,  144,  196,  326,  326,  326,  326,  326,  138,  139,
 /*  1860 */   122,  140,  141,  142,  143,   93,  108,  112,  114,  110,
 /*  1870 */    96,  116,  118,  120,   93,  108,  112,  114,  110,   96,
 /*  1880 */   116,  118,  120,  144,  291,  326,  326,  162,  326,  326,
 /*  1890 */   138,  139,  326,  140,  141,  142,  143,   93,  108,  112,
 /*  1900 */   114,  110,   96,  116,  118,  120,   93,  108,  112,  114,
 /*  1910 */   110,   96,  116,  118,  120,  325,  279,  326,  326,  432,
 /*  1920 */    16,  415,  421,  420,  145,  414,  387,  560,  326,  314,
 /*  1930 */   164,  163,  326,  157,  158,  160,  159,  443,  326,  326,
 /*  1940 */   326,  326,  326,  344,  328,  329,  330,  331,  332,  333,
 /*  1950 */   325,  326,  326,  326,    7,  326,  326,   10,  326,  326,
 /*  1960 */   326,  428,  275,  326,  314,  326,  326,  326,  326,  326,
 /*  1970 */   326,  326,  326,  326,  326,  326,  326,  326,   23,  328,
 /*  1980 */   329,  330,  331,  332,  333,
};
static YYCODETYPE yy_lookahead[] = {
 /*     0 */    18,   78,   79,   80,   22,   82,   83,   25,   69,   70,
 /*    10 */    71,   72,   73,   74,   75,   33,   34,   35,   36,   30,
 /*    20 */    38,   39,   40,   41,   84,   43,   86,   87,   88,   47,
 /*    30 */    48,   49,   61,   62,   52,   64,   65,   66,   67,   18,
 /*    40 */    58,   59,   60,   22,  107,   63,   25,   77,   81,   79,
 /*    50 */    80,  114,   82,   83,   33,   34,   35,   36,  107,   38,
 /*    60 */    39,   40,   41,    1,   43,  114,  115,    1,   47,   48,
 /*    70 */    49,  120,   22,   52,   72,   73,   74,   75,   18,   58,
 /*    80 */    59,   60,   22,   21,   63,   25,  119,   18,  121,  122,
 /*    90 */   123,  116,  117,   33,   34,   35,   36,  107,   38,   39,
 /*   100 */    40,   41,   21,   43,  114,  115,   18,   47,   48,   49,
 /*   110 */   120,   84,   52,   86,   87,   88,   50,   18,   58,   59,
 /*   120 */    60,   22,   22,   63,   25,   25,   26,   55,   28,   29,
 /*   130 */    61,   62,   33,   34,   35,   36,   18,   38,   39,   40,
 /*   140 */    41,   84,   43,   86,   87,   88,   47,   48,   49,   22,
 /*   150 */    18,   52,   25,   26,    1,   28,   18,   58,   59,   60,
 /*   160 */    22,   22,   63,   25,   25,   26,   79,   28,   29,   82,
 /*   170 */    83,   33,   34,   35,   36,   55,   38,   39,   40,   41,
 /*   180 */    18,   43,    8,    9,   10,   47,   48,   49,   22,   18,
 /*   190 */    52,   25,   26,   18,   28,   18,   58,   59,   60,   22,
 /*   200 */    22,   63,   25,   25,   26,   30,   28,   29,   55,   55,
 /*   210 */    33,   34,   35,   36,    1,   38,   39,   40,   41,   83,
 /*   220 */    43,   86,   87,   88,   47,   48,   49,   22,   27,   52,
 /*   230 */    25,   26,   19,   28,   18,   58,   59,   60,   22,   22,
 /*   240 */    63,   25,   25,   26,    1,   28,   29,  116,  117,   33,
 /*   250 */    34,   35,   36,    1,   38,   39,   40,   41,   26,   43,
 /*   260 */    28,   29,   19,   47,   48,   49,   22,  117,   52,   25,
 /*   270 */    26,   19,   28,   18,   58,   59,   60,   22,   22,   63,
 /*   280 */    25,   25,   26,    1,   28,   29,  116,  117,   33,   34,
 /*   290 */    35,   36,    1,   38,   39,   40,   41,   26,   43,   28,
 /*   300 */    29,   19,   47,   48,   49,   22,   18,   52,   25,   26,
 /*   310 */    19,   28,   18,   58,   59,   60,   22,   22,   63,   25,
 /*   320 */    25,   26,    1,   28,   29,  116,  117,   33,   34,   35,
 /*   330 */    36,    1,   38,   39,   40,   41,   21,   43,   23,    1,
 /*   340 */    19,   47,   48,   49,   22,   81,   52,   25,   26,   19,
 /*   350 */    28,   18,   58,   59,   60,   22,   22,   63,   25,   25,
 /*   360 */    26,   81,   28,   29,   22,   55,   33,   34,   35,   36,
 /*   370 */    32,   38,   39,   40,   41,   15,   43,  107,   18,   18,
 /*   380 */    47,   48,   49,  113,  114,   52,   44,  123,   46,   18,
 /*   390 */    18,   58,   59,   60,   22,   18,   63,   25,    5,   18,
 /*   400 */    81,  121,  122,  123,   18,   33,   34,   35,   36,   27,
 /*   410 */    38,   39,   40,   41,   27,   43,  116,  117,  107,   47,
 /*   420 */    48,   49,   79,   80,   52,  114,   83,  107,   19,   18,
 /*   430 */    58,   59,   60,   22,  114,   63,   25,   81,   61,   62,
 /*   440 */   120,   54,   61,   62,   33,   34,   35,   36,   18,   38,
 /*   450 */    39,   40,   41,   19,   43,  108,  109,   22,   47,   48,
 /*   460 */    49,    1,   26,   52,   28,   26,  107,   28,   18,   58,
 /*   470 */    59,   60,   22,  114,   63,   25,   55,  104,   31,   44,
 /*   480 */   107,   46,   18,   33,   34,   35,   36,  114,   38,   39,
 /*   490 */    40,   41,   32,   43,   30,   55,  107,   47,   48,   49,
 /*   500 */    53,   54,   52,  114,   27,  107,   18,   18,   58,   59,
 /*   510 */    60,   22,  114,   63,   25,   21,  104,   23,   30,  107,
 /*   520 */   105,  106,   33,   34,   35,   36,  114,   38,   39,   40,
 /*   530 */    41,   18,   43,  107,   27,  107,   47,   48,   49,    1,
 /*   540 */   114,   52,  114,    1,   18,   55,   18,   58,   59,   60,
 /*   550 */    22,    1,   63,   25,   27,  104,   30,   55,  107,   81,
 /*   560 */   106,   33,   34,   35,   36,  114,   38,   39,   40,   41,
 /*   570 */    32,   43,  107,    1,   32,   47,   48,   49,   22,  114,
 /*   580 */    52,   81,   32,  107,   18,   18,   58,   59,   60,   22,
 /*   590 */   114,   63,   25,   53,   54,   19,   21,   27,   19,   18,
 /*   600 */    33,   34,   35,   36,   32,   38,   39,   40,   41,   19,
 /*   610 */    43,  107,   18,  107,   47,   48,   49,   27,  114,   52,
 /*   620 */   114,   19,  107,   54,   18,   58,   59,   60,   22,  114,
 /*   630 */    63,   25,   81,   81,   27,   19,  107,   42,  118,   33,
 /*   640 */    34,   35,   36,  114,   38,   39,   40,   41,   19,   43,
 /*   650 */   107,   19,   21,   47,   48,   49,   27,  114,   52,   27,
 /*   660 */    18,  107,   18,   18,   58,   59,   60,   22,  114,   63,
 /*   670 */    25,   21,   42,   81,   31,  107,   19,   81,   33,   34,
 /*   680 */    35,   36,  114,   38,   39,   40,   41,   19,   43,   18,
 /*   690 */    18,  107,   47,   48,   49,   27,   45,   52,  114,   45,
 /*   700 */   107,   19,   18,   58,   59,   60,   22,  114,   63,   25,
 /*   710 */    45,   19,   31,   19,   31,   81,   21,   33,   34,   35,
 /*   720 */    36,   45,   38,   39,   40,   41,   22,   43,   55,   31,
 /*   730 */    45,   47,   48,   49,   18,  109,   52,   50,   18,   81,
 /*   740 */    19,   18,   58,   59,   60,   22,   18,   63,   25,   18,
 /*   750 */     0,   19,   18,   53,   19,   18,   33,   34,   35,   36,
 /*   760 */    18,   38,   39,   40,   41,   18,   43,   19,   30,   18,
 /*   770 */    47,   48,   49,   21,   18,   52,   18,   22,   45,    1,
 /*   780 */    18,   58,   59,   60,   22,   21,   63,   25,   81,   20,
 /*   790 */    19,   55,   18,   21,   19,   33,   34,   35,   36,   32,
 /*   800 */    38,   39,   40,   41,  118,   43,   21,   31,   18,   47,
 /*   810 */    48,   49,   55,   27,   52,   27,   21,   30,   18,   18,
 /*   820 */    58,   59,   60,   22,   27,   63,   25,   22,   30,   18,
 /*   830 */    21,   32,   18,   31,   33,   34,   35,   36,   31,   38,
 /*   840 */    39,   40,   41,   21,   43,   21,  124,  124,   47,   48,
 /*   850 */    49,  124,  124,   52,  124,  124,  124,  124,   18,   58,
 /*   860 */    59,   60,   22,  124,   63,   25,  124,  124,  124,  124,
 /*   870 */   124,  124,  124,   33,   34,   35,   36,  124,   38,   39,
 /*   880 */    40,   41,  124,   43,  124,  124,  124,   47,   48,   49,
 /*   890 */   124,  124,   52,  124,  124,  124,  124,   18,   58,   59,
 /*   900 */    60,   22,  124,   63,   25,  124,  124,  124,  124,  124,
 /*   910 */   124,  124,   33,   34,   35,   36,  124,   38,   39,   40,
 /*   920 */    41,  124,   43,  124,  124,  124,   47,   48,   49,  124,
 /*   930 */   124,   52,  124,  124,  124,  124,   18,   58,   59,   60,
 /*   940 */    22,  124,   63,   25,  124,  124,  124,  124,  124,  124,
 /*   950 */   124,   33,   34,   35,   36,  124,   38,   39,   40,   41,
 /*   960 */   124,   43,  124,  124,  124,   47,   48,   49,  107,  124,
 /*   970 */    52,  124,   18,  112,  113,  114,   58,   59,   60,   25,
 /*   980 */   124,   63,  124,  124,  124,  124,  124,   33,   34,   35,
 /*   990 */    36,  107,   38,   39,   40,   41,  124,   43,  114,  115,
 /*  1000 */   124,   47,   48,   49,  120,  124,   52,  124,  124,  124,
 /*  1010 */   124,  124,   58,   59,   60,   75,  124,   63,   27,  124,
 /*  1020 */    76,   77,   78,   79,   80,   85,   82,   83,  124,   89,
 /*  1030 */    90,   91,   92,   93,   94,   95,   96,   97,   98,   99,
 /*  1040 */   100,  101,  102,  103,   53,   54,   75,   56,   57,  124,
 /*  1050 */    17,  124,  124,   20,  114,  124,   85,   24,   25,  124,
 /*  1060 */    89,   90,   91,   92,   93,   94,   95,   96,   97,   98,
 /*  1070 */    99,  100,  101,  102,  103,  107,  124,   75,  124,  124,
 /*  1080 */   124,  124,  114,  115,  124,  114,  124,   85,  120,  124,
 /*  1090 */   124,   89,   90,   91,   92,   93,   94,   95,   96,   97,
 /*  1100 */    98,   99,  100,  101,  102,  103,  107,   75,  124,  110,
 /*  1110 */   124,  107,  107,  114,  110,  110,  114,   85,  114,  114,
 /*  1120 */   124,   89,   90,   91,   92,   93,   94,   95,   96,   97,
 /*  1130 */    98,   99,  100,  101,  102,  103,  124,   75,  107,  124,
 /*  1140 */   124,  107,  111,  124,  110,  114,  114,   85,  114,  124,
 /*  1150 */   124,   89,   90,   91,   92,   93,   94,   95,   96,   97,
 /*  1160 */    98,   99,  100,  101,  102,  103,  107,   75,  107,  124,
 /*  1170 */   111,  110,  124,  114,  124,  114,  114,   85,  124,  124,
 /*  1180 */   124,   89,   90,   91,   92,   93,   94,   95,   96,   97,
 /*  1190 */    98,   99,  100,  101,  102,  103,  107,   75,  124,  110,
 /*  1200 */   124,  124,  124,  114,  124,  124,  114,   85,  124,  124,
 /*  1210 */   124,   89,   90,   91,   92,   93,   94,   95,   96,   97,
 /*  1220 */    98,   99,  100,  101,  102,  103,  124,   75,  124,  124,
 /*  1230 */   124,  124,  124,  124,  124,  124,  114,   85,  124,  124,
 /*  1240 */   124,   89,   90,   91,   92,   93,   94,   95,   96,   97,
 /*  1250 */    98,   99,  100,  101,  102,  103,  124,   75,  124,  124,
 /*  1260 */   124,  124,  124,  124,  124,  124,  114,   85,  124,  124,
 /*  1270 */   124,   89,   90,   91,   92,   93,   94,   95,   96,   97,
 /*  1280 */    98,   99,  100,  101,  102,  103,  124,   75,  124,  124,
 /*  1290 */   124,  124,  124,  124,  124,  124,  114,   85,  124,  124,
 /*  1300 */   124,   89,   90,   91,   92,   93,   94,   95,   96,   97,
 /*  1310 */    98,   99,  100,  101,  102,  103,  124,   75,  124,  124,
 /*  1320 */   124,  124,  124,  124,  124,  124,  114,   85,  124,  124,
 /*  1330 */   124,   89,   90,   91,   92,   93,   94,   95,   96,   97,
 /*  1340 */    98,   99,  100,  101,  102,  103,  124,   75,  124,  124,
 /*  1350 */   124,  124,  124,  124,  124,  124,  114,   85,  124,  124,
 /*  1360 */   124,   89,   90,   91,   92,   93,   94,   95,   96,   97,
 /*  1370 */    98,   99,  100,  101,  102,  103,  124,   75,  124,  124,
 /*  1380 */   124,  124,  124,  124,  124,  124,  114,   85,  124,  124,
 /*  1390 */   124,   89,   90,   91,   92,   93,   94,   95,   96,   97,
 /*  1400 */    98,   99,  100,  101,  102,  103,  124,   75,  124,  124,
 /*  1410 */   124,  124,  124,  124,  124,  124,  114,   85,  124,  124,
 /*  1420 */   124,   89,   90,   91,   92,   93,   94,   95,   96,   97,
 /*  1430 */    98,   99,  100,  101,  102,  103,  124,   75,  124,  124,
 /*  1440 */   124,  124,  124,  124,  124,  124,  114,   85,  124,  124,
 /*  1450 */   124,   89,   90,   91,   92,   93,   94,   95,   96,   97,
 /*  1460 */    98,   99,  100,  101,  102,  103,  124,   75,  124,  124,
 /*  1470 */   124,  124,  124,  124,  124,  124,  114,   85,  124,  124,
 /*  1480 */   124,   89,   90,   91,   92,   93,   94,   95,   96,   97,
 /*  1490 */    98,   99,  100,  101,  102,  103,  124,   75,  124,  124,
 /*  1500 */   124,  124,  124,  124,  124,  124,  114,   85,  124,  124,
 /*  1510 */   124,   89,   90,   91,   92,   93,   94,   95,   96,   97,
 /*  1520 */    98,   99,  100,  101,  102,  103,   18,   75,  124,  124,
 /*  1530 */   124,   76,   77,   78,   79,   80,  114,   82,   83,  124,
 /*  1540 */   124,   89,   90,   91,   92,   93,   94,   95,   96,   97,
 /*  1550 */    98,   99,  100,  101,  102,  103,   11,   12,   13,   14,
 /*  1560 */    15,   16,  124,   18,  124,  124,  114,  124,  124,   61,
 /*  1570 */    62,  124,   64,   65,   66,   67,   31,   32,  124,    2,
 /*  1580 */     3,    4,    5,    6,    7,    8,    9,   10,  124,    2,
 /*  1590 */     3,    4,    5,    6,    7,    8,    9,   10,   21,   54,
 /*  1600 */   124,  124,  124,  124,  124,  124,   61,   62,   21,   64,
 /*  1610 */    65,   66,   67,  124,  124,  124,  124,  124,   11,   12,
 /*  1620 */    13,   14,   15,   16,  124,   18,    2,    3,    4,    5,
 /*  1630 */     6,    7,    8,    9,   10,  124,  124,  124,   31,   32,
 /*  1640 */   124,    2,    3,    4,    5,    6,    7,    8,    9,   10,
 /*  1650 */   124,   76,   77,   78,   79,   80,   32,   82,   83,  124,
 /*  1660 */    21,   54,  124,  124,  124,  124,  124,  124,   61,   62,
 /*  1670 */   124,   64,   65,   66,   67,  124,  124,  124,  124,  124,
 /*  1680 */    11,   12,   13,   14,   15,   16,  124,   18,    2,    3,
 /*  1690 */     4,    5,    6,    7,    8,    9,   10,  124,  124,  124,
 /*  1700 */    31,   32,  124,  124,  124,   19,  124,  124,  124,  124,
 /*  1710 */   124,  124,   11,   12,   13,   14,   15,   16,  124,   18,
 /*  1720 */   124,  124,  124,   54,  124,  124,  124,  124,  124,  124,
 /*  1730 */    61,   62,   31,   64,   65,   66,   67,  124,  124,  124,
 /*  1740 */     4,  124,   18,  124,   11,   12,   13,   14,   15,   16,
 /*  1750 */   124,   18,  124,  124,   18,   54,   55,   21,  124,  124,
 /*  1760 */   124,  124,   61,   62,   31,   64,   65,   66,   67,   33,
 /*  1770 */    34,   35,   36,   37,   38,  124,   11,   12,   13,   14,
 /*  1780 */    15,   16,  124,   18,   51,   61,   62,   54,   64,   65,
 /*  1790 */    66,   67,  124,  124,   61,   62,   31,   64,   65,   66,
 /*  1800 */    67,  124,  124,  124,    4,  124,  124,  124,   11,   12,
 /*  1810 */    13,   14,   15,   16,  124,   18,   51,  124,   18,   54,
 /*  1820 */   124,  124,  124,  124,  124,  124,   61,   62,   31,   64,
 /*  1830 */    65,   66,   67,   33,   34,   35,   36,   37,   38,  124,
 /*  1840 */    11,   12,   13,   14,   15,   16,  124,   18,  124,  124,
 /*  1850 */   124,   54,   55,  124,  124,  124,  124,  124,   61,   62,
 /*  1860 */    31,   64,   65,   66,   67,    2,    3,    4,    5,    6,
 /*  1870 */     7,    8,    9,   10,    2,    3,    4,    5,    6,    7,
 /*  1880 */     8,    9,   10,   54,   21,  124,  124,   18,  124,  124,
 /*  1890 */    61,   62,  124,   64,   65,   66,   67,    2,    3,    4,
 /*  1900 */     5,    6,    7,    8,    9,   10,    2,    3,    4,    5,
 /*  1910 */     6,    7,    8,    9,   10,    4,   21,  124,  124,   76,
 /*  1920 */    77,   78,   79,   80,   55,   82,   83,   55,  124,   18,
 /*  1930 */    61,   62,  124,   64,   65,   66,   67,    0,  124,  124,
 /*  1940 */   124,  124,  124,   32,   33,   34,   35,   36,   37,   38,
 /*  1950 */     4,  124,  124,  124,   17,  124,  124,   20,  124,  124,
 /*  1960 */   124,   24,   25,  124,   18,  124,  124,  124,  124,  124,
 /*  1970 */   124,  124,  124,  124,  124,  124,  124,  124,   32,   33,
 /*  1980 */    34,   35,   36,   37,   38,
};
#define YY_SHIFT_USE_DFLT (-30)
static short yy_shift_ofst[] = {
 /*     0 */  1033,  750, 1937,  -30,  -30,  -30,  -30,  758,  576,  -30,
 /*    10 */   742,  315,  256,  556,  -30,  -30,  139,  244,  -30,  738,
 /*    20 */   728,  683, 1946, 1736,  138,  -30,  567,  -30,  -30,  -30,
 /*    30 */   -30,  -30,  -30,  -30,  -30,  -30,  -30,  -30,  -30,  -30,
 /*    40 */   -30,  -30,  -30,  -30,  775,  -30,  735,  -30, 1829,  695,
 /*    50 */   840,  918,  595,  575,  528,  372,  -30, 1829,  824,  342,
 /*    60 */   -30,  435,  -30,  -30,  -29,  654,  954,  954,  764,  801,
 /*    70 */   879,  -30, 1829,  772,  606,  411,  -30,  644,   66, 1733,
 /*    80 */  1577,  954,  489,  -30,  720,  321,  -30,  720,  -30,  991,
 /*    90 */  1829,  -30, 1904, 1829, 1829,  -30, 1829,  642,  -30,  569,
 /*   100 */   381,  502,  -30,  310,  -30,  120,  -30,  174, 1829,  174,
 /*   110 */  1829,  174, 1829,  174, 1829,  174, 1829,  -30, 1829,  -30,
 /*   120 */  1829,  -30, 1829, 1624,  -30,  447,  672,  681, 1607, 1904,
 /*   130 */  1829,  -30,  756,  778,  642,  -30, 1829,  -30,  -30,  -30,
 /*   140 */   -30,  -30,  -30,  -30, 1869,  -30,  153,  -30, 1508,  -30,
 /*   150 */   733, 1724,  -30,  -30,  -30,  -30,  -30,  -30,  -30,  -30,
 /*   160 */   -30,  -30,  685,  676,  665,  -30,  671,  643, 1669,  -30,
 /*   170 */   460,  -30, 1829,  -30,  -30,  -30,  538,  -30,  -30,  377,
 /*   180 */   421,  -30,  154,  -30,   72,  -30, 1545,  550,  -30,  -30,
 /*   190 */   174,  800,  387, 1829,  -30, 1797,  786, 1829,  -30,  757,
 /*   200 */   797, 1829,  -30, 1872, 1701,  201, 1829,  -30,  440,  477,
 /*   210 */  1829,  -30,  -30,  -30,  -30, 1829,  330,  -30, 1829,  -30,
 /*   220 */  1904,  -30,  692,  -30, 1829, 1686,  -30,  731,  540,  747,
 /*   230 */   748,  -30,   69,  736,  771,  -30,  490,  579,  -30,  673,
 /*   240 */   732,  -30,  360,  616,  -30,  734,  698, 1829,  338,  682,
 /*   250 */   -30,  566,  243,  -30,  566,  -30,  788,  -29,  -30,  -30,
 /*   260 */   566,  252,  -30,  566,  282,  -30,  566,  213,  -30,  566,
 /*   270 */   291,  -30,  700,  737,  681,  -30,  -30, 1829, 1895,  954,
 /*   280 */   645,  -30,  716,  687, 1765, 1639,  954,  723,  -30, 1829,
 /*   290 */  1863,  954,  762,  -30,  -30, 1587,  -30,  651,  954,  954,
 /*   300 */   -30,  -30,  630,  650,   21,  -18,  -30,  -30,   62,  954,
 /*   310 */   684,  -30, 1800,  -30,  607,  -29,  -30,  581,  570,  -29,
 /*   320 */   -30,  513,  527,  -29,  -30,  386,  393,  361,  288,  171,
 /*   330 */   162,  132,  118,   88,  -30,  542,   81,   50,  -30,  436,
 /*   340 */   -11,  814,  807, 1911,  809,   60,  -30,   99,  -30,  572,
 /*   350 */   795,  177,  -30,  255,  -30,  787,  790,  776,  767,  785,
 /*   360 */   333,  -30,  294,  -30,  798,  811,  802,  799,  822,  450,
 /*   370 */   -30,  216,  -30,  217,  322,  -30,  232,  371,  382,  -29,
 /*   380 */   409,  -30,  430,  507,  -29,  434,  -30,  -30,  -30,  271,
 /*   390 */   175,  590,  -30,  -29,  602,  -30,  464,  629,  -30,  -29,
 /*   400 */   657,  -30,  488,  632,  -30,  -29,  694,  -30,  526,  668,
 /*   410 */   -30,  -29,  721,  -30,  -30,  334,  127,  166,  439,  205,
 /*   420 */   -30,  283,  751,  752,  295,  755,  -30,  -30,  769,  774,
 /*   430 */   494,  178,  805,  -30,  -30,  594,  631,  100,  704,  -30,
 /*   440 */   -30,  -30,
};
#define YY_REDUCE_USE_DFLT (-78)
static short yy_reduce_ofst[] = {
 /*     0 */   -61,  -78,    2,  -78,  -78,  -78,  -78,  -78,  -78,  -78,
 /*    10 */   -78,  -78,  944,  -78,  -78,  -78,  -77,  136,  -78,  -78,
 /*    20 */   -78,  -78,  -60,   57, 1062,  -78, 1452,  -78,  -78,  -78,
 /*    30 */   -78,  -78,  -78,  -78,  -78,  -78,  -78,  -78,  -78,  -78,
 /*    40 */   -78,  -78,  -78,  -78,  -78,  -78,  -78,  -78,  373,  -78,
 /*    50 */  1302, 1452,  -78,  -78, 1092, 1452,  -78,  412,  -78,  415,
 /*    60 */   -78,  454,  -78,  -78,  592,  -78, 1362, 1452,  -78, 1182,
 /*    70 */  1452,  -78,  451,  -78, 1392, 1452,  -78,  -78,  -78,  -63,
 /*    80 */   -78, 1152, 1452,  -78,  347,  -78,  -78,  626,  -78,  -78,
 /*    90 */  1005,  -78,  -78,  398,  426,  -78,  428,  520,  -78,  -78,
 /*   100 */   -78,  -78,  -78,  -78,  -78,  -78,  -78,  -78,  465,  -78,
 /*   110 */   476,  -78,  504,  -78,  506,  -78,  515,  -78,  529,  -78,
 /*   120 */   543,  -78,  554,  -78,  -78,  -78,  -78,  -78,  -10,  -78,
 /*   130 */   584,  -78,  -78,  -78,  686,  -78,  593,  -78,  -78,  -78,
 /*   140 */   -78,  -78,  -78,  -78,  -33,  -78,  -78,  -78,  280,  -78,
 /*   150 */   -78,  264,  -78,  -78,  -78,  -78,  -78,  -78,  -78,  -78,
 /*   160 */   -78,  -78,  -78,  -78,  -78,  -78,  -78,  -78,  -49,  -78,
 /*   170 */   -78,  -78,  320,  -78,  -78,  -78,  -78,  -78,  -78,  -78,
 /*   180 */   -78,  -78,  -78,  -78,  -78,  -78,  968,  -78,  -78,  -78,
 /*   190 */   -78,  -78,  -78, 1061,  -78, 1059,  -78, 1004,  -78,  -78,
 /*   200 */   -78, 1034,  -78,  -78, 1031,  -78,  999,  -78,  -78,  -78,
 /*   210 */  1089,  -78,  -78,  -78,  -78,  861,  -78,  -78,  270,  -78,
 /*   220 */   -78,  -78,  -78,  -78,  568,  -78,  -78,  -78,  -78,  -78,
 /*   230 */   -78,  -78,  -78,  -78,  -78,  -78,  -78,  -78,  -78,  -78,
 /*   240 */   -78,  -78,  -78,  -78,  -78,  -78,  -78,  884,  -78,  -78,
 /*   250 */   -78,  -25,  -78,  -78,  150,  -78,  -78,  707,  -78,  -78,
 /*   260 */   131,  -78,  -78,  170,  -78,  -78,  209,  -78,  -78,  300,
 /*   270 */   -78,  -78,  -78,  -78,  -78,  -78,  -78,  311,  -78, 1332,
 /*   280 */  1452,  -78,  -78,  -78,  359,  -78, 1422, 1452,  -78,  389,
 /*   290 */   -78, 1212, 1452,  -78,  -78,  -78,  -78,  -78,  940, 1452,
 /*   300 */   -78,  -78,  -78,  -78, 1272, 1452,  -78,  -78,  -78, 1002,
 /*   310 */  1452,  -78,  135,  -78,  -78,  551,  -78,  -78,  -78,  500,
 /*   320 */   -78,  -78,  -78,  356,  -78,  -78,  -78,  -78,  -78,  -78,
 /*   330 */   -78,  -78,  -78,  -78,  -78,  -78,  -78,  -78,  -78,  -78,
 /*   340 */   -78,  -78,  -78,   27,  -78,  971,  -78, 1452,  -78,  -78,
 /*   350 */   -78, 1122,  -78, 1452,  -78,  -78,  -78,  -78,  -78,  -78,
 /*   360 */  1032,  -78, 1452,  -78,  -78,  -78,  -78,  -78,  -78, 1242,
 /*   370 */   -78, 1452,  -78,   87,  136,  -78,  -78,  -78,  -78,  319,
 /*   380 */   -78,  -78,  -78,  -78,  478,  -78,  -78,  -78,  -78,  -78,
 /*   390 */   -78,  -78,  -78,  552,  -78,  -78,  -78,  -78,  -78,  596,
 /*   400 */   -78,  -78,  -78,  -78,  -78,  634,  -78,  -78,  -78,  -78,
 /*   410 */   -78,  658,  -78,  -78,  -78,  -30,  343,  136,  -78,  136,
 /*   420 */   -78,  136,  -78,  -78, 1455,  -78,  -78,  -78,  -78,  -78,
 /*   430 */   -78, 1843,  -78,  -78,  -78,  -78,  -78, 1575,  -78,  -78,
 /*   440 */   -78,  -78,
};
static YYACTIONTYPE yy_default[] = {
 /*     0 */   642,  642,  642,  444,  446,  447,  448,  642,  642,  449,
 /*    10 */   642,  642,  642,  642,  450,  454,  642,  642,  479,  642,
 /*    20 */   642,  642,  642,  642,  642,  483,  642,  487,  510,  512,
 /*    30 */   513,  514,  515,  516,  517,  518,  519,  520,  521,  522,
 /*    40 */   523,  524,  525,  526,  642,  527,  642,  528,  642,  642,
 /*    50 */   642,  642,  531,  642,  642,  642,  532,  642,  642,  642,
 /*    60 */   535,  642,  536,  537,  642,  642,  642,  539,  642,  642,
 /*    70 */   642,  542,  642,  642,  642,  642,  544,  642,  642,  642,
 /*    80 */   642,  642,  642,  545,  642,  642,  549,  642,  550,  642,
 /*    90 */   642,  552,  582,  642,  642,  583,  642,  642,  584,  642,
 /*   100 */   642,  642,  637,  642,  638,  642,  639,  586,  642,  587,
 /*   110 */   642,  588,  642,  589,  642,  590,  642,  596,  642,  597,
 /*   120 */   642,  598,  642,  642,  591,  602,  642,  592,  642,  621,
 /*   130 */   642,  599,  642,  642,  642,  600,  642,  601,  603,  604,
 /*   140 */   605,  606,  607,  608,  642,  609,  642,  610,  642,  622,
 /*   150 */   642,  642,  624,  629,  630,  631,  632,  633,  634,  635,
 /*   160 */   636,  625,  630,  632,  631,  623,  642,  611,  642,  612,
 /*   170 */   642,  613,  642,  616,  619,  620,  642,  617,  618,  642,
 /*   180 */   642,  593,  642,  594,  642,  595,  642,  642,  614,  615,
 /*   190 */   585,  642,  642,  642,  553,  642,  642,  642,  554,  642,
 /*   200 */   642,  642,  555,  642,  642,  642,  642,  556,  642,  642,
 /*   210 */   642,  557,  558,  559,  551,  642,  642,  561,  642,  562,
 /*   220 */   564,  563,  642,  565,  642,  642,  566,  642,  642,  642,
 /*   230 */   642,  567,  642,  642,  642,  568,  642,  642,  569,  642,
 /*   240 */   642,  570,  642,  642,  571,  642,  642,  642,  642,  642,
 /*   250 */   572,  642,  642,  573,  642,  578,  580,  642,  581,  579,
 /*   260 */   642,  642,  574,  642,  642,  575,  642,  642,  576,  642,
 /*   270 */   642,  577,  642,  642,  642,  641,  511,  642,  642,  642,
 /*   280 */   642,  546,  642,  642,  642,  642,  642,  642,  547,  642,
 /*   290 */   642,  642,  642,  548,  543,  642,  541,  642,  642,  540,
 /*   300 */   538,  533,  529,  642,  642,  642,  534,  530,  642,  642,
 /*   310 */   642,  488,  642,  495,  497,  642,  500,  642,  498,  642,
 /*   320 */   501,  642,  499,  642,  502,  642,  642,  642,  642,  642,
 /*   330 */   642,  642,  642,  642,  496,  642,  642,  642,  484,  642,
 /*   340 */   642,  642,  642,  642,  642,  642,  485,  642,  489,  642,
 /*   350 */   642,  642,  486,  642,  490,  642,  642,  642,  642,  642,
 /*   360 */   642,  492,  642,  494,  642,  642,  642,  642,  642,  642,
 /*   370 */   491,  642,  493,  642,  642,  477,  642,  642,  642,  642,
 /*   380 */   642,  480,  642,  642,  642,  642,  481,  482,  467,  642,
 /*   390 */   642,  642,  469,  642,  642,  472,  642,  642,  474,  642,
 /*   400 */   642,  476,  642,  642,  470,  642,  642,  471,  642,  642,
 /*   410 */   473,  642,  642,  475,  478,  642,  642,  642,  642,  642,
 /*   420 */   468,  642,  642,  642,  642,  642,  451,  455,  642,  642,
 /*   430 */   642,  642,  642,  452,  456,  642,  642,  642,  642,  453,
 /*   440 */   457,  445,
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
 /*  38 */ "xx_class_const_definition ::= COMMENT CONST IDENTIFIER ASSIGN xx_literal_expr DOTCOMMA",
 /*  39 */ "xx_class_const_definition ::= CONST IDENTIFIER ASSIGN xx_literal_expr DOTCOMMA",
 /*  40 */ "xx_class_methods_definition ::= xx_class_method_definition",
 /*  41 */ "xx_class_method_definition ::= PUBLIC FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  42 */ "xx_class_method_definition ::= PUBLIC FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  43 */ "xx_class_method_definition ::= COMMENT PUBLIC FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  44 */ "xx_class_method_definition ::= COMMENT PUBLIC FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  45 */ "xx_class_method_definition ::= PUBLIC FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  46 */ "xx_class_method_definition ::= PUBLIC FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE xx_parameter_list BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  47 */ "xx_class_method_definition ::= COMMENT PUBLIC FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  48 */ "xx_class_method_definition ::= COMMENT PUBLIC FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  49 */ "xx_class_method_definition ::= PROTECTED FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  50 */ "xx_class_method_definition ::= COMMENT PROTECTED FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  51 */ "xx_class_method_definition ::= PROTECTED FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  52 */ "xx_class_method_definition ::= COMMENT PROTECTED FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  53 */ "xx_parameter_list ::= xx_parameter_list COMMA xx_parameter",
 /*  54 */ "xx_parameter_list ::= xx_parameter",
 /*  55 */ "xx_parameter ::= IDENTIFIER",
 /*  56 */ "xx_parameter ::= xx_parameter_type IDENTIFIER",
 /*  57 */ "xx_parameter ::= xx_parameter_cast IDENTIFIER",
 /*  58 */ "xx_parameter ::= IDENTIFIER ASSIGN xx_literal_expr",
 /*  59 */ "xx_parameter ::= xx_parameter_type IDENTIFIER ASSIGN xx_literal_expr",
 /*  60 */ "xx_parameter ::= xx_parameter_cast IDENTIFIER ASSIGN xx_literal_expr",
 /*  61 */ "xx_parameter_cast ::= LESS IDENTIFIER GREATER",
 /*  62 */ "xx_parameter_type ::= TYPE_INTEGER",
 /*  63 */ "xx_parameter_type ::= TYPE_DOUBLE",
 /*  64 */ "xx_parameter_type ::= TYPE_BOOL",
 /*  65 */ "xx_parameter_type ::= TYPE_STRING",
 /*  66 */ "xx_parameter_type ::= TYPE_ARRAY",
 /*  67 */ "xx_parameter_type ::= TYPE_VAR",
 /*  68 */ "xx_statement_list ::= xx_statement_list xx_statement",
 /*  69 */ "xx_statement_list ::= xx_statement",
 /*  70 */ "xx_statement ::= xx_let_statement",
 /*  71 */ "xx_statement ::= xx_if_statement",
 /*  72 */ "xx_statement ::= xx_loop_statement",
 /*  73 */ "xx_statement ::= xx_echo_statement",
 /*  74 */ "xx_statement ::= xx_return_statement",
 /*  75 */ "xx_statement ::= xx_mcall_statement",
 /*  76 */ "xx_statement ::= xx_unset_statement",
 /*  77 */ "xx_statement ::= xx_throw_statement",
 /*  78 */ "xx_statement ::= xx_declare_statement",
 /*  79 */ "xx_statement ::= xx_break_statement",
 /*  80 */ "xx_statement ::= xx_continue_statement",
 /*  81 */ "xx_statement ::= xx_while_statement",
 /*  82 */ "xx_statement ::= xx_switch_statement",
 /*  83 */ "xx_statement ::= xx_for_statement",
 /*  84 */ "xx_statement ::= xx_comment",
 /*  85 */ "xx_break_statement ::= BREAK DOTCOMMA",
 /*  86 */ "xx_continue_statement ::= CONTINUE DOTCOMMA",
 /*  87 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /*  88 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE ELSE BRACKET_OPEN BRACKET_CLOSE",
 /*  89 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  90 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE ELSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  91 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE ELSE BRACKET_OPEN BRACKET_CLOSE",
 /*  92 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE ELSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  93 */ "xx_switch_statement ::= SWITCH xx_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /*  94 */ "xx_switch_statement ::= SWITCH xx_eval_expr BRACKET_OPEN xx_case_clauses BRACKET_CLOSE",
 /*  95 */ "xx_case_clauses ::= xx_case_clauses xx_case_clause",
 /*  96 */ "xx_case_clauses ::= xx_case_clause",
 /*  97 */ "xx_case_clause ::= CASE xx_literal_expr COLON xx_statement_list",
 /*  98 */ "xx_case_clause ::= DEFAULT COLON xx_statement_list",
 /*  99 */ "xx_loop_statement ::= LOOP BRACKET_OPEN BRACKET_CLOSE",
 /* 100 */ "xx_loop_statement ::= LOOP BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 101 */ "xx_while_statement ::= WHILE xx_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /* 102 */ "xx_while_statement ::= WHILE xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 103 */ "xx_for_statement ::= FOR IDENTIFIER IN xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 104 */ "xx_for_statement ::= FOR IDENTIFIER IN REVERSE xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 105 */ "xx_for_statement ::= FOR IDENTIFIER COMMA IDENTIFIER IN xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 106 */ "xx_for_statement ::= FOR IDENTIFIER COMMA IDENTIFIER IN REVERSE xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 107 */ "xx_let_statement ::= LET xx_let_assignments DOTCOMMA",
 /* 108 */ "xx_let_assignments ::= xx_let_assignments COMMA xx_let_assignment",
 /* 109 */ "xx_let_assignments ::= xx_let_assignment",
 /* 110 */ "xx_let_assignment ::= IDENTIFIER ASSIGN xx_assign_expr",
 /* 111 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER ASSIGN xx_assign_expr",
 /* 112 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE ASSIGN xx_assign_expr",
 /* 113 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER SBRACKET_OPEN xx_index_expr SBRACKET_CLOSE ASSIGN xx_assign_expr",
 /* 114 */ "xx_let_assignment ::= IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE ASSIGN xx_assign_expr",
 /* 115 */ "xx_let_assignment ::= IDENTIFIER SBRACKET_OPEN xx_index_expr SBRACKET_CLOSE ASSIGN xx_assign_expr",
 /* 116 */ "xx_let_assignment ::= IDENTIFIER INCR",
 /* 117 */ "xx_let_assignment ::= IDENTIFIER DECR",
 /* 118 */ "xx_index_expr ::= xx_common_expr",
 /* 119 */ "xx_echo_statement ::= ECHO xx_echo_expressions DOTCOMMA",
 /* 120 */ "xx_echo_expressions ::= xx_echo_expressions COMMA xx_echo_expression",
 /* 121 */ "xx_echo_expressions ::= xx_echo_expression",
 /* 122 */ "xx_echo_expression ::= xx_common_expr",
 /* 123 */ "xx_mcall_statement ::= xx_mcall_expr DOTCOMMA",
 /* 124 */ "xx_return_statement ::= RETURN xx_common_expr DOTCOMMA",
 /* 125 */ "xx_unset_statement ::= UNSET IDENTIFIER ARROW IDENTIFIER DOTCOMMA",
 /* 126 */ "xx_unset_statement ::= UNSET IDENTIFIER SBRACKET_OPEN IDENTIFIER SBRACKET_CLOSE DOTCOMMA",
 /* 127 */ "xx_unset_statement ::= UNSET IDENTIFIER SBRACKET_OPEN INTEGER SBRACKET_CLOSE DOTCOMMA",
 /* 128 */ "xx_unset_statement ::= UNSET IDENTIFIER SBRACKET_OPEN STRING SBRACKET_CLOSE DOTCOMMA",
 /* 129 */ "xx_throw_statement ::= THROW IDENTIFIER DOTCOMMA",
 /* 130 */ "xx_throw_statement ::= THROW NEW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE DOTCOMMA",
 /* 131 */ "xx_declare_statement ::= TYPE_INTEGER xx_declare_variable_list DOTCOMMA",
 /* 132 */ "xx_declare_statement ::= TYPE_DOUBLE xx_declare_variable_list DOTCOMMA",
 /* 133 */ "xx_declare_statement ::= TYPE_STRING xx_declare_variable_list DOTCOMMA",
 /* 134 */ "xx_declare_statement ::= TYPE_BOOL xx_declare_variable_list DOTCOMMA",
 /* 135 */ "xx_declare_statement ::= TYPE_VAR xx_declare_variable_list DOTCOMMA",
 /* 136 */ "xx_declare_variable_list ::= xx_declare_variable_list COMMA xx_declare_variable",
 /* 137 */ "xx_declare_variable_list ::= xx_declare_variable",
 /* 138 */ "xx_declare_variable ::= IDENTIFIER",
 /* 139 */ "xx_declare_variable ::= IDENTIFIER ASSIGN xx_literal_expr",
 /* 140 */ "xx_assign_expr ::= xx_common_expr",
 /* 141 */ "xx_common_expr ::= NOT xx_common_expr",
 /* 142 */ "xx_common_expr ::= ISSET xx_isset_expr",
 /* 143 */ "xx_common_expr ::= xx_common_expr EQUALS xx_common_expr",
 /* 144 */ "xx_common_expr ::= xx_common_expr NOTEQUALS xx_common_expr",
 /* 145 */ "xx_common_expr ::= xx_common_expr IDENTICAL xx_common_expr",
 /* 146 */ "xx_common_expr ::= xx_common_expr NOTIDENTICAL xx_common_expr",
 /* 147 */ "xx_common_expr ::= xx_common_expr LESS xx_common_expr",
 /* 148 */ "xx_common_expr ::= xx_common_expr GREATER xx_common_expr",
 /* 149 */ "xx_common_expr ::= PARENTHESES_OPEN xx_common_expr PARENTHESES_CLOSE",
 /* 150 */ "xx_common_expr ::= IDENTIFIER ARROW IDENTIFIER",
 /* 151 */ "xx_common_expr ::= IDENTIFIER SBRACKET_OPEN IDENTIFIER SBRACKET_CLOSE",
 /* 152 */ "xx_common_expr ::= IDENTIFIER SBRACKET_OPEN INTEGER SBRACKET_CLOSE",
 /* 153 */ "xx_common_expr ::= IDENTIFIER SBRACKET_OPEN STRING SBRACKET_CLOSE",
 /* 154 */ "xx_common_expr ::= xx_common_expr ADD xx_common_expr",
 /* 155 */ "xx_common_expr ::= xx_common_expr SUB xx_common_expr",
 /* 156 */ "xx_common_expr ::= xx_common_expr CONCAT xx_common_expr",
 /* 157 */ "xx_common_expr ::= INSTANCEOF xx_common_expr",
 /* 158 */ "xx_common_expr ::= FETCH IDENTIFIER COMMA xx_isset_expr",
 /* 159 */ "xx_common_expr ::= TYPEOF xx_common_expr",
 /* 160 */ "xx_common_expr ::= IDENTIFIER",
 /* 161 */ "xx_common_expr ::= INTEGER",
 /* 162 */ "xx_common_expr ::= STRING",
 /* 163 */ "xx_common_expr ::= DOUBLE",
 /* 164 */ "xx_common_expr ::= NULL",
 /* 165 */ "xx_common_expr ::= TRUE",
 /* 166 */ "xx_common_expr ::= FALSE",
 /* 167 */ "xx_common_expr ::= SBRACKET_OPEN SBRACKET_CLOSE",
 /* 168 */ "xx_common_expr ::= SBRACKET_OPEN xx_array_list SBRACKET_CLOSE",
 /* 169 */ "xx_common_expr ::= NEW IDENTIFIER",
 /* 170 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 171 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 172 */ "xx_common_expr ::= IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 173 */ "xx_common_expr ::= IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 174 */ "xx_common_expr ::= xx_mcall_expr",
 /* 175 */ "xx_mcall_expr ::= IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 176 */ "xx_mcall_expr ::= IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 177 */ "xx_call_parameters ::= xx_call_parameters COMMA xx_call_parameter",
 /* 178 */ "xx_call_parameters ::= xx_call_parameter",
 /* 179 */ "xx_call_parameter ::= xx_common_expr",
 /* 180 */ "xx_array_list ::= xx_array_list COMMA xx_array_item",
 /* 181 */ "xx_array_list ::= xx_array_item",
 /* 182 */ "xx_array_item ::= xx_array_key COLON xx_array_value",
 /* 183 */ "xx_array_item ::= xx_array_value",
 /* 184 */ "xx_array_key ::= IDENTIFIER",
 /* 185 */ "xx_array_key ::= STRING",
 /* 186 */ "xx_array_key ::= INTEGER",
 /* 187 */ "xx_array_value ::= xx_literal_expr",
 /* 188 */ "xx_array_value ::= IDENTIFIER",
 /* 189 */ "xx_literal_expr ::= INTEGER",
 /* 190 */ "xx_literal_expr ::= STRING",
 /* 191 */ "xx_literal_expr ::= DOUBLE",
 /* 192 */ "xx_literal_expr ::= NULL",
 /* 193 */ "xx_literal_expr ::= FALSE",
 /* 194 */ "xx_literal_expr ::= TRUE",
 /* 195 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN IDENTIFIER SBRACKET_CLOSE",
 /* 196 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN INTEGER SBRACKET_CLOSE",
 /* 197 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN STRING SBRACKET_CLOSE",
 /* 198 */ "xx_eval_expr ::= xx_common_expr",
 /* 199 */ "xx_comment ::= COMMENT",
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
// 1812 "parser.c"
      break;
    case 70:
// 773 "parser.lemon"
{ json_object_put((yypminor->yy175)); }
// 1817 "parser.c"
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
  { 82, 6 },
  { 82, 5 },
  { 79, 1 },
  { 83, 7 },
  { 83, 8 },
  { 83, 8 },
  { 83, 9 },
  { 83, 8 },
  { 83, 9 },
  { 83, 9 },
  { 83, 10 },
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
// 2234 "parser.c"
        break;
      case 1:
      case 4:
      case 5:
      case 6:
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
      case 118:
      case 140:
      case 174:
      case 179:
      case 187:
      case 198:
// 775 "parser.lemon"
{
	yygotominor.yy175 = yymsp[0].minor.yy175;
}
// 2265 "parser.c"
        break;
      case 2:
      case 25:
      case 35:
      case 37:
      case 68:
      case 95:
// 779 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_list(yymsp[-1].minor.yy175, yymsp[0].minor.yy175);
}
// 2277 "parser.c"
        break;
      case 3:
      case 26:
      case 36:
      case 40:
      case 54:
      case 69:
      case 96:
      case 109:
      case 121:
      case 137:
      case 178:
      case 181:
// 783 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_list(NULL, yymsp[0].minor.yy175);
}
// 2295 "parser.c"
        break;
      case 7:
// 799 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_namespace(yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(17,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2304 "parser.c"
        break;
      case 8:
// 803 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, 0, NULL, status->scanner_state);
  yy_destructor(20,&yymsp[-4].minor);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2314 "parser.c"
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
// 2325 "parser.c"
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
// 2336 "parser.c"
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
// 2348 "parser.c"
        break;
      case 12:
// 819 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-2].minor.yy0, NULL, 0, NULL, status->scanner_state);
  yy_destructor(20,&yymsp[-3].minor);
  yy_destructor(21,&yymsp[-1].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2358 "parser.c"
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
// 2369 "parser.c"
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
// 2380 "parser.c"
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
// 2392 "parser.c"
        break;
      case 16:
// 835 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(yymsp[0].minor.yy175, NULL, NULL, status->scanner_state);
}
// 2399 "parser.c"
        break;
      case 17:
// 839 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(NULL, NULL, yymsp[0].minor.yy175, status->scanner_state);
}
// 2406 "parser.c"
        break;
      case 18:
// 843 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(NULL, yymsp[0].minor.yy175, NULL, status->scanner_state);
}
// 2413 "parser.c"
        break;
      case 19:
// 847 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(yymsp[-1].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
}
// 2420 "parser.c"
        break;
      case 20:
// 851 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(yymsp[-1].minor.yy175, NULL, yymsp[0].minor.yy175, status->scanner_state);
}
// 2427 "parser.c"
        break;
      case 21:
// 855 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(yymsp[0].minor.yy175, NULL, yymsp[-1].minor.yy175, status->scanner_state);
}
// 2434 "parser.c"
        break;
      case 22:
// 859 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(NULL, yymsp[0].minor.yy175, yymsp[-1].minor.yy175, status->scanner_state);
}
// 2441 "parser.c"
        break;
      case 23:
// 863 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(yymsp[-2].minor.yy175, yymsp[0].minor.yy175, yymsp[-1].minor.yy175, status->scanner_state);
}
// 2448 "parser.c"
        break;
      case 24:
// 867 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(yymsp[-1].minor.yy175, yymsp[0].minor.yy175, yymsp[-2].minor.yy175, status->scanner_state);
}
// 2455 "parser.c"
        break;
      case 27:
// 879 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_property(XX_T_PUBLIC, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(26,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2464 "parser.c"
        break;
      case 28:
// 883 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_property(XX_T_PUBLIC, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(26,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2473 "parser.c"
        break;
      case 29:
// 887 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_property(XX_T_PUBLIC, yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, NULL, status->scanner_state);
  yy_destructor(26,&yymsp[-4].minor);
  yy_destructor(27,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2483 "parser.c"
        break;
      case 30:
// 891 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_property(XX_T_PUBLIC, yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(26,&yymsp[-4].minor);
  yy_destructor(27,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2493 "parser.c"
        break;
      case 31:
// 895 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_property(XX_T_PROTECTED, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2502 "parser.c"
        break;
      case 32:
// 899 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_property(XX_T_PROTECTED, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(28,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2511 "parser.c"
        break;
      case 33:
// 903 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_property(XX_T_PROTECTED, yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-4].minor);
  yy_destructor(27,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2521 "parser.c"
        break;
      case 34:
// 907 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_property(XX_T_PROTECTED, yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(28,&yymsp[-4].minor);
  yy_destructor(27,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2531 "parser.c"
        break;
      case 38:
// 923 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(29,&yymsp[-4].minor);
  yy_destructor(27,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2541 "parser.c"
        break;
      case 39:
// 927 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, NULL, status->scanner_state);
  yy_destructor(29,&yymsp[-4].minor);
  yy_destructor(27,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2551 "parser.c"
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
// 2564 "parser.c"
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
// 2577 "parser.c"
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
// 2590 "parser.c"
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
// 2603 "parser.c"
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
// 2616 "parser.c"
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
// 2629 "parser.c"
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
// 2642 "parser.c"
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
// 2655 "parser.c"
        break;
      case 49:
// 967 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(XX_T_PROTECTED, yymsp[-4].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-6].minor);
  yy_destructor(30,&yymsp[-5].minor);
  yy_destructor(31,&yymsp[-3].minor);
  yy_destructor(32,&yymsp[-2].minor);
  yy_destructor(21,&yymsp[-1].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2668 "parser.c"
        break;
      case 50:
// 971 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(XX_T_PROTECTED, yymsp[-4].minor.yy0, NULL, NULL, yymsp[-7].minor.yy0, status->scanner_state);
  yy_destructor(28,&yymsp[-6].minor);
  yy_destructor(30,&yymsp[-5].minor);
  yy_destructor(31,&yymsp[-3].minor);
  yy_destructor(32,&yymsp[-2].minor);
  yy_destructor(21,&yymsp[-1].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2681 "parser.c"
        break;
      case 51:
// 975 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(XX_T_PROTECTED, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy175, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-7].minor);
  yy_destructor(30,&yymsp[-6].minor);
  yy_destructor(31,&yymsp[-4].minor);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2694 "parser.c"
        break;
      case 52:
// 979 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(XX_T_PROTECTED, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy175, yymsp[-8].minor.yy0, status->scanner_state);
  yy_destructor(28,&yymsp[-7].minor);
  yy_destructor(30,&yymsp[-6].minor);
  yy_destructor(31,&yymsp[-4].minor);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2707 "parser.c"
        break;
      case 53:
      case 108:
      case 120:
      case 136:
      case 177:
      case 180:
// 983 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_list(yymsp[-2].minor.yy175, yymsp[0].minor.yy175);
  yy_destructor(1,&yymsp[-1].minor);
}
// 2720 "parser.c"
        break;
      case 55:
// 991 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_parameter(NULL, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 2727 "parser.c"
        break;
      case 56:
// 995 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_parameter(yymsp[-1].minor.yy175, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 2734 "parser.c"
        break;
      case 57:
// 999 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_parameter(NULL, yymsp[-1].minor.yy175, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 2741 "parser.c"
        break;
      case 58:
// 1003 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_parameter(NULL, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(27,&yymsp[-1].minor);
}
// 2749 "parser.c"
        break;
      case 59:
// 1007 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_parameter(yymsp[-3].minor.yy175, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(27,&yymsp[-1].minor);
}
// 2757 "parser.c"
        break;
      case 60:
// 1011 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_parameter(NULL, yymsp[-3].minor.yy175, yymsp[-2].minor.yy0, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(27,&yymsp[-1].minor);
}
// 2765 "parser.c"
        break;
      case 61:
// 1015 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(4,&yymsp[-2].minor);
  yy_destructor(5,&yymsp[0].minor);
}
// 2774 "parser.c"
        break;
      case 62:
// 1019 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_INTEGER);
  yy_destructor(33,&yymsp[0].minor);
}
// 2782 "parser.c"
        break;
      case 63:
// 1023 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_DOUBLE);
  yy_destructor(34,&yymsp[0].minor);
}
// 2790 "parser.c"
        break;
      case 64:
// 1027 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_BOOL);
  yy_destructor(35,&yymsp[0].minor);
}
// 2798 "parser.c"
        break;
      case 65:
// 1031 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_STRING);
  yy_destructor(36,&yymsp[0].minor);
}
// 2806 "parser.c"
        break;
      case 66:
// 1035 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_ARRAY);
  yy_destructor(37,&yymsp[0].minor);
}
// 2814 "parser.c"
        break;
      case 67:
// 1039 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_VAR);
  yy_destructor(38,&yymsp[0].minor);
}
// 2822 "parser.c"
        break;
      case 85:
// 1111 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_break_statement(status->scanner_state);
  yy_destructor(39,&yymsp[-1].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2831 "parser.c"
        break;
      case 86:
// 1115 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_continue_statement(status->scanner_state);
  yy_destructor(40,&yymsp[-1].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2840 "parser.c"
        break;
      case 87:
// 1119 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_if_statement(yymsp[-2].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(41,&yymsp[-3].minor);
  yy_destructor(21,&yymsp[-1].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2850 "parser.c"
        break;
      case 88:
// 1123 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_if_statement(yymsp[-5].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(41,&yymsp[-6].minor);
  yy_destructor(21,&yymsp[-4].minor);
  yy_destructor(22,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(21,&yymsp[-1].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2863 "parser.c"
        break;
      case 89:
// 1127 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_if_statement(yymsp[-3].minor.yy175, yymsp[-1].minor.yy175, NULL, status->scanner_state);
  yy_destructor(41,&yymsp[-4].minor);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2873 "parser.c"
        break;
      case 90:
// 1131 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_if_statement(yymsp[-7].minor.yy175, yymsp[-5].minor.yy175, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(41,&yymsp[-8].minor);
  yy_destructor(21,&yymsp[-6].minor);
  yy_destructor(22,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-3].minor);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2886 "parser.c"
        break;
      case 91:
// 1135 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_if_statement(yymsp[-6].minor.yy175, yymsp[-4].minor.yy175, NULL, status->scanner_state);
  yy_destructor(41,&yymsp[-7].minor);
  yy_destructor(21,&yymsp[-5].minor);
  yy_destructor(22,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(21,&yymsp[-1].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2899 "parser.c"
        break;
      case 92:
// 1139 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_if_statement(yymsp[-6].minor.yy175, NULL, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(41,&yymsp[-7].minor);
  yy_destructor(21,&yymsp[-5].minor);
  yy_destructor(22,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-3].minor);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2912 "parser.c"
        break;
      case 93:
// 1143 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_switch_statement(yymsp[-2].minor.yy175, NULL, status->scanner_state);
  yy_destructor(43,&yymsp[-3].minor);
  yy_destructor(21,&yymsp[-1].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2922 "parser.c"
        break;
      case 94:
// 1147 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_switch_statement(yymsp[-3].minor.yy175, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(43,&yymsp[-4].minor);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2932 "parser.c"
        break;
      case 97:
// 1159 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_case_clause(yymsp[-2].minor.yy175, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(44,&yymsp[-3].minor);
  yy_destructor(45,&yymsp[-1].minor);
}
// 2941 "parser.c"
        break;
      case 98:
// 1163 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_case_clause(NULL, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[-1].minor);
}
// 2950 "parser.c"
        break;
      case 99:
// 1167 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_loop_statement(NULL, status->scanner_state);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(21,&yymsp[-1].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2960 "parser.c"
        break;
      case 100:
// 1171 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_loop_statement(yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(47,&yymsp[-3].minor);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2970 "parser.c"
        break;
      case 101:
// 1175 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_while_statement(yymsp[-2].minor.yy175, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-3].minor);
  yy_destructor(21,&yymsp[-1].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2980 "parser.c"
        break;
      case 102:
// 1179 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_while_statement(yymsp[-3].minor.yy175, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(48,&yymsp[-4].minor);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2990 "parser.c"
        break;
      case 103:
// 1183 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_for_statement(yymsp[-3].minor.yy175, NULL, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(49,&yymsp[-6].minor);
  yy_destructor(50,&yymsp[-4].minor);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 3001 "parser.c"
        break;
      case 104:
// 1187 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_for_statement(yymsp[-3].minor.yy175, NULL, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(49,&yymsp[-7].minor);
  yy_destructor(50,&yymsp[-5].minor);
  yy_destructor(51,&yymsp[-4].minor);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 3013 "parser.c"
        break;
      case 105:
// 1191 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_for_statement(yymsp[-3].minor.yy175, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(49,&yymsp[-8].minor);
  yy_destructor(1,&yymsp[-6].minor);
  yy_destructor(50,&yymsp[-4].minor);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 3025 "parser.c"
        break;
      case 106:
// 1195 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_for_statement(yymsp[-3].minor.yy175, yymsp[-8].minor.yy0, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(49,&yymsp[-9].minor);
  yy_destructor(1,&yymsp[-7].minor);
  yy_destructor(50,&yymsp[-5].minor);
  yy_destructor(51,&yymsp[-4].minor);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 3038 "parser.c"
        break;
      case 107:
// 1199 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_statement(yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 3047 "parser.c"
        break;
      case 110:
// 1212 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("variable", yymsp[-2].minor.yy0, NULL, NULL, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(27,&yymsp[-1].minor);
}
// 3055 "parser.c"
        break;
      case 111:
// 1217 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("object-property", yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(53,&yymsp[-3].minor);
  yy_destructor(27,&yymsp[-1].minor);
}
// 3064 "parser.c"
        break;
      case 112:
// 1222 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("object-property-append", yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(53,&yymsp[-5].minor);
  yy_destructor(54,&yymsp[-3].minor);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(27,&yymsp[-1].minor);
}
// 3075 "parser.c"
        break;
      case 113:
// 1227 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("object-property-array-index", yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy175, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(53,&yymsp[-6].minor);
  yy_destructor(54,&yymsp[-4].minor);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(27,&yymsp[-1].minor);
}
// 3086 "parser.c"
        break;
      case 114:
// 1232 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("variable-append", yymsp[-4].minor.yy0, NULL, NULL, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(54,&yymsp[-3].minor);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(27,&yymsp[-1].minor);
}
// 3096 "parser.c"
        break;
      case 115:
// 1237 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("array-index", yymsp[-5].minor.yy0, NULL, yymsp[-3].minor.yy175, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(54,&yymsp[-4].minor);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(27,&yymsp[-1].minor);
}
// 3106 "parser.c"
        break;
      case 116:
// 1242 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("incr", yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(56,&yymsp[0].minor);
}
// 3114 "parser.c"
        break;
      case 117:
// 1247 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("decr", yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(57,&yymsp[0].minor);
}
// 3122 "parser.c"
        break;
      case 119:
// 1255 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_echo_statement(yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(58,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 3131 "parser.c"
        break;
      case 122:
// 1267 "parser.lemon"
{
	yygotominor.yy175 = yymsp[0].minor.yy175;;
}
// 3138 "parser.c"
        break;
      case 123:
// 1272 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_mcall_statement(yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(19,&yymsp[0].minor);
}
// 3146 "parser.c"
        break;
      case 124:
// 1277 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_return_statement(yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(59,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 3155 "parser.c"
        break;
      case 125:
// 1282 "parser.lemon"
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
// 3169 "parser.c"
        break;
      case 126:
// 1291 "parser.lemon"
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
// 3184 "parser.c"
        break;
      case 127:
// 1300 "parser.lemon"
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
// 3199 "parser.c"
        break;
      case 128:
// 1309 "parser.lemon"
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
// 3214 "parser.c"
        break;
      case 129:
// 1318 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_throw_exception(yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(63,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 3223 "parser.c"
        break;
      case 130:
// 1323 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_throw_exception(yymsp[-4].minor.yy0, yymsp[-2].minor.yy175, status->scanner_state);
  yy_destructor(63,&yymsp[-6].minor);
  yy_destructor(15,&yymsp[-5].minor);
  yy_destructor(31,&yymsp[-3].minor);
  yy_destructor(32,&yymsp[-1].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 3235 "parser.c"
        break;
      case 131:
// 1327 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_INTEGER, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(33,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 3244 "parser.c"
        break;
      case 132:
// 1331 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_DOUBLE, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(34,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 3253 "parser.c"
        break;
      case 133:
// 1335 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_STRING, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(36,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 3262 "parser.c"
        break;
      case 134:
// 1339 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_BOOL, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(35,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 3271 "parser.c"
        break;
      case 135:
// 1343 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_VAR, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 3280 "parser.c"
        break;
      case 138:
// 1355 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_variable(yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3287 "parser.c"
        break;
      case 139:
// 1359 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_variable(yymsp[-2].minor.yy0, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(27,&yymsp[-1].minor);
}
// 3295 "parser.c"
        break;
      case 141:
// 1367 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("not", yymsp[0].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(16,&yymsp[-1].minor);
}
// 3303 "parser.c"
        break;
      case 142:
// 1371 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("isset", yymsp[0].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(11,&yymsp[-1].minor);
}
// 3311 "parser.c"
        break;
      case 143:
// 1375 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("equals", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(2,&yymsp[-1].minor);
}
// 3319 "parser.c"
        break;
      case 144:
// 1379 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("not-equals", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(7,&yymsp[-1].minor);
}
// 3327 "parser.c"
        break;
      case 145:
// 1383 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("identical", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(3,&yymsp[-1].minor);
}
// 3335 "parser.c"
        break;
      case 146:
// 1387 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("not-identical", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(6,&yymsp[-1].minor);
}
// 3343 "parser.c"
        break;
      case 147:
// 1391 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("less", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(4,&yymsp[-1].minor);
}
// 3351 "parser.c"
        break;
      case 148:
// 1395 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("greater", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(5,&yymsp[-1].minor);
}
// 3359 "parser.c"
        break;
      case 149:
// 1399 "parser.lemon"
{
	yygotominor.yy175 = yymsp[-1].minor.yy175;
  yy_destructor(31,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3368 "parser.c"
        break;
      case 150:
// 1403 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(53,&yymsp[-1].minor);
}
// 3376 "parser.c"
        break;
      case 151:
      case 195:
// 1407 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(55,&yymsp[0].minor);
}
// 3386 "parser.c"
        break;
      case 152:
      case 196:
// 1412 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_INTEGER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(55,&yymsp[0].minor);
}
// 3396 "parser.c"
        break;
      case 153:
      case 197:
// 1417 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_STRING, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(55,&yymsp[0].minor);
}
// 3406 "parser.c"
        break;
      case 154:
// 1422 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("add", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(8,&yymsp[-1].minor);
}
// 3414 "parser.c"
        break;
      case 155:
// 1427 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("sub", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(9,&yymsp[-1].minor);
}
// 3422 "parser.c"
        break;
      case 156:
// 1432 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("concat", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(10,&yymsp[-1].minor);
}
// 3430 "parser.c"
        break;
      case 157:
// 1437 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("instanceof", yymsp[0].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(14,&yymsp[-1].minor);
}
// 3438 "parser.c"
        break;
      case 158:
// 1442 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("fetch", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(12,&yymsp[-3].minor);
  yy_destructor(1,&yymsp[-1].minor);
}
// 3447 "parser.c"
        break;
      case 159:
// 1447 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("typeof", yymsp[0].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(13,&yymsp[-1].minor);
}
// 3455 "parser.c"
        break;
      case 160:
      case 184:
      case 188:
// 1452 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state);
}
// 3464 "parser.c"
        break;
      case 161:
      case 186:
      case 189:
// 1457 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_INTEGER, yymsp[0].minor.yy0, status->scanner_state);
}
// 3473 "parser.c"
        break;
      case 162:
      case 185:
      case 190:
// 1462 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_STRING, yymsp[0].minor.yy0, status->scanner_state);
}
// 3482 "parser.c"
        break;
      case 163:
      case 191:
// 1467 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_DOUBLE, yymsp[0].minor.yy0, status->scanner_state);
}
// 3490 "parser.c"
        break;
      case 164:
      case 192:
// 1472 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_NULL, NULL, status->scanner_state);
  yy_destructor(65,&yymsp[0].minor);
}
// 3499 "parser.c"
        break;
      case 165:
      case 194:
// 1477 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_TRUE, NULL, status->scanner_state);
  yy_destructor(66,&yymsp[0].minor);
}
// 3508 "parser.c"
        break;
      case 166:
      case 193:
// 1482 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_FALSE, NULL, status->scanner_state);
  yy_destructor(67,&yymsp[0].minor);
}
// 3517 "parser.c"
        break;
      case 167:
// 1487 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("empty-array", NULL, NULL, NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-1].minor);
  yy_destructor(55,&yymsp[0].minor);
}
// 3526 "parser.c"
        break;
      case 168:
// 1492 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("array", yymsp[-1].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(55,&yymsp[0].minor);
}
// 3535 "parser.c"
        break;
      case 169:
// 1497 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_new_instance(yymsp[0].minor.yy0, NULL, status->scanner_state);
  yy_destructor(15,&yymsp[-1].minor);
}
// 3543 "parser.c"
        break;
      case 170:
// 1502 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_new_instance(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(15,&yymsp[-3].minor);
  yy_destructor(31,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3553 "parser.c"
        break;
      case 171:
// 1507 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_new_instance(yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(15,&yymsp[-4].minor);
  yy_destructor(31,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3563 "parser.c"
        break;
      case 172:
// 1512 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_fcall(yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(31,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3572 "parser.c"
        break;
      case 173:
// 1517 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_fcall(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(31,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3581 "parser.c"
        break;
      case 175:
// 1527 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_mcall(yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(53,&yymsp[-4].minor);
  yy_destructor(31,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3591 "parser.c"
        break;
      case 176:
// 1532 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_mcall(yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(53,&yymsp[-3].minor);
  yy_destructor(31,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3601 "parser.c"
        break;
      case 182:
// 1556 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_array_item(yymsp[-2].minor.yy175, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(45,&yymsp[-1].minor);
}
// 3609 "parser.c"
        break;
      case 183:
// 1560 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_array_item(NULL, yymsp[0].minor.yy175, status->scanner_state);
}
// 3616 "parser.c"
        break;
      case 199:
// 1627 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_comment(yymsp[0].minor.yy0, status->scanner_state);
}
// 3623 "parser.c"
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

// 3699 "parser.c"
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
