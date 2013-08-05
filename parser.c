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

static json_object *xx_ret_for_statement(json_object *expr, xx_parser_token *K, xx_parser_token *V, json_object *statements, xx_scanner_state *state)
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


// 692 "parser.c"
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
#define YYNOCODE 124
#define YYACTIONTYPE unsigned short int
#define xx_TOKENTYPE xx_parser_token*
typedef union {
  xx_TOKENTYPE yy0;
  json_object* yy55;
  int yy247;
} YYMINORTYPE;
#define YYSTACKDEPTH 100
#define xx_ARG_SDECL xx_parser_status *status;
#define xx_ARG_PDECL ,xx_parser_status *status
#define xx_ARG_FETCH xx_parser_status *status = yypParser->status
#define xx_ARG_STORE yypParser->status = status
#define YYNSTATE 432
#define YYNRULE 198
#define YYERRORSYMBOL 67
#define YYERRSYMDT yy247
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
 /*     0 */   272,  116,  118,  120,  296,  162,  616,  275,  631,    1,
 /*    10 */     2,  431,    4,    5,    6,  251,  260,  266,  263,  193,
 /*    20 */   269,   44,   46,   48,  330,   57,  345,  367,  153,   68,
 /*    30 */    72,   77,  406,   84,  409,  410,  238,  365,  377,  215,
 /*    40 */   224,  227,  272,  254,  242,  195,   83,  164,  163,  275,
 /*    50 */   157,  158,  160,  159,  303,  307,  311,  251,  260,  266,
 /*    60 */   263,  271,  269,   44,   46,   48,  146,   57,  165,  150,
 /*    70 */   161,   68,   72,   77,  148,   84,    3,    4,    5,    6,
 /*    80 */   185,  215,  224,  227,  272,   18,  242,  380,  338,  386,
 /*    90 */   367,  275,  154,  363,   17,  378,  277,  404,  377,  251,
 /*   100 */   260,  266,  263,  129,  269,   44,   46,   48,  306,   57,
 /*   110 */   173,  248,   80,   68,   72,   77,  175,   84,  325,  173,
 /*   120 */   324,  307,  311,  215,  224,  227,  272,  147,  242,   92,
 /*   130 */    76,  153,  194,  275,  155,  156,  173,  157,  158,  160,
 /*   140 */   159,  251,  260,  266,  263,   79,  269,   44,   46,   48,
 /*   150 */   339,   57,  324,  307,  311,   68,   72,   77,  453,   84,
 /*   160 */   245,  408,  392,  243,  398,  215,  224,  227,  272,   87,
 /*   170 */   242,   92,  301,  152,  198,  275,  155,  156,  173,  157,
 /*   180 */   158,  160,  159,  251,  260,  266,  263,   86,  269,   44,
 /*   190 */    46,   48,   58,   57,  180,  285,  416,   68,   72,   77,
 /*   200 */   456,   84,  173,  329,   19,  256,  354,  215,  224,  227,
 /*   210 */   272,  129,  242,  172,  344,  309,  448,  275,  173,  379,
 /*   220 */   392,  133,  398,  372,  174,  251,  260,  266,  263,  218,
 /*   230 */   269,   44,   46,   48,  240,   57,  182,  184,  153,   68,
 /*   240 */    72,   77,  454,   84,  171,  329,   19,  217,  354,  215,
 /*   250 */   224,  227,  272,  203,  242,  308,  342,  199,  452,  275,
 /*   260 */   173,  366,   19,  127,  354,  372,  172,  251,  260,  266,
 /*   270 */   263,  254,  269,   44,   46,   48,  310,   57,  149,  150,
 /*   280 */   161,   68,   72,   77,  450,   84,  233,  329,   19,  253,
 /*   290 */   354,  215,  224,  227,  272,  203,  242,  177,  286,  208,
 /*   300 */   424,  275,  173,  379,  392,  273,  398,  372,   62,  251,
 /*   310 */   260,  266,  263,  129,  269,   44,   46,   48,  368,   57,
 /*   320 */   173,  176,  241,   68,   72,   77,  175,   84,  236,  239,
 /*   330 */    64,  312,  287,  215,  224,  227,  272,   12,  242,  412,
 /*   340 */   351,  369,   15,  275,  302,  379,  392,  200,  398,  372,
 /*   350 */   172,  251,  260,  266,  263,  190,  269,   44,   46,   48,
 /*   360 */    49,   57,  173,  285,  299,   68,   72,   77,  455,   84,
 /*   370 */   173,  329,   19,   14,  354,  215,  224,  227,  272,   95,
 /*   380 */   242,  188,   52,  371,  449,  275,  173,  379,  392,  382,
 /*   390 */   398,  372,  172,  251,  260,  266,  263,  383,  269,   44,
 /*   400 */    46,   48,   73,   57,   90,  285,  370,   68,   72,   77,
 /*   410 */   451,   84,  173,  329,   19,  101,  354,  215,  224,  227,
 /*   420 */   272,  107,  242,  249,  353,   61,  290,  275,  173,  191,
 /*   430 */   204,  313,  212,  213,  302,  251,  260,  266,  263,  109,
 /*   440 */   269,   44,   46,   48,  364,   57,  173,  365,  377,   68,
 /*   450 */    72,   77,  298,   84,  324,  307,  311,  103,  105,  215,
 /*   460 */   224,  227,  272,  111,  242,  326,  292,  274,  417,  275,
 /*   470 */   173,  379,  392,   65,  398,  372,   60,  251,  260,  266,
 /*   480 */   263,  129,  269,   44,   46,   48,  373,   57,  173,  170,
 /*   490 */   314,   68,   72,   77,  175,   84,   85,  214,   64,  374,
 /*   500 */   287,  215,  224,  227,  272,  113,  242,  330,   71,  345,
 /*   510 */   430,  275,  173,  379,  392,  128,  398,  372,  201,  251,
 /*   520 */   260,  266,  263,  129,  269,   44,   46,   48,  375,   57,
 /*   530 */   173,  187,  376,   68,   72,   77,  175,   84,  407,  378,
 /*   540 */   115,  244,  377,  215,  224,  227,  272,  173,  242,   92,
 /*   550 */   284,    7,  202,  275,   10,  316,  173,  420,  418,  275,
 /*   560 */   317,  251,  260,  266,  263,  117,  269,   44,   46,   48,
 /*   570 */    59,   57,  173,  119,  167,   68,   72,   77,  186,   84,
 /*   580 */   173,  302,  121,  229,  232,  215,  224,  227,  272,  173,
 /*   590 */   242,  220,  291,  252,  259,  275,  216,  221,  173,  126,
 /*   600 */   179,  493,  254,  251,  260,  266,  263,  123,  269,   44,
 /*   610 */    46,   48,  340,   57,  173,  131,  494,   68,   72,   77,
 /*   620 */   262,   84,  173,  254,  137,  254,   47,  215,  224,  227,
 /*   630 */   272,  173,  242,   92,  360,  495,  207,  275,  261,  259,
 /*   640 */   173,  265,  225,  268,  278,  251,  260,  266,  263,  173,
 /*   650 */   269,   44,   46,   48,  388,   57,  264,  259,  220,   68,
 /*   660 */    72,   77,  389,   84,  219,  173,  267,  259,  496,  215,
 /*   670 */   224,  227,  272,  393,  242,   92,  283,  497,  211,  275,
 /*   680 */   270,  259,  173,  432,  280,   20,  381,  251,  260,  266,
 /*   690 */   263,  173,  269,   44,   46,   48,  394,   57,  331,  279,
 /*   700 */   400,   68,   72,   77,  395,   84,   11,  387,  401,  384,
 /*   710 */   399,  215,  224,  227,  272,  134,  242,   92,   27,  346,
 /*   720 */    91,  275,  355,  380,  173,  386,  421,  385,  425,  251,
 /*   730 */   260,  266,  263,  498,  269,   44,   46,   48,  499,   57,
 /*   740 */   429,  246,  168,   68,   72,   77,   89,   84,  247,   99,
 /*   750 */   327,  390,  391,  215,  224,  227,  272,  135,  242,  328,
 /*   760 */   362,  223,   74,  275,  250,  427,  331,   54,  332,  419,
 /*   770 */   206,  251,  260,  266,  263,  396,  269,   44,   46,   48,
 /*   780 */   397,   57,  333,  335,   66,   68,   72,   77,  192,   84,
 /*   790 */    53,  255,  288,  414,  403,  215,  224,  227,  272,  402,
 /*   800 */   242,  228,   25,  257,    9,  275,  209,  181,    8,  210,
 /*   810 */   341,  293,  258,  251,  260,  266,  263,  294,  269,   44,
 /*   820 */    46,   48,  230,   57,   98,  231,  614,   68,   72,   77,
 /*   830 */   100,   84,   22,  102,   50,  346,  151,  215,  224,  227,
 /*   840 */   272,  104,  242,  347,  297,  615,  106,  275,  348,   45,
 /*   850 */   349,  183,  350,  426,   21,  251,  260,  266,  263,  423,
 /*   860 */   269,   44,   46,   48,  234,   57,   69,  235,   20,   68,
 /*   870 */    72,   77,   63,   84,  355,   78,  356,  357,  197,  215,
 /*   880 */   224,  227,  272,  237,  242,  358,  336,  413,  359,  275,
 /*   890 */   305,  323,   88,  323,  323,  323,  323,  251,  260,  266,
 /*   900 */   263,  323,  269,   44,   46,   48,  323,   57,  323,  323,
 /*   910 */   323,   68,   72,   77,  323,   84,  323,  323,  323,  323,
 /*   920 */   323,  215,  224,  227,  272,  323,  242,  323,   56,  323,
 /*   930 */   323,  275,  323,  323,  323,  323,  323,  323,  323,  251,
 /*   940 */   260,  266,  263,  323,  269,   44,   46,   48,  323,   57,
 /*   950 */   323,  323,  323,   68,   72,   77,  323,   84,  323,  272,
 /*   960 */   323,  323,  323,  215,  224,  227,  275,  323,  242,  323,
 /*   970 */   323,  323,  323,  323,  251,  260,  266,  263,  323,  269,
 /*   980 */    44,   46,   48,  323,   57,  323,  323,  323,   68,   72,
 /*   990 */    77,  323,   84,  323,  323,  323,   43,  323,  215,  224,
 /*  1000 */   227,  323,  323,  242,  323,  323,   82,  323,  323,  323,
 /*  1010 */   276,   29,   30,   31,   32,   33,   34,   35,   36,   37,
 /*  1020 */    38,   39,   40,   41,   42,  323,  323,  323,   43,  323,
 /*  1030 */   422,   16,  405,  411,  410,  222,  404,  377,   26,  323,
 /*  1040 */   323,  323,  276,   29,   30,   31,   32,   33,   34,   35,
 /*  1050 */    36,   37,   38,   39,   40,   41,   42,  323,  323,   43,
 /*  1060 */   323,  323,  323,  323,  323,  323,  323,  222,  323,  300,
 /*  1070 */   323,  323,  323,  276,   29,   30,   31,   32,   33,   34,
 /*  1080 */    35,   36,   37,   38,   39,   40,   41,   42,  323,   43,
 /*  1090 */   323,  323,  323,  323,  323,  323,  323,  323,  222,  295,
 /*  1100 */   323,  323,  323,  276,   29,   30,   31,   32,   33,   34,
 /*  1110 */    35,   36,   37,   38,   39,   40,   41,   42,  323,   43,
 /*  1120 */   323,  323,  323,  323,  323,  323,  323,  323,  222,  337,
 /*  1130 */   323,  323,  323,  276,   29,   30,   31,   32,   33,   34,
 /*  1140 */    35,   36,   37,   38,   39,   40,   41,   42,  323,   43,
 /*  1150 */   323,  323,  323,  323,  323,  323,  323,  323,  222,  289,
 /*  1160 */   323,  323,  323,  276,   29,   30,   31,   32,   33,   34,
 /*  1170 */    35,   36,   37,   38,   39,   40,   41,   42,  323,   43,
 /*  1180 */   323,  323,  323,  323,  323,  323,  323,  323,  222,  361,
 /*  1190 */   323,  323,  323,  276,   29,   30,   31,   32,   33,   34,
 /*  1200 */    35,   36,   37,   38,   39,   40,   41,   42,  323,   43,
 /*  1210 */   323,  323,  323,  323,  323,  323,  323,  323,  222,  282,
 /*  1220 */   323,  323,  323,  276,   29,   30,   31,   32,   33,   34,
 /*  1230 */    35,   36,   37,   38,   39,   40,   41,   42,  323,   43,
 /*  1240 */   323,  323,  323,  323,  323,  323,  323,  323,  222,   67,
 /*  1250 */   323,  323,  323,  276,   29,   30,   31,   32,   33,   34,
 /*  1260 */    35,   36,   37,   38,   39,   40,   41,   42,  323,   43,
 /*  1270 */   323,  323,  323,  323,  323,  323,  323,  323,  222,   51,
 /*  1280 */   323,  323,  323,  276,   29,   30,   31,   32,   33,   34,
 /*  1290 */    35,   36,   37,   38,   39,   40,   41,   42,  323,   43,
 /*  1300 */   323,  323,  323,  323,  323,  323,  323,  323,  222,  352,
 /*  1310 */   323,  323,  323,  276,   29,   30,   31,   32,   33,   34,
 /*  1320 */    35,   36,   37,   38,   39,   40,   41,   42,  323,   43,
 /*  1330 */   323,  323,  323,  323,  323,  323,  323,  323,  222,   70,
 /*  1340 */   323,  323,  323,  276,   29,   30,   31,   32,   33,   34,
 /*  1350 */    35,   36,   37,   38,   39,   40,   41,   42,  323,   43,
 /*  1360 */   323,  323,  323,  323,  323,  323,  323,  323,  222,   55,
 /*  1370 */   323,  323,  323,  276,   29,   30,   31,   32,   33,   34,
 /*  1380 */    35,   36,   37,   38,   39,   40,   41,   42,  323,   43,
 /*  1390 */   323,  323,  323,  323,  323,  323,  323,  323,  222,  343,
 /*  1400 */   323,  323,  323,  276,   29,   30,   31,   32,   33,   34,
 /*  1410 */    35,   36,   37,   38,   39,   40,   41,   42,  323,   43,
 /*  1420 */   323,  323,  323,  323,  323,  323,  323,  323,  222,   75,
 /*  1430 */   323,  323,  323,  276,   29,   30,   31,   32,   33,   34,
 /*  1440 */    35,   36,   37,   38,   39,   40,   41,   42,   43,   93,
 /*  1450 */   108,  112,  114,  110,   96,  116,  118,  120,  222,  323,
 /*  1460 */   323,  323,   28,   29,   30,   31,   32,   33,   34,   35,
 /*  1470 */    36,   37,   38,   39,   40,   41,   42,   97,  132,  136,
 /*  1480 */   130,  166,   94,  323,  125,  323,  323,  222,  415,   16,
 /*  1490 */   405,  411,  410,  323,  404,  377,  323,  122,  189,  323,
 /*  1500 */   323,   93,  108,  112,  114,  110,   96,  116,  118,  120,
 /*  1510 */   428,   16,  405,  411,  410,  323,  404,  377,  323,  144,
 /*  1520 */   628,  323,  323,  323,  323,  323,  138,  139,  323,  140,
 /*  1530 */   141,  142,  143,  323,  323,  323,  323,  323,   97,  132,
 /*  1540 */   136,  130,  166,   94,  323,  125,  323,  323,  323,  323,
 /*  1550 */   323,  323,   97,  132,  136,  130,  166,   94,  122,  125,
 /*  1560 */    93,  108,  112,  114,  110,   96,  116,  118,  120,  323,
 /*  1570 */   323,  323,  122,  323,  323,  323,  323,  323,  323,  281,
 /*  1580 */   144,  205,  323,  323,  323,  323,  323,  138,  139,  323,
 /*  1590 */   140,  141,  142,  143,  144,  196,  323,  323,  323,  323,
 /*  1600 */   323,  138,  139,  323,  140,  141,  142,  143,  323,  323,
 /*  1610 */   323,  323,  323,   97,  132,  136,  130,  166,   94,  323,
 /*  1620 */   125,   93,  108,  112,  114,  110,   96,  116,  118,  120,
 /*  1630 */   323,  323,  323,  122,  169,  323,   93,  108,  112,  114,
 /*  1640 */   110,   96,  116,  118,  120,  323,  323,  323,  323,  323,
 /*  1650 */   323,  124,  323,  226,  323,  144,  323,  323,  323,  323,
 /*  1660 */   323,  323,  138,  139,  323,  140,  141,  142,  143,  323,
 /*  1670 */   323,  323,  323,  323,   97,  132,  136,  130,  166,   94,
 /*  1680 */   323,  125,   93,  108,  112,  114,  110,   96,  116,  118,
 /*  1690 */   120,  323,  323,  323,  122,  178,   13,   16,  405,  411,
 /*  1700 */   410,   81,  404,  377,   93,  108,  112,  114,  110,   96,
 /*  1710 */   116,  118,  120,  323,  323,  323,  144,  315,  323,  323,
 /*  1720 */   323,  323,  323,  138,  139,  162,  140,  141,  142,  143,
 /*  1730 */   323,  304,  323,  323,  323,   97,  132,  136,  130,  166,
 /*  1740 */    94,  323,  125,  323,  323,  334,  318,  319,  320,  321,
 /*  1750 */   322,  323,  323,  323,  323,  122,  548,  323,  323,  323,
 /*  1760 */   323,  145,  323,  323,  323,  323,  323,  164,  163,  323,
 /*  1770 */   157,  158,  160,  159,  323,  323,  323,  144,  323,  323,
 /*  1780 */   323,  323,  315,  433,  138,  139,  323,  140,  141,  142,
 /*  1790 */   143,  323,  315,  323,  323,  323,  304,  323,  323,   24,
 /*  1800 */     7,  323,  323,   10,  315,  323,  304,  418,  275,  323,
 /*  1810 */   323,  318,  319,  320,  321,  322,  323,  323,  304,  323,
 /*  1820 */    23,  318,  319,  320,  321,  322,  323,  323,  323,  323,
 /*  1830 */   323,  323,  323,  318,  319,  320,  321,  322,  323,
};
static YYCODETYPE yy_lookahead[] = {
 /*     0 */    18,    8,    9,   10,   22,   18,   45,   25,   68,   69,
 /*    10 */    70,   71,   72,   73,   74,   33,   34,   35,   36,   27,
 /*    20 */    38,   39,   40,   41,   26,   43,   28,   29,   80,   47,
 /*    30 */    48,   49,   76,   51,   78,   79,   19,   81,   82,   57,
 /*    40 */    58,   59,   18,    1,   62,   53,   22,   60,   61,   25,
 /*    50 */    63,   64,   65,   66,   85,   86,   87,   33,   34,   35,
 /*    60 */    36,   19,   38,   39,   40,   41,  118,   43,  120,  121,
 /*    70 */   122,   47,   48,   49,    1,   51,   71,   72,   73,   74,
 /*    80 */    54,   57,   58,   59,   18,   82,   62,   26,   22,   28,
 /*    90 */    29,   25,   18,   77,   78,   79,    1,   81,   82,   33,
 /*   100 */    34,   35,   36,  106,   38,   39,   40,   41,   80,   43,
 /*   110 */   113,  114,  106,   47,   48,   49,  119,   51,   83,  113,
 /*   120 */    85,   86,   87,   57,   58,   59,   18,   54,   62,  106,
 /*   130 */    22,   80,  109,   25,   60,   61,  113,   63,   64,   65,
 /*   140 */    66,   33,   34,   35,   36,   50,   38,   39,   40,   41,
 /*   150 */    83,   43,   85,   86,   87,   47,   48,   49,   22,   51,
 /*   160 */    15,   25,   26,   18,   28,   57,   58,   59,   18,    1,
 /*   170 */    62,  106,   22,  122,  109,   25,   60,   61,  113,   63,
 /*   180 */    64,   65,   66,   33,   34,   35,   36,   19,   38,   39,
 /*   190 */    40,   41,  103,   43,   18,  106,   22,   47,   48,   49,
 /*   200 */    22,   51,  113,   25,   26,   18,   28,   57,   58,   59,
 /*   210 */    18,  106,   62,    1,   22,   27,   22,   25,  113,   25,
 /*   220 */    26,   18,   28,   29,  119,   33,   34,   35,   36,    1,
 /*   230 */    38,   39,   40,   41,   54,   43,   60,   61,   80,   47,
 /*   240 */    48,   49,   22,   51,   32,   25,   26,   19,   28,   57,
 /*   250 */    58,   59,   18,  106,   62,   18,   22,  110,   22,   25,
 /*   260 */   113,   25,   26,   18,   28,   29,    1,   33,   34,   35,
 /*   270 */    36,    1,   38,   39,   40,   41,   80,   43,  120,  121,
 /*   280 */   122,   47,   48,   49,   22,   51,   18,   25,   26,   19,
 /*   290 */    28,   57,   58,   59,   18,  106,   62,   32,   22,  110,
 /*   300 */    22,   25,  113,   25,   26,   52,   28,   29,   22,   33,
 /*   310 */    34,   35,   36,  106,   38,   39,   40,   41,   18,   43,
 /*   320 */   113,  114,   19,   47,   48,   49,  119,   51,   60,   61,
 /*   330 */    44,   18,   46,   57,   58,   59,   18,   21,   62,   23,
 /*   340 */    22,   27,   22,   25,    1,   25,   26,   54,   28,   29,
 /*   350 */     1,   33,   34,   35,   36,  106,   38,   39,   40,   41,
 /*   360 */   103,   43,  113,  106,   21,   47,   48,   49,   22,   51,
 /*   370 */   113,   25,   26,   22,   28,   57,   58,   59,   18,  106,
 /*   380 */    62,   32,   22,   19,   22,   25,  113,   25,   26,   19,
 /*   390 */    28,   29,    1,   33,   34,   35,   36,   27,   38,   39,
 /*   400 */    40,   41,  103,   43,   27,  106,   80,   47,   48,   49,
 /*   410 */    22,   51,  113,   25,   26,   18,   28,   57,   58,   59,
 /*   420 */    18,  106,   62,   32,   22,  104,  105,   25,  113,   52,
 /*   430 */    53,   27,   55,   56,    1,   33,   34,   35,   36,  106,
 /*   440 */    38,   39,   40,   41,   78,   43,  113,   81,   82,   47,
 /*   450 */    48,   49,   83,   51,   85,   86,   87,   60,   61,   57,
 /*   460 */    58,   59,   18,  106,   62,   32,   22,   18,   22,   25,
 /*   470 */   113,   25,   26,   80,   28,   29,   22,   33,   34,   35,
 /*   480 */    36,  106,   38,   39,   40,   41,   18,   43,  113,  114,
 /*   490 */    80,   47,   48,   49,  119,   51,  107,  108,   44,   27,
 /*   500 */    46,   57,   58,   59,   18,  106,   62,   26,   22,   28,
 /*   510 */    22,   25,  113,   25,   26,   31,   28,   29,   27,   33,
 /*   520 */    34,   35,   36,  106,   38,   39,   40,   41,   80,   43,
 /*   530 */   113,  114,   19,   47,   48,   49,  119,   51,   78,   79,
 /*   540 */   106,   19,   82,   57,   58,   59,   18,  113,   62,  106,
 /*   550 */    22,   17,  109,   25,   20,   18,  113,   18,   24,   25,
 /*   560 */     5,   33,   34,   35,   36,  106,   38,   39,   40,   41,
 /*   570 */    21,   43,  113,  106,   18,   47,   48,   49,   31,   51,
 /*   580 */   113,    1,  106,   52,   53,   57,   58,   59,   18,  113,
 /*   590 */    62,  106,   22,  115,  116,   25,  111,  112,  113,   52,
 /*   600 */    53,   18,    1,   33,   34,   35,   36,  106,   38,   39,
 /*   610 */    40,   41,   32,   43,  113,  106,   18,   47,   48,   49,
 /*   620 */    19,   51,  113,    1,  106,    1,   19,   57,   58,   59,
 /*   630 */    18,  113,   62,  106,   22,   18,  109,   25,  115,  116,
 /*   640 */   113,   19,  106,   19,   18,   33,   34,   35,   36,  113,
 /*   650 */    38,   39,   40,   41,   19,   43,  115,  116,  106,   47,
 /*   660 */    48,   49,   27,   51,  112,  113,  115,  116,   18,   57,
 /*   670 */    58,   59,   18,   18,   62,  106,   22,   18,  109,   25,
 /*   680 */   115,  116,  113,    0,  106,   30,   18,   33,   34,   35,
 /*   690 */    36,  113,   38,   39,   40,   41,   19,   43,   30,   50,
 /*   700 */    19,   47,   48,   49,   27,   51,   18,   18,   27,   80,
 /*   710 */    18,   57,   58,   59,   18,    1,   62,  106,   22,   30,
 /*   720 */   109,   25,   30,   26,  113,   28,   21,   19,   23,   33,
 /*   730 */    34,   35,   36,   18,   38,   39,   40,   41,   18,   43,
 /*   740 */    22,   18,   31,   47,   48,   49,   18,   51,   31,   18,
 /*   750 */    21,   80,   19,   57,   58,   59,   18,  117,   62,   22,
 /*   760 */    22,   19,   21,   25,   19,   21,   30,   21,   18,   20,
 /*   770 */    27,   33,   34,   35,   36,   80,   38,   39,   40,   41,
 /*   780 */    19,   43,   31,   21,   45,   47,   48,   49,   18,   51,
 /*   790 */    42,  116,   45,   21,   19,   57,   58,   59,   18,   80,
 /*   800 */    62,   18,   22,   27,   19,   25,   54,   54,   18,   27,
 /*   810 */    21,   42,   80,   33,   34,   35,   36,   21,   38,   39,
 /*   820 */    40,   41,   18,   43,  117,   19,   45,   47,   48,   49,
 /*   830 */    53,   51,   31,   54,   21,   30,   45,   57,   58,   59,
 /*   840 */    18,   54,   62,   18,   22,   45,   54,   25,   31,   19,
 /*   850 */    32,   54,   21,   18,   18,   33,   34,   35,   36,   22,
 /*   860 */    38,   39,   40,   41,   54,   43,   21,   19,   30,   47,
 /*   870 */    48,   49,  105,   51,   30,   18,   18,   31,   27,   57,
 /*   880 */    58,   59,   18,   54,   62,   32,   22,   18,   21,   25,
 /*   890 */    27,  123,  108,  123,  123,  123,  123,   33,   34,   35,
 /*   900 */    36,  123,   38,   39,   40,   41,  123,   43,  123,  123,
 /*   910 */   123,   47,   48,   49,  123,   51,  123,  123,  123,  123,
 /*   920 */   123,   57,   58,   59,   18,  123,   62,  123,   22,  123,
 /*   930 */   123,   25,  123,  123,  123,  123,  123,  123,  123,   33,
 /*   940 */    34,   35,   36,  123,   38,   39,   40,   41,  123,   43,
 /*   950 */   123,  123,  123,   47,   48,   49,  123,   51,  123,   18,
 /*   960 */   123,  123,  123,   57,   58,   59,   25,  123,   62,  123,
 /*   970 */   123,  123,  123,  123,   33,   34,   35,   36,  123,   38,
 /*   980 */    39,   40,   41,  123,   43,  123,  123,  123,   47,   48,
 /*   990 */    49,  123,   51,  123,  123,  123,   74,  123,   57,   58,
 /*  1000 */    59,  123,  123,   62,  123,  123,   84,  123,  123,  123,
 /*  1010 */    88,   89,   90,   91,   92,   93,   94,   95,   96,   97,
 /*  1020 */    98,   99,  100,  101,  102,  123,  123,  123,   74,  123,
 /*  1030 */    75,   76,   77,   78,   79,  113,   81,   82,   84,  123,
 /*  1040 */   123,  123,   88,   89,   90,   91,   92,   93,   94,   95,
 /*  1050 */    96,   97,   98,   99,  100,  101,  102,  123,  123,   74,
 /*  1060 */   123,  123,  123,  123,  123,  123,  123,  113,  123,   84,
 /*  1070 */   123,  123,  123,   88,   89,   90,   91,   92,   93,   94,
 /*  1080 */    95,   96,   97,   98,   99,  100,  101,  102,  123,   74,
 /*  1090 */   123,  123,  123,  123,  123,  123,  123,  123,  113,   84,
 /*  1100 */   123,  123,  123,   88,   89,   90,   91,   92,   93,   94,
 /*  1110 */    95,   96,   97,   98,   99,  100,  101,  102,  123,   74,
 /*  1120 */   123,  123,  123,  123,  123,  123,  123,  123,  113,   84,
 /*  1130 */   123,  123,  123,   88,   89,   90,   91,   92,   93,   94,
 /*  1140 */    95,   96,   97,   98,   99,  100,  101,  102,  123,   74,
 /*  1150 */   123,  123,  123,  123,  123,  123,  123,  123,  113,   84,
 /*  1160 */   123,  123,  123,   88,   89,   90,   91,   92,   93,   94,
 /*  1170 */    95,   96,   97,   98,   99,  100,  101,  102,  123,   74,
 /*  1180 */   123,  123,  123,  123,  123,  123,  123,  123,  113,   84,
 /*  1190 */   123,  123,  123,   88,   89,   90,   91,   92,   93,   94,
 /*  1200 */    95,   96,   97,   98,   99,  100,  101,  102,  123,   74,
 /*  1210 */   123,  123,  123,  123,  123,  123,  123,  123,  113,   84,
 /*  1220 */   123,  123,  123,   88,   89,   90,   91,   92,   93,   94,
 /*  1230 */    95,   96,   97,   98,   99,  100,  101,  102,  123,   74,
 /*  1240 */   123,  123,  123,  123,  123,  123,  123,  123,  113,   84,
 /*  1250 */   123,  123,  123,   88,   89,   90,   91,   92,   93,   94,
 /*  1260 */    95,   96,   97,   98,   99,  100,  101,  102,  123,   74,
 /*  1270 */   123,  123,  123,  123,  123,  123,  123,  123,  113,   84,
 /*  1280 */   123,  123,  123,   88,   89,   90,   91,   92,   93,   94,
 /*  1290 */    95,   96,   97,   98,   99,  100,  101,  102,  123,   74,
 /*  1300 */   123,  123,  123,  123,  123,  123,  123,  123,  113,   84,
 /*  1310 */   123,  123,  123,   88,   89,   90,   91,   92,   93,   94,
 /*  1320 */    95,   96,   97,   98,   99,  100,  101,  102,  123,   74,
 /*  1330 */   123,  123,  123,  123,  123,  123,  123,  123,  113,   84,
 /*  1340 */   123,  123,  123,   88,   89,   90,   91,   92,   93,   94,
 /*  1350 */    95,   96,   97,   98,   99,  100,  101,  102,  123,   74,
 /*  1360 */   123,  123,  123,  123,  123,  123,  123,  123,  113,   84,
 /*  1370 */   123,  123,  123,   88,   89,   90,   91,   92,   93,   94,
 /*  1380 */    95,   96,   97,   98,   99,  100,  101,  102,  123,   74,
 /*  1390 */   123,  123,  123,  123,  123,  123,  123,  123,  113,   84,
 /*  1400 */   123,  123,  123,   88,   89,   90,   91,   92,   93,   94,
 /*  1410 */    95,   96,   97,   98,   99,  100,  101,  102,  123,   74,
 /*  1420 */   123,  123,  123,  123,  123,  123,  123,  123,  113,   84,
 /*  1430 */   123,  123,  123,   88,   89,   90,   91,   92,   93,   94,
 /*  1440 */    95,   96,   97,   98,   99,  100,  101,  102,   74,    2,
 /*  1450 */     3,    4,    5,    6,    7,    8,    9,   10,  113,  123,
 /*  1460 */   123,  123,   88,   89,   90,   91,   92,   93,   94,   95,
 /*  1470 */    96,   97,   98,   99,  100,  101,  102,   11,   12,   13,
 /*  1480 */    14,   15,   16,  123,   18,  123,  123,  113,   75,   76,
 /*  1490 */    77,   78,   79,  123,   81,   82,  123,   31,   32,  123,
 /*  1500 */   123,    2,    3,    4,    5,    6,    7,    8,    9,   10,
 /*  1510 */    75,   76,   77,   78,   79,  123,   81,   82,  123,   53,
 /*  1520 */    21,  123,  123,  123,  123,  123,   60,   61,  123,   63,
 /*  1530 */    64,   65,   66,  123,  123,  123,  123,  123,   11,   12,
 /*  1540 */    13,   14,   15,   16,  123,   18,  123,  123,  123,  123,
 /*  1550 */   123,  123,   11,   12,   13,   14,   15,   16,   31,   18,
 /*  1560 */     2,    3,    4,    5,    6,    7,    8,    9,   10,  123,
 /*  1570 */   123,  123,   31,  123,  123,  123,  123,  123,  123,   21,
 /*  1580 */    53,   54,  123,  123,  123,  123,  123,   60,   61,  123,
 /*  1590 */    63,   64,   65,   66,   53,   54,  123,  123,  123,  123,
 /*  1600 */   123,   60,   61,  123,   63,   64,   65,   66,  123,  123,
 /*  1610 */   123,  123,  123,   11,   12,   13,   14,   15,   16,  123,
 /*  1620 */    18,    2,    3,    4,    5,    6,    7,    8,    9,   10,
 /*  1630 */   123,  123,  123,   31,   32,  123,    2,    3,    4,    5,
 /*  1640 */     6,    7,    8,    9,   10,  123,  123,  123,  123,  123,
 /*  1650 */   123,   32,  123,   19,  123,   53,  123,  123,  123,  123,
 /*  1660 */   123,  123,   60,   61,  123,   63,   64,   65,   66,  123,
 /*  1670 */   123,  123,  123,  123,   11,   12,   13,   14,   15,   16,
 /*  1680 */   123,   18,    2,    3,    4,    5,    6,    7,    8,    9,
 /*  1690 */    10,  123,  123,  123,   31,   32,   75,   76,   77,   78,
 /*  1700 */    79,   21,   81,   82,    2,    3,    4,    5,    6,    7,
 /*  1710 */     8,    9,   10,  123,  123,  123,   53,    4,  123,  123,
 /*  1720 */   123,  123,  123,   60,   61,   18,   63,   64,   65,   66,
 /*  1730 */   123,   18,  123,  123,  123,   11,   12,   13,   14,   15,
 /*  1740 */    16,  123,   18,  123,  123,   32,   33,   34,   35,   36,
 /*  1750 */    37,   38,  123,  123,  123,   31,   54,  123,  123,  123,
 /*  1760 */   123,   54,  123,  123,  123,  123,  123,   60,   61,  123,
 /*  1770 */    63,   64,   65,   66,  123,  123,  123,   53,  123,  123,
 /*  1780 */   123,  123,    4,    0,   60,   61,  123,   63,   64,   65,
 /*  1790 */    66,  123,    4,  123,  123,  123,   18,  123,  123,   21,
 /*  1800 */    17,  123,  123,   20,    4,  123,   18,   24,   25,  123,
 /*  1810 */   123,   33,   34,   35,   36,   37,   38,  123,   18,  123,
 /*  1820 */    32,   33,   34,   35,   36,   37,   38,  123,  123,  123,
 /*  1830 */   123,  123,  123,   33,   34,   35,   36,   37,   38,
};
#define YY_SHIFT_USE_DFLT (-40)
static short yy_shift_ofst[] = {
 /*     0 */   534,  683, 1783,  -40,  -40,  -40,  -40,  790,  785,  -40,
 /*    10 */   688,  316,  320,  351,  -40,  -40,  194,  388,  -40,  838,
 /*    20 */   836,  801, 1788, 1778,  780,  -40,  696,  -40,  -40,  -40,
 /*    30 */   -40,  -40,  -40,  -40,  -40,  -40,  -40,  -40,  -40,  -40,
 /*    40 */   -40,  -40,  -40,  -40,  830,  -40,  607,  -40, 1724,  813,
 /*    50 */   444,  360,  748,  746,  570,  906,  -40, 1724,  549,  454,
 /*    60 */   -40,  286,  -40,  -40,  116,  739,  941,  941,  845,  276,
 /*    70 */   486,  -40, 1724,  741,  528,  108,  -40,  857,   95, 1724,
 /*    80 */  1680,  941,   24,  -40,  728,  168,  -40,  728,  -40,  377,
 /*    90 */  1724,  -40, 1447, 1724, 1724,  -40, 1724,  731,  -40,  777,
 /*   100 */   397,  779,  -40,  787,  -40,  792,  -40,   -7, 1724,   -7,
 /*   110 */  1724,   -7, 1724,   -7, 1724,   -7, 1724,  -40, 1724,  -40,
 /*   120 */  1724,  -40, 1724, 1619,  -40,  547,  245,  484, 1663, 1447,
 /*   130 */  1724,  -40,  203,  714,  731,  -40, 1724,  -40,  -40,  -40,
 /*   140 */   -40,  -40,  -40,  -40, 1707,  -40,   73,  -40,  -13,  -40,
 /*   150 */   791,   74,  -40,  -40,  -40,  -40,  -40,  -40,  -40,  -40,
 /*   160 */   -40,  -40,  781,  800,  -39,  -40,  556,  711, 1602,  -40,
 /*   170 */   212,  -40, 1724,  -40,  -40,  -40,  265,  -40,  -40,  176,
 /*   180 */   753,  -40,  797,  -40,   26,  -40, 1466,  349,  -40,  -40,
 /*   190 */    -7,  770,   -8, 1724,  -40, 1541,  851, 1724,  -40,  293,
 /*   200 */   491, 1724,  -40, 1702, 1527,  743, 1724,  -40,  752,  782,
 /*   210 */  1724,  -40,  -40,  -40,  -40, 1724,  228,  -40, 1724,  -40,
 /*   220 */  1447,  -40,  742,  -40, 1724, 1634,  -40,  783,  531,  804,
 /*   230 */   806,  -40,  268,  810,  848,  -40,  829,   17,  -40,  180,
 /*   240 */   303,  -40,  145,  522,  -40,  723,  717, 1724,  391,  745,
 /*   250 */   -40,  187,  270,  -40,  187,  -40,  776,  116,  -40,  -40,
 /*   260 */   187,  601,  -40,  187,  622,  -40,  187,  624,  -40,  187,
 /*   270 */    42,  -40,  253,  449,  484,  -40,  -40,  626,  649, 1724,
 /*   280 */  1558,  941,  654,  -40,  -40, 1499,  -40,  747,  941,  941,
 /*   290 */   -40,  -40,  769,  796,  822,  -18,  -40,  -40,  343,  941,
 /*   300 */   150,  -40, 1800,  -40,  863,  116,  -40,  237,  188,  116,
 /*   310 */   -40,  313,  404,  116,  -40,  537,  555,  583,  598,  617,
 /*   320 */   650,  659,  715,  720,  -40,  433,  729,  737,  -40,  481,
 /*   330 */   736,  750,  751, 1713,  762,  864,  -40,   66,  -40,  580,
 /*   340 */   789,  234,  -40,  192,  -40,  805,  825,  817,  818,  831,
 /*   350 */   318,  -40,  402,  -40,  844,  858,  846,  853,  867,  612,
 /*   360 */   -40,  738,  -40,  236,  346,  -40,   -2,  300,  314,  116,
 /*   370 */   364,  -40,  468,  472,  116,  513,  -40,  -40,  -40,   61,
 /*   380 */   668,  370,  -40,  116,  708,  -40,  689,  635,  -40,  116,
 /*   390 */   733,  -40,  655,  677,  -40,  116,  761,  -40,  692,  681,
 /*   400 */   -40,  116,  775,  -40,  -40,  362,  136,  178,  697,  220,
 /*   410 */   -40,  262,  869,  772,  446,  174,  -40,  -40,  749,  539,
 /*   420 */   705,  278,  837,  -40,  -40,  835,  744,  488,  718,  -40,
 /*   430 */   -40,  -40,
};
#define YY_REDUCE_USE_DFLT (-61)
static short yy_reduce_ofst[] = {
 /*     0 */   -60,  -61,    5,  -61,  -61,  -61,  -61,  -61,  -61,  -61,
 /*    10 */   -61,  -61, 1621,  -61,  -61,  -61,   16,    3,  -61,  -61,
 /*    20 */   -61,  -61,   35,  369,  954,  -61, 1374,  -61,  -61,  -61,
 /*    30 */   -61,  -61,  -61,  -61,  -61,  -61,  -61,  -61,  -61,  -61,
 /*    40 */   -61,  -61,  -61,  -61,  -61,  -61,  -61,  -61,  257,  -61,
 /*    50 */  1195, 1374,  -61,  -61, 1285, 1374,  -61,   89,  -61,  321,
 /*    60 */   -61,  767,  -61,  -61,  393,  -61, 1165, 1374,  -61, 1255,
 /*    70 */  1374,  -61,  299,  -61, 1345, 1374,  -61,  -61,  -61,    6,
 /*    80 */   -61,  922, 1374,  -61,  389,  -61,  -61,  784,  -61,  -61,
 /*    90 */   611,  -61,  -61,  249,  273,  -61,  315,  707,  -61,  -61,
 /*   100 */   -61,  -61,  -61,  -61,  -61,  -61,  -61,  -61,  333,  -61,
 /*   110 */   357,  -61,  399,  -61,  434,  -61,  459,  -61,  467,  -61,
 /*   120 */   476,  -61,  501,  -61,  -61,  -61,  -61,  -61,  207,  -61,
 /*   130 */   509,  -61,  -61,  -61,  640,  -61,  518,  -61,  -61,  -61,
 /*   140 */   -61,  -61,  -61,  -61,  -52,  -61,  -61,  -61,  158,  -61,
 /*   150 */   -61,   51,  -61,  -61,  -61,  -61,  -61,  -61,  -61,  -61,
 /*   160 */   -61,  -61,  -61,  -61,  -61,  -61,  -61,  -61,  375,  -61,
 /*   170 */   -61,  -61,  105,  -61,  -61,  -61,  -61,  -61,  -61,  -61,
 /*   180 */   -61,  -61,  -61,  -61,  -61,  -61,  417,  -61,  -61,  -61,
 /*   190 */   -61,  -61,  -61,   23,  -61,  147,  -61,   65,  -61,  -61,
 /*   200 */   -61,  443,  -61,  -61,  189,  -61,  527,  -61,  -61,  -61,
 /*   210 */   569,  -61,  -61,  -61,  -61,  485,  -61,  -61,  552,  -61,
 /*   220 */   -61,  -61,  -61,  -61,  536,  -61,  -61,  -61,  -61,  -61,
 /*   230 */   -61,  -61,  -61,  -61,  -61,  -61,  -61,  -61,  -61,  -61,
 /*   240 */   -61,  -61,  -61,  -61,  -61,  -61,  -61,   -3,  -61,  -61,
 /*   250 */   -61,  478,  -61,  -61,  675,  -61,  -61,  732,  -61,  -61,
 /*   260 */   523,  -61,  -61,  541,  -61,  -61,  551,  -61,  -61,  565,
 /*   270 */   -61,  -61,  -61,  -61,  -61,  -61,  -61,  -61,  -61,  578,
 /*   280 */   -61, 1135, 1374,  -61,  -61,  -61,  -61,  -61, 1075, 1374,
 /*   290 */   -61,  -61,  -61,  -61, 1015, 1374,  -61,  -61,  -61,  985,
 /*   300 */  1374,  -61,  -31,  -61,  -61,   28,  -61,  -61,  -61,  196,
 /*   310 */   -61,  -61,  -61,  410,  -61,  -61,  -61,  -61,  -61,  -61,
 /*   320 */   -61,  -61,  -61,  -61,  -61,  -61,  -61,  -61,  -61,  -61,
 /*   330 */   -61,  -61,  -61,   67,  -61, 1045,  -61, 1374,  -61,  -61,
 /*   340 */   -61, 1315,  -61, 1374,  -61,  -61,  -61,  -61,  -61,  -61,
 /*   350 */  1225,  -61, 1374,  -61,  -61,  -61,  -61,  -61,  -61, 1105,
 /*   360 */   -61, 1374,  -61,  366,    3,  -61,  -61,  -61,  -61,  326,
 /*   370 */   -61,  -61,  -61,  -61,  448,  -61,  -61,  -61,  -61,  -61,
 /*   380 */   -61,  -61,  -61,  629,  -61,  -61,  -61,  -61,  -61,  671,
 /*   390 */   -61,  -61,  -61,  -61,  -61,  695,  -61,  -61,  -61,  -61,
 /*   400 */   -61,  719,  -61,  -61,  -61,  -44,  460,    3,  -61,    3,
 /*   410 */   -61,    3,  -61,  -61, 1413,  -61,  -61,  -61,  -61,  -61,
 /*   420 */   -61,  955,  -61,  -61,  -61,  -61,  -61, 1435,  -61,  -61,
 /*   430 */   -61,  -61,
};
static YYACTIONTYPE yy_default[] = {
 /*     0 */   630,  630,  630,  434,  436,  437,  438,  630,  630,  439,
 /*    10 */   630,  630,  630,  630,  440,  444,  630,  630,  469,  630,
 /*    20 */   630,  630,  630,  630,  630,  473,  630,  477,  500,  502,
 /*    30 */   503,  504,  505,  506,  507,  508,  509,  510,  511,  512,
 /*    40 */   513,  514,  515,  516,  630,  517,  630,  518,  630,  630,
 /*    50 */   630,  630,  521,  630,  630,  630,  522,  630,  630,  630,
 /*    60 */   525,  630,  526,  527,  630,  630,  630,  529,  630,  630,
 /*    70 */   630,  532,  630,  630,  630,  630,  534,  630,  630,  630,
 /*    80 */   630,  630,  630,  535,  630,  630,  537,  630,  538,  630,
 /*    90 */   630,  540,  570,  630,  630,  571,  630,  630,  572,  630,
 /*   100 */   630,  630,  625,  630,  626,  630,  627,  574,  630,  575,
 /*   110 */   630,  576,  630,  577,  630,  578,  630,  584,  630,  585,
 /*   120 */   630,  586,  630,  630,  579,  590,  630,  580,  630,  609,
 /*   130 */   630,  587,  630,  630,  630,  588,  630,  589,  591,  592,
 /*   140 */   593,  594,  595,  596,  630,  597,  630,  598,  630,  610,
 /*   150 */   630,  630,  612,  617,  618,  619,  620,  621,  622,  623,
 /*   160 */   624,  613,  618,  620,  619,  611,  630,  599,  630,  600,
 /*   170 */   630,  601,  630,  604,  607,  608,  630,  605,  606,  630,
 /*   180 */   630,  581,  630,  582,  630,  583,  630,  630,  602,  603,
 /*   190 */   573,  630,  630,  630,  541,  630,  630,  630,  542,  630,
 /*   200 */   630,  630,  543,  630,  630,  630,  630,  544,  630,  630,
 /*   210 */   630,  545,  546,  547,  539,  630,  630,  549,  630,  550,
 /*   220 */   552,  551,  630,  553,  630,  630,  554,  630,  630,  630,
 /*   230 */   630,  555,  630,  630,  630,  556,  630,  630,  557,  630,
 /*   240 */   630,  558,  630,  630,  559,  630,  630,  630,  630,  630,
 /*   250 */   560,  630,  630,  561,  630,  566,  568,  630,  569,  567,
 /*   260 */   630,  630,  562,  630,  630,  563,  630,  630,  564,  630,
 /*   270 */   630,  565,  630,  630,  630,  629,  501,  630,  630,  630,
 /*   280 */   630,  630,  630,  536,  533,  630,  531,  630,  630,  530,
 /*   290 */   528,  523,  519,  630,  630,  630,  524,  520,  630,  630,
 /*   300 */   630,  478,  630,  485,  487,  630,  490,  630,  488,  630,
 /*   310 */   491,  630,  489,  630,  492,  630,  630,  630,  630,  630,
 /*   320 */   630,  630,  630,  630,  486,  630,  630,  630,  474,  630,
 /*   330 */   630,  630,  630,  630,  630,  630,  475,  630,  479,  630,
 /*   340 */   630,  630,  476,  630,  480,  630,  630,  630,  630,  630,
 /*   350 */   630,  482,  630,  484,  630,  630,  630,  630,  630,  630,
 /*   360 */   481,  630,  483,  630,  630,  467,  630,  630,  630,  630,
 /*   370 */   630,  470,  630,  630,  630,  630,  471,  472,  457,  630,
 /*   380 */   630,  630,  459,  630,  630,  462,  630,  630,  464,  630,
 /*   390 */   630,  466,  630,  630,  460,  630,  630,  461,  630,  630,
 /*   400 */   463,  630,  630,  465,  468,  630,  630,  630,  630,  630,
 /*   410 */   458,  630,  630,  630,  630,  630,  441,  445,  630,  630,
 /*   420 */   630,  630,  630,  442,  446,  630,  630,  630,  630,  443,
 /*   430 */   447,  435,
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
  "WHILE",         "FOR",           "IN",            "LET",         
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
  "xx_let_assignment",  "xx_assign_expr",  "xx_index_expr",  "xx_echo_expressions",
  "xx_echo_expression",  "xx_mcall_expr",  "xx_call_parameters",  "xx_declare_variable_list",
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
 /* 104 */ "xx_for_statement ::= FOR IDENTIFIER COMMA IDENTIFIER IN xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 105 */ "xx_let_statement ::= LET xx_let_assignments DOTCOMMA",
 /* 106 */ "xx_let_assignments ::= xx_let_assignments COMMA xx_let_assignment",
 /* 107 */ "xx_let_assignments ::= xx_let_assignment",
 /* 108 */ "xx_let_assignment ::= IDENTIFIER ASSIGN xx_assign_expr",
 /* 109 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER ASSIGN xx_assign_expr",
 /* 110 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE ASSIGN xx_assign_expr",
 /* 111 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER SBRACKET_OPEN xx_index_expr SBRACKET_CLOSE ASSIGN xx_assign_expr",
 /* 112 */ "xx_let_assignment ::= IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE ASSIGN xx_assign_expr",
 /* 113 */ "xx_let_assignment ::= IDENTIFIER SBRACKET_OPEN xx_index_expr SBRACKET_CLOSE ASSIGN xx_assign_expr",
 /* 114 */ "xx_let_assignment ::= IDENTIFIER INCR",
 /* 115 */ "xx_let_assignment ::= IDENTIFIER DECR",
 /* 116 */ "xx_index_expr ::= xx_common_expr",
 /* 117 */ "xx_echo_statement ::= ECHO xx_echo_expressions DOTCOMMA",
 /* 118 */ "xx_echo_expressions ::= xx_echo_expressions COMMA xx_echo_expression",
 /* 119 */ "xx_echo_expressions ::= xx_echo_expression",
 /* 120 */ "xx_echo_expression ::= xx_common_expr",
 /* 121 */ "xx_mcall_statement ::= xx_mcall_expr DOTCOMMA",
 /* 122 */ "xx_return_statement ::= RETURN xx_common_expr DOTCOMMA",
 /* 123 */ "xx_unset_statement ::= UNSET IDENTIFIER ARROW IDENTIFIER DOTCOMMA",
 /* 124 */ "xx_unset_statement ::= UNSET IDENTIFIER SBRACKET_OPEN IDENTIFIER SBRACKET_CLOSE DOTCOMMA",
 /* 125 */ "xx_unset_statement ::= UNSET IDENTIFIER SBRACKET_OPEN INTEGER SBRACKET_CLOSE DOTCOMMA",
 /* 126 */ "xx_unset_statement ::= UNSET IDENTIFIER SBRACKET_OPEN STRING SBRACKET_CLOSE DOTCOMMA",
 /* 127 */ "xx_throw_statement ::= THROW IDENTIFIER DOTCOMMA",
 /* 128 */ "xx_throw_statement ::= THROW NEW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE DOTCOMMA",
 /* 129 */ "xx_declare_statement ::= TYPE_INTEGER xx_declare_variable_list DOTCOMMA",
 /* 130 */ "xx_declare_statement ::= TYPE_DOUBLE xx_declare_variable_list DOTCOMMA",
 /* 131 */ "xx_declare_statement ::= TYPE_STRING xx_declare_variable_list DOTCOMMA",
 /* 132 */ "xx_declare_statement ::= TYPE_BOOL xx_declare_variable_list DOTCOMMA",
 /* 133 */ "xx_declare_statement ::= TYPE_VAR xx_declare_variable_list DOTCOMMA",
 /* 134 */ "xx_declare_variable_list ::= xx_declare_variable_list COMMA xx_declare_variable",
 /* 135 */ "xx_declare_variable_list ::= xx_declare_variable",
 /* 136 */ "xx_declare_variable ::= IDENTIFIER",
 /* 137 */ "xx_declare_variable ::= IDENTIFIER ASSIGN xx_literal_expr",
 /* 138 */ "xx_assign_expr ::= xx_common_expr",
 /* 139 */ "xx_common_expr ::= NOT xx_common_expr",
 /* 140 */ "xx_common_expr ::= ISSET xx_isset_expr",
 /* 141 */ "xx_common_expr ::= xx_common_expr EQUALS xx_common_expr",
 /* 142 */ "xx_common_expr ::= xx_common_expr NOTEQUALS xx_common_expr",
 /* 143 */ "xx_common_expr ::= xx_common_expr IDENTICAL xx_common_expr",
 /* 144 */ "xx_common_expr ::= xx_common_expr NOTIDENTICAL xx_common_expr",
 /* 145 */ "xx_common_expr ::= xx_common_expr LESS xx_common_expr",
 /* 146 */ "xx_common_expr ::= xx_common_expr GREATER xx_common_expr",
 /* 147 */ "xx_common_expr ::= PARENTHESES_OPEN xx_common_expr PARENTHESES_CLOSE",
 /* 148 */ "xx_common_expr ::= IDENTIFIER ARROW IDENTIFIER",
 /* 149 */ "xx_common_expr ::= IDENTIFIER SBRACKET_OPEN IDENTIFIER SBRACKET_CLOSE",
 /* 150 */ "xx_common_expr ::= IDENTIFIER SBRACKET_OPEN INTEGER SBRACKET_CLOSE",
 /* 151 */ "xx_common_expr ::= IDENTIFIER SBRACKET_OPEN STRING SBRACKET_CLOSE",
 /* 152 */ "xx_common_expr ::= xx_common_expr ADD xx_common_expr",
 /* 153 */ "xx_common_expr ::= xx_common_expr SUB xx_common_expr",
 /* 154 */ "xx_common_expr ::= xx_common_expr CONCAT xx_common_expr",
 /* 155 */ "xx_common_expr ::= INSTANCEOF xx_common_expr",
 /* 156 */ "xx_common_expr ::= FETCH IDENTIFIER COMMA xx_isset_expr",
 /* 157 */ "xx_common_expr ::= TYPEOF xx_common_expr",
 /* 158 */ "xx_common_expr ::= IDENTIFIER",
 /* 159 */ "xx_common_expr ::= INTEGER",
 /* 160 */ "xx_common_expr ::= STRING",
 /* 161 */ "xx_common_expr ::= DOUBLE",
 /* 162 */ "xx_common_expr ::= NULL",
 /* 163 */ "xx_common_expr ::= TRUE",
 /* 164 */ "xx_common_expr ::= FALSE",
 /* 165 */ "xx_common_expr ::= SBRACKET_OPEN SBRACKET_CLOSE",
 /* 166 */ "xx_common_expr ::= SBRACKET_OPEN xx_array_list SBRACKET_CLOSE",
 /* 167 */ "xx_common_expr ::= NEW IDENTIFIER",
 /* 168 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 169 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 170 */ "xx_common_expr ::= IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 171 */ "xx_common_expr ::= IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 172 */ "xx_common_expr ::= xx_mcall_expr",
 /* 173 */ "xx_mcall_expr ::= IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 174 */ "xx_mcall_expr ::= IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 175 */ "xx_call_parameters ::= xx_call_parameters COMMA xx_call_parameter",
 /* 176 */ "xx_call_parameters ::= xx_call_parameter",
 /* 177 */ "xx_call_parameter ::= xx_common_expr",
 /* 178 */ "xx_array_list ::= xx_array_list COMMA xx_array_item",
 /* 179 */ "xx_array_list ::= xx_array_item",
 /* 180 */ "xx_array_item ::= xx_array_key COLON xx_array_value",
 /* 181 */ "xx_array_item ::= xx_array_value",
 /* 182 */ "xx_array_key ::= IDENTIFIER",
 /* 183 */ "xx_array_key ::= STRING",
 /* 184 */ "xx_array_key ::= INTEGER",
 /* 185 */ "xx_array_value ::= xx_literal_expr",
 /* 186 */ "xx_array_value ::= IDENTIFIER",
 /* 187 */ "xx_literal_expr ::= INTEGER",
 /* 188 */ "xx_literal_expr ::= STRING",
 /* 189 */ "xx_literal_expr ::= DOUBLE",
 /* 190 */ "xx_literal_expr ::= NULL",
 /* 191 */ "xx_literal_expr ::= FALSE",
 /* 192 */ "xx_literal_expr ::= TRUE",
 /* 193 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN IDENTIFIER SBRACKET_CLOSE",
 /* 194 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN INTEGER SBRACKET_CLOSE",
 /* 195 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN STRING SBRACKET_CLOSE",
 /* 196 */ "xx_eval_expr ::= xx_common_expr",
 /* 197 */ "xx_comment ::= COMMENT",
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
// 758 "parser.lemon"
{
	/*if ((yypminor->yy0)) {
		if ((yypminor->yy0)->free_flag) {
			efree((yypminor->yy0)->token);
		}
		efree((yypminor->yy0));
	}*/
}
// 1774 "parser.c"
      break;
    case 69:
// 771 "parser.lemon"
{ json_object_put((yypminor->yy55)); }
// 1779 "parser.c"
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
  { 68, 1 },
  { 69, 1 },
  { 70, 2 },
  { 70, 1 },
  { 71, 1 },
  { 71, 1 },
  { 71, 1 },
  { 72, 3 },
  { 73, 5 },
  { 73, 7 },
  { 73, 6 },
  { 73, 8 },
  { 73, 4 },
  { 73, 6 },
  { 73, 5 },
  { 73, 7 },
  { 75, 1 },
  { 75, 1 },
  { 75, 1 },
  { 75, 2 },
  { 75, 2 },
  { 75, 2 },
  { 75, 2 },
  { 75, 3 },
  { 75, 3 },
  { 76, 2 },
  { 76, 1 },
  { 79, 4 },
  { 79, 3 },
  { 79, 5 },
  { 79, 6 },
  { 79, 3 },
  { 79, 4 },
  { 79, 5 },
  { 79, 6 },
  { 77, 2 },
  { 77, 1 },
  { 78, 2 },
  { 81, 6 },
  { 81, 5 },
  { 78, 1 },
  { 82, 7 },
  { 82, 8 },
  { 82, 8 },
  { 82, 9 },
  { 82, 8 },
  { 82, 9 },
  { 82, 9 },
  { 82, 10 },
  { 82, 7 },
  { 82, 8 },
  { 82, 8 },
  { 82, 9 },
  { 83, 3 },
  { 83, 1 },
  { 85, 1 },
  { 85, 2 },
  { 85, 2 },
  { 85, 3 },
  { 85, 4 },
  { 85, 4 },
  { 87, 3 },
  { 86, 1 },
  { 86, 1 },
  { 86, 1 },
  { 86, 1 },
  { 86, 1 },
  { 86, 1 },
  { 84, 2 },
  { 84, 1 },
  { 88, 1 },
  { 88, 1 },
  { 88, 1 },
  { 88, 1 },
  { 88, 1 },
  { 88, 1 },
  { 88, 1 },
  { 88, 1 },
  { 88, 1 },
  { 88, 1 },
  { 88, 1 },
  { 88, 1 },
  { 88, 1 },
  { 88, 1 },
  { 88, 1 },
  { 98, 2 },
  { 99, 2 },
  { 90, 4 },
  { 90, 7 },
  { 90, 5 },
  { 90, 9 },
  { 90, 8 },
  { 90, 8 },
  { 101, 4 },
  { 101, 5 },
  { 104, 2 },
  { 104, 1 },
  { 105, 4 },
  { 105, 3 },
  { 91, 3 },
  { 91, 4 },
  { 100, 4 },
  { 100, 5 },
  { 102, 7 },
  { 102, 9 },
  { 89, 3 },
  { 107, 3 },
  { 107, 1 },
  { 108, 3 },
  { 108, 5 },
  { 108, 7 },
  { 108, 8 },
  { 108, 5 },
  { 108, 6 },
  { 108, 2 },
  { 108, 2 },
  { 110, 1 },
  { 92, 3 },
  { 111, 3 },
  { 111, 1 },
  { 112, 1 },
  { 94, 2 },
  { 93, 3 },
  { 95, 5 },
  { 95, 6 },
  { 95, 6 },
  { 95, 6 },
  { 96, 3 },
  { 96, 7 },
  { 97, 3 },
  { 97, 3 },
  { 97, 3 },
  { 97, 3 },
  { 97, 3 },
  { 115, 3 },
  { 115, 1 },
  { 116, 1 },
  { 116, 3 },
  { 109, 1 },
  { 106, 2 },
  { 106, 2 },
  { 106, 3 },
  { 106, 3 },
  { 106, 3 },
  { 106, 3 },
  { 106, 3 },
  { 106, 3 },
  { 106, 3 },
  { 106, 3 },
  { 106, 4 },
  { 106, 4 },
  { 106, 4 },
  { 106, 3 },
  { 106, 3 },
  { 106, 3 },
  { 106, 2 },
  { 106, 4 },
  { 106, 2 },
  { 106, 1 },
  { 106, 1 },
  { 106, 1 },
  { 106, 1 },
  { 106, 1 },
  { 106, 1 },
  { 106, 1 },
  { 106, 2 },
  { 106, 3 },
  { 106, 2 },
  { 106, 4 },
  { 106, 5 },
  { 106, 4 },
  { 106, 3 },
  { 106, 1 },
  { 113, 6 },
  { 113, 5 },
  { 114, 3 },
  { 114, 1 },
  { 119, 1 },
  { 118, 3 },
  { 118, 1 },
  { 120, 3 },
  { 120, 1 },
  { 121, 1 },
  { 121, 1 },
  { 121, 1 },
  { 122, 1 },
  { 122, 1 },
  { 80, 1 },
  { 80, 1 },
  { 80, 1 },
  { 80, 1 },
  { 80, 1 },
  { 80, 1 },
  { 117, 4 },
  { 117, 4 },
  { 117, 4 },
  { 103, 1 },
  { 74, 1 },
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
	status->ret = yymsp[0].minor.yy55;
}
// 2194 "parser.c"
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
      case 116:
      case 138:
      case 172:
      case 177:
      case 185:
      case 196:
// 773 "parser.lemon"
{
	yygotominor.yy55 = yymsp[0].minor.yy55;
}
// 2225 "parser.c"
        break;
      case 2:
      case 25:
      case 35:
      case 37:
      case 68:
      case 95:
// 777 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_list(yymsp[-1].minor.yy55, yymsp[0].minor.yy55);
}
// 2237 "parser.c"
        break;
      case 3:
      case 26:
      case 36:
      case 40:
      case 54:
      case 69:
      case 96:
      case 107:
      case 119:
      case 135:
      case 176:
      case 179:
// 781 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_list(NULL, yymsp[0].minor.yy55);
}
// 2255 "parser.c"
        break;
      case 7:
// 797 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_namespace(yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(17,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2264 "parser.c"
        break;
      case 8:
// 801 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[-1].minor.yy55, 0, NULL, status->scanner_state);
  yy_destructor(20,&yymsp[-4].minor);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2274 "parser.c"
        break;
      case 9:
// 805 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class(yymsp[-5].minor.yy0, yymsp[-1].minor.yy55, 0, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(20,&yymsp[-6].minor);
  yy_destructor(23,&yymsp[-4].minor);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2285 "parser.c"
        break;
      case 10:
// 809 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[-1].minor.yy55, 1, NULL, status->scanner_state);
  yy_destructor(24,&yymsp[-5].minor);
  yy_destructor(20,&yymsp[-4].minor);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2296 "parser.c"
        break;
      case 11:
// 813 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class(yymsp[-5].minor.yy0, yymsp[-1].minor.yy55, 1, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(24,&yymsp[-7].minor);
  yy_destructor(20,&yymsp[-6].minor);
  yy_destructor(23,&yymsp[-4].minor);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2308 "parser.c"
        break;
      case 12:
// 817 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class(yymsp[-2].minor.yy0, NULL, 0, NULL, status->scanner_state);
  yy_destructor(20,&yymsp[-3].minor);
  yy_destructor(21,&yymsp[-1].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2318 "parser.c"
        break;
      case 13:
// 821 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class(yymsp[-4].minor.yy0, NULL, 0, yymsp[-2].minor.yy0, status->scanner_state);
  yy_destructor(20,&yymsp[-5].minor);
  yy_destructor(23,&yymsp[-3].minor);
  yy_destructor(21,&yymsp[-1].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2329 "parser.c"
        break;
      case 14:
// 825 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class(yymsp[-2].minor.yy0, NULL, 1, NULL, status->scanner_state);
  yy_destructor(24,&yymsp[-4].minor);
  yy_destructor(20,&yymsp[-3].minor);
  yy_destructor(21,&yymsp[-1].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2340 "parser.c"
        break;
      case 15:
// 829 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class(yymsp[-4].minor.yy0, NULL, 1, yymsp[-2].minor.yy0, status->scanner_state);
  yy_destructor(24,&yymsp[-6].minor);
  yy_destructor(20,&yymsp[-5].minor);
  yy_destructor(23,&yymsp[-3].minor);
  yy_destructor(21,&yymsp[-1].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2352 "parser.c"
        break;
      case 16:
// 833 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_definition(yymsp[0].minor.yy55, NULL, NULL, status->scanner_state);
}
// 2359 "parser.c"
        break;
      case 17:
// 837 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_definition(NULL, NULL, yymsp[0].minor.yy55, status->scanner_state);
}
// 2366 "parser.c"
        break;
      case 18:
// 841 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_definition(NULL, yymsp[0].minor.yy55, NULL, status->scanner_state);
}
// 2373 "parser.c"
        break;
      case 19:
// 845 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_definition(yymsp[-1].minor.yy55, yymsp[0].minor.yy55, NULL, status->scanner_state);
}
// 2380 "parser.c"
        break;
      case 20:
// 849 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_definition(yymsp[-1].minor.yy55, NULL, yymsp[0].minor.yy55, status->scanner_state);
}
// 2387 "parser.c"
        break;
      case 21:
// 853 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_definition(yymsp[0].minor.yy55, NULL, yymsp[-1].minor.yy55, status->scanner_state);
}
// 2394 "parser.c"
        break;
      case 22:
// 857 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_definition(NULL, yymsp[0].minor.yy55, yymsp[-1].minor.yy55, status->scanner_state);
}
// 2401 "parser.c"
        break;
      case 23:
// 861 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_definition(yymsp[-2].minor.yy55, yymsp[0].minor.yy55, yymsp[-1].minor.yy55, status->scanner_state);
}
// 2408 "parser.c"
        break;
      case 24:
// 865 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_definition(yymsp[-1].minor.yy55, yymsp[0].minor.yy55, yymsp[-2].minor.yy55, status->scanner_state);
}
// 2415 "parser.c"
        break;
      case 27:
// 877 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_property(XX_T_PUBLIC, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(26,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2424 "parser.c"
        break;
      case 28:
// 881 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_property(XX_T_PUBLIC, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(26,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2433 "parser.c"
        break;
      case 29:
// 885 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_property(XX_T_PUBLIC, yymsp[-3].minor.yy0, yymsp[-1].minor.yy55, NULL, status->scanner_state);
  yy_destructor(26,&yymsp[-4].minor);
  yy_destructor(27,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2443 "parser.c"
        break;
      case 30:
// 889 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_property(XX_T_PUBLIC, yymsp[-3].minor.yy0, yymsp[-1].minor.yy55, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(26,&yymsp[-4].minor);
  yy_destructor(27,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2453 "parser.c"
        break;
      case 31:
// 893 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_property(XX_T_PROTECTED, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2462 "parser.c"
        break;
      case 32:
// 897 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_property(XX_T_PROTECTED, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(28,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2471 "parser.c"
        break;
      case 33:
// 901 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_property(XX_T_PROTECTED, yymsp[-3].minor.yy0, yymsp[-1].minor.yy55, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-4].minor);
  yy_destructor(27,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2481 "parser.c"
        break;
      case 34:
// 905 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_property(XX_T_PROTECTED, yymsp[-3].minor.yy0, yymsp[-1].minor.yy55, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(28,&yymsp[-4].minor);
  yy_destructor(27,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2491 "parser.c"
        break;
      case 38:
// 921 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy55, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(29,&yymsp[-4].minor);
  yy_destructor(27,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2501 "parser.c"
        break;
      case 39:
// 925 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy55, NULL, status->scanner_state);
  yy_destructor(29,&yymsp[-4].minor);
  yy_destructor(27,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2511 "parser.c"
        break;
      case 41:
// 933 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-4].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(26,&yymsp[-6].minor);
  yy_destructor(30,&yymsp[-5].minor);
  yy_destructor(31,&yymsp[-3].minor);
  yy_destructor(32,&yymsp[-2].minor);
  yy_destructor(21,&yymsp[-1].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2524 "parser.c"
        break;
      case 42:
// 937 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-5].minor.yy0, yymsp[-3].minor.yy55, NULL, NULL, status->scanner_state);
  yy_destructor(26,&yymsp[-7].minor);
  yy_destructor(30,&yymsp[-6].minor);
  yy_destructor(31,&yymsp[-4].minor);
  yy_destructor(32,&yymsp[-2].minor);
  yy_destructor(21,&yymsp[-1].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2537 "parser.c"
        break;
      case 43:
// 941 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-4].minor.yy0, NULL, NULL, yymsp[-7].minor.yy0, status->scanner_state);
  yy_destructor(26,&yymsp[-6].minor);
  yy_destructor(30,&yymsp[-5].minor);
  yy_destructor(31,&yymsp[-3].minor);
  yy_destructor(32,&yymsp[-2].minor);
  yy_destructor(21,&yymsp[-1].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2550 "parser.c"
        break;
      case 44:
// 945 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-5].minor.yy0, yymsp[-3].minor.yy55, NULL, yymsp[-8].minor.yy0, status->scanner_state);
  yy_destructor(26,&yymsp[-7].minor);
  yy_destructor(30,&yymsp[-6].minor);
  yy_destructor(31,&yymsp[-4].minor);
  yy_destructor(32,&yymsp[-2].minor);
  yy_destructor(21,&yymsp[-1].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2563 "parser.c"
        break;
      case 45:
// 949 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy55, NULL, status->scanner_state);
  yy_destructor(26,&yymsp[-7].minor);
  yy_destructor(30,&yymsp[-6].minor);
  yy_destructor(31,&yymsp[-4].minor);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2576 "parser.c"
        break;
      case 46:
// 953 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-6].minor.yy0, yymsp[-3].minor.yy55, yymsp[-1].minor.yy55, NULL, status->scanner_state);
  yy_destructor(26,&yymsp[-8].minor);
  yy_destructor(30,&yymsp[-7].minor);
  yy_destructor(31,&yymsp[-5].minor);
  yy_destructor(32,&yymsp[-4].minor);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2589 "parser.c"
        break;
      case 47:
// 957 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy55, yymsp[-8].minor.yy0, status->scanner_state);
  yy_destructor(26,&yymsp[-7].minor);
  yy_destructor(30,&yymsp[-6].minor);
  yy_destructor(31,&yymsp[-4].minor);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2602 "parser.c"
        break;
      case 48:
// 961 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-6].minor.yy0, yymsp[-4].minor.yy55, yymsp[-1].minor.yy55, yymsp[-9].minor.yy0, status->scanner_state);
  yy_destructor(26,&yymsp[-8].minor);
  yy_destructor(30,&yymsp[-7].minor);
  yy_destructor(31,&yymsp[-5].minor);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2615 "parser.c"
        break;
      case 49:
// 965 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_method(XX_T_PROTECTED, yymsp[-4].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-6].minor);
  yy_destructor(30,&yymsp[-5].minor);
  yy_destructor(31,&yymsp[-3].minor);
  yy_destructor(32,&yymsp[-2].minor);
  yy_destructor(21,&yymsp[-1].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2628 "parser.c"
        break;
      case 50:
// 969 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_method(XX_T_PROTECTED, yymsp[-4].minor.yy0, NULL, NULL, yymsp[-7].minor.yy0, status->scanner_state);
  yy_destructor(28,&yymsp[-6].minor);
  yy_destructor(30,&yymsp[-5].minor);
  yy_destructor(31,&yymsp[-3].minor);
  yy_destructor(32,&yymsp[-2].minor);
  yy_destructor(21,&yymsp[-1].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2641 "parser.c"
        break;
      case 51:
// 973 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_method(XX_T_PROTECTED, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy55, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-7].minor);
  yy_destructor(30,&yymsp[-6].minor);
  yy_destructor(31,&yymsp[-4].minor);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2654 "parser.c"
        break;
      case 52:
// 977 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_method(XX_T_PROTECTED, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy55, yymsp[-8].minor.yy0, status->scanner_state);
  yy_destructor(28,&yymsp[-7].minor);
  yy_destructor(30,&yymsp[-6].minor);
  yy_destructor(31,&yymsp[-4].minor);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2667 "parser.c"
        break;
      case 53:
      case 106:
      case 118:
      case 134:
      case 175:
      case 178:
// 981 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_list(yymsp[-2].minor.yy55, yymsp[0].minor.yy55);
  yy_destructor(1,&yymsp[-1].minor);
}
// 2680 "parser.c"
        break;
      case 55:
// 989 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_parameter(NULL, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 2687 "parser.c"
        break;
      case 56:
// 993 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_parameter(yymsp[-1].minor.yy55, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 2694 "parser.c"
        break;
      case 57:
// 997 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_parameter(NULL, yymsp[-1].minor.yy55, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 2701 "parser.c"
        break;
      case 58:
// 1001 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_parameter(NULL, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy55, status->scanner_state);
  yy_destructor(27,&yymsp[-1].minor);
}
// 2709 "parser.c"
        break;
      case 59:
// 1005 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_parameter(yymsp[-3].minor.yy55, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy55, status->scanner_state);
  yy_destructor(27,&yymsp[-1].minor);
}
// 2717 "parser.c"
        break;
      case 60:
// 1009 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_parameter(NULL, yymsp[-3].minor.yy55, yymsp[-2].minor.yy0, yymsp[0].minor.yy55, status->scanner_state);
  yy_destructor(27,&yymsp[-1].minor);
}
// 2725 "parser.c"
        break;
      case 61:
// 1013 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(4,&yymsp[-2].minor);
  yy_destructor(5,&yymsp[0].minor);
}
// 2734 "parser.c"
        break;
      case 62:
// 1017 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_type(XX_TYPE_INTEGER);
  yy_destructor(33,&yymsp[0].minor);
}
// 2742 "parser.c"
        break;
      case 63:
// 1021 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_type(XX_TYPE_DOUBLE);
  yy_destructor(34,&yymsp[0].minor);
}
// 2750 "parser.c"
        break;
      case 64:
// 1025 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_type(XX_TYPE_BOOL);
  yy_destructor(35,&yymsp[0].minor);
}
// 2758 "parser.c"
        break;
      case 65:
// 1029 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_type(XX_TYPE_STRING);
  yy_destructor(36,&yymsp[0].minor);
}
// 2766 "parser.c"
        break;
      case 66:
// 1033 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_type(XX_TYPE_ARRAY);
  yy_destructor(37,&yymsp[0].minor);
}
// 2774 "parser.c"
        break;
      case 67:
// 1037 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_type(XX_TYPE_VAR);
  yy_destructor(38,&yymsp[0].minor);
}
// 2782 "parser.c"
        break;
      case 85:
// 1109 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_break_statement(status->scanner_state);
  yy_destructor(39,&yymsp[-1].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2791 "parser.c"
        break;
      case 86:
// 1113 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_continue_statement(status->scanner_state);
  yy_destructor(40,&yymsp[-1].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2800 "parser.c"
        break;
      case 87:
// 1117 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_if_statement(yymsp[-2].minor.yy55, NULL, NULL, status->scanner_state);
  yy_destructor(41,&yymsp[-3].minor);
  yy_destructor(21,&yymsp[-1].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2810 "parser.c"
        break;
      case 88:
// 1121 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_if_statement(yymsp[-5].minor.yy55, NULL, NULL, status->scanner_state);
  yy_destructor(41,&yymsp[-6].minor);
  yy_destructor(21,&yymsp[-4].minor);
  yy_destructor(22,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(21,&yymsp[-1].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2823 "parser.c"
        break;
      case 89:
// 1125 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_if_statement(yymsp[-3].minor.yy55, yymsp[-1].minor.yy55, NULL, status->scanner_state);
  yy_destructor(41,&yymsp[-4].minor);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2833 "parser.c"
        break;
      case 90:
// 1129 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_if_statement(yymsp[-7].minor.yy55, yymsp[-5].minor.yy55, yymsp[-1].minor.yy55, status->scanner_state);
  yy_destructor(41,&yymsp[-8].minor);
  yy_destructor(21,&yymsp[-6].minor);
  yy_destructor(22,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-3].minor);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2846 "parser.c"
        break;
      case 91:
// 1133 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_if_statement(yymsp[-6].minor.yy55, yymsp[-4].minor.yy55, NULL, status->scanner_state);
  yy_destructor(41,&yymsp[-7].minor);
  yy_destructor(21,&yymsp[-5].minor);
  yy_destructor(22,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(21,&yymsp[-1].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2859 "parser.c"
        break;
      case 92:
// 1137 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_if_statement(yymsp[-6].minor.yy55, NULL, yymsp[-1].minor.yy55, status->scanner_state);
  yy_destructor(41,&yymsp[-7].minor);
  yy_destructor(21,&yymsp[-5].minor);
  yy_destructor(22,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-3].minor);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2872 "parser.c"
        break;
      case 93:
// 1141 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_switch_statement(yymsp[-2].minor.yy55, NULL, status->scanner_state);
  yy_destructor(43,&yymsp[-3].minor);
  yy_destructor(21,&yymsp[-1].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2882 "parser.c"
        break;
      case 94:
// 1145 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_switch_statement(yymsp[-3].minor.yy55, yymsp[-1].minor.yy55, status->scanner_state);
  yy_destructor(43,&yymsp[-4].minor);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2892 "parser.c"
        break;
      case 97:
// 1157 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_case_clause(yymsp[-2].minor.yy55, yymsp[0].minor.yy55, status->scanner_state);
  yy_destructor(44,&yymsp[-3].minor);
  yy_destructor(45,&yymsp[-1].minor);
}
// 2901 "parser.c"
        break;
      case 98:
// 1161 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_case_clause(NULL, yymsp[0].minor.yy55, status->scanner_state);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[-1].minor);
}
// 2910 "parser.c"
        break;
      case 99:
// 1165 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_loop_statement(NULL, status->scanner_state);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(21,&yymsp[-1].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2920 "parser.c"
        break;
      case 100:
// 1169 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_loop_statement(yymsp[-1].minor.yy55, status->scanner_state);
  yy_destructor(47,&yymsp[-3].minor);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2930 "parser.c"
        break;
      case 101:
// 1173 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_while_statement(yymsp[-2].minor.yy55, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-3].minor);
  yy_destructor(21,&yymsp[-1].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2940 "parser.c"
        break;
      case 102:
// 1177 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_while_statement(yymsp[-3].minor.yy55, yymsp[-1].minor.yy55, status->scanner_state);
  yy_destructor(48,&yymsp[-4].minor);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2950 "parser.c"
        break;
      case 103:
// 1181 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_for_statement(yymsp[-3].minor.yy55, NULL, yymsp[-5].minor.yy0, yymsp[-1].minor.yy55, status->scanner_state);
  yy_destructor(49,&yymsp[-6].minor);
  yy_destructor(50,&yymsp[-4].minor);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2961 "parser.c"
        break;
      case 104:
// 1185 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_for_statement(yymsp[-3].minor.yy55, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-1].minor.yy55, status->scanner_state);
  yy_destructor(49,&yymsp[-8].minor);
  yy_destructor(1,&yymsp[-6].minor);
  yy_destructor(50,&yymsp[-4].minor);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2973 "parser.c"
        break;
      case 105:
// 1189 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_let_statement(yymsp[-1].minor.yy55, status->scanner_state);
  yy_destructor(51,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2982 "parser.c"
        break;
      case 108:
// 1202 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_let_assignment("variable", yymsp[-2].minor.yy0, NULL, NULL, yymsp[0].minor.yy55, status->scanner_state);
  yy_destructor(27,&yymsp[-1].minor);
}
// 2990 "parser.c"
        break;
      case 109:
// 1207 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_let_assignment("object-property", yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy55, status->scanner_state);
  yy_destructor(52,&yymsp[-3].minor);
  yy_destructor(27,&yymsp[-1].minor);
}
// 2999 "parser.c"
        break;
      case 110:
// 1212 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_let_assignment("object-property-append", yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy55, status->scanner_state);
  yy_destructor(52,&yymsp[-5].minor);
  yy_destructor(53,&yymsp[-3].minor);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(27,&yymsp[-1].minor);
}
// 3010 "parser.c"
        break;
      case 111:
// 1217 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_let_assignment("object-property-array-index", yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy55, yymsp[0].minor.yy55, status->scanner_state);
  yy_destructor(52,&yymsp[-6].minor);
  yy_destructor(53,&yymsp[-4].minor);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(27,&yymsp[-1].minor);
}
// 3021 "parser.c"
        break;
      case 112:
// 1222 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_let_assignment("variable-append", yymsp[-4].minor.yy0, NULL, NULL, yymsp[0].minor.yy55, status->scanner_state);
  yy_destructor(53,&yymsp[-3].minor);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(27,&yymsp[-1].minor);
}
// 3031 "parser.c"
        break;
      case 113:
// 1227 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_let_assignment("array-index", yymsp[-5].minor.yy0, NULL, yymsp[-3].minor.yy55, yymsp[0].minor.yy55, status->scanner_state);
  yy_destructor(53,&yymsp[-4].minor);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(27,&yymsp[-1].minor);
}
// 3041 "parser.c"
        break;
      case 114:
// 1232 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_let_assignment("incr", yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(55,&yymsp[0].minor);
}
// 3049 "parser.c"
        break;
      case 115:
// 1237 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_let_assignment("decr", yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(56,&yymsp[0].minor);
}
// 3057 "parser.c"
        break;
      case 117:
// 1245 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_echo_statement(yymsp[-1].minor.yy55, status->scanner_state);
  yy_destructor(57,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 3066 "parser.c"
        break;
      case 120:
// 1257 "parser.lemon"
{
	yygotominor.yy55 = yymsp[0].minor.yy55;;
}
// 3073 "parser.c"
        break;
      case 121:
// 1262 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_mcall_statement(yymsp[-1].minor.yy55, status->scanner_state);
  yy_destructor(19,&yymsp[0].minor);
}
// 3081 "parser.c"
        break;
      case 122:
// 1267 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_return_statement(yymsp[-1].minor.yy55, status->scanner_state);
  yy_destructor(58,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 3090 "parser.c"
        break;
      case 123:
// 1272 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(59,&yymsp[-4].minor);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 3104 "parser.c"
        break;
      case 124:
// 1281 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(59,&yymsp[-5].minor);
  yy_destructor(53,&yymsp[-3].minor);
  yy_destructor(54,&yymsp[-1].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 3119 "parser.c"
        break;
      case 125:
// 1290 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_INTEGER, yymsp[-2].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(59,&yymsp[-5].minor);
  yy_destructor(53,&yymsp[-3].minor);
  yy_destructor(54,&yymsp[-1].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 3134 "parser.c"
        break;
      case 126:
// 1299 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_STRING, yymsp[-2].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(59,&yymsp[-5].minor);
  yy_destructor(53,&yymsp[-3].minor);
  yy_destructor(54,&yymsp[-1].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 3149 "parser.c"
        break;
      case 127:
// 1308 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_throw_exception(yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(62,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 3158 "parser.c"
        break;
      case 128:
// 1313 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_throw_exception(yymsp[-4].minor.yy0, yymsp[-2].minor.yy55, status->scanner_state);
  yy_destructor(62,&yymsp[-6].minor);
  yy_destructor(15,&yymsp[-5].minor);
  yy_destructor(31,&yymsp[-3].minor);
  yy_destructor(32,&yymsp[-1].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 3170 "parser.c"
        break;
      case 129:
// 1317 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_declare_statement(XX_T_TYPE_INTEGER, yymsp[-1].minor.yy55, status->scanner_state);
  yy_destructor(33,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 3179 "parser.c"
        break;
      case 130:
// 1321 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_declare_statement(XX_T_TYPE_DOUBLE, yymsp[-1].minor.yy55, status->scanner_state);
  yy_destructor(34,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 3188 "parser.c"
        break;
      case 131:
// 1325 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_declare_statement(XX_T_TYPE_STRING, yymsp[-1].minor.yy55, status->scanner_state);
  yy_destructor(36,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 3197 "parser.c"
        break;
      case 132:
// 1329 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_declare_statement(XX_T_TYPE_BOOL, yymsp[-1].minor.yy55, status->scanner_state);
  yy_destructor(35,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 3206 "parser.c"
        break;
      case 133:
// 1333 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_declare_statement(XX_T_TYPE_VAR, yymsp[-1].minor.yy55, status->scanner_state);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 3215 "parser.c"
        break;
      case 136:
// 1345 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_declare_variable(yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3222 "parser.c"
        break;
      case 137:
// 1349 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_declare_variable(yymsp[-2].minor.yy0, yymsp[0].minor.yy55, status->scanner_state);
  yy_destructor(27,&yymsp[-1].minor);
}
// 3230 "parser.c"
        break;
      case 139:
// 1357 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("not", yymsp[0].minor.yy55, NULL, NULL, status->scanner_state);
  yy_destructor(16,&yymsp[-1].minor);
}
// 3238 "parser.c"
        break;
      case 140:
// 1361 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("isset", yymsp[0].minor.yy55, NULL, NULL, status->scanner_state);
  yy_destructor(11,&yymsp[-1].minor);
}
// 3246 "parser.c"
        break;
      case 141:
// 1365 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("equals", yymsp[-2].minor.yy55, yymsp[0].minor.yy55, NULL, status->scanner_state);
  yy_destructor(2,&yymsp[-1].minor);
}
// 3254 "parser.c"
        break;
      case 142:
// 1369 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("not-equals", yymsp[-2].minor.yy55, yymsp[0].minor.yy55, NULL, status->scanner_state);
  yy_destructor(7,&yymsp[-1].minor);
}
// 3262 "parser.c"
        break;
      case 143:
// 1373 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("identical", yymsp[-2].minor.yy55, yymsp[0].minor.yy55, NULL, status->scanner_state);
  yy_destructor(3,&yymsp[-1].minor);
}
// 3270 "parser.c"
        break;
      case 144:
// 1377 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("not-identical", yymsp[-2].minor.yy55, yymsp[0].minor.yy55, NULL, status->scanner_state);
  yy_destructor(6,&yymsp[-1].minor);
}
// 3278 "parser.c"
        break;
      case 145:
// 1381 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("less", yymsp[-2].minor.yy55, yymsp[0].minor.yy55, NULL, status->scanner_state);
  yy_destructor(4,&yymsp[-1].minor);
}
// 3286 "parser.c"
        break;
      case 146:
// 1385 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("greater", yymsp[-2].minor.yy55, yymsp[0].minor.yy55, NULL, status->scanner_state);
  yy_destructor(5,&yymsp[-1].minor);
}
// 3294 "parser.c"
        break;
      case 147:
// 1389 "parser.lemon"
{
	yygotominor.yy55 = yymsp[-1].minor.yy55;
  yy_destructor(31,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3303 "parser.c"
        break;
      case 148:
// 1393 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(52,&yymsp[-1].minor);
}
// 3311 "parser.c"
        break;
      case 149:
      case 193:
// 1397 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(54,&yymsp[0].minor);
}
// 3321 "parser.c"
        break;
      case 150:
      case 194:
// 1402 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_INTEGER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(54,&yymsp[0].minor);
}
// 3331 "parser.c"
        break;
      case 151:
      case 195:
// 1407 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_STRING, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(54,&yymsp[0].minor);
}
// 3341 "parser.c"
        break;
      case 152:
// 1412 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("add", yymsp[-2].minor.yy55, yymsp[0].minor.yy55, NULL, status->scanner_state);
  yy_destructor(8,&yymsp[-1].minor);
}
// 3349 "parser.c"
        break;
      case 153:
// 1417 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("sub", yymsp[-2].minor.yy55, yymsp[0].minor.yy55, NULL, status->scanner_state);
  yy_destructor(9,&yymsp[-1].minor);
}
// 3357 "parser.c"
        break;
      case 154:
// 1422 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("concat", yymsp[-2].minor.yy55, yymsp[0].minor.yy55, NULL, status->scanner_state);
  yy_destructor(10,&yymsp[-1].minor);
}
// 3365 "parser.c"
        break;
      case 155:
// 1427 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("instanceof", yymsp[0].minor.yy55, NULL, NULL, status->scanner_state);
  yy_destructor(14,&yymsp[-1].minor);
}
// 3373 "parser.c"
        break;
      case 156:
// 1432 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("fetch", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy55, NULL, status->scanner_state);
  yy_destructor(12,&yymsp[-3].minor);
  yy_destructor(1,&yymsp[-1].minor);
}
// 3382 "parser.c"
        break;
      case 157:
// 1437 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("typeof", yymsp[0].minor.yy55, NULL, NULL, status->scanner_state);
  yy_destructor(13,&yymsp[-1].minor);
}
// 3390 "parser.c"
        break;
      case 158:
      case 182:
      case 186:
// 1442 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state);
}
// 3399 "parser.c"
        break;
      case 159:
      case 184:
      case 187:
// 1447 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_literal(XX_T_INTEGER, yymsp[0].minor.yy0, status->scanner_state);
}
// 3408 "parser.c"
        break;
      case 160:
      case 183:
      case 188:
// 1452 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_literal(XX_T_STRING, yymsp[0].minor.yy0, status->scanner_state);
}
// 3417 "parser.c"
        break;
      case 161:
      case 189:
// 1457 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_literal(XX_T_DOUBLE, yymsp[0].minor.yy0, status->scanner_state);
}
// 3425 "parser.c"
        break;
      case 162:
      case 190:
// 1462 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_literal(XX_T_NULL, NULL, status->scanner_state);
  yy_destructor(64,&yymsp[0].minor);
}
// 3434 "parser.c"
        break;
      case 163:
      case 192:
// 1467 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_literal(XX_T_TRUE, NULL, status->scanner_state);
  yy_destructor(65,&yymsp[0].minor);
}
// 3443 "parser.c"
        break;
      case 164:
      case 191:
// 1472 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_literal(XX_T_FALSE, NULL, status->scanner_state);
  yy_destructor(66,&yymsp[0].minor);
}
// 3452 "parser.c"
        break;
      case 165:
// 1477 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("empty-array", NULL, NULL, NULL, status->scanner_state);
  yy_destructor(53,&yymsp[-1].minor);
  yy_destructor(54,&yymsp[0].minor);
}
// 3461 "parser.c"
        break;
      case 166:
// 1482 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("array", yymsp[-1].minor.yy55, NULL, NULL, status->scanner_state);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(54,&yymsp[0].minor);
}
// 3470 "parser.c"
        break;
      case 167:
// 1487 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_new_instance(yymsp[0].minor.yy0, NULL, status->scanner_state);
  yy_destructor(15,&yymsp[-1].minor);
}
// 3478 "parser.c"
        break;
      case 168:
// 1492 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_new_instance(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(15,&yymsp[-3].minor);
  yy_destructor(31,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3488 "parser.c"
        break;
      case 169:
// 1497 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_new_instance(yymsp[-3].minor.yy0, yymsp[-1].minor.yy55, status->scanner_state);
  yy_destructor(15,&yymsp[-4].minor);
  yy_destructor(31,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3498 "parser.c"
        break;
      case 170:
// 1502 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_fcall(yymsp[-3].minor.yy0, yymsp[-1].minor.yy55, status->scanner_state);
  yy_destructor(31,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3507 "parser.c"
        break;
      case 171:
// 1507 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_fcall(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(31,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3516 "parser.c"
        break;
      case 173:
// 1517 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_mcall(yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy55, status->scanner_state);
  yy_destructor(52,&yymsp[-4].minor);
  yy_destructor(31,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3526 "parser.c"
        break;
      case 174:
// 1522 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_mcall(yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(52,&yymsp[-3].minor);
  yy_destructor(31,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3536 "parser.c"
        break;
      case 180:
// 1546 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_array_item(yymsp[-2].minor.yy55, yymsp[0].minor.yy55, status->scanner_state);
  yy_destructor(45,&yymsp[-1].minor);
}
// 3544 "parser.c"
        break;
      case 181:
// 1550 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_array_item(NULL, yymsp[0].minor.yy55, status->scanner_state);
}
// 3551 "parser.c"
        break;
      case 197:
// 1617 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_comment(yymsp[0].minor.yy0, status->scanner_state);
}
// 3558 "parser.c"
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

// 3634 "parser.c"
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
