/* Driver template for the LEMON parser generator.
** The author disclaims copyright to this source code.
*/
/* First off, code is include which follows the "include" declaration
** in the input file. */
#include <stdio.h>
// 45 "parser.lemon"


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
		case XX_T_CHAR:
			json_object_object_add(ret, "type", json_object_new_string("char"));
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

static json_object *xx_ret_do_while_statement(json_object *expr, json_object *statements, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("do-while"));
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
	if (expr) {
		json_object_object_add(ret, "expr", expr);
	}

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


// 767 "parser.c"
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
#define YYNOCODE 157
#define YYACTIONTYPE unsigned short int
#define xx_TOKENTYPE xx_parser_token*
typedef union {
  xx_TOKENTYPE yy0;
  json_object* yy103;
  int yy313;
} YYMINORTYPE;
#define YYSTACKDEPTH 100
#define xx_ARG_SDECL xx_parser_status *status;
#define xx_ARG_PDECL ,xx_parser_status *status
#define xx_ARG_FETCH xx_parser_status *status = yypParser->status
#define xx_ARG_STORE yypParser->status = status
#define YYNSTATE 499
#define YYNRULE 240
#define YYERRORSYMBOL 94
#define YYERRSYMDT yy313
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
 /*     0 */    89,  154,  158,  152,  190,   86,  294,  297,  147,  429,
 /*    10 */   442,   17,  457,  465,    7,  471,  445,   10,  160,  441,
 /*    20 */   130,  485,  357,  131,  132,  133,  134,  135,  136,  137,
 /*    30 */   138,  139,  140,  141,   11,  126,  128,   88,  102,  106,
 /*    40 */   108,  110,  112,  104,  100,  114,  116,  124,  118,  120,
 /*    50 */   122,  474,  457,  465,  562,  168,  445,  177,  352,  441,
 /*    60 */   290,  205,  381,  161,  162,  357,  163,  164,  165,  166,
 /*    70 */   167,  210,  172,  316,  325,  334,  337,  328,  331,  340,
 /*    80 */   346,  343,  151,  349,   46,   48,   50,  280,   59,  431,
 /*    90 */   197,  198,   70,   74,   79,  219,  355,  199,  226,  353,
 /*   100 */   170,  352,  189,  174,  185,   82,  414,  196,  357,  500,
 /*   110 */   277,  288,  292,  279,  319,  307,  316,  325,  334,  337,
 /*   120 */   328,  331,  340,  346,  343,  282,  349,   46,   48,   50,
 /*   130 */   202,   59,  281,  197,  198,   70,   74,   79,  219,    7,
 /*   140 */   318,  226,   10,  413,  352,  171,  485,  357,   73,  317,
 /*   150 */   324,  357,  196,  277,  288,  292,   62,  231,  307,  316,
 /*   160 */   325,  334,  337,  328,  331,  340,  346,  343,  253,  349,
 /*   170 */    46,   48,   50,  249,   59,  314,  197,  198,   70,   74,
 /*   180 */    79,  219,  410,   66,  226,  382,  177,  352,  118,  120,
 /*   190 */   122,   54,  326,  324,  357,  177,  277,  288,  292,   64,
 /*   200 */   411,  307,  316,  325,  334,  337,  328,  331,  340,  346,
 /*   210 */   343,  253,  349,   46,   48,   50,  262,   59,  409,  197,
 /*   220 */   198,   70,   74,   79,  219,  310,   66,  226,  382,  308,
 /*   230 */   352,  173,  174,  185,  438,  319,  461,  357,  243,  277,
 /*   240 */   288,  292,  176,  462,  307,  316,  325,  334,  337,  328,
 /*   250 */   331,  340,  346,  343,  253,  349,   46,   48,   50,  270,
 /*   260 */    59,  327,  197,  198,   70,   74,   79,  219,  329,  324,
 /*   270 */   226,  393,  406,  352,  415,  404,  408,  440,  332,  324,
 /*   280 */   357,  242,  277,  288,  292,  319,  319,  307,  316,  325,
 /*   290 */   334,  337,  328,  331,  340,  346,  343,  364,  349,   46,
 /*   300 */    48,   50,  319,   59,  400,  404,  408,   70,   74,   79,
 /*   310 */   219,  333,  336,  226,  435,  407,  352,  415,  404,  408,
 /*   320 */   396,  335,  324,  357,  196,  277,  288,  292,  330,  496,
 /*   330 */   307,  316,  325,  334,  337,  328,  331,  340,  346,  343,
 /*   340 */    76,  349,   46,   48,   50,  319,   59,  195,  437,  111,
 /*   350 */    70,   74,   79,  219,  338,  324,  226,  197,  198,  352,
 /*   360 */   221,  144,  405,  363,  341,  324,  357,  399,  277,  288,
 /*   370 */   292,  339,  319,  307,  316,  325,  334,  337,  328,  331,
 /*   380 */   340,  346,  343,  142,  349,   46,   48,   50,  298,   59,
 /*   390 */   436,  197,  198,   70,   74,   79,  219,   19,  342,  226,
 /*   400 */    18,  467,  352,  441,  344,  324,  376,  319,  468,  357,
 /*   410 */   403,  277,  288,  292,  347,  324,  307,  316,  325,  334,
 /*   420 */   337,  328,  331,  340,  346,  343,  153,  349,   46,   48,
 /*   430 */    50,   93,   59,  345,  197,  198,   70,   74,   79,  219,
 /*   440 */   350,  324,  226,  301,  304,  352,   63,  385,  319,  434,
 /*   450 */   319,   12,  357,  479,  277,  288,  292,  227,  276,  307,
 /*   460 */   316,  325,  334,  337,  328,  331,  340,  346,  343,   99,
 /*   470 */   349,   46,   48,   50,  351,   59,  348,  197,  198,   70,
 /*   480 */    74,   79,  219,  426,  459,  226,   95,   97,  352,  441,
 /*   490 */   441,  402,  392,  229,  196,  357,  220,  277,  288,  292,
 /*   500 */   395,   65,  307,  316,  325,  334,  337,  328,  331,  340,
 /*   510 */   346,  343,  105,  349,   46,   48,   50,  208,   59,  228,
 /*   520 */   197,  198,   70,   74,   79,  219,  389,  488,  226,  492,
 /*   530 */     8,  352,  388,  383,  379,  370,  366,  377,  357,  206,
 /*   540 */   277,  288,  292,    9,  365,  307,  316,  325,  334,  337,
 /*   550 */   328,  331,  340,  346,  343,  367,  349,   46,   48,   50,
 /*   560 */   247,   59,   61,  197,  198,   70,   74,   79,  219,  150,
 /*   570 */   271,  226,  448,  356,  352,  449,  354,  450,  225,   14,
 /*   580 */   451,  357,  321,  277,  288,  292,  250,  453,  307,  316,
 /*   590 */   325,  334,  337,  328,  331,  340,  346,  343,   87,  349,
 /*   600 */    46,   48,   50,  454,   59,  455,  197,  198,   70,   74,
 /*   610 */    79,  219,  456,  251,  226,  192,  145,  352,  725,  191,
 /*   620 */    91,   27,  416,   47,  357,  724,  277,  288,  292,  255,
 /*   630 */   723,  307,  316,  325,  334,  337,  328,  331,  340,  346,
 /*   640 */   343,  113,  349,   46,   48,   50,  463,   59,  464,  197,
 /*   650 */   198,   70,   74,   79,  219,  256,  272,  226,  323,  322,
 /*   660 */   352,  156,  469,  320,  391,   56,  470,  357,  315,  277,
 /*   670 */   288,  292,  260,   55,  307,  316,  325,  334,  337,  328,
 /*   680 */   331,  340,  346,  343,  127,  349,   46,   48,   50,  312,
 /*   690 */    59,   98,  197,  198,   70,   74,   79,  219,  311,   24,
 /*   700 */   226,  309,  175,  352,  305,  306,   83,   58,   52,   96,
 /*   710 */   357,  303,  277,  288,  292,  480,  302,  307,  316,  325,
 /*   720 */   334,  337,  328,  331,  340,  346,  343,  109,  349,   46,
 /*   730 */    48,   50,   94,   59,   80,  197,  198,   70,   74,   79,
 /*   740 */   219,  481,  263,  226,  300,  299,  352,  149,  483,   92,
 /*   750 */    25,  264,  296,  357,  295,  277,  288,  292,  486,  487,
 /*   760 */   307,  316,  325,  334,  337,  328,  331,  340,  346,  343,
 /*   770 */   119,  349,   46,   48,   50,  155,   59,   90,  197,  198,
 /*   780 */    70,   74,   79,  219,  293,  490,  226,   49,   22,  352,
 /*   790 */   268,  287,  499,  375,  493,   21,  357,   85,  277,  288,
 /*   800 */   292,  494,  285,  307,  316,  325,  334,  337,  328,  331,
 /*   810 */   340,  346,  343,  222,  349,   46,   48,   50,   71,   59,
 /*   820 */   238,  197,  198,   70,   74,   79,  219,  157,  428,  226,
 /*   830 */    67,   68,  352,  230,  398,  398,  387,  398,  398,  357,
 /*   840 */   398,  277,  288,  292,  398,  398,  307,  316,  325,  334,
 /*   850 */   337,  328,  331,  340,  346,  343,  107,  349,   46,   48,
 /*   860 */    50,  398,   59,  398,  197,  198,   70,   74,   79,  219,
 /*   870 */   398,  398,  226,  398,  398,  352,  398,  398,  398,   78,
 /*   880 */   398,  398,  357,  398,  277,  288,  292,  398,  398,  307,
 /*   890 */   316,  325,  334,  337,  328,  331,  340,  346,  343,  146,
 /*   900 */   349,   46,   48,   50,  398,   59,  398,  197,  198,   70,
 /*   910 */    74,   79,  219,  398,  398,  226,  398,  398,  352,  398,
 /*   920 */   398,  398,  380,  398,  398,  357,  398,  277,  288,  292,
 /*   930 */   398,  398,  307,  316,  325,  334,  337,  328,  331,  340,
 /*   940 */   346,  343,  360,  349,   46,   48,   50,  398,   59,  398,
 /*   950 */   197,  198,   70,   74,   79,  219,  398,  398,  226,  398,
 /*   960 */   398,  352,  398,  398,  398,  432,  398,  398,  357,  398,
 /*   970 */   277,  288,  292,  398,  398,  307,  316,  325,  334,  337,
 /*   980 */   328,  331,  340,  346,  343,  115,  349,   46,   48,   50,
 /*   990 */   398,   59,  398,  197,  198,   70,   74,   79,  219,  398,
 /*  1000 */   398,  226,  398,  398,  352,  398,  398,  398,  398,  398,
 /*  1010 */   398,  357,  398,  277,  288,  292,  398,  398,  307,  316,
 /*  1020 */   325,  334,  337,  328,  331,  340,  346,  343,  117,  349,
 /*  1030 */    46,   48,   50,  398,   59,  398,  197,  198,   70,   74,
 /*  1040 */    79,  219,  398,  398,  226,  398,  398,  352,  398,  398,
 /*  1050 */   398,  386,  398,  398,  357,  398,  277,  288,  292,  398,
 /*  1060 */   398,  307,  316,  325,  334,  337,  328,  331,  340,  346,
 /*  1070 */   343,  129,  349,   46,   48,   50,  398,   59,  398,  197,
 /*  1080 */   198,   70,   74,   79,  219,  398,  398,  226,  398,  398,
 /*  1090 */   352,  196,  417,  418,  420,  419,  421,  357,  399,  277,
 /*  1100 */   288,  292,  398,  398,  307,  316,  325,  334,  337,  328,
 /*  1110 */   331,  340,  346,  343,  216,  349,   46,   48,   50,  398,
 /*  1120 */    59,  394,  466,  159,   70,   74,   79,  219,  423,  398,
 /*  1130 */   226,  197,  198,   20,   45,  422,  424,  232,    3,    4,
 /*  1140 */     5,    6,  277,  288,  292,   77,  398,  307,  398,  398,
 /*  1150 */   358,   29,   30,   31,   32,   33,   34,   35,   36,   37,
 /*  1160 */    38,   39,   40,   41,   42,   43,   44,  398,  398,   45,
 /*  1170 */   233,  234,  235,  236,  237,  123,  258,  398,  284,  286,
 /*  1180 */   369,  398,  398,  197,  198,  358,   29,   30,   31,   32,
 /*  1190 */    33,   34,   35,   36,   37,   38,   39,   40,   41,   42,
 /*  1200 */    43,   44,  398,  398,  398,  473,   45,  476,  477,  465,
 /*  1210 */   289,  444,  445,  284,  286,  441,  398,  374,  197,  198,
 /*  1220 */   398,  398,  358,   29,   30,   31,   32,   33,   34,   35,
 /*  1230 */    36,   37,   38,   39,   40,   41,   42,   43,   44,  398,
 /*  1240 */   398,  179,  180,  398,   45,  181,  182,  184,  183,  398,
 /*  1250 */   284,  286,  398,  398,  398,   26,  398,  398,  186,  398,
 /*  1260 */   358,   29,   30,   31,   32,   33,   34,   35,   36,   37,
 /*  1270 */    38,   39,   40,   41,   42,   43,   44,  398,  398,  398,
 /*  1280 */    45,  740,    1,    2,  498,    4,    5,    6,  284,  286,
 /*  1290 */   398,   57,  398,  398,  178,  398,  358,   29,   30,   31,
 /*  1300 */    32,   33,   34,   35,   36,   37,   38,   39,   40,   41,
 /*  1310 */    42,   43,   44,  188,  187,   45,  398,  181,  182,  184,
 /*  1320 */   183,  121,  398,  398,  284,  286,  362,  214,  398,  197,
 /*  1330 */   198,  358,   29,   30,   31,   32,   33,   34,   35,   36,
 /*  1340 */    37,   38,   39,   40,   41,   42,   43,   44,  398,  179,
 /*  1350 */   180,  398,   45,  181,  182,  184,  183,  101,  232,  284,
 /*  1360 */   286,  148,  211,   69,  204,  197,  198,  398,  358,   29,
 /*  1370 */    30,   31,   32,   33,   34,   35,   36,   37,   38,   39,
 /*  1380 */    40,   41,   42,   43,   44,  398,  398,  398,  398,  398,
 /*  1390 */    45,  233,  234,  235,  236,  237,  284,  286,  398,  398,
 /*  1400 */   398,   72,  398,  398,  398,  398,  358,   29,   30,   31,
 /*  1410 */    32,   33,   34,   35,   36,   37,   38,   39,   40,   41,
 /*  1420 */    42,   43,   44,  151,  398,  398,   45,  398,  398,  398,
 /*  1430 */   232,  197,  198,  215,  284,  286,  398,  397,  200,  398,
 /*  1440 */   398,  398,  358,   29,   30,   31,   32,   33,   34,   35,
 /*  1450 */    36,   37,   38,   39,   40,   41,   42,   43,   44,  398,
 /*  1460 */   398,   45,  398,  233,  234,  235,  236,  237,  398,  245,
 /*  1470 */   284,  286,  433,  398,  398,  398,  398,  358,   29,   30,
 /*  1480 */    31,   32,   33,   34,   35,   36,   37,   38,   39,   40,
 /*  1490 */    41,   42,   43,   44,  398,  398,  398,  398,   45,  114,
 /*  1500 */   116,  124,  118,  120,  122,  284,  286,  398,  398,   81,
 /*  1510 */   398,  398,  398,  398,  358,   29,   30,   31,   32,   33,
 /*  1520 */    34,   35,   36,   37,   38,   39,   40,   41,   42,   43,
 /*  1530 */    44,  151,  398,  398,  398,  398,   45,  398,  398,  197,
 /*  1540 */   198,  194,  284,  286,  398,  398,  200,  390,  398,  398,
 /*  1550 */   398,  398,  358,   29,   30,   31,   32,   33,   34,   35,
 /*  1560 */    36,   37,   38,   39,   40,   41,   42,   43,   44,  151,
 /*  1570 */   398,  398,   45,  398,  398,  398,  398,  197,  198,  207,
 /*  1580 */   284,  286,  398,  439,  200,  398,  398,  398,  358,   29,
 /*  1590 */    30,   31,   32,   33,   34,   35,   36,   37,   38,   39,
 /*  1600 */    40,   41,   42,   43,   44,  151,  398,   45,  398,  398,
 /*  1610 */   398,  398,  398,  197,  198,  201,  284,  286,  224,  398,
 /*  1620 */   200,  398,  398,  358,   29,   30,   31,   32,   33,   34,
 /*  1630 */    35,   36,   37,   38,   39,   40,   41,   42,   43,   44,
 /*  1640 */   282,  398,  398,  398,   45,  398,  278,  283,  197,  198,
 /*  1650 */   398,  284,  286,  398,  398,  384,  398,  398,  398,  398,
 /*  1660 */   358,   29,   30,   31,   32,   33,   34,   35,   36,   37,
 /*  1670 */    38,   39,   40,   41,   42,   43,   44,  151,  398,  398,
 /*  1680 */   398,  398,   45,  398,  398,  197,  198,  313,  284,  286,
 /*  1690 */   398,  398,  200,   53,  398,  398,  398,  398,  358,   29,
 /*  1700 */    30,   31,   32,   33,   34,   35,   36,   37,   38,   39,
 /*  1710 */    40,   41,   42,   43,   44,  398,  398,  398,   45,  398,
 /*  1720 */   443,  212,   19,  398,  444,  445,  284,  286,  441,  197,
 /*  1730 */   198,  398,  398,  398,   28,   29,   30,   31,   32,   33,
 /*  1740 */    34,   35,   36,   37,   38,   39,   40,   41,   42,   43,
 /*  1750 */    44,   89,  154,  158,  152,  190,   86,  398,  398,  147,
 /*  1760 */   398,  398,  284,  286,  398,  398,  232,  398,  398,  160,
 /*  1770 */   398,  130,   89,  154,  158,  152,  190,   86,  240,  398,
 /*  1780 */   147,  398,  269,  398,  398,  240,  197,  198,  398,  252,
 /*  1790 */   160,  398,  130,  197,  198,  398,  398,  398,  371,  233,
 /*  1800 */   234,  235,  236,  237,  241,  266,  168,  254,  274,  275,
 /*  1810 */   398,  398,  398,  398,  161,  162,  398,  163,  164,  165,
 /*  1820 */   166,  167,   84,  372,  398,  218,  398,  168,  259,  398,
 /*  1830 */   398,  197,  198,  197,  198,  161,  162,  398,  163,  164,
 /*  1840 */   165,  166,  167,   89,  154,  158,  152,  190,   86,  240,
 /*  1850 */   398,  147,  398,  248,  398,  398,  398,  197,  198,  398,
 /*  1860 */   398,  160,  398,  130,   89,  154,  158,  152,  190,   86,
 /*  1870 */   217,  398,  147,  398,  398,  398,  398,  398,  398,  398,
 /*  1880 */   398,  398,  160,  398,  130,   89,  154,  158,  152,  190,
 /*  1890 */    86,  209,  240,  147,  398,  398,  257,  398,  168,  267,
 /*  1900 */   197,  198,  398,  160,  398,  130,  161,  162,  398,  163,
 /*  1910 */   164,  165,  166,  167,  240,  398,  240,  398,  273,  168,
 /*  1920 */   261,  398,  197,  198,  197,  198,  398,  161,  162,  398,
 /*  1930 */   163,  164,  165,  166,  167,   60,  103,  398,  218,  398,
 /*  1940 */   168,  398,  398,  398,  197,  198,  197,  198,  161,  162,
 /*  1950 */   398,  163,  164,  165,  166,  167,   89,  154,  158,  152,
 /*  1960 */   190,   86,  193,  125,  147,  398,  417,  418,  420,  419,
 /*  1970 */   421,  197,  198,  398,  160,  398,  130,   89,  154,  158,
 /*  1980 */   152,  190,   86,  203,  240,  147,  398,  398,  244,  398,
 /*  1990 */   398,  398,  197,  198,  398,  160,  398,  130,  398,  398,
 /*  2000 */    15,   75,  423,  458,  218,  452,  398,  398,  398,  422,
 /*  2010 */   424,  168,  197,  198,  398,  398,  398,  398,  398,  161,
 /*  2020 */   162,  398,  163,  164,  165,  166,  167,  378,  398,  398,
 /*  2030 */   218,  398,  168,  398,  398,  398,  398,  398,  197,  198,
 /*  2040 */   161,  162,  398,  163,  164,  165,  166,  167,   89,  154,
 /*  2050 */   158,  152,  190,   86,  398,  398,  147,  398,  417,  418,
 /*  2060 */   420,  419,  421,  398,  398,  398,  160,  398,  130,   89,
 /*  2070 */   154,  158,  152,  190,   86,  398,  398,  147,  291,  398,
 /*  2080 */   398,   51,  398,  398,  218,  398,  398,  160,  460,  130,
 /*  2090 */   398,  398,  197,  198,  423,  359,  240,  398,  398,  427,
 /*  2100 */   265,  422,  424,  168,  197,  198,  398,  398,  398,  398,
 /*  2110 */   398,  161,  162,  398,  163,  164,  165,  166,  167,  240,
 /*  2120 */   398,  398,  398,  239,  168,  398,  398,  197,  198,  398,
 /*  2130 */   398,  398,  161,  162,  398,  163,  164,  165,  166,  167,
 /*  2140 */    89,  154,  158,  152,  190,   86,  398,  412,  147,  398,
 /*  2150 */   398,  398,  398,  398,  398,  398,  398,  398,  160,  398,
 /*  2160 */   130,   89,  154,  158,  152,  190,   86,  401,  398,  147,
 /*  2170 */   398,  398,  398,  398,  398,  398,  398,  398,  398,  160,
 /*  2180 */   398,  130,  131,  132,  133,  134,  135,  136,  137,  138,
 /*  2190 */   139,  140,  141,  398,  398,  168,  246,  398,  398,  398,
 /*  2200 */   398,  398,  398,  161,  162,  398,  163,  164,  165,  166,
 /*  2210 */   167,  398,  398,  398,  398,  398,  168,  398,  417,  418,
 /*  2220 */   420,  419,  421,  398,  161,  162,  398,  163,  164,  165,
 /*  2230 */   166,  167,  126,  128,   88,  102,  106,  108,  110,  112,
 /*  2240 */   104,  100,  114,  116,  124,  118,  120,  122,  398,  398,
 /*  2250 */   398,  398,  516,  398,  423,  458,  398,  452,  398,  223,
 /*  2260 */   398,  422,  424,  398,  398,  398,  398,  126,  128,   88,
 /*  2270 */   102,  106,  108,  110,  112,  104,  100,  114,  116,  124,
 /*  2280 */   118,  120,  122,  398,  398,  398,  398,  398,  398,  143,
 /*  2290 */   126,  128,   88,  102,  106,  108,  110,  112,  104,  100,
 /*  2300 */   114,  116,  124,  118,  120,  122,  126,  128,   88,  102,
 /*  2310 */   106,  108,  110,  112,  104,  100,  114,  116,  124,  118,
 /*  2320 */   120,  122,  398,  417,  418,  420,  419,  421,  398,  398,
 /*  2330 */   398,  398,  398,  373,  398,  398,  398,  398,  398,  398,
 /*  2340 */   398,  126,  128,   88,  102,  106,  108,  110,  112,  104,
 /*  2350 */   100,  114,  116,  124,  118,  120,  122,  518,  398,  423,
 /*  2360 */   425,  398,  213,  398,  398,  398,  422,  424,  361,  398,
 /*  2370 */   126,  128,   88,  102,  106,  108,  110,  112,  104,  100,
 /*  2380 */   114,  116,  124,  118,  120,  122,  126,  128,   88,  102,
 /*  2390 */   106,  108,  110,  112,  104,  100,  114,  116,  124,  118,
 /*  2400 */   120,  122,  398,  495,   16,  472,  478,  477,  465,  398,
 /*  2410 */   471,  445,  398,  368,  441,  398,  417,  418,  420,  419,
 /*  2420 */   421,  126,  128,   88,  102,  106,  108,  110,  112,  104,
 /*  2430 */   100,  114,  116,  124,  118,  120,  122,  412,  398,  398,
 /*  2440 */   398,  398,  637,  398,  398,  398,  398,  398,  412,  398,
 /*  2450 */   398,  398,  423,  398,  398,   23,  398,  401,  398,  422,
 /*  2460 */   424,  398,  398,  398,  398,  398,  430,  398,  401,  398,
 /*  2470 */   398,  398,  131,  132,  133,  134,  135,  136,  137,  138,
 /*  2480 */   139,  140,  141,  131,  132,  133,  134,  135,  136,  137,
 /*  2490 */   138,  139,  140,  141,   88,  102,  106,  108,  110,  112,
 /*  2500 */   104,  100,  114,  116,  124,  118,  120,  122,  417,  418,
 /*  2510 */   420,  419,  421,  489,   16,  472,  478,  477,  465,  398,
 /*  2520 */   471,  445,  398,  398,  441,  482,   16,  472,  478,  477,
 /*  2530 */   465,  398,  471,  445,  398,  398,  441,  398,  398,  398,
 /*  2540 */   398,  398,  484,  398,  423,  458,  398,  452,  398,  398,
 /*  2550 */   398,  422,  424,  417,  418,  420,  419,  421,   13,   16,
 /*  2560 */   472,  478,  477,  465,  398,  471,  445,  398,  398,  441,
 /*  2570 */   398,  417,  418,  420,  419,  421,  398,  417,  418,  420,
 /*  2580 */   419,  421,  398,  398,  398,  398,  398,  497,  398,  423,
 /*  2590 */   458,  398,  452,  398,  398,  398,  422,  424,  417,  418,
 /*  2600 */   420,  419,  421,  398,  398,  398,  398,  423,  398,  398,
 /*  2610 */   398,  398,   20,  423,  422,  424,  398,  398,  427,  398,
 /*  2620 */   422,  424,  417,  418,  420,  419,  421,  398,  398,  398,
 /*  2630 */   398,  398,  491,  398,  423,  458,  398,  452,  398,  398,
 /*  2640 */   398,  422,  424,  417,  418,  420,  419,  421,  398,  398,
 /*  2650 */   186,  398,  398,  398,  398,  398,  398,  398,  423,  398,
 /*  2660 */   398,  447,  398,  398,  398,  422,  424,  398,  398,  398,
 /*  2670 */   398,  398,  398,  398,  398,  398,  398,  515,  398,  423,
 /*  2680 */   458,  398,  452,  398,  398,  398,  422,  424,  417,  418,
 /*  2690 */   420,  419,  421,  398,  398,  398,  398,  398,  169,  398,
 /*  2700 */   398,  398,  398,  398,  398,  188,  187,  398,  398,  181,
 /*  2710 */   182,  184,  183,  398,  398,  398,  398,  398,  398,  398,
 /*  2720 */   398,  398,  519,  398,  423,  446,  398,  452,  398,  398,
 /*  2730 */   398,  422,  424,  417,  418,  420,  419,  421,  417,  418,
 /*  2740 */   420,  419,  421,  398,  398,  398,  398,  398,  398,  398,
 /*  2750 */   398,  398,  398,  398,  398,  398,  398,  398,  398,  398,
 /*  2760 */   398,  398,  398,  398,  398,  398,  398,  517,  398,  423,
 /*  2770 */   425,  398,  521,  398,  423,  425,  422,  424,  398,  398,
 /*  2780 */   398,  422,  424,  417,  418,  420,  419,  421,  417,  418,
 /*  2790 */   420,  419,  421,  398,  398,  398,  398,  398,  398,  398,
 /*  2800 */   398,  398,  398,  398,  398,  398,  398,  398,  398,  398,
 /*  2810 */   398,  398,  398,  398,  398,  398,  398,  523,  398,  423,
 /*  2820 */   425,  398,  520,  398,  423,  475,  422,  424,  398,  398,
 /*  2830 */   398,  422,  424,  417,  418,  420,  419,  421,  398,  398,
 /*  2840 */   398,  398,  398,  398,  398,  398,  398,  398,  398,  398,
 /*  2850 */   398,  398,  398,  398,  398,  398,  398,  398,  398,  398,
 /*  2860 */   398,  398,  398,  398,  398,  398,  398,  522,  398,  423,
 /*  2870 */   425,  398,  398,  398,  398,  398,  422,  424,
};
static YYCODETYPE yy_lookahead[] = {
 /*     0 */    23,   24,   25,   26,   27,   28,   77,   78,   31,   43,
 /*    10 */   104,  105,  106,  107,   30,  109,  110,   33,   41,  113,
 /*    20 */    43,   37,   38,   46,   47,   48,   49,   50,   51,   52,
 /*    30 */    53,   54,   55,   56,   31,    7,    8,    9,   10,   11,
 /*    40 */    12,   13,   14,   15,   16,   17,   18,   19,   20,   21,
 /*    50 */    22,  105,  106,  107,   31,   78,  110,  108,   31,  113,
 /*    60 */    32,   31,   35,   86,   87,   38,   89,   90,   91,   92,
 /*    70 */    93,   41,    6,   46,   47,   48,   49,   50,   51,   52,
 /*    80 */    53,   54,  137,   56,   57,   58,   59,    6,   61,   34,
 /*    90 */   145,  146,   65,   66,   67,   68,   77,  152,   71,   80,
 /*   100 */   151,   31,  153,  154,  155,   35,   12,    6,   38,    0,
 /*   110 */    83,   84,   85,   32,    6,   88,   46,   47,   48,   49,
 /*   120 */    50,   51,   52,   53,   54,  137,   56,   57,   58,   59,
 /*   130 */    29,   61,  144,  145,  146,   65,   66,   67,   68,   30,
 /*   140 */    32,   71,   33,   31,   31,   79,   37,   38,   35,  148,
 /*   150 */   149,   38,    6,   83,   84,   85,   35,   31,   88,   46,
 /*   160 */    47,   48,   49,   50,   51,   52,   53,   54,  137,   56,
 /*   170 */    57,   58,   59,  142,   61,   29,  145,  146,   65,   66,
 /*   180 */    67,   68,   39,   62,   71,   64,  108,   31,   20,   21,
 /*   190 */    22,   35,  148,  149,   38,  108,   83,   84,   85,   35,
 /*   200 */   108,   88,   46,   47,   48,   49,   50,   51,   52,   53,
 /*   210 */    54,  137,   56,   57,   58,   59,  142,   61,   31,  145,
 /*   220 */   146,   65,   66,   67,   68,   27,   62,   71,   64,   31,
 /*   230 */    31,  153,  154,  155,   35,    6,   32,   38,  140,   83,
 /*   240 */    84,   85,  155,   39,   88,   46,   47,   48,   49,   50,
 /*   250 */    51,   52,   53,   54,  137,   56,   57,   58,   59,  142,
 /*   260 */    61,   32,  145,  146,   65,   66,   67,   68,  148,  149,
 /*   270 */    71,  111,   39,   31,  114,  115,  116,   35,  148,  149,
 /*   280 */    38,   31,   83,   84,   85,    6,    6,   88,   46,   47,
 /*   290 */    48,   49,   50,   51,   52,   53,   54,    6,   56,   57,
 /*   300 */    58,   59,    6,   61,  114,  115,  116,   65,   66,   67,
 /*   310 */    68,   32,   32,   71,  111,  108,   31,  114,  115,  116,
 /*   320 */    35,  148,  149,   38,    6,   83,   84,   85,   32,   35,
 /*   330 */    88,   46,   47,   48,   49,   50,   51,   52,   53,   54,
 /*   340 */    34,   56,   57,   58,   59,    6,   61,   29,   34,  137,
 /*   350 */    65,   66,   67,   68,  148,  149,   71,  145,  146,   31,
 /*   360 */    69,  115,   31,   35,  148,  149,   38,    6,   83,   84,
 /*   370 */    85,   32,    6,   88,   46,   47,   48,   49,   50,   51,
 /*   380 */    52,   53,   54,  137,   56,   57,   58,   59,   31,   61,
 /*   390 */    29,  145,  146,   65,   66,   67,   68,  107,   32,   71,
 /*   400 */   110,   32,   31,  113,  148,  149,   35,    6,   39,   38,
 /*   410 */   108,   83,   84,   85,  148,  149,   88,   46,   47,   48,
 /*   420 */    49,   50,   51,   52,   53,   54,  137,   56,   57,   58,
 /*   430 */    59,   31,   61,   32,  145,  146,   65,   66,   67,   68,
 /*   440 */   148,  149,   71,   86,   87,   31,  135,  136,    6,   35,
 /*   450 */     6,   34,   38,   36,   83,   84,   85,  138,  139,   88,
 /*   460 */    46,   47,   48,   49,   50,   51,   52,   53,   54,  137,
 /*   470 */    56,   57,   58,   59,   32,   61,   32,  145,  146,   65,
 /*   480 */    66,   67,   68,  107,  107,   71,   86,   87,   31,  113,
 /*   490 */   113,   39,   35,    6,    6,   38,   31,   83,   84,   85,
 /*   500 */    34,  136,   88,   46,   47,   48,   49,   50,   51,   52,
 /*   510 */    53,   54,  137,   56,   57,   58,   59,   29,   61,   32,
 /*   520 */   145,  146,   65,   66,   67,   68,   34,   34,   71,   36,
 /*   530 */    31,   31,   60,   63,   32,   35,   69,   66,   38,   43,
 /*   540 */    83,   84,   85,   32,   31,   88,   46,   47,   48,   49,
 /*   550 */    50,   51,   52,   53,   54,  137,   56,   57,   58,   59,
 /*   560 */   140,   61,   34,  145,  146,   65,   66,   67,   68,   43,
 /*   570 */    79,   71,   41,   31,   31,   39,   31,  108,   35,   35,
 /*   580 */    32,   38,   31,   83,   84,   85,   79,   41,   88,   46,
 /*   590 */    47,   48,   49,   50,   51,   52,   53,   54,  137,   56,
 /*   600 */    57,   58,   59,   39,   61,  108,  145,  146,   65,   66,
 /*   610 */    67,   68,   32,  140,   71,   43,   29,   31,   63,   31,
 /*   620 */    31,   35,  113,   32,   38,   63,   83,   84,   85,   31,
 /*   630 */    63,   88,   46,   47,   48,   49,   50,   51,   52,   53,
 /*   640 */    54,  137,   56,   57,   58,   59,  108,   61,   32,  145,
 /*   650 */   146,   65,   66,   67,   68,  140,  140,   71,  108,   39,
 /*   660 */    31,    6,  108,  149,   35,   34,   32,   38,   32,   83,
 /*   670 */    84,   85,  140,   60,   88,   46,   47,   48,   49,   50,
 /*   680 */    51,   52,   53,   54,  137,   56,   57,   58,   59,   43,
 /*   690 */    61,   79,  145,  146,   65,   66,   67,   68,   31,   34,
 /*   700 */    71,   32,   63,   31,   79,   32,   66,   35,   34,   79,
 /*   710 */    38,   32,   83,   84,   85,   31,   79,   88,   46,   47,
 /*   720 */    48,   49,   50,   51,   52,   53,   54,  137,   56,   57,
 /*   730 */    58,   59,   79,   61,   34,  145,  146,   65,   66,   67,
 /*   740 */    68,   34,   79,   71,   32,   79,   31,   31,   35,   78,
 /*   750 */    35,  140,   32,   38,   31,   83,   84,   85,   33,   31,
 /*   760 */    88,   46,   47,   48,   49,   50,   51,   52,   53,   54,
 /*   770 */   137,   56,   57,   58,   59,   31,   61,  150,  145,  146,
 /*   780 */    65,   66,   67,   68,   31,   35,   71,   32,   43,   31,
 /*   790 */   140,   32,    0,   35,   31,   31,   38,   32,   83,   84,
 /*   800 */    85,   34,   32,   88,   46,   47,   48,   49,   50,   51,
 /*   810 */    52,   53,   54,  137,   56,   57,   58,   59,   34,   61,
 /*   820 */   140,  145,  146,   65,   66,   67,   68,  150,   31,   71,
 /*   830 */   108,   63,   31,  139,  156,  156,   35,  156,  156,   38,
 /*   840 */   156,   83,   84,   85,  156,  156,   88,   46,   47,   48,
 /*   850 */    49,   50,   51,   52,   53,   54,  137,   56,   57,   58,
 /*   860 */    59,  156,   61,  156,  145,  146,   65,   66,   67,   68,
 /*   870 */   156,  156,   71,  156,  156,   31,  156,  156,  156,   35,
 /*   880 */   156,  156,   38,  156,   83,   84,   85,  156,  156,   88,
 /*   890 */    46,   47,   48,   49,   50,   51,   52,   53,   54,  137,
 /*   900 */    56,   57,   58,   59,  156,   61,  156,  145,  146,   65,
 /*   910 */    66,   67,   68,  156,  156,   71,  156,  156,   31,  156,
 /*   920 */   156,  156,   35,  156,  156,   38,  156,   83,   84,   85,
 /*   930 */   156,  156,   88,   46,   47,   48,   49,   50,   51,   52,
 /*   940 */    53,   54,  137,   56,   57,   58,   59,  156,   61,  156,
 /*   950 */   145,  146,   65,   66,   67,   68,  156,  156,   71,  156,
 /*   960 */   156,   31,  156,  156,  156,   35,  156,  156,   38,  156,
 /*   970 */    83,   84,   85,  156,  156,   88,   46,   47,   48,   49,
 /*   980 */    50,   51,   52,   53,   54,  137,   56,   57,   58,   59,
 /*   990 */   156,   61,  156,  145,  146,   65,   66,   67,   68,  156,
 /*  1000 */   156,   71,  156,  156,   31,  156,  156,  156,   35,  156,
 /*  1010 */   156,   38,  156,   83,   84,   85,  156,  156,   88,   46,
 /*  1020 */    47,   48,   49,   50,   51,   52,   53,   54,  137,   56,
 /*  1030 */    57,   58,   59,  156,   61,  156,  145,  146,   65,   66,
 /*  1040 */    67,   68,  156,  156,   71,  156,  156,   31,  156,  156,
 /*  1050 */   156,   35,  156,  156,   38,  156,   83,   84,   85,  156,
 /*  1060 */   156,   88,   46,   47,   48,   49,   50,   51,   52,   53,
 /*  1070 */    54,  137,   56,   57,   58,   59,  156,   61,  156,  145,
 /*  1080 */   146,   65,   66,   67,   68,  156,  156,   71,  156,  156,
 /*  1090 */    31,    6,    1,    2,    3,    4,    5,   38,    6,   83,
 /*  1100 */    84,   85,  156,  156,   88,   46,   47,   48,   49,   50,
 /*  1110 */    51,   52,   53,   54,   29,   56,   57,   58,   59,  156,
 /*  1120 */    61,   29,   31,  137,   65,   66,   67,   68,   37,  156,
 /*  1130 */    71,  145,  146,   42,  101,   44,   45,   39,   98,   99,
 /*  1140 */   100,  101,   83,   84,   85,  112,  156,   88,  156,  156,
 /*  1150 */   117,  118,  119,  120,  121,  122,  123,  124,  125,  126,
 /*  1160 */   127,  128,  129,  130,  131,  132,  133,  156,  156,  101,
 /*  1170 */    72,   73,   74,   75,   76,  137,   78,  156,  145,  146,
 /*  1180 */   112,  156,  156,  145,  146,  117,  118,  119,  120,  121,
 /*  1190 */   122,  123,  124,  125,  126,  127,  128,  129,  130,  131,
 /*  1200 */   132,  133,  156,  156,  156,  103,  101,  105,  106,  107,
 /*  1210 */   137,  109,  110,  145,  146,  113,  156,  112,  145,  146,
 /*  1220 */   156,  156,  117,  118,  119,  120,  121,  122,  123,  124,
 /*  1230 */   125,  126,  127,  128,  129,  130,  131,  132,  133,  156,
 /*  1240 */   156,   86,   87,  156,  101,   90,   91,   92,   93,  156,
 /*  1250 */   145,  146,  156,  156,  156,  112,  156,  156,   31,  156,
 /*  1260 */   117,  118,  119,  120,  121,  122,  123,  124,  125,  126,
 /*  1270 */   127,  128,  129,  130,  131,  132,  133,  156,  156,  156,
 /*  1280 */   101,   95,   96,   97,   98,   99,  100,  101,  145,  146,
 /*  1290 */   156,  112,  156,  156,   31,  156,  117,  118,  119,  120,
 /*  1300 */   121,  122,  123,  124,  125,  126,  127,  128,  129,  130,
 /*  1310 */   131,  132,  133,   86,   87,  101,  156,   90,   91,   92,
 /*  1320 */    93,  137,  156,  156,  145,  146,  112,   43,  156,  145,
 /*  1330 */   146,  117,  118,  119,  120,  121,  122,  123,  124,  125,
 /*  1340 */   126,  127,  128,  129,  130,  131,  132,  133,  156,   86,
 /*  1350 */    87,  156,  101,   90,   91,   92,   93,  137,   39,  145,
 /*  1360 */   146,   77,   78,  112,   80,  145,  146,  156,  117,  118,
 /*  1370 */   119,  120,  121,  122,  123,  124,  125,  126,  127,  128,
 /*  1380 */   129,  130,  131,  132,  133,  156,  156,  156,  156,  156,
 /*  1390 */   101,   72,   73,   74,   75,   76,  145,  146,  156,  156,
 /*  1400 */   156,  112,  156,  156,  156,  156,  117,  118,  119,  120,
 /*  1410 */   121,  122,  123,  124,  125,  126,  127,  128,  129,  130,
 /*  1420 */   131,  132,  133,  137,  156,  156,  101,  156,  156,  156,
 /*  1430 */    39,  145,  146,  147,  145,  146,  156,  112,  152,  156,
 /*  1440 */   156,  156,  117,  118,  119,  120,  121,  122,  123,  124,
 /*  1450 */   125,  126,  127,  128,  129,  130,  131,  132,  133,  156,
 /*  1460 */   156,  101,  156,   72,   73,   74,   75,   76,  156,   78,
 /*  1470 */   145,  146,  112,  156,  156,  156,  156,  117,  118,  119,
 /*  1480 */   120,  121,  122,  123,  124,  125,  126,  127,  128,  129,
 /*  1490 */   130,  131,  132,  133,  156,  156,  156,  156,  101,   17,
 /*  1500 */    18,   19,   20,   21,   22,  145,  146,  156,  156,  112,
 /*  1510 */   156,  156,  156,  156,  117,  118,  119,  120,  121,  122,
 /*  1520 */   123,  124,  125,  126,  127,  128,  129,  130,  131,  132,
 /*  1530 */   133,  137,  156,  156,  156,  156,  101,  156,  156,  145,
 /*  1540 */   146,  147,  145,  146,  156,  156,  152,  112,  156,  156,
 /*  1550 */   156,  156,  117,  118,  119,  120,  121,  122,  123,  124,
 /*  1560 */   125,  126,  127,  128,  129,  130,  131,  132,  133,  137,
 /*  1570 */   156,  156,  101,  156,  156,  156,  156,  145,  146,  147,
 /*  1580 */   145,  146,  156,  112,  152,  156,  156,  156,  117,  118,
 /*  1590 */   119,  120,  121,  122,  123,  124,  125,  126,  127,  128,
 /*  1600 */   129,  130,  131,  132,  133,  137,  156,  101,  156,  156,
 /*  1610 */   156,  156,  156,  145,  146,  147,  145,  146,  112,  156,
 /*  1620 */   152,  156,  156,  117,  118,  119,  120,  121,  122,  123,
 /*  1630 */   124,  125,  126,  127,  128,  129,  130,  131,  132,  133,
 /*  1640 */   137,  156,  156,  156,  101,  156,  143,  144,  145,  146,
 /*  1650 */   156,  145,  146,  156,  156,  112,  156,  156,  156,  156,
 /*  1660 */   117,  118,  119,  120,  121,  122,  123,  124,  125,  126,
 /*  1670 */   127,  128,  129,  130,  131,  132,  133,  137,  156,  156,
 /*  1680 */   156,  156,  101,  156,  156,  145,  146,  147,  145,  146,
 /*  1690 */   156,  156,  152,  112,  156,  156,  156,  156,  117,  118,
 /*  1700 */   119,  120,  121,  122,  123,  124,  125,  126,  127,  128,
 /*  1710 */   129,  130,  131,  132,  133,  156,  156,  156,  101,  156,
 /*  1720 */   105,  137,  107,  156,  109,  110,  145,  146,  113,  145,
 /*  1730 */   146,  156,  156,  156,  117,  118,  119,  120,  121,  122,
 /*  1740 */   123,  124,  125,  126,  127,  128,  129,  130,  131,  132,
 /*  1750 */   133,   23,   24,   25,   26,   27,   28,  156,  156,   31,
 /*  1760 */   156,  156,  145,  146,  156,  156,   39,  156,  156,   41,
 /*  1770 */   156,   43,   23,   24,   25,   26,   27,   28,  137,  156,
 /*  1780 */    31,  156,  141,  156,  156,  137,  145,  146,  156,  141,
 /*  1790 */    41,  156,   43,  145,  146,  156,  156,  156,   70,   72,
 /*  1800 */    73,   74,   75,   76,   77,   78,   78,   80,   81,   82,
 /*  1810 */   156,  156,  156,  156,   86,   87,  156,   89,   90,   91,
 /*  1820 */    92,   93,  134,  137,  156,  137,  156,   78,   79,  156,
 /*  1830 */   156,  145,  146,  145,  146,   86,   87,  156,   89,   90,
 /*  1840 */    91,   92,   93,   23,   24,   25,   26,   27,   28,  137,
 /*  1850 */   156,   31,  156,  141,  156,  156,  156,  145,  146,  156,
 /*  1860 */   156,   41,  156,   43,   23,   24,   25,   26,   27,   28,
 /*  1870 */    29,  156,   31,  156,  156,  156,  156,  156,  156,  156,
 /*  1880 */   156,  156,   41,  156,   43,   23,   24,   25,   26,   27,
 /*  1890 */    28,   29,  137,   31,  156,  156,  141,  156,   78,   79,
 /*  1900 */   145,  146,  156,   41,  156,   43,   86,   87,  156,   89,
 /*  1910 */    90,   91,   92,   93,  137,  156,  137,  156,  141,   78,
 /*  1920 */   141,  156,  145,  146,  145,  146,  156,   86,   87,  156,
 /*  1930 */    89,   90,   91,   92,   93,  134,  137,  156,  137,  156,
 /*  1940 */    78,  156,  156,  156,  145,  146,  145,  146,   86,   87,
 /*  1950 */   156,   89,   90,   91,   92,   93,   23,   24,   25,   26,
 /*  1960 */    27,   28,   29,  137,   31,  156,    1,    2,    3,    4,
 /*  1970 */     5,  145,  146,  156,   41,  156,   43,   23,   24,   25,
 /*  1980 */    26,   27,   28,   29,  137,   31,  156,  156,  141,  156,
 /*  1990 */   156,  156,  145,  146,  156,   41,  156,   43,  156,  156,
 /*  2000 */    35,  134,   37,   38,  137,   40,  156,  156,  156,   44,
 /*  2010 */    45,   78,  145,  146,  156,  156,  156,  156,  156,   86,
 /*  2020 */    87,  156,   89,   90,   91,   92,   93,  134,  156,  156,
 /*  2030 */   137,  156,   78,  156,  156,  156,  156,  156,  145,  146,
 /*  2040 */    86,   87,  156,   89,   90,   91,   92,   93,   23,   24,
 /*  2050 */    25,   26,   27,   28,  156,  156,   31,  156,    1,    2,
 /*  2060 */     3,    4,    5,  156,  156,  156,   41,  156,   43,   23,
 /*  2070 */    24,   25,   26,   27,   28,  156,  156,   31,   32,  156,
 /*  2080 */   156,  134,  156,  156,  137,  156,  156,   41,   31,   43,
 /*  2090 */   156,  156,  145,  146,   37,   70,  137,  156,  156,   42,
 /*  2100 */   141,   44,   45,   78,  145,  146,  156,  156,  156,  156,
 /*  2110 */   156,   86,   87,  156,   89,   90,   91,   92,   93,  137,
 /*  2120 */   156,  156,  156,  141,   78,  156,  156,  145,  146,  156,
 /*  2130 */   156,  156,   86,   87,  156,   89,   90,   91,   92,   93,
 /*  2140 */    23,   24,   25,   26,   27,   28,  156,   11,   31,  156,
 /*  2150 */   156,  156,  156,  156,  156,  156,  156,  156,   41,  156,
 /*  2160 */    43,   23,   24,   25,   26,   27,   28,   31,  156,   31,
 /*  2170 */   156,  156,  156,  156,  156,  156,  156,  156,  156,   41,
 /*  2180 */   156,   43,   46,   47,   48,   49,   50,   51,   52,   53,
 /*  2190 */    54,   55,   56,  156,  156,   78,   79,  156,  156,  156,
 /*  2200 */   156,  156,  156,   86,   87,  156,   89,   90,   91,   92,
 /*  2210 */    93,  156,  156,  156,  156,  156,   78,  156,    1,    2,
 /*  2220 */     3,    4,    5,  156,   86,   87,  156,   89,   90,   91,
 /*  2230 */    92,   93,    7,    8,    9,   10,   11,   12,   13,   14,
 /*  2240 */    15,   16,   17,   18,   19,   20,   21,   22,  156,  156,
 /*  2250 */   156,  156,   35,  156,   37,   38,  156,   40,  156,   34,
 /*  2260 */   156,   44,   45,  156,  156,  156,  156,    7,    8,    9,
 /*  2270 */    10,   11,   12,   13,   14,   15,   16,   17,   18,   19,
 /*  2280 */    20,   21,   22,  156,  156,  156,  156,  156,  156,   29,
 /*  2290 */     7,    8,    9,   10,   11,   12,   13,   14,   15,   16,
 /*  2300 */    17,   18,   19,   20,   21,   22,    7,    8,    9,   10,
 /*  2310 */    11,   12,   13,   14,   15,   16,   17,   18,   19,   20,
 /*  2320 */    21,   22,  156,    1,    2,    3,    4,    5,  156,  156,
 /*  2330 */   156,  156,  156,   34,  156,  156,  156,  156,  156,  156,
 /*  2340 */   156,    7,    8,    9,   10,   11,   12,   13,   14,   15,
 /*  2350 */    16,   17,   18,   19,   20,   21,   22,   35,  156,   37,
 /*  2360 */    38,  156,   79,  156,  156,  156,   44,   45,   34,  156,
 /*  2370 */     7,    8,    9,   10,   11,   12,   13,   14,   15,   16,
 /*  2380 */    17,   18,   19,   20,   21,   22,    7,    8,    9,   10,
 /*  2390 */    11,   12,   13,   14,   15,   16,   17,   18,   19,   20,
 /*  2400 */    21,   22,  156,  102,  103,  104,  105,  106,  107,  156,
 /*  2410 */   109,  110,  156,   34,  113,  156,    1,    2,    3,    4,
 /*  2420 */     5,    7,    8,    9,   10,   11,   12,   13,   14,   15,
 /*  2430 */    16,   17,   18,   19,   20,   21,   22,   11,  156,  156,
 /*  2440 */   156,  156,   79,  156,  156,  156,  156,  156,   11,  156,
 /*  2450 */   156,  156,   37,  156,  156,   29,  156,   31,  156,   44,
 /*  2460 */    45,  156,  156,  156,  156,  156,   29,  156,   31,  156,
 /*  2470 */   156,  156,   46,   47,   48,   49,   50,   51,   52,   53,
 /*  2480 */    54,   55,   56,   46,   47,   48,   49,   50,   51,   52,
 /*  2490 */    53,   54,   55,   56,    9,   10,   11,   12,   13,   14,
 /*  2500 */    15,   16,   17,   18,   19,   20,   21,   22,    1,    2,
 /*  2510 */     3,    4,    5,  102,  103,  104,  105,  106,  107,  156,
 /*  2520 */   109,  110,  156,  156,  113,  102,  103,  104,  105,  106,
 /*  2530 */   107,  156,  109,  110,  156,  156,  113,  156,  156,  156,
 /*  2540 */   156,  156,   35,  156,   37,   38,  156,   40,  156,  156,
 /*  2550 */   156,   44,   45,    1,    2,    3,    4,    5,  102,  103,
 /*  2560 */   104,  105,  106,  107,  156,  109,  110,  156,  156,  113,
 /*  2570 */   156,    1,    2,    3,    4,    5,  156,    1,    2,    3,
 /*  2580 */     4,    5,  156,  156,  156,  156,  156,   35,  156,   37,
 /*  2590 */    38,  156,   40,  156,  156,  156,   44,   45,    1,    2,
 /*  2600 */     3,    4,    5,  156,  156,  156,  156,   37,  156,  156,
 /*  2610 */   156,  156,   42,   37,   44,   45,  156,  156,   42,  156,
 /*  2620 */    44,   45,    1,    2,    3,    4,    5,  156,  156,  156,
 /*  2630 */   156,  156,   35,  156,   37,   38,  156,   40,  156,  156,
 /*  2640 */   156,   44,   45,    1,    2,    3,    4,    5,  156,  156,
 /*  2650 */    31,  156,  156,  156,  156,  156,  156,  156,   37,  156,
 /*  2660 */   156,   40,  156,  156,  156,   44,   45,  156,  156,  156,
 /*  2670 */   156,  156,  156,  156,  156,  156,  156,   35,  156,   37,
 /*  2680 */    38,  156,   40,  156,  156,  156,   44,   45,    1,    2,
 /*  2690 */     3,    4,    5,  156,  156,  156,  156,  156,   79,  156,
 /*  2700 */   156,  156,  156,  156,  156,   86,   87,  156,  156,   90,
 /*  2710 */    91,   92,   93,  156,  156,  156,  156,  156,  156,  156,
 /*  2720 */   156,  156,   35,  156,   37,   38,  156,   40,  156,  156,
 /*  2730 */   156,   44,   45,    1,    2,    3,    4,    5,    1,    2,
 /*  2740 */     3,    4,    5,  156,  156,  156,  156,  156,  156,  156,
 /*  2750 */   156,  156,  156,  156,  156,  156,  156,  156,  156,  156,
 /*  2760 */   156,  156,  156,  156,  156,  156,  156,   35,  156,   37,
 /*  2770 */    38,  156,   35,  156,   37,   38,   44,   45,  156,  156,
 /*  2780 */   156,   44,   45,    1,    2,    3,    4,    5,    1,    2,
 /*  2790 */     3,    4,    5,  156,  156,  156,  156,  156,  156,  156,
 /*  2800 */   156,  156,  156,  156,  156,  156,  156,  156,  156,  156,
 /*  2810 */   156,  156,  156,  156,  156,  156,  156,   35,  156,   37,
 /*  2820 */    38,  156,   35,  156,   37,   38,   44,   45,  156,  156,
 /*  2830 */   156,   44,   45,    1,    2,    3,    4,    5,  156,  156,
 /*  2840 */   156,  156,  156,  156,  156,  156,  156,  156,  156,  156,
 /*  2850 */   156,  156,  156,  156,  156,  156,  156,  156,  156,  156,
 /*  2860 */   156,  156,  156,  156,  156,  156,  156,   35,  156,   37,
 /*  2870 */    38,  156,  156,  156,  156,  156,   44,   45,
};
#define YY_SHIFT_USE_DFLT (-72)
static short yy_shift_ofst[] = {
 /*     0 */   -16,  792,  109,  -72,  -72,  -72,  -72,  499,  511,  -72,
 /*    10 */     3,  417, 1965,  544,  -72,  -72, 2642, 2322,  -72, 2570,
 /*    20 */   764,  745, 2426,  665,  715,  -72,  586,  -72,  -72,  -72,
 /*    30 */   -72,  -72,  -72,  -72,  -72,  -72,  -72,  -72,  -72,  -72,
 /*    40 */   -72,  -72,  -72,  -72,  -72,  -72,  591,  -72,  755,  -72,
 /*    50 */  2138,  674,  801,  156,  613,  631, 1016,  672,  -72, 2138,
 /*    60 */   528,  121,  -72,  164,  -72,  -72, 1155,  768, 1059, 1059,
 /*    70 */   784,   27,  113,  -72, 2138,  306,  887,  844,  -72,  700,
 /*    80 */   371,   70,  640, 2138,  765,  -72, 2138,  -72, 2138,  589,
 /*    90 */   -72,  671,  400,  653,  -72,  630,  -72,  612,  -72, 1482,
 /*   100 */  2138, 1482, 2138, 1482, 2138, 1482, 2138, 1482, 2138, 1482,
 /*   110 */  2138, 1482, 2138, 1482, 2138,  168, 2138,  168, 2138,  -72,
 /*   120 */  2138,  -72, 2138,  -72, 2138,  168, 2138, 2485, 2138, 2485,
 /*   130 */   -23,  -72,  -72,  -72,  -72,  -72,  -72,  -72,  -72,  -72,
 /*   140 */   -72,  -72, 2260,  -72,  587, 2138,  -72, 1284,  716,  526,
 /*   150 */  1954, 2414, 2138,  -72,  744,  655,  589,  -72, 2138,  -72,
 /*   160 */   -72,  -72,  -72,  -72,  -72,  -72,  -72,  -72, 2619,  -72,
 /*   170 */    66,  -72, 1227,  -72,  639, 1263,  -72,  -72,  -72,  -72,
 /*   180 */   -72,  -72,  -72,  -72,  -72,  -72,  567,  562,  555,  -72,
 /*   190 */   588,  572, 1933,  -72,  318,  -72, 2138,  -72,  -72,  -72,
 /*   200 */   -72,  101,  -72,  -72,   30,  496, 1862,  488,  -72,  -72,
 /*   210 */   -72, 2138, 2283,  -72, 1841, 1085,  -72,  -72, 2414,  465,
 /*   220 */   291, 2025, 2225, 1059,  543,  -72,  126,  487,  -72,  126,
 /*   230 */   -72, 1727,  -72,  -72,  -72,  -72,  -72,  -72, 2138,  -72,
 /*   240 */  2414,  250, 1391, 2138,  -72, 2117, 1319, 2138,  -72,  507,
 /*   250 */  1319, 2138,  -72, 2363,  598, 1098, 2138,  -72, 1749, 1319,
 /*   260 */  2138,  -72,  663, 1319, 2138,  -72, 1820, 1319, 2138,  -72,
 /*   270 */   491, 1319, 2138,  -72,  -72,  -72,  -72, 2138,   81,  -72,
 /*   280 */  2138,  -72, 2414,  -72,  770,  -72,  759,  -72, 2046,   28,
 /*   290 */   -72,  -72,  753,  -71,  723,  720,  -72,  357,  666,  712,
 /*   300 */   -72,  637,  679,  -72,  625,  673,  -72,  198,  669,  -72,
 /*   310 */   667,  646, 2138,  146,  636,  -72,  551,  108,  -72,  551,
 /*   320 */   -72,  620, 1155,  -72,  -72,  551,  229,  -72,  551,  296,
 /*   330 */   -72,  551,  279,  -72,  551,  280,  -72,  551,  339,  -72,
 /*   340 */   551,  366,  -72,  551,  401,  -72,  551,  444,  -72,  551,
 /*   350 */   442,  -72,   19,  545,  496,  542,  526,  -72,  -72, 2138,
 /*   360 */  2334, 1059,  328,  -72,  513,  467, 1728, 2379, 1059,  500,
 /*   370 */   -72, 2138, 2299, 1059,  758,  -72,  471, 2138,  502,  -72,
 /*   380 */   -72,  -72,  470, 1059, 1059,  -72,  -72,  472,  492,  457,
 /*   390 */   629,  -72,  -72, 1092,  466,  285,  -72,  973,  -72, 2136,
 /*   400 */   -72,  452, 1155,  -72,  331,  233, 1155,  -72,  187,  143,
 /*   410 */  1155,  -72,  112,   94,   23,  -72,  -72,  -72,  -72,  -72,
 /*   420 */   -72,  -72,  -72,  -72,  -72, 2415, 2576,  797,  -34, 2437,
 /*   430 */    55,  930,  -72,  414,  -72,  361,  314,  199,  -72,  242,
 /*   440 */   -72,  -72, 2687, 2832,  -72,  -72, 2621,  531,  536, 1155,
 /*   450 */   548,  -72,  546,  564, 1155,  580,  -72,  -72, 2621, 2057,
 /*   460 */   204,  -72, 1155,  616,  -72, 1091,  369,  -72, 1155,  634,
 /*   470 */   -72,  -72, 2217, 2787, 2782, 2415, 2737,  -72, 2732,  684,
 /*   480 */   707, 2507,  713,  -72,  -72,  725,  728,  493, 2597,  750,
 /*   490 */   -72,  -72,  763,  767, 2552,  294,  -72,  -72,  -72,
};
#define YY_REDUCE_USE_DFLT (-95)
static short yy_reduce_ofst[] = {
 /*     0 */  1186,  -95, 1040,  -95,  -95,  -95,  -95,  -95,  -95,  -95,
 /*    10 */   -95,  -95, 2456,  -95,  -95,  -95,  -94,  290,  -95,  509,
 /*    20 */   -95,  -95,  160,  -95, 1143,  -95, 1617,  -95,  -95,  -95,
 /*    30 */   -95,  -95,  -95,  -95,  -95,  -95,  -95,  -95,  -95,  -95,
 /*    40 */   -95,  -95,  -95,  -95,  -95,  -95,  -95,  -95,  -95,  -95,
 /*    50 */  1947,  -95, 1581, 1617,  -95,  -95, 1179, 1617,  -95, 1801,
 /*    60 */   -95,  311,  -95,  365,  -95,  -95,  722,  -95, 1251, 1617,
 /*    70 */   -95, 1289, 1617,  -95, 1867,  -95, 1033, 1617,  -95,  -95,
 /*    80 */  1397, 1617,  -95, 1688,  -95,  -95,  461,  -95,  332,  627,
 /*    90 */   -95,  -95,  -95,  -95,  -95,  -95,  -95,  -95,  -95,  -95,
 /*   100 */  1220,  -95, 1799,  -95,  375,  -95,  719,  -95,  590,  -95,
 /*   110 */   212,  -95,  504,  -95,  848,  -95,  891,  -95,  633,  -95,
 /*   120 */  1184,  -95, 1038,  -95, 1826,  -95,  547,  -95,  934,  -95,
 /*   130 */   246,  -95,  -95,  -95,  -95,  -95,  -95,  -95,  -95,  -95,
 /*   140 */   -95,  -95,  -95,  -95,  -95,  762,  -95,  -95,  -95,  -95,
 /*   150 */  1468,  -95,  289,  -95,  -95,  -95,  677,  -95,  986,  -95,
 /*   160 */   -95,  -95,  -95,  -95,  -95,  -95,  -95,  -95,  -51,  -95,
 /*   170 */   -95,  -95,   78,  -95,  -95,   87,  -95,  -95,  -95,  -95,
 /*   180 */   -95,  -95,  -95,  -95,  -95,  -95,  -95,  -95,  -95,  -95,
 /*   190 */   -95,  -95, 1394,  -95,  -95,  -95,  -55,  -95,  -95,  -95,
 /*   200 */   -95,  -95,  -95,  -95,  -95,  -95, 1432,  -95,  -95,  -95,
 /*   210 */   -95, 1584,  -95,  -95, 1286,  -95,  -95,  -95,  -95,  -95,
 /*   220 */   -95,  676,  -95, 1506, 1617,  -95,  319,  -95,  -95,  694,
 /*   230 */   -95,  680,  -95,  -95,  -95,  -95,  -95,  -95, 1982,  -95,
 /*   240 */   -95,  -95,   98, 1847,  -95,   31,  420, 1712,  -95,  -95,
 /*   250 */   473, 1648,  -95,  -95,  -95,  515, 1755,  -95,   74,  532,
 /*   260 */  1779,  -95,  -95,  611, 1959,  -95,  117,  650, 1641,  -95,
 /*   270 */   -95,  516, 1777,  -95,  -95,  -95,  -95, 1503,  -95,  -95,
 /*   280 */   -12,  -95,  -95,  -95,  -95,  -95,  -95,  -95, 1073,  -95,
 /*   290 */   -95,  -95,  -95,  -95,  -95,  -95,  -95,  -95,  -95,  -95,
 /*   300 */   -95,  -95,  -95,  -95,  -95,  -95,  -95,  -95,  -95,  -95,
 /*   310 */   -95,  -95, 1540,  -95,  -95,  -95,    1,  -95,  -95,  514,
 /*   320 */   -95,  -95,  550,  -95,  -95,   44,  -95,  -95,  120,  -95,
 /*   330 */   -95,  130,  -95,  -95,  173,  -95,  -95,  206,  -95,  -95,
 /*   340 */   216,  -95,  -95,  256,  -95,  -95,  266,  -95,  -95,  292,
 /*   350 */   -95,  -95,  -95,  -95,  -95,  -95,  -95,  -95,  -95,  805,
 /*   360 */   -95, 1214, 1617,  -95,  -95,  -95,  418,  -95, 1068, 1617,
 /*   370 */   -95, 1686,  -95, 1105, 1617,  -95,  -95, 1893,  -95,  -95,
 /*   380 */   -95,  -95,  -95, 1543, 1617,  -95,  -95,  -95,  -95, 1435,
 /*   390 */  1617,  -95,  -95,  -95,  -95, 1325,  -95, 1617,  -95,  190,
 /*   400 */   -95,  -95,  302,  -95,  -95,  -95,  207,  -95,  -95,  -95,
 /*   410 */    92,  -95,  -95,  -95,  -95,  -95,  -95,  -95,  -95,  -95,
 /*   420 */   -95,  -95,  -95,  -95,  -95,  376,  509,  -95,  -95,  203,
 /*   430 */   -95, 1360,  -95, 1617,  -95,  -95,  -95, 1471,  -95, 1617,
 /*   440 */   -95,  -95, 1615,  290,  -95,  -95,  376,  -95,  -95,  469,
 /*   450 */   -95,  -95,  -95,  -95,  497,  -95,  -95,  -95,  377,  509,
 /*   460 */   -95,  -95,  538,  -95,  -95,  509,  -95,  -95,  554,  -95,
 /*   470 */   -95,  -95, 1102,  -54,  290,  377,  290,  -95,  290,  -95,
 /*   480 */   -95, 2423,  -95,  -95,  -95,  -95,  -95,  -95, 2411,  -95,
 /*   490 */   -95,  -95,  -95,  -95, 2301,  -95,  -95,  -95,  -95,
};
static YYACTIONTYPE yy_default[] = {
 /*     0 */   739,  739,  739,  501,  503,  504,  505,  739,  739,  506,
 /*    10 */   739,  739,  739,  739,  507,  511,  739,  739,  532,  739,
 /*    20 */   739,  739,  739,  739,  739,  536,  739,  538,  574,  576,
 /*    30 */   577,  578,  579,  580,  581,  582,  583,  584,  585,  586,
 /*    40 */   587,  588,  589,  590,  591,  592,  739,  593,  739,  594,
 /*    50 */   739,  739,  739,  739,  597,  739,  739,  739,  598,  739,
 /*    60 */   739,  739,  601,  739,  602,  603,  739,  739,  739,  605,
 /*    70 */   739,  739,  739,  608,  739,  739,  739,  739,  610,  739,
 /*    80 */   739,  739,  739,  739,  739,  612,  739,  667,  739,  739,
 /*    90 */   668,  739,  739,  739,  734,  739,  735,  739,  736,  669,
 /*   100 */   739,  670,  739,  671,  739,  672,  739,  673,  739,  674,
 /*   110 */   739,  675,  739,  676,  739,  683,  739,  684,  739,  685,
 /*   120 */   739,  686,  739,  687,  739,  688,  739,  689,  739,  690,
 /*   130 */   739,  563,  564,  565,  566,  567,  568,  569,  570,  571,
 /*   140 */   572,  573,  739,  677,  739,  739,  678,  695,  739,  679,
 /*   150 */   739,  718,  739,  691,  739,  739,  739,  692,  739,  693,
 /*   160 */   694,  696,  697,  698,  699,  700,  701,  702,  739,  703,
 /*   170 */   739,  704,  739,  719,  739,  739,  721,  726,  727,  728,
 /*   180 */   729,  730,  731,  732,  733,  722,  727,  729,  728,  720,
 /*   190 */   739,  705,  739,  706,  739,  707,  739,  710,  711,  716,
 /*   200 */   717,  739,  714,  715,  739,  680,  739,  739,  712,  713,
 /*   210 */   681,  739,  739,  682,  739,  739,  708,  709,  737,  739,
 /*   220 */   739,  739,  739,  739,  739,  613,  739,  739,  617,  739,
 /*   230 */   618,  739,  620,  621,  622,  623,  624,  625,  739,  626,
 /*   240 */   666,  739,  739,  739,  627,  739,  739,  739,  628,  739,
 /*   250 */   739,  739,  629,  739,  739,  739,  739,  630,  739,  739,
 /*   260 */   739,  631,  739,  739,  739,  632,  739,  739,  739,  633,
 /*   270 */   739,  739,  739,  634,  635,  636,  619,  739,  739,  638,
 /*   280 */   739,  639,  641,  640,  739,  642,  739,  643,  739,  739,
 /*   290 */   644,  645,  739,  739,  739,  739,  646,  739,  739,  739,
 /*   300 */   647,  739,  739,  648,  739,  739,  649,  739,  739,  650,
 /*   310 */   739,  739,  739,  739,  739,  651,  739,  739,  652,  739,
 /*   320 */   662,  664,  739,  665,  663,  739,  739,  653,  739,  739,
 /*   330 */   654,  739,  739,  655,  739,  739,  656,  739,  739,  657,
 /*   340 */   739,  739,  658,  739,  739,  659,  739,  739,  660,  739,
 /*   350 */   739,  661,  739,  739,  739,  739,  739,  738,  575,  739,
 /*   360 */   739,  739,  739,  614,  739,  739,  739,  739,  739,  739,
 /*   370 */   615,  739,  739,  739,  739,  616,  739,  739,  739,  611,
 /*   380 */   609,  607,  739,  739,  606,  604,  599,  595,  739,  739,
 /*   390 */   739,  600,  596,  739,  739,  739,  537,  739,  539,  739,
 /*   400 */   554,  556,  739,  559,  739,  557,  739,  560,  739,  558,
 /*   410 */   739,  561,  739,  739,  739,  555,  544,  546,  547,  548,
 /*   420 */   549,  550,  551,  552,  553,  739,  739,  739,  739,  739,
 /*   430 */   739,  739,  540,  739,  542,  739,  739,  739,  541,  739,
 /*   440 */   543,  545,  739,  739,  530,  533,  739,  739,  739,  739,
 /*   450 */   739,  534,  739,  739,  739,  739,  535,  524,  739,  739,
 /*   460 */   739,  526,  739,  739,  528,  739,  739,  527,  739,  739,
 /*   470 */   529,  531,  739,  739,  739,  739,  739,  525,  739,  739,
 /*   480 */   739,  739,  739,  508,  512,  739,  739,  739,  739,  739,
 /*   490 */   509,  513,  739,  739,  739,  739,  510,  514,  502,
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
  "PRIVATE",       "SCOPED",        "COMMA",         "AND",         
  "OR",            "EQUALS",        "IDENTICAL",     "LESS",        
  "GREATER",       "LESSEQUAL",     "GREATEREQUAL",  "NOTIDENTICAL",
  "NOTEQUALS",     "ADD",           "SUB",           "CONCAT",      
  "MUL",           "DIV",           "MOD",           "ISSET",       
  "FETCH",         "TYPEOF",        "INSTANCEOF",    "NEW",         
  "NOT",           "PARENTHESES_CLOSE",  "NAMESPACE",     "IDENTIFIER",  
  "DOTCOMMA",      "CLASS",         "BRACKET_OPEN",  "BRACKET_CLOSE",
  "EXTENDS",       "ABSTRACT",      "COMMENT",       "ASSIGN",      
  "CONST",         "CONSTANT",      "FUNCTION",      "PARENTHESES_OPEN",
  "INLINE",        "FINAL",         "TYPE_INTEGER",  "TYPE_UINTEGER",
  "TYPE_LONG",     "TYPE_ULONG",    "TYPE_CHAR",     "TYPE_UCHAR",  
  "TYPE_DOUBLE",   "TYPE_BOOL",     "TYPE_STRING",   "TYPE_ARRAY",  
  "TYPE_VAR",      "BREAK",         "CONTINUE",      "IF",          
  "ELSE",          "SWITCH",        "CASE",          "COLON",       
  "DEFAULT",       "LOOP",          "WHILE",         "DO",          
  "FOR",           "IN",            "REVERSE",       "LET",         
  "ADDASSIGN",     "SUBASSIGN",     "MULASSIGN",     "DIVASSIGN",   
  "CONCATASSIGN",  "ARROW",         "SBRACKET_OPEN",  "SBRACKET_CLOSE",
  "DOUBLECOLON",   "INCR",          "DECR",          "ECHO",        
  "RETURN",        "UNSET",         "INTEGER",       "STRING",      
  "THROW",         "CHAR",          "DOUBLE",        "NULL",        
  "TRUE",          "FALSE",         "error",         "program",     
  "xx_language",   "xx_top_statement_list",  "xx_top_statement",  "xx_namespace_def",
  "xx_class_def",  "xx_comment",    "xx_class_definition",  "xx_class_properties_definition",
  "xx_class_consts_definition",  "xx_class_methods_definition",  "xx_class_property_definition",  "xx_visibility_list",
  "xx_literal_expr",  "xx_class_const_definition",  "xx_class_method_definition",  "xx_parameter_list",
  "xx_statement_list",  "xx_visibility",  "xx_parameter",  "xx_parameter_type",
  "xx_parameter_cast",  "xx_statement",  "xx_let_statement",  "xx_if_statement",
  "xx_loop_statement",  "xx_echo_statement",  "xx_return_statement",  "xx_mcall_statement",
  "xx_scall_statement",  "xx_unset_statement",  "xx_throw_statement",  "xx_declare_statement",
  "xx_break_statement",  "xx_continue_statement",  "xx_while_statement",  "xx_do_while_statement",
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
 /*  27 */ "xx_class_property_definition ::= COMMENT xx_visibility_list IDENTIFIER DOTCOMMA",
 /*  28 */ "xx_class_property_definition ::= xx_visibility_list IDENTIFIER DOTCOMMA",
 /*  29 */ "xx_class_property_definition ::= COMMENT xx_visibility_list IDENTIFIER ASSIGN xx_literal_expr DOTCOMMA",
 /*  30 */ "xx_class_property_definition ::= xx_visibility_list IDENTIFIER ASSIGN xx_literal_expr DOTCOMMA",
 /*  31 */ "xx_class_consts_definition ::= xx_class_consts_definition xx_class_const_definition",
 /*  32 */ "xx_class_consts_definition ::= xx_class_const_definition",
 /*  33 */ "xx_class_methods_definition ::= xx_class_methods_definition xx_class_method_definition",
 /*  34 */ "xx_class_methods_definition ::= xx_class_method_definition",
 /*  35 */ "xx_class_const_definition ::= COMMENT CONST CONSTANT ASSIGN xx_literal_expr DOTCOMMA",
 /*  36 */ "xx_class_const_definition ::= CONST CONSTANT ASSIGN xx_literal_expr DOTCOMMA",
 /*  37 */ "xx_class_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  38 */ "xx_class_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  39 */ "xx_class_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  40 */ "xx_class_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  41 */ "xx_class_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  42 */ "xx_class_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  43 */ "xx_class_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  44 */ "xx_class_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  45 */ "xx_visibility_list ::= xx_visibility_list xx_visibility",
 /*  46 */ "xx_visibility_list ::= xx_visibility",
 /*  47 */ "xx_visibility ::= PUBLIC",
 /*  48 */ "xx_visibility ::= PROTECTED",
 /*  49 */ "xx_visibility ::= PRIVATE",
 /*  50 */ "xx_visibility ::= STATIC",
 /*  51 */ "xx_visibility ::= SCOPED",
 /*  52 */ "xx_visibility ::= INLINE",
 /*  53 */ "xx_visibility ::= ABSTRACT",
 /*  54 */ "xx_visibility ::= FINAL",
 /*  55 */ "xx_parameter_list ::= xx_parameter_list COMMA xx_parameter",
 /*  56 */ "xx_parameter_list ::= xx_parameter",
 /*  57 */ "xx_parameter ::= IDENTIFIER",
 /*  58 */ "xx_parameter ::= xx_parameter_type IDENTIFIER",
 /*  59 */ "xx_parameter ::= xx_parameter_cast IDENTIFIER",
 /*  60 */ "xx_parameter ::= IDENTIFIER ASSIGN xx_literal_expr",
 /*  61 */ "xx_parameter ::= xx_parameter_type IDENTIFIER ASSIGN xx_literal_expr",
 /*  62 */ "xx_parameter ::= xx_parameter_cast IDENTIFIER ASSIGN xx_literal_expr",
 /*  63 */ "xx_parameter_cast ::= LESS IDENTIFIER GREATER",
 /*  64 */ "xx_parameter_type ::= TYPE_INTEGER",
 /*  65 */ "xx_parameter_type ::= TYPE_UINTEGER",
 /*  66 */ "xx_parameter_type ::= TYPE_LONG",
 /*  67 */ "xx_parameter_type ::= TYPE_ULONG",
 /*  68 */ "xx_parameter_type ::= TYPE_CHAR",
 /*  69 */ "xx_parameter_type ::= TYPE_UCHAR",
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
 /*  90 */ "xx_statement ::= xx_do_while_statement",
 /*  91 */ "xx_statement ::= xx_switch_statement",
 /*  92 */ "xx_statement ::= xx_for_statement",
 /*  93 */ "xx_statement ::= xx_comment",
 /*  94 */ "xx_break_statement ::= BREAK DOTCOMMA",
 /*  95 */ "xx_continue_statement ::= CONTINUE DOTCOMMA",
 /*  96 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /*  97 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE ELSE BRACKET_OPEN BRACKET_CLOSE",
 /*  98 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  99 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE ELSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 100 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE ELSE BRACKET_OPEN BRACKET_CLOSE",
 /* 101 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE ELSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 102 */ "xx_switch_statement ::= SWITCH xx_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /* 103 */ "xx_switch_statement ::= SWITCH xx_eval_expr BRACKET_OPEN xx_case_clauses BRACKET_CLOSE",
 /* 104 */ "xx_case_clauses ::= xx_case_clauses xx_case_clause",
 /* 105 */ "xx_case_clauses ::= xx_case_clause",
 /* 106 */ "xx_case_clause ::= CASE xx_literal_expr COLON xx_statement_list",
 /* 107 */ "xx_case_clause ::= DEFAULT COLON xx_statement_list",
 /* 108 */ "xx_loop_statement ::= LOOP BRACKET_OPEN BRACKET_CLOSE",
 /* 109 */ "xx_loop_statement ::= LOOP BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 110 */ "xx_while_statement ::= WHILE xx_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /* 111 */ "xx_while_statement ::= WHILE xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 112 */ "xx_do_while_statement ::= DO BRACKET_OPEN BRACKET_CLOSE WHILE xx_eval_expr DOTCOMMA",
 /* 113 */ "xx_do_while_statement ::= DO BRACKET_OPEN xx_statement_list BRACKET_CLOSE WHILE xx_eval_expr DOTCOMMA",
 /* 114 */ "xx_for_statement ::= FOR IDENTIFIER IN xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 115 */ "xx_for_statement ::= FOR IDENTIFIER IN REVERSE xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 116 */ "xx_for_statement ::= FOR IDENTIFIER COMMA IDENTIFIER IN xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 117 */ "xx_for_statement ::= FOR IDENTIFIER COMMA IDENTIFIER IN REVERSE xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 118 */ "xx_let_statement ::= LET xx_let_assignments DOTCOMMA",
 /* 119 */ "xx_let_assignments ::= xx_let_assignments COMMA xx_let_assignment",
 /* 120 */ "xx_let_assignments ::= xx_let_assignment",
 /* 121 */ "xx_assignment_operator ::= ASSIGN",
 /* 122 */ "xx_assignment_operator ::= ADDASSIGN",
 /* 123 */ "xx_assignment_operator ::= SUBASSIGN",
 /* 124 */ "xx_assignment_operator ::= MULASSIGN",
 /* 125 */ "xx_assignment_operator ::= DIVASSIGN",
 /* 126 */ "xx_assignment_operator ::= CONCATASSIGN",
 /* 127 */ "xx_let_assignment ::= IDENTIFIER xx_assignment_operator xx_assign_expr",
 /* 128 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER xx_assignment_operator xx_assign_expr",
 /* 129 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 130 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER SBRACKET_OPEN xx_index_expr SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 131 */ "xx_let_assignment ::= IDENTIFIER DOUBLECOLON IDENTIFIER xx_assignment_operator xx_assign_expr",
 /* 132 */ "xx_let_assignment ::= IDENTIFIER DOUBLECOLON IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 133 */ "xx_let_assignment ::= IDENTIFIER DOUBLECOLON IDENTIFIER SBRACKET_OPEN xx_index_expr SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 134 */ "xx_let_assignment ::= IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 135 */ "xx_let_assignment ::= IDENTIFIER SBRACKET_OPEN xx_index_expr SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 136 */ "xx_let_assignment ::= IDENTIFIER INCR",
 /* 137 */ "xx_let_assignment ::= IDENTIFIER DECR",
 /* 138 */ "xx_index_expr ::= xx_common_expr",
 /* 139 */ "xx_echo_statement ::= ECHO xx_echo_expressions DOTCOMMA",
 /* 140 */ "xx_echo_expressions ::= xx_echo_expressions COMMA xx_echo_expression",
 /* 141 */ "xx_echo_expressions ::= xx_echo_expression",
 /* 142 */ "xx_echo_expression ::= xx_common_expr",
 /* 143 */ "xx_mcall_statement ::= xx_mcall_expr DOTCOMMA",
 /* 144 */ "xx_scall_statement ::= xx_scall_expr DOTCOMMA",
 /* 145 */ "xx_return_statement ::= RETURN xx_common_expr DOTCOMMA",
 /* 146 */ "xx_return_statement ::= RETURN DOTCOMMA",
 /* 147 */ "xx_unset_statement ::= UNSET IDENTIFIER ARROW IDENTIFIER DOTCOMMA",
 /* 148 */ "xx_unset_statement ::= UNSET IDENTIFIER SBRACKET_OPEN IDENTIFIER SBRACKET_CLOSE DOTCOMMA",
 /* 149 */ "xx_unset_statement ::= UNSET IDENTIFIER SBRACKET_OPEN INTEGER SBRACKET_CLOSE DOTCOMMA",
 /* 150 */ "xx_unset_statement ::= UNSET IDENTIFIER SBRACKET_OPEN STRING SBRACKET_CLOSE DOTCOMMA",
 /* 151 */ "xx_throw_statement ::= THROW IDENTIFIER DOTCOMMA",
 /* 152 */ "xx_throw_statement ::= THROW NEW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE DOTCOMMA",
 /* 153 */ "xx_declare_statement ::= TYPE_INTEGER xx_declare_variable_list DOTCOMMA",
 /* 154 */ "xx_declare_statement ::= TYPE_UINTEGER xx_declare_variable_list DOTCOMMA",
 /* 155 */ "xx_declare_statement ::= TYPE_CHAR xx_declare_variable_list DOTCOMMA",
 /* 156 */ "xx_declare_statement ::= TYPE_UCHAR xx_declare_variable_list DOTCOMMA",
 /* 157 */ "xx_declare_statement ::= TYPE_LONG xx_declare_variable_list DOTCOMMA",
 /* 158 */ "xx_declare_statement ::= TYPE_ULONG xx_declare_variable_list DOTCOMMA",
 /* 159 */ "xx_declare_statement ::= TYPE_DOUBLE xx_declare_variable_list DOTCOMMA",
 /* 160 */ "xx_declare_statement ::= TYPE_STRING xx_declare_variable_list DOTCOMMA",
 /* 161 */ "xx_declare_statement ::= TYPE_BOOL xx_declare_variable_list DOTCOMMA",
 /* 162 */ "xx_declare_statement ::= TYPE_VAR xx_declare_variable_list DOTCOMMA",
 /* 163 */ "xx_declare_variable_list ::= xx_declare_variable_list COMMA xx_declare_variable",
 /* 164 */ "xx_declare_variable_list ::= xx_declare_variable",
 /* 165 */ "xx_declare_variable ::= IDENTIFIER",
 /* 166 */ "xx_declare_variable ::= IDENTIFIER ASSIGN xx_literal_expr",
 /* 167 */ "xx_assign_expr ::= xx_common_expr",
 /* 168 */ "xx_common_expr ::= NOT xx_common_expr",
 /* 169 */ "xx_common_expr ::= ISSET xx_isset_expr",
 /* 170 */ "xx_common_expr ::= xx_common_expr EQUALS xx_common_expr",
 /* 171 */ "xx_common_expr ::= xx_common_expr NOTEQUALS xx_common_expr",
 /* 172 */ "xx_common_expr ::= xx_common_expr IDENTICAL xx_common_expr",
 /* 173 */ "xx_common_expr ::= xx_common_expr NOTIDENTICAL xx_common_expr",
 /* 174 */ "xx_common_expr ::= xx_common_expr LESS xx_common_expr",
 /* 175 */ "xx_common_expr ::= xx_common_expr GREATER xx_common_expr",
 /* 176 */ "xx_common_expr ::= xx_common_expr LESSEQUAL xx_common_expr",
 /* 177 */ "xx_common_expr ::= xx_common_expr GREATEREQUAL xx_common_expr",
 /* 178 */ "xx_common_expr ::= PARENTHESES_OPEN xx_common_expr PARENTHESES_CLOSE",
 /* 179 */ "xx_common_expr ::= PARENTHESES_OPEN xx_parameter_type PARENTHESES_CLOSE xx_common_expr",
 /* 180 */ "xx_common_expr ::= IDENTIFIER ARROW IDENTIFIER",
 /* 181 */ "xx_common_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER",
 /* 182 */ "xx_common_expr ::= IDENTIFIER DOUBLECOLON CONSTANT",
 /* 183 */ "xx_common_expr ::= IDENTIFIER SBRACKET_OPEN xx_common_expr SBRACKET_CLOSE",
 /* 184 */ "xx_common_expr ::= xx_common_expr ADD xx_common_expr",
 /* 185 */ "xx_common_expr ::= xx_common_expr SUB xx_common_expr",
 /* 186 */ "xx_common_expr ::= xx_common_expr MUL xx_common_expr",
 /* 187 */ "xx_common_expr ::= xx_common_expr DIV xx_common_expr",
 /* 188 */ "xx_common_expr ::= xx_common_expr MOD xx_common_expr",
 /* 189 */ "xx_common_expr ::= xx_common_expr CONCAT xx_common_expr",
 /* 190 */ "xx_common_expr ::= xx_common_expr AND xx_common_expr",
 /* 191 */ "xx_common_expr ::= xx_common_expr OR xx_common_expr",
 /* 192 */ "xx_common_expr ::= INSTANCEOF xx_common_expr",
 /* 193 */ "xx_common_expr ::= FETCH IDENTIFIER COMMA xx_isset_expr",
 /* 194 */ "xx_common_expr ::= TYPEOF xx_common_expr",
 /* 195 */ "xx_common_expr ::= CONSTANT",
 /* 196 */ "xx_common_expr ::= IDENTIFIER",
 /* 197 */ "xx_common_expr ::= INTEGER",
 /* 198 */ "xx_common_expr ::= STRING",
 /* 199 */ "xx_common_expr ::= CHAR",
 /* 200 */ "xx_common_expr ::= DOUBLE",
 /* 201 */ "xx_common_expr ::= NULL",
 /* 202 */ "xx_common_expr ::= TRUE",
 /* 203 */ "xx_common_expr ::= FALSE",
 /* 204 */ "xx_common_expr ::= SBRACKET_OPEN SBRACKET_CLOSE",
 /* 205 */ "xx_common_expr ::= SBRACKET_OPEN xx_array_list SBRACKET_CLOSE",
 /* 206 */ "xx_common_expr ::= NEW IDENTIFIER",
 /* 207 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 208 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 209 */ "xx_common_expr ::= IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 210 */ "xx_common_expr ::= IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 211 */ "xx_common_expr ::= xx_mcall_expr",
 /* 212 */ "xx_common_expr ::= xx_scall_expr",
 /* 213 */ "xx_scall_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 214 */ "xx_scall_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 215 */ "xx_mcall_expr ::= IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 216 */ "xx_mcall_expr ::= IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 217 */ "xx_call_parameters ::= xx_call_parameters COMMA xx_call_parameter",
 /* 218 */ "xx_call_parameters ::= xx_call_parameter",
 /* 219 */ "xx_call_parameter ::= xx_common_expr",
 /* 220 */ "xx_array_list ::= xx_array_list COMMA xx_array_item",
 /* 221 */ "xx_array_list ::= xx_array_item",
 /* 222 */ "xx_array_item ::= xx_array_key COLON xx_array_value",
 /* 223 */ "xx_array_item ::= xx_array_value",
 /* 224 */ "xx_array_key ::= IDENTIFIER",
 /* 225 */ "xx_array_key ::= STRING",
 /* 226 */ "xx_array_key ::= INTEGER",
 /* 227 */ "xx_array_value ::= xx_literal_expr",
 /* 228 */ "xx_array_value ::= IDENTIFIER",
 /* 229 */ "xx_literal_expr ::= INTEGER",
 /* 230 */ "xx_literal_expr ::= STRING",
 /* 231 */ "xx_literal_expr ::= DOUBLE",
 /* 232 */ "xx_literal_expr ::= NULL",
 /* 233 */ "xx_literal_expr ::= FALSE",
 /* 234 */ "xx_literal_expr ::= TRUE",
 /* 235 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN IDENTIFIER SBRACKET_CLOSE",
 /* 236 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN INTEGER SBRACKET_CLOSE",
 /* 237 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN STRING SBRACKET_CLOSE",
 /* 238 */ "xx_eval_expr ::= xx_common_expr",
 /* 239 */ "xx_comment ::= COMMENT",
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
    case 89:
    case 90:
    case 91:
    case 92:
    case 93:
// 838 "parser.lemon"
{
	/*if ((yypminor->yy0)) {
		if ((yypminor->yy0)->free_flag) {
			efree((yypminor->yy0)->token);
		}
		efree((yypminor->yy0));
	}*/
}
// 2152 "parser.c"
      break;
    case 96:
// 851 "parser.lemon"
{ json_object_put((yypminor->yy103)); }
// 2157 "parser.c"
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
  { 95, 1 },
  { 96, 1 },
  { 97, 2 },
  { 97, 1 },
  { 98, 1 },
  { 98, 1 },
  { 98, 1 },
  { 99, 3 },
  { 100, 5 },
  { 100, 7 },
  { 100, 6 },
  { 100, 8 },
  { 100, 4 },
  { 100, 6 },
  { 100, 5 },
  { 100, 7 },
  { 102, 1 },
  { 102, 1 },
  { 102, 1 },
  { 102, 2 },
  { 102, 2 },
  { 102, 2 },
  { 102, 2 },
  { 102, 3 },
  { 102, 3 },
  { 103, 2 },
  { 103, 1 },
  { 106, 4 },
  { 106, 3 },
  { 106, 6 },
  { 106, 5 },
  { 104, 2 },
  { 104, 1 },
  { 105, 2 },
  { 105, 1 },
  { 109, 6 },
  { 109, 5 },
  { 110, 7 },
  { 110, 8 },
  { 110, 8 },
  { 110, 9 },
  { 110, 8 },
  { 110, 9 },
  { 110, 9 },
  { 110, 10 },
  { 107, 2 },
  { 107, 1 },
  { 113, 1 },
  { 113, 1 },
  { 113, 1 },
  { 113, 1 },
  { 113, 1 },
  { 113, 1 },
  { 113, 1 },
  { 113, 1 },
  { 111, 3 },
  { 111, 1 },
  { 114, 1 },
  { 114, 2 },
  { 114, 2 },
  { 114, 3 },
  { 114, 4 },
  { 114, 4 },
  { 116, 3 },
  { 115, 1 },
  { 115, 1 },
  { 115, 1 },
  { 115, 1 },
  { 115, 1 },
  { 115, 1 },
  { 115, 1 },
  { 115, 1 },
  { 115, 1 },
  { 115, 1 },
  { 115, 1 },
  { 112, 2 },
  { 112, 1 },
  { 117, 1 },
  { 117, 1 },
  { 117, 1 },
  { 117, 1 },
  { 117, 1 },
  { 117, 1 },
  { 117, 1 },
  { 117, 1 },
  { 117, 1 },
  { 117, 1 },
  { 117, 1 },
  { 117, 1 },
  { 117, 1 },
  { 117, 1 },
  { 117, 1 },
  { 117, 1 },
  { 117, 1 },
  { 128, 2 },
  { 129, 2 },
  { 119, 4 },
  { 119, 7 },
  { 119, 5 },
  { 119, 9 },
  { 119, 8 },
  { 119, 8 },
  { 132, 4 },
  { 132, 5 },
  { 135, 2 },
  { 135, 1 },
  { 136, 4 },
  { 136, 3 },
  { 120, 3 },
  { 120, 4 },
  { 130, 4 },
  { 130, 5 },
  { 131, 6 },
  { 131, 7 },
  { 133, 7 },
  { 133, 8 },
  { 133, 9 },
  { 133, 10 },
  { 118, 3 },
  { 138, 3 },
  { 138, 1 },
  { 140, 1 },
  { 140, 1 },
  { 140, 1 },
  { 140, 1 },
  { 140, 1 },
  { 140, 1 },
  { 139, 3 },
  { 139, 5 },
  { 139, 7 },
  { 139, 8 },
  { 139, 5 },
  { 139, 7 },
  { 139, 8 },
  { 139, 5 },
  { 139, 6 },
  { 139, 2 },
  { 139, 2 },
  { 142, 1 },
  { 121, 3 },
  { 143, 3 },
  { 143, 1 },
  { 144, 1 },
  { 123, 2 },
  { 124, 2 },
  { 122, 3 },
  { 122, 2 },
  { 125, 5 },
  { 125, 6 },
  { 125, 6 },
  { 125, 6 },
  { 126, 3 },
  { 126, 7 },
  { 127, 3 },
  { 127, 3 },
  { 127, 3 },
  { 127, 3 },
  { 127, 3 },
  { 127, 3 },
  { 127, 3 },
  { 127, 3 },
  { 127, 3 },
  { 127, 3 },
  { 148, 3 },
  { 148, 1 },
  { 149, 1 },
  { 149, 3 },
  { 141, 1 },
  { 137, 2 },
  { 137, 2 },
  { 137, 3 },
  { 137, 3 },
  { 137, 3 },
  { 137, 3 },
  { 137, 3 },
  { 137, 3 },
  { 137, 3 },
  { 137, 3 },
  { 137, 3 },
  { 137, 4 },
  { 137, 3 },
  { 137, 3 },
  { 137, 3 },
  { 137, 4 },
  { 137, 3 },
  { 137, 3 },
  { 137, 3 },
  { 137, 3 },
  { 137, 3 },
  { 137, 3 },
  { 137, 3 },
  { 137, 3 },
  { 137, 2 },
  { 137, 4 },
  { 137, 2 },
  { 137, 1 },
  { 137, 1 },
  { 137, 1 },
  { 137, 1 },
  { 137, 1 },
  { 137, 1 },
  { 137, 1 },
  { 137, 1 },
  { 137, 1 },
  { 137, 2 },
  { 137, 3 },
  { 137, 2 },
  { 137, 4 },
  { 137, 5 },
  { 137, 4 },
  { 137, 3 },
  { 137, 1 },
  { 137, 1 },
  { 146, 6 },
  { 146, 5 },
  { 145, 6 },
  { 145, 5 },
  { 147, 3 },
  { 147, 1 },
  { 152, 1 },
  { 151, 3 },
  { 151, 1 },
  { 153, 3 },
  { 153, 1 },
  { 154, 1 },
  { 154, 1 },
  { 154, 1 },
  { 155, 1 },
  { 155, 1 },
  { 108, 1 },
  { 108, 1 },
  { 108, 1 },
  { 108, 1 },
  { 108, 1 },
  { 108, 1 },
  { 150, 4 },
  { 150, 4 },
  { 150, 4 },
  { 134, 1 },
  { 101, 1 },
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
// 847 "parser.lemon"
{
	status->ret = yymsp[0].minor.yy103;
}
// 2614 "parser.c"
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
      case 93:
      case 138:
      case 167:
      case 211:
      case 212:
      case 219:
      case 227:
      case 238:
// 853 "parser.lemon"
{
	yygotominor.yy103 = yymsp[0].minor.yy103;
}
// 2648 "parser.c"
        break;
      case 2:
      case 25:
      case 31:
      case 33:
      case 45:
      case 75:
      case 104:
// 857 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_list(yymsp[-1].minor.yy103, yymsp[0].minor.yy103);
}
// 2661 "parser.c"
        break;
      case 3:
      case 26:
      case 32:
      case 34:
      case 46:
      case 56:
      case 76:
      case 105:
      case 120:
      case 141:
      case 164:
      case 218:
      case 221:
// 861 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_list(NULL, yymsp[0].minor.yy103);
}
// 2680 "parser.c"
        break;
      case 7:
// 877 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_namespace(yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 2689 "parser.c"
        break;
      case 8:
// 881 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[-1].minor.yy103, 0, NULL, status->scanner_state);
  yy_destructor(33,&yymsp[-4].minor);
  yy_destructor(34,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 2699 "parser.c"
        break;
      case 9:
// 885 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class(yymsp[-5].minor.yy0, yymsp[-1].minor.yy103, 0, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(33,&yymsp[-6].minor);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(34,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 2710 "parser.c"
        break;
      case 10:
// 889 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[-1].minor.yy103, 1, NULL, status->scanner_state);
  yy_destructor(37,&yymsp[-5].minor);
  yy_destructor(33,&yymsp[-4].minor);
  yy_destructor(34,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 2721 "parser.c"
        break;
      case 11:
// 893 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class(yymsp[-5].minor.yy0, yymsp[-1].minor.yy103, 1, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(37,&yymsp[-7].minor);
  yy_destructor(33,&yymsp[-6].minor);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(34,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 2733 "parser.c"
        break;
      case 12:
// 897 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class(yymsp[-2].minor.yy0, NULL, 0, NULL, status->scanner_state);
  yy_destructor(33,&yymsp[-3].minor);
  yy_destructor(34,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 2743 "parser.c"
        break;
      case 13:
// 901 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class(yymsp[-4].minor.yy0, NULL, 0, yymsp[-2].minor.yy0, status->scanner_state);
  yy_destructor(33,&yymsp[-5].minor);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(34,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 2754 "parser.c"
        break;
      case 14:
// 905 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class(yymsp[-2].minor.yy0, NULL, 1, NULL, status->scanner_state);
  yy_destructor(37,&yymsp[-4].minor);
  yy_destructor(33,&yymsp[-3].minor);
  yy_destructor(34,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 2765 "parser.c"
        break;
      case 15:
// 909 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class(yymsp[-4].minor.yy0, NULL, 1, yymsp[-2].minor.yy0, status->scanner_state);
  yy_destructor(37,&yymsp[-6].minor);
  yy_destructor(33,&yymsp[-5].minor);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(34,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 2777 "parser.c"
        break;
      case 16:
// 913 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_definition(yymsp[0].minor.yy103, NULL, NULL, status->scanner_state);
}
// 2784 "parser.c"
        break;
      case 17:
// 917 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_definition(NULL, NULL, yymsp[0].minor.yy103, status->scanner_state);
}
// 2791 "parser.c"
        break;
      case 18:
// 921 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_definition(NULL, yymsp[0].minor.yy103, NULL, status->scanner_state);
}
// 2798 "parser.c"
        break;
      case 19:
// 925 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_definition(yymsp[-1].minor.yy103, yymsp[0].minor.yy103, NULL, status->scanner_state);
}
// 2805 "parser.c"
        break;
      case 20:
// 929 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_definition(yymsp[-1].minor.yy103, NULL, yymsp[0].minor.yy103, status->scanner_state);
}
// 2812 "parser.c"
        break;
      case 21:
// 933 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_definition(yymsp[0].minor.yy103, NULL, yymsp[-1].minor.yy103, status->scanner_state);
}
// 2819 "parser.c"
        break;
      case 22:
// 937 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_definition(NULL, yymsp[0].minor.yy103, yymsp[-1].minor.yy103, status->scanner_state);
}
// 2826 "parser.c"
        break;
      case 23:
// 941 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_definition(yymsp[-2].minor.yy103, yymsp[0].minor.yy103, yymsp[-1].minor.yy103, status->scanner_state);
}
// 2833 "parser.c"
        break;
      case 24:
// 945 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_definition(yymsp[-1].minor.yy103, yymsp[0].minor.yy103, yymsp[-2].minor.yy103, status->scanner_state);
}
// 2840 "parser.c"
        break;
      case 27:
// 957 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_property(yymsp[-2].minor.yy103, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(32,&yymsp[0].minor);
}
// 2848 "parser.c"
        break;
      case 28:
// 961 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_property(yymsp[-2].minor.yy103, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(32,&yymsp[0].minor);
}
// 2856 "parser.c"
        break;
      case 29:
// 965 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_property(yymsp[-4].minor.yy103, yymsp[-3].minor.yy0, yymsp[-1].minor.yy103, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(39,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 2865 "parser.c"
        break;
      case 30:
// 969 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_property(yymsp[-4].minor.yy103, yymsp[-3].minor.yy0, yymsp[-1].minor.yy103, NULL, status->scanner_state);
  yy_destructor(39,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 2874 "parser.c"
        break;
      case 35:
// 989 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy103, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(40,&yymsp[-4].minor);
  yy_destructor(39,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 2884 "parser.c"
        break;
      case 36:
// 993 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy103, NULL, status->scanner_state);
  yy_destructor(40,&yymsp[-4].minor);
  yy_destructor(39,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 2894 "parser.c"
        break;
      case 37:
// 997 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_method(yymsp[-6].minor.yy103, yymsp[-4].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(42,&yymsp[-5].minor);
  yy_destructor(43,&yymsp[-3].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(34,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 2906 "parser.c"
        break;
      case 38:
// 1001 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_method(yymsp[-7].minor.yy103, yymsp[-5].minor.yy0, yymsp[-3].minor.yy103, NULL, NULL, status->scanner_state);
  yy_destructor(42,&yymsp[-6].minor);
  yy_destructor(43,&yymsp[-4].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(34,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 2918 "parser.c"
        break;
      case 39:
// 1005 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_method(yymsp[-7].minor.yy103, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy103, NULL, status->scanner_state);
  yy_destructor(42,&yymsp[-6].minor);
  yy_destructor(43,&yymsp[-4].minor);
  yy_destructor(29,&yymsp[-3].minor);
  yy_destructor(34,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 2930 "parser.c"
        break;
      case 40:
// 1009 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_method(yymsp[-8].minor.yy103, yymsp[-6].minor.yy0, yymsp[-4].minor.yy103, yymsp[-1].minor.yy103, NULL, status->scanner_state);
  yy_destructor(42,&yymsp[-7].minor);
  yy_destructor(43,&yymsp[-5].minor);
  yy_destructor(29,&yymsp[-3].minor);
  yy_destructor(34,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 2942 "parser.c"
        break;
      case 41:
// 1013 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_method(yymsp[-6].minor.yy103, yymsp[-4].minor.yy0, NULL, NULL, yymsp[-7].minor.yy0, status->scanner_state);
  yy_destructor(42,&yymsp[-5].minor);
  yy_destructor(43,&yymsp[-3].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(34,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 2954 "parser.c"
        break;
      case 42:
// 1017 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_method(yymsp[-7].minor.yy103, yymsp[-5].minor.yy0, yymsp[-3].minor.yy103, NULL, yymsp[-8].minor.yy0, status->scanner_state);
  yy_destructor(42,&yymsp[-6].minor);
  yy_destructor(43,&yymsp[-4].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(34,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 2966 "parser.c"
        break;
      case 43:
// 1021 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_method(yymsp[-7].minor.yy103, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy103, yymsp[-8].minor.yy0, status->scanner_state);
  yy_destructor(42,&yymsp[-6].minor);
  yy_destructor(43,&yymsp[-4].minor);
  yy_destructor(29,&yymsp[-3].minor);
  yy_destructor(34,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 2978 "parser.c"
        break;
      case 44:
// 1025 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_method(yymsp[-8].minor.yy103, yymsp[-6].minor.yy0, yymsp[-4].minor.yy103, yymsp[-1].minor.yy103, yymsp[-9].minor.yy0, status->scanner_state);
  yy_destructor(42,&yymsp[-7].minor);
  yy_destructor(43,&yymsp[-5].minor);
  yy_destructor(29,&yymsp[-3].minor);
  yy_destructor(34,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 2990 "parser.c"
        break;
      case 47:
// 1037 "parser.lemon"
{
	yygotominor.yy103 = json_object_new_string("public");
  yy_destructor(1,&yymsp[0].minor);
}
// 2998 "parser.c"
        break;
      case 48:
// 1041 "parser.lemon"
{
	yygotominor.yy103 = json_object_new_string("protected");
  yy_destructor(2,&yymsp[0].minor);
}
// 3006 "parser.c"
        break;
      case 49:
// 1045 "parser.lemon"
{
	yygotominor.yy103 = json_object_new_string("private");
  yy_destructor(4,&yymsp[0].minor);
}
// 3014 "parser.c"
        break;
      case 50:
// 1049 "parser.lemon"
{
	yygotominor.yy103 = json_object_new_string("static");
  yy_destructor(3,&yymsp[0].minor);
}
// 3022 "parser.c"
        break;
      case 51:
// 1053 "parser.lemon"
{
	yygotominor.yy103 = json_object_new_string("scoped");
  yy_destructor(5,&yymsp[0].minor);
}
// 3030 "parser.c"
        break;
      case 52:
// 1057 "parser.lemon"
{
	yygotominor.yy103 = json_object_new_string("inline");
  yy_destructor(44,&yymsp[0].minor);
}
// 3038 "parser.c"
        break;
      case 53:
// 1061 "parser.lemon"
{
	yygotominor.yy103 = json_object_new_string("abstract");
  yy_destructor(37,&yymsp[0].minor);
}
// 3046 "parser.c"
        break;
      case 54:
// 1065 "parser.lemon"
{
	yygotominor.yy103 = json_object_new_string("final");
  yy_destructor(45,&yymsp[0].minor);
}
// 3054 "parser.c"
        break;
      case 55:
      case 119:
      case 140:
      case 163:
      case 217:
      case 220:
// 1069 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_list(yymsp[-2].minor.yy103, yymsp[0].minor.yy103);
  yy_destructor(6,&yymsp[-1].minor);
}
// 3067 "parser.c"
        break;
      case 57:
// 1077 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_parameter(NULL, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3074 "parser.c"
        break;
      case 58:
// 1081 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_parameter(yymsp[-1].minor.yy103, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3081 "parser.c"
        break;
      case 59:
// 1085 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_parameter(NULL, yymsp[-1].minor.yy103, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3088 "parser.c"
        break;
      case 60:
// 1089 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_parameter(NULL, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy103, status->scanner_state);
  yy_destructor(39,&yymsp[-1].minor);
}
// 3096 "parser.c"
        break;
      case 61:
// 1093 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_parameter(yymsp[-3].minor.yy103, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy103, status->scanner_state);
  yy_destructor(39,&yymsp[-1].minor);
}
// 3104 "parser.c"
        break;
      case 62:
// 1097 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_parameter(NULL, yymsp[-3].minor.yy103, yymsp[-2].minor.yy0, yymsp[0].minor.yy103, status->scanner_state);
  yy_destructor(39,&yymsp[-1].minor);
}
// 3112 "parser.c"
        break;
      case 63:
// 1101 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(11,&yymsp[-2].minor);
  yy_destructor(12,&yymsp[0].minor);
}
// 3121 "parser.c"
        break;
      case 64:
// 1105 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_type(XX_TYPE_INTEGER);
  yy_destructor(46,&yymsp[0].minor);
}
// 3129 "parser.c"
        break;
      case 65:
// 1109 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_type(XX_TYPE_UINTEGER);
  yy_destructor(47,&yymsp[0].minor);
}
// 3137 "parser.c"
        break;
      case 66:
// 1113 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_type(XX_TYPE_LONG);
  yy_destructor(48,&yymsp[0].minor);
}
// 3145 "parser.c"
        break;
      case 67:
// 1117 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_type(XX_TYPE_ULONG);
  yy_destructor(49,&yymsp[0].minor);
}
// 3153 "parser.c"
        break;
      case 68:
// 1121 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_type(XX_TYPE_CHAR);
  yy_destructor(50,&yymsp[0].minor);
}
// 3161 "parser.c"
        break;
      case 69:
// 1125 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_type(XX_TYPE_UCHAR);
  yy_destructor(51,&yymsp[0].minor);
}
// 3169 "parser.c"
        break;
      case 70:
// 1129 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_type(XX_TYPE_DOUBLE);
  yy_destructor(52,&yymsp[0].minor);
}
// 3177 "parser.c"
        break;
      case 71:
// 1133 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_type(XX_TYPE_BOOL);
  yy_destructor(53,&yymsp[0].minor);
}
// 3185 "parser.c"
        break;
      case 72:
// 1137 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_type(XX_TYPE_STRING);
  yy_destructor(54,&yymsp[0].minor);
}
// 3193 "parser.c"
        break;
      case 73:
// 1141 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_type(XX_TYPE_ARRAY);
  yy_destructor(55,&yymsp[0].minor);
}
// 3201 "parser.c"
        break;
      case 74:
// 1145 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_type(XX_TYPE_VAR);
  yy_destructor(56,&yymsp[0].minor);
}
// 3209 "parser.c"
        break;
      case 94:
// 1225 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_break_statement(status->scanner_state);
  yy_destructor(57,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3218 "parser.c"
        break;
      case 95:
// 1229 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_continue_statement(status->scanner_state);
  yy_destructor(58,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3227 "parser.c"
        break;
      case 96:
// 1233 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_if_statement(yymsp[-2].minor.yy103, NULL, NULL, status->scanner_state);
  yy_destructor(59,&yymsp[-3].minor);
  yy_destructor(34,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3237 "parser.c"
        break;
      case 97:
// 1237 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_if_statement(yymsp[-5].minor.yy103, NULL, NULL, status->scanner_state);
  yy_destructor(59,&yymsp[-6].minor);
  yy_destructor(34,&yymsp[-4].minor);
  yy_destructor(35,&yymsp[-3].minor);
  yy_destructor(60,&yymsp[-2].minor);
  yy_destructor(34,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3250 "parser.c"
        break;
      case 98:
// 1241 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_if_statement(yymsp[-3].minor.yy103, yymsp[-1].minor.yy103, NULL, status->scanner_state);
  yy_destructor(59,&yymsp[-4].minor);
  yy_destructor(34,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3260 "parser.c"
        break;
      case 99:
// 1245 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_if_statement(yymsp[-7].minor.yy103, yymsp[-5].minor.yy103, yymsp[-1].minor.yy103, status->scanner_state);
  yy_destructor(59,&yymsp[-8].minor);
  yy_destructor(34,&yymsp[-6].minor);
  yy_destructor(35,&yymsp[-4].minor);
  yy_destructor(60,&yymsp[-3].minor);
  yy_destructor(34,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3273 "parser.c"
        break;
      case 100:
// 1249 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_if_statement(yymsp[-6].minor.yy103, yymsp[-4].minor.yy103, NULL, status->scanner_state);
  yy_destructor(59,&yymsp[-7].minor);
  yy_destructor(34,&yymsp[-5].minor);
  yy_destructor(35,&yymsp[-3].minor);
  yy_destructor(60,&yymsp[-2].minor);
  yy_destructor(34,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3286 "parser.c"
        break;
      case 101:
// 1253 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_if_statement(yymsp[-6].minor.yy103, NULL, yymsp[-1].minor.yy103, status->scanner_state);
  yy_destructor(59,&yymsp[-7].minor);
  yy_destructor(34,&yymsp[-5].minor);
  yy_destructor(35,&yymsp[-4].minor);
  yy_destructor(60,&yymsp[-3].minor);
  yy_destructor(34,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3299 "parser.c"
        break;
      case 102:
// 1257 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_switch_statement(yymsp[-2].minor.yy103, NULL, status->scanner_state);
  yy_destructor(61,&yymsp[-3].minor);
  yy_destructor(34,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3309 "parser.c"
        break;
      case 103:
// 1261 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_switch_statement(yymsp[-3].minor.yy103, yymsp[-1].minor.yy103, status->scanner_state);
  yy_destructor(61,&yymsp[-4].minor);
  yy_destructor(34,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3319 "parser.c"
        break;
      case 106:
// 1273 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_case_clause(yymsp[-2].minor.yy103, yymsp[0].minor.yy103, status->scanner_state);
  yy_destructor(62,&yymsp[-3].minor);
  yy_destructor(63,&yymsp[-1].minor);
}
// 3328 "parser.c"
        break;
      case 107:
// 1277 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_case_clause(NULL, yymsp[0].minor.yy103, status->scanner_state);
  yy_destructor(64,&yymsp[-2].minor);
  yy_destructor(63,&yymsp[-1].minor);
}
// 3337 "parser.c"
        break;
      case 108:
// 1281 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_loop_statement(NULL, status->scanner_state);
  yy_destructor(65,&yymsp[-2].minor);
  yy_destructor(34,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3347 "parser.c"
        break;
      case 109:
// 1285 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_loop_statement(yymsp[-1].minor.yy103, status->scanner_state);
  yy_destructor(65,&yymsp[-3].minor);
  yy_destructor(34,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3357 "parser.c"
        break;
      case 110:
// 1289 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_while_statement(yymsp[-2].minor.yy103, NULL, status->scanner_state);
  yy_destructor(66,&yymsp[-3].minor);
  yy_destructor(34,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3367 "parser.c"
        break;
      case 111:
// 1293 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_while_statement(yymsp[-3].minor.yy103, yymsp[-1].minor.yy103, status->scanner_state);
  yy_destructor(66,&yymsp[-4].minor);
  yy_destructor(34,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3377 "parser.c"
        break;
      case 112:
// 1297 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_do_while_statement(yymsp[-1].minor.yy103, NULL, status->scanner_state);
  yy_destructor(67,&yymsp[-5].minor);
  yy_destructor(34,&yymsp[-4].minor);
  yy_destructor(35,&yymsp[-3].minor);
  yy_destructor(66,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3389 "parser.c"
        break;
      case 113:
// 1301 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_do_while_statement(yymsp[-1].minor.yy103, yymsp[-4].minor.yy103, status->scanner_state);
  yy_destructor(67,&yymsp[-6].minor);
  yy_destructor(34,&yymsp[-5].minor);
  yy_destructor(35,&yymsp[-3].minor);
  yy_destructor(66,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3401 "parser.c"
        break;
      case 114:
// 1305 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_for_statement(yymsp[-3].minor.yy103, NULL, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy103, status->scanner_state);
  yy_destructor(68,&yymsp[-6].minor);
  yy_destructor(69,&yymsp[-4].minor);
  yy_destructor(34,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3412 "parser.c"
        break;
      case 115:
// 1309 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_for_statement(yymsp[-3].minor.yy103, NULL, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy103, status->scanner_state);
  yy_destructor(68,&yymsp[-7].minor);
  yy_destructor(69,&yymsp[-5].minor);
  yy_destructor(70,&yymsp[-4].minor);
  yy_destructor(34,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3424 "parser.c"
        break;
      case 116:
// 1313 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_for_statement(yymsp[-3].minor.yy103, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy103, status->scanner_state);
  yy_destructor(68,&yymsp[-8].minor);
  yy_destructor(6,&yymsp[-6].minor);
  yy_destructor(69,&yymsp[-4].minor);
  yy_destructor(34,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3436 "parser.c"
        break;
      case 117:
// 1317 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_for_statement(yymsp[-3].minor.yy103, yymsp[-8].minor.yy0, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy103, status->scanner_state);
  yy_destructor(68,&yymsp[-9].minor);
  yy_destructor(6,&yymsp[-7].minor);
  yy_destructor(69,&yymsp[-5].minor);
  yy_destructor(70,&yymsp[-4].minor);
  yy_destructor(34,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3449 "parser.c"
        break;
      case 118:
// 1321 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_let_statement(yymsp[-1].minor.yy103, status->scanner_state);
  yy_destructor(71,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3458 "parser.c"
        break;
      case 121:
// 1334 "parser.lemon"
{
	yygotominor.yy103 = json_object_new_string("assign");
  yy_destructor(39,&yymsp[0].minor);
}
// 3466 "parser.c"
        break;
      case 122:
// 1339 "parser.lemon"
{
	yygotominor.yy103 = json_object_new_string("add-assign");
  yy_destructor(72,&yymsp[0].minor);
}
// 3474 "parser.c"
        break;
      case 123:
// 1344 "parser.lemon"
{
	yygotominor.yy103 = json_object_new_string("sub-assign");
  yy_destructor(73,&yymsp[0].minor);
}
// 3482 "parser.c"
        break;
      case 124:
// 1348 "parser.lemon"
{
	yygotominor.yy103 = json_object_new_string("mult-assign");
  yy_destructor(74,&yymsp[0].minor);
}
// 3490 "parser.c"
        break;
      case 125:
// 1352 "parser.lemon"
{
	yygotominor.yy103 = json_object_new_string("div-assign");
  yy_destructor(75,&yymsp[0].minor);
}
// 3498 "parser.c"
        break;
      case 126:
// 1356 "parser.lemon"
{
	yygotominor.yy103 = json_object_new_string("concat-assign");
  yy_destructor(76,&yymsp[0].minor);
}
// 3506 "parser.c"
        break;
      case 127:
// 1361 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_let_assignment("variable", yymsp[-1].minor.yy103, yymsp[-2].minor.yy0, NULL, NULL, yymsp[0].minor.yy103, status->scanner_state);
}
// 3513 "parser.c"
        break;
      case 128:
// 1366 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_let_assignment("object-property", yymsp[-1].minor.yy103, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy103, status->scanner_state);
  yy_destructor(77,&yymsp[-3].minor);
}
// 3521 "parser.c"
        break;
      case 129:
// 1371 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_let_assignment("object-property-append", yymsp[-1].minor.yy103, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy103, status->scanner_state);
  yy_destructor(77,&yymsp[-5].minor);
  yy_destructor(78,&yymsp[-3].minor);
  yy_destructor(79,&yymsp[-2].minor);
}
// 3531 "parser.c"
        break;
      case 130:
// 1376 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_let_assignment("object-property-array-index", yymsp[-1].minor.yy103, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy103, yymsp[0].minor.yy103, status->scanner_state);
  yy_destructor(77,&yymsp[-6].minor);
  yy_destructor(78,&yymsp[-4].minor);
  yy_destructor(79,&yymsp[-2].minor);
}
// 3541 "parser.c"
        break;
      case 131:
// 1381 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_let_assignment("static-property", yymsp[-1].minor.yy103, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy103, status->scanner_state);
  yy_destructor(80,&yymsp[-3].minor);
}
// 3549 "parser.c"
        break;
      case 132:
// 1386 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_let_assignment("static-property-append", yymsp[-1].minor.yy103, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy103, status->scanner_state);
  yy_destructor(80,&yymsp[-5].minor);
  yy_destructor(78,&yymsp[-3].minor);
  yy_destructor(79,&yymsp[-2].minor);
}
// 3559 "parser.c"
        break;
      case 133:
// 1391 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_let_assignment("static-property-array-index", yymsp[-1].minor.yy103, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy103, yymsp[0].minor.yy103, status->scanner_state);
  yy_destructor(80,&yymsp[-6].minor);
  yy_destructor(78,&yymsp[-4].minor);
  yy_destructor(79,&yymsp[-2].minor);
}
// 3569 "parser.c"
        break;
      case 134:
// 1396 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_let_assignment("variable-append", yymsp[-1].minor.yy103, yymsp[-4].minor.yy0, NULL, NULL, yymsp[0].minor.yy103, status->scanner_state);
  yy_destructor(78,&yymsp[-3].minor);
  yy_destructor(79,&yymsp[-2].minor);
}
// 3578 "parser.c"
        break;
      case 135:
// 1401 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_let_assignment("array-index", yymsp[-1].minor.yy103, yymsp[-5].minor.yy0, NULL, yymsp[-3].minor.yy103, yymsp[0].minor.yy103, status->scanner_state);
  yy_destructor(78,&yymsp[-4].minor);
  yy_destructor(79,&yymsp[-2].minor);
}
// 3587 "parser.c"
        break;
      case 136:
// 1406 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_let_assignment("incr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(81,&yymsp[0].minor);
}
// 3595 "parser.c"
        break;
      case 137:
// 1411 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_let_assignment("decr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(82,&yymsp[0].minor);
}
// 3603 "parser.c"
        break;
      case 139:
// 1419 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_echo_statement(yymsp[-1].minor.yy103, status->scanner_state);
  yy_destructor(83,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3612 "parser.c"
        break;
      case 142:
// 1431 "parser.lemon"
{
	yygotominor.yy103 = yymsp[0].minor.yy103;;
}
// 3619 "parser.c"
        break;
      case 143:
// 1436 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_mcall_statement(yymsp[-1].minor.yy103, status->scanner_state);
  yy_destructor(32,&yymsp[0].minor);
}
// 3627 "parser.c"
        break;
      case 144:
// 1441 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_scall_statement(yymsp[-1].minor.yy103, status->scanner_state);
  yy_destructor(32,&yymsp[0].minor);
}
// 3635 "parser.c"
        break;
      case 145:
// 1446 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_return_statement(yymsp[-1].minor.yy103, status->scanner_state);
  yy_destructor(84,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3644 "parser.c"
        break;
      case 146:
// 1451 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_return_statement(NULL, status->scanner_state);
  yy_destructor(84,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3653 "parser.c"
        break;
      case 147:
// 1456 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(85,&yymsp[-4].minor);
  yy_destructor(77,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3667 "parser.c"
        break;
      case 148:
// 1465 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(85,&yymsp[-5].minor);
  yy_destructor(78,&yymsp[-3].minor);
  yy_destructor(79,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3682 "parser.c"
        break;
      case 149:
// 1474 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_INTEGER, yymsp[-2].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(85,&yymsp[-5].minor);
  yy_destructor(78,&yymsp[-3].minor);
  yy_destructor(79,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3697 "parser.c"
        break;
      case 150:
// 1483 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_STRING, yymsp[-2].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(85,&yymsp[-5].minor);
  yy_destructor(78,&yymsp[-3].minor);
  yy_destructor(79,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3712 "parser.c"
        break;
      case 151:
// 1492 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_throw_exception(yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(88,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3721 "parser.c"
        break;
      case 152:
// 1497 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_throw_exception(yymsp[-4].minor.yy0, yymsp[-2].minor.yy103, status->scanner_state);
  yy_destructor(88,&yymsp[-6].minor);
  yy_destructor(27,&yymsp[-5].minor);
  yy_destructor(43,&yymsp[-3].minor);
  yy_destructor(29,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3733 "parser.c"
        break;
      case 153:
// 1501 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_declare_statement(XX_T_TYPE_INTEGER, yymsp[-1].minor.yy103, status->scanner_state);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3742 "parser.c"
        break;
      case 154:
// 1505 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_declare_statement(XX_T_TYPE_UINTEGER, yymsp[-1].minor.yy103, status->scanner_state);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3751 "parser.c"
        break;
      case 155:
// 1509 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_declare_statement(XX_T_TYPE_CHAR, yymsp[-1].minor.yy103, status->scanner_state);
  yy_destructor(50,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3760 "parser.c"
        break;
      case 156:
// 1513 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_declare_statement(XX_T_TYPE_UCHAR, yymsp[-1].minor.yy103, status->scanner_state);
  yy_destructor(51,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3769 "parser.c"
        break;
      case 157:
// 1517 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_declare_statement(XX_T_TYPE_LONG, yymsp[-1].minor.yy103, status->scanner_state);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3778 "parser.c"
        break;
      case 158:
// 1521 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_declare_statement(XX_T_TYPE_ULONG, yymsp[-1].minor.yy103, status->scanner_state);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3787 "parser.c"
        break;
      case 159:
// 1525 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_declare_statement(XX_T_TYPE_DOUBLE, yymsp[-1].minor.yy103, status->scanner_state);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3796 "parser.c"
        break;
      case 160:
// 1529 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_declare_statement(XX_T_TYPE_STRING, yymsp[-1].minor.yy103, status->scanner_state);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3805 "parser.c"
        break;
      case 161:
// 1533 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_declare_statement(XX_T_TYPE_BOOL, yymsp[-1].minor.yy103, status->scanner_state);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3814 "parser.c"
        break;
      case 162:
// 1537 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_declare_statement(XX_T_TYPE_VAR, yymsp[-1].minor.yy103, status->scanner_state);
  yy_destructor(56,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3823 "parser.c"
        break;
      case 165:
// 1549 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_declare_variable(yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3830 "parser.c"
        break;
      case 166:
// 1553 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_declare_variable(yymsp[-2].minor.yy0, yymsp[0].minor.yy103, status->scanner_state);
  yy_destructor(39,&yymsp[-1].minor);
}
// 3838 "parser.c"
        break;
      case 168:
// 1561 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("not", yymsp[0].minor.yy103, NULL, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-1].minor);
}
// 3846 "parser.c"
        break;
      case 169:
// 1565 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("isset", yymsp[0].minor.yy103, NULL, NULL, status->scanner_state);
  yy_destructor(23,&yymsp[-1].minor);
}
// 3854 "parser.c"
        break;
      case 170:
// 1569 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("equals", yymsp[-2].minor.yy103, yymsp[0].minor.yy103, NULL, status->scanner_state);
  yy_destructor(9,&yymsp[-1].minor);
}
// 3862 "parser.c"
        break;
      case 171:
// 1573 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("not-equals", yymsp[-2].minor.yy103, yymsp[0].minor.yy103, NULL, status->scanner_state);
  yy_destructor(16,&yymsp[-1].minor);
}
// 3870 "parser.c"
        break;
      case 172:
// 1577 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("identical", yymsp[-2].minor.yy103, yymsp[0].minor.yy103, NULL, status->scanner_state);
  yy_destructor(10,&yymsp[-1].minor);
}
// 3878 "parser.c"
        break;
      case 173:
// 1581 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("not-identical", yymsp[-2].minor.yy103, yymsp[0].minor.yy103, NULL, status->scanner_state);
  yy_destructor(15,&yymsp[-1].minor);
}
// 3886 "parser.c"
        break;
      case 174:
// 1585 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("less", yymsp[-2].minor.yy103, yymsp[0].minor.yy103, NULL, status->scanner_state);
  yy_destructor(11,&yymsp[-1].minor);
}
// 3894 "parser.c"
        break;
      case 175:
// 1589 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("greater", yymsp[-2].minor.yy103, yymsp[0].minor.yy103, NULL, status->scanner_state);
  yy_destructor(12,&yymsp[-1].minor);
}
// 3902 "parser.c"
        break;
      case 176:
// 1593 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("less-equal", yymsp[-2].minor.yy103, yymsp[0].minor.yy103, NULL, status->scanner_state);
  yy_destructor(13,&yymsp[-1].minor);
}
// 3910 "parser.c"
        break;
      case 177:
// 1597 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("greater-equal", yymsp[-2].minor.yy103, yymsp[0].minor.yy103, NULL, status->scanner_state);
  yy_destructor(14,&yymsp[-1].minor);
}
// 3918 "parser.c"
        break;
      case 178:
// 1601 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("list", yymsp[-1].minor.yy103, NULL, NULL, status->scanner_state);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 3927 "parser.c"
        break;
      case 179:
// 1605 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("cast", yymsp[-2].minor.yy103, yymsp[0].minor.yy103, NULL, status->scanner_state);
  yy_destructor(43,&yymsp[-3].minor);
  yy_destructor(29,&yymsp[-1].minor);
}
// 3936 "parser.c"
        break;
      case 180:
// 1609 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(77,&yymsp[-1].minor);
}
// 3944 "parser.c"
        break;
      case 181:
// 1613 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("static-property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(80,&yymsp[-1].minor);
}
// 3952 "parser.c"
        break;
      case 182:
// 1617 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("static-constant-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(80,&yymsp[-1].minor);
}
// 3960 "parser.c"
        break;
      case 183:
// 1622 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), yymsp[-1].minor.yy103, NULL, status->scanner_state);
  yy_destructor(78,&yymsp[-2].minor);
  yy_destructor(79,&yymsp[0].minor);
}
// 3969 "parser.c"
        break;
      case 184:
// 1627 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("add", yymsp[-2].minor.yy103, yymsp[0].minor.yy103, NULL, status->scanner_state);
  yy_destructor(17,&yymsp[-1].minor);
}
// 3977 "parser.c"
        break;
      case 185:
// 1632 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("sub", yymsp[-2].minor.yy103, yymsp[0].minor.yy103, NULL, status->scanner_state);
  yy_destructor(18,&yymsp[-1].minor);
}
// 3985 "parser.c"
        break;
      case 186:
// 1637 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("mul", yymsp[-2].minor.yy103, yymsp[0].minor.yy103, NULL, status->scanner_state);
  yy_destructor(20,&yymsp[-1].minor);
}
// 3993 "parser.c"
        break;
      case 187:
// 1642 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("div", yymsp[-2].minor.yy103, yymsp[0].minor.yy103, NULL, status->scanner_state);
  yy_destructor(21,&yymsp[-1].minor);
}
// 4001 "parser.c"
        break;
      case 188:
// 1647 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("mod", yymsp[-2].minor.yy103, yymsp[0].minor.yy103, NULL, status->scanner_state);
  yy_destructor(22,&yymsp[-1].minor);
}
// 4009 "parser.c"
        break;
      case 189:
// 1652 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("concat", yymsp[-2].minor.yy103, yymsp[0].minor.yy103, NULL, status->scanner_state);
  yy_destructor(19,&yymsp[-1].minor);
}
// 4017 "parser.c"
        break;
      case 190:
// 1657 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("and", yymsp[-2].minor.yy103, yymsp[0].minor.yy103, NULL, status->scanner_state);
  yy_destructor(7,&yymsp[-1].minor);
}
// 4025 "parser.c"
        break;
      case 191:
// 1662 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("or", yymsp[-2].minor.yy103, yymsp[0].minor.yy103, NULL, status->scanner_state);
  yy_destructor(8,&yymsp[-1].minor);
}
// 4033 "parser.c"
        break;
      case 192:
// 1667 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("instanceof", yymsp[0].minor.yy103, NULL, NULL, status->scanner_state);
  yy_destructor(26,&yymsp[-1].minor);
}
// 4041 "parser.c"
        break;
      case 193:
// 1672 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("fetch", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy103, NULL, status->scanner_state);
  yy_destructor(24,&yymsp[-3].minor);
  yy_destructor(6,&yymsp[-1].minor);
}
// 4050 "parser.c"
        break;
      case 194:
// 1677 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("typeof", yymsp[0].minor.yy103, NULL, NULL, status->scanner_state);
  yy_destructor(25,&yymsp[-1].minor);
}
// 4058 "parser.c"
        break;
      case 195:
// 1682 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_literal(XX_T_CONSTANT, yymsp[0].minor.yy0, status->scanner_state);
}
// 4065 "parser.c"
        break;
      case 196:
      case 224:
      case 228:
// 1687 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state);
}
// 4074 "parser.c"
        break;
      case 197:
      case 226:
      case 229:
// 1692 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_literal(XX_T_INTEGER, yymsp[0].minor.yy0, status->scanner_state);
}
// 4083 "parser.c"
        break;
      case 198:
      case 225:
      case 230:
// 1697 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_literal(XX_T_STRING, yymsp[0].minor.yy0, status->scanner_state);
}
// 4092 "parser.c"
        break;
      case 199:
// 1702 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_literal(XX_T_CHAR, yymsp[0].minor.yy0, status->scanner_state);
}
// 4099 "parser.c"
        break;
      case 200:
      case 231:
// 1707 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_literal(XX_T_DOUBLE, yymsp[0].minor.yy0, status->scanner_state);
}
// 4107 "parser.c"
        break;
      case 201:
      case 232:
// 1712 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_literal(XX_T_NULL, NULL, status->scanner_state);
  yy_destructor(91,&yymsp[0].minor);
}
// 4116 "parser.c"
        break;
      case 202:
      case 234:
// 1717 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_literal(XX_T_TRUE, NULL, status->scanner_state);
  yy_destructor(92,&yymsp[0].minor);
}
// 4125 "parser.c"
        break;
      case 203:
      case 233:
// 1722 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_literal(XX_T_FALSE, NULL, status->scanner_state);
  yy_destructor(93,&yymsp[0].minor);
}
// 4134 "parser.c"
        break;
      case 204:
// 1727 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("empty-array", NULL, NULL, NULL, status->scanner_state);
  yy_destructor(78,&yymsp[-1].minor);
  yy_destructor(79,&yymsp[0].minor);
}
// 4143 "parser.c"
        break;
      case 205:
// 1732 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("array", yymsp[-1].minor.yy103, NULL, NULL, status->scanner_state);
  yy_destructor(78,&yymsp[-2].minor);
  yy_destructor(79,&yymsp[0].minor);
}
// 4152 "parser.c"
        break;
      case 206:
// 1737 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_new_instance(yymsp[0].minor.yy0, NULL, status->scanner_state);
  yy_destructor(27,&yymsp[-1].minor);
}
// 4160 "parser.c"
        break;
      case 207:
// 1742 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_new_instance(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(27,&yymsp[-3].minor);
  yy_destructor(43,&yymsp[-1].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 4170 "parser.c"
        break;
      case 208:
// 1747 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_new_instance(yymsp[-3].minor.yy0, yymsp[-1].minor.yy103, status->scanner_state);
  yy_destructor(27,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 4180 "parser.c"
        break;
      case 209:
// 1752 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_fcall(yymsp[-3].minor.yy0, yymsp[-1].minor.yy103, status->scanner_state);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 4189 "parser.c"
        break;
      case 210:
// 1757 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_fcall(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(43,&yymsp[-1].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 4198 "parser.c"
        break;
      case 213:
// 1772 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_scall(yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy103, status->scanner_state);
  yy_destructor(80,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 4208 "parser.c"
        break;
      case 214:
// 1777 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_scall(yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(80,&yymsp[-3].minor);
  yy_destructor(43,&yymsp[-1].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 4218 "parser.c"
        break;
      case 215:
// 1782 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_mcall(yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy103, status->scanner_state);
  yy_destructor(77,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 4228 "parser.c"
        break;
      case 216:
// 1787 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_mcall(yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(77,&yymsp[-3].minor);
  yy_destructor(43,&yymsp[-1].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 4238 "parser.c"
        break;
      case 222:
// 1811 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_array_item(yymsp[-2].minor.yy103, yymsp[0].minor.yy103, status->scanner_state);
  yy_destructor(63,&yymsp[-1].minor);
}
// 4246 "parser.c"
        break;
      case 223:
// 1815 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_array_item(NULL, yymsp[0].minor.yy103, status->scanner_state);
}
// 4253 "parser.c"
        break;
      case 235:
// 1864 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(78,&yymsp[-2].minor);
  yy_destructor(79,&yymsp[0].minor);
}
// 4262 "parser.c"
        break;
      case 236:
// 1869 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_INTEGER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(78,&yymsp[-2].minor);
  yy_destructor(79,&yymsp[0].minor);
}
// 4271 "parser.c"
        break;
      case 237:
// 1874 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_STRING, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(78,&yymsp[-2].minor);
  yy_destructor(79,&yymsp[0].minor);
}
// 4280 "parser.c"
        break;
      case 239:
// 1882 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_comment(yymsp[0].minor.yy0, status->scanner_state);
}
// 4287 "parser.c"
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
// 804 "parser.lemon"


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

// 4363 "parser.c"
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
			case XX_T_DO:
				xx_(xx_parser, XX_DO, NULL, parser_status);
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
			case XX_T_MUL:
				xx_(xx_parser, XX_MUL, NULL, parser_status);
				break;
			case XX_T_DIV:
				xx_(xx_parser, XX_DIV, NULL, parser_status);
				break;
			case XX_T_MOD:
				xx_(xx_parser, XX_MOD, NULL, parser_status);
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
			case XX_T_CHAR:
				xx_parse_with_token(xx_parser, XX_T_CHAR, XX_CHAR, &token, parser_status);
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
