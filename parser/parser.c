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
 /*     0 */   335,  181,  182,  306,  376,   62,  365,  340,  347,  180,
 /*    10 */   183,  184,  186,  185,  489,  299,  323,  329,  326,  266,
 /*    20 */   332,   45,   47,   49,  379,   58,  265,  199,  200,   69,
 /*    30 */    73,   78,    9,  463,   85,  466,  467,  455,  545,  450,
 /*    40 */   424,  425,   19,  396,  374,  421,  261,  272,  275,   86,
 /*    50 */   260,  290,  308,  311,  314,  317,  320,  335,  181,  182,
 /*    60 */   545,   72,   80,    8,  340,  407,   91,  183,  184,  186,
 /*    70 */   185,   11,  299,  323,  329,  326,  237,  332,   45,   47,
 /*    80 */    49,  233,   58,   88,  199,  200,   69,   73,   78,   14,
 /*    90 */   392,   85,  188,   92,   93,   94,   95,   96,  542,  229,
 /*   100 */   380,  384,  388,  261,  272,  275,   87,  381,  290,  308,
 /*   110 */   311,  314,  317,  320,  335,   22,  338,  146,   77,  336,
 /*   120 */   542,  340,  138,  139,  140,  141,  142,  143,   91,  299,
 /*   130 */   323,  329,  326,   24,  332,   45,   47,   49,  144,   58,
 /*   140 */   302,  190,  189,   69,   73,   78,  199,  200,   85,   21,
 /*   150 */   183,  184,  186,  185,  207,   92,   93,   94,   95,   96,
 /*   160 */   261,  272,  275,  334,  212,  290,  308,  311,  314,  317,
 /*   170 */   320,  335,  127,  129,  131,  361,  333,  307,  340,  725,
 /*   180 */     1,    2,  488,    4,    5,    6,  299,  323,  329,  326,
 /*   190 */    46,  332,   45,   47,   49,   99,   58,  302,   48,  245,
 /*   200 */    69,   73,   78,  199,  200,   85,  464,  437,  455,  478,
 /*   210 */   450,  482,  425,   19,  330,  307,  421,  261,  272,  275,
 /*   220 */   331,  153,  290,  308,  311,  314,  317,  320,  335,  199,
 /*   230 */   200,  196,  359,  266,  293,  340,  202,   91,  291,  262,
 /*   240 */   267,  199,  200,  299,  323,  329,  326,   51,  332,   45,
 /*   250 */    47,   49,   99,   58,  302,   54,  253,   69,   73,   78,
 /*   260 */   199,  200,   85,  153,   92,   93,   94,   95,   96,   61,
 /*   270 */   242,  199,  200,  209,  261,  272,  275,  328,  202,  290,
 /*   280 */   308,  311,  314,  317,  320,  335,   55,   50,   66,  346,
 /*   290 */   360,   65,  340,  362,  446,  447,  448,  449,  199,  200,
 /*   300 */   299,  323,  329,  326,   64,  332,   45,   47,   49,  153,
 /*   310 */    58,  327,  307,  153,   69,   73,   78,  199,  200,   85,
 /*   320 */   456,  199,  200,  221,  201,  220,   99,   12,  202,  469,
 /*   330 */   232,  261,  272,  275,  199,  200,  290,  308,  311,  314,
 /*   340 */   317,  320,  335,   18,   19,  423,  353,  421,   60,  340,
 /*   350 */   424,  425,   19,  150,  213,  421,  206,  299,  323,  329,
 /*   360 */   326,  379,  332,   45,   47,   49,   99,   58,  302,  153,
 /*   370 */   257,   69,   73,   78,  199,  200,   85,  199,  200,  203,
 /*   380 */   406,  416,  373,  421,  202,  395,  384,  388,  261,  272,
 /*   390 */   275,  325,  153,  290,  308,  311,  314,  317,  320,  335,
 /*   400 */   199,  200,  296,  358,  324,  307,  340,  202,  490,   67,
 /*   410 */   446,  447,  448,  449,  299,  323,  329,  326,  237,  332,
 /*   420 */    45,   47,   49,  254,   58,  302,  199,  200,   69,   73,
 /*   430 */    78,  273,   59,   85,    7,  360,  440,   10,   79,  199,
 /*   440 */   200,  475,  340,  199,  200,  261,  272,  275,  322,  179,
 /*   450 */   290,  308,  311,  314,  317,  320,  335,   99,  321,  307,
 /*   460 */    57,  241,  439,  340,  450,  199,  200,  406,  179,   89,
 /*   470 */   421,  299,  323,  329,  326,   70,  332,   45,   47,   49,
 /*   480 */    99,   58,  302,   75,  228,   69,   73,   78,  199,  200,
 /*   490 */    85,    7,  116,  172,   10,  191,  176,  187,  475,  340,
 /*   500 */   199,  200,  261,  272,  275,  319,   90,  290,  308,  311,
 /*   510 */   314,  317,  320,  335,  175,  176,  187,  366,  318,  307,
 /*   520 */   340,  315,  307,    3,    4,    5,    6,   63,  299,  323,
 /*   530 */   329,  326,  302,  332,   45,   47,   49,   99,   58,  312,
 /*   540 */   307,   98,   69,   73,   78,  199,  200,   85,   99,   65,
 /*   550 */    97,  362,  249,  106,   99,  316,  199,  200,  236,  261,
 /*   560 */   272,  275,  199,  200,  290,  308,  311,  314,  317,  320,
 /*   570 */   335,  237,   74,  118,   53,  360,  246,  340,  105,  199,
 /*   580 */   200,  199,  200,  199,  200,  299,  323,  329,  326,  132,
 /*   590 */   332,   45,   47,   49,  109,   58,  441,  199,  200,   69,
 /*   600 */    73,   78,  415,  442,   85,  395,  384,  388,  309,  307,
 /*   610 */   214,  161,  302,  198,  108,  107,  261,  272,  275,  199,
 /*   620 */   200,  290,  308,  311,  314,  317,  320,  335,  134,  543,
 /*   630 */   355,  367,  136,  297,  340,  313,  199,  200,  199,  200,
 /*   640 */   199,  200,  299,  323,  329,  326,  224,  332,   45,   47,
 /*   650 */    49,  543,   58,  111,  199,  200,   69,   73,   78,  216,
 /*   660 */   218,   85,  155,  110,  112,  174,  302,  302,  120,  147,
 /*   670 */   199,  200,  281,  261,  272,  275,  199,  200,  290,  308,
 /*   680 */   311,  314,  317,  320,  335,  113,  130,  264,   84,  310,
 /*   690 */   301,  340,  300,  307,  199,  200,  277,  280,  151,  299,
 /*   700 */   323,  329,  326,  102,  332,   45,   47,   49,  124,   58,
 /*   710 */   263,  199,  200,   69,   73,   78,  199,  200,   85,  128,
 /*   720 */   544,  284,  287,  198,  396,  350,  152,  199,  200,  173,
 /*   730 */   261,  272,  275,  199,  200,  290,  308,  311,  314,  317,
 /*   740 */   320,  335,  544,  197,  343,  372,   20,  198,  340,  157,
 /*   750 */   158,  179,  199,  200,  177,  159,  299,  323,  329,  326,
 /*   760 */   114,  332,   45,   47,   49,  148,   58,  204,  199,  200,
 /*   770 */    69,   73,   78,  199,  200,   85,   81,  708,  709,  710,
 /*   780 */   193,  198,  122,  194,  199,  200,  208,  261,  272,  275,
 /*   790 */   199,  200,  290,  308,  311,  314,  317,  320,  335,  178,
 /*   800 */   457,  210,  371,  215,  198,  340,  217,  458,  219,  226,
 /*   810 */   227,  231,  234,  299,  323,  329,  326,  126,  332,   45,
 /*   820 */    47,   49,  235,   58,  222,  199,  200,   69,   73,   78,
 /*   830 */   239,  240,   85,  244,  247,  248,  255,  252,  256,  269,
 /*   840 */   271,  276,  278,  282,  261,  272,  275,  279,  283,  290,
 /*   850 */   308,  311,  314,  317,  320,  335,  285,  286,  288,   25,
 /*   860 */   289,  292,  340,  294,  295,  298,  304,  337,  339,  305,
 /*   870 */   299,  323,  329,  326,  303,  332,   45,   47,   49,  348,
 /*   880 */    58,  349,  368,  369,   69,   73,   78,  375,  383,   85,
 /*   890 */   382,  385,  363,  386,  387,  389,  390,  391,  393,  394,
 /*   900 */   557,  261,  272,  275,  408,  397,  290,  308,  311,  314,
 /*   910 */   317,  320,  335,  409,  411,  428,  378,  417,  429,  340,
 /*   920 */   430,  431,  436,  433,  434,  445,  435,  299,  323,  329,
 /*   930 */   326,  444,  332,   45,   47,   49,  460,   58,  443,  470,
 /*   940 */   471,   69,   73,   78,  473,  459,   85,  476,  477,  483,
 /*   950 */   480,  486,  389,  484,  389,  389,  389,  389,  261,  272,
 /*   960 */   275,  389,  389,  290,  308,  311,  314,  317,  320,  335,
 /*   970 */   389,  389,  389,  412,  389,  389,  340,  389,  389,  389,
 /*   980 */   389,  389,  389,  389,  299,  323,  329,  326,  389,  332,
 /*   990 */    45,   47,   49,  389,   58,  389,  389,  389,   69,   73,
 /*  1000 */    78,  389,  389,   85,  389,  389,  389,  389,  389,  389,
 /*  1010 */   389,  389,  389,  389,  389,  261,  272,  275,  389,  389,
 /*  1020 */   290,  308,  311,  314,  317,  320,  335,  389,  389,  389,
 /*  1030 */   414,  389,  389,  340,  389,  389,  389,  389,  389,  389,
 /*  1040 */   389,  299,  323,  329,  326,  389,  332,   45,   47,   49,
 /*  1050 */   389,   58,  389,  389,  389,   69,   73,   78,  389,  389,
 /*  1060 */    85,  389,  389,  389,  389,  389,  389,  389,  389,  389,
 /*  1070 */   389,  389,  261,  272,  275,  389,  389,  290,  308,  311,
 /*  1080 */   314,  317,  320,  335,  389,  389,  389,  418,  389,  389,
 /*  1090 */   340,  389,  389,  389,  389,  389,  389,  389,  299,  323,
 /*  1100 */   329,  326,  389,  332,   45,   47,   49,  389,   58,  389,
 /*  1110 */   389,  389,   69,   73,   78,  389,  389,   85,  389,  389,
 /*  1120 */   389,  389,  389,  389,  389,  389,  389,  389,  389,  261,
 /*  1130 */   272,  275,  389,  389,  290,  308,  311,  314,  317,  320,
 /*  1140 */   335,  389,  389,  389,  420,  389,  389,  340,  389,  389,
 /*  1150 */   389,  389,  389,  389,  389,  299,  323,  329,  326,  389,
 /*  1160 */   332,   45,   47,   49,  389,   58,  389,  389,  389,   69,
 /*  1170 */    73,   78,  389,  389,   85,  389,  389,  389,  389,  389,
 /*  1180 */   389,  389,  389,  389,  389,  389,  261,  272,  275,  389,
 /*  1190 */   389,  290,  308,  311,  314,  317,  320,  335,  389,  389,
 /*  1200 */   389,   27,  389,  389,  340,  389,  389,  389,  389,  389,
 /*  1210 */   389,  389,  299,  323,  329,  326,  389,  332,   45,   47,
 /*  1220 */    49,  389,   58,  389,  389,  389,   69,   73,   78,  389,
 /*  1230 */   389,   85,  389,  389,  389,  389,  389,  389,  389,  389,
 /*  1240 */   335,  389,  389,  261,  272,  275,  389,  340,  290,  308,
 /*  1250 */   311,  314,  317,  320,  389,  299,  323,  329,  326,  389,
 /*  1260 */   332,   45,   47,   49,  389,   58,  392,  389,  389,   69,
 /*  1270 */    73,   78,  389,  389,   85,  389,  389,  389,  389,  389,
 /*  1280 */   389,  410,  389,  381,  389,  389,  261,  272,  275,  389,
 /*  1290 */   389,  290,  308,  311,  314,  317,  320,  389,  138,  139,
 /*  1300 */   140,  141,  142,  143,  389,  389,  104,  156,  160,  154,
 /*  1310 */   192,  101,  389,  389,  149,  389,  389,  389,  389,  389,
 /*  1320 */   389,  389,  389,  389,  162,  389,  137,  389,  389,  138,
 /*  1330 */   139,  140,  141,  142,  143,  389,  133,  135,  100,  115,
 /*  1340 */   119,  121,  123,  125,  117,  103,  127,  129,  131,  389,
 /*  1350 */   389,  389,  389,  389,  389,  170,  389,  389,  274,  188,
 /*  1360 */   389,  389,  389,  163,  164,  389,   44,  389,  389,  389,
 /*  1370 */   389,  165,  166,  167,  168,  169,  389,  389,  389,  352,
 /*  1380 */   389,  389,  389,  389,  341,   29,   30,   31,   32,   33,
 /*  1390 */    34,   35,   36,   37,   38,   39,   40,   41,   42,   43,
 /*  1400 */   389,  171,  389,   44,  389,  389,  389,  389,  190,  189,
 /*  1410 */   389,  268,  270,  389,  389,  389,  364,  183,  184,  186,
 /*  1420 */   185,  341,   29,   30,   31,   32,   33,   34,   35,   36,
 /*  1430 */    37,   38,   39,   40,   41,   42,   43,  479,   16,  462,
 /*  1440 */   468,  467,  455,  389,  450,  461,  425,   19,  268,  270,
 /*  1450 */   421,  389,   44,  100,  115,  119,  121,  123,  125,  117,
 /*  1460 */   103,  127,  129,  131,  389,  377,  389,  389,  389,  389,
 /*  1470 */   341,   29,   30,   31,   32,   33,   34,   35,   36,   37,
 /*  1480 */    38,   39,   40,   41,   42,   43,   44,  389,  389,  389,
 /*  1490 */   389,  389,  389,  389,  389,  389,  389,  268,  270,  345,
 /*  1500 */   389,  389,  389,  389,  341,   29,   30,   31,   32,   33,
 /*  1510 */    34,   35,   36,   37,   38,   39,   40,   41,   42,   43,
 /*  1520 */    44,  389,  389,  389,  389,  389,  389,  389,  389,  389,
 /*  1530 */   389,  268,  270,  357,  389,  389,  389,  389,  341,   29,
 /*  1540 */    30,   31,   32,   33,   34,   35,   36,   37,   38,   39,
 /*  1550 */    40,   41,   42,   43,  389,  389,  389,  389,  389,  389,
 /*  1560 */   389,  389,   44,  389,  389,  268,  270,  389,  389,  389,
 /*  1570 */   389,  389,  389,  389,  389,   56,  389,  389,  389,  389,
 /*  1580 */   341,   29,   30,   31,   32,   33,   34,   35,   36,   37,
 /*  1590 */    38,   39,   40,   41,   42,   43,   44,  389,  389,  389,
 /*  1600 */   389,  389,  389,  389,  389,  389,  389,  268,  270,  419,
 /*  1610 */   389,  389,  389,  389,  341,   29,   30,   31,   32,   33,
 /*  1620 */    34,   35,   36,   37,   38,   39,   40,   41,   42,   43,
 /*  1630 */    44,  389,  389,  389,  389,  389,  389,  389,  389,  389,
 /*  1640 */   389,  268,  270,  370,  389,  389,  389,  389,  341,   29,
 /*  1650 */    30,   31,   32,   33,   34,   35,   36,   37,   38,   39,
 /*  1660 */    40,   41,   42,   43,  389,  389,  389,  389,  389,  389,
 /*  1670 */   389,  389,   44,  389,  389,  268,  270,  389,  389,  389,
 /*  1680 */   389,  389,  389,  389,  389,   83,  389,  389,  389,  389,
 /*  1690 */   341,   29,   30,   31,   32,   33,   34,   35,   36,   37,
 /*  1700 */    38,   39,   40,   41,   42,   43,   44,  389,  389,  389,
 /*  1710 */   389,  389,  389,  389,  389,  389,  389,  268,  270,   26,
 /*  1720 */   389,  389,  389,  389,  341,   29,   30,   31,   32,   33,
 /*  1730 */    34,   35,   36,   37,   38,   39,   40,   41,   42,   43,
 /*  1740 */    44,  389,  389,  389,  389,  389,  389,  389,  389,  389,
 /*  1750 */   389,  268,  270,   71,  389,  389,  389,  389,  341,   29,
 /*  1760 */    30,   31,   32,   33,   34,   35,   36,   37,   38,   39,
 /*  1770 */    40,   41,   42,   43,  389,  389,  389,  389,  389,  389,
 /*  1780 */   389,  389,   44,  389,  389,  268,  270,  389,  389,  389,
 /*  1790 */   389,  389,  389,  389,  389,   52,  389,  389,  389,  389,
 /*  1800 */   341,   29,   30,   31,   32,   33,   34,   35,   36,   37,
 /*  1810 */    38,   39,   40,   41,   42,   43,   44,  389,  389,  389,
 /*  1820 */   389,  389,  389,  389,  389,  389,  389,  268,  270,  413,
 /*  1830 */   389,  389,  389,  389,  341,   29,   30,   31,   32,   33,
 /*  1840 */    34,   35,   36,   37,   38,   39,   40,   41,   42,   43,
 /*  1850 */    44,  389,  389,  389,  389,  389,  389,  389,  389,  389,
 /*  1860 */   389,  268,  270,   76,  389,  389,  389,  389,  341,   29,
 /*  1870 */    30,   31,   32,   33,   34,   35,   36,   37,   38,   39,
 /*  1880 */    40,   41,   42,   43,  389,  389,  389,  389,  389,  389,
 /*  1890 */   389,  389,   44,  389,  389,  268,  270,  389,  389,  389,
 /*  1900 */   389,  389,  389,  389,  389,   68,  389,  389,  389,  389,
 /*  1910 */   341,   29,   30,   31,   32,   33,   34,   35,   36,   37,
 /*  1920 */    38,   39,   40,   41,   42,   43,   44,  389,  422,   17,
 /*  1930 */   437,  455,  389,  450,  461,  425,   19,  268,  270,  421,
 /*  1940 */   389,  389,  389,  389,   28,   29,   30,   31,   32,   33,
 /*  1950 */    34,   35,   36,   37,   38,   39,   40,   41,   42,   43,
 /*  1960 */   389,  389,  389,  392,  389,  104,  156,  160,  154,  192,
 /*  1970 */   101,  268,  270,  149,  389,  389,  389,  389,   23,  389,
 /*  1980 */   381,  389,  389,  162,  389,  137,  104,  156,  160,  154,
 /*  1990 */   192,  101,  211,  389,  149,  138,  139,  140,  141,  142,
 /*  2000 */   143,  389,  389,  389,  162,  389,  137,  389,  398,  399,
 /*  2010 */   400,  401,  389,  389,  170,  230,  389,  451,  452,  453,
 /*  2020 */   454,  389,  163,  164,  389,  389,  389,  389,  389,  389,
 /*  2030 */   165,  166,  167,  168,  169,  170,  389,  389,  512,  389,
 /*  2040 */   403,  405,  389,  163,  164,  389,  389,  402,  404,  403,
 /*  2050 */   389,  165,  166,  167,  168,  169,  402,  404,  389,  398,
 /*  2060 */   399,  400,  401,  104,  156,  160,  154,  192,  101,  223,
 /*  2070 */   389,  149,  389,  389,  389,  389,  389,  389,  389,  389,
 /*  2080 */   389,  162,  389,  137,  104,  156,  160,  154,  192,  101,
 /*  2090 */   205,  403,  149,  389,  389,  389,  389,  389,  402,  404,
 /*  2100 */   389,  389,  162,  389,  137,  104,  156,  160,  154,  192,
 /*  2110 */   101,  389,  170,  149,  389,  389,  389,  389,  389,  389,
 /*  2120 */   163,  164,  389,  162,  389,  137,  389,  389,  165,  166,
 /*  2130 */   167,  168,  169,  170,  389,  389,  389,  389,  389,  389,
 /*  2140 */   389,  163,  164,  389,  389,  389,  342,  389,  389,  165,
 /*  2150 */   166,  167,  168,  169,  170,  389,  398,  399,  400,  401,
 /*  2160 */   389,  389,  163,  164,  389,  389,  389,  389,  389,  389,
 /*  2170 */   165,  166,  167,  168,  169,  389,  389,  389,  389,  389,
 /*  2180 */   389,  389,  104,  156,  160,  154,  192,  101,  403,  389,
 /*  2190 */   149,  427,  389,  389,  389,  402,  404,  389,  389,  389,
 /*  2200 */   162,  389,  137,  104,  156,  160,  154,  192,  101,  389,
 /*  2210 */   389,  149,  389,  389,  389,  389,  389,  389,  389,  389,
 /*  2220 */   389,  162,  389,  137,  389,  398,  399,  400,  401,  389,
 /*  2230 */   389,  170,  251,  389,  389,  389,  389,  389,  389,  163,
 /*  2240 */   164,  389,  389,  389,  389,  389,  389,  165,  166,  167,
 /*  2250 */   168,  169,  170,  243,  389,  511,  389,  403,  405,  389,
 /*  2260 */   163,  164,  389,  389,  402,  404,  389,  389,  165,  166,
 /*  2270 */   167,  168,  169,  104,  156,  160,  154,  192,  101,  195,
 /*  2280 */   389,  149,  389,  389,  389,  389,  389,  389,  389,  389,
 /*  2290 */   389,  162,  389,  137,  104,  156,  160,  154,  192,  101,
 /*  2300 */   389,  389,  149,  389,  389,  389,  389,  389,  389,  389,
 /*  2310 */   389,  389,  162,  389,  137,  389,  104,  156,  160,  154,
 /*  2320 */   192,  101,  170,  389,  149,  389,  389,  389,  389,  389,
 /*  2330 */   163,  164,  389,  389,  162,  354,  137,  389,  165,  166,
 /*  2340 */   167,  168,  169,  170,  389,  389,  389,  389,  389,  389,
 /*  2350 */   389,  163,  164,  389,  389,  389,  389,  389,  389,  165,
 /*  2360 */   166,  167,  168,  169,  389,  170,  389,  389,  389,  389,
 /*  2370 */   389,  389,  389,  163,  164,  389,  389,  389,   91,  389,
 /*  2380 */   389,  165,  166,  167,  168,  169,  389,  389,  133,  135,
 /*  2390 */   100,  115,  119,  121,  123,  125,  117,  103,  127,  129,
 /*  2400 */   131,  389,  389,  389,  389,   92,   93,   94,   95,   96,
 /*  2410 */   225,  250,  722,  238,  258,  259,  389,  389,  389,  133,
 /*  2420 */   135,  100,  115,  119,  121,  123,  125,  117,  103,  127,
 /*  2430 */   129,  131,  485,   16,  462,  468,  467,  455,  389,  450,
 /*  2440 */   461,  425,   19,  344,  389,  421,  389,  389,  389,  389,
 /*  2450 */   133,  135,  100,  115,  119,  121,  123,  125,  117,  103,
 /*  2460 */   127,  129,  131,  472,   16,  462,  468,  467,  455,  389,
 /*  2470 */   450,  461,  425,   19,   82,  389,  421,  389,  389,  389,
 /*  2480 */   389,  133,  135,  100,  115,  119,  121,  123,  125,  117,
 /*  2490 */   103,  127,  129,  131,  389,  389,  389,  389,  389,  389,
 /*  2500 */   145,  389,  389,  389,  389,  389,  389,  133,  135,  100,
 /*  2510 */   115,  119,  121,  123,  125,  117,  103,  127,  129,  131,
 /*  2520 */    13,   16,  462,  468,  467,  455,  389,  450,  461,  425,
 /*  2530 */    19,  356,  389,  421,  389,  389,  389,  389,  133,  135,
 /*  2540 */   100,  115,  119,  121,  123,  125,  117,  103,  127,  129,
 /*  2550 */   131,  133,  135,  100,  115,  119,  121,  123,  125,  117,
 /*  2560 */   103,  127,  129,  131,  389,  451,  452,  453,  454,  389,
 /*  2570 */   389,  389,  389,  389,  389,  351,  389,  389,  389,  389,
 /*  2580 */   389,  389,  133,  135,  100,  115,  119,  121,  123,  125,
 /*  2590 */   117,  103,  127,  129,  131,  487,  389,  403,  438,  389,
 /*  2600 */   432,  624,  389,  389,  402,  404,  451,  452,  453,  454,
 /*  2610 */   389,  389,  389,  389,  389,  389,  389,  389,  389,  389,
 /*  2620 */   389,  389,  389,  389,  389,  389,  389,  389,  389,  389,
 /*  2630 */   389,  389,  389,  389,  389,  389,  481,  389,  403,  438,
 /*  2640 */   389,  432,  389,  389,  389,  402,  404,  398,  399,  400,
 /*  2650 */   401,  389,  389,  389,  389,  389,  389,  389,  389,  389,
 /*  2660 */   389,  389,  389,  389,  389,  389,  389,  389,  389,  389,
 /*  2670 */   389,  389,  389,  389,  389,  389,  389,  509,  389,  403,
 /*  2680 */   426,  389,  432,  389,  389,  389,  402,  404,  451,  452,
 /*  2690 */   453,  454,  389,  389,  389,  389,  389,  389,  389,  389,
 /*  2700 */   389,  389,  389,  389,  389,  389,  389,  389,  389,  389,
 /*  2710 */   389,  389,  389,  389,  389,  389,  389,  389,  505,  389,
 /*  2720 */   403,  438,  389,  432,  389,  389,  389,  402,  404,  451,
 /*  2730 */   452,  453,  454,  389,  389,  389,  389,  389,  389,  389,
 /*  2740 */   389,  389,  389,  389,  389,  389,  389,  389,  389,  389,
 /*  2750 */   389,  389,  389,  389,  389,  389,  389,  389,  389,   15,
 /*  2760 */   389,  403,  438,  389,  432,  389,  389,  389,  402,  404,
 /*  2770 */   451,  452,  453,  454,  389,  389,  389,  389,  389,  389,
 /*  2780 */   389,  389,  389,  389,  389,  389,  389,  389,  389,  389,
 /*  2790 */   389,  389,  389,  389,  389,  389,  389,  389,  389,  389,
 /*  2800 */   506,  389,  403,  438,  389,  432,  389,  389,  389,  402,
 /*  2810 */   404,  451,  452,  453,  454,  398,  399,  400,  401,  389,
 /*  2820 */   389,  389,  389,  389,  389,  389,  389,  389,  389,  389,
 /*  2830 */   389,  389,  389,  389,  389,  389,  389,  389,  389,  389,
 /*  2840 */   389,  474,  389,  403,  438,  508,  432,  403,  405,  389,
 /*  2850 */   402,  404,  389,  389,  402,  404,  398,  399,  400,  401,
 /*  2860 */   451,  452,  453,  454,  389,  389,  389,  389,  389,  389,
 /*  2870 */   389,  389,  389,  389,  389,  389,  389,  389,  389,  389,
 /*  2880 */   389,  389,  389,  389,  389,  389,  507,  389,  403,  405,
 /*  2890 */   510,  389,  403,  465,  389,  402,  404,  389,  389,  402,
 /*  2900 */   404,  398,  399,  400,  401,  389,  389,  389,  389,  389,
 /*  2910 */   389,  389,  389,  389,  389,  389,  389,  389,  389,  389,
 /*  2920 */   389,  389,  389,  389,  389,  389,  389,  389,  389,  389,
 /*  2930 */   389,  513,  389,  403,  405,  451,  452,  453,  454,  389,
 /*  2940 */   402,  404,  389,  389,  389,  389,  389,  389,  389,  389,
 /*  2950 */   389,  389,  389,  389,  389,  389,  389,  389,  389,  389,
 /*  2960 */   389,  389,  389,  389,  389,  389,  389,  403,  389,  389,
 /*  2970 */   427,  389,  389,  389,  402,  404,
};
static YYCODETYPE yy_lookahead[] = {
 /*     0 */    27,   76,   77,  103,   31,  131,  132,   34,    5,   27,
 /*    10 */    85,   86,   87,   88,    0,   42,   43,   44,   45,  133,
 /*    20 */    47,   48,   49,   50,    5,   52,  140,  141,  142,   56,
 /*    30 */    57,   58,   28,   98,   61,  100,  101,  102,   16,  104,
 /*    40 */   105,  106,  107,   16,   25,  110,   73,   74,   75,  134,
 /*    50 */   135,   78,   79,   80,   81,   82,   83,   27,   76,   77,
 /*    60 */    38,   31,   59,   27,   34,   38,   35,   85,   86,   87,
 /*    70 */    88,   27,   42,   43,   44,   45,  133,   47,   48,   49,
 /*    80 */    50,  138,   52,    5,  141,  142,   56,   57,   58,   31,
 /*    90 */    10,   61,   27,   62,   63,   64,   65,   66,   16,   68,
 /*   100 */   111,  112,  113,   73,   74,   75,   28,   27,   78,   79,
 /*   110 */    80,   81,   82,   83,   27,   39,   67,  112,   31,   70,
 /*   120 */    38,   34,   42,   43,   44,   45,   46,   47,   35,   42,
 /*   130 */    43,   44,   45,   30,   47,   48,   49,   50,  133,   52,
 /*   140 */     5,   76,   77,   56,   57,   58,  141,  142,   61,   27,
 /*   150 */    85,   86,   87,   88,   27,   62,   63,   64,   65,   66,
 /*   160 */    73,   74,   75,   28,   37,   78,   79,   80,   81,   82,
 /*   170 */    83,   27,   16,   17,   18,   31,  144,  145,   34,   90,
 /*   180 */    91,   92,   93,   94,   95,   96,   42,   43,   44,   45,
 /*   190 */    28,   47,   48,   49,   50,  133,   52,    5,   28,  137,
 /*   200 */    56,   57,   58,  141,  142,   61,  100,  101,  102,   30,
 /*   210 */   104,   32,  106,  107,  144,  145,  110,   73,   74,   75,
 /*   220 */    28,  133,   78,   79,   80,   81,   82,   83,   27,  141,
 /*   230 */   142,  143,   31,  133,   23,   34,  148,   35,   27,  139,
 /*   240 */   140,  141,  142,   42,   43,   44,   45,   30,   47,   48,
 /*   250 */    49,   50,  133,   52,    5,   51,  137,   56,   57,   58,
 /*   260 */   141,  142,   61,  133,   62,   63,   64,   65,   66,   31,
 /*   270 */    68,  141,  142,  143,   73,   74,   75,   28,  148,   78,
 /*   280 */    79,   80,   81,   82,   83,   27,   30,  130,  103,   31,
 /*   290 */   133,   53,   34,   55,    1,    2,    3,    4,  141,  142,
 /*   300 */    42,   43,   44,   45,  132,   47,   48,   49,   50,  133,
 /*   310 */    52,  144,  145,  133,   56,   57,   58,  141,  142,   61,
 /*   320 */    27,  141,  142,  143,  148,   39,  133,   30,  148,   32,
 /*   330 */   137,   73,   74,   75,  141,  142,   78,   79,   80,   81,
 /*   340 */    82,   83,   27,  106,  107,  100,   31,  110,   30,   34,
 /*   350 */   105,  106,  107,   67,   68,  110,   70,   42,   43,   44,
 /*   360 */    45,    5,   47,   48,   49,   50,  133,   52,    5,  133,
 /*   370 */   137,   56,   57,   58,  141,  142,   61,  141,  142,  143,
 /*   380 */   107,   25,  108,  110,  148,  111,  112,  113,   73,   74,
 /*   390 */    75,   28,  133,   78,   79,   80,   81,   82,   83,   27,
 /*   400 */   141,  142,  143,   31,  144,  145,   34,  148,    0,   54,
 /*   410 */     1,    2,    3,    4,   42,   43,   44,   45,  133,   47,
 /*   420 */    48,   49,   50,  138,   52,    5,  141,  142,   56,   57,
 /*   430 */    58,  133,  130,   61,   26,  133,   27,   29,   27,  141,
 /*   440 */   142,   33,   34,  141,  142,   73,   74,   75,   28,  103,
 /*   450 */    78,   79,   80,   81,   82,   83,   27,  133,  144,  145,
 /*   460 */    31,  137,  102,   34,  104,  141,  142,  107,  103,  135,
 /*   470 */   110,   42,   43,   44,   45,   30,   47,   48,   49,   50,
 /*   480 */   133,   52,    5,   30,  137,   56,   57,   58,  141,  142,
 /*   490 */    61,   26,  133,  147,   29,  149,  150,  151,   33,   34,
 /*   500 */   141,  142,   73,   74,   75,   28,   27,   78,   79,   80,
 /*   510 */    81,   82,   83,   27,  149,  150,  151,   31,  144,  145,
 /*   520 */    34,  144,  145,   93,   94,   95,   96,   31,   42,   43,
 /*   530 */    44,   45,    5,   47,   48,   49,   50,  133,   52,  144,
 /*   540 */   145,  137,   56,   57,   58,  141,  142,   61,  133,   53,
 /*   550 */   136,   55,  137,   27,  133,   28,  141,  142,  137,   73,
 /*   560 */    74,   75,  141,  142,   78,   79,   80,   81,   82,   83,
 /*   570 */    27,  133,  130,  133,   31,  133,  138,   34,  146,  141,
 /*   580 */   142,  141,  142,  141,  142,   42,   43,   44,   45,  133,
 /*   590 */    47,   48,   49,   50,   69,   52,   28,  141,  142,   56,
 /*   600 */    57,   58,  108,   35,   61,  111,  112,  113,  144,  145,
 /*   610 */    27,  133,    5,    5,   27,   68,   73,   74,   75,  141,
 /*   620 */   142,   78,   79,   80,   81,   82,   83,   27,  133,   16,
 /*   630 */   133,   31,  133,   25,   34,   28,  141,  142,  141,  142,
 /*   640 */   141,  142,   42,   43,   44,   45,  133,   47,   48,   49,
 /*   650 */    50,   38,   52,   69,  141,  142,   56,   57,   58,   76,
 /*   660 */    77,   61,  133,   76,   77,    5,    5,    5,  133,   25,
 /*   670 */   141,  142,   27,   73,   74,   75,  141,  142,   78,   79,
 /*   680 */    80,   81,   82,   83,   27,   69,  133,    5,   31,   28,
 /*   690 */    28,   34,  144,  145,  141,  142,   67,   68,   27,   42,
 /*   700 */    43,   44,   45,  133,   47,   48,   49,   50,  133,   52,
 /*   710 */    28,  141,  142,   56,   57,   58,  141,  142,   61,  133,
 /*   720 */    16,   76,   77,    5,   16,  133,   39,  141,  142,   69,
 /*   730 */    73,   74,   75,  141,  142,   78,   79,   80,   81,   82,
 /*   740 */    83,   27,   38,   25,  133,   31,   38,    5,   34,   27,
 /*   750 */     5,  103,  141,  142,   54,  146,   42,   43,   44,   45,
 /*   760 */   133,   47,   48,   49,   50,  133,   52,   25,  141,  142,
 /*   770 */    56,   57,   58,  141,  142,   61,  133,   54,   54,   54,
 /*   780 */    27,    5,  133,   39,  141,  142,   39,   73,   74,   75,
 /*   790 */   141,  142,   78,   79,   80,   81,   82,   83,   27,  151,
 /*   800 */    28,   25,   31,   69,    5,   34,   69,   35,   69,   27,
 /*   810 */   136,  136,   69,   42,   43,   44,   45,  133,   47,   48,
 /*   820 */    49,   50,  136,   52,   25,  141,  142,   56,   57,   58,
 /*   830 */    27,  136,   61,  136,   69,  136,   69,  136,  136,   28,
 /*   840 */    28,   27,   27,   69,   73,   74,   75,   28,   28,   78,
 /*   850 */    79,   80,   81,   82,   83,   27,   69,   28,   69,   31,
 /*   860 */    28,   28,   34,   27,   39,   28,   27,   27,   27,   35,
 /*   870 */    42,   43,   44,   45,  145,   47,   48,   49,   50,   27,
 /*   880 */    52,   59,   51,   30,   56,   57,   58,   30,  103,   61,
 /*   890 */    35,   27,   54,   35,  103,   27,   35,  103,   27,   11,
 /*   900 */    27,   73,   74,   75,   27,  110,   78,   79,   80,   81,
 /*   910 */    82,   83,   27,   39,   30,   37,   31,   30,   35,   34,
 /*   920 */   103,   28,   28,   37,   35,  104,  103,   42,   43,   44,
 /*   930 */    45,   28,   47,   48,   49,   50,   28,   52,  103,   27,
 /*   940 */    30,   56,   57,   58,   31,  103,   61,   29,   27,   27,
 /*   950 */    31,   31,  152,   30,  152,  152,  152,  152,   73,   74,
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
 /*  1240 */    27,  152,  152,   73,   74,   75,  152,   34,   78,   79,
 /*  1250 */    80,   81,   82,   83,  152,   42,   43,   44,   45,  152,
 /*  1260 */    47,   48,   49,   50,  152,   52,   10,  152,  152,   56,
 /*  1270 */    57,   58,  152,  152,   61,  152,  152,  152,  152,  152,
 /*  1280 */   152,   25,  152,   27,  152,  152,   73,   74,   75,  152,
 /*  1290 */   152,   78,   79,   80,   81,   82,   83,  152,   42,   43,
 /*  1300 */    44,   45,   46,   47,  152,  152,   19,   20,   21,   22,
 /*  1310 */    23,   24,  152,  152,   27,  152,  152,  152,  152,  152,
 /*  1320 */   152,  152,  152,  152,   37,  152,   39,  152,  152,   42,
 /*  1330 */    43,   44,   45,   46,   47,  152,    6,    7,    8,    9,
 /*  1340 */    10,   11,   12,   13,   14,   15,   16,   17,   18,  152,
 /*  1350 */   152,  152,  152,  152,  152,   68,  152,  152,   28,   27,
 /*  1360 */   152,  152,  152,   76,   77,  152,   96,  152,  152,  152,
 /*  1370 */   152,   84,   85,   86,   87,   88,  152,  152,  152,  109,
 /*  1380 */   152,  152,  152,  152,  114,  115,  116,  117,  118,  119,
 /*  1390 */   120,  121,  122,  123,  124,  125,  126,  127,  128,  129,
 /*  1400 */   152,   69,  152,   96,  152,  152,  152,  152,   76,   77,
 /*  1410 */   152,  141,  142,  152,  152,  152,  109,   85,   86,   87,
 /*  1420 */    88,  114,  115,  116,  117,  118,  119,  120,  121,  122,
 /*  1430 */   123,  124,  125,  126,  127,  128,  129,   97,   98,   99,
 /*  1440 */   100,  101,  102,  152,  104,  105,  106,  107,  141,  142,
 /*  1450 */   110,  152,   96,    8,    9,   10,   11,   12,   13,   14,
 /*  1460 */    15,   16,   17,   18,  152,  109,  152,  152,  152,  152,
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
 /*  1960 */   152,  152,  152,   10,  152,   19,   20,   21,   22,   23,
 /*  1970 */    24,  141,  142,   27,  152,  152,  152,  152,   25,  152,
 /*  1980 */    27,  152,  152,   37,  152,   39,   19,   20,   21,   22,
 /*  1990 */    23,   24,   25,  152,   27,   42,   43,   44,   45,   46,
 /*  2000 */    47,  152,  152,  152,   37,  152,   39,  152,    1,    2,
 /*  2010 */     3,    4,  152,  152,   68,   69,  152,    1,    2,    3,
 /*  2020 */     4,  152,   76,   77,  152,  152,  152,  152,  152,  152,
 /*  2030 */    84,   85,   86,   87,   88,   68,  152,  152,   31,  152,
 /*  2040 */    33,   34,  152,   76,   77,  152,  152,   40,   41,   33,
 /*  2050 */   152,   84,   85,   86,   87,   88,   40,   41,  152,    1,
 /*  2060 */     2,    3,    4,   19,   20,   21,   22,   23,   24,   25,
 /*  2070 */   152,   27,  152,  152,  152,  152,  152,  152,  152,  152,
 /*  2080 */   152,   37,  152,   39,   19,   20,   21,   22,   23,   24,
 /*  2090 */    25,   33,   27,  152,  152,  152,  152,  152,   40,   41,
 /*  2100 */   152,  152,   37,  152,   39,   19,   20,   21,   22,   23,
 /*  2110 */    24,  152,   68,   27,  152,  152,  152,  152,  152,  152,
 /*  2120 */    76,   77,  152,   37,  152,   39,  152,  152,   84,   85,
 /*  2130 */    86,   87,   88,   68,  152,  152,  152,  152,  152,  152,
 /*  2140 */   152,   76,   77,  152,  152,  152,   60,  152,  152,   84,
 /*  2150 */    85,   86,   87,   88,   68,  152,    1,    2,    3,    4,
 /*  2160 */   152,  152,   76,   77,  152,  152,  152,  152,  152,  152,
 /*  2170 */    84,   85,   86,   87,   88,  152,  152,  152,  152,  152,
 /*  2180 */   152,  152,   19,   20,   21,   22,   23,   24,   33,  152,
 /*  2190 */    27,   36,  152,  152,  152,   40,   41,  152,  152,  152,
 /*  2200 */    37,  152,   39,   19,   20,   21,   22,   23,   24,  152,
 /*  2210 */   152,   27,  152,  152,  152,  152,  152,  152,  152,  152,
 /*  2220 */   152,   37,  152,   39,  152,    1,    2,    3,    4,  152,
 /*  2230 */   152,   68,   69,  152,  152,  152,  152,  152,  152,   76,
 /*  2240 */    77,  152,  152,  152,  152,  152,  152,   84,   85,   86,
 /*  2250 */    87,   88,   68,   69,  152,   31,  152,   33,   34,  152,
 /*  2260 */    76,   77,  152,  152,   40,   41,  152,  152,   84,   85,
 /*  2270 */    86,   87,   88,   19,   20,   21,   22,   23,   24,   25,
 /*  2280 */   152,   27,  152,  152,  152,  152,  152,  152,  152,  152,
 /*  2290 */   152,   37,  152,   39,   19,   20,   21,   22,   23,   24,
 /*  2300 */   152,  152,   27,  152,  152,  152,  152,  152,  152,  152,
 /*  2310 */   152,  152,   37,  152,   39,  152,   19,   20,   21,   22,
 /*  2320 */    23,   24,   68,  152,   27,  152,  152,  152,  152,  152,
 /*  2330 */    76,   77,  152,  152,   37,   60,   39,  152,   84,   85,
 /*  2340 */    86,   87,   88,   68,  152,  152,  152,  152,  152,  152,
 /*  2350 */   152,   76,   77,  152,  152,  152,  152,  152,  152,   84,
 /*  2360 */    85,   86,   87,   88,  152,   68,  152,  152,  152,  152,
 /*  2370 */   152,  152,  152,   76,   77,  152,  152,  152,   35,  152,
 /*  2380 */   152,   84,   85,   86,   87,   88,  152,  152,    6,    7,
 /*  2390 */     8,    9,   10,   11,   12,   13,   14,   15,   16,   17,
 /*  2400 */    18,  152,  152,  152,  152,   62,   63,   64,   65,   66,
 /*  2410 */    67,   68,   30,   70,   71,   72,  152,  152,  152,    6,
 /*  2420 */     7,    8,    9,   10,   11,   12,   13,   14,   15,   16,
 /*  2430 */    17,   18,   97,   98,   99,  100,  101,  102,  152,  104,
 /*  2440 */   105,  106,  107,   30,  152,  110,  152,  152,  152,  152,
 /*  2450 */     6,    7,    8,    9,   10,   11,   12,   13,   14,   15,
 /*  2460 */    16,   17,   18,   97,   98,   99,  100,  101,  102,  152,
 /*  2470 */   104,  105,  106,  107,   30,  152,  110,  152,  152,  152,
 /*  2480 */   152,    6,    7,    8,    9,   10,   11,   12,   13,   14,
 /*  2490 */    15,   16,   17,   18,  152,  152,  152,  152,  152,  152,
 /*  2500 */    25,  152,  152,  152,  152,  152,  152,    6,    7,    8,
 /*  2510 */     9,   10,   11,   12,   13,   14,   15,   16,   17,   18,
 /*  2520 */    97,   98,   99,  100,  101,  102,  152,  104,  105,  106,
 /*  2530 */   107,   30,  152,  110,  152,  152,  152,  152,    6,    7,
 /*  2540 */     8,    9,   10,   11,   12,   13,   14,   15,   16,   17,
 /*  2550 */    18,    6,    7,    8,    9,   10,   11,   12,   13,   14,
 /*  2560 */    15,   16,   17,   18,  152,    1,    2,    3,    4,  152,
 /*  2570 */   152,  152,  152,  152,  152,   30,  152,  152,  152,  152,
 /*  2580 */   152,  152,    6,    7,    8,    9,   10,   11,   12,   13,
 /*  2590 */    14,   15,   16,   17,   18,   31,  152,   33,   34,  152,
 /*  2600 */    36,   69,  152,  152,   40,   41,    1,    2,    3,    4,
 /*  2610 */   152,  152,  152,  152,  152,  152,  152,  152,  152,  152,
 /*  2620 */   152,  152,  152,  152,  152,  152,  152,  152,  152,  152,
 /*  2630 */   152,  152,  152,  152,  152,  152,   31,  152,   33,   34,
 /*  2640 */   152,   36,  152,  152,  152,   40,   41,    1,    2,    3,
 /*  2650 */     4,  152,  152,  152,  152,  152,  152,  152,  152,  152,
 /*  2660 */   152,  152,  152,  152,  152,  152,  152,  152,  152,  152,
 /*  2670 */   152,  152,  152,  152,  152,  152,  152,   31,  152,   33,
 /*  2680 */    34,  152,   36,  152,  152,  152,   40,   41,    1,    2,
 /*  2690 */     3,    4,  152,  152,  152,  152,  152,  152,  152,  152,
 /*  2700 */   152,  152,  152,  152,  152,  152,  152,  152,  152,  152,
 /*  2710 */   152,  152,  152,  152,  152,  152,  152,  152,   31,  152,
 /*  2720 */    33,   34,  152,   36,  152,  152,  152,   40,   41,    1,
 /*  2730 */     2,    3,    4,  152,  152,  152,  152,  152,  152,  152,
 /*  2740 */   152,  152,  152,  152,  152,  152,  152,  152,  152,  152,
 /*  2750 */   152,  152,  152,  152,  152,  152,  152,  152,  152,   31,
 /*  2760 */   152,   33,   34,  152,   36,  152,  152,  152,   40,   41,
 /*  2770 */     1,    2,    3,    4,  152,  152,  152,  152,  152,  152,
 /*  2780 */   152,  152,  152,  152,  152,  152,  152,  152,  152,  152,
 /*  2790 */   152,  152,  152,  152,  152,  152,  152,  152,  152,  152,
 /*  2800 */    31,  152,   33,   34,  152,   36,  152,  152,  152,   40,
 /*  2810 */    41,    1,    2,    3,    4,    1,    2,    3,    4,  152,
 /*  2820 */   152,  152,  152,  152,  152,  152,  152,  152,  152,  152,
 /*  2830 */   152,  152,  152,  152,  152,  152,  152,  152,  152,  152,
 /*  2840 */   152,   31,  152,   33,   34,   31,   36,   33,   34,  152,
 /*  2850 */    40,   41,  152,  152,   40,   41,    1,    2,    3,    4,
 /*  2860 */     1,    2,    3,    4,  152,  152,  152,  152,  152,  152,
 /*  2870 */   152,  152,  152,  152,  152,  152,  152,  152,  152,  152,
 /*  2880 */   152,  152,  152,  152,  152,  152,   31,  152,   33,   34,
 /*  2890 */    31,  152,   33,   34,  152,   40,   41,  152,  152,   40,
 /*  2900 */    41,    1,    2,    3,    4,  152,  152,  152,  152,  152,
 /*  2910 */   152,  152,  152,  152,  152,  152,  152,  152,  152,  152,
 /*  2920 */   152,  152,  152,  152,  152,  152,  152,  152,  152,  152,
 /*  2930 */   152,   31,  152,   33,   34,    1,    2,    3,    4,  152,
 /*  2940 */    40,   41,  152,  152,  152,  152,  152,  152,  152,  152,
 /*  2950 */   152,  152,  152,  152,  152,  152,  152,  152,  152,  152,
 /*  2960 */   152,  152,  152,  152,  152,  152,  152,   33,  152,  152,
 /*  2970 */    36,  152,  152,  152,   40,   41,
};
#define YY_SHIFT_USE_DFLT (-76)
static short yy_shift_ofst[] = {
 /*     0 */   465,   14,  408,  -76,  -76,  -76,  -76,   36,    4,  -76,
 /*    10 */    44,  297, 2728,   58,  -76,  -76, 2687, 2814,  -76,  708,
 /*    20 */   122,   76, 1953,  103,  828,  -76, 1170,  -76,  -76,  -76,
 /*    30 */   -76,  -76,  -76,  -76,  -76,  -76,  -76,  -76,  -76,  -76,
 /*    40 */   -76,  -76,  -76,  -76,  -76,  162,  -76,  170,  -76, 2297,
 /*    50 */   217,  600,  543,  204,  256,  486,  429,  -76, 2297,  318,
 /*    60 */   238,  -76,  496,  -76,  -76,  -75,  355, 1213, 1213,  445,
 /*    70 */   144,   30,  -76, 2297,  453,  201,   87,  -76,  411,    3,
 /*    80 */  2086, 2444, 1213,  657,  -76,  479,   78,  -76,  479,  -76,
 /*    90 */  2343,  -76,  -76,  -76,  -76,  -76,  -76, 2297,  -76, 2576,
 /*   100 */  2297, 2297,  -76, 2297,  526,  -76,  547,  587,  525,  -76,
 /*   110 */   584,  -76,  616,  -76,  156, 2297,  156, 2297,  156, 2297,
 /*   120 */   156, 2297,  156, 2297,  156, 2297,  156, 2297,  -76, 2297,
 /*   130 */   -76, 2297,  -76, 2297, 1445, 2297, 1445, 1287,  -76,  -76,
 /*   140 */   -76,  -76,  -76,  -76, 2475,  -76,  644, 2297,  -76,  286,
 /*   150 */   671,  687, 2065, 2576, 2297,  -76,  722,  745,  526,  -76,
 /*   160 */  2297,  -76,  -76,  -76,  -76,  -76,  -76,  -76,  -76,  -76,
 /*   170 */  1332,  -76,  660,  -76,   65,  -76,  700,  -18,  -76,  -76,
 /*   180 */   -76,  -76,  -76,  -76,  -76,  -76,  -76,  -76,  723,  724,
 /*   190 */   725,  -76,  753,  744, 2254,  -76,  718,  -76, 2297,  -76,
 /*   200 */   -76,  -76,  -76,  742,  -76,  -76,  127,  747, 1967,  776,
 /*   210 */   -76,  -76,  -76,  583,  734,  -76,  737,  -76,  739,  -76,
 /*   220 */  2044,  799,  -76,  -76,  156,  782,   31, 2297,  -76, 1946,
 /*   230 */    93, 2297,  -76,  743,   93, 2297,  -76, 2532,  803,  202,
 /*   240 */  2297,  -76, 2184,   93, 2297,  -76,  765,   93, 2297,  -76,
 /*   250 */  2163,   93, 2297,  -76,  767,   93, 2297,  -76,  -76,  -76,
 /*   260 */   -76, 2297,  682,  -76, 2297,  -76, 2576,  -76,  811,  -76,
 /*   270 */   812,  -76, 2297, 1330,  -76,  814,  629,  815,  819,  -76,
 /*   280 */   645,  774,  820,  -76,  787,  829,  -76,  789,  832,  -76,
 /*   290 */   211,  833,  -76,  836,  825, 2297,  608,  837,  -76,  839,
 /*   300 */   662,  -76,  839,  -76,  834,  -75,  -76,  -76,  839,  661,
 /*   310 */   -76,  839,  607,  -76,  839,  527,  -76,  839,  477,  -76,
 /*   320 */   839,  420,  -76,  839,  363,  -76,  839,  249,  -76,  839,
 /*   330 */   192,  -76,  839,  135,  -76,   49,  840,  747,  841,  687,
 /*   340 */   -76,  -76, 2297, 2413, 1213,  258,  -76,  852,  822, 2275,
 /*   350 */  2545, 1213,  315,  -76, 2297, 2501, 1213,  372,  -76,  -76,
 /*   360 */  2382,  -76,  838, 1213, 1213,  -76,  -76,  831,  853,  714,
 /*   370 */   771,  -76,  -76,   19,  857,  -27,  -76,  885,  -76,   80,
 /*   380 */   -76,  855,  -75,  -76,  864,  858,  -75,  -76,  868,  861,
 /*   390 */   -75,  -76,  871,  888,  873,  -76, 2058,  -76,  -76,  -76,
 /*   400 */   -76,  -76,  -76,  -76,  -76, 2058,   27,  877,  874, 1256,
 /*   410 */   884,  942,  -76,  999,  -76,  356,  887, 1056,  -76, 1113,
 /*   420 */   -76,  -76, 2646, 2007,  -76,  -76, 2155,  878,  883,  -75,
 /*   430 */   893,  -76,  886,  889,  -75,  894,  -76,  -76, 2934,  409,
 /*   440 */   568,  -76,  -75,  903,  -76,  -76,  -76,  -76,  -76,  -76,
 /*   450 */   -76,   82,  613,  704,   22,  293,  772,  -76,  -75,  908,
 /*   460 */   -76,  -76, 2769, 2859, 2900, 2016, 2224,  -76, 2855,  912,
 /*   470 */   910, 2810,  913,  -76,  -76,  918,  921,  179, 2605,  919,
 /*   480 */   -76,  -76,  922,  923, 2564,  920,  -76,  -76,  -76,
};
#define YY_REDUCE_USE_DFLT (-127)
static short yy_reduce_ofst[] = {
 /*     0 */    89, -127,  430, -127, -127, -127, -127, -127, -127, -127,
 /*    10 */  -127, -127, 2423, -127, -127, -127, 1829,  237, -127, -127,
 /*    20 */  -127, -127,  274, -127, 1610, -127, 1830, -127, -127, -127,
 /*    30 */  -127, -127, -127, -127, -127, -127, -127, -127, -127, -127,
 /*    40 */  -127, -127, -127, -127, -127, -127, -127, -127, -127,  157,
 /*    50 */  -127, 1686, 1830, -127, -127, 1466, 1830, -127,  302, -127,
 /*    60 */  -126, -127,  172, -127, -127,  185, -127, 1796, 1830, -127,
 /*    70 */  1644, 1830, -127,  442, -127, 1754, 1830, -127, -127, -127,
 /*    80 */   643, -127, 1576, 1830, -127,  -85, -127, -127,  334, -127,
 /*    90 */   414, -127, -127, -127, -127, -127, -127,  404, -127, -127,
 /*   100 */   513,  570, -127,  627,  432, -127, -127, -127, -127, -127,
 /*   110 */  -127, -127, -127, -127, -127,  359, -127,  440, -127,  535,
 /*   120 */  -127,  649, -127,  575, -127,  684, -127,  586, -127,  553,
 /*   130 */  -127,  456, -127,  495, -127,  499, -127,    5, -127, -127,
 /*   140 */  -127, -127, -127, -127, -127, -127, -127,  632, -127, -127,
 /*   150 */  -127, -127,  236, -127,  529, -127, -127, -127,  609, -127,
 /*   160 */   478, -127, -127, -127, -127, -127, -127, -127, -127, -127,
 /*   170 */   346, -127, -127, -127,  365, -127, -127,  648, -127, -127,
 /*   180 */  -127, -127, -127, -127, -127, -127, -127, -127, -127, -127,
 /*   190 */  -127, -127, -127, -127,   88, -127, -127, -127,  176, -127,
 /*   200 */  -127, -127, -127, -127, -127, -127, -127, -127,  130, -127,
 /*   210 */  -127, -127, -127, -127, -127, -127, -127, -127, -127, -127,
 /*   220 */   180, -127, -127, -127, -127, -127,  674,  347, -127,  -57,
 /*   230 */   675,  193, -127, -127,  686,  421, -127, -127, -127,  695,
 /*   240 */   324, -127,  438,  697,   62, -127, -127,  699,  415, -127,
 /*   250 */   285,  701,  119, -127, -127,  702,  233, -127, -127, -127,
 /*   260 */  -127,  100, -127, -127, -114, -127, -127, -127, -127, -127,
 /*   270 */  -127, -127,  298, -127, -127, -127, -127, -127, -127, -127,
 /*   280 */  -127, -127, -127, -127, -127, -127, -127, -127, -127, -127,
 /*   290 */  -127, -127, -127, -127, -127,  259, -127, -127, -127,  548,
 /*   300 */  -127, -127,  729, -127, -127, -100, -127, -127,  464, -127,
 /*   310 */  -127,  395, -127, -127,  377, -127, -127,  374, -127, -127,
 /*   320 */   314, -127, -127,  260, -127, -127,  167, -127, -127,   70,
 /*   330 */  -127, -127,   32, -127, -127, -127, -127, -127, -127, -127,
 /*   340 */  -127, -127,  611, -127, 1390, 1830, -127, -127, -127,  592,
 /*   350 */  -127, 1270, 1830, -127,  497, -127, 1424, 1830, -127, -127,
 /*   360 */  -127, -127, -127, 1307, 1830, -127, -127, -127, -127, 1534,
 /*   370 */  1830, -127, -127, -127, -127, 1356, -127, 1830, -127,  -11,
 /*   380 */  -127, -127,  785, -127, -127, -127,  791, -127, -127, -127,
 /*   390 */   794, -127, -127, -127, -127, -127,  795, -127, -127, -127,
 /*   400 */  -127, -127, -127, -127, -127,  273, -127, -127, -127,  494,
 /*   410 */  -127, 1720, -127, 1830, -127, -127, -127, 1500, -127, 1830,
 /*   420 */  -127, -127,  245,  237, -127, -127,  273, -127, -127,  817,
 /*   430 */  -127, -127, -127, -127,  823, -127, -127, -127,  360,  821,
 /*   440 */  -127, -127,  835, -127, -127, -127, -127, -127, -127, -127,
 /*   450 */  -127, -127, -127, -127, -127,  821, -127, -127,  842, -127,
 /*   460 */  -127, -127,  -65,  106,  237,  360,  237, -127,  237, -127,
 /*   470 */  -127, 2366, -127, -127, -127, -127, -127, -127, 1340, -127,
 /*   480 */  -127, -127, -127, -127, 2335, -127, -127, -127, -127,
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
// 2138 "parser.c"
      break;
    case 91:
// 830 "parser.lemon"
{ json_object_put((yypminor->yy159)); }
// 2143 "parser.c"
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
// 2595 "parser.c"
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
// 2628 "parser.c"
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
// 2641 "parser.c"
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
// 2661 "parser.c"
        break;
      case 7:
// 856 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_namespace(yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(26,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 2670 "parser.c"
        break;
      case 8:
// 860 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[-1].minor.yy159, 0, NULL, status->scanner_state);
  yy_destructor(29,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 2680 "parser.c"
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
// 2691 "parser.c"
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
// 2702 "parser.c"
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
// 2714 "parser.c"
        break;
      case 12:
// 876 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_class(yymsp[-2].minor.yy0, NULL, 0, NULL, status->scanner_state);
  yy_destructor(29,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 2724 "parser.c"
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
// 2735 "parser.c"
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
// 2746 "parser.c"
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
// 2758 "parser.c"
        break;
      case 16:
// 892 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_class_definition(yymsp[0].minor.yy159, NULL, NULL, status->scanner_state);
}
// 2765 "parser.c"
        break;
      case 17:
// 896 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_class_definition(NULL, NULL, yymsp[0].minor.yy159, status->scanner_state);
}
// 2772 "parser.c"
        break;
      case 18:
// 900 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_class_definition(NULL, yymsp[0].minor.yy159, NULL, status->scanner_state);
}
// 2779 "parser.c"
        break;
      case 19:
// 904 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_class_definition(yymsp[-1].minor.yy159, yymsp[0].minor.yy159, NULL, status->scanner_state);
}
// 2786 "parser.c"
        break;
      case 20:
// 908 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_class_definition(yymsp[-1].minor.yy159, NULL, yymsp[0].minor.yy159, status->scanner_state);
}
// 2793 "parser.c"
        break;
      case 21:
// 912 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_class_definition(yymsp[0].minor.yy159, NULL, yymsp[-1].minor.yy159, status->scanner_state);
}
// 2800 "parser.c"
        break;
      case 22:
// 916 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_class_definition(NULL, yymsp[0].minor.yy159, yymsp[-1].minor.yy159, status->scanner_state);
}
// 2807 "parser.c"
        break;
      case 23:
// 920 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_class_definition(yymsp[-2].minor.yy159, yymsp[0].minor.yy159, yymsp[-1].minor.yy159, status->scanner_state);
}
// 2814 "parser.c"
        break;
      case 24:
// 924 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_class_definition(yymsp[-1].minor.yy159, yymsp[0].minor.yy159, yymsp[-2].minor.yy159, status->scanner_state);
}
// 2821 "parser.c"
        break;
      case 27:
// 936 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_class_property(yymsp[-2].minor.yy159, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(28,&yymsp[0].minor);
}
// 2829 "parser.c"
        break;
      case 28:
// 940 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_class_property(yymsp[-2].minor.yy159, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[0].minor);
}
// 2837 "parser.c"
        break;
      case 29:
// 944 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_class_property(yymsp[-4].minor.yy159, yymsp[-3].minor.yy0, yymsp[-1].minor.yy159, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(35,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 2846 "parser.c"
        break;
      case 30:
// 948 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_class_property(yymsp[-4].minor.yy159, yymsp[-3].minor.yy0, yymsp[-1].minor.yy159, NULL, status->scanner_state);
  yy_destructor(35,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 2855 "parser.c"
        break;
      case 33:
      case 53:
// 960 "parser.lemon"
{
	yygotominor.yy159 = json_object_new_string("public");
  yy_destructor(1,&yymsp[0].minor);
}
// 2864 "parser.c"
        break;
      case 34:
      case 54:
// 964 "parser.lemon"
{
	yygotominor.yy159 = json_object_new_string("protected");
  yy_destructor(2,&yymsp[0].minor);
}
// 2873 "parser.c"
        break;
      case 35:
      case 55:
// 968 "parser.lemon"
{
	yygotominor.yy159 = json_object_new_string("static");
  yy_destructor(3,&yymsp[0].minor);
}
// 2882 "parser.c"
        break;
      case 36:
      case 56:
// 972 "parser.lemon"
{
	yygotominor.yy159 = json_object_new_string("private");
  yy_destructor(4,&yymsp[0].minor);
}
// 2891 "parser.c"
        break;
      case 41:
// 992 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy159, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(35,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 2901 "parser.c"
        break;
      case 42:
// 996 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy159, NULL, status->scanner_state);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(35,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 2911 "parser.c"
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
// 2923 "parser.c"
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
// 2935 "parser.c"
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
// 2947 "parser.c"
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
// 2959 "parser.c"
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
// 2971 "parser.c"
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
// 2983 "parser.c"
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
// 2995 "parser.c"
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
// 3007 "parser.c"
        break;
      case 51:
// 1032 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_list(yymsp[-2].minor.yy159, yymsp[0].minor.yy159);
  yy_destructor(16,&yymsp[-1].minor);
}
// 3015 "parser.c"
        break;
      case 57:
// 1056 "parser.lemon"
{
	yygotominor.yy159 = json_object_new_string("private");
  yy_destructor(40,&yymsp[0].minor);
}
// 3023 "parser.c"
        break;
      case 58:
// 1060 "parser.lemon"
{
	yygotominor.yy159 = json_object_new_string("abstract");
  yy_destructor(33,&yymsp[0].minor);
}
// 3031 "parser.c"
        break;
      case 59:
// 1064 "parser.lemon"
{
	yygotominor.yy159 = json_object_new_string("final");
  yy_destructor(41,&yymsp[0].minor);
}
// 3039 "parser.c"
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
// 3052 "parser.c"
        break;
      case 62:
// 1116 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_parameter(NULL, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3059 "parser.c"
        break;
      case 63:
// 1120 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_parameter(yymsp[-1].minor.yy159, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3066 "parser.c"
        break;
      case 64:
// 1124 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_parameter(NULL, yymsp[-1].minor.yy159, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3073 "parser.c"
        break;
      case 65:
// 1128 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_parameter(NULL, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy159, status->scanner_state);
  yy_destructor(35,&yymsp[-1].minor);
}
// 3081 "parser.c"
        break;
      case 66:
// 1132 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_parameter(yymsp[-3].minor.yy159, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy159, status->scanner_state);
  yy_destructor(35,&yymsp[-1].minor);
}
// 3089 "parser.c"
        break;
      case 67:
// 1136 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_parameter(NULL, yymsp[-3].minor.yy159, yymsp[-2].minor.yy0, yymsp[0].minor.yy159, status->scanner_state);
  yy_destructor(35,&yymsp[-1].minor);
}
// 3097 "parser.c"
        break;
      case 68:
// 1140 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(10,&yymsp[-2].minor);
  yy_destructor(11,&yymsp[0].minor);
}
// 3106 "parser.c"
        break;
      case 69:
// 1144 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_type(XX_TYPE_INTEGER);
  yy_destructor(42,&yymsp[0].minor);
}
// 3114 "parser.c"
        break;
      case 70:
// 1148 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_type(XX_TYPE_DOUBLE);
  yy_destructor(43,&yymsp[0].minor);
}
// 3122 "parser.c"
        break;
      case 71:
// 1152 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_type(XX_TYPE_BOOL);
  yy_destructor(44,&yymsp[0].minor);
}
// 3130 "parser.c"
        break;
      case 72:
// 1156 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_type(XX_TYPE_STRING);
  yy_destructor(45,&yymsp[0].minor);
}
// 3138 "parser.c"
        break;
      case 73:
// 1160 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_type(XX_TYPE_ARRAY);
  yy_destructor(46,&yymsp[0].minor);
}
// 3146 "parser.c"
        break;
      case 74:
// 1164 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_type(XX_TYPE_VAR);
  yy_destructor(47,&yymsp[0].minor);
}
// 3154 "parser.c"
        break;
      case 93:
// 1240 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_break_statement(status->scanner_state);
  yy_destructor(48,&yymsp[-1].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3163 "parser.c"
        break;
      case 94:
// 1244 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_continue_statement(status->scanner_state);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3172 "parser.c"
        break;
      case 95:
// 1248 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_if_statement(yymsp[-2].minor.yy159, NULL, NULL, status->scanner_state);
  yy_destructor(50,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3182 "parser.c"
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
// 3195 "parser.c"
        break;
      case 97:
// 1256 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_if_statement(yymsp[-3].minor.yy159, yymsp[-1].minor.yy159, NULL, status->scanner_state);
  yy_destructor(50,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3205 "parser.c"
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
// 3218 "parser.c"
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
// 3231 "parser.c"
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
// 3244 "parser.c"
        break;
      case 101:
// 1272 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_switch_statement(yymsp[-2].minor.yy159, NULL, status->scanner_state);
  yy_destructor(52,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3254 "parser.c"
        break;
      case 102:
// 1276 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_switch_statement(yymsp[-3].minor.yy159, yymsp[-1].minor.yy159, status->scanner_state);
  yy_destructor(52,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3264 "parser.c"
        break;
      case 105:
// 1288 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_case_clause(yymsp[-2].minor.yy159, yymsp[0].minor.yy159, status->scanner_state);
  yy_destructor(53,&yymsp[-3].minor);
  yy_destructor(54,&yymsp[-1].minor);
}
// 3273 "parser.c"
        break;
      case 106:
// 1292 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_case_clause(NULL, yymsp[0].minor.yy159, status->scanner_state);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(54,&yymsp[-1].minor);
}
// 3282 "parser.c"
        break;
      case 107:
// 1296 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_loop_statement(NULL, status->scanner_state);
  yy_destructor(56,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3292 "parser.c"
        break;
      case 108:
// 1300 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_loop_statement(yymsp[-1].minor.yy159, status->scanner_state);
  yy_destructor(56,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3302 "parser.c"
        break;
      case 109:
// 1304 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_while_statement(yymsp[-2].minor.yy159, NULL, status->scanner_state);
  yy_destructor(57,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3312 "parser.c"
        break;
      case 110:
// 1308 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_while_statement(yymsp[-3].minor.yy159, yymsp[-1].minor.yy159, status->scanner_state);
  yy_destructor(57,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3322 "parser.c"
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
// 3333 "parser.c"
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
// 3345 "parser.c"
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
// 3357 "parser.c"
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
// 3370 "parser.c"
        break;
      case 115:
// 1328 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_let_statement(yymsp[-1].minor.yy159, status->scanner_state);
  yy_destructor(61,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3379 "parser.c"
        break;
      case 118:
// 1341 "parser.lemon"
{
	yygotominor.yy159 = json_object_new_string("assign");
  yy_destructor(35,&yymsp[0].minor);
}
// 3387 "parser.c"
        break;
      case 119:
// 1346 "parser.lemon"
{
	yygotominor.yy159 = json_object_new_string("add-assign");
  yy_destructor(62,&yymsp[0].minor);
}
// 3395 "parser.c"
        break;
      case 120:
// 1351 "parser.lemon"
{
	yygotominor.yy159 = json_object_new_string("sub-assign");
  yy_destructor(63,&yymsp[0].minor);
}
// 3403 "parser.c"
        break;
      case 121:
// 1355 "parser.lemon"
{
	yygotominor.yy159 = json_object_new_string("mult-assign");
  yy_destructor(64,&yymsp[0].minor);
}
// 3411 "parser.c"
        break;
      case 122:
// 1359 "parser.lemon"
{
	yygotominor.yy159 = json_object_new_string("div-assign");
  yy_destructor(65,&yymsp[0].minor);
}
// 3419 "parser.c"
        break;
      case 123:
// 1363 "parser.lemon"
{
	yygotominor.yy159 = json_object_new_string("concat-assign");
  yy_destructor(66,&yymsp[0].minor);
}
// 3427 "parser.c"
        break;
      case 124:
// 1368 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_let_assignment("variable", yymsp[-1].minor.yy159, yymsp[-2].minor.yy0, NULL, NULL, yymsp[0].minor.yy159, status->scanner_state);
}
// 3434 "parser.c"
        break;
      case 125:
// 1373 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_let_assignment("object-property", yymsp[-1].minor.yy159, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy159, status->scanner_state);
  yy_destructor(67,&yymsp[-3].minor);
}
// 3442 "parser.c"
        break;
      case 126:
// 1378 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_let_assignment("object-property-append", yymsp[-1].minor.yy159, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy159, status->scanner_state);
  yy_destructor(67,&yymsp[-5].minor);
  yy_destructor(68,&yymsp[-3].minor);
  yy_destructor(69,&yymsp[-2].minor);
}
// 3452 "parser.c"
        break;
      case 127:
// 1383 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_let_assignment("object-property-array-index", yymsp[-1].minor.yy159, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy159, yymsp[0].minor.yy159, status->scanner_state);
  yy_destructor(67,&yymsp[-6].minor);
  yy_destructor(68,&yymsp[-4].minor);
  yy_destructor(69,&yymsp[-2].minor);
}
// 3462 "parser.c"
        break;
      case 128:
// 1388 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_let_assignment("static-property", yymsp[-1].minor.yy159, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy159, status->scanner_state);
  yy_destructor(70,&yymsp[-3].minor);
}
// 3470 "parser.c"
        break;
      case 129:
// 1393 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_let_assignment("static-property-append", yymsp[-1].minor.yy159, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy159, status->scanner_state);
  yy_destructor(70,&yymsp[-5].minor);
  yy_destructor(68,&yymsp[-3].minor);
  yy_destructor(69,&yymsp[-2].minor);
}
// 3480 "parser.c"
        break;
      case 130:
// 1398 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_let_assignment("static-property-array-index", yymsp[-1].minor.yy159, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy159, yymsp[0].minor.yy159, status->scanner_state);
  yy_destructor(70,&yymsp[-6].minor);
  yy_destructor(68,&yymsp[-4].minor);
  yy_destructor(69,&yymsp[-2].minor);
}
// 3490 "parser.c"
        break;
      case 131:
// 1403 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_let_assignment("variable-append", yymsp[-1].minor.yy159, yymsp[-4].minor.yy0, NULL, NULL, yymsp[0].minor.yy159, status->scanner_state);
  yy_destructor(68,&yymsp[-3].minor);
  yy_destructor(69,&yymsp[-2].minor);
}
// 3499 "parser.c"
        break;
      case 132:
// 1408 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_let_assignment("array-index", yymsp[-1].minor.yy159, yymsp[-5].minor.yy0, NULL, yymsp[-3].minor.yy159, yymsp[0].minor.yy159, status->scanner_state);
  yy_destructor(68,&yymsp[-4].minor);
  yy_destructor(69,&yymsp[-2].minor);
}
// 3508 "parser.c"
        break;
      case 133:
// 1413 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_let_assignment("incr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(71,&yymsp[0].minor);
}
// 3516 "parser.c"
        break;
      case 134:
// 1418 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_let_assignment("decr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(72,&yymsp[0].minor);
}
// 3524 "parser.c"
        break;
      case 136:
// 1426 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_echo_statement(yymsp[-1].minor.yy159, status->scanner_state);
  yy_destructor(73,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3533 "parser.c"
        break;
      case 139:
// 1438 "parser.lemon"
{
	yygotominor.yy159 = yymsp[0].minor.yy159;;
}
// 3540 "parser.c"
        break;
      case 140:
// 1443 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_mcall_statement(yymsp[-1].minor.yy159, status->scanner_state);
  yy_destructor(28,&yymsp[0].minor);
}
// 3548 "parser.c"
        break;
      case 141:
// 1448 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_scall_statement(yymsp[-1].minor.yy159, status->scanner_state);
  yy_destructor(28,&yymsp[0].minor);
}
// 3556 "parser.c"
        break;
      case 142:
// 1453 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_return_statement(yymsp[-1].minor.yy159, status->scanner_state);
  yy_destructor(74,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3565 "parser.c"
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
// 3579 "parser.c"
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
// 3594 "parser.c"
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
// 3609 "parser.c"
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
// 3624 "parser.c"
        break;
      case 147:
// 1494 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_throw_exception(yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(78,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3633 "parser.c"
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
// 3645 "parser.c"
        break;
      case 149:
// 1503 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_declare_statement(XX_T_TYPE_INTEGER, yymsp[-1].minor.yy159, status->scanner_state);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3654 "parser.c"
        break;
      case 150:
// 1507 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_declare_statement(XX_T_TYPE_UINTEGER, yymsp[-1].minor.yy159, status->scanner_state);
  yy_destructor(79,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3663 "parser.c"
        break;
      case 151:
// 1511 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_declare_statement(XX_T_TYPE_CHAR, yymsp[-1].minor.yy159, status->scanner_state);
  yy_destructor(80,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3672 "parser.c"
        break;
      case 152:
// 1515 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_declare_statement(XX_T_TYPE_UCHAR, yymsp[-1].minor.yy159, status->scanner_state);
  yy_destructor(81,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3681 "parser.c"
        break;
      case 153:
// 1519 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_declare_statement(XX_T_TYPE_LONG, yymsp[-1].minor.yy159, status->scanner_state);
  yy_destructor(82,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3690 "parser.c"
        break;
      case 154:
// 1523 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_declare_statement(XX_T_TYPE_ULONG, yymsp[-1].minor.yy159, status->scanner_state);
  yy_destructor(83,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3699 "parser.c"
        break;
      case 155:
// 1527 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_declare_statement(XX_T_TYPE_DOUBLE, yymsp[-1].minor.yy159, status->scanner_state);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3708 "parser.c"
        break;
      case 156:
// 1531 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_declare_statement(XX_T_TYPE_STRING, yymsp[-1].minor.yy159, status->scanner_state);
  yy_destructor(45,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3717 "parser.c"
        break;
      case 157:
// 1535 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_declare_statement(XX_T_TYPE_BOOL, yymsp[-1].minor.yy159, status->scanner_state);
  yy_destructor(44,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3726 "parser.c"
        break;
      case 158:
// 1539 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_declare_statement(XX_T_TYPE_VAR, yymsp[-1].minor.yy159, status->scanner_state);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3735 "parser.c"
        break;
      case 161:
// 1551 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_declare_variable(yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3742 "parser.c"
        break;
      case 162:
// 1555 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_declare_variable(yymsp[-2].minor.yy0, yymsp[0].minor.yy159, status->scanner_state);
  yy_destructor(35,&yymsp[-1].minor);
}
// 3750 "parser.c"
        break;
      case 164:
// 1563 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("not", yymsp[0].minor.yy159, NULL, NULL, status->scanner_state);
  yy_destructor(24,&yymsp[-1].minor);
}
// 3758 "parser.c"
        break;
      case 165:
// 1567 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("isset", yymsp[0].minor.yy159, NULL, NULL, status->scanner_state);
  yy_destructor(19,&yymsp[-1].minor);
}
// 3766 "parser.c"
        break;
      case 166:
// 1571 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("equals", yymsp[-2].minor.yy159, yymsp[0].minor.yy159, NULL, status->scanner_state);
  yy_destructor(8,&yymsp[-1].minor);
}
// 3774 "parser.c"
        break;
      case 167:
// 1575 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("not-equals", yymsp[-2].minor.yy159, yymsp[0].minor.yy159, NULL, status->scanner_state);
  yy_destructor(15,&yymsp[-1].minor);
}
// 3782 "parser.c"
        break;
      case 168:
// 1579 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("identical", yymsp[-2].minor.yy159, yymsp[0].minor.yy159, NULL, status->scanner_state);
  yy_destructor(9,&yymsp[-1].minor);
}
// 3790 "parser.c"
        break;
      case 169:
// 1583 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("not-identical", yymsp[-2].minor.yy159, yymsp[0].minor.yy159, NULL, status->scanner_state);
  yy_destructor(14,&yymsp[-1].minor);
}
// 3798 "parser.c"
        break;
      case 170:
// 1587 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("less", yymsp[-2].minor.yy159, yymsp[0].minor.yy159, NULL, status->scanner_state);
  yy_destructor(10,&yymsp[-1].minor);
}
// 3806 "parser.c"
        break;
      case 171:
// 1591 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("greater", yymsp[-2].minor.yy159, yymsp[0].minor.yy159, NULL, status->scanner_state);
  yy_destructor(11,&yymsp[-1].minor);
}
// 3814 "parser.c"
        break;
      case 172:
// 1595 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("less-equal", yymsp[-2].minor.yy159, yymsp[0].minor.yy159, NULL, status->scanner_state);
  yy_destructor(12,&yymsp[-1].minor);
}
// 3822 "parser.c"
        break;
      case 173:
// 1599 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("greater-equal", yymsp[-2].minor.yy159, yymsp[0].minor.yy159, NULL, status->scanner_state);
  yy_destructor(13,&yymsp[-1].minor);
}
// 3830 "parser.c"
        break;
      case 174:
// 1603 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("list", yymsp[-1].minor.yy159, NULL, NULL, status->scanner_state);
  yy_destructor(39,&yymsp[-2].minor);
  yy_destructor(25,&yymsp[0].minor);
}
// 3839 "parser.c"
        break;
      case 175:
// 1607 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("cast", yymsp[-2].minor.yy159, yymsp[0].minor.yy159, NULL, status->scanner_state);
  yy_destructor(39,&yymsp[-3].minor);
  yy_destructor(25,&yymsp[-1].minor);
}
// 3848 "parser.c"
        break;
      case 176:
// 1611 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(67,&yymsp[-1].minor);
}
// 3856 "parser.c"
        break;
      case 177:
// 1615 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("static-property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(70,&yymsp[-1].minor);
}
// 3864 "parser.c"
        break;
      case 178:
// 1619 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("static-constant-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(70,&yymsp[-1].minor);
}
// 3872 "parser.c"
        break;
      case 179:
      case 230:
// 1623 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(68,&yymsp[-2].minor);
  yy_destructor(69,&yymsp[0].minor);
}
// 3882 "parser.c"
        break;
      case 180:
      case 231:
// 1628 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_INTEGER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(68,&yymsp[-2].minor);
  yy_destructor(69,&yymsp[0].minor);
}
// 3892 "parser.c"
        break;
      case 181:
      case 232:
// 1633 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_STRING, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(68,&yymsp[-2].minor);
  yy_destructor(69,&yymsp[0].minor);
}
// 3902 "parser.c"
        break;
      case 182:
// 1638 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("add", yymsp[-2].minor.yy159, yymsp[0].minor.yy159, NULL, status->scanner_state);
  yy_destructor(16,&yymsp[-1].minor);
}
// 3910 "parser.c"
        break;
      case 183:
// 1643 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("sub", yymsp[-2].minor.yy159, yymsp[0].minor.yy159, NULL, status->scanner_state);
  yy_destructor(17,&yymsp[-1].minor);
}
// 3918 "parser.c"
        break;
      case 184:
// 1648 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("concat", yymsp[-2].minor.yy159, yymsp[0].minor.yy159, NULL, status->scanner_state);
  yy_destructor(18,&yymsp[-1].minor);
}
// 3926 "parser.c"
        break;
      case 185:
// 1653 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("and", yymsp[-2].minor.yy159, yymsp[0].minor.yy159, NULL, status->scanner_state);
  yy_destructor(6,&yymsp[-1].minor);
}
// 3934 "parser.c"
        break;
      case 186:
// 1658 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("or", yymsp[-2].minor.yy159, yymsp[0].minor.yy159, NULL, status->scanner_state);
  yy_destructor(7,&yymsp[-1].minor);
}
// 3942 "parser.c"
        break;
      case 187:
// 1663 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("instanceof", yymsp[0].minor.yy159, NULL, NULL, status->scanner_state);
  yy_destructor(22,&yymsp[-1].minor);
}
// 3950 "parser.c"
        break;
      case 188:
// 1668 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("fetch", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy159, NULL, status->scanner_state);
  yy_destructor(20,&yymsp[-3].minor);
  yy_destructor(5,&yymsp[-1].minor);
}
// 3959 "parser.c"
        break;
      case 189:
// 1673 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("typeof", yymsp[0].minor.yy159, NULL, NULL, status->scanner_state);
  yy_destructor(21,&yymsp[-1].minor);
}
// 3967 "parser.c"
        break;
      case 190:
// 1678 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_literal(XX_T_CONSTANT, yymsp[0].minor.yy0, status->scanner_state);
}
// 3974 "parser.c"
        break;
      case 191:
      case 219:
      case 223:
// 1683 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state);
}
// 3983 "parser.c"
        break;
      case 192:
      case 221:
      case 224:
// 1688 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_literal(XX_T_INTEGER, yymsp[0].minor.yy0, status->scanner_state);
}
// 3992 "parser.c"
        break;
      case 193:
      case 220:
      case 225:
// 1693 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_literal(XX_T_STRING, yymsp[0].minor.yy0, status->scanner_state);
}
// 4001 "parser.c"
        break;
      case 194:
// 1698 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_literal(XX_T_SCHAR, yymsp[0].minor.yy0, status->scanner_state);
}
// 4008 "parser.c"
        break;
      case 195:
      case 226:
// 1703 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_literal(XX_T_DOUBLE, yymsp[0].minor.yy0, status->scanner_state);
}
// 4016 "parser.c"
        break;
      case 196:
      case 227:
// 1708 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_literal(XX_T_NULL, NULL, status->scanner_state);
  yy_destructor(86,&yymsp[0].minor);
}
// 4025 "parser.c"
        break;
      case 197:
      case 229:
// 1713 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_literal(XX_T_TRUE, NULL, status->scanner_state);
  yy_destructor(87,&yymsp[0].minor);
}
// 4034 "parser.c"
        break;
      case 198:
      case 228:
// 1718 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_literal(XX_T_FALSE, NULL, status->scanner_state);
  yy_destructor(88,&yymsp[0].minor);
}
// 4043 "parser.c"
        break;
      case 199:
// 1723 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("empty-array", NULL, NULL, NULL, status->scanner_state);
  yy_destructor(68,&yymsp[-1].minor);
  yy_destructor(69,&yymsp[0].minor);
}
// 4052 "parser.c"
        break;
      case 200:
// 1728 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_expr("array", yymsp[-1].minor.yy159, NULL, NULL, status->scanner_state);
  yy_destructor(68,&yymsp[-2].minor);
  yy_destructor(69,&yymsp[0].minor);
}
// 4061 "parser.c"
        break;
      case 201:
// 1733 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_new_instance(yymsp[0].minor.yy0, NULL, status->scanner_state);
  yy_destructor(23,&yymsp[-1].minor);
}
// 4069 "parser.c"
        break;
      case 202:
// 1738 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_new_instance(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(23,&yymsp[-3].minor);
  yy_destructor(39,&yymsp[-1].minor);
  yy_destructor(25,&yymsp[0].minor);
}
// 4079 "parser.c"
        break;
      case 203:
// 1743 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_new_instance(yymsp[-3].minor.yy0, yymsp[-1].minor.yy159, status->scanner_state);
  yy_destructor(23,&yymsp[-4].minor);
  yy_destructor(39,&yymsp[-2].minor);
  yy_destructor(25,&yymsp[0].minor);
}
// 4089 "parser.c"
        break;
      case 204:
// 1748 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_fcall(yymsp[-3].minor.yy0, yymsp[-1].minor.yy159, status->scanner_state);
  yy_destructor(39,&yymsp[-2].minor);
  yy_destructor(25,&yymsp[0].minor);
}
// 4098 "parser.c"
        break;
      case 205:
// 1753 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_fcall(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(39,&yymsp[-1].minor);
  yy_destructor(25,&yymsp[0].minor);
}
// 4107 "parser.c"
        break;
      case 208:
// 1768 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_scall(yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy159, status->scanner_state);
  yy_destructor(70,&yymsp[-4].minor);
  yy_destructor(39,&yymsp[-2].minor);
  yy_destructor(25,&yymsp[0].minor);
}
// 4117 "parser.c"
        break;
      case 209:
// 1773 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_scall(yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(70,&yymsp[-3].minor);
  yy_destructor(39,&yymsp[-1].minor);
  yy_destructor(25,&yymsp[0].minor);
}
// 4127 "parser.c"
        break;
      case 210:
// 1778 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_mcall(yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy159, status->scanner_state);
  yy_destructor(67,&yymsp[-4].minor);
  yy_destructor(39,&yymsp[-2].minor);
  yy_destructor(25,&yymsp[0].minor);
}
// 4137 "parser.c"
        break;
      case 211:
// 1783 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_mcall(yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(67,&yymsp[-3].minor);
  yy_destructor(39,&yymsp[-1].minor);
  yy_destructor(25,&yymsp[0].minor);
}
// 4147 "parser.c"
        break;
      case 217:
// 1807 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_array_item(yymsp[-2].minor.yy159, yymsp[0].minor.yy159, status->scanner_state);
  yy_destructor(54,&yymsp[-1].minor);
}
// 4155 "parser.c"
        break;
      case 218:
// 1811 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_array_item(NULL, yymsp[0].minor.yy159, status->scanner_state);
}
// 4162 "parser.c"
        break;
      case 234:
// 1878 "parser.lemon"
{
	yygotominor.yy159 = xx_ret_comment(yymsp[0].minor.yy0, status->scanner_state);
}
// 4169 "parser.c"
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

// 4245 "parser.c"
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
