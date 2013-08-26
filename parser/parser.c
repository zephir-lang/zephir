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
		case XX_T_CONSTANT:
			json_object_object_add(ret, "type", json_object_new_string("constant"));
			break;
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


// 747 "parser.c"
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
#define YYNOCODE 153
#define YYACTIONTYPE unsigned short int
#define xx_TOKENTYPE xx_parser_token*
typedef union {
  xx_TOKENTYPE yy0;
  json_object* yy159;
  int yy305;
} YYMINORTYPE;
#define YYSTACKDEPTH 100
#define xx_ARG_SDECL xx_parser_status *status;
#define xx_ARG_PDECL ,xx_parser_status *status
#define xx_ARG_FETCH xx_parser_status *status = yypParser->status
#define xx_ARG_STORE yypParser->status = status
#define YYNSTATE 474
#define YYNRULE 230
#define YYERRORSYMBOL 89
#define YYERRSYMDT yy305
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
 /*     0 */   322,   12,   74,  454,  351,  345,  465,  325,  179,  383,
 /*    10 */   384,  385,  386,  199,  200,  286,  310,  316,  313,  690,
 /*    20 */   319,   45,   47,   49,   99,   58,  181,  182,  244,   69,
 /*    30 */    73,   78,  199,  200,   85,  183,  184,  186,  185,  494,
 /*    40 */   289,  388,  411,  381,  417,  248,  259,  262,  387,  389,
 /*    50 */   277,  295,  298,  301,  304,  307,  178,  322,  218,   50,
 /*    60 */   146,  343,  345,  297,  325,   20,  436,  437,  438,  439,
 /*    70 */   199,  200,  286,  310,  316,  313,   64,  319,   45,   47,
 /*    80 */    49,  144,   58,   86,  247,  153,   69,   73,   78,  199,
 /*    90 */   200,   85,  458,  199,  200,  203,  491,   88,  388,  423,
 /*   100 */   202,  417,  248,  259,  262,  387,  389,  277,  295,  298,
 /*   110 */   301,  304,  307,  174,  322,  463,   59,  467,  338,  345,
 /*   120 */    87,  325,  332,  436,  437,  438,  439,  199,  200,  286,
 /*   130 */   310,  316,  313,  253,  319,   45,   47,   49,   81,   58,
 /*   140 */   252,  199,  200,   69,   73,   78,  199,  200,   85,   62,
 /*   150 */   350,  179,  291,   15,  391,  388,  423,  406,  417,  248,
 /*   160 */   259,  262,  387,  389,  277,  295,  298,  301,  304,  307,
 /*   170 */   198,  322,  127,  129,  131,  344,   80,  173,  325,  233,
 /*   180 */   436,  437,  438,  439,  471,   63,  286,  310,  316,  313,
 /*   190 */   197,  319,   45,   47,   49,  172,   58,  191,  176,  187,
 /*   200 */    69,   73,   78,  264,  267,   85,  424,   65,  435,  347,
 /*   210 */   490,  391,  388,  423,  406,  417,  248,  259,  262,  387,
 /*   220 */   389,  277,  295,  298,  301,  304,  307,  198,  322,  280,
 /*   230 */    61,  413,   77,  278,  469,  325,  198,  436,  437,  438,
 /*   240 */   439,  279,  207,  286,  310,  316,  313,  204,  319,   45,
 /*   250 */    47,   49,   65,   58,  347,  414,  215,   69,   73,   78,
 /*   260 */   358,  415,   85,  380,  369,  373,   97,  466,  416,  388,
 /*   270 */   423,  360,  417,  248,  259,  262,  387,  389,  277,  295,
 /*   280 */   298,  301,  304,  307,  198,  322,  365,  369,  373,  397,
 /*   290 */   209,  211,  325,  198,  436,  437,  438,  439,  219,  418,
 /*   300 */   286,  310,  316,  313,  221,  319,   45,   47,   49,  234,
 /*   310 */    58,  287,  294,  284,   69,   73,   78,  400,  289,   85,
 /*   320 */   380,  369,  373,  364,  472,  281,  388,  423,  381,  417,
 /*   330 */   248,  259,  262,  387,  389,  277,  295,  298,  301,  304,
 /*   340 */   307,  288,  322,  359,   18,   19,  363,  223,  406,  325,
 /*   350 */   392,  436,  437,  438,  439,  199,  200,  286,  310,  316,
 /*   360 */   313,  106,  319,   45,   47,   49,   99,   58,  296,  294,
 /*   370 */   240,   69,   73,   78,  199,  200,   85,    3,    4,    5,
 /*   380 */     6,  459,  251,  388,  423,  527,  417,  248,  259,  262,
 /*   390 */   387,  389,  277,  295,  298,  301,  304,  307,  364,  322,
 /*   400 */   419,  299,  294,  399,  102,  250,  325,  527,  383,  384,
 /*   410 */   385,  386,  199,  200,  286,  310,  316,  313,  401,  319,
 /*   420 */    45,   47,   49,   99,   58,  302,  294,  235,   69,   73,
 /*   430 */    78,  199,  200,   85,  236,  289,  289,  421,  497,  241,
 /*   440 */   388,  390,  199,  200,  248,  259,  262,  387,  389,  277,
 /*   450 */   295,  298,  301,  304,  307,  420,  322,  282,  300,  303,
 /*   460 */   352,  148,   75,  325,  208,  383,  384,  385,  386,  199,
 /*   470 */   200,  286,  310,  316,  313,  468,  319,   45,   47,   49,
 /*   480 */    99,   58,  305,  294,  231,   69,   73,   78,  199,  200,
 /*   490 */    85,  236,  289,  308,  294,  498,  232,  388,  390,  199,
 /*   500 */   200,  248,  259,  262,  387,  389,  277,  295,  298,  301,
 /*   510 */   304,  307,   55,  322,  177,  306,  289,   27,  260,  323,
 /*   520 */   325,  256,  383,  384,  385,  386,  199,  200,  286,  310,
 /*   530 */   316,  313,  430,  319,   45,   47,   49,   99,   58,  309,
 /*   540 */   217,  227,   69,   73,   78,  199,  200,   85,  367,  157,
 /*   550 */   179,  528,  493,  289,  388,  390,  529,  268,  248,  259,
 /*   560 */   262,  387,  389,  277,  295,  298,  301,  304,  307,  285,
 /*   570 */   322,  311,  294,  528,  403,  114,  312,  325,  529,  383,
 /*   580 */   384,  385,  386,  199,  200,  286,  310,  316,  313,  324,
 /*   590 */   319,   45,   47,   49,  328,   58,  175,  176,  187,   69,
 /*   600 */    73,   78,  199,  200,   85,  271,  274,  289,  368,  492,
 /*   610 */   289,  388,  390,  314,  294,  248,  259,  262,  387,  389,
 /*   620 */   277,  295,  298,  301,  304,  307,  428,  322,  317,  294,
 /*   630 */   315,   84,  118,  318,  325,  429,  436,  437,  438,  439,
 /*   640 */   199,  200,  286,  310,  316,  313,  193,  319,   45,   47,
 /*   650 */    49,   99,   58,  320,  294,   98,   69,   73,   78,  199,
 /*   660 */   200,   85,  335,  289,  370,  530,  495,  152,  388,  450,
 /*   670 */   199,  200,  248,  259,  262,  387,  389,  277,  295,  298,
 /*   680 */   301,  304,  307,  426,  322,  371,  321,  530,   25,  340,
 /*   690 */   427,  325,  258,  383,  384,  385,  386,  199,  200,  286,
 /*   700 */   310,  316,  313,  442,  319,   45,   47,   49,  136,   58,
 /*   710 */   443,  372,  210,   69,   73,   78,  199,  200,   85,  134,
 /*   720 */   151,  374,  375,  496,  239,  388,  390,  199,  200,  248,
 /*   730 */   259,  262,  387,  389,  277,  295,  298,  301,  304,  307,
 /*   740 */    70,  322,   51,  376,  290,  346,  132,   60,  325,  378,
 /*   750 */   383,  384,  385,  386,  199,  200,  286,  310,  316,  313,
 /*   760 */   194,  319,   45,   47,   49,  130,   58,  379,  542,   14,
 /*   770 */    69,   73,   78,  199,  200,   85,   48,  431,  432,  433,
 /*   780 */   434,   67,  388,  158,  382,  412,  248,  259,  262,  387,
 /*   790 */   389,  277,  295,  298,  301,  304,  307,  292,  322,  333,
 /*   800 */   444,  212,  361,  441,  475,  325,  334,  436,  437,  438,
 /*   810 */   439,  445,  213,  286,  310,  316,  313,  263,  319,   45,
 /*   820 */    47,   49,  293,   58,  242,   46,   11,   69,   73,   78,
 /*   830 */     7,   66,   85,   10,  147,    9,   54,  460,  325,  388,
 /*   840 */   150,  206,  412,  248,  259,  262,  387,  389,  277,  295,
 /*   850 */   298,  301,  304,  307,  265,  322,    8,  394,  225,  405,
 /*   860 */   243,  217,  325,  266,  383,  384,  385,  386,  461,  108,
 /*   870 */   286,  310,  316,  313,   90,  319,   45,   47,   49,  226,
 /*   880 */    58,  393,  113,  269,   69,   73,   78,  408,  688,   85,
 /*   890 */   128,  462,  409,  410,   19,  396,  388,  406,  199,  200,
 /*   900 */   248,  259,  262,  387,  389,  277,  295,  298,  301,  304,
 /*   910 */   307,  270,  322,  111,   89,  159,  331,  110,  112,  325,
 /*   920 */   705,    1,    2,  473,    4,    5,    6,  286,  310,  316,
 /*   930 */   313,  456,  319,   45,   47,   49,  126,   58,  348,  272,
 /*   940 */   455,   69,   73,   78,  199,  200,   85,  188,  431,  432,
 /*   950 */   433,  434,   91,  180,  474,  109,  273,  248,  259,  262,
 /*   960 */   402,  107,  277,  295,  298,  301,  304,  307,  689,  322,
 /*   970 */    79,  105,  230,   72,  425,  275,  325,  276,   24,   92,
 /*   980 */    93,   94,   95,   96,  286,  310,  316,  313,  353,  319,
 /*   990 */    45,   47,   49,  354,   58,  190,  189,   22,   69,   73,
 /*  1000 */    78,  181,  182,   85,  183,  184,  186,  185,   21,  122,
 /*  1010 */   183,  184,  186,  185,  248,  259,  262,  199,  200,  277,
 /*  1020 */   295,  298,  301,  304,  307,  384,  322,  384,  384,  384,
 /*  1030 */    53,  124,  384,  325,  384,  436,  437,  438,  439,  199,
 /*  1040 */   200,  286,  310,  316,  313,  384,  319,   45,   47,   49,
 /*  1050 */   384,   58,  384,  384,  153,   69,   73,   78,  384,  384,
 /*  1060 */    85,  384,  199,  200,  283,  384,  384,  388,  384,  202,
 /*  1070 */   384,  248,  259,  262,  387,  389,  277,  295,  298,  301,
 /*  1080 */   304,  307,  384,  322,  384,  116,  384,   57,  384,  384,
 /*  1090 */   325,  384,   91,  199,  200,  384,  384,  384,  286,  310,
 /*  1100 */   316,  313,  384,  319,   45,   47,   49,  155,   58,  384,
 /*  1110 */   384,  384,   69,   73,   78,  199,  200,   85,  161,   92,
 /*  1120 */    93,   94,   95,   96,  384,  228,  199,  200,  248,  259,
 /*  1130 */   262,  384,  384,  277,  295,  298,  301,  304,  307,  384,
 /*  1140 */   322,  120,  384,  384,  357,  253,  384,  325,  384,  199,
 /*  1150 */   200,  249,  254,  199,  200,  286,  310,  316,  313,  384,
 /*  1160 */   319,   45,   47,   49,  384,   58,  384,  384,  384,   69,
 /*  1170 */    73,   78,    7,  384,   85,   10,  384,  384,  384,  460,
 /*  1180 */   325,  384,  384,  384,  384,  248,  259,  262,  384,  384,
 /*  1190 */   277,  295,  298,  301,  304,  307,  384,  322,  384,  384,
 /*  1200 */   384,  356,  384,  384,  325,  384,  384,  384,  384,  384,
 /*  1210 */   384,  384,  286,  310,  316,  313,  384,  319,   45,   47,
 /*  1220 */    49,  153,   58,  377,  153,  384,   69,   73,   78,  199,
 /*  1230 */   200,   85,  199,  200,  214,  384,  201,  384,   23,  202,
 /*  1240 */   366,  384,  248,  259,  262,  384,  322,  277,  295,  298,
 /*  1250 */   301,  304,  307,  325,  384,  138,  139,  140,  141,  142,
 /*  1260 */   143,  286,  310,  316,  313,  384,  319,   45,   47,   49,
 /*  1270 */   377,   58,  384,  384,  153,   69,   73,   78,  384,  384,
 /*  1280 */    85,  384,  199,  200,  220,  395,  384,  366,  384,  202,
 /*  1290 */   384,  248,  259,  262,  384,  384,  277,  295,  298,  301,
 /*  1300 */   304,  307,  138,  139,  140,  141,  142,  143,  384,  384,
 /*  1310 */   384,  104,  156,  160,  154,  192,  101,  384,  384,  149,
 /*  1320 */   384,  384,  384,  384,   44,  384,  384,  384,  384,  162,
 /*  1330 */   384,  137,  384,  384,  138,  139,  140,  141,  142,  143,
 /*  1340 */   384,  384,   28,   29,   30,   31,   32,   33,   34,   35,
 /*  1350 */    36,   37,   38,   39,   40,   41,   42,   43,  384,  384,
 /*  1360 */   170,  384,  384,  384,  384,  188,  384,  163,  164,  255,
 /*  1370 */   257,  384,   44,  153,  384,  165,  166,  167,  168,  169,
 /*  1380 */   384,  199,  200,  196,  384,  404,  384,  384,  202,  384,
 /*  1390 */   326,   29,   30,   31,   32,   33,   34,   35,   36,   37,
 /*  1400 */    38,   39,   40,   41,   42,   43,  384,  171,   44,  384,
 /*  1410 */   384,  384,  384,  190,  189,  384,  384,  255,  257,  384,
 /*  1420 */   384,   83,  183,  184,  186,  185,  326,   29,   30,   31,
 /*  1430 */    32,   33,   34,   35,   36,   37,   38,   39,   40,   41,
 /*  1440 */    42,   43,  464,   16,  447,  453,  452,  440,  384,  435,
 /*  1450 */   446,  410,   19,  255,  257,  406,  384,   44,  100,  115,
 /*  1460 */   119,  121,  123,  125,  117,  103,  127,  129,  131,  384,
 /*  1470 */    26,  384,  384,  384,  384,  326,   29,   30,   31,   32,
 /*  1480 */    33,   34,   35,   36,   37,   38,   39,   40,   41,   42,
 /*  1490 */    43,   44,  384,  384,  384,  384,  384,  384,  384,   91,
 /*  1500 */   384,  384,  255,  257,  342,  384,  384,  384,  384,  326,
 /*  1510 */    29,   30,   31,   32,   33,   34,   35,   36,   37,   38,
 /*  1520 */    39,   40,   41,   42,   43,   44,   92,   93,   94,   95,
 /*  1530 */    96,  224,  237,  384,  245,  246,  255,  257,   56,  384,
 /*  1540 */   384,  384,  377,  326,   29,   30,   31,   32,   33,   34,
 /*  1550 */    35,   36,   37,   38,   39,   40,   41,   42,   43,  366,
 /*  1560 */   384,  384,  449,  422,  440,  384,  435,   44,  410,   19,
 /*  1570 */   255,  257,  406,  384,  138,  139,  140,  141,  142,  143,
 /*  1580 */   337,  384,  384,  384,  384,  326,   29,   30,   31,   32,
 /*  1590 */    33,   34,   35,   36,   37,   38,   39,   40,   41,   42,
 /*  1600 */    43,   44,  384,  384,  384,  384,  384,  384,  384,  384,
 /*  1610 */   384,  384,  255,  257,   52,  384,  384,  384,  384,  326,
 /*  1620 */    29,   30,   31,   32,   33,   34,   35,   36,   37,   38,
 /*  1630 */    39,   40,   41,   42,   43,   44,  384,  384,  384,  384,
 /*  1640 */   384,  384,  384,  384,  384,  384,  255,  257,  398,  384,
 /*  1650 */   384,  384,  384,  326,   29,   30,   31,   32,   33,   34,
 /*  1660 */    35,   36,   37,   38,   39,   40,   41,   42,   43,  384,
 /*  1670 */   384,  384,  384,  384,  384,  384,  384,   44,  384,  384,
 /*  1680 */   255,  257,  384,  384,  384,  384,  384,  384,  384,  384,
 /*  1690 */   349,  384,  384,  384,  384,  326,   29,   30,   31,   32,
 /*  1700 */    33,   34,   35,   36,   37,   38,   39,   40,   41,   42,
 /*  1710 */    43,   44,  384,  384,  384,  384,  384,  384,  384,  384,
 /*  1720 */   384,  384,  255,  257,  362,  384,  384,  384,  384,  326,
 /*  1730 */    29,   30,   31,   32,   33,   34,   35,   36,   37,   38,
 /*  1740 */    39,   40,   41,   42,   43,   44,  384,  384,  384,  384,
 /*  1750 */   384,  384,  384,  384,  384,  384,  255,  257,   68,  384,
 /*  1760 */   384,  384,  384,  326,   29,   30,   31,   32,   33,   34,
 /*  1770 */    35,   36,   37,   38,   39,   40,   41,   42,   43,  384,
 /*  1780 */   384,  384,  384,  384,  384,  384,  384,   44,  384,  384,
 /*  1790 */   255,  257,  384,  384,  384,  384,  384,  384,  384,  384,
 /*  1800 */    76,  384,  384,  384,  384,  326,   29,   30,   31,   32,
 /*  1810 */    33,   34,   35,   36,   37,   38,   39,   40,   41,   42,
 /*  1820 */    43,   44,  384,  384,  384,  384,  384,  384,  384,  384,
 /*  1830 */   384,  384,  255,  257,   71,  384,  384,  384,  384,  326,
 /*  1840 */    29,   30,   31,   32,   33,   34,   35,   36,   37,   38,
 /*  1850 */    39,   40,   41,   42,   43,   44,  384,  384,  384,  384,
 /*  1860 */   384,  384,  384,  384,  384,  384,  255,  257,  330,  384,
 /*  1870 */   384,  384,  384,  326,   29,   30,   31,   32,   33,   34,
 /*  1880 */    35,   36,   37,   38,   39,   40,   41,   42,   43,  384,
 /*  1890 */   384,  384,  384,  384,  384,  384,  384,   44,  384,  384,
 /*  1900 */   255,  257,  384,  384,  384,  384,  384,  384,  384,  384,
 /*  1910 */   355,  384,  384,  384,  384,  326,   29,   30,   31,   32,
 /*  1920 */    33,   34,   35,   36,   37,   38,   39,   40,   41,   42,
 /*  1930 */    43,  384,  384,  384,  384,  384,  104,  156,  160,  154,
 /*  1940 */   192,  101,  255,  257,  149,  384,  384,  384,  384,  384,
 /*  1950 */   384,  384,  384,  384,  162,  384,  137,  104,  156,  160,
 /*  1960 */   154,  192,  101,  195,  384,  149,  384,  384,  384,  384,
 /*  1970 */   384,  384,  384,  384,  384,  162,  384,  137,  384,  384,
 /*  1980 */   384,  384,  384,  384,  384,  170,  229,  384,  384,  384,
 /*  1990 */   384,  384,  163,  164,  384,  384,  384,  384,  384,  384,
 /*  2000 */   165,  166,  167,  168,  169,  384,  170,  384,  384,  384,
 /*  2010 */   384,  384,  384,  163,  164,  384,  384,  384,  384,  384,
 /*  2020 */   384,  165,  166,  167,  168,  169,  104,  156,  160,  154,
 /*  2030 */   192,  101,  205,  384,  149,  384,  384,  384,  384,  384,
 /*  2040 */   384,  384,  384,  384,  162,  384,  137,  104,  156,  160,
 /*  2050 */   154,  192,  101,  384,  384,  149,  384,  384,  384,  384,
 /*  2060 */   384,  384,  384,  384,  384,  162,  384,  137,  384,  384,
 /*  2070 */   384,  384,  384,  384,  384,  170,  384,  384,  384,  384,
 /*  2080 */   384,  384,  163,  164,  384,  384,  384,  384,  327,  384,
 /*  2090 */   165,  166,  167,  168,  169,  384,  170,  384,  384,  384,
 /*  2100 */   384,  384,  384,  163,  164,  384,  384,  384,  384,  384,
 /*  2110 */   384,  165,  166,  167,  168,  169,  104,  156,  160,  154,
 /*  2120 */   192,  101,  222,  448,  149,  451,  452,  440,  384,  435,
 /*  2130 */   409,  410,   19,  384,  162,  406,  137,  104,  156,  160,
 /*  2140 */   154,  192,  101,  216,  384,  149,  384,  384,  384,  384,
 /*  2150 */   384,  384,  384,  384,  384,  162,  384,  137,  104,  156,
 /*  2160 */   160,  154,  192,  101,  384,  170,  149,  384,  384,  384,
 /*  2170 */   384,  384,  163,  164,  384,  384,  162,  384,  137,  384,
 /*  2180 */   165,  166,  167,  168,  169,  384,  170,  384,  384,  384,
 /*  2190 */   384,  384,  384,  163,  164,  384,  384,  384,  384,  384,
 /*  2200 */   384,  165,  166,  167,  168,  169,  384,  170,  238,  384,
 /*  2210 */   384,  384,  384,  384,  163,  164,  384,  384,  384,  384,
 /*  2220 */   384,  384,  165,  166,  167,  168,  169,  104,  156,  160,
 /*  2230 */   154,  192,  101,  384,  384,  149,  384,  384,  384,  384,
 /*  2240 */   384,  384,  384,  384,  384,  162,  384,  137,  384,  104,
 /*  2250 */   156,  160,  154,  192,  101,  384,  384,  149,  384,  384,
 /*  2260 */   384,  384,  384,  384,  384,  384,  384,  162,  339,  137,
 /*  2270 */   384,  384,  384,  384,  384,  384,  170,  384,  384,  384,
 /*  2280 */   384,  384,  384,  163,  164,  384,  384,  384,  384,  384,
 /*  2290 */   384,  165,  166,  167,  168,  169,  384,  384,  170,  384,
 /*  2300 */   384,  384,  384,  384,  384,  163,  164,  384,  384,  384,
 /*  2310 */   384,  384,  384,  165,  166,  167,  168,  169,  384,  384,
 /*  2320 */   133,  135,  100,  115,  119,  121,  123,  125,  117,  103,
 /*  2330 */   127,  129,  131,  470,   16,  447,  453,  452,  440,  384,
 /*  2340 */   435,  446,  410,   19,  341,  384,  406,  384,  384,  384,
 /*  2350 */   384,  133,  135,  100,  115,  119,  121,  123,  125,  117,
 /*  2360 */   103,  127,  129,  131,  384,  384,  384,  384,  384,  384,
 /*  2370 */   384,  384,  384,  384,  384,   82,  384,  384,  384,  384,
 /*  2380 */   384,  384,  133,  135,  100,  115,  119,  121,  123,  125,
 /*  2390 */   117,  103,  127,  129,  131,  133,  135,  100,  115,  119,
 /*  2400 */   121,  123,  125,  117,  103,  127,  129,  131,  384,  384,
 /*  2410 */   384,  384,  384,  384,  384,  384,  384,  261,  384,  384,
 /*  2420 */   384,  384,  384,  384,  133,  135,  100,  115,  119,  121,
 /*  2430 */   123,  125,  117,  103,  127,  129,  131,  384,  384,  384,
 /*  2440 */   384,  384,  384,  384,  384,  606,  384,  384,  329,  384,
 /*  2450 */   384,  384,  133,  135,  100,  115,  119,  121,  123,  125,
 /*  2460 */   117,  103,  127,  129,  131,  384,  384,  384,  384,  384,
 /*  2470 */   384,  384,  384,  384,  384,  384,  702,  384,  384,  384,
 /*  2480 */   384,  384,  384,  133,  135,  100,  115,  119,  121,  123,
 /*  2490 */   125,  117,  103,  127,  129,  131,  384,  384,  384,  384,
 /*  2500 */   384,  384,  145,  384,  384,  384,  384,  384,  384,  133,
 /*  2510 */   135,  100,  115,  119,  121,  123,  125,  117,  103,  127,
 /*  2520 */   129,  131,  457,   16,  447,  453,  452,  440,  384,  435,
 /*  2530 */   446,  410,   19,  336,  384,  406,  384,  384,  384,  384,
 /*  2540 */   133,  135,  100,  115,  119,  121,  123,  125,  117,  103,
 /*  2550 */   127,  129,  131,   13,   16,  447,  453,  452,  440,  384,
 /*  2560 */   435,  446,  410,   19,  384,  384,  406,  384,  384,  384,
 /*  2570 */   384,  384,  384,  384,  384,  384,  384,  384,  384,  384,
 /*  2580 */   384,  384,  384,  407,   17,  422,  440,  384,  435,  446,
 /*  2590 */   410,   19,  384,  384,  406,
};
static YYCODETYPE yy_lookahead[] = {
 /*     0 */    27,   30,  130,   32,   31,  133,   31,   34,  103,    1,
 /*    10 */     2,    3,    4,  141,  142,   42,   43,   44,   45,   54,
 /*    20 */    47,   48,   49,   50,  133,   52,   75,   76,  137,   56,
 /*    30 */    57,   58,  141,  142,   61,   84,   85,   86,   87,   31,
 /*    40 */     5,   33,   34,   16,   36,   72,   73,   74,   40,   41,
 /*    50 */    77,   78,   79,   80,   81,   82,  151,   27,   27,  130,
 /*    60 */   112,   31,  133,   28,   34,   38,    1,    2,    3,    4,
 /*    70 */   141,  142,   42,   43,   44,   45,  132,   47,   48,   49,
 /*    80 */    50,  133,   52,  134,  135,  133,   56,   57,   58,  141,
 /*    90 */   142,   61,   31,  141,  142,  143,   31,    5,   33,   34,
 /*   100 */   148,   36,   72,   73,   74,   40,   41,   77,   78,   79,
 /*   110 */    80,   81,   82,    5,   27,   30,  130,   32,   31,  133,
 /*   120 */    28,   34,    5,    1,    2,    3,    4,  141,  142,   42,
 /*   130 */    43,   44,   45,  133,   47,   48,   49,   50,  133,   52,
 /*   140 */   140,  141,  142,   56,   57,   58,  141,  142,   61,  131,
 /*   150 */   132,  103,   27,   31,  107,   33,   34,  110,   36,   72,
 /*   160 */    73,   74,   40,   41,   77,   78,   79,   80,   81,   82,
 /*   170 */     5,   27,   16,   17,   18,   31,   59,   69,   34,   69,
 /*   180 */     1,    2,    3,    4,   31,   31,   42,   43,   44,   45,
 /*   190 */    25,   47,   48,   49,   50,  147,   52,  149,  150,  151,
 /*   200 */    56,   57,   58,   67,   68,   61,  102,   53,  104,   55,
 /*   210 */    31,  107,   33,   34,  110,   36,   72,   73,   74,   40,
 /*   220 */    41,   77,   78,   79,   80,   81,   82,    5,   27,   23,
 /*   230 */    31,   37,   31,   27,   30,   34,    5,    1,    2,    3,
 /*   240 */     4,   28,   27,   42,   43,   44,   45,   25,   47,   48,
 /*   250 */    49,   50,   53,   52,   55,   35,   25,   56,   57,   58,
 /*   260 */   108,  103,   61,  111,  112,  113,  136,   31,   28,   33,
 /*   270 */    34,   30,   36,   72,   73,   74,   40,   41,   77,   78,
 /*   280 */    79,   80,   81,   82,    5,   27,  111,  112,  113,   31,
 /*   290 */    75,   76,   34,    5,    1,    2,    3,    4,   39,   37,
 /*   300 */    42,   43,   44,   45,   25,   47,   48,   49,   50,  136,
 /*   310 */    52,  144,  145,   25,   56,   57,   58,  108,    5,   61,
 /*   320 */   111,  112,  113,    5,   31,   27,   33,   34,   16,   36,
 /*   330 */    72,   73,   74,   40,   41,   77,   78,   79,   80,   81,
 /*   340 */    82,   28,   27,   25,  106,  107,   31,  133,  110,   34,
 /*   350 */    38,    1,    2,    3,    4,  141,  142,   42,   43,   44,
 /*   360 */    45,   27,   47,   48,   49,   50,  133,   52,  144,  145,
 /*   370 */   137,   56,   57,   58,  141,  142,   61,   93,   94,   95,
 /*   380 */    96,   31,    5,   33,   34,   16,   36,   72,   73,   74,
 /*   390 */    40,   41,   77,   78,   79,   80,   81,   82,    5,   27,
 /*   400 */    35,  144,  145,   31,  133,   28,   34,   38,    1,    2,
 /*   410 */     3,    4,  141,  142,   42,   43,   44,   45,   25,   47,
 /*   420 */    48,   49,   50,  133,   52,  144,  145,  137,   56,   57,
 /*   430 */    58,  141,  142,   61,  133,    5,    5,   28,   31,  138,
 /*   440 */    33,   34,  141,  142,   72,   73,   74,   40,   41,   77,
 /*   450 */    78,   79,   80,   81,   82,  103,   27,   39,   28,   28,
 /*   460 */    31,  133,   30,   34,   69,    1,    2,    3,    4,  141,
 /*   470 */   142,   42,   43,   44,   45,   27,   47,   48,   49,   50,
 /*   480 */   133,   52,  144,  145,  137,   56,   57,   58,  141,  142,
 /*   490 */    61,  133,    5,  144,  145,   31,  138,   33,   34,  141,
 /*   500 */   142,   72,   73,   74,   40,   41,   77,   78,   79,   80,
 /*   510 */    81,   82,   30,   27,   54,   28,    5,   31,  133,   67,
 /*   520 */    34,   28,    1,    2,    3,    4,  141,  142,   42,   43,
 /*   530 */    44,   45,  104,   47,   48,   49,   50,  133,   52,   28,
 /*   540 */    88,  137,   56,   57,   58,  141,  142,   61,   35,   27,
 /*   550 */   103,   16,   31,    5,   33,   34,   16,   27,   72,   73,
 /*   560 */    74,   40,   41,   77,   78,   79,   80,   81,   82,   28,
 /*   570 */    27,  144,  145,   38,   31,  133,   28,   34,   38,    1,
 /*   580 */     2,    3,    4,  141,  142,   42,   43,   44,   45,   27,
 /*   590 */    47,   48,   49,   50,  133,   52,  149,  150,  151,   56,
 /*   600 */    57,   58,  141,  142,   61,   75,   76,    5,  103,   31,
 /*   610 */     5,   33,   34,  144,  145,   72,   73,   74,   40,   41,
 /*   620 */    77,   78,   79,   80,   81,   82,  103,   27,  144,  145,
 /*   630 */    28,   31,  133,   28,   34,   28,    1,    2,    3,    4,
 /*   640 */   141,  142,   42,   43,   44,   45,   27,   47,   48,   49,
 /*   650 */    50,  133,   52,  144,  145,  137,   56,   57,   58,  141,
 /*   660 */   142,   61,  133,    5,   27,   16,   31,   39,   33,   34,
 /*   670 */   141,  142,   72,   73,   74,   40,   41,   77,   78,   79,
 /*   680 */    80,   81,   82,   28,   27,   35,   28,   38,   31,  133,
 /*   690 */    35,   34,   28,    1,    2,    3,    4,  141,  142,   42,
 /*   700 */    43,   44,   45,   28,   47,   48,   49,   50,  133,   52,
 /*   710 */    35,  103,   69,   56,   57,   58,  141,  142,   61,  133,
 /*   720 */    27,   27,   35,   31,  136,   33,   34,  141,  142,   72,
 /*   730 */    73,   74,   40,   41,   77,   78,   79,   80,   81,   82,
 /*   740 */    30,   27,   30,  103,  145,   31,  133,   30,   34,   27,
 /*   750 */     1,    2,    3,    4,  141,  142,   42,   43,   44,   45,
 /*   760 */    39,   47,   48,   49,   50,  133,   52,   11,   27,   31,
 /*   770 */    56,   57,   58,  141,  142,   61,   28,    1,    2,    3,
 /*   780 */     4,   54,   33,    5,  110,   36,   72,   73,   74,   40,
 /*   790 */    41,   77,   78,   79,   80,   81,   82,   35,   27,   27,
 /*   800 */   103,   69,   31,   27,    0,   34,   59,    1,    2,    3,
 /*   810 */     4,   28,   39,   42,   43,   44,   45,   27,   47,   48,
 /*   820 */    49,   50,  103,   52,   69,   28,   27,   56,   57,   58,
 /*   830 */    26,  103,   61,   29,   25,   28,   51,   33,   34,   33,
 /*   840 */    67,   68,   36,   72,   73,   74,   40,   41,   77,   78,
 /*   850 */    79,   80,   81,   82,   27,   27,   27,   39,   27,   31,
 /*   860 */   136,   88,   34,   28,    1,    2,    3,    4,   29,   27,
 /*   870 */    42,   43,   44,   45,   27,   47,   48,   49,   50,  136,
 /*   880 */    52,   27,   69,   69,   56,   57,   58,  100,   54,   61,
 /*   890 */   133,   27,  105,  106,  107,   30,   33,  110,  141,  142,
 /*   900 */    72,   73,   74,   40,   41,   77,   78,   79,   80,   81,
 /*   910 */    82,   28,   27,   69,  135,  146,   31,   75,   76,   34,
 /*   920 */    90,   91,   92,   93,   94,   95,   96,   42,   43,   44,
 /*   930 */    45,   30,   47,   48,   49,   50,  133,   52,   54,   69,
 /*   940 */    27,   56,   57,   58,  141,  142,   61,   27,    1,    2,
 /*   950 */     3,    4,   35,   27,    0,   69,   28,   72,   73,   74,
 /*   960 */    30,   68,   77,   78,   79,   80,   81,   82,   54,   27,
 /*   970 */    27,  146,  136,   31,   27,   69,   34,   28,   30,   62,
 /*   980 */    63,   64,   65,   66,   42,   43,   44,   45,   51,   47,
 /*   990 */    48,   49,   50,   30,   52,   75,   76,   39,   56,   57,
 /*  1000 */    58,   75,   76,   61,   84,   85,   86,   87,   27,  133,
 /*  1010 */    84,   85,   86,   87,   72,   73,   74,  141,  142,   77,
 /*  1020 */    78,   79,   80,   81,   82,  152,   27,  152,  152,  152,
 /*  1030 */    31,  133,  152,   34,  152,    1,    2,    3,    4,  141,
 /*  1040 */   142,   42,   43,   44,   45,  152,   47,   48,   49,   50,
 /*  1050 */   152,   52,  152,  152,  133,   56,   57,   58,  152,  152,
 /*  1060 */    61,  152,  141,  142,  143,  152,  152,   33,  152,  148,
 /*  1070 */   152,   72,   73,   74,   40,   41,   77,   78,   79,   80,
 /*  1080 */    81,   82,  152,   27,  152,  133,  152,   31,  152,  152,
 /*  1090 */    34,  152,   35,  141,  142,  152,  152,  152,   42,   43,
 /*  1100 */    44,   45,  152,   47,   48,   49,   50,  133,   52,  152,
 /*  1110 */   152,  152,   56,   57,   58,  141,  142,   61,  133,   62,
 /*  1120 */    63,   64,   65,   66,  152,   68,  141,  142,   72,   73,
 /*  1130 */    74,  152,  152,   77,   78,   79,   80,   81,   82,  152,
 /*  1140 */    27,  133,  152,  152,   31,  133,  152,   34,  152,  141,
 /*  1150 */   142,  139,  140,  141,  142,   42,   43,   44,   45,  152,
 /*  1160 */    47,   48,   49,   50,  152,   52,  152,  152,  152,   56,
 /*  1170 */    57,   58,   26,  152,   61,   29,  152,  152,  152,   33,
 /*  1180 */    34,  152,  152,  152,  152,   72,   73,   74,  152,  152,
 /*  1190 */    77,   78,   79,   80,   81,   82,  152,   27,  152,  152,
 /*  1200 */   152,   31,  152,  152,   34,  152,  152,  152,  152,  152,
 /*  1210 */   152,  152,   42,   43,   44,   45,  152,   47,   48,   49,
 /*  1220 */    50,  133,   52,   10,  133,  152,   56,   57,   58,  141,
 /*  1230 */   142,   61,  141,  142,  143,  152,  148,  152,   25,  148,
 /*  1240 */    27,  152,   72,   73,   74,  152,   27,   77,   78,   79,
 /*  1250 */    80,   81,   82,   34,  152,   42,   43,   44,   45,   46,
 /*  1260 */    47,   42,   43,   44,   45,  152,   47,   48,   49,   50,
 /*  1270 */    10,   52,  152,  152,  133,   56,   57,   58,  152,  152,
 /*  1280 */    61,  152,  141,  142,  143,   25,  152,   27,  152,  148,
 /*  1290 */   152,   72,   73,   74,  152,  152,   77,   78,   79,   80,
 /*  1300 */    81,   82,   42,   43,   44,   45,   46,   47,  152,  152,
 /*  1310 */   152,   19,   20,   21,   22,   23,   24,  152,  152,   27,
 /*  1320 */   152,  152,  152,  152,   96,  152,  152,  152,  152,   37,
 /*  1330 */   152,   39,  152,  152,   42,   43,   44,   45,   46,   47,
 /*  1340 */   152,  152,  114,  115,  116,  117,  118,  119,  120,  121,
 /*  1350 */   122,  123,  124,  125,  126,  127,  128,  129,  152,  152,
 /*  1360 */    68,  152,  152,  152,  152,   27,  152,   75,   76,  141,
 /*  1370 */   142,  152,   96,  133,  152,   83,   84,   85,   86,   87,
 /*  1380 */   152,  141,  142,  143,  152,  109,  152,  152,  148,  152,
 /*  1390 */   114,  115,  116,  117,  118,  119,  120,  121,  122,  123,
 /*  1400 */   124,  125,  126,  127,  128,  129,  152,   69,   96,  152,
 /*  1410 */   152,  152,  152,   75,   76,  152,  152,  141,  142,  152,
 /*  1420 */   152,  109,   84,   85,   86,   87,  114,  115,  116,  117,
 /*  1430 */   118,  119,  120,  121,  122,  123,  124,  125,  126,  127,
 /*  1440 */   128,  129,   97,   98,   99,  100,  101,  102,  152,  104,
 /*  1450 */   105,  106,  107,  141,  142,  110,  152,   96,    8,    9,
 /*  1460 */    10,   11,   12,   13,   14,   15,   16,   17,   18,  152,
 /*  1470 */   109,  152,  152,  152,  152,  114,  115,  116,  117,  118,
 /*  1480 */   119,  120,  121,  122,  123,  124,  125,  126,  127,  128,
 /*  1490 */   129,   96,  152,  152,  152,  152,  152,  152,  152,   35,
 /*  1500 */   152,  152,  141,  142,  109,  152,  152,  152,  152,  114,
 /*  1510 */   115,  116,  117,  118,  119,  120,  121,  122,  123,  124,
 /*  1520 */   125,  126,  127,  128,  129,   96,   62,   63,   64,   65,
 /*  1530 */    66,   67,   68,  152,   70,   71,  141,  142,  109,  152,
 /*  1540 */   152,  152,   10,  114,  115,  116,  117,  118,  119,  120,
 /*  1550 */   121,  122,  123,  124,  125,  126,  127,  128,  129,   27,
 /*  1560 */   152,  152,  100,  101,  102,  152,  104,   96,  106,  107,
 /*  1570 */   141,  142,  110,  152,   42,   43,   44,   45,   46,   47,
 /*  1580 */   109,  152,  152,  152,  152,  114,  115,  116,  117,  118,
 /*  1590 */   119,  120,  121,  122,  123,  124,  125,  126,  127,  128,
 /*  1600 */   129,   96,  152,  152,  152,  152,  152,  152,  152,  152,
 /*  1610 */   152,  152,  141,  142,  109,  152,  152,  152,  152,  114,
 /*  1620 */   115,  116,  117,  118,  119,  120,  121,  122,  123,  124,
 /*  1630 */   125,  126,  127,  128,  129,   96,  152,  152,  152,  152,
 /*  1640 */   152,  152,  152,  152,  152,  152,  141,  142,  109,  152,
 /*  1650 */   152,  152,  152,  114,  115,  116,  117,  118,  119,  120,
 /*  1660 */   121,  122,  123,  124,  125,  126,  127,  128,  129,  152,
 /*  1670 */   152,  152,  152,  152,  152,  152,  152,   96,  152,  152,
 /*  1680 */   141,  142,  152,  152,  152,  152,  152,  152,  152,  152,
 /*  1690 */   109,  152,  152,  152,  152,  114,  115,  116,  117,  118,
 /*  1700 */   119,  120,  121,  122,  123,  124,  125,  126,  127,  128,
 /*  1710 */   129,   96,  152,  152,  152,  152,  152,  152,  152,  152,
 /*  1720 */   152,  152,  141,  142,  109,  152,  152,  152,  152,  114,
 /*  1730 */   115,  116,  117,  118,  119,  120,  121,  122,  123,  124,
 /*  1740 */   125,  126,  127,  128,  129,   96,  152,  152,  152,  152,
 /*  1750 */   152,  152,  152,  152,  152,  152,  141,  142,  109,  152,
 /*  1760 */   152,  152,  152,  114,  115,  116,  117,  118,  119,  120,
 /*  1770 */   121,  122,  123,  124,  125,  126,  127,  128,  129,  152,
 /*  1780 */   152,  152,  152,  152,  152,  152,  152,   96,  152,  152,
 /*  1790 */   141,  142,  152,  152,  152,  152,  152,  152,  152,  152,
 /*  1800 */   109,  152,  152,  152,  152,  114,  115,  116,  117,  118,
 /*  1810 */   119,  120,  121,  122,  123,  124,  125,  126,  127,  128,
 /*  1820 */   129,   96,  152,  152,  152,  152,  152,  152,  152,  152,
 /*  1830 */   152,  152,  141,  142,  109,  152,  152,  152,  152,  114,
 /*  1840 */   115,  116,  117,  118,  119,  120,  121,  122,  123,  124,
 /*  1850 */   125,  126,  127,  128,  129,   96,  152,  152,  152,  152,
 /*  1860 */   152,  152,  152,  152,  152,  152,  141,  142,  109,  152,
 /*  1870 */   152,  152,  152,  114,  115,  116,  117,  118,  119,  120,
 /*  1880 */   121,  122,  123,  124,  125,  126,  127,  128,  129,  152,
 /*  1890 */   152,  152,  152,  152,  152,  152,  152,   96,  152,  152,
 /*  1900 */   141,  142,  152,  152,  152,  152,  152,  152,  152,  152,
 /*  1910 */   109,  152,  152,  152,  152,  114,  115,  116,  117,  118,
 /*  1920 */   119,  120,  121,  122,  123,  124,  125,  126,  127,  128,
 /*  1930 */   129,  152,  152,  152,  152,  152,   19,   20,   21,   22,
 /*  1940 */    23,   24,  141,  142,   27,  152,  152,  152,  152,  152,
 /*  1950 */   152,  152,  152,  152,   37,  152,   39,   19,   20,   21,
 /*  1960 */    22,   23,   24,   25,  152,   27,  152,  152,  152,  152,
 /*  1970 */   152,  152,  152,  152,  152,   37,  152,   39,  152,  152,
 /*  1980 */   152,  152,  152,  152,  152,   68,   69,  152,  152,  152,
 /*  1990 */   152,  152,   75,   76,  152,  152,  152,  152,  152,  152,
 /*  2000 */    83,   84,   85,   86,   87,  152,   68,  152,  152,  152,
 /*  2010 */   152,  152,  152,   75,   76,  152,  152,  152,  152,  152,
 /*  2020 */   152,   83,   84,   85,   86,   87,   19,   20,   21,   22,
 /*  2030 */    23,   24,   25,  152,   27,  152,  152,  152,  152,  152,
 /*  2040 */   152,  152,  152,  152,   37,  152,   39,   19,   20,   21,
 /*  2050 */    22,   23,   24,  152,  152,   27,  152,  152,  152,  152,
 /*  2060 */   152,  152,  152,  152,  152,   37,  152,   39,  152,  152,
 /*  2070 */   152,  152,  152,  152,  152,   68,  152,  152,  152,  152,
 /*  2080 */   152,  152,   75,   76,  152,  152,  152,  152,   60,  152,
 /*  2090 */    83,   84,   85,   86,   87,  152,   68,  152,  152,  152,
 /*  2100 */   152,  152,  152,   75,   76,  152,  152,  152,  152,  152,
 /*  2110 */   152,   83,   84,   85,   86,   87,   19,   20,   21,   22,
 /*  2120 */    23,   24,   25,   98,   27,  100,  101,  102,  152,  104,
 /*  2130 */   105,  106,  107,  152,   37,  110,   39,   19,   20,   21,
 /*  2140 */    22,   23,   24,   25,  152,   27,  152,  152,  152,  152,
 /*  2150 */   152,  152,  152,  152,  152,   37,  152,   39,   19,   20,
 /*  2160 */    21,   22,   23,   24,  152,   68,   27,  152,  152,  152,
 /*  2170 */   152,  152,   75,   76,  152,  152,   37,  152,   39,  152,
 /*  2180 */    83,   84,   85,   86,   87,  152,   68,  152,  152,  152,
 /*  2190 */   152,  152,  152,   75,   76,  152,  152,  152,  152,  152,
 /*  2200 */   152,   83,   84,   85,   86,   87,  152,   68,   69,  152,
 /*  2210 */   152,  152,  152,  152,   75,   76,  152,  152,  152,  152,
 /*  2220 */   152,  152,   83,   84,   85,   86,   87,   19,   20,   21,
 /*  2230 */    22,   23,   24,  152,  152,   27,  152,  152,  152,  152,
 /*  2240 */   152,  152,  152,  152,  152,   37,  152,   39,  152,   19,
 /*  2250 */    20,   21,   22,   23,   24,  152,  152,   27,  152,  152,
 /*  2260 */   152,  152,  152,  152,  152,  152,  152,   37,   60,   39,
 /*  2270 */   152,  152,  152,  152,  152,  152,   68,  152,  152,  152,
 /*  2280 */   152,  152,  152,   75,   76,  152,  152,  152,  152,  152,
 /*  2290 */   152,   83,   84,   85,   86,   87,  152,  152,   68,  152,
 /*  2300 */   152,  152,  152,  152,  152,   75,   76,  152,  152,  152,
 /*  2310 */   152,  152,  152,   83,   84,   85,   86,   87,  152,  152,
 /*  2320 */     6,    7,    8,    9,   10,   11,   12,   13,   14,   15,
 /*  2330 */    16,   17,   18,   97,   98,   99,  100,  101,  102,  152,
 /*  2340 */   104,  105,  106,  107,   30,  152,  110,  152,  152,  152,
 /*  2350 */   152,    6,    7,    8,    9,   10,   11,   12,   13,   14,
 /*  2360 */    15,   16,   17,   18,  152,  152,  152,  152,  152,  152,
 /*  2370 */   152,  152,  152,  152,  152,   30,  152,  152,  152,  152,
 /*  2380 */   152,  152,    6,    7,    8,    9,   10,   11,   12,   13,
 /*  2390 */    14,   15,   16,   17,   18,    6,    7,    8,    9,   10,
 /*  2400 */    11,   12,   13,   14,   15,   16,   17,   18,  152,  152,
 /*  2410 */   152,  152,  152,  152,  152,  152,  152,   28,  152,  152,
 /*  2420 */   152,  152,  152,  152,    6,    7,    8,    9,   10,   11,
 /*  2430 */    12,   13,   14,   15,   16,   17,   18,  152,  152,  152,
 /*  2440 */   152,  152,  152,  152,  152,   69,  152,  152,   30,  152,
 /*  2450 */   152,  152,    6,    7,    8,    9,   10,   11,   12,   13,
 /*  2460 */    14,   15,   16,   17,   18,  152,  152,  152,  152,  152,
 /*  2470 */   152,  152,  152,  152,  152,  152,   30,  152,  152,  152,
 /*  2480 */   152,  152,  152,    6,    7,    8,    9,   10,   11,   12,
 /*  2490 */    13,   14,   15,   16,   17,   18,  152,  152,  152,  152,
 /*  2500 */   152,  152,   25,  152,  152,  152,  152,  152,  152,    6,
 /*  2510 */     7,    8,    9,   10,   11,   12,   13,   14,   15,   16,
 /*  2520 */    17,   18,   97,   98,   99,  100,  101,  102,  152,  104,
 /*  2530 */   105,  106,  107,   30,  152,  110,  152,  152,  152,  152,
 /*  2540 */     6,    7,    8,    9,   10,   11,   12,   13,   14,   15,
 /*  2550 */    16,   17,   18,   97,   98,   99,  100,  101,  102,  152,
 /*  2560 */   104,  105,  106,  107,  152,  152,  110,  152,  152,  152,
 /*  2570 */   152,  152,  152,  152,  152,  152,  152,  152,  152,  152,
 /*  2580 */   152,  152,  152,   99,  100,  101,  102,  152,  104,  105,
 /*  2590 */   106,  107,  152,  152,  110,
};
#define YY_SHIFT_USE_DFLT (-50)
static short yy_shift_ofst[] = {
 /*     0 */  1146,  954,  804,  -50,  -50,  -50,  -50,  829,  807,  -50,
 /*    10 */   799,  -29,  122,  738,  -50,  -50,  179,  521,  -50,   27,
 /*    20 */   981,  958, 1213,  948,  657,  -50,  486,  -50,  -50,  -50,
 /*    30 */   -50,  -50,  -50,  -50,  -50,  -50,  -50,  -50,  -50,  -50,
 /*    40 */   -50,  -50,  -50,  -50,  -50,  797,  -50,  748,  -50, 2230,
 /*    50 */   712,  429,  999,  785,  482,  -27, 1056,  -50, 2230,  717,
 /*    60 */   199,  -50,  154,  -50,  -50,  -49,  727, 1219, 1219,  710,
 /*    70 */   714,  942,  -50, 2230,  432,  144,  201,  -50,  943,  117,
 /*    80 */  2028, 2345, 1219,  600,  -50,  847,   92,  -50,  847,  -50,
 /*    90 */  1464,  -50,  -50,  -50,  -50,  -50,  -50, 2230,  -50, 2534,
 /*   100 */  2230, 2230,  -50, 2230,  334,  -50,  893,  842,  886,  -50,
 /*   110 */   844,  -50,  813,  -50,  156, 2230,  156, 2230,  156, 2230,
 /*   120 */   156, 2230,  156, 2230,  156, 2230,  156, 2230,  -50, 2230,
 /*   130 */   -50, 2230,  -50, 2230, 1450, 2230, 1450, 1292,  -50,  -50,
 /*   140 */   -50,  -50,  -50,  -50, 2477,  -50,  809, 2230,  -50,  773,
 /*   150 */   693,  628, 2007, 2534, 2230,  -50,  522,  778,  334,  -50,
 /*   160 */  2230,  -50,  -50,  -50,  -50,  -50,  -50,  -50,  -50,  -50,
 /*   170 */  1338,  -50,  108,  -50,  920,  -50,  460,  926,  -50,  -50,
 /*   180 */   -50,  -50,  -50,  -50,  -50,  -50,  -50,  -50,  834,  914,
 /*   190 */   -35,  -50,  619,  721, 1938,  -50,  165,  -50, 2230,  -50,
 /*   200 */   -50,  -50,  -50,  222,  -50,  -50,  215,  395,  -50,  643,
 /*   210 */   -50,  732,  -50, 2118,  231,  -50,  -50,   31,  259, 2097,
 /*   220 */   279,  -50,  -50,  156,  831, 1057, 2230,  -50, 1917,  917,
 /*   230 */  2230,  -50,  110,  917, 2230,  -50, 2376, 2139,  917, 2230,
 /*   240 */   -50,  755,  917, 2230,  -50,  -50,  -50,  -50, 2230,  377,
 /*   250 */   -50, 2230,  -50, 2534,  -50,  493,  -50,  664,  -50, 2230,
 /*   260 */  2389,  -50,  790,  136,  827,  835,  -50,  530,  814,  883,
 /*   270 */   -50,  870,  928,  -50,  906,  949,  -50,  206,  213,  -50,
 /*   280 */   298,  418, 2230,  288,  541,  -50,  125,  313,  -50,  125,
 /*   290 */   -50,  762,  -49,  -50,  -50,  125,   35,  -50,  125,  430,
 /*   300 */   -50,  125,  431,  -50,  125,  487,  -50,  125,  511,  -50,
 /*   310 */   125,  548,  -50,  125,  602,  -50,  125,  605,  -50,  125,
 /*   320 */   658,  -50,  452,  562,  628,  -50,  -50, 2230, 2418, 1219,
 /*   330 */   885,  -50,  772,  747, 2208, 2503, 1219,   87,  -50, 2230,
 /*   340 */  2314, 1219,   30,  -50,  -50, 2446,  -50,  884, 1219, 1219,
 /*   350 */   -50,  -50,  937,  963, 1113, 1170,  -50,  -50,  318,  241,
 /*   360 */   771,  -50,  315,  -50, 1532,  -50,  513,  -49,  -50,  637,
 /*   370 */   650,  -49,  -50,  694,  687,  -49,  -50,  722,  756,  741,
 /*   380 */   -50,  863,  -50,  -50,  -50,  -50,  -50,  -50,  -50,  -50,
 /*   390 */   863,  312,  854,  818, 1260,  865,  258,  -50,  372,  -50,
 /*   400 */   393,  930,  543,  -50,  828,  -50,  -50,    8,  407,  -50,
 /*   410 */   -50,  749,  194,  220,  -49,  240,  -50,  262,  365,  -49,
 /*   420 */   409,  -50,  -50,  806,  947,  655,  -50,  -49,  607,  -50,
 /*   430 */   -50,  -50,  -50,  -50,  -50,  -50,  369,  535,  540,  649,
 /*   440 */   776,  675,  -50,  -49,  783,  -50,  -50,   65,  635,  464,
 /*   450 */  1034,  692,  -50,  578,  913,  901,  350,   61,  -50,  -50,
 /*   460 */   839,  864,   85,  236,  -25,  -50,  -50,  448,  204,  293,
 /*   470 */   153,  -50,  -50,  -50,
};
#define YY_REDUCE_USE_DFLT (-129)
static short yy_reduce_ofst[] = {
 /*     0 */   830, -129,  284, -129, -129, -129, -129, -129, -129, -129,
 /*    10 */  -129, -129, 2456, -129, -129, -129, 2484,  238, -129, -129,
 /*    20 */  -129, -129,  152, -129, 1361, -129, 1228, -129, -129, -129,
 /*    30 */  -129, -129, -129, -129, -129, -129, -129, -129, -129, -129,
 /*    40 */  -129, -129, -129, -129, -129, -129, -129, -129, -129,  -71,
 /*    50 */  -129, 1505, 1228, -129, -129, 1429, 1228, -129,  -14, -129,
 /*    60 */    18, -129,  -56, -129, -129,  728, -129, 1649, 1228, -129,
 /*    70 */  1725, 1228, -129, -128, -129, 1691, 1228, -129, -129, -129,
 /*    80 */     5, -129, 1312, 1228, -129,  -51, -129, -129,  779, -129,
 /*    90 */   130, -129, -129, -129, -129, -129, -129,  518, -129, -129,
 /*   100 */   214,  271, -129,  442,  825, -129, -129, -129, -129, -129,
 /*   110 */  -129, -129, -129, -129, -129,  952, -129,  499, -129, 1008,
 /*   120 */  -129,  876, -129,  898, -129,  803, -129,  757, -129,  632,
 /*   130 */  -129,  613, -129,  586, -129,  575, -129,  -52, -129, -129,
 /*   140 */  -129, -129, -129, -129, -129, -129, -129,  328, -129, -129,
 /*   150 */  -129, -129,  -48, -129,  974, -129, -129, -129,  769, -129,
 /*   160 */   985, -129, -129, -129, -129, -129, -129, -129, -129, -129,
 /*   170 */    48, -129, -129, -129,  447, -129, -129,  -95, -129, -129,
 /*   180 */  -129, -129, -129, -129, -129, -129, -129, -129, -129, -129,
 /*   190 */  -129, -129, -129, -129, 1240, -129, -129, -129, 1088, -129,
 /*   200 */  -129, -129, -129, -129, -129, -129, -129, -129, -129, -129,
 /*   210 */  -129, -129, -129, 1091, -129, -129, -129, -129, -129, 1141,
 /*   220 */  -129, -129, -129, -129, -129,  743,  404, -129,  358,  836,
 /*   230 */   347, -129, -129,  173,  290, -129, -129,  301,  588,  233,
 /*   240 */  -129, -129,  724, -109, -129, -129, -129, -129, 1012, -129,
 /*   250 */  -129,    0, -129, -129, -129, -129, -129, -129, -129,  385,
 /*   260 */  -129, -129, -129, -129, -129, -129, -129, -129, -129, -129,
 /*   270 */  -129, -129, -129, -129, -129, -129, -129, -129, -129, -129,
 /*   280 */  -129, -129,  921, -129, -129, -129,  167, -129, -129,  599,
 /*   290 */  -129, -129,  719, -129, -129,  224, -129, -129,  257, -129,
 /*   300 */  -129,  281, -129, -129,  338, -129, -129,  349, -129, -129,
 /*   310 */   427, -129, -129,  469, -129, -129,  484, -129, -129,  509,
 /*   320 */  -129, -129, -129, -129, -129, -129, -129,  461, -129, 1759,
 /*   330 */  1228, -129, -129, -129,  529, -129, 1471, 1228, -129,  556,
 /*   340 */  -129, 1395, 1228, -129, -129, -129, -129, -129, 1581, 1228,
 /*   350 */  -129, -129, -129, -129, 1801, 1228, -129, -129, -129, -129,
 /*   360 */  1615, -129, 1228, -129,  175, -129, -129,  505, -129, -129,
 /*   370 */  -129,  608, -129, -129, -129,  640, -129, -129, -129, -129,
 /*   380 */  -129,  674, -129, -129, -129, -129, -129, -129, -129, -129,
 /*   390 */    47, -129, -129, -129,  209, -129, 1539, -129, 1228, -129,
 /*   400 */  -129, -129, 1276, -129, 1228, -129, -129,  787,  238, -129,
 /*   410 */  -129,   47, -129, -129,  158, -129, -129, -129, -129,  352,
 /*   420 */  -129, -129, -129,  104,  428, -129, -129,  523, -129, -129,
 /*   430 */  -129, -129, -129, -129, -129, -129, -129, -129, -129, -129,
 /*   440 */   428, -129, -129,  697, -129, -129, -129, 2025, 1462,  238,
 /*   450 */   104,  238, -129,  238, -129, -129, 2425, -129, -129, -129,
 /*   460 */  -129, -129, -129, 1345, -129, -129, -129, -129, -129, 2236,
 /*   470 */  -129, -129, -129, -129,
};
static YYACTIONTYPE yy_default[] = {
 /*     0 */   704,  704,  704,  476,  478,  479,  480,  704,  704,  481,
 /*    10 */   704,  704,  704,  704,  482,  486,  704,  704,  513,  704,
 /*    20 */   704,  704,  704,  704,  704,  517,  704,  519,  549,  551,
 /*    30 */   552,  553,  554,  555,  556,  557,  558,  559,  560,  561,
 /*    40 */   562,  563,  564,  565,  566,  704,  567,  704,  568,  704,
 /*    50 */   704,  704,  704,  571,  704,  704,  704,  572,  704,  704,
 /*    60 */   704,  575,  704,  576,  577,  704,  704,  704,  579,  704,
 /*    70 */   704,  704,  582,  704,  704,  704,  704,  584,  704,  704,
 /*    80 */   704,  704,  704,  704,  585,  704,  704,  589,  704,  590,
 /*    90 */   704,  592,  593,  594,  595,  596,  597,  704,  598,  634,
 /*   100 */   704,  704,  635,  704,  704,  636,  704,  704,  704,  699,
 /*   110 */   704,  700,  704,  701,  638,  704,  639,  704,  640,  704,
 /*   120 */   641,  704,  642,  704,  643,  704,  644,  704,  651,  704,
 /*   130 */   652,  704,  653,  704,  654,  704,  655,  704,  543,  544,
 /*   140 */   545,  546,  547,  548,  704,  645,  704,  704,  646,  660,
 /*   150 */   704,  647,  704,  683,  704,  656,  704,  704,  704,  657,
 /*   160 */   704,  658,  659,  661,  662,  663,  664,  665,  666,  667,
 /*   170 */   704,  668,  704,  669,  704,  684,  704,  704,  686,  691,
 /*   180 */   692,  693,  694,  695,  696,  697,  698,  687,  692,  694,
 /*   190 */   693,  685,  704,  670,  704,  671,  704,  672,  704,  675,
 /*   200 */   676,  681,  682,  704,  679,  680,  704,  704,  648,  704,
 /*   210 */   649,  704,  650,  704,  704,  673,  674,  704,  704,  704,
 /*   220 */   704,  677,  678,  637,  704,  704,  704,  599,  704,  704,
 /*   230 */   704,  600,  704,  704,  704,  601,  704,  704,  704,  704,
 /*   240 */   602,  704,  704,  704,  603,  604,  605,  591,  704,  704,
 /*   250 */   607,  704,  608,  610,  609,  704,  611,  704,  612,  704,
 /*   260 */   704,  613,  704,  704,  704,  704,  614,  704,  704,  704,
 /*   270 */   615,  704,  704,  616,  704,  704,  617,  704,  704,  618,
 /*   280 */   704,  704,  704,  704,  704,  619,  704,  704,  620,  704,
 /*   290 */   630,  632,  704,  633,  631,  704,  704,  621,  704,  704,
 /*   300 */   622,  704,  704,  623,  704,  704,  624,  704,  704,  625,
 /*   310 */   704,  704,  626,  704,  704,  627,  704,  704,  628,  704,
 /*   320 */   704,  629,  704,  704,  704,  703,  550,  704,  704,  704,
 /*   330 */   704,  586,  704,  704,  704,  704,  704,  704,  587,  704,
 /*   340 */   704,  704,  704,  588,  583,  704,  581,  704,  704,  580,
 /*   350 */   578,  573,  569,  704,  704,  704,  574,  570,  704,  704,
 /*   360 */   704,  518,  704,  520,  704,  534,  536,  704,  539,  704,
 /*   370 */   537,  704,  540,  704,  538,  704,  541,  704,  704,  704,
 /*   380 */   535,  704,  525,  527,  528,  529,  530,  531,  532,  533,
 /*   390 */   704,  704,  704,  704,  704,  704,  704,  521,  704,  523,
 /*   400 */   704,  704,  704,  522,  704,  524,  526,  704,  704,  511,
 /*   410 */   514,  704,  704,  704,  704,  704,  515,  704,  704,  704,
 /*   420 */   704,  516,  499,  704,  704,  704,  501,  704,  704,  503,
 /*   430 */   505,  507,  508,  509,  510,  506,  507,  508,  509,  510,
 /*   440 */   704,  704,  502,  704,  704,  504,  512,  704,  704,  704,
 /*   450 */   704,  704,  500,  704,  704,  704,  704,  704,  483,  487,
 /*   460 */   704,  704,  704,  704,  704,  484,  488,  704,  704,  704,
 /*   470 */   704,  485,  489,  477,
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
  "CONST",         "CONSTANT",      "FUNCTION",      "PARENTHESES_OPEN",
  "INLINE",        "FINAL",         "TYPE_INTEGER",  "TYPE_DOUBLE", 
  "TYPE_BOOL",     "TYPE_STRING",   "TYPE_ARRAY",    "TYPE_VAR",    
  "BREAK",         "CONTINUE",      "IF",            "ELSE",        
  "SWITCH",        "CASE",          "COLON",         "DEFAULT",     
  "LOOP",          "WHILE",         "FOR",           "IN",          
  "REVERSE",       "LET",           "ADDASSIGN",     "SUBASSIGN",   
  "MULASSIGN",     "DIVASSIGN",     "CONCATASSIGN",  "ARROW",       
  "SBRACKET_OPEN",  "SBRACKET_CLOSE",  "INCR",          "DECR",        
  "ECHO",          "RETURN",        "UNSET",         "INTEGER",     
  "STRING",        "THROW",         "TYPE_UINTEGER",  "TYPE_CHAR",   
  "TYPE_UCHAR",    "TYPE_LONG",     "TYPE_ULONG",    "SCHAR",       
  "DOUBLE",        "NULL",          "TRUE",          "FALSE",       
  "DOUBLECOLON",   "error",         "program",       "xx_language", 
  "xx_top_statement_list",  "xx_top_statement",  "xx_namespace_def",  "xx_class_def",
  "xx_comment",    "xx_class_definition",  "xx_class_properties_definition",  "xx_class_consts_definition",
  "xx_class_methods_definition",  "xx_class_property_definition",  "xx_property_visibility_list",  "xx_literal_expr",
  "xx_property_visibility",  "xx_class_const_definition",  "xx_class_method_definition",  "xx_method_visibility_list",
  "xx_parameter_list",  "xx_statement_list",  "xx_method_visibility",  "xx_parameter",
  "xx_parameter_type",  "xx_parameter_cast",  "xx_statement",  "xx_let_statement",
  "xx_if_statement",  "xx_loop_statement",  "xx_echo_statement",  "xx_return_statement",
  "xx_mcall_statement",  "xx_scall_statement",  "xx_unset_statement",  "xx_throw_statement",
  "xx_declare_statement",  "xx_break_statement",  "xx_continue_statement",  "xx_while_statement",
  "xx_switch_statement",  "xx_for_statement",  "xx_eval_expr",  "xx_case_clauses",
  "xx_case_clause",  "xx_common_expr",  "xx_let_assignments",  "xx_let_assignment",
  "xx_assignment_operator",  "xx_assign_expr",  "xx_index_expr",  "xx_echo_expressions",
  "xx_echo_expression",  "xx_mcall_expr",  "xx_scall_expr",  "xx_call_parameters",
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
 /*  41 */ "xx_class_const_definition ::= COMMENT CONST CONSTANT ASSIGN xx_literal_expr DOTCOMMA",
 /*  42 */ "xx_class_const_definition ::= CONST CONSTANT ASSIGN xx_literal_expr DOTCOMMA",
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
 /* 185 */ "xx_common_expr ::= CONSTANT",
 /* 186 */ "xx_common_expr ::= IDENTIFIER",
 /* 187 */ "xx_common_expr ::= INTEGER",
 /* 188 */ "xx_common_expr ::= STRING",
 /* 189 */ "xx_common_expr ::= SCHAR",
 /* 190 */ "xx_common_expr ::= DOUBLE",
 /* 191 */ "xx_common_expr ::= NULL",
 /* 192 */ "xx_common_expr ::= TRUE",
 /* 193 */ "xx_common_expr ::= FALSE",
 /* 194 */ "xx_common_expr ::= SBRACKET_OPEN SBRACKET_CLOSE",
 /* 195 */ "xx_common_expr ::= SBRACKET_OPEN xx_array_list SBRACKET_CLOSE",
 /* 196 */ "xx_common_expr ::= NEW IDENTIFIER",
 /* 197 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 198 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 199 */ "xx_common_expr ::= IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 200 */ "xx_common_expr ::= IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 201 */ "xx_common_expr ::= xx_mcall_expr",
 /* 202 */ "xx_common_expr ::= xx_scall_expr",
 /* 203 */ "xx_scall_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 204 */ "xx_scall_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 205 */ "xx_mcall_expr ::= IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 206 */ "xx_mcall_expr ::= IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 207 */ "xx_call_parameters ::= xx_call_parameters COMMA xx_call_parameter",
 /* 208 */ "xx_call_parameters ::= xx_call_parameter",
 /* 209 */ "xx_call_parameter ::= xx_common_expr",
 /* 210 */ "xx_array_list ::= xx_array_list COMMA xx_array_item",
 /* 211 */ "xx_array_list ::= xx_array_item",
 /* 212 */ "xx_array_item ::= xx_array_key COLON xx_array_value",
 /* 213 */ "xx_array_item ::= xx_array_value",
 /* 214 */ "xx_array_key ::= IDENTIFIER",
 /* 215 */ "xx_array_key ::= STRING",
 /* 216 */ "xx_array_key ::= INTEGER",
 /* 217 */ "xx_array_value ::= xx_literal_expr",
 /* 218 */ "xx_array_value ::= IDENTIFIER",
 /* 219 */ "xx_literal_expr ::= INTEGER",
 /* 220 */ "xx_literal_expr ::= STRING",
 /* 221 */ "xx_literal_expr ::= DOUBLE",
 /* 222 */ "xx_literal_expr ::= NULL",
 /* 223 */ "xx_literal_expr ::= FALSE",
 /* 224 */ "xx_literal_expr ::= TRUE",
 /* 225 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN IDENTIFIER SBRACKET_CLOSE",
 /* 226 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN INTEGER SBRACKET_CLOSE",
 /* 227 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN STRING SBRACKET_CLOSE",
 /* 228 */ "xx_eval_expr ::= xx_common_expr",
 /* 229 */ "xx_comment ::= COMMENT",
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
    case 88:
// 817 "parser.lemon"
{
	/*if ((yypminor->yy0)) {
		if ((yypminor->yy0)->free_flag) {
			efree((yypminor->yy0)->token);
		}
		efree((yypminor->yy0));
	}*/
}
// 2054 "parser.c"
      break;
    case 91:
// 830 "parser.lemon"
{ json_object_put((yypminor->yy159)); }
// 2059 "parser.c"
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
  { 90, 1 },
  { 91, 1 },
  { 92, 2 },
  { 92, 1 },
  { 93, 1 },
  { 93, 1 },
  { 93, 1 },
  { 94, 3 },
  { 95, 5 },
  { 95, 7 },
  { 95, 6 },
  { 95, 8 },
  { 95, 4 },
  { 95, 6 },
  { 95, 5 },
  { 95, 7 },
  { 97, 1 },
  { 97, 1 },
  { 97, 1 },
  { 97, 2 },
  { 97, 2 },
  { 97, 2 },
  { 97, 2 },
  { 97, 3 },
  { 97, 3 },
  { 98, 2 },
  { 98, 1 },
  { 101, 4 },
  { 101, 3 },
  { 101, 6 },
  { 101, 5 },
  { 102, 2 },
  { 102, 1 },
  { 104, 1 },
  { 104, 1 },
  { 104, 1 },
  { 104, 1 },
  { 99, 2 },
  { 99, 1 },
  { 100, 2 },
  { 100, 1 },
  { 105, 6 },
  { 105, 5 },
  { 106, 7 },
  { 106, 8 },
  { 106, 8 },
  { 106, 9 },
  { 106, 8 },
  { 106, 9 },
  { 106, 9 },
  { 106, 10 },
  { 107, 3 },
  { 107, 1 },
  { 110, 1 },
  { 110, 1 },
  { 110, 1 },
  { 110, 1 },
  { 110, 1 },
  { 110, 1 },
  { 110, 1 },
  { 108, 3 },
  { 108, 1 },
  { 111, 1 },
  { 111, 2 },
  { 111, 2 },
  { 111, 3 },
  { 111, 4 },
  { 111, 4 },
  { 113, 3 },
  { 112, 1 },
  { 112, 1 },
  { 112, 1 },
  { 112, 1 },
  { 112, 1 },
  { 112, 1 },
  { 109, 2 },
  { 109, 1 },
  { 114, 1 },
  { 114, 1 },
  { 114, 1 },
  { 114, 1 },
  { 114, 1 },
  { 114, 1 },
  { 114, 1 },
  { 114, 1 },
  { 114, 1 },
  { 114, 1 },
  { 114, 1 },
  { 114, 1 },
  { 114, 1 },
  { 114, 1 },
  { 114, 1 },
  { 114, 1 },
  { 125, 2 },
  { 126, 2 },
  { 116, 4 },
  { 116, 7 },
  { 116, 5 },
  { 116, 9 },
  { 116, 8 },
  { 116, 8 },
  { 128, 4 },
  { 128, 5 },
  { 131, 2 },
  { 131, 1 },
  { 132, 4 },
  { 132, 3 },
  { 117, 3 },
  { 117, 4 },
  { 127, 4 },
  { 127, 5 },
  { 129, 7 },
  { 129, 8 },
  { 129, 9 },
  { 129, 10 },
  { 115, 3 },
  { 134, 3 },
  { 134, 1 },
  { 136, 1 },
  { 136, 1 },
  { 136, 1 },
  { 136, 1 },
  { 136, 1 },
  { 136, 1 },
  { 135, 3 },
  { 135, 5 },
  { 135, 7 },
  { 135, 8 },
  { 135, 5 },
  { 135, 6 },
  { 135, 2 },
  { 135, 2 },
  { 138, 1 },
  { 118, 3 },
  { 139, 3 },
  { 139, 1 },
  { 140, 1 },
  { 120, 2 },
  { 121, 2 },
  { 119, 3 },
  { 122, 5 },
  { 122, 6 },
  { 122, 6 },
  { 122, 6 },
  { 123, 3 },
  { 123, 7 },
  { 124, 3 },
  { 124, 3 },
  { 124, 3 },
  { 124, 3 },
  { 124, 3 },
  { 124, 3 },
  { 124, 3 },
  { 124, 3 },
  { 124, 3 },
  { 124, 3 },
  { 144, 3 },
  { 144, 1 },
  { 145, 1 },
  { 145, 3 },
  { 137, 1 },
  { 133, 2 },
  { 133, 2 },
  { 133, 3 },
  { 133, 3 },
  { 133, 3 },
  { 133, 3 },
  { 133, 3 },
  { 133, 3 },
  { 133, 3 },
  { 133, 3 },
  { 133, 3 },
  { 133, 4 },
  { 133, 3 },
  { 133, 4 },
  { 133, 4 },
  { 133, 4 },
  { 133, 3 },
  { 133, 3 },
  { 133, 3 },
  { 133, 3 },
  { 133, 3 },
  { 133, 2 },
  { 133, 4 },
  { 133, 2 },
  { 133, 1 },
  { 133, 1 },
  { 133, 1 },
  { 133, 1 },
  { 133, 1 },
  { 133, 1 },
  { 133, 1 },
  { 133, 1 },
  { 133, 1 },
  { 133, 2 },
  { 133, 3 },
  { 133, 2 },
  { 133, 4 },
  { 133, 5 },
  { 133, 4 },
  { 133, 3 },
  { 133, 1 },
  { 133, 1 },
  { 142, 6 },
  { 142, 5 },
  { 141, 6 },
  { 141, 5 },
  { 143, 3 },
  { 143, 1 },
  { 148, 1 },
  { 147, 3 },
  { 147, 1 },
  { 149, 3 },
  { 149, 1 },
  { 150, 1 },
  { 150, 1 },
  { 150, 1 },
  { 151, 1 },
  { 151, 1 },
  { 103, 1 },
  { 103, 1 },
  { 103, 1 },
  { 103, 1 },
  { 103, 1 },
  { 103, 1 },
  { 146, 4 },
  { 146, 4 },
  { 146, 4 },
  { 130, 1 },
  { 96, 1 },
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
// 826 "parser.lemon"
{
	status->ret = yymsp[0].minor.yy159;
}
// 2506 "parser.c"
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
      case 201:
      case 202:
      case 209:
      case 217:
      case 228:
// 832 "parser.lemon"
{
	yygotominor.yy159 = yymsp[0].minor.yy159;
}
// 2539 "parser.c"
        break;
      case 2:
      case 25:
      case 31:
      case 37:
      case 39:
      case 75:
      case 103:
// 836 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_list(yymsp[-1].minor.yy159, yymsp[0].minor.yy159);
}
// 2552 "parser.c"
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
      case 208:
      case 211:
// 840 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_list(NULL, yymsp[0].minor.yy159);
}
// 2572 "parser.c"
        break;
      case 7:
// 856 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_namespace(yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(26,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 2581 "parser.c"
        break;
      case 8:
// 860 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[-1].minor.yy159, 0, NULL, status->scanner_state);
  yy_destructor(29,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 2591 "parser.c"
        break;
      case 9:
// 864 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_class(yymsp[-5].minor.yy0, yymsp[-1].minor.yy159, 0, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(29,&yymsp[-6].minor);
  yy_destructor(32,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 2602 "parser.c"
        break;
      case 10:
// 868 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[-1].minor.yy159, 1, NULL, status->scanner_state);
  yy_destructor(33,&yymsp[-5].minor);
  yy_destructor(29,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 2613 "parser.c"
        break;
      case 11:
// 872 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_class(yymsp[-5].minor.yy0, yymsp[-1].minor.yy159, 1, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(33,&yymsp[-7].minor);
  yy_destructor(29,&yymsp[-6].minor);
  yy_destructor(32,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 2625 "parser.c"
        break;
      case 12:
// 876 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_class(yymsp[-2].minor.yy0, NULL, 0, NULL, status->scanner_state);
  yy_destructor(29,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 2635 "parser.c"
        break;
      case 13:
// 880 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_class(yymsp[-4].minor.yy0, NULL, 0, yymsp[-2].minor.yy0, status->scanner_state);
  yy_destructor(29,&yymsp[-5].minor);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 2646 "parser.c"
        break;
      case 14:
// 884 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_class(yymsp[-2].minor.yy0, NULL, 1, NULL, status->scanner_state);
  yy_destructor(33,&yymsp[-4].minor);
  yy_destructor(29,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 2657 "parser.c"
        break;
      case 15:
// 888 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_class(yymsp[-4].minor.yy0, NULL, 1, yymsp[-2].minor.yy0, status->scanner_state);
  yy_destructor(33,&yymsp[-6].minor);
  yy_destructor(29,&yymsp[-5].minor);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 2669 "parser.c"
        break;
      case 16:
// 892 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_class_definition(yymsp[0].minor.yy159, NULL, NULL, status->scanner_state);
}
// 2676 "parser.c"
        break;
      case 17:
// 896 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_class_definition(NULL, NULL, yymsp[0].minor.yy159, status->scanner_state);
}
// 2683 "parser.c"
        break;
      case 18:
// 900 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_class_definition(NULL, yymsp[0].minor.yy159, NULL, status->scanner_state);
}
// 2690 "parser.c"
        break;
      case 19:
// 904 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_class_definition(yymsp[-1].minor.yy159, yymsp[0].minor.yy159, NULL, status->scanner_state);
}
// 2697 "parser.c"
        break;
      case 20:
// 908 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_class_definition(yymsp[-1].minor.yy159, NULL, yymsp[0].minor.yy159, status->scanner_state);
}
// 2704 "parser.c"
        break;
      case 21:
// 912 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_class_definition(yymsp[0].minor.yy159, NULL, yymsp[-1].minor.yy159, status->scanner_state);
}
// 2711 "parser.c"
        break;
      case 22:
// 916 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_class_definition(NULL, yymsp[0].minor.yy159, yymsp[-1].minor.yy159, status->scanner_state);
}
// 2718 "parser.c"
        break;
      case 23:
// 920 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_class_definition(yymsp[-2].minor.yy159, yymsp[0].minor.yy159, yymsp[-1].minor.yy159, status->scanner_state);
}
// 2725 "parser.c"
        break;
      case 24:
// 924 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_class_definition(yymsp[-1].minor.yy159, yymsp[0].minor.yy159, yymsp[-2].minor.yy159, status->scanner_state);
}
// 2732 "parser.c"
        break;
      case 27:
// 936 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_class_property(yymsp[-2].minor.yy159, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(28,&yymsp[0].minor);
}
// 2740 "parser.c"
        break;
      case 28:
// 940 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_class_property(yymsp[-2].minor.yy159, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[0].minor);
}
// 2748 "parser.c"
        break;
      case 29:
// 944 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_class_property(yymsp[-4].minor.yy159, yymsp[-3].minor.yy0, yymsp[-1].minor.yy159, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(35,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 2757 "parser.c"
        break;
      case 30:
// 948 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_class_property(yymsp[-4].minor.yy159, yymsp[-3].minor.yy0, yymsp[-1].minor.yy159, NULL, status->scanner_state);
  yy_destructor(35,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 2766 "parser.c"
        break;
      case 33:
      case 53:
// 960 "parser.lemon"
{
	yygotominor.yy159 = json_object_new_string("public");
  yy_destructor(1,&yymsp[0].minor);
}
// 2775 "parser.c"
        break;
      case 34:
      case 54:
// 964 "parser.lemon"
{
	yygotominor.yy159 = json_object_new_string("protected");
  yy_destructor(2,&yymsp[0].minor);
}
// 2784 "parser.c"
        break;
      case 35:
      case 55:
// 968 "parser.lemon"
{
	yygotominor.yy159 = json_object_new_string("static");
  yy_destructor(3,&yymsp[0].minor);
}
// 2793 "parser.c"
        break;
      case 36:
      case 56:
// 972 "parser.lemon"
{
	yygotominor.yy159 = json_object_new_string("private");
  yy_destructor(4,&yymsp[0].minor);
}
// 2802 "parser.c"
        break;
      case 41:
// 992 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy159, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(35,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 2812 "parser.c"
        break;
      case 42:
// 996 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy159, NULL, status->scanner_state);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(35,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 2822 "parser.c"
        break;
      case 43:
// 1000 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_class_method(yymsp[-6].minor.yy159, yymsp[-4].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-5].minor);
  yy_destructor(39,&yymsp[-3].minor);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 2834 "parser.c"
        break;
      case 44:
// 1004 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_class_method(yymsp[-7].minor.yy159, yymsp[-5].minor.yy0, yymsp[-3].minor.yy159, NULL, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-6].minor);
  yy_destructor(39,&yymsp[-4].minor);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 2846 "parser.c"
        break;
      case 45:
// 1008 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_class_method(yymsp[-7].minor.yy159, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy159, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-6].minor);
  yy_destructor(39,&yymsp[-4].minor);
  yy_destructor(25,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 2858 "parser.c"
        break;
      case 46:
// 1012 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_class_method(yymsp[-8].minor.yy159, yymsp[-6].minor.yy0, yymsp[-4].minor.yy159, yymsp[-1].minor.yy159, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-7].minor);
  yy_destructor(39,&yymsp[-5].minor);
  yy_destructor(25,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 2870 "parser.c"
        break;
      case 47:
// 1016 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_class_method(yymsp[-6].minor.yy159, yymsp[-4].minor.yy0, NULL, NULL, yymsp[-7].minor.yy0, status->scanner_state);
  yy_destructor(38,&yymsp[-5].minor);
  yy_destructor(39,&yymsp[-3].minor);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 2882 "parser.c"
        break;
      case 48:
// 1020 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_class_method(yymsp[-7].minor.yy159, yymsp[-5].minor.yy0, yymsp[-3].minor.yy159, NULL, yymsp[-8].minor.yy0, status->scanner_state);
  yy_destructor(38,&yymsp[-6].minor);
  yy_destructor(39,&yymsp[-4].minor);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 2894 "parser.c"
        break;
      case 49:
// 1024 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_class_method(yymsp[-7].minor.yy159, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy159, yymsp[-8].minor.yy0, status->scanner_state);
  yy_destructor(38,&yymsp[-6].minor);
  yy_destructor(39,&yymsp[-4].minor);
  yy_destructor(25,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 2906 "parser.c"
        break;
      case 50:
// 1028 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_class_method(yymsp[-8].minor.yy159, yymsp[-6].minor.yy0, yymsp[-4].minor.yy159, yymsp[-1].minor.yy159, yymsp[-9].minor.yy0, status->scanner_state);
  yy_destructor(38,&yymsp[-7].minor);
  yy_destructor(39,&yymsp[-5].minor);
  yy_destructor(25,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 2918 "parser.c"
        break;
      case 51:
// 1032 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_list(yymsp[-2].minor.yy159, yymsp[0].minor.yy159);
  yy_destructor(16,&yymsp[-1].minor);
}
// 2926 "parser.c"
        break;
      case 57:
// 1056 "parser.lemon"
{
	yygotominor.yy159 = json_object_new_string("private");
  yy_destructor(40,&yymsp[0].minor);
}
// 2934 "parser.c"
        break;
      case 58:
// 1060 "parser.lemon"
{
	yygotominor.yy159 = json_object_new_string("abstract");
  yy_destructor(33,&yymsp[0].minor);
}
// 2942 "parser.c"
        break;
      case 59:
// 1064 "parser.lemon"
{
	yygotominor.yy159 = json_object_new_string("final");
  yy_destructor(41,&yymsp[0].minor);
}
// 2950 "parser.c"
        break;
      case 60:
      case 116:
      case 134:
      case 156:
      case 207:
      case 210:
// 1108 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_list(yymsp[-2].minor.yy159, yymsp[0].minor.yy159);
  yy_destructor(5,&yymsp[-1].minor);
}
// 2963 "parser.c"
        break;
      case 62:
// 1116 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_parameter(NULL, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 2970 "parser.c"
        break;
      case 63:
// 1120 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_parameter(yymsp[-1].minor.yy159, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 2977 "parser.c"
        break;
      case 64:
// 1124 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_parameter(NULL, yymsp[-1].minor.yy159, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 2984 "parser.c"
        break;
      case 65:
// 1128 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_parameter(NULL, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy159, status->scanner_state);
  yy_destructor(35,&yymsp[-1].minor);
}
// 2992 "parser.c"
        break;
      case 66:
// 1132 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_parameter(yymsp[-3].minor.yy159, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy159, status->scanner_state);
  yy_destructor(35,&yymsp[-1].minor);
}
// 3000 "parser.c"
        break;
      case 67:
// 1136 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_parameter(NULL, yymsp[-3].minor.yy159, yymsp[-2].minor.yy0, yymsp[0].minor.yy159, status->scanner_state);
  yy_destructor(35,&yymsp[-1].minor);
}
// 3008 "parser.c"
        break;
      case 68:
// 1140 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(10,&yymsp[-2].minor);
  yy_destructor(11,&yymsp[0].minor);
}
// 3017 "parser.c"
        break;
      case 69:
// 1144 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_type(XX_TYPE_INTEGER);
  yy_destructor(42,&yymsp[0].minor);
}
// 3025 "parser.c"
        break;
      case 70:
// 1148 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_type(XX_TYPE_DOUBLE);
  yy_destructor(43,&yymsp[0].minor);
}
// 3033 "parser.c"
        break;
      case 71:
// 1152 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_type(XX_TYPE_BOOL);
  yy_destructor(44,&yymsp[0].minor);
}
// 3041 "parser.c"
        break;
      case 72:
// 1156 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_type(XX_TYPE_STRING);
  yy_destructor(45,&yymsp[0].minor);
}
// 3049 "parser.c"
        break;
      case 73:
// 1160 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_type(XX_TYPE_ARRAY);
  yy_destructor(46,&yymsp[0].minor);
}
// 3057 "parser.c"
        break;
      case 74:
// 1164 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_type(XX_TYPE_VAR);
  yy_destructor(47,&yymsp[0].minor);
}
// 3065 "parser.c"
        break;
      case 93:
// 1240 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_break_statement(status->scanner_state);
  yy_destructor(48,&yymsp[-1].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3074 "parser.c"
        break;
      case 94:
// 1244 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_continue_statement(status->scanner_state);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3083 "parser.c"
        break;
      case 95:
// 1248 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_if_statement(yymsp[-2].minor.yy159, NULL, NULL, status->scanner_state);
  yy_destructor(50,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3093 "parser.c"
        break;
      case 96:
// 1252 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_if_statement(yymsp[-5].minor.yy159, NULL, NULL, status->scanner_state);
  yy_destructor(50,&yymsp[-6].minor);
  yy_destructor(30,&yymsp[-4].minor);
  yy_destructor(31,&yymsp[-3].minor);
  yy_destructor(51,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3106 "parser.c"
        break;
      case 97:
// 1256 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_if_statement(yymsp[-3].minor.yy159, yymsp[-1].minor.yy159, NULL, status->scanner_state);
  yy_destructor(50,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3116 "parser.c"
        break;
      case 98:
// 1260 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_if_statement(yymsp[-7].minor.yy159, yymsp[-5].minor.yy159, yymsp[-1].minor.yy159, status->scanner_state);
  yy_destructor(50,&yymsp[-8].minor);
  yy_destructor(30,&yymsp[-6].minor);
  yy_destructor(31,&yymsp[-4].minor);
  yy_destructor(51,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3129 "parser.c"
        break;
      case 99:
// 1264 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_if_statement(yymsp[-6].minor.yy159, yymsp[-4].minor.yy159, NULL, status->scanner_state);
  yy_destructor(50,&yymsp[-7].minor);
  yy_destructor(30,&yymsp[-5].minor);
  yy_destructor(31,&yymsp[-3].minor);
  yy_destructor(51,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3142 "parser.c"
        break;
      case 100:
// 1268 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_if_statement(yymsp[-6].minor.yy159, NULL, yymsp[-1].minor.yy159, status->scanner_state);
  yy_destructor(50,&yymsp[-7].minor);
  yy_destructor(30,&yymsp[-5].minor);
  yy_destructor(31,&yymsp[-4].minor);
  yy_destructor(51,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3155 "parser.c"
        break;
      case 101:
// 1272 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_switch_statement(yymsp[-2].minor.yy159, NULL, status->scanner_state);
  yy_destructor(52,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3165 "parser.c"
        break;
      case 102:
// 1276 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_switch_statement(yymsp[-3].minor.yy159, yymsp[-1].minor.yy159, status->scanner_state);
  yy_destructor(52,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3175 "parser.c"
        break;
      case 105:
// 1288 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_case_clause(yymsp[-2].minor.yy159, yymsp[0].minor.yy159, status->scanner_state);
  yy_destructor(53,&yymsp[-3].minor);
  yy_destructor(54,&yymsp[-1].minor);
}
// 3184 "parser.c"
        break;
      case 106:
// 1292 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_case_clause(NULL, yymsp[0].minor.yy159, status->scanner_state);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(54,&yymsp[-1].minor);
}
// 3193 "parser.c"
        break;
      case 107:
// 1296 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_loop_statement(NULL, status->scanner_state);
  yy_destructor(56,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3203 "parser.c"
        break;
      case 108:
// 1300 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_loop_statement(yymsp[-1].minor.yy159, status->scanner_state);
  yy_destructor(56,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3213 "parser.c"
        break;
      case 109:
// 1304 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_while_statement(yymsp[-2].minor.yy159, NULL, status->scanner_state);
  yy_destructor(57,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3223 "parser.c"
        break;
      case 110:
// 1308 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_while_statement(yymsp[-3].minor.yy159, yymsp[-1].minor.yy159, status->scanner_state);
  yy_destructor(57,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3233 "parser.c"
        break;
      case 111:
// 1312 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_for_statement(yymsp[-3].minor.yy159, NULL, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy159, status->scanner_state);
  yy_destructor(58,&yymsp[-6].minor);
  yy_destructor(59,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3244 "parser.c"
        break;
      case 112:
// 1316 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_for_statement(yymsp[-3].minor.yy159, NULL, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy159, status->scanner_state);
  yy_destructor(58,&yymsp[-7].minor);
  yy_destructor(59,&yymsp[-5].minor);
  yy_destructor(60,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3256 "parser.c"
        break;
      case 113:
// 1320 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_for_statement(yymsp[-3].minor.yy159, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy159, status->scanner_state);
  yy_destructor(58,&yymsp[-8].minor);
  yy_destructor(5,&yymsp[-6].minor);
  yy_destructor(59,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3268 "parser.c"
        break;
      case 114:
// 1324 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_for_statement(yymsp[-3].minor.yy159, yymsp[-8].minor.yy0, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy159, status->scanner_state);
  yy_destructor(58,&yymsp[-9].minor);
  yy_destructor(5,&yymsp[-7].minor);
  yy_destructor(59,&yymsp[-5].minor);
  yy_destructor(60,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3281 "parser.c"
        break;
      case 115:
// 1328 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_let_statement(yymsp[-1].minor.yy159, status->scanner_state);
  yy_destructor(61,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3290 "parser.c"
        break;
      case 118:
// 1341 "parser.lemon"
{
	yygotominor.yy159 = json_object_new_string("assign");
  yy_destructor(35,&yymsp[0].minor);
}
// 3298 "parser.c"
        break;
      case 119:
// 1346 "parser.lemon"
{
	yygotominor.yy159 = json_object_new_string("add-assign");
  yy_destructor(62,&yymsp[0].minor);
}
// 3306 "parser.c"
        break;
      case 120:
// 1351 "parser.lemon"
{
	yygotominor.yy159 = json_object_new_string("sub-assign");
  yy_destructor(63,&yymsp[0].minor);
}
// 3314 "parser.c"
        break;
      case 121:
// 1355 "parser.lemon"
{
	yygotominor.yy159 = json_object_new_string("mult-assign");
  yy_destructor(64,&yymsp[0].minor);
}
// 3322 "parser.c"
        break;
      case 122:
// 1359 "parser.lemon"
{
	yygotominor.yy159 = json_object_new_string("div-assign");
  yy_destructor(65,&yymsp[0].minor);
}
// 3330 "parser.c"
        break;
      case 123:
// 1363 "parser.lemon"
{
	yygotominor.yy159 = json_object_new_string("concat-assign");
  yy_destructor(66,&yymsp[0].minor);
}
// 3338 "parser.c"
        break;
      case 124:
// 1368 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_let_assignment("variable", yymsp[-1].minor.yy159, yymsp[-2].minor.yy0, NULL, NULL, yymsp[0].minor.yy159, status->scanner_state);
}
// 3345 "parser.c"
        break;
      case 125:
// 1373 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_let_assignment("object-property", yymsp[-1].minor.yy159, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy159, status->scanner_state);
  yy_destructor(67,&yymsp[-3].minor);
}
// 3353 "parser.c"
        break;
      case 126:
// 1378 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_let_assignment("object-property-append", yymsp[-1].minor.yy159, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy159, status->scanner_state);
  yy_destructor(67,&yymsp[-5].minor);
  yy_destructor(68,&yymsp[-3].minor);
  yy_destructor(69,&yymsp[-2].minor);
}
// 3363 "parser.c"
        break;
      case 127:
// 1383 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_let_assignment("object-property-array-index", yymsp[-1].minor.yy159, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy159, yymsp[0].minor.yy159, status->scanner_state);
  yy_destructor(67,&yymsp[-6].minor);
  yy_destructor(68,&yymsp[-4].minor);
  yy_destructor(69,&yymsp[-2].minor);
}
// 3373 "parser.c"
        break;
      case 128:
// 1388 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_let_assignment("variable-append", yymsp[-1].minor.yy159, yymsp[-4].minor.yy0, NULL, NULL, yymsp[0].minor.yy159, status->scanner_state);
  yy_destructor(68,&yymsp[-3].minor);
  yy_destructor(69,&yymsp[-2].minor);
}
// 3382 "parser.c"
        break;
      case 129:
// 1393 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_let_assignment("array-index", yymsp[-1].minor.yy159, yymsp[-5].minor.yy0, NULL, yymsp[-3].minor.yy159, yymsp[0].minor.yy159, status->scanner_state);
  yy_destructor(68,&yymsp[-4].minor);
  yy_destructor(69,&yymsp[-2].minor);
}
// 3391 "parser.c"
        break;
      case 130:
// 1398 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_let_assignment("incr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(70,&yymsp[0].minor);
}
// 3399 "parser.c"
        break;
      case 131:
// 1403 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_let_assignment("decr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(71,&yymsp[0].minor);
}
// 3407 "parser.c"
        break;
      case 133:
// 1411 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_echo_statement(yymsp[-1].minor.yy159, status->scanner_state);
  yy_destructor(72,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3416 "parser.c"
        break;
      case 136:
// 1423 "parser.lemon"
{
	yygotominor.yy159 = yymsp[0].minor.yy159;;
}
// 3423 "parser.c"
        break;
      case 137:
// 1428 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_mcall_statement(yymsp[-1].minor.yy159, status->scanner_state);
  yy_destructor(28,&yymsp[0].minor);
}
// 3431 "parser.c"
        break;
      case 138:
// 1433 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_scall_statement(yymsp[-1].minor.yy159, status->scanner_state);
  yy_destructor(28,&yymsp[0].minor);
}
// 3439 "parser.c"
        break;
      case 139:
// 1438 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_return_statement(yymsp[-1].minor.yy159, status->scanner_state);
  yy_destructor(73,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3448 "parser.c"
        break;
      case 140:
// 1443 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(74,&yymsp[-4].minor);
  yy_destructor(67,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3462 "parser.c"
        break;
      case 141:
// 1452 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(74,&yymsp[-5].minor);
  yy_destructor(68,&yymsp[-3].minor);
  yy_destructor(69,&yymsp[-1].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3477 "parser.c"
        break;
      case 142:
// 1461 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_INTEGER, yymsp[-2].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(74,&yymsp[-5].minor);
  yy_destructor(68,&yymsp[-3].minor);
  yy_destructor(69,&yymsp[-1].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3492 "parser.c"
        break;
      case 143:
// 1470 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_STRING, yymsp[-2].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(74,&yymsp[-5].minor);
  yy_destructor(68,&yymsp[-3].minor);
  yy_destructor(69,&yymsp[-1].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3507 "parser.c"
        break;
      case 144:
// 1479 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_throw_exception(yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(77,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3516 "parser.c"
        break;
      case 145:
// 1484 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_throw_exception(yymsp[-4].minor.yy0, yymsp[-2].minor.yy159, status->scanner_state);
  yy_destructor(77,&yymsp[-6].minor);
  yy_destructor(23,&yymsp[-5].minor);
  yy_destructor(39,&yymsp[-3].minor);
  yy_destructor(25,&yymsp[-1].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3528 "parser.c"
        break;
      case 146:
// 1488 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_declare_statement(XX_T_TYPE_INTEGER, yymsp[-1].minor.yy159, status->scanner_state);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3537 "parser.c"
        break;
      case 147:
// 1492 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_declare_statement(XX_T_TYPE_UINTEGER, yymsp[-1].minor.yy159, status->scanner_state);
  yy_destructor(78,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3546 "parser.c"
        break;
      case 148:
// 1496 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_declare_statement(XX_T_TYPE_CHAR, yymsp[-1].minor.yy159, status->scanner_state);
  yy_destructor(79,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3555 "parser.c"
        break;
      case 149:
// 1500 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_declare_statement(XX_T_TYPE_UCHAR, yymsp[-1].minor.yy159, status->scanner_state);
  yy_destructor(80,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3564 "parser.c"
        break;
      case 150:
// 1504 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_declare_statement(XX_T_TYPE_LONG, yymsp[-1].minor.yy159, status->scanner_state);
  yy_destructor(81,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3573 "parser.c"
        break;
      case 151:
// 1508 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_declare_statement(XX_T_TYPE_ULONG, yymsp[-1].minor.yy159, status->scanner_state);
  yy_destructor(82,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3582 "parser.c"
        break;
      case 152:
// 1512 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_declare_statement(XX_T_TYPE_DOUBLE, yymsp[-1].minor.yy159, status->scanner_state);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3591 "parser.c"
        break;
      case 153:
// 1516 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_declare_statement(XX_T_TYPE_STRING, yymsp[-1].minor.yy159, status->scanner_state);
  yy_destructor(45,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3600 "parser.c"
        break;
      case 154:
// 1520 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_declare_statement(XX_T_TYPE_BOOL, yymsp[-1].minor.yy159, status->scanner_state);
  yy_destructor(44,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3609 "parser.c"
        break;
      case 155:
// 1524 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_declare_statement(XX_T_TYPE_VAR, yymsp[-1].minor.yy159, status->scanner_state);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3618 "parser.c"
        break;
      case 158:
// 1536 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_declare_variable(yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3625 "parser.c"
        break;
      case 159:
// 1540 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_declare_variable(yymsp[-2].minor.yy0, yymsp[0].minor.yy159, status->scanner_state);
  yy_destructor(35,&yymsp[-1].minor);
}
// 3633 "parser.c"
        break;
      case 161:
// 1548 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("not", yymsp[0].minor.yy159, NULL, NULL, status->scanner_state);
  yy_destructor(24,&yymsp[-1].minor);
}
// 3641 "parser.c"
        break;
      case 162:
// 1552 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("isset", yymsp[0].minor.yy159, NULL, NULL, status->scanner_state);
  yy_destructor(19,&yymsp[-1].minor);
}
// 3649 "parser.c"
        break;
      case 163:
// 1556 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("equals", yymsp[-2].minor.yy159, yymsp[0].minor.yy159, NULL, status->scanner_state);
  yy_destructor(8,&yymsp[-1].minor);
}
// 3657 "parser.c"
        break;
      case 164:
// 1560 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("not-equals", yymsp[-2].minor.yy159, yymsp[0].minor.yy159, NULL, status->scanner_state);
  yy_destructor(15,&yymsp[-1].minor);
}
// 3665 "parser.c"
        break;
      case 165:
// 1564 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("identical", yymsp[-2].minor.yy159, yymsp[0].minor.yy159, NULL, status->scanner_state);
  yy_destructor(9,&yymsp[-1].minor);
}
// 3673 "parser.c"
        break;
      case 166:
// 1568 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("not-identical", yymsp[-2].minor.yy159, yymsp[0].minor.yy159, NULL, status->scanner_state);
  yy_destructor(14,&yymsp[-1].minor);
}
// 3681 "parser.c"
        break;
      case 167:
// 1572 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("less", yymsp[-2].minor.yy159, yymsp[0].minor.yy159, NULL, status->scanner_state);
  yy_destructor(10,&yymsp[-1].minor);
}
// 3689 "parser.c"
        break;
      case 168:
// 1576 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("greater", yymsp[-2].minor.yy159, yymsp[0].minor.yy159, NULL, status->scanner_state);
  yy_destructor(11,&yymsp[-1].minor);
}
// 3697 "parser.c"
        break;
      case 169:
// 1580 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("less-equal", yymsp[-2].minor.yy159, yymsp[0].minor.yy159, NULL, status->scanner_state);
  yy_destructor(12,&yymsp[-1].minor);
}
// 3705 "parser.c"
        break;
      case 170:
// 1584 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("greater-equal", yymsp[-2].minor.yy159, yymsp[0].minor.yy159, NULL, status->scanner_state);
  yy_destructor(13,&yymsp[-1].minor);
}
// 3713 "parser.c"
        break;
      case 171:
// 1588 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("list", yymsp[-1].minor.yy159, NULL, NULL, status->scanner_state);
  yy_destructor(39,&yymsp[-2].minor);
  yy_destructor(25,&yymsp[0].minor);
}
// 3722 "parser.c"
        break;
      case 172:
// 1592 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("cast", yymsp[-2].minor.yy159, yymsp[0].minor.yy159, NULL, status->scanner_state);
  yy_destructor(39,&yymsp[-3].minor);
  yy_destructor(25,&yymsp[-1].minor);
}
// 3731 "parser.c"
        break;
      case 173:
// 1596 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(67,&yymsp[-1].minor);
}
// 3739 "parser.c"
        break;
      case 174:
      case 225:
// 1600 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(68,&yymsp[-2].minor);
  yy_destructor(69,&yymsp[0].minor);
}
// 3749 "parser.c"
        break;
      case 175:
      case 226:
// 1605 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_INTEGER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(68,&yymsp[-2].minor);
  yy_destructor(69,&yymsp[0].minor);
}
// 3759 "parser.c"
        break;
      case 176:
      case 227:
// 1610 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_STRING, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(68,&yymsp[-2].minor);
  yy_destructor(69,&yymsp[0].minor);
}
// 3769 "parser.c"
        break;
      case 177:
// 1615 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("add", yymsp[-2].minor.yy159, yymsp[0].minor.yy159, NULL, status->scanner_state);
  yy_destructor(16,&yymsp[-1].minor);
}
// 3777 "parser.c"
        break;
      case 178:
// 1620 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("sub", yymsp[-2].minor.yy159, yymsp[0].minor.yy159, NULL, status->scanner_state);
  yy_destructor(17,&yymsp[-1].minor);
}
// 3785 "parser.c"
        break;
      case 179:
// 1625 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("concat", yymsp[-2].minor.yy159, yymsp[0].minor.yy159, NULL, status->scanner_state);
  yy_destructor(18,&yymsp[-1].minor);
}
// 3793 "parser.c"
        break;
      case 180:
// 1630 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("and", yymsp[-2].minor.yy159, yymsp[0].minor.yy159, NULL, status->scanner_state);
  yy_destructor(6,&yymsp[-1].minor);
}
// 3801 "parser.c"
        break;
      case 181:
// 1635 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("or", yymsp[-2].minor.yy159, yymsp[0].minor.yy159, NULL, status->scanner_state);
  yy_destructor(7,&yymsp[-1].minor);
}
// 3809 "parser.c"
        break;
      case 182:
// 1640 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("instanceof", yymsp[0].minor.yy159, NULL, NULL, status->scanner_state);
  yy_destructor(22,&yymsp[-1].minor);
}
// 3817 "parser.c"
        break;
      case 183:
// 1645 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("fetch", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy159, NULL, status->scanner_state);
  yy_destructor(20,&yymsp[-3].minor);
  yy_destructor(5,&yymsp[-1].minor);
}
// 3826 "parser.c"
        break;
      case 184:
// 1650 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("typeof", yymsp[0].minor.yy159, NULL, NULL, status->scanner_state);
  yy_destructor(21,&yymsp[-1].minor);
}
// 3834 "parser.c"
        break;
      case 185:
// 1655 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_literal(XX_T_CONSTANT, yymsp[0].minor.yy0, status->scanner_state);
}
// 3841 "parser.c"
        break;
      case 186:
      case 214:
      case 218:
// 1660 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state);
}
// 3850 "parser.c"
        break;
      case 187:
      case 216:
      case 219:
// 1665 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_literal(XX_T_INTEGER, yymsp[0].minor.yy0, status->scanner_state);
}
// 3859 "parser.c"
        break;
      case 188:
      case 215:
      case 220:
// 1670 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_literal(XX_T_STRING, yymsp[0].minor.yy0, status->scanner_state);
}
// 3868 "parser.c"
        break;
      case 189:
// 1675 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_literal(XX_T_SCHAR, yymsp[0].minor.yy0, status->scanner_state);
}
// 3875 "parser.c"
        break;
      case 190:
      case 221:
// 1680 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_literal(XX_T_DOUBLE, yymsp[0].minor.yy0, status->scanner_state);
}
// 3883 "parser.c"
        break;
      case 191:
      case 222:
// 1685 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_literal(XX_T_NULL, NULL, status->scanner_state);
  yy_destructor(85,&yymsp[0].minor);
}
// 3892 "parser.c"
        break;
      case 192:
      case 224:
// 1690 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_literal(XX_T_TRUE, NULL, status->scanner_state);
  yy_destructor(86,&yymsp[0].minor);
}
// 3901 "parser.c"
        break;
      case 193:
      case 223:
// 1695 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_literal(XX_T_FALSE, NULL, status->scanner_state);
  yy_destructor(87,&yymsp[0].minor);
}
// 3910 "parser.c"
        break;
      case 194:
// 1700 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("empty-array", NULL, NULL, NULL, status->scanner_state);
  yy_destructor(68,&yymsp[-1].minor);
  yy_destructor(69,&yymsp[0].minor);
}
// 3919 "parser.c"
        break;
      case 195:
// 1705 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("array", yymsp[-1].minor.yy159, NULL, NULL, status->scanner_state);
  yy_destructor(68,&yymsp[-2].minor);
  yy_destructor(69,&yymsp[0].minor);
}
// 3928 "parser.c"
        break;
      case 196:
// 1710 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_new_instance(yymsp[0].minor.yy0, NULL, status->scanner_state);
  yy_destructor(23,&yymsp[-1].minor);
}
// 3936 "parser.c"
        break;
      case 197:
// 1715 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_new_instance(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(23,&yymsp[-3].minor);
  yy_destructor(39,&yymsp[-1].minor);
  yy_destructor(25,&yymsp[0].minor);
}
// 3946 "parser.c"
        break;
      case 198:
// 1720 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_new_instance(yymsp[-3].minor.yy0, yymsp[-1].minor.yy159, status->scanner_state);
  yy_destructor(23,&yymsp[-4].minor);
  yy_destructor(39,&yymsp[-2].minor);
  yy_destructor(25,&yymsp[0].minor);
}
// 3956 "parser.c"
        break;
      case 199:
// 1725 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_fcall(yymsp[-3].minor.yy0, yymsp[-1].minor.yy159, status->scanner_state);
  yy_destructor(39,&yymsp[-2].minor);
  yy_destructor(25,&yymsp[0].minor);
}
// 3965 "parser.c"
        break;
      case 200:
// 1730 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_fcall(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(39,&yymsp[-1].minor);
  yy_destructor(25,&yymsp[0].minor);
}
// 3974 "parser.c"
        break;
      case 203:
// 1745 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_scall(yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy159, status->scanner_state);
  yy_destructor(88,&yymsp[-4].minor);
  yy_destructor(39,&yymsp[-2].minor);
  yy_destructor(25,&yymsp[0].minor);
}
// 3984 "parser.c"
        break;
      case 204:
// 1750 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_scall(yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(88,&yymsp[-3].minor);
  yy_destructor(39,&yymsp[-1].minor);
  yy_destructor(25,&yymsp[0].minor);
}
// 3994 "parser.c"
        break;
      case 205:
// 1755 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_mcall(yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy159, status->scanner_state);
  yy_destructor(67,&yymsp[-4].minor);
  yy_destructor(39,&yymsp[-2].minor);
  yy_destructor(25,&yymsp[0].minor);
}
// 4004 "parser.c"
        break;
      case 206:
// 1760 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_mcall(yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(67,&yymsp[-3].minor);
  yy_destructor(39,&yymsp[-1].minor);
  yy_destructor(25,&yymsp[0].minor);
}
// 4014 "parser.c"
        break;
      case 212:
// 1784 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_array_item(yymsp[-2].minor.yy159, yymsp[0].minor.yy159, status->scanner_state);
  yy_destructor(54,&yymsp[-1].minor);
}
// 4022 "parser.c"
        break;
      case 213:
// 1788 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_array_item(NULL, yymsp[0].minor.yy159, status->scanner_state);
}
// 4029 "parser.c"
        break;
      case 229:
// 1855 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_comment(yymsp[0].minor.yy0, status->scanner_state);
}
// 4036 "parser.c"
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
// 783 "parser.lemon"


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

// 4112 "parser.c"
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
	parser_status->number_brackets = 0;

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
				parser_status->number_brackets++;
				xx_(xx_parser, XX_BRACKET_OPEN, NULL, parser_status);
				break;
			case XX_T_BRACKET_CLOSE:
				parser_status->number_brackets--;
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
				if (parser_status->number_brackets <= 1) {
					xx_parse_with_token(xx_parser, XX_T_COMMENT, XX_COMMENT, &token, parser_status);
				}
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
			case XX_T_CONSTANT:
				xx_parse_with_token(xx_parser, XX_T_CONSTANT, XX_CONSTANT, &token, parser_status);
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
