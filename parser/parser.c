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
#define YYNSTATE 489
#define YYNRULE 235
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
 /*     0 */   335,   18,   19,   88,   25,  421,  489,  340,  725,    1,
 /*    10 */     2,  488,    4,    5,    6,  299,  323,  329,  326,  237,
 /*    20 */   332,   45,   47,   49,  233,   58,   87,  199,  200,   69,
 /*    30 */    73,   78,    9,  463,   85,  466,  467,  455,  396,  450,
 /*    40 */   424,  425,   19,   62,  365,  421,  261,  272,  275,   86,
 /*    50 */   260,  290,  308,  311,  314,  317,  320,  335,  181,  182,
 /*    60 */    20,   27,  347,  198,  340,    8,   91,  183,  184,  186,
 /*    70 */   185,  179,  299,  323,  329,  326,  237,  332,   45,   47,
 /*    80 */    49,  246,   58,  197,  199,  200,   69,   73,   78,   22,
 /*    90 */   392,   85,  188,   92,   93,   94,   95,   96,   11,  229,
 /*   100 */   380,  384,  388,  261,  272,  275,   14,  381,  290,  308,
 /*   110 */   311,  314,  317,  320,  335,   21,   80,  146,  367,  178,
 /*   120 */   198,  340,  138,  139,  140,  141,  142,  143,   91,  299,
 /*   130 */   323,  329,  326,  174,  332,   45,   47,   49,  144,   58,
 /*   140 */   204,  190,  189,   69,   73,   78,  199,  200,   85,  180,
 /*   150 */   183,  184,  186,  185,  207,   92,   93,   94,   95,   96,
 /*   160 */   261,  272,  275,  108,  212,  290,  308,  311,  314,  317,
 /*   170 */   320,  335,   12,  266,  469,   53,  266,   24,  340,  262,
 /*   180 */   267,  199,  200,  265,  199,  200,  299,  323,  329,  326,
 /*   190 */    46,  332,   45,   47,   49,   81,   58,  173,  181,  182,
 /*   200 */    69,   73,   78,  199,  200,   85,  153,  183,  184,  186,
 /*   210 */   185,  198,  110,  112,  199,  200,  203,  261,  272,  275,
 /*   220 */   396,  202,  290,  308,  311,  314,  317,  320,  335,   51,
 /*   230 */    99,  210,  366,    7,   98,  340,   10,   91,  199,  200,
 /*   240 */   475,  340,  407,  299,  323,  329,  326,  542,  332,   45,
 /*   250 */    47,   49,   99,   58,  264,   48,  228,   69,   73,   78,
 /*   260 */   199,  200,   85,  153,   92,   93,   94,   95,   96,  542,
 /*   270 */   242,  199,  200,  196,  261,  272,  275,  263,  202,  290,
 /*   280 */   308,  311,  314,  317,  320,  335,  277,  280,  423,   57,
 /*   290 */   543,  198,  340,  424,  425,   19,  338,  179,  421,  336,
 /*   300 */   299,  323,  329,  326,   54,  332,   45,   47,   49,   99,
 /*   310 */    58,  222,  543,  232,   69,   73,   78,  199,  200,   85,
 /*   320 */   464,  437,  455,   55,  450,  293,  425,   19,  198,  291,
 /*   330 */   421,  261,  272,  275,  300,  307,  290,  308,  311,  314,
 /*   340 */   317,  320,  335,  175,  176,  187,  361,  406,  297,  340,
 /*   350 */   421,   60,    3,    4,    5,    6,   61,  299,  323,  329,
 /*   360 */   326,   67,  332,   45,   47,   49,   99,   58,  302,   64,
 /*   370 */   236,   69,   73,   78,  199,  200,   85,  153,   65,  179,
 /*   380 */   362,  446,  447,  448,  449,  199,  200,  209,  261,  272,
 /*   390 */   275,  301,  202,  290,  308,  311,  314,  317,  320,  335,
 /*   400 */   439,   70,  450,   72,   75,  406,  340,  440,  421,  446,
 /*   410 */   447,  448,  449,   66,  299,  323,  329,  326,   79,  332,
 /*   420 */    45,   47,   49,  172,   58,  191,  176,  187,   69,   73,
 /*   430 */    78,  309,  307,   85,  153,  456,  373,  220,  302,  395,
 /*   440 */   384,  388,  199,  200,  221,  261,  272,  275,  544,  202,
 /*   450 */   290,  308,  311,  314,  317,  320,  335,  127,  129,  131,
 /*   460 */   359,  310,  379,  340,  379,  150,  213,  478,  206,  482,
 /*   470 */   544,  299,  323,  329,  326,   90,  332,   45,   47,   49,
 /*   480 */   153,   58,  374,  153,  416,   69,   73,   78,  199,  200,
 /*   490 */    85,  199,  200,  296,   50,  201,  415,  360,  202,  395,
 /*   500 */   384,  388,  261,  272,  275,  199,  200,  290,  308,  311,
 /*   510 */   314,  317,  320,  335,  224,   59,  237,   77,  360,  545,
 /*   520 */   340,  254,  199,  200,  199,  200,  199,  200,  299,  323,
 /*   530 */   329,  326,   89,  332,   45,   47,   49,   74,   58,  302,
 /*   540 */   360,  545,   69,   73,   78,  441,   99,   85,  199,  200,
 /*   550 */   241,   97,  442,  302,  199,  200,  312,  307,  214,  261,
 /*   560 */   272,  275,  313,  106,  290,  308,  311,  314,  317,  320,
 /*   570 */   335,   99,  105,  102,   84,  245,  316,  340,  107,  199,
 /*   580 */   200,  199,  200,  315,  307,  299,  323,  329,  326,  302,
 /*   590 */   332,   45,   47,   49,   99,   58,  318,  307,  249,   69,
 /*   600 */    73,   78,  199,  200,   85,   99,   63,  216,  218,  253,
 /*   610 */   302,   99,  319,  199,  200,  257,  261,  272,  275,  199,
 /*   620 */   200,  290,  308,  311,  314,  317,  320,  335,   65,  114,
 /*   630 */   362,  346,  116,  322,  340,  109,  281,  199,  200,  147,
 /*   640 */   199,  200,  299,  323,  329,  326,  118,  332,   45,   47,
 /*   650 */    49,  120,   58,  151,  199,  200,   69,   73,   78,  199,
 /*   660 */   200,   85,  122,  321,  307,  324,  307,  302,  124,  302,
 /*   670 */   199,  200,  111,  261,  272,  275,  199,  200,  290,  308,
 /*   680 */   311,  314,  317,  320,  335,  284,  287,  302,  353,  126,
 /*   690 */   325,  340,  328,  327,  307,  330,  307,  199,  200,  299,
 /*   700 */   323,  329,  326,  128,  332,   45,   47,   49,  113,   58,
 /*   710 */   331,  199,  200,   69,   73,   78,  130,  152,   85,  132,
 /*   720 */   333,  307,  457,  157,  199,  200,  302,  199,  200,  458,
 /*   730 */   261,  272,  275,  158,  134,  290,  308,  311,  314,  317,
 /*   740 */   320,  335,  199,  200,  159,  358,  177,  708,  340,  334,
 /*   750 */   193,  709,  710,  226,  194,  208,  299,  323,  329,  326,
 /*   760 */   136,  332,   45,   47,   49,  148,   58,  227,  199,  200,
 /*   770 */    69,   73,   78,  199,  200,   85,  155,  215,  217,  219,
 /*   780 */   231,  234,  161,  235,  199,  200,  239,  261,  272,  275,
 /*   790 */   199,  200,  290,  308,  311,  314,  317,  320,  335,  240,
 /*   800 */   247,  244,  372,  248,  252,  340,  255,  256,  269,  271,
 /*   810 */   276,  278,  282,  299,  323,  329,  326,  273,  332,   45,
 /*   820 */    47,   49,  343,   58,  279,  199,  200,   69,   73,   78,
 /*   830 */   199,  200,   85,  350,  283,  285,  286,  288,  289,  355,
 /*   840 */   292,  199,  200,  294,  261,  272,  275,  199,  200,  290,
 /*   850 */   308,  311,  314,  317,  320,  335,  295,  298,  304,  371,
 /*   860 */   305,  306,  340,  303,  337,  339,  348,  363,  349,  368,
 /*   870 */   299,  323,  329,  326,  369,  332,   45,   47,   49,  375,
 /*   880 */    58,  382,  383,  385,   69,   73,   78,  386,  387,   85,
 /*   890 */   389,  391,  390,  393,  394,  557,  397,  408,  409,  411,
 /*   900 */   429,  261,  272,  275,  417,  430,  290,  308,  311,  314,
 /*   910 */   317,  320,  335,  428,  431,  433,  376,  434,  435,  340,
 /*   920 */   436,  444,  460,  445,  443,  459,  470,  299,  323,  329,
 /*   930 */   326,  471,  332,   45,   47,   49,  473,   58,  476,  477,
 /*   940 */   484,   69,   73,   78,  480,  483,   85,  486,  389,  389,
 /*   950 */   389,  389,  389,  389,  389,  389,  389,  389,  261,  272,
 /*   960 */   275,  389,  389,  290,  308,  311,  314,  317,  320,  335,
 /*   970 */   389,  389,  389,  378,  389,  389,  340,  389,  389,  389,
 /*   980 */   389,  389,  389,  389,  299,  323,  329,  326,  389,  332,
 /*   990 */    45,   47,   49,  389,   58,  389,  389,  389,   69,   73,
 /*  1000 */    78,  389,  389,   85,  389,  389,  389,  389,  389,  389,
 /*  1010 */   389,  389,  389,  389,  389,  261,  272,  275,  389,  389,
 /*  1020 */   290,  308,  311,  314,  317,  320,  335,  389,  389,  389,
 /*  1030 */   412,  389,  389,  340,  389,  389,  389,  389,  389,  389,
 /*  1040 */   389,  299,  323,  329,  326,  389,  332,   45,   47,   49,
 /*  1050 */   389,   58,  389,  389,  389,   69,   73,   78,  389,  389,
 /*  1060 */    85,  389,  389,  389,  389,  389,  389,  389,  389,  389,
 /*  1070 */   389,  389,  261,  272,  275,  389,  389,  290,  308,  311,
 /*  1080 */   314,  317,  320,  335,  389,  389,  389,  414,  389,  389,
 /*  1090 */   340,  389,  389,  389,  389,  389,  389,  389,  299,  323,
 /*  1100 */   329,  326,  389,  332,   45,   47,   49,  389,   58,  389,
 /*  1110 */   389,  389,   69,   73,   78,  389,  389,   85,  389,  389,
 /*  1120 */   389,  389,  389,  389,  389,  389,  389,  389,  389,  261,
 /*  1130 */   272,  275,  389,  389,  290,  308,  311,  314,  317,  320,
 /*  1140 */   335,  389,  389,  389,  418,  389,  389,  340,  389,  389,
 /*  1150 */   389,  389,  389,  389,  389,  299,  323,  329,  326,  389,
 /*  1160 */   332,   45,   47,   49,  389,   58,  389,  389,  389,   69,
 /*  1170 */    73,   78,  389,  389,   85,  389,  389,  389,  389,  389,
 /*  1180 */   389,  389,  389,  389,  389,  389,  261,  272,  275,  389,
 /*  1190 */   389,  290,  308,  311,  314,  317,  320,  335,  389,  389,
 /*  1200 */   389,  420,  389,  389,  340,  389,  389,  389,  389,  389,
 /*  1210 */   389,  389,  299,  323,  329,  326,  389,  332,   45,   47,
 /*  1220 */    49,  389,   58,  389,  389,  389,   69,   73,   78,  389,
 /*  1230 */   389,   85,  389,  389,  389,  389,  389,  389,  389,  389,
 /*  1240 */   335,  389,  389,  261,  272,  275,  490,  340,  290,  308,
 /*  1250 */   311,  314,  317,  320,  389,  299,  323,  329,  326,  389,
 /*  1260 */   332,   45,   47,   49,  389,   58,  392,  389,  389,   69,
 /*  1270 */    73,   78,    7,  389,   85,   10,  389,  389,  389,  475,
 /*  1280 */   340,   23,  389,  381,  389,  389,  261,  272,  275,  389,
 /*  1290 */   389,  290,  308,  311,  314,  317,  320,  389,  138,  139,
 /*  1300 */   140,  141,  142,  143,  389,  389,  104,  156,  160,  154,
 /*  1310 */   192,  101,  389,  389,  149,  389,  389,  389,  389,  389,
 /*  1320 */   389,  389,  389,  389,  162,  389,  137,  389,  389,  138,
 /*  1330 */   139,  140,  141,  142,  143,  389,  133,  135,  100,  115,
 /*  1340 */   119,  121,  123,  125,  117,  103,  127,  129,  131,   13,
 /*  1350 */    16,  462,  468,  467,  455,  170,  450,  461,  425,   19,
 /*  1360 */    82,  389,  421,  163,  164,  389,   44,  389,  389,  389,
 /*  1370 */   389,  165,  166,  167,  168,  169,  389,  389,  389,   26,
 /*  1380 */   389,  389,  389,  389,  341,   29,   30,   31,   32,   33,
 /*  1390 */    34,   35,   36,   37,   38,   39,   40,   41,   42,   43,
 /*  1400 */   389,  389,  389,   44,  389,  389,  389,  389,  389,  389,
 /*  1410 */    91,  268,  270,  389,  389,  389,   52,  389,  389,  389,
 /*  1420 */   389,  341,   29,   30,   31,   32,   33,   34,   35,   36,
 /*  1430 */    37,   38,   39,   40,   41,   42,   43,   92,   93,   94,
 /*  1440 */    95,   96,  225,  250,  389,  238,  258,  259,  268,  270,
 /*  1450 */   389,  389,   44,  389,  100,  115,  119,  121,  123,  125,
 /*  1460 */   117,  103,  127,  129,  131,   56,  389,  389,  389,  389,
 /*  1470 */   341,   29,   30,   31,   32,   33,   34,   35,   36,   37,
 /*  1480 */    38,   39,   40,   41,   42,   43,   44,  389,  389,  389,
 /*  1490 */   389,  389,  389,  389,  389,  389,  389,  268,  270,   68,
 /*  1500 */   389,  389,  389,  389,  341,   29,   30,   31,   32,   33,
 /*  1510 */    34,   35,   36,   37,   38,   39,   40,   41,   42,   43,
 /*  1520 */    44,  389,  389,  389,  389,  389,  389,  389,  389,  389,
 /*  1530 */   389,  268,  270,   71,  389,  389,  389,  389,  341,   29,
 /*  1540 */    30,   31,   32,   33,   34,   35,   36,   37,   38,   39,
 /*  1550 */    40,   41,   42,   43,  389,  389,  389,  389,  389,  389,
 /*  1560 */   389,  389,   44,  389,  389,  268,  270,  389,  389,  389,
 /*  1570 */   389,  389,  389,  389,  389,   76,  389,  389,  389,  389,
 /*  1580 */   341,   29,   30,   31,   32,   33,   34,   35,   36,   37,
 /*  1590 */    38,   39,   40,   41,   42,   43,   44,  389,  389,  389,
 /*  1600 */   389,  389,  389,  389,  389,  389,  389,  268,  270,   83,
 /*  1610 */   389,  389,  389,  389,  341,   29,   30,   31,   32,   33,
 /*  1620 */    34,   35,   36,   37,   38,   39,   40,   41,   42,   43,
 /*  1630 */    44,  389,  389,  389,  389,  389,  389,  389,  389,  389,
 /*  1640 */   389,  268,  270,  345,  389,  389,  389,  389,  341,   29,
 /*  1650 */    30,   31,   32,   33,   34,   35,   36,   37,   38,   39,
 /*  1660 */    40,   41,   42,   43,  389,  389,  389,  389,  389,  389,
 /*  1670 */   389,  389,   44,  389,  389,  268,  270,  389,  389,  389,
 /*  1680 */   389,  389,  389,  389,  389,  352,  389,  389,  389,  389,
 /*  1690 */   341,   29,   30,   31,   32,   33,   34,   35,   36,   37,
 /*  1700 */    38,   39,   40,   41,   42,   43,   44,  389,  389,  389,
 /*  1710 */   389,  389,  389,  389,  389,  389,  389,  268,  270,  357,
 /*  1720 */   389,  389,  389,  389,  341,   29,   30,   31,   32,   33,
 /*  1730 */    34,   35,   36,   37,   38,   39,   40,   41,   42,   43,
 /*  1740 */    44,  389,  389,  389,  389,  389,  389,  389,  389,  389,
 /*  1750 */   389,  268,  270,  364,  389,  389,  389,  389,  341,   29,
 /*  1760 */    30,   31,   32,   33,   34,   35,   36,   37,   38,   39,
 /*  1770 */    40,   41,   42,   43,  389,  389,  389,  389,  389,  389,
 /*  1780 */   389,  389,   44,  389,  389,  268,  270,  389,  389,  389,
 /*  1790 */   389,  389,  389,  389,  389,  370,  389,  389,  389,  389,
 /*  1800 */   341,   29,   30,   31,   32,   33,   34,   35,   36,   37,
 /*  1810 */    38,   39,   40,   41,   42,   43,   44,  389,  389,  389,
 /*  1820 */   389,  389,  389,  389,  389,  389,  389,  268,  270,  377,
 /*  1830 */   389,  389,  389,  389,  341,   29,   30,   31,   32,   33,
 /*  1840 */    34,   35,   36,   37,   38,   39,   40,   41,   42,   43,
 /*  1850 */    44,  389,  389,  389,  389,  389,  389,  389,  389,  389,
 /*  1860 */   389,  268,  270,  413,  389,  389,  389,  389,  341,   29,
 /*  1870 */    30,   31,   32,   33,   34,   35,   36,   37,   38,   39,
 /*  1880 */    40,   41,   42,   43,  389,  389,  389,  389,  389,  389,
 /*  1890 */   389,  389,   44,  389,  389,  268,  270,  389,  389,  389,
 /*  1900 */   389,  389,  389,  389,  389,  419,  389,  389,  389,  389,
 /*  1910 */   341,   29,   30,   31,   32,   33,   34,   35,   36,   37,
 /*  1920 */    38,   39,   40,   41,   42,   43,   44,  389,  422,   17,
 /*  1930 */   437,  455,  389,  450,  461,  425,   19,  268,  270,  421,
 /*  1940 */   389,  389,  389,  389,   28,   29,   30,   31,   32,   33,
 /*  1950 */    34,   35,   36,   37,   38,   39,   40,   41,   42,   43,
 /*  1960 */   389,  389,  389,  389,  392,  104,  156,  160,  154,  192,
 /*  1970 */   101,  268,  270,  149,  389,  389,  389,  389,  389,  410,
 /*  1980 */   389,  381,  389,  162,  389,  137,  389,  104,  156,  160,
 /*  1990 */   154,  192,  101,  205,  389,  149,  138,  139,  140,  141,
 /*  2000 */   142,  143,  389,  389,  389,  162,  342,  137,  472,   16,
 /*  2010 */   462,  468,  467,  455,  170,  450,  461,  425,   19,  389,
 /*  2020 */   389,  421,  163,  164,  389,  389,  389,  389,  389,  389,
 /*  2030 */   165,  166,  167,  168,  169,  389,  170,  389,  389,  389,
 /*  2040 */   389,  389,  389,  389,  163,  164,  389,  389,  389,  389,
 /*  2050 */   389,  389,  165,  166,  167,  168,  169,  389,  389,  389,
 /*  2060 */   389,  389,  389,  389,  104,  156,  160,  154,  192,  101,
 /*  2070 */   195,  389,  149,  389,  389,  389,  389,  389,  389,  389,
 /*  2080 */   389,  389,  162,  389,  137,  104,  156,  160,  154,  192,
 /*  2090 */   101,  211,  389,  149,  389,  389,  389,  389,  188,  389,
 /*  2100 */   389,  389,  389,  162,  389,  137,  104,  156,  160,  154,
 /*  2110 */   192,  101,  223,  170,  149,  389,  389,  389,  389,  389,
 /*  2120 */   389,  163,  164,  389,  162,  389,  137,  389,  389,  165,
 /*  2130 */   166,  167,  168,  169,  170,  389,  389,  389,  389,  389,
 /*  2140 */   171,  389,  163,  164,  389,  389,  389,  190,  189,  389,
 /*  2150 */   165,  166,  167,  168,  169,  170,  183,  184,  186,  185,
 /*  2160 */   389,  389,  389,  163,  164,  389,  389,  389,  389,  389,
 /*  2170 */   389,  165,  166,  167,  168,  169,  389,  389,  389,  389,
 /*  2180 */   389,  389,  389,  104,  156,  160,  154,  192,  101,  389,
 /*  2190 */   389,  149,  389,  389,  389,  389,  389,  389,  389,  389,
 /*  2200 */   389,  162,  389,  137,  104,  156,  160,  154,  192,  101,
 /*  2210 */   389,  389,  149,  389,  389,  389,  389,  389,  389,  389,
 /*  2220 */   389,  389,  162,  389,  137,  389,  104,  156,  160,  154,
 /*  2230 */   192,  101,  170,  230,  149,  389,  389,  389,  389,  389,
 /*  2240 */   163,  164,  389,  389,  162,  389,  137,  389,  165,  166,
 /*  2250 */   167,  168,  169,  170,  243,  389,  389,  389,  389,  389,
 /*  2260 */   389,  163,  164,  389,  389,  389,  389,  389,  389,  165,
 /*  2270 */   166,  167,  168,  169,  389,  170,  251,  389,  389,  389,
 /*  2280 */   389,  389,  389,  163,  164,  398,  399,  400,  401,  389,
 /*  2290 */   389,  165,  166,  167,  168,  169,  104,  156,  160,  154,
 /*  2300 */   192,  101,  389,  389,  149,  389,  389,  389,  389,  389,
 /*  2310 */   389,  389,  389,  389,  162,  389,  137,  403,  104,  156,
 /*  2320 */   160,  154,  192,  101,  402,  404,  149,  389,  389,  389,
 /*  2330 */   389,  389,  389,  389,  389,  389,  162,  354,  137,  479,
 /*  2340 */    16,  462,  468,  467,  455,  170,  450,  461,  425,   19,
 /*  2350 */   389,  389,  421,  163,  164,  389,  389,  389,  389,  389,
 /*  2360 */   389,  165,  166,  167,  168,  169,  389,  170,  389,  389,
 /*  2370 */   389,  389,  389,  389,  389,  163,  164,  389,  389,  389,
 /*  2380 */   389,  389,  389,  165,  166,  167,  168,  169,  389,  389,
 /*  2390 */   133,  135,  100,  115,  119,  121,  123,  125,  117,  103,
 /*  2400 */   127,  129,  131,  389,  389,  389,  389,  389,  389,  145,
 /*  2410 */   389,  389,  389,  389,  389,  389,  133,  135,  100,  115,
 /*  2420 */   119,  121,  123,  125,  117,  103,  127,  129,  131,  133,
 /*  2430 */   135,  100,  115,  119,  121,  123,  125,  117,  103,  127,
 /*  2440 */   129,  131,  389,  389,  389,  389,  389,  389,  389,  389,
 /*  2450 */   389,  274,  389,  389,  389,  389,  389,  389,  133,  135,
 /*  2460 */   100,  115,  119,  121,  123,  125,  117,  103,  127,  129,
 /*  2470 */   131,  389,  389,  389,  389,  389,  389,  389,  389,  624,
 /*  2480 */   389,  389,  344,  389,  389,  389,  133,  135,  100,  115,
 /*  2490 */   119,  121,  123,  125,  117,  103,  127,  129,  131,  485,
 /*  2500 */    16,  462,  468,  467,  455,  389,  450,  461,  425,   19,
 /*  2510 */   351,  389,  421,  389,  389,  389,  389,  133,  135,  100,
 /*  2520 */   115,  119,  121,  123,  125,  117,  103,  127,  129,  131,
 /*  2530 */   389,  451,  452,  453,  454,  389,  389,  389,  389,  389,
 /*  2540 */   389,  356,  389,  389,  389,  389,  389,  389,  133,  135,
 /*  2550 */   100,  115,  119,  121,  123,  125,  117,  103,  127,  129,
 /*  2560 */   131,   15,  389,  403,  438,  389,  432,  389,  389,  389,
 /*  2570 */   402,  404,  722,  451,  452,  453,  454,  389,  389,  133,
 /*  2580 */   135,  100,  115,  119,  121,  123,  125,  117,  103,  127,
 /*  2590 */   129,  131,  398,  399,  400,  401,  389,  389,  389,  389,
 /*  2600 */   389,  389,  389,  505,  389,  403,  438,  389,  432,  389,
 /*  2610 */   389,  389,  402,  404,  451,  452,  453,  454,  389,  389,
 /*  2620 */   389,  389,  509,  389,  403,  426,  389,  432,  389,  389,
 /*  2630 */   389,  402,  404,  451,  452,  453,  454,  389,  389,  389,
 /*  2640 */   389,  389,  389,  389,  506,  389,  403,  438,  389,  432,
 /*  2650 */   389,  389,  389,  402,  404,  451,  452,  453,  454,  398,
 /*  2660 */   399,  400,  401,  474,  389,  403,  438,  389,  432,  389,
 /*  2670 */   389,  389,  402,  404,  451,  452,  453,  454,  398,  399,
 /*  2680 */   400,  401,  389,  389,  389,  481,  389,  403,  438,  508,
 /*  2690 */   432,  403,  405,  389,  402,  404,  389,  389,  402,  404,
 /*  2700 */   451,  452,  453,  454,  487,  389,  403,  438,  512,  432,
 /*  2710 */   403,  405,  389,  402,  404,  389,  389,  402,  404,  398,
 /*  2720 */   399,  400,  401,  398,  399,  400,  401,  389,  389,  389,
 /*  2730 */   510,  389,  403,  465,  398,  399,  400,  401,  389,  402,
 /*  2740 */   404,  389,  389,  389,  389,  389,  389,  389,  389,  513,
 /*  2750 */   389,  403,  405,  511,  389,  403,  405,  389,  402,  404,
 /*  2760 */   389,  389,  402,  404,  507,  389,  403,  405,  398,  399,
 /*  2770 */   400,  401,  389,  402,  404,  389,  389,  389,  389,  389,
 /*  2780 */   389,  389,  389,  389,  389,  389,  389,  389,  389,  389,
 /*  2790 */   389,  389,  389,  389,  389,  389,  389,  389,  389,  389,
 /*  2800 */   403,  389,  389,  427,  389,  389,  389,  402,  404,  451,
 /*  2810 */   452,  453,  454,  451,  452,  453,  454,  389,  389,  389,
 /*  2820 */   389,  389,  389,  389,  389,  389,  389,  389,  389,  389,
 /*  2830 */   389,  389,  389,  389,  389,  389,  389,  389,  389,  389,
 /*  2840 */   389,  403,  389,  389,  427,  403,  389,  389,  402,  404,
 /*  2850 */   389,  389,  402,  404,
};
static YYCODETYPE yy_lookahead[] = {
 /*     0 */    27,  106,  107,    5,   31,  110,    0,   34,   90,   91,
 /*    10 */    92,   93,   94,   95,   96,   42,   43,   44,   45,  133,
 /*    20 */    47,   48,   49,   50,  138,   52,   28,  141,  142,   56,
 /*    30 */    57,   58,   28,   98,   61,  100,  101,  102,   16,  104,
 /*    40 */   105,  106,  107,  131,  132,  110,   73,   74,   75,  134,
 /*    50 */   135,   78,   79,   80,   81,   82,   83,   27,   76,   77,
 /*    60 */    38,   31,    5,    5,   34,   27,   35,   85,   86,   87,
 /*    70 */    88,  103,   42,   43,   44,   45,  133,   47,   48,   49,
 /*    80 */    50,  138,   52,   25,  141,  142,   56,   57,   58,   39,
 /*    90 */    10,   61,   27,   62,   63,   64,   65,   66,   27,   68,
 /*   100 */   111,  112,  113,   73,   74,   75,   31,   27,   78,   79,
 /*   110 */    80,   81,   82,   83,   27,   27,   59,  112,   31,  151,
 /*   120 */     5,   34,   42,   43,   44,   45,   46,   47,   35,   42,
 /*   130 */    43,   44,   45,    5,   47,   48,   49,   50,  133,   52,
 /*   140 */    25,   76,   77,   56,   57,   58,  141,  142,   61,   27,
 /*   150 */    85,   86,   87,   88,   27,   62,   63,   64,   65,   66,
 /*   160 */    73,   74,   75,   27,   37,   78,   79,   80,   81,   82,
 /*   170 */    83,   27,   30,  133,   32,   31,  133,   30,   34,  139,
 /*   180 */   140,  141,  142,  140,  141,  142,   42,   43,   44,   45,
 /*   190 */    28,   47,   48,   49,   50,  133,   52,   69,   76,   77,
 /*   200 */    56,   57,   58,  141,  142,   61,  133,   85,   86,   87,
 /*   210 */    88,    5,   76,   77,  141,  142,  143,   73,   74,   75,
 /*   220 */    16,  148,   78,   79,   80,   81,   82,   83,   27,   30,
 /*   230 */   133,   25,   31,   26,  137,   34,   29,   35,  141,  142,
 /*   240 */    33,   34,   38,   42,   43,   44,   45,   16,   47,   48,
 /*   250 */    49,   50,  133,   52,    5,   28,  137,   56,   57,   58,
 /*   260 */   141,  142,   61,  133,   62,   63,   64,   65,   66,   38,
 /*   270 */    68,  141,  142,  143,   73,   74,   75,   28,  148,   78,
 /*   280 */    79,   80,   81,   82,   83,   27,   67,   68,  100,   31,
 /*   290 */    16,    5,   34,  105,  106,  107,   67,  103,  110,   70,
 /*   300 */    42,   43,   44,   45,   51,   47,   48,   49,   50,  133,
 /*   310 */    52,   25,   38,  137,   56,   57,   58,  141,  142,   61,
 /*   320 */   100,  101,  102,   30,  104,   23,  106,  107,    5,   27,
 /*   330 */   110,   73,   74,   75,  144,  145,   78,   79,   80,   81,
 /*   340 */    82,   83,   27,  149,  150,  151,   31,  107,   25,   34,
 /*   350 */   110,   30,   93,   94,   95,   96,   31,   42,   43,   44,
 /*   360 */    45,   54,   47,   48,   49,   50,  133,   52,    5,  132,
 /*   370 */   137,   56,   57,   58,  141,  142,   61,  133,   53,  103,
 /*   380 */    55,    1,    2,    3,    4,  141,  142,  143,   73,   74,
 /*   390 */    75,   28,  148,   78,   79,   80,   81,   82,   83,   27,
 /*   400 */   102,   30,  104,   31,   30,  107,   34,   27,  110,    1,
 /*   410 */     2,    3,    4,  103,   42,   43,   44,   45,   27,   47,
 /*   420 */    48,   49,   50,  147,   52,  149,  150,  151,   56,   57,
 /*   430 */    58,  144,  145,   61,  133,   27,  108,   39,    5,  111,
 /*   440 */   112,  113,  141,  142,  143,   73,   74,   75,   16,  148,
 /*   450 */    78,   79,   80,   81,   82,   83,   27,   16,   17,   18,
 /*   460 */    31,   28,    5,   34,    5,   67,   68,   30,   70,   32,
 /*   470 */    38,   42,   43,   44,   45,   27,   47,   48,   49,   50,
 /*   480 */   133,   52,   25,  133,   25,   56,   57,   58,  141,  142,
 /*   490 */    61,  141,  142,  143,  130,  148,  108,  133,  148,  111,
 /*   500 */   112,  113,   73,   74,   75,  141,  142,   78,   79,   80,
 /*   510 */    81,   82,   83,   27,  133,  130,  133,   31,  133,   16,
 /*   520 */    34,  138,  141,  142,  141,  142,  141,  142,   42,   43,
 /*   530 */    44,   45,  135,   47,   48,   49,   50,  130,   52,    5,
 /*   540 */   133,   38,   56,   57,   58,   28,  133,   61,  141,  142,
 /*   550 */   137,  136,   35,    5,  141,  142,  144,  145,   27,   73,
 /*   560 */    74,   75,   28,   27,   78,   79,   80,   81,   82,   83,
 /*   570 */    27,  133,  146,  133,   31,  137,   28,   34,   68,  141,
 /*   580 */   142,  141,  142,  144,  145,   42,   43,   44,   45,    5,
 /*   590 */    47,   48,   49,   50,  133,   52,  144,  145,  137,   56,
 /*   600 */    57,   58,  141,  142,   61,  133,   31,   76,   77,  137,
 /*   610 */     5,  133,   28,  141,  142,  137,   73,   74,   75,  141,
 /*   620 */   142,   78,   79,   80,   81,   82,   83,   27,   53,  133,
 /*   630 */    55,   31,  133,   28,   34,   69,   27,  141,  142,   25,
 /*   640 */   141,  142,   42,   43,   44,   45,  133,   47,   48,   49,
 /*   650 */    50,  133,   52,   27,  141,  142,   56,   57,   58,  141,
 /*   660 */   142,   61,  133,  144,  145,  144,  145,    5,  133,    5,
 /*   670 */   141,  142,   69,   73,   74,   75,  141,  142,   78,   79,
 /*   680 */    80,   81,   82,   83,   27,   76,   77,    5,   31,  133,
 /*   690 */    28,   34,   28,  144,  145,  144,  145,  141,  142,   42,
 /*   700 */    43,   44,   45,  133,   47,   48,   49,   50,   69,   52,
 /*   710 */    28,  141,  142,   56,   57,   58,  133,   39,   61,  133,
 /*   720 */   144,  145,   28,   27,  141,  142,    5,  141,  142,   35,
 /*   730 */    73,   74,   75,    5,  133,   78,   79,   80,   81,   82,
 /*   740 */    83,   27,  141,  142,  146,   31,   54,   54,   34,   28,
 /*   750 */    27,   54,   54,   27,   39,   39,   42,   43,   44,   45,
 /*   760 */   133,   47,   48,   49,   50,  133,   52,  136,  141,  142,
 /*   770 */    56,   57,   58,  141,  142,   61,  133,   69,   69,   69,
 /*   780 */   136,   69,  133,  136,  141,  142,   27,   73,   74,   75,
 /*   790 */   141,  142,   78,   79,   80,   81,   82,   83,   27,  136,
 /*   800 */    69,  136,   31,  136,  136,   34,   69,  136,   28,   28,
 /*   810 */    27,   27,   69,   42,   43,   44,   45,  133,   47,   48,
 /*   820 */    49,   50,  133,   52,   28,  141,  142,   56,   57,   58,
 /*   830 */   141,  142,   61,  133,   28,   69,   28,   69,   28,  133,
 /*   840 */    28,  141,  142,   27,   73,   74,   75,  141,  142,   78,
 /*   850 */    79,   80,   81,   82,   83,   27,   39,   28,   27,   31,
 /*   860 */    35,  103,   34,  145,   27,   27,   27,   54,   59,   51,
 /*   870 */    42,   43,   44,   45,   30,   47,   48,   49,   50,   30,
 /*   880 */    52,   35,  103,   27,   56,   57,   58,   35,  103,   61,
 /*   890 */    27,  103,   35,   27,   11,   27,  110,   27,   39,   30,
 /*   900 */    35,   73,   74,   75,   30,  103,   78,   79,   80,   81,
 /*   910 */    82,   83,   27,   37,   28,   37,   31,   35,  103,   34,
 /*   920 */    28,   28,   28,  104,  103,  103,   27,   42,   43,   44,
 /*   930 */    45,   30,   47,   48,   49,   50,   31,   52,   29,   27,
 /*   940 */    30,   56,   57,   58,   31,   27,   61,   31,  152,  152,
 /*   950 */   152,  152,  152,  152,  152,  152,  152,  152,   73,   74,
 /*   960 */    75,  152,  152,   78,   79,   80,   81,   82,   83,   27,
 /*   970 */   152,  152,  152,   31,  152,  152,   34,  152,  152,  152,
 /*   980 */   152,  152,  152,  152,   42,   43,   44,   45,  152,   47,
 /*   990 */    48,   49,   50,  152,   52,  152,  152,  152,   56,   57,
 /*  1000 */    58,  152,  152,   61,  152,  152,  152,  152,  152,  152,
 /*  1010 */   152,  152,  152,  152,  152,   73,   74,   75,  152,  152,
 /*  1020 */    78,   79,   80,   81,   82,   83,   27,  152,  152,  152,
 /*  1030 */    31,  152,  152,   34,  152,  152,  152,  152,  152,  152,
 /*  1040 */   152,   42,   43,   44,   45,  152,   47,   48,   49,   50,
 /*  1050 */   152,   52,  152,  152,  152,   56,   57,   58,  152,  152,
 /*  1060 */    61,  152,  152,  152,  152,  152,  152,  152,  152,  152,
 /*  1070 */   152,  152,   73,   74,   75,  152,  152,   78,   79,   80,
 /*  1080 */    81,   82,   83,   27,  152,  152,  152,   31,  152,  152,
 /*  1090 */    34,  152,  152,  152,  152,  152,  152,  152,   42,   43,
 /*  1100 */    44,   45,  152,   47,   48,   49,   50,  152,   52,  152,
 /*  1110 */   152,  152,   56,   57,   58,  152,  152,   61,  152,  152,
 /*  1120 */   152,  152,  152,  152,  152,  152,  152,  152,  152,   73,
 /*  1130 */    74,   75,  152,  152,   78,   79,   80,   81,   82,   83,
 /*  1140 */    27,  152,  152,  152,   31,  152,  152,   34,  152,  152,
 /*  1150 */   152,  152,  152,  152,  152,   42,   43,   44,   45,  152,
 /*  1160 */    47,   48,   49,   50,  152,   52,  152,  152,  152,   56,
 /*  1170 */    57,   58,  152,  152,   61,  152,  152,  152,  152,  152,
 /*  1180 */   152,  152,  152,  152,  152,  152,   73,   74,   75,  152,
 /*  1190 */   152,   78,   79,   80,   81,   82,   83,   27,  152,  152,
 /*  1200 */   152,   31,  152,  152,   34,  152,  152,  152,  152,  152,
 /*  1210 */   152,  152,   42,   43,   44,   45,  152,   47,   48,   49,
 /*  1220 */    50,  152,   52,  152,  152,  152,   56,   57,   58,  152,
 /*  1230 */   152,   61,  152,  152,  152,  152,  152,  152,  152,  152,
 /*  1240 */    27,  152,  152,   73,   74,   75,    0,   34,   78,   79,
 /*  1250 */    80,   81,   82,   83,  152,   42,   43,   44,   45,  152,
 /*  1260 */    47,   48,   49,   50,  152,   52,   10,  152,  152,   56,
 /*  1270 */    57,   58,   26,  152,   61,   29,  152,  152,  152,   33,
 /*  1280 */    34,   25,  152,   27,  152,  152,   73,   74,   75,  152,
 /*  1290 */   152,   78,   79,   80,   81,   82,   83,  152,   42,   43,
 /*  1300 */    44,   45,   46,   47,  152,  152,   19,   20,   21,   22,
 /*  1310 */    23,   24,  152,  152,   27,  152,  152,  152,  152,  152,
 /*  1320 */   152,  152,  152,  152,   37,  152,   39,  152,  152,   42,
 /*  1330 */    43,   44,   45,   46,   47,  152,    6,    7,    8,    9,
 /*  1340 */    10,   11,   12,   13,   14,   15,   16,   17,   18,   97,
 /*  1350 */    98,   99,  100,  101,  102,   68,  104,  105,  106,  107,
 /*  1360 */    30,  152,  110,   76,   77,  152,   96,  152,  152,  152,
 /*  1370 */   152,   84,   85,   86,   87,   88,  152,  152,  152,  109,
 /*  1380 */   152,  152,  152,  152,  114,  115,  116,  117,  118,  119,
 /*  1390 */   120,  121,  122,  123,  124,  125,  126,  127,  128,  129,
 /*  1400 */   152,  152,  152,   96,  152,  152,  152,  152,  152,  152,
 /*  1410 */    35,  141,  142,  152,  152,  152,  109,  152,  152,  152,
 /*  1420 */   152,  114,  115,  116,  117,  118,  119,  120,  121,  122,
 /*  1430 */   123,  124,  125,  126,  127,  128,  129,   62,   63,   64,
 /*  1440 */    65,   66,   67,   68,  152,   70,   71,   72,  141,  142,
 /*  1450 */   152,  152,   96,  152,    8,    9,   10,   11,   12,   13,
 /*  1460 */    14,   15,   16,   17,   18,  109,  152,  152,  152,  152,
 /*  1470 */   114,  115,  116,  117,  118,  119,  120,  121,  122,  123,
 /*  1480 */   124,  125,  126,  127,  128,  129,   96,  152,  152,  152,
 /*  1490 */   152,  152,  152,  152,  152,  152,  152,  141,  142,  109,
 /*  1500 */   152,  152,  152,  152,  114,  115,  116,  117,  118,  119,
 /*  1510 */   120,  121,  122,  123,  124,  125,  126,  127,  128,  129,
 /*  1520 */    96,  152,  152,  152,  152,  152,  152,  152,  152,  152,
 /*  1530 */   152,  141,  142,  109,  152,  152,  152,  152,  114,  115,
 /*  1540 */   116,  117,  118,  119,  120,  121,  122,  123,  124,  125,
 /*  1550 */   126,  127,  128,  129,  152,  152,  152,  152,  152,  152,
 /*  1560 */   152,  152,   96,  152,  152,  141,  142,  152,  152,  152,
 /*  1570 */   152,  152,  152,  152,  152,  109,  152,  152,  152,  152,
 /*  1580 */   114,  115,  116,  117,  118,  119,  120,  121,  122,  123,
 /*  1590 */   124,  125,  126,  127,  128,  129,   96,  152,  152,  152,
 /*  1600 */   152,  152,  152,  152,  152,  152,  152,  141,  142,  109,
 /*  1610 */   152,  152,  152,  152,  114,  115,  116,  117,  118,  119,
 /*  1620 */   120,  121,  122,  123,  124,  125,  126,  127,  128,  129,
 /*  1630 */    96,  152,  152,  152,  152,  152,  152,  152,  152,  152,
 /*  1640 */   152,  141,  142,  109,  152,  152,  152,  152,  114,  115,
 /*  1650 */   116,  117,  118,  119,  120,  121,  122,  123,  124,  125,
 /*  1660 */   126,  127,  128,  129,  152,  152,  152,  152,  152,  152,
 /*  1670 */   152,  152,   96,  152,  152,  141,  142,  152,  152,  152,
 /*  1680 */   152,  152,  152,  152,  152,  109,  152,  152,  152,  152,
 /*  1690 */   114,  115,  116,  117,  118,  119,  120,  121,  122,  123,
 /*  1700 */   124,  125,  126,  127,  128,  129,   96,  152,  152,  152,
 /*  1710 */   152,  152,  152,  152,  152,  152,  152,  141,  142,  109,
 /*  1720 */   152,  152,  152,  152,  114,  115,  116,  117,  118,  119,
 /*  1730 */   120,  121,  122,  123,  124,  125,  126,  127,  128,  129,
 /*  1740 */    96,  152,  152,  152,  152,  152,  152,  152,  152,  152,
 /*  1750 */   152,  141,  142,  109,  152,  152,  152,  152,  114,  115,
 /*  1760 */   116,  117,  118,  119,  120,  121,  122,  123,  124,  125,
 /*  1770 */   126,  127,  128,  129,  152,  152,  152,  152,  152,  152,
 /*  1780 */   152,  152,   96,  152,  152,  141,  142,  152,  152,  152,
 /*  1790 */   152,  152,  152,  152,  152,  109,  152,  152,  152,  152,
 /*  1800 */   114,  115,  116,  117,  118,  119,  120,  121,  122,  123,
 /*  1810 */   124,  125,  126,  127,  128,  129,   96,  152,  152,  152,
 /*  1820 */   152,  152,  152,  152,  152,  152,  152,  141,  142,  109,
 /*  1830 */   152,  152,  152,  152,  114,  115,  116,  117,  118,  119,
 /*  1840 */   120,  121,  122,  123,  124,  125,  126,  127,  128,  129,
 /*  1850 */    96,  152,  152,  152,  152,  152,  152,  152,  152,  152,
 /*  1860 */   152,  141,  142,  109,  152,  152,  152,  152,  114,  115,
 /*  1870 */   116,  117,  118,  119,  120,  121,  122,  123,  124,  125,
 /*  1880 */   126,  127,  128,  129,  152,  152,  152,  152,  152,  152,
 /*  1890 */   152,  152,   96,  152,  152,  141,  142,  152,  152,  152,
 /*  1900 */   152,  152,  152,  152,  152,  109,  152,  152,  152,  152,
 /*  1910 */   114,  115,  116,  117,  118,  119,  120,  121,  122,  123,
 /*  1920 */   124,  125,  126,  127,  128,  129,   96,  152,   99,  100,
 /*  1930 */   101,  102,  152,  104,  105,  106,  107,  141,  142,  110,
 /*  1940 */   152,  152,  152,  152,  114,  115,  116,  117,  118,  119,
 /*  1950 */   120,  121,  122,  123,  124,  125,  126,  127,  128,  129,
 /*  1960 */   152,  152,  152,  152,   10,   19,   20,   21,   22,   23,
 /*  1970 */    24,  141,  142,   27,  152,  152,  152,  152,  152,   25,
 /*  1980 */   152,   27,  152,   37,  152,   39,  152,   19,   20,   21,
 /*  1990 */    22,   23,   24,   25,  152,   27,   42,   43,   44,   45,
 /*  2000 */    46,   47,  152,  152,  152,   37,   60,   39,   97,   98,
 /*  2010 */    99,  100,  101,  102,   68,  104,  105,  106,  107,  152,
 /*  2020 */   152,  110,   76,   77,  152,  152,  152,  152,  152,  152,
 /*  2030 */    84,   85,   86,   87,   88,  152,   68,  152,  152,  152,
 /*  2040 */   152,  152,  152,  152,   76,   77,  152,  152,  152,  152,
 /*  2050 */   152,  152,   84,   85,   86,   87,   88,  152,  152,  152,
 /*  2060 */   152,  152,  152,  152,   19,   20,   21,   22,   23,   24,
 /*  2070 */    25,  152,   27,  152,  152,  152,  152,  152,  152,  152,
 /*  2080 */   152,  152,   37,  152,   39,   19,   20,   21,   22,   23,
 /*  2090 */    24,   25,  152,   27,  152,  152,  152,  152,   27,  152,
 /*  2100 */   152,  152,  152,   37,  152,   39,   19,   20,   21,   22,
 /*  2110 */    23,   24,   25,   68,   27,  152,  152,  152,  152,  152,
 /*  2120 */   152,   76,   77,  152,   37,  152,   39,  152,  152,   84,
 /*  2130 */    85,   86,   87,   88,   68,  152,  152,  152,  152,  152,
 /*  2140 */    69,  152,   76,   77,  152,  152,  152,   76,   77,  152,
 /*  2150 */    84,   85,   86,   87,   88,   68,   85,   86,   87,   88,
 /*  2160 */   152,  152,  152,   76,   77,  152,  152,  152,  152,  152,
 /*  2170 */   152,   84,   85,   86,   87,   88,  152,  152,  152,  152,
 /*  2180 */   152,  152,  152,   19,   20,   21,   22,   23,   24,  152,
 /*  2190 */   152,   27,  152,  152,  152,  152,  152,  152,  152,  152,
 /*  2200 */   152,   37,  152,   39,   19,   20,   21,   22,   23,   24,
 /*  2210 */   152,  152,   27,  152,  152,  152,  152,  152,  152,  152,
 /*  2220 */   152,  152,   37,  152,   39,  152,   19,   20,   21,   22,
 /*  2230 */    23,   24,   68,   69,   27,  152,  152,  152,  152,  152,
 /*  2240 */    76,   77,  152,  152,   37,  152,   39,  152,   84,   85,
 /*  2250 */    86,   87,   88,   68,   69,  152,  152,  152,  152,  152,
 /*  2260 */   152,   76,   77,  152,  152,  152,  152,  152,  152,   84,
 /*  2270 */    85,   86,   87,   88,  152,   68,   69,  152,  152,  152,
 /*  2280 */   152,  152,  152,   76,   77,    1,    2,    3,    4,  152,
 /*  2290 */   152,   84,   85,   86,   87,   88,   19,   20,   21,   22,
 /*  2300 */    23,   24,  152,  152,   27,  152,  152,  152,  152,  152,
 /*  2310 */   152,  152,  152,  152,   37,  152,   39,   33,   19,   20,
 /*  2320 */    21,   22,   23,   24,   40,   41,   27,  152,  152,  152,
 /*  2330 */   152,  152,  152,  152,  152,  152,   37,   60,   39,   97,
 /*  2340 */    98,   99,  100,  101,  102,   68,  104,  105,  106,  107,
 /*  2350 */   152,  152,  110,   76,   77,  152,  152,  152,  152,  152,
 /*  2360 */   152,   84,   85,   86,   87,   88,  152,   68,  152,  152,
 /*  2370 */   152,  152,  152,  152,  152,   76,   77,  152,  152,  152,
 /*  2380 */   152,  152,  152,   84,   85,   86,   87,   88,  152,  152,
 /*  2390 */     6,    7,    8,    9,   10,   11,   12,   13,   14,   15,
 /*  2400 */    16,   17,   18,  152,  152,  152,  152,  152,  152,   25,
 /*  2410 */   152,  152,  152,  152,  152,  152,    6,    7,    8,    9,
 /*  2420 */    10,   11,   12,   13,   14,   15,   16,   17,   18,    6,
 /*  2430 */     7,    8,    9,   10,   11,   12,   13,   14,   15,   16,
 /*  2440 */    17,   18,  152,  152,  152,  152,  152,  152,  152,  152,
 /*  2450 */   152,   28,  152,  152,  152,  152,  152,  152,    6,    7,
 /*  2460 */     8,    9,   10,   11,   12,   13,   14,   15,   16,   17,
 /*  2470 */    18,  152,  152,  152,  152,  152,  152,  152,  152,   69,
 /*  2480 */   152,  152,   30,  152,  152,  152,    6,    7,    8,    9,
 /*  2490 */    10,   11,   12,   13,   14,   15,   16,   17,   18,   97,
 /*  2500 */    98,   99,  100,  101,  102,  152,  104,  105,  106,  107,
 /*  2510 */    30,  152,  110,  152,  152,  152,  152,    6,    7,    8,
 /*  2520 */     9,   10,   11,   12,   13,   14,   15,   16,   17,   18,
 /*  2530 */   152,    1,    2,    3,    4,  152,  152,  152,  152,  152,
 /*  2540 */   152,   30,  152,  152,  152,  152,  152,  152,    6,    7,
 /*  2550 */     8,    9,   10,   11,   12,   13,   14,   15,   16,   17,
 /*  2560 */    18,   31,  152,   33,   34,  152,   36,  152,  152,  152,
 /*  2570 */    40,   41,   30,    1,    2,    3,    4,  152,  152,    6,
 /*  2580 */     7,    8,    9,   10,   11,   12,   13,   14,   15,   16,
 /*  2590 */    17,   18,    1,    2,    3,    4,  152,  152,  152,  152,
 /*  2600 */   152,  152,  152,   31,  152,   33,   34,  152,   36,  152,
 /*  2610 */   152,  152,   40,   41,    1,    2,    3,    4,  152,  152,
 /*  2620 */   152,  152,   31,  152,   33,   34,  152,   36,  152,  152,
 /*  2630 */   152,   40,   41,    1,    2,    3,    4,  152,  152,  152,
 /*  2640 */   152,  152,  152,  152,   31,  152,   33,   34,  152,   36,
 /*  2650 */   152,  152,  152,   40,   41,    1,    2,    3,    4,    1,
 /*  2660 */     2,    3,    4,   31,  152,   33,   34,  152,   36,  152,
 /*  2670 */   152,  152,   40,   41,    1,    2,    3,    4,    1,    2,
 /*  2680 */     3,    4,  152,  152,  152,   31,  152,   33,   34,   31,
 /*  2690 */    36,   33,   34,  152,   40,   41,  152,  152,   40,   41,
 /*  2700 */     1,    2,    3,    4,   31,  152,   33,   34,   31,   36,
 /*  2710 */    33,   34,  152,   40,   41,  152,  152,   40,   41,    1,
 /*  2720 */     2,    3,    4,    1,    2,    3,    4,  152,  152,  152,
 /*  2730 */    31,  152,   33,   34,    1,    2,    3,    4,  152,   40,
 /*  2740 */    41,  152,  152,  152,  152,  152,  152,  152,  152,   31,
 /*  2750 */   152,   33,   34,   31,  152,   33,   34,  152,   40,   41,
 /*  2760 */   152,  152,   40,   41,   31,  152,   33,   34,    1,    2,
 /*  2770 */     3,    4,  152,   40,   41,  152,  152,  152,  152,  152,
 /*  2780 */   152,  152,  152,  152,  152,  152,  152,  152,  152,  152,
 /*  2790 */   152,  152,  152,  152,  152,  152,  152,  152,  152,  152,
 /*  2800 */    33,  152,  152,   36,  152,  152,  152,   40,   41,    1,
 /*  2810 */     2,    3,    4,    1,    2,    3,    4,  152,  152,  152,
 /*  2820 */   152,  152,  152,  152,  152,  152,  152,  152,  152,  152,
 /*  2830 */   152,  152,  152,  152,  152,  152,  152,  152,  152,  152,
 /*  2840 */   152,   33,  152,  152,   36,   33,  152,  152,   40,   41,
 /*  2850 */   152,  152,   40,   41,
};
#define YY_SHIFT_USE_DFLT (-28)
static short yy_shift_ofst[] = {
 /*     0 */   207,    6, 1246,  -28,  -28,  -28,  -28,   38,    4,  -28,
 /*    10 */    71,  142, 2530,   75,  -28,  -28, 2572, 2658,  -28,   22,
 /*    20 */    88,   50, 1256,  147,  -27,  -28,   30,  -28,  -28,  -28,
 /*    30 */   -28,  -28,  -28,  -28,  -28,  -28,  -28,  -28,  -28,  -28,
 /*    40 */   -28,  -28,  -28,  -28,  -28,  162,  -28,  227,  -28, 2299,
 /*    50 */   199,   87,  144,  253,  293,  201,  258,  -28, 2299,  321,
 /*    60 */   325,  -28,  575,  -28,  -28,  -18,  307, 1213, 1213,  371,
 /*    70 */   315,  372,  -28, 2299,  374,  429,  486,  -28,  391,   57,
 /*    80 */  1946, 1330, 1213,  543,  -28,  448,   -2,  -28,  448,  -28,
 /*    90 */  1375,  -28,  -28,  -28,  -28,  -28,  -28, 2299,  -28, 2573,
 /*   100 */  2299, 2299,  -28, 2299,  536,  -28,  510,  136,  566,  -28,
 /*   110 */   603,  -28,  639,  -28,  441, 2299,  441, 2299,  441, 2299,
 /*   120 */   441, 2299,  441, 2299,  441, 2299,  441, 2299,  -28, 2299,
 /*   130 */   -28, 2299,  -28, 2299, 1446, 2299, 1446, 1287,  -28,  -28,
 /*   140 */   -28,  -28,  -28,  -28, 2384,  -28,  614, 2299,  -28,  398,
 /*   150 */   626,  678, 1968, 2573, 2299,  -28,  696,  728,  536,  -28,
 /*   160 */  2299,  -28,  -28,  -28,  -28,  -28,  -28,  -28,  -28,  -28,
 /*   170 */  2071,  -28,  128,  -28,   65,  -28,  692,  122,  -28,  -28,
 /*   180 */   -28,  -28,  -28,  -28,  -28,  -28,  -28,  -28,  693,  697,
 /*   190 */   698,  -28,  723,  715, 2045,  -28,   58,  -28, 2299,  -28,
 /*   200 */   -28,  -28,  -28,  115,  -28,  -28,  127,  716, 2066,  206,
 /*   210 */   -28,  -28,  -28,  531,  708,  -28,  709,  -28,  710,  -28,
 /*   220 */  2087,  286,  -28,  -28,  441,  726,   31, 2299,  -28, 2164,
 /*   230 */    93, 2299,  -28,  712,   93, 2299,  -28, 2410,  759,  202,
 /*   240 */  2299,  -28, 2185,   93, 2299,  -28,  731,   93, 2299,  -28,
 /*   250 */  2207,   93, 2299,  -28,  737,   93, 2299,  -28,  -28,  -28,
 /*   260 */   -28, 2299,  249,  -28, 2299,  -28, 2573,  -28,  780,  -28,
 /*   270 */   781,  -28, 2299, 2423,  -28,  783,  219,  784,  796,  -28,
 /*   280 */   609,  743,  806,  -28,  766,  808,  -28,  768,  810,  -28,
 /*   290 */   302,  812,  -28,  816,  817, 2299,  323,  829,  -28,  831,
 /*   300 */   363,  -28,  831,  -28,  825,  -18,  -28,  -28,  831,  433,
 /*   310 */   -28,  831,  534,  -28,  831,  548,  -28,  831,  584,  -28,
 /*   320 */   831,  605,  -28,  831,  662,  -28,  831,  664,  -28,  831,
 /*   330 */   682,  -28,  831,  721,  -28,  229,  837,  716,  838,  678,
 /*   340 */   -28,  -28, 2299, 2452, 1213,  600,  -28,  839,  809, 2277,
 /*   350 */  2480, 1213,  657,  -28, 2299, 2511, 1213,  714,  -28,  -28,
 /*   360 */  2542,  -28,  813, 1213, 1213,  -28,  -28,  818,  844,  771,
 /*   370 */   828,  -28,  -28,  457,  849,  885,  -28,  942,  -28,   80,
 /*   380 */   -28,  846,  -18,  -28,  856,  852,  -18,  -28,  863,  857,
 /*   390 */   -18,  -28,  866,  883,  868,  -28, 2284,  -28,  -28,  -28,
 /*   400 */   -28,  -28,  -28,  -28,  -28, 2284,  204,  870,  859, 1954,
 /*   410 */   869,  999,  -28, 1056,  -28,  459,  874, 1113,  -28, 1170,
 /*   420 */   -28,  -28, 2591, 2677,  -28,  -28, 2767,  876,  865,  -18,
 /*   430 */   886,  -28,  878,  882,  -18,  892,  -28,  -28, 2808,  380,
 /*   440 */   517,  -28,  -18,  893,  -28,  -28,  -28,  -28,  -28,  -28,
 /*   450 */   -28,  231,  274,  432,  503,  408,  694,  -28,  -18,  894,
 /*   460 */   -28,  -28, 2613, 2699, 2718, 2812, 2722,  -28, 2733,  899,
 /*   470 */   901, 2632,  905,  -28,  -28,  909,  912,  437, 2654,  913,
 /*   480 */   -28,  -28,  918,  910, 2673,  916,  -28,  -28,  -28,
};
#define YY_REDUCE_USE_DFLT (-115)
static short yy_reduce_ofst[] = {
 /*     0 */   -82, -115,  259, -115, -115, -115, -115, -115, -115, -115,
 /*    10 */  -115, -115, 1252, -115, -115, -115, 1829, -105, -115, -115,
 /*    20 */  -115, -115,  328, -115, 1270, -115, 1830, -115, -115, -115,
 /*    30 */  -115, -115, -115, -115, -115, -115, -115, -115, -115, -115,
 /*    40 */  -115, -115, -115, -115, -115, -115, -115, -115, -115,  364,
 /*    50 */  -115, 1307, 1830, -115, -115, 1356, 1830, -115,  385, -115,
 /*    60 */   -88, -115,  237, -115, -115,  310, -115, 1390, 1830, -115,
 /*    70 */  1424, 1830, -115,  407, -115, 1466, 1830, -115, -115, -115,
 /*    80 */    62, -115, 1500, 1830, -115,  -85, -115, -115,  397, -115,
 /*    90 */   415, -115, -115, -115, -115, -115, -115,   97, -115, -115,
 /*   100 */   381,  440, -115,  496,  426, -115, -115, -115, -115, -115,
 /*   110 */  -115, -115, -115, -115, -115,  499, -115,  513, -115,  518,
 /*   120 */  -115,  529, -115,  535, -115,  556, -115,  570, -115,  583,
 /*   130 */  -115,  586, -115,  601, -115,  627, -115,    5, -115, -115,
 /*   140 */  -115, -115, -115, -115, -115, -115, -115,  632, -115, -115,
 /*   150 */  -115, -115,   73, -115,  643, -115, -115, -115,  598, -115,
 /*   160 */   649, -115, -115, -115, -115, -115, -115, -115, -115, -115,
 /*   170 */   276, -115, -115, -115,  194, -115, -115,  -32, -115, -115,
 /*   180 */  -115, -115, -115, -115, -115, -115, -115, -115, -115, -115,
 /*   190 */  -115, -115, -115, -115,  130, -115, -115, -115,  347, -115,
 /*   200 */  -115, -115, -115, -115, -115, -115, -115, -115,  244, -115,
 /*   210 */  -115, -115, -115, -115, -115, -115, -115, -115, -115, -115,
 /*   220 */   301, -115, -115, -115, -115, -115,  631,  119, -115, -114,
 /*   230 */   644,  176, -115, -115,  647,  233, -115, -115, -115,  663,
 /*   240 */   413, -115,  -57,  665,  438, -115, -115,  667,  461, -115,
 /*   250 */   383,  668,  472, -115, -115,  671,  478, -115, -115, -115,
 /*   260 */  -115,   40, -115, -115,   43, -115, -115, -115, -115, -115,
 /*   270 */  -115, -115,  684, -115, -115, -115, -115, -115, -115, -115,
 /*   280 */  -115, -115, -115, -115, -115, -115, -115, -115, -115, -115,
 /*   290 */  -115, -115, -115, -115, -115,  350, -115, -115, -115,  190,
 /*   300 */  -115, -115,  718, -115, -115,  758, -115, -115,  287, -115,
 /*   310 */  -115,  412, -115, -115,  439, -115, -115,  452, -115, -115,
 /*   320 */   519, -115, -115,  521, -115, -115,  549, -115, -115,  551,
 /*   330 */  -115, -115,  576, -115, -115, -115, -115, -115, -115, -115,
 /*   340 */  -115, -115,  689, -115, 1534, 1830, -115, -115, -115,  700,
 /*   350 */  -115, 1576, 1830, -115,  706, -115, 1610, 1830, -115, -115,
 /*   360 */  -115, -115, -115, 1644, 1830, -115, -115, -115, -115, 1686,
 /*   370 */  1830, -115, -115, -115, -115, 1720, -115, 1830, -115,  -11,
 /*   380 */  -115, -115,  779, -115, -115, -115,  785, -115, -115, -115,
 /*   390 */   788, -115, -115, -115, -115, -115,  786, -115, -115, -115,
 /*   400 */  -115, -115, -115, -115, -115,  240, -115, -115, -115,  388,
 /*   410 */  -115, 1754, -115, 1830, -115, -115, -115, 1796, -115, 1830,
 /*   420 */  -115, -115,  188, -105, -115, -115,  240, -115, -115,  802,
 /*   430 */  -115, -115, -115, -115,  815, -115, -115, -115,  298,  819,
 /*   440 */  -115, -115,  821, -115, -115, -115, -115, -115, -115, -115,
 /*   450 */  -115, -115, -115, -115, -115,  819, -115, -115,  822, -115,
 /*   460 */  -115, -115,  -65,  220, -105,  298, -105, -115, -105, -115,
 /*   470 */  -115, 1911, -115, -115, -115, -115, -115, -115, 2242, -115,
 /*   480 */  -115, -115, -115, -115, 2402, -115, -115, -115, -115,
};
static YYACTIONTYPE yy_default[] = {
 /*     0 */   724,  724,  724,  491,  493,  494,  495,  724,  724,  496,
 /*    10 */   724,  724,  724,  724,  497,  501,  724,  724,  528,  724,
 /*    20 */   724,  724,  724,  724,  724,  532,  724,  534,  564,  566,
 /*    30 */   567,  568,  569,  570,  571,  572,  573,  574,  575,  576,
 /*    40 */   577,  578,  579,  580,  581,  724,  582,  724,  583,  724,
 /*    50 */   724,  724,  724,  586,  724,  724,  724,  587,  724,  724,
 /*    60 */   724,  590,  724,  591,  592,  724,  724,  724,  594,  724,
 /*    70 */   724,  724,  597,  724,  724,  724,  724,  599,  724,  724,
 /*    80 */   724,  724,  724,  724,  600,  724,  724,  604,  724,  605,
 /*    90 */   724,  607,  608,  609,  610,  611,  612,  724,  613,  652,
 /*   100 */   724,  724,  653,  724,  724,  654,  724,  724,  724,  719,
 /*   110 */   724,  720,  724,  721,  656,  724,  657,  724,  658,  724,
 /*   120 */   659,  724,  660,  724,  661,  724,  662,  724,  671,  724,
 /*   130 */   672,  724,  673,  724,  674,  724,  675,  724,  558,  559,
 /*   140 */   560,  561,  562,  563,  724,  663,  724,  724,  664,  680,
 /*   150 */   724,  665,  724,  703,  724,  676,  724,  724,  724,  677,
 /*   160 */   724,  678,  679,  681,  682,  683,  684,  685,  686,  687,
 /*   170 */   724,  688,  724,  689,  724,  704,  724,  724,  706,  711,
 /*   180 */   712,  713,  714,  715,  716,  717,  718,  707,  712,  714,
 /*   190 */   713,  705,  724,  690,  724,  691,  724,  692,  724,  695,
 /*   200 */   696,  701,  702,  724,  699,  700,  724,  666,  724,  724,
 /*   210 */   697,  698,  667,  724,  724,  668,  724,  669,  724,  670,
 /*   220 */   724,  724,  693,  694,  655,  724,  724,  724,  614,  724,
 /*   230 */   724,  724,  615,  724,  724,  724,  616,  724,  724,  724,
 /*   240 */   724,  617,  724,  724,  724,  618,  724,  724,  724,  619,
 /*   250 */   724,  724,  724,  620,  724,  724,  724,  621,  622,  623,
 /*   260 */   606,  724,  724,  625,  724,  626,  628,  627,  724,  629,
 /*   270 */   724,  630,  724,  724,  631,  724,  724,  724,  724,  632,
 /*   280 */   724,  724,  724,  633,  724,  724,  634,  724,  724,  635,
 /*   290 */   724,  724,  636,  724,  724,  724,  724,  724,  637,  724,
 /*   300 */   724,  638,  724,  648,  650,  724,  651,  649,  724,  724,
 /*   310 */   639,  724,  724,  640,  724,  724,  641,  724,  724,  642,
 /*   320 */   724,  724,  643,  724,  724,  644,  724,  724,  645,  724,
 /*   330 */   724,  646,  724,  724,  647,  724,  724,  724,  724,  724,
 /*   340 */   723,  565,  724,  724,  724,  724,  601,  724,  724,  724,
 /*   350 */   724,  724,  724,  602,  724,  724,  724,  724,  603,  598,
 /*   360 */   724,  596,  724,  724,  595,  593,  588,  584,  724,  724,
 /*   370 */   724,  589,  585,  724,  724,  724,  533,  724,  535,  724,
 /*   380 */   549,  551,  724,  554,  724,  552,  724,  555,  724,  553,
 /*   390 */   724,  556,  724,  724,  724,  550,  724,  540,  542,  543,
 /*   400 */   544,  545,  546,  547,  548,  724,  724,  724,  724,  724,
 /*   410 */   724,  724,  536,  724,  538,  724,  724,  724,  537,  724,
 /*   420 */   539,  541,  724,  724,  526,  529,  724,  724,  724,  724,
 /*   430 */   724,  530,  724,  724,  724,  724,  531,  514,  724,  724,
 /*   440 */   724,  516,  724,  724,  518,  520,  522,  523,  524,  525,
 /*   450 */   521,  522,  523,  524,  525,  724,  724,  517,  724,  724,
 /*   460 */   519,  527,  724,  724,  724,  724,  724,  515,  724,  724,
 /*   470 */   724,  724,  724,  498,  502,  724,  724,  724,  724,  724,
 /*   480 */   499,  503,  724,  724,  724,  724,  500,  504,  492,
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
  "SBRACKET_OPEN",  "SBRACKET_CLOSE",  "DOUBLECOLON",   "INCR",        
  "DECR",          "ECHO",          "RETURN",        "UNSET",       
  "INTEGER",       "STRING",        "THROW",         "TYPE_UINTEGER",
  "TYPE_CHAR",     "TYPE_UCHAR",    "TYPE_LONG",     "TYPE_ULONG",  
  "SCHAR",         "DOUBLE",        "NULL",          "TRUE",        
  "FALSE",         "error",         "program",       "xx_language", 
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
 /* 128 */ "xx_let_assignment ::= IDENTIFIER DOUBLECOLON IDENTIFIER xx_assignment_operator xx_assign_expr",
 /* 129 */ "xx_let_assignment ::= IDENTIFIER DOUBLECOLON IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 130 */ "xx_let_assignment ::= IDENTIFIER DOUBLECOLON IDENTIFIER SBRACKET_OPEN xx_index_expr SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 131 */ "xx_let_assignment ::= IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 132 */ "xx_let_assignment ::= IDENTIFIER SBRACKET_OPEN xx_index_expr SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 133 */ "xx_let_assignment ::= IDENTIFIER INCR",
 /* 134 */ "xx_let_assignment ::= IDENTIFIER DECR",
 /* 135 */ "xx_index_expr ::= xx_common_expr",
 /* 136 */ "xx_echo_statement ::= ECHO xx_echo_expressions DOTCOMMA",
 /* 137 */ "xx_echo_expressions ::= xx_echo_expressions COMMA xx_echo_expression",
 /* 138 */ "xx_echo_expressions ::= xx_echo_expression",
 /* 139 */ "xx_echo_expression ::= xx_common_expr",
 /* 140 */ "xx_mcall_statement ::= xx_mcall_expr DOTCOMMA",
 /* 141 */ "xx_scall_statement ::= xx_scall_expr DOTCOMMA",
 /* 142 */ "xx_return_statement ::= RETURN xx_common_expr DOTCOMMA",
 /* 143 */ "xx_unset_statement ::= UNSET IDENTIFIER ARROW IDENTIFIER DOTCOMMA",
 /* 144 */ "xx_unset_statement ::= UNSET IDENTIFIER SBRACKET_OPEN IDENTIFIER SBRACKET_CLOSE DOTCOMMA",
 /* 145 */ "xx_unset_statement ::= UNSET IDENTIFIER SBRACKET_OPEN INTEGER SBRACKET_CLOSE DOTCOMMA",
 /* 146 */ "xx_unset_statement ::= UNSET IDENTIFIER SBRACKET_OPEN STRING SBRACKET_CLOSE DOTCOMMA",
 /* 147 */ "xx_throw_statement ::= THROW IDENTIFIER DOTCOMMA",
 /* 148 */ "xx_throw_statement ::= THROW NEW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE DOTCOMMA",
 /* 149 */ "xx_declare_statement ::= TYPE_INTEGER xx_declare_variable_list DOTCOMMA",
 /* 150 */ "xx_declare_statement ::= TYPE_UINTEGER xx_declare_variable_list DOTCOMMA",
 /* 151 */ "xx_declare_statement ::= TYPE_CHAR xx_declare_variable_list DOTCOMMA",
 /* 152 */ "xx_declare_statement ::= TYPE_UCHAR xx_declare_variable_list DOTCOMMA",
 /* 153 */ "xx_declare_statement ::= TYPE_LONG xx_declare_variable_list DOTCOMMA",
 /* 154 */ "xx_declare_statement ::= TYPE_ULONG xx_declare_variable_list DOTCOMMA",
 /* 155 */ "xx_declare_statement ::= TYPE_DOUBLE xx_declare_variable_list DOTCOMMA",
 /* 156 */ "xx_declare_statement ::= TYPE_STRING xx_declare_variable_list DOTCOMMA",
 /* 157 */ "xx_declare_statement ::= TYPE_BOOL xx_declare_variable_list DOTCOMMA",
 /* 158 */ "xx_declare_statement ::= TYPE_VAR xx_declare_variable_list DOTCOMMA",
 /* 159 */ "xx_declare_variable_list ::= xx_declare_variable_list COMMA xx_declare_variable",
 /* 160 */ "xx_declare_variable_list ::= xx_declare_variable",
 /* 161 */ "xx_declare_variable ::= IDENTIFIER",
 /* 162 */ "xx_declare_variable ::= IDENTIFIER ASSIGN xx_literal_expr",
 /* 163 */ "xx_assign_expr ::= xx_common_expr",
 /* 164 */ "xx_common_expr ::= NOT xx_common_expr",
 /* 165 */ "xx_common_expr ::= ISSET xx_isset_expr",
 /* 166 */ "xx_common_expr ::= xx_common_expr EQUALS xx_common_expr",
 /* 167 */ "xx_common_expr ::= xx_common_expr NOTEQUALS xx_common_expr",
 /* 168 */ "xx_common_expr ::= xx_common_expr IDENTICAL xx_common_expr",
 /* 169 */ "xx_common_expr ::= xx_common_expr NOTIDENTICAL xx_common_expr",
 /* 170 */ "xx_common_expr ::= xx_common_expr LESS xx_common_expr",
 /* 171 */ "xx_common_expr ::= xx_common_expr GREATER xx_common_expr",
 /* 172 */ "xx_common_expr ::= xx_common_expr LESSEQUAL xx_common_expr",
 /* 173 */ "xx_common_expr ::= xx_common_expr GREATEREQUAL xx_common_expr",
 /* 174 */ "xx_common_expr ::= PARENTHESES_OPEN xx_common_expr PARENTHESES_CLOSE",
 /* 175 */ "xx_common_expr ::= PARENTHESES_OPEN xx_parameter_type PARENTHESES_CLOSE xx_common_expr",
 /* 176 */ "xx_common_expr ::= IDENTIFIER ARROW IDENTIFIER",
 /* 177 */ "xx_common_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER",
 /* 178 */ "xx_common_expr ::= IDENTIFIER DOUBLECOLON CONSTANT",
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
 /* 190 */ "xx_common_expr ::= CONSTANT",
 /* 191 */ "xx_common_expr ::= IDENTIFIER",
 /* 192 */ "xx_common_expr ::= INTEGER",
 /* 193 */ "xx_common_expr ::= STRING",
 /* 194 */ "xx_common_expr ::= SCHAR",
 /* 195 */ "xx_common_expr ::= DOUBLE",
 /* 196 */ "xx_common_expr ::= NULL",
 /* 197 */ "xx_common_expr ::= TRUE",
 /* 198 */ "xx_common_expr ::= FALSE",
 /* 199 */ "xx_common_expr ::= SBRACKET_OPEN SBRACKET_CLOSE",
 /* 200 */ "xx_common_expr ::= SBRACKET_OPEN xx_array_list SBRACKET_CLOSE",
 /* 201 */ "xx_common_expr ::= NEW IDENTIFIER",
 /* 202 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 203 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 204 */ "xx_common_expr ::= IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 205 */ "xx_common_expr ::= IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 206 */ "xx_common_expr ::= xx_mcall_expr",
 /* 207 */ "xx_common_expr ::= xx_scall_expr",
 /* 208 */ "xx_scall_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 209 */ "xx_scall_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 210 */ "xx_mcall_expr ::= IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 211 */ "xx_mcall_expr ::= IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 212 */ "xx_call_parameters ::= xx_call_parameters COMMA xx_call_parameter",
 /* 213 */ "xx_call_parameters ::= xx_call_parameter",
 /* 214 */ "xx_call_parameter ::= xx_common_expr",
 /* 215 */ "xx_array_list ::= xx_array_list COMMA xx_array_item",
 /* 216 */ "xx_array_list ::= xx_array_item",
 /* 217 */ "xx_array_item ::= xx_array_key COLON xx_array_value",
 /* 218 */ "xx_array_item ::= xx_array_value",
 /* 219 */ "xx_array_key ::= IDENTIFIER",
 /* 220 */ "xx_array_key ::= STRING",
 /* 221 */ "xx_array_key ::= INTEGER",
 /* 222 */ "xx_array_value ::= xx_literal_expr",
 /* 223 */ "xx_array_value ::= IDENTIFIER",
 /* 224 */ "xx_literal_expr ::= INTEGER",
 /* 225 */ "xx_literal_expr ::= STRING",
 /* 226 */ "xx_literal_expr ::= DOUBLE",
 /* 227 */ "xx_literal_expr ::= NULL",
 /* 228 */ "xx_literal_expr ::= FALSE",
 /* 229 */ "xx_literal_expr ::= TRUE",
 /* 230 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN IDENTIFIER SBRACKET_CLOSE",
 /* 231 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN INTEGER SBRACKET_CLOSE",
 /* 232 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN STRING SBRACKET_CLOSE",
 /* 233 */ "xx_eval_expr ::= xx_common_expr",
 /* 234 */ "xx_comment ::= COMMENT",
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
// 2114 "parser.c"
      break;
    case 91:
// 830 "parser.lemon"
{ json_object_put((yypminor->yy159)); }
// 2119 "parser.c"
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
  { 133, 3 },
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
// 2571 "parser.c"
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
      case 135:
      case 163:
      case 206:
      case 207:
      case 214:
      case 222:
      case 233:
// 832 "parser.lemon"
{
	yygotominor.yy159 = yymsp[0].minor.yy159;
}
// 2604 "parser.c"
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
// 2617 "parser.c"
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
      case 138:
      case 160:
      case 213:
      case 216:
// 840 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_list(NULL, yymsp[0].minor.yy159);
}
// 2637 "parser.c"
        break;
      case 7:
// 856 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_namespace(yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(26,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 2646 "parser.c"
        break;
      case 8:
// 860 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[-1].minor.yy159, 0, NULL, status->scanner_state);
  yy_destructor(29,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 2656 "parser.c"
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
// 2667 "parser.c"
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
// 2678 "parser.c"
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
// 2690 "parser.c"
        break;
      case 12:
// 876 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_class(yymsp[-2].minor.yy0, NULL, 0, NULL, status->scanner_state);
  yy_destructor(29,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 2700 "parser.c"
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
// 2711 "parser.c"
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
// 2722 "parser.c"
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
// 2734 "parser.c"
        break;
      case 16:
// 892 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_class_definition(yymsp[0].minor.yy159, NULL, NULL, status->scanner_state);
}
// 2741 "parser.c"
        break;
      case 17:
// 896 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_class_definition(NULL, NULL, yymsp[0].minor.yy159, status->scanner_state);
}
// 2748 "parser.c"
        break;
      case 18:
// 900 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_class_definition(NULL, yymsp[0].minor.yy159, NULL, status->scanner_state);
}
// 2755 "parser.c"
        break;
      case 19:
// 904 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_class_definition(yymsp[-1].minor.yy159, yymsp[0].minor.yy159, NULL, status->scanner_state);
}
// 2762 "parser.c"
        break;
      case 20:
// 908 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_class_definition(yymsp[-1].minor.yy159, NULL, yymsp[0].minor.yy159, status->scanner_state);
}
// 2769 "parser.c"
        break;
      case 21:
// 912 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_class_definition(yymsp[0].minor.yy159, NULL, yymsp[-1].minor.yy159, status->scanner_state);
}
// 2776 "parser.c"
        break;
      case 22:
// 916 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_class_definition(NULL, yymsp[0].minor.yy159, yymsp[-1].minor.yy159, status->scanner_state);
}
// 2783 "parser.c"
        break;
      case 23:
// 920 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_class_definition(yymsp[-2].minor.yy159, yymsp[0].minor.yy159, yymsp[-1].minor.yy159, status->scanner_state);
}
// 2790 "parser.c"
        break;
      case 24:
// 924 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_class_definition(yymsp[-1].minor.yy159, yymsp[0].minor.yy159, yymsp[-2].minor.yy159, status->scanner_state);
}
// 2797 "parser.c"
        break;
      case 27:
// 936 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_class_property(yymsp[-2].minor.yy159, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(28,&yymsp[0].minor);
}
// 2805 "parser.c"
        break;
      case 28:
// 940 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_class_property(yymsp[-2].minor.yy159, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[0].minor);
}
// 2813 "parser.c"
        break;
      case 29:
// 944 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_class_property(yymsp[-4].minor.yy159, yymsp[-3].minor.yy0, yymsp[-1].minor.yy159, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(35,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 2822 "parser.c"
        break;
      case 30:
// 948 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_class_property(yymsp[-4].minor.yy159, yymsp[-3].minor.yy0, yymsp[-1].minor.yy159, NULL, status->scanner_state);
  yy_destructor(35,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 2831 "parser.c"
        break;
      case 33:
      case 53:
// 960 "parser.lemon"
{
	yygotominor.yy159 = json_object_new_string("public");
  yy_destructor(1,&yymsp[0].minor);
}
// 2840 "parser.c"
        break;
      case 34:
      case 54:
// 964 "parser.lemon"
{
	yygotominor.yy159 = json_object_new_string("protected");
  yy_destructor(2,&yymsp[0].minor);
}
// 2849 "parser.c"
        break;
      case 35:
      case 55:
// 968 "parser.lemon"
{
	yygotominor.yy159 = json_object_new_string("static");
  yy_destructor(3,&yymsp[0].minor);
}
// 2858 "parser.c"
        break;
      case 36:
      case 56:
// 972 "parser.lemon"
{
	yygotominor.yy159 = json_object_new_string("private");
  yy_destructor(4,&yymsp[0].minor);
}
// 2867 "parser.c"
        break;
      case 41:
// 992 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy159, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(35,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 2877 "parser.c"
        break;
      case 42:
// 996 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy159, NULL, status->scanner_state);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(35,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 2887 "parser.c"
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
// 2899 "parser.c"
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
// 2911 "parser.c"
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
// 2923 "parser.c"
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
// 2935 "parser.c"
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
// 2947 "parser.c"
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
// 2959 "parser.c"
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
// 2971 "parser.c"
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
// 2983 "parser.c"
        break;
      case 51:
// 1032 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_list(yymsp[-2].minor.yy159, yymsp[0].minor.yy159);
  yy_destructor(16,&yymsp[-1].minor);
}
// 2991 "parser.c"
        break;
      case 57:
// 1056 "parser.lemon"
{
	yygotominor.yy159 = json_object_new_string("private");
  yy_destructor(40,&yymsp[0].minor);
}
// 2999 "parser.c"
        break;
      case 58:
// 1060 "parser.lemon"
{
	yygotominor.yy159 = json_object_new_string("abstract");
  yy_destructor(33,&yymsp[0].minor);
}
// 3007 "parser.c"
        break;
      case 59:
// 1064 "parser.lemon"
{
	yygotominor.yy159 = json_object_new_string("final");
  yy_destructor(41,&yymsp[0].minor);
}
// 3015 "parser.c"
        break;
      case 60:
      case 116:
      case 137:
      case 159:
      case 212:
      case 215:
// 1108 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_list(yymsp[-2].minor.yy159, yymsp[0].minor.yy159);
  yy_destructor(5,&yymsp[-1].minor);
}
// 3028 "parser.c"
        break;
      case 62:
// 1116 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_parameter(NULL, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3035 "parser.c"
        break;
      case 63:
// 1120 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_parameter(yymsp[-1].minor.yy159, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3042 "parser.c"
        break;
      case 64:
// 1124 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_parameter(NULL, yymsp[-1].minor.yy159, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3049 "parser.c"
        break;
      case 65:
// 1128 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_parameter(NULL, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy159, status->scanner_state);
  yy_destructor(35,&yymsp[-1].minor);
}
// 3057 "parser.c"
        break;
      case 66:
// 1132 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_parameter(yymsp[-3].minor.yy159, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy159, status->scanner_state);
  yy_destructor(35,&yymsp[-1].minor);
}
// 3065 "parser.c"
        break;
      case 67:
// 1136 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_parameter(NULL, yymsp[-3].minor.yy159, yymsp[-2].minor.yy0, yymsp[0].minor.yy159, status->scanner_state);
  yy_destructor(35,&yymsp[-1].minor);
}
// 3073 "parser.c"
        break;
      case 68:
// 1140 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(10,&yymsp[-2].minor);
  yy_destructor(11,&yymsp[0].minor);
}
// 3082 "parser.c"
        break;
      case 69:
// 1144 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_type(XX_TYPE_INTEGER);
  yy_destructor(42,&yymsp[0].minor);
}
// 3090 "parser.c"
        break;
      case 70:
// 1148 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_type(XX_TYPE_DOUBLE);
  yy_destructor(43,&yymsp[0].minor);
}
// 3098 "parser.c"
        break;
      case 71:
// 1152 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_type(XX_TYPE_BOOL);
  yy_destructor(44,&yymsp[0].minor);
}
// 3106 "parser.c"
        break;
      case 72:
// 1156 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_type(XX_TYPE_STRING);
  yy_destructor(45,&yymsp[0].minor);
}
// 3114 "parser.c"
        break;
      case 73:
// 1160 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_type(XX_TYPE_ARRAY);
  yy_destructor(46,&yymsp[0].minor);
}
// 3122 "parser.c"
        break;
      case 74:
// 1164 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_type(XX_TYPE_VAR);
  yy_destructor(47,&yymsp[0].minor);
}
// 3130 "parser.c"
        break;
      case 93:
// 1240 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_break_statement(status->scanner_state);
  yy_destructor(48,&yymsp[-1].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3139 "parser.c"
        break;
      case 94:
// 1244 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_continue_statement(status->scanner_state);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3148 "parser.c"
        break;
      case 95:
// 1248 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_if_statement(yymsp[-2].minor.yy159, NULL, NULL, status->scanner_state);
  yy_destructor(50,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3158 "parser.c"
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
// 3171 "parser.c"
        break;
      case 97:
// 1256 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_if_statement(yymsp[-3].minor.yy159, yymsp[-1].minor.yy159, NULL, status->scanner_state);
  yy_destructor(50,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3181 "parser.c"
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
// 3194 "parser.c"
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
// 3207 "parser.c"
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
// 3220 "parser.c"
        break;
      case 101:
// 1272 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_switch_statement(yymsp[-2].minor.yy159, NULL, status->scanner_state);
  yy_destructor(52,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3230 "parser.c"
        break;
      case 102:
// 1276 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_switch_statement(yymsp[-3].minor.yy159, yymsp[-1].minor.yy159, status->scanner_state);
  yy_destructor(52,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3240 "parser.c"
        break;
      case 105:
// 1288 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_case_clause(yymsp[-2].minor.yy159, yymsp[0].minor.yy159, status->scanner_state);
  yy_destructor(53,&yymsp[-3].minor);
  yy_destructor(54,&yymsp[-1].minor);
}
// 3249 "parser.c"
        break;
      case 106:
// 1292 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_case_clause(NULL, yymsp[0].minor.yy159, status->scanner_state);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(54,&yymsp[-1].minor);
}
// 3258 "parser.c"
        break;
      case 107:
// 1296 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_loop_statement(NULL, status->scanner_state);
  yy_destructor(56,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3268 "parser.c"
        break;
      case 108:
// 1300 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_loop_statement(yymsp[-1].minor.yy159, status->scanner_state);
  yy_destructor(56,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3278 "parser.c"
        break;
      case 109:
// 1304 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_while_statement(yymsp[-2].minor.yy159, NULL, status->scanner_state);
  yy_destructor(57,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3288 "parser.c"
        break;
      case 110:
// 1308 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_while_statement(yymsp[-3].minor.yy159, yymsp[-1].minor.yy159, status->scanner_state);
  yy_destructor(57,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3298 "parser.c"
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
// 3309 "parser.c"
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
// 3321 "parser.c"
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
// 3333 "parser.c"
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
// 3346 "parser.c"
        break;
      case 115:
// 1328 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_let_statement(yymsp[-1].minor.yy159, status->scanner_state);
  yy_destructor(61,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3355 "parser.c"
        break;
      case 118:
// 1341 "parser.lemon"
{
	yygotominor.yy159 = json_object_new_string("assign");
  yy_destructor(35,&yymsp[0].minor);
}
// 3363 "parser.c"
        break;
      case 119:
// 1346 "parser.lemon"
{
	yygotominor.yy159 = json_object_new_string("add-assign");
  yy_destructor(62,&yymsp[0].minor);
}
// 3371 "parser.c"
        break;
      case 120:
// 1351 "parser.lemon"
{
	yygotominor.yy159 = json_object_new_string("sub-assign");
  yy_destructor(63,&yymsp[0].minor);
}
// 3379 "parser.c"
        break;
      case 121:
// 1355 "parser.lemon"
{
	yygotominor.yy159 = json_object_new_string("mult-assign");
  yy_destructor(64,&yymsp[0].minor);
}
// 3387 "parser.c"
        break;
      case 122:
// 1359 "parser.lemon"
{
	yygotominor.yy159 = json_object_new_string("div-assign");
  yy_destructor(65,&yymsp[0].minor);
}
// 3395 "parser.c"
        break;
      case 123:
// 1363 "parser.lemon"
{
	yygotominor.yy159 = json_object_new_string("concat-assign");
  yy_destructor(66,&yymsp[0].minor);
}
// 3403 "parser.c"
        break;
      case 124:
// 1368 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_let_assignment("variable", yymsp[-1].minor.yy159, yymsp[-2].minor.yy0, NULL, NULL, yymsp[0].minor.yy159, status->scanner_state);
}
// 3410 "parser.c"
        break;
      case 125:
// 1373 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_let_assignment("object-property", yymsp[-1].minor.yy159, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy159, status->scanner_state);
  yy_destructor(67,&yymsp[-3].minor);
}
// 3418 "parser.c"
        break;
      case 126:
// 1378 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_let_assignment("object-property-append", yymsp[-1].minor.yy159, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy159, status->scanner_state);
  yy_destructor(67,&yymsp[-5].minor);
  yy_destructor(68,&yymsp[-3].minor);
  yy_destructor(69,&yymsp[-2].minor);
}
// 3428 "parser.c"
        break;
      case 127:
// 1383 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_let_assignment("object-property-array-index", yymsp[-1].minor.yy159, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy159, yymsp[0].minor.yy159, status->scanner_state);
  yy_destructor(67,&yymsp[-6].minor);
  yy_destructor(68,&yymsp[-4].minor);
  yy_destructor(69,&yymsp[-2].minor);
}
// 3438 "parser.c"
        break;
      case 128:
// 1388 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_let_assignment("static-property", yymsp[-1].minor.yy159, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy159, status->scanner_state);
  yy_destructor(70,&yymsp[-3].minor);
}
// 3446 "parser.c"
        break;
      case 129:
// 1393 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_let_assignment("static-property-append", yymsp[-1].minor.yy159, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy159, status->scanner_state);
  yy_destructor(70,&yymsp[-5].minor);
  yy_destructor(68,&yymsp[-3].minor);
  yy_destructor(69,&yymsp[-2].minor);
}
// 3456 "parser.c"
        break;
      case 130:
// 1398 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_let_assignment("static-property-array-index", yymsp[-1].minor.yy159, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy159, yymsp[0].minor.yy159, status->scanner_state);
  yy_destructor(70,&yymsp[-6].minor);
  yy_destructor(68,&yymsp[-4].minor);
  yy_destructor(69,&yymsp[-2].minor);
}
// 3466 "parser.c"
        break;
      case 131:
// 1403 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_let_assignment("variable-append", yymsp[-1].minor.yy159, yymsp[-4].minor.yy0, NULL, NULL, yymsp[0].minor.yy159, status->scanner_state);
  yy_destructor(68,&yymsp[-3].minor);
  yy_destructor(69,&yymsp[-2].minor);
}
// 3475 "parser.c"
        break;
      case 132:
// 1408 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_let_assignment("array-index", yymsp[-1].minor.yy159, yymsp[-5].minor.yy0, NULL, yymsp[-3].minor.yy159, yymsp[0].minor.yy159, status->scanner_state);
  yy_destructor(68,&yymsp[-4].minor);
  yy_destructor(69,&yymsp[-2].minor);
}
// 3484 "parser.c"
        break;
      case 133:
// 1413 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_let_assignment("incr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(71,&yymsp[0].minor);
}
// 3492 "parser.c"
        break;
      case 134:
// 1418 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_let_assignment("decr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(72,&yymsp[0].minor);
}
// 3500 "parser.c"
        break;
      case 136:
// 1426 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_echo_statement(yymsp[-1].minor.yy159, status->scanner_state);
  yy_destructor(73,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3509 "parser.c"
        break;
      case 139:
// 1438 "parser.lemon"
{
	yygotominor.yy159 = yymsp[0].minor.yy159;;
}
// 3516 "parser.c"
        break;
      case 140:
// 1443 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_mcall_statement(yymsp[-1].minor.yy159, status->scanner_state);
  yy_destructor(28,&yymsp[0].minor);
}
// 3524 "parser.c"
        break;
      case 141:
// 1448 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_scall_statement(yymsp[-1].minor.yy159, status->scanner_state);
  yy_destructor(28,&yymsp[0].minor);
}
// 3532 "parser.c"
        break;
      case 142:
// 1453 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_return_statement(yymsp[-1].minor.yy159, status->scanner_state);
  yy_destructor(74,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3541 "parser.c"
        break;
      case 143:
// 1458 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(75,&yymsp[-4].minor);
  yy_destructor(67,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3555 "parser.c"
        break;
      case 144:
// 1467 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(75,&yymsp[-5].minor);
  yy_destructor(68,&yymsp[-3].minor);
  yy_destructor(69,&yymsp[-1].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3570 "parser.c"
        break;
      case 145:
// 1476 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_INTEGER, yymsp[-2].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(75,&yymsp[-5].minor);
  yy_destructor(68,&yymsp[-3].minor);
  yy_destructor(69,&yymsp[-1].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3585 "parser.c"
        break;
      case 146:
// 1485 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_STRING, yymsp[-2].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(75,&yymsp[-5].minor);
  yy_destructor(68,&yymsp[-3].minor);
  yy_destructor(69,&yymsp[-1].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3600 "parser.c"
        break;
      case 147:
// 1494 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_throw_exception(yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(78,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3609 "parser.c"
        break;
      case 148:
// 1499 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_throw_exception(yymsp[-4].minor.yy0, yymsp[-2].minor.yy159, status->scanner_state);
  yy_destructor(78,&yymsp[-6].minor);
  yy_destructor(23,&yymsp[-5].minor);
  yy_destructor(39,&yymsp[-3].minor);
  yy_destructor(25,&yymsp[-1].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3621 "parser.c"
        break;
      case 149:
// 1503 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_declare_statement(XX_T_TYPE_INTEGER, yymsp[-1].minor.yy159, status->scanner_state);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3630 "parser.c"
        break;
      case 150:
// 1507 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_declare_statement(XX_T_TYPE_UINTEGER, yymsp[-1].minor.yy159, status->scanner_state);
  yy_destructor(79,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3639 "parser.c"
        break;
      case 151:
// 1511 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_declare_statement(XX_T_TYPE_CHAR, yymsp[-1].minor.yy159, status->scanner_state);
  yy_destructor(80,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3648 "parser.c"
        break;
      case 152:
// 1515 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_declare_statement(XX_T_TYPE_UCHAR, yymsp[-1].minor.yy159, status->scanner_state);
  yy_destructor(81,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3657 "parser.c"
        break;
      case 153:
// 1519 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_declare_statement(XX_T_TYPE_LONG, yymsp[-1].minor.yy159, status->scanner_state);
  yy_destructor(82,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3666 "parser.c"
        break;
      case 154:
// 1523 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_declare_statement(XX_T_TYPE_ULONG, yymsp[-1].minor.yy159, status->scanner_state);
  yy_destructor(83,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3675 "parser.c"
        break;
      case 155:
// 1527 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_declare_statement(XX_T_TYPE_DOUBLE, yymsp[-1].minor.yy159, status->scanner_state);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3684 "parser.c"
        break;
      case 156:
// 1531 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_declare_statement(XX_T_TYPE_STRING, yymsp[-1].minor.yy159, status->scanner_state);
  yy_destructor(45,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3693 "parser.c"
        break;
      case 157:
// 1535 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_declare_statement(XX_T_TYPE_BOOL, yymsp[-1].minor.yy159, status->scanner_state);
  yy_destructor(44,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3702 "parser.c"
        break;
      case 158:
// 1539 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_declare_statement(XX_T_TYPE_VAR, yymsp[-1].minor.yy159, status->scanner_state);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3711 "parser.c"
        break;
      case 161:
// 1551 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_declare_variable(yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3718 "parser.c"
        break;
      case 162:
// 1555 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_declare_variable(yymsp[-2].minor.yy0, yymsp[0].minor.yy159, status->scanner_state);
  yy_destructor(35,&yymsp[-1].minor);
}
// 3726 "parser.c"
        break;
      case 164:
// 1563 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("not", yymsp[0].minor.yy159, NULL, NULL, status->scanner_state);
  yy_destructor(24,&yymsp[-1].minor);
}
// 3734 "parser.c"
        break;
      case 165:
// 1567 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("isset", yymsp[0].minor.yy159, NULL, NULL, status->scanner_state);
  yy_destructor(19,&yymsp[-1].minor);
}
// 3742 "parser.c"
        break;
      case 166:
// 1571 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("equals", yymsp[-2].minor.yy159, yymsp[0].minor.yy159, NULL, status->scanner_state);
  yy_destructor(8,&yymsp[-1].minor);
}
// 3750 "parser.c"
        break;
      case 167:
// 1575 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("not-equals", yymsp[-2].minor.yy159, yymsp[0].minor.yy159, NULL, status->scanner_state);
  yy_destructor(15,&yymsp[-1].minor);
}
// 3758 "parser.c"
        break;
      case 168:
// 1579 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("identical", yymsp[-2].minor.yy159, yymsp[0].minor.yy159, NULL, status->scanner_state);
  yy_destructor(9,&yymsp[-1].minor);
}
// 3766 "parser.c"
        break;
      case 169:
// 1583 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("not-identical", yymsp[-2].minor.yy159, yymsp[0].minor.yy159, NULL, status->scanner_state);
  yy_destructor(14,&yymsp[-1].minor);
}
// 3774 "parser.c"
        break;
      case 170:
// 1587 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("less", yymsp[-2].minor.yy159, yymsp[0].minor.yy159, NULL, status->scanner_state);
  yy_destructor(10,&yymsp[-1].minor);
}
// 3782 "parser.c"
        break;
      case 171:
// 1591 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("greater", yymsp[-2].minor.yy159, yymsp[0].minor.yy159, NULL, status->scanner_state);
  yy_destructor(11,&yymsp[-1].minor);
}
// 3790 "parser.c"
        break;
      case 172:
// 1595 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("less-equal", yymsp[-2].minor.yy159, yymsp[0].minor.yy159, NULL, status->scanner_state);
  yy_destructor(12,&yymsp[-1].minor);
}
// 3798 "parser.c"
        break;
      case 173:
// 1599 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("greater-equal", yymsp[-2].minor.yy159, yymsp[0].minor.yy159, NULL, status->scanner_state);
  yy_destructor(13,&yymsp[-1].minor);
}
// 3806 "parser.c"
        break;
      case 174:
// 1603 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("list", yymsp[-1].minor.yy159, NULL, NULL, status->scanner_state);
  yy_destructor(39,&yymsp[-2].minor);
  yy_destructor(25,&yymsp[0].minor);
}
// 3815 "parser.c"
        break;
      case 175:
// 1607 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("cast", yymsp[-2].minor.yy159, yymsp[0].minor.yy159, NULL, status->scanner_state);
  yy_destructor(39,&yymsp[-3].minor);
  yy_destructor(25,&yymsp[-1].minor);
}
// 3824 "parser.c"
        break;
      case 176:
// 1611 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(67,&yymsp[-1].minor);
}
// 3832 "parser.c"
        break;
      case 177:
// 1615 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("static-property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(70,&yymsp[-1].minor);
}
// 3840 "parser.c"
        break;
      case 178:
// 1619 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("static-constant-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(70,&yymsp[-1].minor);
}
// 3848 "parser.c"
        break;
      case 179:
      case 230:
// 1623 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(68,&yymsp[-2].minor);
  yy_destructor(69,&yymsp[0].minor);
}
// 3858 "parser.c"
        break;
      case 180:
      case 231:
// 1628 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_INTEGER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(68,&yymsp[-2].minor);
  yy_destructor(69,&yymsp[0].minor);
}
// 3868 "parser.c"
        break;
      case 181:
      case 232:
// 1633 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_STRING, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(68,&yymsp[-2].minor);
  yy_destructor(69,&yymsp[0].minor);
}
// 3878 "parser.c"
        break;
      case 182:
// 1638 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("add", yymsp[-2].minor.yy159, yymsp[0].minor.yy159, NULL, status->scanner_state);
  yy_destructor(16,&yymsp[-1].minor);
}
// 3886 "parser.c"
        break;
      case 183:
// 1643 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("sub", yymsp[-2].minor.yy159, yymsp[0].minor.yy159, NULL, status->scanner_state);
  yy_destructor(17,&yymsp[-1].minor);
}
// 3894 "parser.c"
        break;
      case 184:
// 1648 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("concat", yymsp[-2].minor.yy159, yymsp[0].minor.yy159, NULL, status->scanner_state);
  yy_destructor(18,&yymsp[-1].minor);
}
// 3902 "parser.c"
        break;
      case 185:
// 1653 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("and", yymsp[-2].minor.yy159, yymsp[0].minor.yy159, NULL, status->scanner_state);
  yy_destructor(6,&yymsp[-1].minor);
}
// 3910 "parser.c"
        break;
      case 186:
// 1658 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("or", yymsp[-2].minor.yy159, yymsp[0].minor.yy159, NULL, status->scanner_state);
  yy_destructor(7,&yymsp[-1].minor);
}
// 3918 "parser.c"
        break;
      case 187:
// 1663 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("instanceof", yymsp[0].minor.yy159, NULL, NULL, status->scanner_state);
  yy_destructor(22,&yymsp[-1].minor);
}
// 3926 "parser.c"
        break;
      case 188:
// 1668 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("fetch", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy159, NULL, status->scanner_state);
  yy_destructor(20,&yymsp[-3].minor);
  yy_destructor(5,&yymsp[-1].minor);
}
// 3935 "parser.c"
        break;
      case 189:
// 1673 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("typeof", yymsp[0].minor.yy159, NULL, NULL, status->scanner_state);
  yy_destructor(21,&yymsp[-1].minor);
}
// 3943 "parser.c"
        break;
      case 190:
// 1678 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_literal(XX_T_CONSTANT, yymsp[0].minor.yy0, status->scanner_state);
}
// 3950 "parser.c"
        break;
      case 191:
      case 219:
      case 223:
// 1683 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state);
}
// 3959 "parser.c"
        break;
      case 192:
      case 221:
      case 224:
// 1688 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_literal(XX_T_INTEGER, yymsp[0].minor.yy0, status->scanner_state);
}
// 3968 "parser.c"
        break;
      case 193:
      case 220:
      case 225:
// 1693 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_literal(XX_T_STRING, yymsp[0].minor.yy0, status->scanner_state);
}
// 3977 "parser.c"
        break;
      case 194:
// 1698 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_literal(XX_T_SCHAR, yymsp[0].minor.yy0, status->scanner_state);
}
// 3984 "parser.c"
        break;
      case 195:
      case 226:
// 1703 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_literal(XX_T_DOUBLE, yymsp[0].minor.yy0, status->scanner_state);
}
// 3992 "parser.c"
        break;
      case 196:
      case 227:
// 1708 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_literal(XX_T_NULL, NULL, status->scanner_state);
  yy_destructor(86,&yymsp[0].minor);
}
// 4001 "parser.c"
        break;
      case 197:
      case 229:
// 1713 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_literal(XX_T_TRUE, NULL, status->scanner_state);
  yy_destructor(87,&yymsp[0].minor);
}
// 4010 "parser.c"
        break;
      case 198:
      case 228:
// 1718 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_literal(XX_T_FALSE, NULL, status->scanner_state);
  yy_destructor(88,&yymsp[0].minor);
}
// 4019 "parser.c"
        break;
      case 199:
// 1723 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("empty-array", NULL, NULL, NULL, status->scanner_state);
  yy_destructor(68,&yymsp[-1].minor);
  yy_destructor(69,&yymsp[0].minor);
}
// 4028 "parser.c"
        break;
      case 200:
// 1728 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("array", yymsp[-1].minor.yy159, NULL, NULL, status->scanner_state);
  yy_destructor(68,&yymsp[-2].minor);
  yy_destructor(69,&yymsp[0].minor);
}
// 4037 "parser.c"
        break;
      case 201:
// 1733 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_new_instance(yymsp[0].minor.yy0, NULL, status->scanner_state);
  yy_destructor(23,&yymsp[-1].minor);
}
// 4045 "parser.c"
        break;
      case 202:
// 1738 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_new_instance(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(23,&yymsp[-3].minor);
  yy_destructor(39,&yymsp[-1].minor);
  yy_destructor(25,&yymsp[0].minor);
}
// 4055 "parser.c"
        break;
      case 203:
// 1743 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_new_instance(yymsp[-3].minor.yy0, yymsp[-1].minor.yy159, status->scanner_state);
  yy_destructor(23,&yymsp[-4].minor);
  yy_destructor(39,&yymsp[-2].minor);
  yy_destructor(25,&yymsp[0].minor);
}
// 4065 "parser.c"
        break;
      case 204:
// 1748 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_fcall(yymsp[-3].minor.yy0, yymsp[-1].minor.yy159, status->scanner_state);
  yy_destructor(39,&yymsp[-2].minor);
  yy_destructor(25,&yymsp[0].minor);
}
// 4074 "parser.c"
        break;
      case 205:
// 1753 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_fcall(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(39,&yymsp[-1].minor);
  yy_destructor(25,&yymsp[0].minor);
}
// 4083 "parser.c"
        break;
      case 208:
// 1768 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_scall(yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy159, status->scanner_state);
  yy_destructor(70,&yymsp[-4].minor);
  yy_destructor(39,&yymsp[-2].minor);
  yy_destructor(25,&yymsp[0].minor);
}
// 4093 "parser.c"
        break;
      case 209:
// 1773 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_scall(yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(70,&yymsp[-3].minor);
  yy_destructor(39,&yymsp[-1].minor);
  yy_destructor(25,&yymsp[0].minor);
}
// 4103 "parser.c"
        break;
      case 210:
// 1778 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_mcall(yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy159, status->scanner_state);
  yy_destructor(67,&yymsp[-4].minor);
  yy_destructor(39,&yymsp[-2].minor);
  yy_destructor(25,&yymsp[0].minor);
}
// 4113 "parser.c"
        break;
      case 211:
// 1783 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_mcall(yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(67,&yymsp[-3].minor);
  yy_destructor(39,&yymsp[-1].minor);
  yy_destructor(25,&yymsp[0].minor);
}
// 4123 "parser.c"
        break;
      case 217:
// 1807 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_array_item(yymsp[-2].minor.yy159, yymsp[0].minor.yy159, status->scanner_state);
  yy_destructor(54,&yymsp[-1].minor);
}
// 4131 "parser.c"
        break;
      case 218:
// 1811 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_array_item(NULL, yymsp[0].minor.yy159, status->scanner_state);
}
// 4138 "parser.c"
        break;
      case 234:
// 1878 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_comment(yymsp[0].minor.yy0, status->scanner_state);
}
// 4145 "parser.c"
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

// 4221 "parser.c"
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
