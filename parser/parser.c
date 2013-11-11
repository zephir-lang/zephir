/** The author disclaims copyright to this source code.
*/
/* First off, code is include which follows the "include" declaration
** in the input file. */
#include <stdio.h>
// 51 "parser.lemon"


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

static json_object *xx_ret_class(xx_parser_token *T, json_object *class_definition, int is_abstract, int is_final,
	xx_parser_token *E, json_object *I, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("class"));
	json_object_object_add(ret, "name", json_object_new_string(T->token));

	json_object_object_add(ret, "abstract", json_object_new_int(is_abstract));
	json_object_object_add(ret, "final", json_object_new_int(is_final));

	if (E) {
		json_object_object_add(ret, "extends", json_object_new_string(E->token));
	}

	if (I) {
		json_object_object_add(ret, "implements", I);
	}

	if (class_definition) {
		json_object_object_add(ret, "definition", class_definition);
	}

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}

static json_object *xx_ret_interface(xx_parser_token *T, json_object *interface_definition, xx_parser_token *E, xx_scanner_state *state)
{
  json_object *ret = json_object_new_object();

  json_object_object_add(ret, "type", json_object_new_string("interface"));
  json_object_object_add(ret, "name", json_object_new_string(T->token));

  if (E) {
    json_object_object_add(ret, "extends", json_object_new_string(E->token));
  }

  if (interface_definition) {
    json_object_object_add(ret, "definition", interface_definition);
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

static json_object *xx_ret_interface_definition(json_object *methods, json_object *constants, xx_scanner_state *state)
{
  json_object *ret = json_object_new_object();

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

static json_object *xx_ret_class_property(json_object *visibility, xx_parser_token *T,
		json_object *default_value, xx_parser_token *D, json_object *shortcuts, xx_scanner_state *state)
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

	if (shortcuts) {
		json_object_object_add(ret, "shortcuts", shortcuts);
	}

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}

static json_object *xx_ret_property_shortcut(xx_parser_token *C, xx_parser_token *D, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("shortcut"));
	if (C) {
		json_object_object_add(ret, "docblock", json_object_new_string(C->token));
	}
	json_object_object_add(ret, "name", json_object_new_string(D->token));

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

static json_object *xx_ret_class_method(json_object *visibility, xx_parser_token *T, json_object *parameters,
	json_object *statements, xx_parser_token *D, json_object *return_type, xx_scanner_state *state)
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

	if (return_type) {
		json_object_object_add(ret, "return-type", return_type);
	}

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}

static json_object *xx_ret_parameter(json_object *type, json_object *cast, xx_parser_token *N, json_object *default_value,
	int mandatory, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("parameter"));
	json_object_object_add(ret, "name", json_object_new_string(N->token));

	if (type) {
		json_object_object_add(ret, "data-type", type);
		json_object_object_add(ret, "mandatory", json_object_new_int(mandatory));
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

static json_object *xx_ret_return_type(int is_void, json_object *return_type_list, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("return-type"));

	json_object_object_add(ret, "list", return_type_list);

	json_object_object_add(ret, "void", json_object_new_int(is_void));

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}

static json_object *xx_ret_return_type_item(json_object *type, json_object *cast, int mandatory, int collection, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("return-type-parameter"));

	if (type) {
		json_object_object_add(ret, "data-type", type);
		json_object_object_add(ret, "mandatory", json_object_new_int(mandatory));
	}

	if (cast) {
		json_object_object_add(ret, "cast", cast);
		json_object_object_add(ret, "collection", json_object_new_int(collection));
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
		case XX_TYPE_LONG:
			return json_object_new_string("long");
		case XX_TYPE_ULONG:
			return json_object_new_string("ulong");
		case XX_TYPE_STRING:
			return json_object_new_string("string");
		case XX_TYPE_CHAR:
			return json_object_new_string("char");
		case XX_TYPE_ARRAY:
			return json_object_new_string("array");
		case XX_TYPE_VAR:
			return json_object_new_string("variable");
		default:
			fprintf(stderr, "unknown type?\n");
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

	if (expr) {
		json_object_object_add(ret, "type", json_object_new_string("case"));
		json_object_object_add(ret, "expr", expr);
	} else {
		json_object_object_add(ret, "type", json_object_new_string("default"));
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

static json_object *xx_ret_empty_statement(xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("empty"));

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

static json_object *xx_ret_fetch_statement(json_object *expr, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("fetch"));
	json_object_object_add(ret, "expr", expr);

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}

static json_object *xx_ret_fcall_statement(json_object *expr, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("fcall"));
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

static json_object *xx_ret_unset_statement(json_object *expr, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("unset"));

	json_object_object_add(ret, "expr", expr);

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

static json_object *xx_ret_new_instance(int dynamic, xx_parser_token *T, json_object *parameters, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("new"));
	json_object_object_add(ret, "class", json_object_new_string(T->token));
	json_object_object_add(ret, "dynamic", json_object_new_int(dynamic));

	if (parameters) {
		json_object_object_add(ret, "parameters", parameters);
	}

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}

static json_object *xx_ret_throw_exception(json_object *expr, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("throw"));
	if (expr) {
		json_object_object_add(ret, "expr", expr);
	}

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}

static json_object *xx_ret_fcall(int type, xx_parser_token *F, json_object *parameters, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("fcall"));
	json_object_object_add(ret, "name", json_object_new_string(F->token));
	json_object_object_add(ret, "call-type", json_object_new_int(type));

	if (parameters) {
		json_object_object_add(ret, "parameters", parameters);
	}

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}

static json_object *xx_ret_mcall(int type, json_object *O, xx_parser_token *M, json_object *parameters, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("mcall"));
	json_object_object_add(ret, "variable", O);
	json_object_object_add(ret, "name", json_object_new_string(M->token));
	json_object_object_add(ret, "call-type", json_object_new_int(type));

	if (parameters) {
		json_object_object_add(ret, "parameters", parameters);
	}

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}

static json_object *xx_ret_scall(int dynamic_class, xx_parser_token *O, xx_parser_token *M, json_object *parameters, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("scall"));
	json_object_object_add(ret, "dynamic-class", json_object_new_int(dynamic_class));
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

static json_object *xx_ret_cblock(xx_parser_token *T, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("cblock"));
	json_object_object_add(ret, "value", json_object_new_string(T->token));

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}


// 943 "parser.c"
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
#define YYNOCODE 190
#define YYACTIONTYPE unsigned short int
#define xx_TOKENTYPE xx_parser_token*
typedef union {
  xx_TOKENTYPE yy0;
  json_object* yy259;
  int yy379;
} YYMINORTYPE;
#define YYSTACKDEPTH 100
#define xx_ARG_SDECL xx_parser_status *status;
#define xx_ARG_PDECL ,xx_parser_status *status
#define xx_ARG_FETCH xx_parser_status *status = yypParser->status
#define xx_ARG_STORE yypParser->status = status
#define YYNSTATE 673
#define YYNRULE 328
#define YYERRORSYMBOL 105
#define YYERRSYMDT yy379
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
 /*     0 */   232,  241,  244,  235,  238,   63,  530,   34,   58,   37,
 /*    10 */    39,   40,  281, 1002,    1,    2,  672,    4,    5,    6,
 /*    20 */     7,    8,  986,  202,  229,  289,  294,  308,  226,  408,
 /*    30 */   304,  454,  427,  285,  185,  605,  152,  625,  609,  631,
 /*    40 */   307,  315,  598,  522,  137,  607,  303,  626,  275,  206,
 /*    50 */   122,  548,  485,  494,  503,  506,  497,  500,  509,  515,
 /*    60 */   512,  985,  518,  186,  188,  190,  623,  199,  253,  254,
 /*    70 */   250,  210,  214,  219,  389,  133,  286,  396,  322,  324,
 /*    80 */   323,  293,  391,  297,  147,  319,  325,  460,  475,  479,
 /*    90 */   482,  296,  298,  299,  300,  301,  302,  523,  232,  241,
 /*   100 */   244,  235,  238,  257,  259,  267,  261,  263,  265,  622,
 /*   110 */   281,  116,  636,  251,  488,  254,  250,   82,   90,  619,
 /*   120 */    96,  369,  229,  289,  294,  308,  226,  625,  304,  488,
 /*   130 */   309,  285,  185,  261,  263,  265,  620,  626,  340,  315,
 /*   140 */   557,  522,  254,  250,  303,  441,  275,  447,  427,  505,
 /*   150 */   485,  494,  503,  506,  497,  500,  509,  515,  512,  431,
 /*   160 */   518,  186,  188,  190,  511,  199,  253,  504,  493,  210,
 /*   170 */   214,  219,  389,  621,  628,  396,  322,  324,  323,  293,
 /*   180 */   145,  297,  651,  362,  325,  460,  475,  479,  482,  296,
 /*   190 */   298,  299,  300,  301,  302,  523,  232,  241,  244,  235,
 /*   200 */   238,  642,  141,  645,  652,  646,  631,  147,  281,  435,
 /*   210 */   661,  123,  607,   36,   37,   39,   40,  122,  613,  204,
 /*   220 */   229,  289,  294,  308,  226,  619,  304,  565,  614,  285,
 /*   230 */   185,  205,  606,   82,   90,  154,   96,  315,  542,  522,
 /*   240 */   123,  607,  303,  660,  275,  206,  122,  548,  485,  494,
 /*   250 */   503,  506,  497,  500,  509,  515,  512,  650,  518,  186,
 /*   260 */   188,  190,  455,  199,  253,  425,   86,  210,  214,  219,
 /*   270 */   389,   83,  664,  396,  322,  324,  323,  293,  314,  297,
 /*   280 */   488,  345,  325,  460,  475,  479,  482,  296,  298,  299,
 /*   290 */   300,  301,  302,  523,  232,  241,  244,  235,  238,  253,
 /*   300 */     3,    4,    5,    6,    7,    8,  281,  616,  380,  322,
 /*   310 */   324,  323,  293,   87,  619,  499,  347,  325,  229,  289,
 /*   320 */   294,  308,  226,   25,  304,   66,   42,  285,  185,  643,
 /*   330 */    24,  609,  631,  154,  122,  315,  546,  522,  607,  153,
 /*   340 */   303,  488,  275,  122,  122,  666,  485,  494,  503,  506,
 /*   350 */   497,  500,  509,  515,  512,  117,  518,  186,  188,  190,
 /*   360 */   667,  199,  253,  402,  648,  210,  214,  219,  389,  203,
 /*   370 */   551,  396,  322,  324,  323,  293,  520,  297,  314,  336,
 /*   380 */   325,  460,  475,  479,  482,  296,  298,  299,  300,  301,
 /*   390 */   302,  523,  232,  241,  244,  235,  238,  403,  404,  405,
 /*   400 */   406,  407,    9,  283,  281,   12,  147,  138,  313,  655,
 /*   410 */   665,  314,  112,  522,  633,  115,  229,  289,  294,  308,
 /*   420 */   226,  619,  304,  314,  634,  285,  185,  463,   82,   90,
 /*   430 */    23,   67,   25,  315,  592,  522,  490,  123,  303,  124,
 /*   440 */   275,  330,  282,  122,  485,  494,  503,  506,  497,  500,
 /*   450 */   509,  515,  512,  374,  518,  186,  188,  190,   62,  199,
 /*   460 */   253,   65,  462,  210,  214,  219,  389,  523,  307,  396,
 /*   470 */   322,  324,  323,  293,   97,  297,  370,  366,  325,  460,
 /*   480 */   475,  479,  482,  296,  298,  299,  300,  301,  302,  523,
 /*   490 */   232,  241,  244,  235,  238,  253,  659,  156,  662,  519,
 /*   500 */   493,  147,  281,  157,  286,  322,  324,  323,  293,  397,
 /*   510 */   459,   66,  379,  325,  229,  289,  294,  308,  226,  144,
 /*   520 */   304,  593,  147,  285,  185,  399,  376,   82,   90,  669,
 /*   530 */    96,  315,  218,  522,  147,  314,  303,  413,  275,  422,
 /*   540 */   427,  594,  485,  494,  503,  506,  497,  500,  509,  515,
 /*   550 */   512,   46,  518,  186,  188,  190,  587,  199,  253,  488,
 /*   560 */   398,  210,  214,  219,  389,  346,  583,  396,  322,  324,
 /*   570 */   323,  293,  653,  297,  651,  329,  325,  460,  475,  479,
 /*   580 */   482,  296,  298,  299,  300,  301,  302,  523,  232,  241,
 /*   590 */   244,  235,  238,   20,  502,   22,  611,  136,  423,   25,
 /*   600 */   281,  425,   60,  314,  137,   28,  124,  122,   82,   90,
 /*   610 */   122,   96,  229,  289,  294,  308,  226,  578,  304,  516,
 /*   620 */   493,  285,  185,  513,  493,  559,  314,  318,  122,  315,
 /*   630 */   553,  522,  288,  367,  303,  159,  275,  510,  493,  599,
 /*   640 */   485,  494,  503,  506,  497,  500,  509,  515,  512,  595,
 /*   650 */   518,  186,  188,  190,  443,  199,  348,  501,  493,  210,
 /*   660 */   214,  219,  389,  326,  327,  396,  507,  493,  402,   15,
 /*   670 */    30,  297,  332,   59,   18,  460,  475,  479,  482,  296,
 /*   680 */   298,  299,  300,  301,  302,  523,  232,  241,  244,  235,
 /*   690 */   238,   31,  279,   34,   58,   37,   39,   40,  281,  290,
 /*   700 */   314,  314,  403,  404,  405,  406,  407,  498,  493,  287,
 /*   710 */   229,  289,  294,  308,  226,  107,  304,  291,  355,  285,
 /*   720 */   185,  663,  365,  651,  495,  493,  122,  315,  564,  522,
 /*   730 */   363,  320,  303,  488,  275,   66,  488,  488,  485,  494,
 /*   740 */   503,  506,  497,  500,  509,  515,  512,  412,  518,  186,
 /*   750 */   188,  190,  426,  199,   69,  430,  492,  210,  214,  219,
 /*   760 */   389,  118,  488,  396,  121,   61,  402,  491,  517,  297,
 /*   770 */   354,  508,  496,  460,  475,  479,  482,  296,  298,  299,
 /*   780 */   300,  301,  302,  523,  232,  241,  244,  235,  238,  560,
 /*   790 */    66,   34,   58,   37,   39,   40,  281,  487,  488,   43,
 /*   800 */   403,  404,  405,  406,  407,  314,  486,  493,  229,  289,
 /*   810 */   294,  308,  226,  571,  304,  489,   44,  285,  185,  448,
 /*   820 */   566,  531,  425,  567,  532,  315,  562,  522,   18,   45,
 /*   830 */   303,   27,  275,  514,  150,  337,  485,  494,  503,  506,
 /*   840 */   497,  500,  509,  515,  512,   70,  518,  186,  188,  190,
 /*   850 */   545,  199,  253,  543,   17,  210,  214,  219,  389,  668,
 /*   860 */   474,  396,  322,  324,  323,  293,   32,  297,  132,  312,
 /*   870 */   325,  460,  475,  479,  482,  296,  298,  299,  300,  301,
 /*   880 */   302,  523,  232,  241,  244,  235,  238,  465,  549,  472,
 /*   890 */    43,  470,  468,  554,  281,  461,  466,  322,  324,  323,
 /*   900 */   293,  555,   95,  109,  146,  561,  229,  289,  294,  308,
 /*   910 */   226,  252,  304,  310,  452,  285,  185,  445,   64,  110,
 /*   920 */   440,  437,  436,  315,  576,  522,  670,  573,  303,  433,
 /*   930 */   275,   13,  432,  201,  485,  494,  503,  506,  497,  500,
 /*   940 */   509,  515,  512,  316,  518,  186,  188,  190,  143,  199,
 /*   950 */   253,  580,  581,  210,  214,  219,  389,  317,  114,  396,
 /*   960 */   322,  324,  323,  293,   93,  297,  417,  373,  325,  460,
 /*   970 */   475,  479,  482,  296,  298,  299,  300,  301,  302,  523,
 /*   980 */   232,  241,  244,  235,  238,  410,  420,  671,  589,  409,
 /*   990 */    35,   10,  281,  142,   21,  322,  324,  323,  293,   80,
 /*  1000 */    79,  358,  601,   88,  229,  289,  294,  308,  226,  140,
 /*  1010 */   304,  120,  400,  285,  185,   92,   38,  401,  328,   14,
 /*  1020 */   225,  315,  162,  522,  139,  673,  303,  629,  275,  223,
 /*  1030 */   617,  618,  485,  494,  503,  506,  497,  500,  509,  515,
 /*  1040 */   512,  615,  518,  186,  188,  190,  215,  199,   84,  388,
 /*  1050 */   333,  210,  214,  219,  389,  723,  334,  396,  724,  322,
 /*  1060 */   324,  323,  293,  297,  390,  624,  335,  460,  475,  479,
 /*  1070 */   482,  296,  298,  299,  300,  301,  302,  523,  232,  241,
 /*  1080 */   244,  235,  238,  410,   85,  220,  627,  414,  187,   91,
 /*  1090 */   281,  127,  630,  322,  324,  323,  293,  189,  341,  128,
 /*  1100 */   639,   11,  229,  289,  294,  308,  226,  216,  304,  635,
 /*  1110 */   342,  285,  185,  637,  638,  640,  377,  343,  378,  315,
 /*  1120 */   160,  522,   16,  192,  303,  372,  275,   89,  371,  211,
 /*  1130 */   485,  494,  503,  506,  497,  500,  509,  515,  512,  135,
 /*  1140 */   518,  186,  188,  190,  224,  199,  649,  388,  208,  210,
 /*  1150 */   214,  219,  389,  207,  196,  396,  195,  322,  324,  323,
 /*  1160 */   293,  297,  129,  130,  656,  460,  475,  479,  482,  296,
 /*  1170 */   298,  299,  300,  301,  302,  523,  232,  241,  244,  235,
 /*  1180 */   238,  572,  421,   34,   58,   37,   39,   40,  281,  419,
 /*  1190 */   987,  657,  322,  324,  323,  293,  654,  519,  519,  519,
 /*  1200 */   229,  289,  294,  308,  226,  658,  304,  519,  519,  285,
 /*  1210 */   185,  134,  519,  519,  519,  519,  519,  315,  596,  522,
 /*  1220 */   519,  519,  303,  519,  275,  519,  519,  519,  485,  494,
 /*  1230 */   503,  506,  497,  500,  509,  515,  512,  519,  518,  186,
 /*  1240 */   188,  190,  519,  199,  519,  465,  519,  210,  214,  219,
 /*  1250 */   389,  519,  519,  396,  464,  322,  324,  323,  293,  297,
 /*  1260 */   519,  519,  519,  460,  475,  479,  482,  296,  298,  299,
 /*  1270 */   300,  301,  302,  523,  232,  241,  244,  235,  238,  410,
 /*  1280 */   519,  519,  519,  418,  519,  519,  281,  519,  519,  322,
 /*  1290 */   324,  323,  293,  519,  519,  519,  519,  519,  229,  289,
 /*  1300 */   294,  308,  226,  519,  304,  519,  519,  285,  185,  519,
 /*  1310 */   519,  519,  519,  519,  519,  315,  529,  522,  519,  519,
 /*  1320 */   303,  519,  275,  519,  519,  519,  485,  494,  503,  506,
 /*  1330 */   497,  500,  509,  515,  512,  519,  518,  186,  188,  190,
 /*  1340 */   544,  199,  519,  388,  519,  210,  214,  219,  389,  519,
 /*  1350 */   519,  396,  519,  322,  324,  323,  293,  297,  519,  519,
 /*  1360 */   519,  460,  475,  479,  482,  296,  298,  299,  300,  301,
 /*  1370 */   302,  523,  232,  241,  244,  235,  238,  410,  519,  519,
 /*  1380 */   519,  424,  519,  519,  281,  519,  519,  322,  324,  323,
 /*  1390 */   293,  519,  519,  519,  519,  519,  229,  289,  294,  308,
 /*  1400 */   226,  519,  304,  519,  519,  285,  185,  519,  519,  519,
 /*  1410 */   519,  519,  519,  315,  547,  522,  519,  519,  303,  519,
 /*  1420 */   275,  519,  519,  519,  485,  494,  503,  506,  497,  500,
 /*  1430 */   509,  515,  512,  519,  518,  186,  188,  190,  519,  199,
 /*  1440 */   253,  519,  519,  210,  214,  219,  389,  519,  519,  396,
 /*  1450 */   322,  324,  323,  293,  519,  297,  519,  519,  339,  460,
 /*  1460 */   475,  479,  482,  296,  298,  299,  300,  301,  302,  523,
 /*  1470 */   232,  241,  244,  235,  238,  410,  519,  519,  519,  434,
 /*  1480 */   519,  519,  281,  519,  519,  322,  324,  323,  293,  519,
 /*  1490 */   519,  519,  519,  519,  229,  289,  294,  308,  226,  519,
 /*  1500 */   304,  519,  519,  285,  185,  519,  519,  519,  519,  519,
 /*  1510 */   519,  315,  586,  522,  519,  519,  303,  519,  275,  519,
 /*  1520 */   519,  519,  485,  494,  503,  506,  497,  500,  509,  515,
 /*  1530 */   512,  519,  518,  186,  188,  190,  191,  199,  519,  388,
 /*  1540 */   519,  210,  214,  219,  389,  519,  519,  396,  519,  322,
 /*  1550 */   324,  323,  293,  297,  519,  519,  519,  460,  475,  479,
 /*  1560 */   482,  296,  298,  299,  300,  301,  302,  523,  232,  241,
 /*  1570 */   244,  235,  238,  410,  519,  519,  519,  438,  519,  519,
 /*  1580 */   281,  519,  519,  322,  324,  323,  293,  519,  519,  519,
 /*  1590 */   519,  519,  229,  289,  294,  308,  226,  519,  304,  519,
 /*  1600 */   519,  285,  185,  519,  519,  519,  519,  519,  519,  315,
 /*  1610 */   213,  522,  519,  519,  303,  519,  275,  519,  519,  519,
 /*  1620 */   485,  494,  503,  506,  497,  500,  509,  515,  512,  519,
 /*  1630 */   518,  186,  188,  190,  200,  199,  519,  388,  519,  210,
 /*  1640 */   214,  219,  389,  519,  519,  396,  519,  322,  324,  323,
 /*  1650 */   293,  297,  519,  519,  519,  460,  475,  479,  482,  296,
 /*  1660 */   298,  299,  300,  301,  302,  523,  232,  241,  244,  235,
 /*  1670 */   238,  410,  519,  519,  519,  442,  519,  519,  281,  519,
 /*  1680 */   519,  322,  324,  323,  293,  519,  519,  519,  519,  519,
 /*  1690 */   229,  289,  294,  308,  226,  519,  304,  519,  519,  285,
 /*  1700 */   185,  519,  519,  519,  519,  519,  519,  315,  395,  522,
 /*  1710 */   519,  519,  303,  519,  275,  519,  519,  519,  485,  494,
 /*  1720 */   503,  506,  497,  500,  509,  515,  512,  519,  518,  186,
 /*  1730 */   188,  190,  519,  199,  385,  519,  519,  210,  214,  219,
 /*  1740 */   389,  519,  519,  396,  322,  324,  323,  293,  519,  297,
 /*  1750 */   519,  519,  519,  460,  475,  479,  482,  296,  298,  299,
 /*  1760 */   300,  301,  302,  523,  232,  241,  244,  235,  238,  410,
 /*  1770 */   519,  519,  519,  446,  519,  519,  281,  519,  519,  322,
 /*  1780 */   324,  323,  293,  519,  519,  519,  519,  519,  229,  289,
 /*  1790 */   294,  308,  226,  519,  304,  519,  519,  285,  185,  519,
 /*  1800 */   519,  519,  519,  519,  519,  315,  568,  522,  519,  519,
 /*  1810 */   303,  519,  275,  519,  519,  519,  485,  494,  503,  506,
 /*  1820 */   497,  500,  509,  515,  512,  519,  518,  186,  188,  190,
 /*  1830 */   519,  199,  284,  519,  519,  210,  214,  219,  389,  519,
 /*  1840 */   519,  396,  322,  324,  323,  293,  519,  297,  519,  519,
 /*  1850 */   519,  460,  475,  479,  482,  296,  298,  299,  300,  301,
 /*  1860 */   302,  523,  232,  241,  244,  235,  238,  410,  519,  519,
 /*  1870 */   519,  449,  519,  519,  281,  519,  519,  322,  324,  323,
 /*  1880 */   293,  519,  519,  519,  519,  519,  229,  289,  294,  308,
 /*  1890 */   226,  519,  304,  519,  519,  285,  185,  519,  519,  519,
 /*  1900 */   519,  519,  519,  315,  222,  522,  519,  519,  303,  519,
 /*  1910 */   275,  519,  519,  519,  485,  494,  503,  506,  497,  500,
 /*  1920 */   509,  515,  512,  519,  518,  186,  188,  190,  519,  199,
 /*  1930 */   392,  519,  519,  210,  214,  219,  389,  519,  519,  396,
 /*  1940 */   322,  324,  323,  293,  519,  297,  519,  519,  519,  460,
 /*  1950 */   475,  479,  482,  296,  298,  299,  300,  301,  302,  523,
 /*  1960 */   232,  241,  244,  235,  238,  410,  519,  519,  519,  453,
 /*  1970 */   519,  519,  281,  519,  519,  322,  324,  323,  293,  519,
 /*  1980 */   519,  519,  519,  519,  229,  289,  294,  308,  226,  519,
 /*  1990 */   304,  519,  519,  285,  185,  519,  519,  519,  519,  519,
 /*  2000 */   519,  315,  558,  522,  519,  519,  303,  519,  275,  519,
 /*  2010 */   519,  519,  485,  494,  503,  506,  497,  500,  509,  515,
 /*  2020 */   512,  519,  518,  186,  188,  190,  519,  199,  268,  519,
 /*  2030 */   519,  210,  214,  219,  389,  519,  519,  396,  322,  324,
 /*  2040 */   323,  293,  519,  297,  519,  519,  519,  460,  475,  479,
 /*  2050 */   482,  296,  298,  299,  300,  301,  302,  523,  232,  241,
 /*  2060 */   244,  235,  238,  410,  519,  519,  519,  456,  519,  519,
 /*  2070 */   281,  519,  519,  322,  324,  323,  293,  519,  519,  519,
 /*  2080 */   519,  519,  229,  289,  294,  308,  226,  519,  304,  519,
 /*  2090 */   519,  285,  185,  519,  519,  519,  519,  519,  519,  315,
 /*  2100 */   536,  522,  519,  519,  303,  519,  275,  519,  519,  519,
 /*  2110 */   485,  494,  503,  506,  497,  500,  509,  515,  512,  519,
 /*  2120 */   518,  186,  188,  190,  519,  199,  272,  519,  519,  210,
 /*  2130 */   214,  219,  389,  519,  278,  396,  322,  324,  323,  293,
 /*  2140 */   519,  297,  519,  519,  519,  460,  475,  479,  482,  296,
 /*  2150 */   298,  299,  300,  301,  302,  523,  232,  241,  244,  235,
 /*  2160 */   238,  276,  519,  519,  519,  519,  519,  519,  281,  519,
 /*  2170 */   519,  322,  324,  323,  293,  519,  519,  519,  519,  519,
 /*  2180 */   229,  289,  294,  308,  226,  519,  304,  519,  519,  285,
 /*  2190 */   185,  519,  519,  519,  519,  519,  519,  315,  590,  522,
 /*  2200 */   519,  519,  303,  519,  275,  519,  519,  519,  485,  494,
 /*  2210 */   503,  506,  497,  500,  509,  515,  512,  519,  518,  186,
 /*  2220 */   188,  190,  519,  199,  533,  519,  519,  210,  214,  219,
 /*  2230 */   389,  519,  519,  396,  322,  324,  323,  293,  519,  297,
 /*  2240 */   519,  519,  519,  460,  475,  479,  482,  296,  298,  299,
 /*  2250 */   300,  301,  302,  523,  232,  241,  244,  235,  238,  113,
 /*  2260 */   519,   34,   58,   37,   39,   40,  281,  588,  519,   34,
 /*  2270 */    58,   37,   39,   40,  519,  519,  519,  519,  229,  289,
 /*  2280 */   294,  308,  226,  519,  304,  519,  519,  285,  185,  519,
 /*  2290 */   519,  519,  519,  519,  519,  315,  552,  522,  519,  519,
 /*  2300 */   303,  519,  275,  519,  519,  519,  485,  494,  503,  506,
 /*  2310 */   497,  500,  509,  515,  512,  519,  518,  186,  188,  190,
 /*  2320 */   519,  199,  247,  519,  519,  210,  214,  219,  389,  519,
 /*  2330 */   519,  396,  322,  324,  323,  293,  519,  297,  519,  519,
 /*  2340 */   519,  460,  475,  479,  482,  296,  298,  299,  300,  301,
 /*  2350 */   302,  523,  232,  241,  244,  235,  238,  600,  519,   34,
 /*  2360 */    58,   37,   39,   40,  281,  119,  519,   34,   58,   37,
 /*  2370 */    39,   40,  519,  519,  519,  519,  229,  289,  294,  308,
 /*  2380 */   226,  519,  304,  519,  519,  285,  185,  519,  519,  519,
 /*  2390 */   519,  519,  519,  315,  584,  522,  519,  519,  303,  519,
 /*  2400 */   275,  519,  519,  519,  485,  494,  503,  506,  497,  500,
 /*  2410 */   509,  515,  512,  519,  518,  186,  188,  190,  519,  199,
 /*  2420 */   483,  519,  519,  210,  214,  219,  389,  519,  519,  396,
 /*  2430 */   322,  324,  323,  293,  519,  297,  519,  519,  519,  460,
 /*  2440 */   475,  479,  482,  296,  298,  299,  300,  301,  302,  523,
 /*  2450 */   232,  241,  244,  235,  238,  270,  519,  519,  519,  519,
 /*  2460 */   519,  519,  281,  519,  519,  322,  324,  323,  293,  519,
 /*  2470 */   519,  519,  519,  519,  229,  289,  294,  308,  226,  519,
 /*  2480 */   304,  519,  519,  285,  185,  519,  519,  519,  519,  519,
 /*  2490 */   519,  315,  198,  522,  519,  519,  303,  519,  275,  519,
 /*  2500 */   519,  519,  485,  494,  503,  506,  497,  500,  509,  515,
 /*  2510 */   512,  519,  518,  186,  188,  190,  519,  199,  249,  519,
 /*  2520 */   519,  210,  214,  219,  389,  519,  519,  396,  322,  324,
 /*  2530 */   323,  293,  519,  297,  519,  519,  519,  460,  475,  479,
 /*  2540 */   482,  296,  298,  299,  300,  301,  302,  523,  232,  241,
 /*  2550 */   244,  235,  238,  227,  519,  519,  519,  519,  519,  519,
 /*  2560 */   281,  519,  519,  322,  324,  323,  293,  519,  519,  519,
 /*  2570 */   519,  519,  229,  289,  294,  308,  226,  519,  304,  519,
 /*  2580 */   519,  285,  185,  519,  519,  519,  519,  519,  519,  315,
 /*  2590 */   541,  522,  519,  519,  303,  519,  275,  519,  519,  519,
 /*  2600 */   485,  494,  503,  506,  497,  500,  509,  515,  512,  519,
 /*  2610 */   518,  186,  188,  190,  519,  199,  262,  519,  519,  210,
 /*  2620 */   214,  219,  389,  519,  519,  396,  322,  324,  323,  293,
 /*  2630 */   519,  297,  519,  519,  519,  460,  475,  479,  482,  296,
 /*  2640 */   298,  299,  300,  301,  302,  523,  232,  241,  244,  235,
 /*  2650 */   238,  538,  519,  519,  519,  519,  519,  519,  281,  519,
 /*  2660 */   519,  322,  324,  323,  293,  519,  519,  519,  519,  519,
 /*  2670 */   229,  289,  294,  308,  226,  519,  304,  519,  519,  285,
 /*  2680 */   185,  519,  519,  519,  519,  519,  519,  315,  602,  522,
 /*  2690 */   519,  519,  303,  519,  275,  519,  519,  519,  485,  494,
 /*  2700 */   503,  506,  497,  500,  509,  515,  512,  519,  518,  186,
 /*  2710 */   188,  190,  519,  199,  274,  519,  519,  210,  214,  219,
 /*  2720 */   389,  519,  519,  396,  322,  324,  323,  293,  519,  297,
 /*  2730 */   519,  519,  519,  460,  475,  479,  482,  296,  298,  299,
 /*  2740 */   300,  301,  302,  523,  232,  241,  244,  235,  238,  292,
 /*  2750 */   519,  519,  519,  519,  519,  519,  281,  519,  519,  322,
 /*  2760 */   324,  323,  293,  519,  519,  519,  519,  519,  229,  289,
 /*  2770 */   294,  308,  226,  519,  304,  519,  519,  285,  185,  519,
 /*  2780 */   519,  519,  519,  519,  519,  315,  574,  522,  519,  519,
 /*  2790 */   303,  519,  275,  519,  519,  519,  485,  494,  503,  506,
 /*  2800 */   497,  500,  509,  515,  512,  519,  518,  186,  188,  190,
 /*  2810 */   519,  199,  236,  519,  519,  210,  214,  219,  389,  519,
 /*  2820 */   519,  396,  322,  324,  323,  293,  519,  297,  519,  519,
 /*  2830 */   519,  460,  475,  479,  482,  296,  298,  299,  300,  301,
 /*  2840 */   302,  523,  232,  241,  244,  235,  238,  280,  519,  519,
 /*  2850 */   519,  519,  519,  519,  281,  519,  519,  322,  324,  323,
 /*  2860 */   293,  519,  519,  519,  519,  519,  229,  289,  294,  308,
 /*  2870 */   226,  519,  304,  519,  519,  285,  185,  519,  519,  519,
 /*  2880 */   519,  519,  519,  315,  604,  522,  519,  519,  303,  519,
 /*  2890 */   275,  519,  519,  519,  485,  494,  503,  506,  497,  500,
 /*  2900 */   509,  515,  512,  519,  518,  186,  188,  190,  519,  199,
 /*  2910 */   260,  519,  519,  210,  214,  219,  389,  519,  519,  396,
 /*  2920 */   322,  324,  323,  293,  519,  297,  519,  519,  519,  460,
 /*  2930 */   475,  479,  482,  296,  298,  299,  300,  301,  302,  523,
 /*  2940 */   232,  241,  244,  235,  238,  384,  519,  519,  519,  519,
 /*  2950 */   519,  519,  281,  519,  519,  322,  324,  323,  293,  519,
 /*  2960 */   519,  519,  519,  519,  229,  289,  294,  308,  226,  519,
 /*  2970 */   304,  519,  519,  285,  185,  519,  519,  519,  519,  519,
 /*  2980 */   519,  315,  194,  522,  519,  519,  303,  519,  275,  519,
 /*  2990 */   519,  519,  485,  494,  503,  506,  497,  500,  509,  515,
 /*  3000 */   512,  519,  518,  186,  188,  190,  519,  199,  239,  519,
 /*  3010 */   519,  210,  214,  219,  389,  519,  519,  396,  322,  324,
 /*  3020 */   323,  293,  519,  297,  519,  519,  519,  460,  475,  479,
 /*  3030 */   482,  296,  298,  299,  300,  301,  302,  523,  232,  241,
 /*  3040 */   244,  235,  238,  233,  519,  519,  519,  519,  519,  519,
 /*  3050 */   281,  519,  519,  322,  324,  323,  293,  519,  519,  519,
 /*  3060 */   519,  519,  229,  289,  294,  308,  226,  519,  304,  519,
 /*  3070 */   519,  285,  185,  519,  519,  519,  519,  519,  519,  315,
 /*  3080 */   570,  522,  519,  519,  303,  519,  275,  519,  519,  519,
 /*  3090 */   485,  494,  503,  506,  497,  500,  509,  515,  512,  519,
 /*  3100 */   518,  186,  188,  190,  519,  199,  258,  519,  519,  210,
 /*  3110 */   214,  219,  389,  519,  519,  396,  322,  324,  323,  293,
 /*  3120 */   519,  297,  519,  519,  519,  460,  475,  479,  482,  296,
 /*  3130 */   298,  299,  300,  301,  302,  523,  232,  241,  244,  235,
 /*  3140 */   238,  264,  519,  519,  519,  519,  519,  519,  281,  519,
 /*  3150 */   519,  322,  324,  323,  293,  519,  519,  519,  519,  519,
 /*  3160 */   229,  289,  294,  308,  226,  519,  304,  519,  519,  285,
 /*  3170 */   185,  519,  519,  519,  519,  519,  519,  315,  519,  522,
 /*  3180 */   519,  519,  303,  519,  275,  519,  519,  519,  485,  494,
 /*  3190 */   503,  506,  497,  500,  509,  515,  512,  519,  518,  186,
 /*  3200 */   188,  190,  519,  199,  383,  519,  519,  210,  214,  219,
 /*  3210 */   389,  519,  519,  396,  322,  324,  323,  293,  519,  297,
 /*  3220 */   519,  519,  519,  460,  475,  479,  482,  296,  298,  299,
 /*  3230 */   300,  301,  302,  523,  232,  241,  244,  235,  238,  519,
 /*  3240 */   519,  519,  519,  519,  519,  519,  281,  519,  519,  519,
 /*  3250 */    78,  519,   98,   99,  101,  100,  102,  519,  229,  289,
 /*  3260 */   294,  308,  226,   81,  304,  519,  242,  285,  519,  519,
 /*  3270 */   519,  519,  519,  519,  519,  315,  322,  324,  323,  293,
 /*  3280 */   303,  519,  275,  519,  519,  519,   47,   48,   49,   50,
 /*  3290 */    51,   52,   53,   54,   55,   56,   57,  104,  105,  519,
 /*  3300 */    73,  519,  183,  164,  519,  579,  519,  103,   71,   72,
 /*  3310 */    74,   75,   77,   76,  519,  519,  519,  297,  519,   98,
 /*  3320 */    99,  101,  100,  102,  161,  296,  298,  299,  300,  301,
 /*  3330 */   302,  519,  519,  524,  165,  166,  167,  168,  169,  170,
 /*  3340 */   171,  172,  173,  174,  175,  176,  177,  178,  179,  180,
 /*  3350 */   181,  182,  184,  519,  519,  519,  521,  519,  612,  519,
 /*  3360 */   183,  164,  519,  519,  104,  105,  467,  469,  471,  473,
 /*  3370 */   519,  519,  579,  519,  103,  519,   98,   99,  101,  100,
 /*  3380 */   102,  519,  197,  519,   98,   99,  101,  100,  102,  519,
 /*  3390 */   519,  524,  165,  166,  167,  168,  169,  170,  171,  172,
 /*  3400 */   173,  174,  175,  176,  177,  178,  179,  180,  181,  182,
 /*  3410 */   184,  519,  519,  519,  521,  519,  183,  164,  519,  519,
 /*  3420 */   519,  104,  105,  519,  467,  469,  471,  473,  519,  104,
 /*  3430 */   105,  103,  705,  608,  519,  131,  519,  519,  585,  103,
 /*  3440 */   519,  519,  519,  519,   41,  519,  519,  524,  165,  166,
 /*  3450 */   167,  168,  169,  170,  171,  172,  173,  174,  175,  176,
 /*  3460 */   177,  178,  179,  180,  181,  182,  184,  519,  519,  519,
 /*  3470 */   521,  519,  519,  519,  183,  164,  519,  519,  519,  519,
 /*  3480 */   467,  469,  471,  473,   47,   48,   49,   50,   51,   52,
 /*  3490 */    53,   54,   55,   56,   57,  519,  591,  519,   98,   99,
 /*  3500 */   101,  100,  102,  519,  519,  524,  165,  166,  167,  168,
 /*  3510 */   169,  170,  171,  172,  173,  174,  175,  176,  177,  178,
 /*  3520 */   179,  180,  181,  182,  184,  519,  519,  519,  521,  519,
 /*  3530 */   183,  164,  519,  519,  519,  519,  519,  519,  467,  469,
 /*  3540 */   471,  473,  519,  104,  105,  519,  710,  125,  519,  131,
 /*  3550 */   519,  519,  540,  103,   98,   99,  101,  100,  102,  519,
 /*  3560 */   519,  524,  165,  166,  167,  168,  169,  170,  171,  172,
 /*  3570 */   173,  174,  175,  176,  177,  178,  179,  180,  181,  182,
 /*  3580 */   184,  519,  519,  519,  521,  519,  519,  519,  183,  164,
 /*  3590 */   519,  519,  519,  519,  467,  469,  471,  473,  519,  104,
 /*  3600 */   105,  519,   19,  125,  519,  131,  519,  519,  519,  103,
 /*  3610 */   550,  519,   98,   99,  101,  100,  102,  519,  519,  524,
 /*  3620 */   165,  166,  167,  168,  169,  170,  171,  172,  173,  174,
 /*  3630 */   175,  176,  177,  178,  179,  180,  181,  182,  184,  519,
 /*  3640 */   519,  519,  521,  519,  183,  164,  519,  519,  519,  519,
 /*  3650 */   519,  519,  467,  469,  471,  473,  519,  104,  105,  519,
 /*  3660 */   148,  610,  519,  131,  519,  519,  212,  103,   98,   99,
 /*  3670 */   101,  100,  102,  519,  519,  524,  165,  166,  167,  168,
 /*  3680 */   169,  170,  171,  172,  173,  174,  175,  176,  177,  178,
 /*  3690 */   179,  180,  181,  182,  184,  519,  519,  519,  521,  519,
 /*  3700 */   519,  519,  183,  164,  519,  519,  519,  519,  467,  469,
 /*  3710 */   471,  473,  519,  104,  105,  519,  702,  610,  519,  131,
 /*  3720 */   519,  519,  519,  103,  556,  519,   98,   99,  101,  100,
 /*  3730 */   102,  519,  519,  524,  165,  166,  167,  168,  169,  170,
 /*  3740 */   171,  172,  173,  174,  175,  176,  177,  178,  179,  180,
 /*  3750 */   181,  182,  184,  519,  519,  519,  521,  519,  183,  164,
 /*  3760 */   519,  519,  519,  519,  519,  519,  467,  469,  471,  473,
 /*  3770 */   519,  104,  105,  519,  701,  610,  519,  131,  519,  519,
 /*  3780 */   217,  103,   98,   99,  101,  100,  102,  519,  519,  524,
 /*  3790 */   165,  166,  167,  168,  169,  170,  171,  172,  173,  174,
 /*  3800 */   175,  176,  177,  178,  179,  180,  181,  182,  184,  519,
 /*  3810 */   519,  519,  521,  519,  519,  519,  183,  164,  519,  519,
 /*  3820 */   519,  519,  467,  469,  471,  473,  519,  104,  105,  519,
 /*  3830 */   703,  577,  519,  519,  519,  519,  519,  103,  563,  519,
 /*  3840 */    98,   99,  101,  100,  102,  519,  519,  524,  165,  166,
 /*  3850 */   167,  168,  169,  170,  171,  172,  173,  174,  175,  176,
 /*  3860 */   177,  178,  179,  180,  181,  182,  184,  519,  519,  519,
 /*  3870 */   521,  519,  183,  164,  519,  519,  519,  519,  519,  519,
 /*  3880 */   467,  469,  471,  473,  519,  104,  105,  519,  709,  577,
 /*  3890 */   519,  519,  519,  519,  221,  103,   98,   99,  101,  100,
 /*  3900 */   102,  519,  519,  524,  165,  166,  167,  168,  169,  170,
 /*  3910 */   171,  172,  173,  174,  175,  176,  177,  178,  179,  180,
 /*  3920 */   181,  182,  184,  519,  519,  519,  521,  519,  519,  519,
 /*  3930 */   183,  164,  519,  519,  519,  519,  467,  469,  471,  473,
 /*  3940 */   519,  104,  105,  519,  711,  106,  519,  519,  519,  519,
 /*  3950 */   519,  103,  209,  519,   98,   99,  101,  100,  102,  519,
 /*  3960 */   519,  524,  165,  166,  167,  168,  169,  170,  171,  172,
 /*  3970 */   173,  174,  175,  176,  177,  178,  179,  180,  181,  182,
 /*  3980 */   184,  519,  519,  519,  521,  519,  183,  164,  519,  519,
 /*  3990 */   519,  519,  519,  519,  467,  469,  471,  473,  519,  104,
 /*  4000 */   105,  519,  708,  577,  519,  519,  519,  519,  394,  103,
 /*  4010 */    98,   99,  101,  100,  102,  519,  519,  524,  165,  166,
 /*  4020 */   167,  168,  169,  170,  171,  172,  173,  174,  175,  176,
 /*  4030 */   177,  178,  179,  180,  181,  182,  184,  519,  519,  519,
 /*  4040 */   521,  519,  519,  519,  183,  164,  519,  519,  519,  519,
 /*  4050 */   467,  469,  471,  473,  519,  104,  105,  519,  706,  644,
 /*  4060 */   519,  519,  519,  519,  519,  103,  569,  519,   98,   99,
 /*  4070 */   101,  100,  102,  519,  519,  524,  165,  166,  167,  168,
 /*  4080 */   169,  170,  171,  172,  173,  174,  175,  176,  177,  178,
 /*  4090 */   179,  180,  181,  182,  184,  519,  519,  519,  521,  519,
 /*  4100 */   183,  164,  519,  519,  519,  519,  519,  519,  467,  469,
 /*  4110 */   471,  473,  519,  104,  105,  519,  707,  577,  519,  519,
 /*  4120 */   519,  519,  193,  103,   98,   99,  101,  100,  102,  519,
 /*  4130 */   519,  524,  165,  166,  167,  168,  169,  170,  171,  172,
 /*  4140 */   173,  174,  175,  176,  177,  178,  179,  180,  181,  182,
 /*  4150 */   184,  519,  519,  519,  521,  519,  519,  519,  183,  164,
 /*  4160 */   519,  519,  519,  519,  467,  469,  471,  473,  519,  104,
 /*  4170 */   105,  386,  704,  577,   98,   99,  101,  100,  102,  103,
 /*  4180 */   528,  322,  324,  323,  293,  519,  519,  519,  519,  524,
 /*  4190 */   165,  166,  167,  168,  169,  170,  171,  172,  173,  174,
 /*  4200 */   175,  176,  177,  178,  179,  180,  181,  182,  184,  519,
 /*  4210 */   519,  519,  521,  632,  183,  164,  519,  519,  519,  104,
 /*  4220 */   105,  255,  467,  469,  471,  473,  519,  155,  519,  103,
 /*  4230 */   519,  322,  324,  323,  293,  519,  603,  519,   98,   99,
 /*  4240 */   101,  100,  102,  519,  519,  524,  165,  166,  167,  168,
 /*  4250 */   169,  170,  171,  172,  173,  174,  175,  176,  177,  178,
 /*  4260 */   179,  180,  181,  182,  184,  519,  519,  519,  521,  519,
 /*  4270 */   519,  519,  183,  164,  519,  519,  519,  519,  467,  469,
 /*  4280 */   471,  473,  519,  104,  105,  519,  712,  106,  519,  519,
 /*  4290 */   519,  519,  519,  103,  575,  519,   98,   99,  101,  100,
 /*  4300 */   102,  519,  519,  524,  165,  166,  167,  168,  169,  170,
 /*  4310 */   171,  172,  173,  174,  175,  176,  177,  178,  179,  180,
 /*  4320 */   181,  182,  184,  519,  519,  519,  521,  519,  183,  164,
 /*  4330 */   519,  519,  519,  305,  519,  519,  467,  469,  471,  473,
 /*  4340 */   519,  104,  105,  322,  324,  323,  293,  126,  519,  353,
 /*  4350 */   535,  103,  361,  357,  360,  519,  519,  519,  519,  524,
 /*  4360 */   165,  166,  167,  168,  169,  170,  171,  172,  173,  174,
 /*  4370 */   175,  176,  177,  178,  179,  180,  181,  182,  184,  519,
 /*  4380 */   519,  519,  521,  519,  519,  519,  183,  164,  519,  519,
 /*  4390 */   519,  519,  467,  469,  471,  473,  519,  149,  519,  519,
 /*  4400 */   151,  641,  647,  519,  646,  631,  519,  519,  597,  519,
 /*  4410 */   137,  607,  519,  519,  519,  519,  122,  524,  165,  166,
 /*  4420 */   167,  168,  169,  170,  171,  172,  173,  174,  175,  176,
 /*  4430 */   177,  178,  179,  180,  181,  182,  184,  519,  519,  519,
 /*  4440 */   521,  519,  183,  164,  519,  519,  519,  305,  519,  519,
 /*  4450 */   467,  469,  471,  473,  519,  519,  519,  322,  324,  323,
 /*  4460 */   293,  519,  519,  519,  519,  519,  356,  357,  360,  519,
 /*  4470 */   519,  519,  519,  163,  165,  166,  167,  168,  169,  170,
 /*  4480 */   171,  172,  173,  174,  175,  176,  177,  178,  179,  180,
 /*  4490 */   181,  182,  184,  519,  519,  519,  521,  519,  232,  241,
 /*  4500 */   244,  235,  238,  519,  476,  519,  467,  469,  471,  473,
 /*  4510 */   281,  519,  519,  519,  322,  324,  323,  293,  519,  519,
 /*  4520 */   519,  519,  229,  289,  294,  308,  226,  519,  304,  519,
 /*  4530 */   305,  285,  519,   98,   99,  101,  100,  102,  519,  315,
 /*  4540 */   322,  324,  323,  293,  303,  519,  275,  519,  519,  416,
 /*  4550 */   519,  359,  519,  519,  519,  519,  519,  273,  269,  271,
 /*  4560 */   519,  519,  228,  234,  240,  243,  246,  248,  237,  231,
 /*  4570 */   257,  259,  267,  261,  263,  265,  519,  519,  104,  105,
 /*  4580 */   519,  297,  254,  250,  519,  519,  155,  519,  103,  296,
 /*  4590 */   298,  299,  300,  301,  302,  230,  519,  232,  241,  244,
 /*  4600 */   235,  238,  674,  256,  387,  322,  324,  323,  293,  281,
 /*  4610 */   519,  519,  519,  519,  322,  324,  323,  293,  519,  519,
 /*  4620 */   519,  229,  289,  294,  308,  226,  344,  304,  519,  519,
 /*  4630 */   285,  519,  519,  519,  519,  519,  519,  519,  315,  519,
 /*  4640 */   519,    9,  519,  303,   12,  275,  138,  519,  655,  665,
 /*  4650 */   519,  519,  522,  519,  519,  519,  273,  269,  271,  519,
 /*  4660 */   519,  228,  234,  240,  243,  246,  248,  237,  231,  257,
 /*  4670 */   259,  267,  261,  263,  265,  519,  519,  519,  519,  519,
 /*  4680 */   297,  254,  250,  519,  519,  477,  519,  519,  296,  298,
 /*  4690 */   299,  300,  301,  302,  519,  519,  232,  241,  244,  235,
 /*  4700 */   238,  266,  519,  519,  519,  382,  523,  519,  281,  519,
 /*  4710 */   519,  322,  324,  323,  293,  322,  324,  323,  293,  519,
 /*  4720 */   229,  289,  294,  308,  226,  519,  304,  519,  519,  285,
 /*  4730 */   519,  519,  519,  519,  519,  295,  519,  315,  519,  519,
 /*  4740 */   519,  519,  303,  519,  275,  322,  324,  323,  293,  519,
 /*  4750 */   273,  269,  271,  519,  519,  228,  234,  240,  243,  246,
 /*  4760 */   248,  237,  231,  257,  259,  267,  261,  263,  265,  519,
 /*  4770 */   480,  519,  525,  519,  519,  254,  250,  519,  519,  297,
 /*  4780 */   322,  324,  323,  293,  519,  519,  534,  296,  298,  299,
 /*  4790 */   300,  301,  302,  245,  519,  232,  241,  244,  235,  238,
 /*  4800 */   519,  519,  519,  322,  324,  323,  293,  281,  519,  519,
 /*  4810 */   519,  519,  519,  519,  519,  519,  519,  519,  519,  229,
 /*  4820 */   289,  294,  308,  226,  519,  304,  519,  519,  285,  519,
 /*  4830 */   519,  519,  519,  519,  526,  519,  315,  519,  519,  519,
 /*  4840 */   519,  303,  519,  275,  322,  324,  323,  293,  519,  273,
 /*  4850 */   269,  271,  519,  519,  228,  234,  240,  243,  246,  248,
 /*  4860 */   237,  231,  257,  259,  267,  261,  263,  265,  519,  519,
 /*  4870 */   519,  537,  519,  519,  254,  250,  519,  519,  297,  519,
 /*  4880 */   519,  519,  519,  519,  519,  393,  296,  298,  299,  300,
 /*  4890 */   301,  302,  519,  519,  232,  241,  244,  235,  238,  519,
 /*  4900 */   519,  519,  519,  519,  519,  519,  281,  519,  519,  519,
 /*  4910 */   519,  519,  519,  519,  519,  519,  519,  519,  229,  289,
 /*  4920 */   294,  308,  226,  519,  304,  519,  519,  306,  519,  519,
 /*  4930 */   519,  519,  519,  519,  519,  315,  519,  519,  519,  519,
 /*  4940 */   303,  519,  275,  519,  519,  352,  519,  519,  273,  269,
 /*  4950 */   271,  519,  519,  228,  234,  240,  243,  246,  248,  237,
 /*  4960 */   231,  257,  259,  267,  261,  263,  265,  519,  519,  519,
 /*  4970 */   519,  519,  519,  254,  250,  519,  519,  351,  519,  519,
 /*  4980 */   519,  519,  519,  519,  527,  350,  298,  299,  300,  301,
 /*  4990 */   302,  519,  519,  232,  241,  244,  235,  238,  519,  519,
 /*  5000 */   519,  519,  519,  519,  519,  281,  519,  519,  519,  519,
 /*  5010 */   519,  519,  519,  519,  519,  519,  519,  229,  289,  294,
 /*  5020 */   308,  226,  519,  304,  519,  519,  285,  519,   98,   99,
 /*  5030 */   101,  100,  102,  519,  315,  519,  519,  519,  519,  303,
 /*  5040 */   519,  275,  519,  519,  444,  519,  519,  519,  519,  519,
 /*  5050 */   519,  519,  273,  269,  271,  519,  519,  228,  234,  240,
 /*  5060 */   243,  246,  248,  237,  231,  257,  259,  267,  261,  263,
 /*  5070 */   265,  519,  519,  104,  105,  519,  297,  254,  250,  519,
 /*  5080 */   519,  108,  519,  103,  296,  298,  299,  300,  301,  302,
 /*  5090 */   519,  519,  232,  241,  244,  235,  238,  519,  878,  519,
 /*  5100 */   519,  519,  519,  519,  281,  519,  519,  519,  519,  519,
 /*  5110 */   519,  519,  519,  519,  519,  519,  229,  289,  294,  308,
 /*  5120 */   226,  321,  304,  519,  519,  285,  519,  519,  519,  519,
 /*  5130 */   519,  519,  519,  315,  519,  519,  519,  519,  303,  519,
 /*  5140 */   275,  519,  519,  519,  519,  519,  519,  519,  519,  519,
 /*  5150 */   519,  273,  269,  271,  519,  519,  228,  234,  240,  243,
 /*  5160 */   246,  248,  237,  231,  257,  259,  267,  261,  263,  265,
 /*  5170 */   519,  519,  519,  519,  519,  297,  254,  250,  519,  519,
 /*  5180 */   481,  519,  519,  296,  298,  299,  300,  301,  302,  519,
 /*  5190 */   519,  232,  241,  244,  235,  238,  519,  519,  519,  519,
 /*  5200 */   519,  519,  519,  281,  519,  519,  519,  519,  519,  519,
 /*  5210 */   519,  519,  519,  519,  519,  229,  289,  294,  308,  226,
 /*  5220 */   375,  304,  519,  519,  285,  519,  519,  519,  519,  519,
 /*  5230 */   519,  519,  315,  519,  519,  519,  519,  303,  519,  275,
 /*  5240 */   519,  519,  519,  519,  519,  273,  269,  271,  519,  519,
 /*  5250 */   228,  234,  240,  243,  246,  248,  237,  231,  257,  259,
 /*  5260 */   267,  261,  263,  265,  519,  519,  519,  519,  519,  519,
 /*  5270 */   254,  250,  519,  519,  297,  519,  519,  519,  519,  519,
 /*  5280 */   519,  539,  296,  298,  299,  300,  301,  302,  519,  519,
 /*  5290 */   232,  241,  244,  235,  238,  519,  519,  519,  519,  519,
 /*  5300 */   519,  519,  281,  519,  519,  519,  519,  519,  519,  519,
 /*  5310 */   519,  519,  519,  519,  229,  289,  294,  308,  226,  331,
 /*  5320 */   304,  519,  519,  285,  519,  519,  519,  519,  519,  519,
 /*  5330 */   519,  315,  519,  519,  519,  519,  303,  519,  275,  519,
 /*  5340 */   519,  519,  519,  519,  519,  519,  519,  519,  519,  273,
 /*  5350 */   269,  271,  519,  519,  228,  234,  240,  243,  246,  248,
 /*  5360 */   237,  231,  257,  259,  267,  261,  263,  265,  519,  519,
 /*  5370 */   519,  519,  519,  297,  254,  250,  519,  519,  484,  519,
 /*  5380 */   519,  296,  298,  299,  300,  301,  302,  519,  519,  232,
 /*  5390 */   241,  244,  235,  238,  519,  519,  519,  519,  519,  519,
 /*  5400 */   519,  281,  519,  519,  519,  519,  519,  519,  519,  519,
 /*  5410 */   519,  519,  519,  229,  289,  294,  308,  226,  519,  304,
 /*  5420 */   519,  519,  285,  478,  519,  519,  519,  519,  519,  519,
 /*  5430 */   315,  519,  519,  519,  519,  303,  519,  275,  519,  519,
 /*  5440 */   519,  519,  519,  273,  269,  271,  519,  519,  228,  234,
 /*  5450 */   240,  243,  246,  248,  237,  231,  257,  259,  267,  261,
 /*  5460 */   263,  265,  519,  519,  519,  519,  519,  277,  254,  250,
 /*  5470 */   519,  519,  297,  519,  519,  519,  519,  519,  519,  519,
 /*  5480 */   296,  298,  299,  300,  301,  302,  519,  519,  232,  241,
 /*  5490 */   244,  235,  238,  519,  519,  519,  519,  519,  519,  519,
 /*  5500 */   281,  519,  519,  519,  519,  519,  519,  519,  519,  519,
 /*  5510 */   519,  519,  229,  289,  294,  308,  226,  364,  304,  519,
 /*  5520 */   519,  285,  519,  519,  519,  519,  519,  519,  519,  315,
 /*  5530 */   519,  519,  519,  519,  303,  519,  275,  519,  519,  519,
 /*  5540 */   519,  519,  273,  269,  271,  519,  519,  228,  234,  240,
 /*  5550 */   243,  246,  248,  237,  231,  257,  259,  267,  261,  263,
 /*  5560 */   265,  519,  519,  519,  519,  519,  519,  254,  250,  519,
 /*  5570 */   519,  297,  519,  519,  519,  519,  519,  519,  519,  296,
 /*  5580 */   298,  299,  300,  301,  302,  519,  519,  232,  241,  244,
 /*  5590 */   235,  238,  519,  519,  519,  519,  519,  519,  519,  281,
 /*  5600 */   519,  519,  519,  519,  519,  519,  519,  519,  519,  519,
 /*  5610 */   519,  229,  289,  294,  308,  226,  368,  304,  519,  519,
 /*  5620 */   285,  519,  519,  519,  519,  519,  519,  519,  315,  519,
 /*  5630 */   519,  519,  519,  303,  519,  275,  519,  519,  519,  519,
 /*  5640 */   519,  519,  269,  271,  519,  519,  228,  234,  240,  243,
 /*  5650 */   246,  248,  237,  231,  257,  259,  267,  261,  263,  265,
 /*  5660 */   519,  519,  519,  519,  519,  519,  254,  250,  519,  519,
 /*  5670 */   297,  519,  519,  519,  519,  519,  519,  519,  296,  298,
 /*  5680 */   299,  300,  301,  302,  519,  519,  232,  241,  244,  235,
 /*  5690 */   238,  519,  519,  519,  519,  519,  519,  519,  281,  519,
 /*  5700 */   519,  519,  519,  519,  519,  519,  519,  519,  519,  519,
 /*  5710 */   229,  289,  294,  308,  226,  349,  304,  519,  519,  285,
 /*  5720 */   519,  519,  519,  519,  519,  519,  519,  315,  519,  519,
 /*  5730 */   519,  519,  303,  519,  275,  519,  519,  228,  234,  240,
 /*  5740 */   243,  246,  248,  237,  231,  257,  259,  267,  261,  263,
 /*  5750 */   265,  519,  450,  411,  519,  519,  519,  254,  250,  519,
 /*  5760 */   519,  519,  519,  519,  519,  519,  402,  519,  519,  297,
 /*  5770 */   519,  519,  519,  519,  519,  519,  519,  296,  298,  299,
 /*  5780 */   300,  301,  302,  519,  519,  232,  241,  244,  235,  238,
 /*  5790 */   519,  519,  519,  519,  519,  519,  519,  281,  519,   94,
 /*  5800 */   403,  404,  405,  406,  407,  519,  439,  457,  458,  229,
 /*  5810 */   289,  294,  308,  226,  519,  304,  111,   94,  285,  519,
 /*  5820 */    68,  519,  519,  519,  519,  519,  315,  519,  519,  519,
 /*  5830 */   519,  303,  519,  275,  158,  519,  451,  519,   68,   47,
 /*  5840 */    48,   49,   50,   51,   52,   53,   54,   55,   56,   57,
 /*  5850 */   519,  519,  519,  519,  519,  519,  519,   47,   48,   49,
 /*  5860 */    50,   51,   52,   53,   54,   55,   56,   57,  297,  519,
 /*  5870 */   519,  519,  519,  519,  519,  519,  296,  298,  299,  300,
 /*  5880 */   301,  302,  519,  519,  232,  241,  244,  235,  238,  519,
 /*  5890 */   519,  519,  519,  519,  519,  519,  281,  519,   94,  519,
 /*  5900 */   519,  519,  519,  519,  519,  519,  519,  519,  229,  289,
 /*  5910 */   294,  308,  226,  338,  304,   29,   94,  285,  519,   68,
 /*  5920 */   519,  519,  519,  519,  519,  315,  519,  519,  519,  519,
 /*  5930 */   303,  519,  275,  582,  519,  519,  519,   68,   47,   48,
 /*  5940 */    49,   50,   51,   52,   53,   54,   55,   56,   57,  519,
 /*  5950 */   519,  519,  519,  519,  519,  519,   47,   48,   49,   50,
 /*  5960 */    51,   52,   53,   54,   55,   56,   57,  297,  519,  519,
 /*  5970 */   519,  519,  519,  519,  519,  296,  298,  299,  300,  301,
 /*  5980 */   302,  519,  519,  232,  241,  244,  235,  238,  519,  519,
 /*  5990 */   519,  519,  519,  519,  519,  281,   41,  519,  519,  519,
 /*  6000 */   415,  519,  519,  519,  519,  519,  519,  229,  289,  294,
 /*  6010 */   308,  226,  381,  304,  402,  519,  285,  519,   98,   99,
 /*  6020 */   101,  100,  102,  519,  315,  519,  519,  519,  519,  303,
 /*  6030 */   519,  275,  519,  519,   33,  519,   47,   48,   49,   50,
 /*  6040 */    51,   52,   53,   54,   55,   56,   57,  519,  403,  404,
 /*  6050 */   405,  406,  407,  519,  519,  428,  429,  519,  519,  519,
 /*  6060 */   519,  519,  519,  104,  105,  519,  297,  519,  519,  519,
 /*  6070 */   519,   26,  519,  103,  296,  298,  299,  300,  301,  302,
 /*  6080 */   519,  519,  232,  241,  244,  235,  238,  519,  519,  519,
 /*  6090 */   519,  519,  519,  519,  281,   94,  519,  519,  519,  519,
 /*  6100 */   519,  519,  519,  519,  519,  519,  229,  289,  294,  308,
 /*  6110 */   226,  311,  304,  519,  519,  285,   68,  519,  519,  519,
 /*  6120 */   519,  519,  519,  315,  519,  519,  519,  519,  303,  519,
 /*  6130 */   275,  519,  519,  519,  519,   47,   48,   49,   50,   51,
 /*  6140 */    52,   53,   54,   55,   56,   57,  519,  519,  519,  519,
 /*  6150 */   519,  519,  519,  519,  519,  519,  519,  519,  519,  519,
 /*  6160 */   519,  519,  519,  519,  519,  297,  519,  519,  519,  519,
 /*  6170 */   519,  519,  519,  296,  298,  299,  300,  301,  302,  519,
 /*  6180 */   519,  232,  241,  244,  235,  238,  519,  519,  519,  519,
 /*  6190 */   519,  519,  519,  281,  519,  519,  519,  519,  519,  519,
 /*  6200 */   519,  519,  519,  519,  519,  229,  289,  294,  308,  226,
 /*  6210 */   519,  304,  519,  519,  285,  519,  519,  519,  519,  519,
 /*  6220 */   519,  519,  315,  519,  519,  519,  519,  303,  519,  275,
 /*  6230 */   519,  519,  519,  519,  519,  519,  519,  519,  519,  519,
 /*  6240 */   519,  519,  519,  519,  519,  519,  519,  519,  519,  519,
 /*  6250 */   519,  519,  519,  519,  519,  519,  519,  519,  519,  519,
 /*  6260 */   519,  519,  519,  519,  297,  519,  519,  519,  519,  519,
 /*  6270 */   519,  519,  296,  298,  299,  300,  301,  302,  519,  519,
 /*  6280 */   232,  241,  244,  235,  238,  519,  519,  519,  519,  519,
 /*  6290 */   519,  519,  281,  519,  519,  519,  519,  519,  519,  519,
 /*  6300 */   519,  519,  519,  519,  229,  289,  294,  308,  226,  519,
 /*  6310 */   304,  519,  519,  306,  519,  519,  519,  519,  519,  519,
 /*  6320 */   519,  315,  519,  519,  519,  519,  303,  519,  275,  519,
 /*  6330 */   519,  519,  519,  519,  519,  519,  519,  519,  519,  519,
 /*  6340 */   519,  519,  519,  519,  519,  519,  519,  519,  519,  519,
 /*  6350 */   519,  519,  519,  519,  519,  519,  519,  519,  519,  519,
 /*  6360 */   519,  519,  519,  351,  519,  519,  519,  519,  519,  519,
 /*  6370 */   519,  350,  298,  299,  300,  301,  302,
};
static YYCODETYPE yy_lookahead[] = {
 /*     0 */     7,    8,    9,   10,   11,  136,    6,  138,  139,  140,
 /*    10 */   141,  142,   19,  106,  107,  108,  109,  110,  111,  112,
 /*    20 */   113,  114,   76,   49,   31,   32,   33,   34,   35,  170,
 /*    30 */    37,  172,  173,   40,   41,  122,  123,   40,  125,  126,
 /*    40 */    55,   48,   49,   50,  131,  132,   53,   50,   55,   75,
 /*    50 */   137,   77,   59,   60,   61,   62,   63,   64,   65,   66,
 /*    60 */    67,   76,   69,   70,   71,   72,    6,   74,  167,   37,
 /*    70 */    38,   78,   79,   80,   81,   51,   91,   84,  177,  178,
 /*    80 */   179,  180,   82,   90,   48,  184,  185,   94,   95,   96,
 /*    90 */    97,   98,   99,  100,  101,  102,  103,  104,    7,    8,
 /*   100 */     9,   10,   11,   25,   26,   27,   28,   29,   30,   49,
 /*   110 */    19,  134,   41,   40,    6,   37,   38,  140,  141,   48,
 /*   120 */   143,   48,   31,   32,   33,   34,   35,   40,   37,    6,
 /*   130 */    40,   40,   41,   28,   29,   30,   49,   50,   48,   48,
 /*   140 */    49,   50,   37,   38,   53,  170,   55,  172,  173,   41,
 /*   150 */    59,   60,   61,   62,   63,   64,   65,   66,   67,   40,
 /*   160 */    69,   70,   71,   72,   41,   74,  167,  181,  182,   78,
 /*   170 */    79,   80,   81,  129,  130,   84,  177,  178,  179,  180,
 /*   180 */   117,   90,  119,  184,  185,   94,   95,   96,   97,   98,
 /*   190 */    99,  100,  101,  102,  103,  104,    7,    8,    9,   10,
 /*   200 */    11,  121,   43,  123,   45,  125,  126,   48,   19,   90,
 /*   210 */   116,  131,  132,  139,  140,  141,  142,  137,   41,   49,
 /*   220 */    31,   32,   33,   34,   35,   48,   37,  134,   51,   40,
 /*   230 */    41,  166,  123,  140,  141,  126,  143,   48,   49,   50,
 /*   240 */   131,  132,   53,   40,   55,   75,  137,   77,   59,   60,
 /*   250 */    61,   62,   63,   64,   65,   66,   67,   40,   69,   70,
 /*   260 */    71,   72,  170,   74,  167,  173,   35,   78,   79,   80,
 /*   270 */    81,   40,  116,   84,  177,  178,  179,  180,    6,   90,
 /*   280 */     6,  184,  185,   94,   95,   96,   97,   98,   99,  100,
 /*   290 */   101,  102,  103,  104,    7,    8,    9,   10,   11,  167,
 /*   300 */   109,  110,  111,  112,  113,  114,   19,   41,   36,  177,
 /*   310 */   178,  179,  180,   40,   48,   41,  184,  185,   31,   32,
 /*   320 */    33,   34,   35,  126,   37,    6,   40,   40,   41,  123,
 /*   330 */   133,  125,  126,  126,  137,   48,   49,   50,  132,  132,
 /*   340 */    53,    6,   55,  137,  137,   44,   59,   60,   61,   62,
 /*   350 */    63,   64,   65,   66,   67,   36,   69,   70,   71,   72,
 /*   360 */    40,   74,  167,   51,    6,   78,   79,   80,   81,  165,
 /*   370 */   166,   84,  177,  178,  179,  180,   41,   90,    6,  184,
 /*   380 */   185,   94,   95,   96,   97,   98,   99,  100,  101,  102,
 /*   390 */   103,  104,    7,    8,    9,   10,   11,   85,   86,   87,
 /*   400 */    88,   89,   39,   20,   19,   42,   48,   44,   36,   46,
 /*   410 */    47,    6,   38,   50,   41,   41,   31,   32,   33,   34,
 /*   420 */    35,   48,   37,    6,   51,   40,   41,    6,  140,  141,
 /*   430 */   124,  143,  126,   48,   49,   50,   40,  131,   53,  133,
 /*   440 */    55,   36,   40,  137,   59,   60,   61,   62,   63,   64,
 /*   450 */    65,   66,   67,   36,   69,   70,   71,   72,   38,   74,
 /*   460 */   167,   41,   41,   78,   79,   80,   81,  104,   55,   84,
 /*   470 */   177,  178,  179,  180,  137,   90,   40,  184,  185,   94,
 /*   480 */    95,   96,   97,   98,   99,  100,  101,  102,  103,  104,
 /*   490 */     7,    8,    9,   10,   11,  167,   43,   40,   45,  181,
 /*   500 */   182,   48,   19,   55,   91,  177,  178,  179,  180,  168,
 /*   510 */   169,    6,  184,  185,   31,   32,   33,   34,   35,   45,
 /*   520 */    37,  134,   48,   40,   41,    6,   90,  140,  141,   43,
 /*   530 */   143,   48,   49,   50,   48,    6,   53,  170,   55,  172,
 /*   540 */   173,   36,   59,   60,   61,   62,   63,   64,   65,   66,
 /*   550 */    67,   20,   69,   70,   71,   72,   38,   74,  167,    6,
 /*   560 */    41,   78,   79,   80,   81,   36,   48,   84,  177,  178,
 /*   570 */   179,  180,  117,   90,  119,  184,  185,   94,   95,   96,
 /*   580 */    97,   98,   99,  100,  101,  102,  103,  104,    7,    8,
 /*   590 */     9,   10,   11,  120,   41,  122,  126,  124,  170,  126,
 /*   600 */    19,  173,  134,    6,  131,   55,  133,  137,  140,  141,
 /*   610 */   137,  143,   31,   32,   33,   34,   35,  126,   37,  181,
 /*   620 */   182,   40,   41,  181,  182,   38,    6,   55,  137,   48,
 /*   630 */    49,   50,   55,   36,   53,   48,   55,  181,  182,   38,
 /*   640 */    59,   60,   61,   62,   63,   64,   65,   66,   67,   48,
 /*   650 */    69,   70,   71,   72,   37,   74,   36,  181,  182,   78,
 /*   660 */    79,   80,   81,   91,   40,   84,  181,  182,   51,   43,
 /*   670 */    38,   90,   48,   41,   48,   94,   95,   96,   97,   98,
 /*   680 */    99,  100,  101,  102,  103,  104,    7,    8,    9,   10,
 /*   690 */    11,  136,   36,  138,  139,  140,  141,  142,   19,   40,
 /*   700 */     6,    6,   85,   86,   87,   88,   89,  181,  182,   40,
 /*   710 */    31,   32,   33,   34,   35,  126,   37,    6,    6,   40,
 /*   720 */    41,  117,   53,  119,  181,  182,  137,   48,   49,   50,
 /*   730 */    36,   36,   53,    6,   55,    6,    6,    6,   59,   60,
 /*   740 */    61,   62,   63,   64,   65,   66,   67,   40,   69,   70,
 /*   750 */    71,   72,   37,   74,   51,   48,  127,   78,   79,   80,
 /*   760 */    81,   38,    6,   84,   41,   36,   51,   51,   41,   90,
 /*   770 */    58,   41,   41,   94,   95,   96,   97,   98,   99,  100,
 /*   780 */   101,  102,  103,  104,    7,    8,    9,   10,   11,  136,
 /*   790 */     6,  138,  139,  140,  141,  142,   19,   41,    6,   20,
 /*   800 */    85,   86,   87,   88,   89,    6,  181,  182,   31,   32,
 /*   810 */    33,   34,   35,   38,   37,  182,   37,   40,   41,  170,
 /*   820 */    36,   40,  173,   48,   82,   48,   49,   50,   48,   58,
 /*   830 */    53,   40,   55,   41,   49,   36,   59,   60,   61,   62,
 /*   840 */    63,   64,   65,   66,   67,  127,   69,   70,   71,   72,
 /*   850 */    41,   74,  167,   79,  115,   78,   79,   80,   81,  116,
 /*   860 */    41,   84,  177,  178,  179,  180,   41,   90,   53,  184,
 /*   870 */   185,   94,   95,   96,   97,   98,   99,  100,  101,  102,
 /*   880 */   103,  104,    7,    8,    9,   10,   11,  167,   76,   41,
 /*   890 */    20,   41,   41,   73,   19,  175,  176,  177,  178,  179,
 /*   900 */   180,   48,   40,   40,  116,   48,   31,   32,   33,   34,
 /*   910 */    35,   55,   37,   55,  170,   40,   41,  170,   41,   55,
 /*   920 */    40,  170,   49,   48,   49,   50,   40,   48,   53,  170,
 /*   930 */    55,   40,   49,   48,   59,   60,   61,   62,   63,   64,
 /*   940 */    65,   66,   67,   40,   69,   70,   71,   72,  116,   74,
 /*   950 */   167,   40,   55,   78,   79,   80,   81,   49,   41,   84,
 /*   960 */   177,  178,  179,  180,  127,   90,  170,  184,  185,   94,
 /*   970 */    95,   96,   97,   98,   99,  100,  101,  102,  103,  104,
 /*   980 */     7,    8,    9,   10,   11,  167,   58,  116,   48,  171,
 /*   990 */    15,   40,   19,   40,   49,  177,  178,  179,  180,   53,
 /*  1000 */    91,   76,   48,   51,   31,   32,   33,   34,   35,  116,
 /*  1010 */    37,   41,  169,   40,   41,   51,   35,   40,   55,  115,
 /*  1020 */    41,   48,   49,   50,   40,    0,   53,  128,   55,   79,
 /*  1030 */   128,   41,   59,   60,   61,   62,   63,   64,   65,   66,
 /*  1040 */    67,  127,   69,   70,   71,   72,  164,   74,   51,  167,
 /*  1050 */    40,   78,   79,   80,   81,   41,   49,   84,   41,  177,
 /*  1060 */   178,  179,  180,   90,   40,  130,   55,   94,   95,   96,
 /*  1070 */    97,   98,   99,  100,  101,  102,  103,  104,    7,    8,
 /*  1080 */     9,   10,   11,  167,  127,   48,   40,  171,   41,   40,
 /*  1090 */    19,   53,   41,  177,  178,  179,  180,   41,   40,   51,
 /*  1100 */   128,   41,   31,   32,   33,   34,   35,   48,   37,  127,
 /*  1110 */    49,   40,   41,  128,   41,   41,   49,   55,   55,   48,
 /*  1120 */    49,   50,   40,   48,   53,   55,   55,  127,   49,   48,
 /*  1130 */    59,   60,   61,   62,   63,   64,   65,   66,   67,   41,
 /*  1140 */    69,   70,   71,   72,  164,   74,  119,  167,   76,   78,
 /*  1150 */    79,   80,   81,  127,   48,   84,   73,  177,  178,  179,
 /*  1160 */   180,   90,  127,   41,   44,   94,   95,   96,   97,   98,
 /*  1170 */    99,  100,  101,  102,  103,  104,    7,    8,    9,   10,
 /*  1180 */    11,  136,  167,  138,  139,  140,  141,  142,   19,  174,
 /*  1190 */    76,   40,  177,  178,  179,  180,  116,  189,  189,  189,
 /*  1200 */    31,   32,   33,   34,   35,  116,   37,  189,  189,   40,
 /*  1210 */    41,  127,  189,  189,  189,  189,  189,   48,   49,   50,
 /*  1220 */   189,  189,   53,  189,   55,  189,  189,  189,   59,   60,
 /*  1230 */    61,   62,   63,   64,   65,   66,   67,  189,   69,   70,
 /*  1240 */    71,   72,  189,   74,  189,  167,  189,   78,   79,   80,
 /*  1250 */    81,  189,  189,   84,  176,  177,  178,  179,  180,   90,
 /*  1260 */   189,  189,  189,   94,   95,   96,   97,   98,   99,  100,
 /*  1270 */   101,  102,  103,  104,    7,    8,    9,   10,   11,  167,
 /*  1280 */   189,  189,  189,  171,  189,  189,   19,  189,  189,  177,
 /*  1290 */   178,  179,  180,  189,  189,  189,  189,  189,   31,   32,
 /*  1300 */    33,   34,   35,  189,   37,  189,  189,   40,   41,  189,
 /*  1310 */   189,  189,  189,  189,  189,   48,   49,   50,  189,  189,
 /*  1320 */    53,  189,   55,  189,  189,  189,   59,   60,   61,   62,
 /*  1330 */    63,   64,   65,   66,   67,  189,   69,   70,   71,   72,
 /*  1340 */   164,   74,  189,  167,  189,   78,   79,   80,   81,  189,
 /*  1350 */   189,   84,  189,  177,  178,  179,  180,   90,  189,  189,
 /*  1360 */   189,   94,   95,   96,   97,   98,   99,  100,  101,  102,
 /*  1370 */   103,  104,    7,    8,    9,   10,   11,  167,  189,  189,
 /*  1380 */   189,  171,  189,  189,   19,  189,  189,  177,  178,  179,
 /*  1390 */   180,  189,  189,  189,  189,  189,   31,   32,   33,   34,
 /*  1400 */    35,  189,   37,  189,  189,   40,   41,  189,  189,  189,
 /*  1410 */   189,  189,  189,   48,   49,   50,  189,  189,   53,  189,
 /*  1420 */    55,  189,  189,  189,   59,   60,   61,   62,   63,   64,
 /*  1430 */    65,   66,   67,  189,   69,   70,   71,   72,  189,   74,
 /*  1440 */   167,  189,  189,   78,   79,   80,   81,  189,  189,   84,
 /*  1450 */   177,  178,  179,  180,  189,   90,  189,  189,  185,   94,
 /*  1460 */    95,   96,   97,   98,   99,  100,  101,  102,  103,  104,
 /*  1470 */     7,    8,    9,   10,   11,  167,  189,  189,  189,  171,
 /*  1480 */   189,  189,   19,  189,  189,  177,  178,  179,  180,  189,
 /*  1490 */   189,  189,  189,  189,   31,   32,   33,   34,   35,  189,
 /*  1500 */    37,  189,  189,   40,   41,  189,  189,  189,  189,  189,
 /*  1510 */   189,   48,   49,   50,  189,  189,   53,  189,   55,  189,
 /*  1520 */   189,  189,   59,   60,   61,   62,   63,   64,   65,   66,
 /*  1530 */    67,  189,   69,   70,   71,   72,  164,   74,  189,  167,
 /*  1540 */   189,   78,   79,   80,   81,  189,  189,   84,  189,  177,
 /*  1550 */   178,  179,  180,   90,  189,  189,  189,   94,   95,   96,
 /*  1560 */    97,   98,   99,  100,  101,  102,  103,  104,    7,    8,
 /*  1570 */     9,   10,   11,  167,  189,  189,  189,  171,  189,  189,
 /*  1580 */    19,  189,  189,  177,  178,  179,  180,  189,  189,  189,
 /*  1590 */   189,  189,   31,   32,   33,   34,   35,  189,   37,  189,
 /*  1600 */   189,   40,   41,  189,  189,  189,  189,  189,  189,   48,
 /*  1610 */    49,   50,  189,  189,   53,  189,   55,  189,  189,  189,
 /*  1620 */    59,   60,   61,   62,   63,   64,   65,   66,   67,  189,
 /*  1630 */    69,   70,   71,   72,  164,   74,  189,  167,  189,   78,
 /*  1640 */    79,   80,   81,  189,  189,   84,  189,  177,  178,  179,
 /*  1650 */   180,   90,  189,  189,  189,   94,   95,   96,   97,   98,
 /*  1660 */    99,  100,  101,  102,  103,  104,    7,    8,    9,   10,
 /*  1670 */    11,  167,  189,  189,  189,  171,  189,  189,   19,  189,
 /*  1680 */   189,  177,  178,  179,  180,  189,  189,  189,  189,  189,
 /*  1690 */    31,   32,   33,   34,   35,  189,   37,  189,  189,   40,
 /*  1700 */    41,  189,  189,  189,  189,  189,  189,   48,   49,   50,
 /*  1710 */   189,  189,   53,  189,   55,  189,  189,  189,   59,   60,
 /*  1720 */    61,   62,   63,   64,   65,   66,   67,  189,   69,   70,
 /*  1730 */    71,   72,  189,   74,  167,  189,  189,   78,   79,   80,
 /*  1740 */    81,  189,  189,   84,  177,  178,  179,  180,  189,   90,
 /*  1750 */   189,  189,  189,   94,   95,   96,   97,   98,   99,  100,
 /*  1760 */   101,  102,  103,  104,    7,    8,    9,   10,   11,  167,
 /*  1770 */   189,  189,  189,  171,  189,  189,   19,  189,  189,  177,
 /*  1780 */   178,  179,  180,  189,  189,  189,  189,  189,   31,   32,
 /*  1790 */    33,   34,   35,  189,   37,  189,  189,   40,   41,  189,
 /*  1800 */   189,  189,  189,  189,  189,   48,   49,   50,  189,  189,
 /*  1810 */    53,  189,   55,  189,  189,  189,   59,   60,   61,   62,
 /*  1820 */    63,   64,   65,   66,   67,  189,   69,   70,   71,   72,
 /*  1830 */   189,   74,  167,  189,  189,   78,   79,   80,   81,  189,
 /*  1840 */   189,   84,  177,  178,  179,  180,  189,   90,  189,  189,
 /*  1850 */   189,   94,   95,   96,   97,   98,   99,  100,  101,  102,
 /*  1860 */   103,  104,    7,    8,    9,   10,   11,  167,  189,  189,
 /*  1870 */   189,  171,  189,  189,   19,  189,  189,  177,  178,  179,
 /*  1880 */   180,  189,  189,  189,  189,  189,   31,   32,   33,   34,
 /*  1890 */    35,  189,   37,  189,  189,   40,   41,  189,  189,  189,
 /*  1900 */   189,  189,  189,   48,   49,   50,  189,  189,   53,  189,
 /*  1910 */    55,  189,  189,  189,   59,   60,   61,   62,   63,   64,
 /*  1920 */    65,   66,   67,  189,   69,   70,   71,   72,  189,   74,
 /*  1930 */   167,  189,  189,   78,   79,   80,   81,  189,  189,   84,
 /*  1940 */   177,  178,  179,  180,  189,   90,  189,  189,  189,   94,
 /*  1950 */    95,   96,   97,   98,   99,  100,  101,  102,  103,  104,
 /*  1960 */     7,    8,    9,   10,   11,  167,  189,  189,  189,  171,
 /*  1970 */   189,  189,   19,  189,  189,  177,  178,  179,  180,  189,
 /*  1980 */   189,  189,  189,  189,   31,   32,   33,   34,   35,  189,
 /*  1990 */    37,  189,  189,   40,   41,  189,  189,  189,  189,  189,
 /*  2000 */   189,   48,   49,   50,  189,  189,   53,  189,   55,  189,
 /*  2010 */   189,  189,   59,   60,   61,   62,   63,   64,   65,   66,
 /*  2020 */    67,  189,   69,   70,   71,   72,  189,   74,  167,  189,
 /*  2030 */   189,   78,   79,   80,   81,  189,  189,   84,  177,  178,
 /*  2040 */   179,  180,  189,   90,  189,  189,  189,   94,   95,   96,
 /*  2050 */    97,   98,   99,  100,  101,  102,  103,  104,    7,    8,
 /*  2060 */     9,   10,   11,  167,  189,  189,  189,  171,  189,  189,
 /*  2070 */    19,  189,  189,  177,  178,  179,  180,  189,  189,  189,
 /*  2080 */   189,  189,   31,   32,   33,   34,   35,  189,   37,  189,
 /*  2090 */   189,   40,   41,  189,  189,  189,  189,  189,  189,   48,
 /*  2100 */    49,   50,  189,  189,   53,  189,   55,  189,  189,  189,
 /*  2110 */    59,   60,   61,   62,   63,   64,   65,   66,   67,  189,
 /*  2120 */    69,   70,   71,   72,  189,   74,  167,  189,  189,   78,
 /*  2130 */    79,   80,   81,  189,  140,   84,  177,  178,  179,  180,
 /*  2140 */   189,   90,  189,  189,  189,   94,   95,   96,   97,   98,
 /*  2150 */    99,  100,  101,  102,  103,  104,    7,    8,    9,   10,
 /*  2160 */    11,  167,  189,  189,  189,  189,  189,  189,   19,  189,
 /*  2170 */   189,  177,  178,  179,  180,  189,  189,  189,  189,  189,
 /*  2180 */    31,   32,   33,   34,   35,  189,   37,  189,  189,   40,
 /*  2190 */    41,  189,  189,  189,  189,  189,  189,   48,   49,   50,
 /*  2200 */   189,  189,   53,  189,   55,  189,  189,  189,   59,   60,
 /*  2210 */    61,   62,   63,   64,   65,   66,   67,  189,   69,   70,
 /*  2220 */    71,   72,  189,   74,  167,  189,  189,   78,   79,   80,
 /*  2230 */    81,  189,  189,   84,  177,  178,  179,  180,  189,   90,
 /*  2240 */   189,  189,  189,   94,   95,   96,   97,   98,   99,  100,
 /*  2250 */   101,  102,  103,  104,    7,    8,    9,   10,   11,  136,
 /*  2260 */   189,  138,  139,  140,  141,  142,   19,  136,  189,  138,
 /*  2270 */   139,  140,  141,  142,  189,  189,  189,  189,   31,   32,
 /*  2280 */    33,   34,   35,  189,   37,  189,  189,   40,   41,  189,
 /*  2290 */   189,  189,  189,  189,  189,   48,   49,   50,  189,  189,
 /*  2300 */    53,  189,   55,  189,  189,  189,   59,   60,   61,   62,
 /*  2310 */    63,   64,   65,   66,   67,  189,   69,   70,   71,   72,
 /*  2320 */   189,   74,  167,  189,  189,   78,   79,   80,   81,  189,
 /*  2330 */   189,   84,  177,  178,  179,  180,  189,   90,  189,  189,
 /*  2340 */   189,   94,   95,   96,   97,   98,   99,  100,  101,  102,
 /*  2350 */   103,  104,    7,    8,    9,   10,   11,  136,  189,  138,
 /*  2360 */   139,  140,  141,  142,   19,  136,  189,  138,  139,  140,
 /*  2370 */   141,  142,  189,  189,  189,  189,   31,   32,   33,   34,
 /*  2380 */    35,  189,   37,  189,  189,   40,   41,  189,  189,  189,
 /*  2390 */   189,  189,  189,   48,   49,   50,  189,  189,   53,  189,
 /*  2400 */    55,  189,  189,  189,   59,   60,   61,   62,   63,   64,
 /*  2410 */    65,   66,   67,  189,   69,   70,   71,   72,  189,   74,
 /*  2420 */   167,  189,  189,   78,   79,   80,   81,  189,  189,   84,
 /*  2430 */   177,  178,  179,  180,  189,   90,  189,  189,  189,   94,
 /*  2440 */    95,   96,   97,   98,   99,  100,  101,  102,  103,  104,
 /*  2450 */     7,    8,    9,   10,   11,  167,  189,  189,  189,  189,
 /*  2460 */   189,  189,   19,  189,  189,  177,  178,  179,  180,  189,
 /*  2470 */   189,  189,  189,  189,   31,   32,   33,   34,   35,  189,
 /*  2480 */    37,  189,  189,   40,   41,  189,  189,  189,  189,  189,
 /*  2490 */   189,   48,   49,   50,  189,  189,   53,  189,   55,  189,
 /*  2500 */   189,  189,   59,   60,   61,   62,   63,   64,   65,   66,
 /*  2510 */    67,  189,   69,   70,   71,   72,  189,   74,  167,  189,
 /*  2520 */   189,   78,   79,   80,   81,  189,  189,   84,  177,  178,
 /*  2530 */   179,  180,  189,   90,  189,  189,  189,   94,   95,   96,
 /*  2540 */    97,   98,   99,  100,  101,  102,  103,  104,    7,    8,
 /*  2550 */     9,   10,   11,  167,  189,  189,  189,  189,  189,  189,
 /*  2560 */    19,  189,  189,  177,  178,  179,  180,  189,  189,  189,
 /*  2570 */   189,  189,   31,   32,   33,   34,   35,  189,   37,  189,
 /*  2580 */   189,   40,   41,  189,  189,  189,  189,  189,  189,   48,
 /*  2590 */    49,   50,  189,  189,   53,  189,   55,  189,  189,  189,
 /*  2600 */    59,   60,   61,   62,   63,   64,   65,   66,   67,  189,
 /*  2610 */    69,   70,   71,   72,  189,   74,  167,  189,  189,   78,
 /*  2620 */    79,   80,   81,  189,  189,   84,  177,  178,  179,  180,
 /*  2630 */   189,   90,  189,  189,  189,   94,   95,   96,   97,   98,
 /*  2640 */    99,  100,  101,  102,  103,  104,    7,    8,    9,   10,
 /*  2650 */    11,  167,  189,  189,  189,  189,  189,  189,   19,  189,
 /*  2660 */   189,  177,  178,  179,  180,  189,  189,  189,  189,  189,
 /*  2670 */    31,   32,   33,   34,   35,  189,   37,  189,  189,   40,
 /*  2680 */    41,  189,  189,  189,  189,  189,  189,   48,   49,   50,
 /*  2690 */   189,  189,   53,  189,   55,  189,  189,  189,   59,   60,
 /*  2700 */    61,   62,   63,   64,   65,   66,   67,  189,   69,   70,
 /*  2710 */    71,   72,  189,   74,  167,  189,  189,   78,   79,   80,
 /*  2720 */    81,  189,  189,   84,  177,  178,  179,  180,  189,   90,
 /*  2730 */   189,  189,  189,   94,   95,   96,   97,   98,   99,  100,
 /*  2740 */   101,  102,  103,  104,    7,    8,    9,   10,   11,  167,
 /*  2750 */   189,  189,  189,  189,  189,  189,   19,  189,  189,  177,
 /*  2760 */   178,  179,  180,  189,  189,  189,  189,  189,   31,   32,
 /*  2770 */    33,   34,   35,  189,   37,  189,  189,   40,   41,  189,
 /*  2780 */   189,  189,  189,  189,  189,   48,   49,   50,  189,  189,
 /*  2790 */    53,  189,   55,  189,  189,  189,   59,   60,   61,   62,
 /*  2800 */    63,   64,   65,   66,   67,  189,   69,   70,   71,   72,
 /*  2810 */   189,   74,  167,  189,  189,   78,   79,   80,   81,  189,
 /*  2820 */   189,   84,  177,  178,  179,  180,  189,   90,  189,  189,
 /*  2830 */   189,   94,   95,   96,   97,   98,   99,  100,  101,  102,
 /*  2840 */   103,  104,    7,    8,    9,   10,   11,  167,  189,  189,
 /*  2850 */   189,  189,  189,  189,   19,  189,  189,  177,  178,  179,
 /*  2860 */   180,  189,  189,  189,  189,  189,   31,   32,   33,   34,
 /*  2870 */    35,  189,   37,  189,  189,   40,   41,  189,  189,  189,
 /*  2880 */   189,  189,  189,   48,   49,   50,  189,  189,   53,  189,
 /*  2890 */    55,  189,  189,  189,   59,   60,   61,   62,   63,   64,
 /*  2900 */    65,   66,   67,  189,   69,   70,   71,   72,  189,   74,
 /*  2910 */   167,  189,  189,   78,   79,   80,   81,  189,  189,   84,
 /*  2920 */   177,  178,  179,  180,  189,   90,  189,  189,  189,   94,
 /*  2930 */    95,   96,   97,   98,   99,  100,  101,  102,  103,  104,
 /*  2940 */     7,    8,    9,   10,   11,  167,  189,  189,  189,  189,
 /*  2950 */   189,  189,   19,  189,  189,  177,  178,  179,  180,  189,
 /*  2960 */   189,  189,  189,  189,   31,   32,   33,   34,   35,  189,
 /*  2970 */    37,  189,  189,   40,   41,  189,  189,  189,  189,  189,
 /*  2980 */   189,   48,   49,   50,  189,  189,   53,  189,   55,  189,
 /*  2990 */   189,  189,   59,   60,   61,   62,   63,   64,   65,   66,
 /*  3000 */    67,  189,   69,   70,   71,   72,  189,   74,  167,  189,
 /*  3010 */   189,   78,   79,   80,   81,  189,  189,   84,  177,  178,
 /*  3020 */   179,  180,  189,   90,  189,  189,  189,   94,   95,   96,
 /*  3030 */    97,   98,   99,  100,  101,  102,  103,  104,    7,    8,
 /*  3040 */     9,   10,   11,  167,  189,  189,  189,  189,  189,  189,
 /*  3050 */    19,  189,  189,  177,  178,  179,  180,  189,  189,  189,
 /*  3060 */   189,  189,   31,   32,   33,   34,   35,  189,   37,  189,
 /*  3070 */   189,   40,   41,  189,  189,  189,  189,  189,  189,   48,
 /*  3080 */    49,   50,  189,  189,   53,  189,   55,  189,  189,  189,
 /*  3090 */    59,   60,   61,   62,   63,   64,   65,   66,   67,  189,
 /*  3100 */    69,   70,   71,   72,  189,   74,  167,  189,  189,   78,
 /*  3110 */    79,   80,   81,  189,  189,   84,  177,  178,  179,  180,
 /*  3120 */   189,   90,  189,  189,  189,   94,   95,   96,   97,   98,
 /*  3130 */    99,  100,  101,  102,  103,  104,    7,    8,    9,   10,
 /*  3140 */    11,  167,  189,  189,  189,  189,  189,  189,   19,  189,
 /*  3150 */   189,  177,  178,  179,  180,  189,  189,  189,  189,  189,
 /*  3160 */    31,   32,   33,   34,   35,  189,   37,  189,  189,   40,
 /*  3170 */    41,  189,  189,  189,  189,  189,  189,   48,  189,   50,
 /*  3180 */   189,  189,   53,  189,   55,  189,  189,  189,   59,   60,
 /*  3190 */    61,   62,   63,   64,   65,   66,   67,  189,   69,   70,
 /*  3200 */    71,   72,  189,   74,  167,  189,  189,   78,   79,   80,
 /*  3210 */    81,  189,  189,   84,  177,  178,  179,  180,  189,   90,
 /*  3220 */   189,  189,  189,   94,   95,   96,   97,   98,   99,  100,
 /*  3230 */   101,  102,  103,  104,    7,    8,    9,   10,   11,  189,
 /*  3240 */   189,  189,  189,  189,  189,  189,   19,  189,  189,  189,
 /*  3250 */    40,  189,    1,    2,    3,    4,    5,  189,   31,   32,
 /*  3260 */    33,   34,   35,   53,   37,  189,  167,   40,  189,  189,
 /*  3270 */   189,  189,  189,  189,  189,   48,  177,  178,  179,  180,
 /*  3280 */    53,  189,   55,  189,  189,  189,   59,   60,   61,   62,
 /*  3290 */    63,   64,   65,   66,   67,   68,   69,   46,   47,  189,
 /*  3300 */    90,  189,  113,  114,  189,   54,  189,   56,   98,   99,
 /*  3310 */   100,  101,  102,  103,  189,  189,  189,   90,  189,    1,
 /*  3320 */     2,    3,    4,    5,  135,   98,   99,  100,  101,  102,
 /*  3330 */   103,  189,  189,  144,  145,  146,  147,  148,  149,  150,
 /*  3340 */   151,  152,  153,  154,  155,  156,  157,  158,  159,  160,
 /*  3350 */   161,  162,  163,  189,  189,  189,  167,  189,   40,  189,
 /*  3360 */   113,  114,  189,  189,   46,   47,  177,  178,  179,  180,
 /*  3370 */   189,  189,   54,  189,   56,  189,    1,    2,    3,    4,
 /*  3380 */     5,  189,  135,  189,    1,    2,    3,    4,    5,  189,
 /*  3390 */   189,  144,  145,  146,  147,  148,  149,  150,  151,  152,
 /*  3400 */   153,  154,  155,  156,  157,  158,  159,  160,  161,  162,
 /*  3410 */   163,  189,  189,  189,  167,  189,  113,  114,  189,  189,
 /*  3420 */   189,   46,   47,  189,  177,  178,  179,  180,  189,   46,
 /*  3430 */    47,   56,   49,   50,  189,   52,  189,  189,  135,   56,
 /*  3440 */   189,  189,  189,  189,   19,  189,  189,  144,  145,  146,
 /*  3450 */   147,  148,  149,  150,  151,  152,  153,  154,  155,  156,
 /*  3460 */   157,  158,  159,  160,  161,  162,  163,  189,  189,  189,
 /*  3470 */   167,  189,  189,  189,  113,  114,  189,  189,  189,  189,
 /*  3480 */   177,  178,  179,  180,   59,   60,   61,   62,   63,   64,
 /*  3490 */    65,   66,   67,   68,   69,  189,  135,  189,    1,    2,
 /*  3500 */     3,    4,    5,  189,  189,  144,  145,  146,  147,  148,
 /*  3510 */   149,  150,  151,  152,  153,  154,  155,  156,  157,  158,
 /*  3520 */   159,  160,  161,  162,  163,  189,  189,  189,  167,  189,
 /*  3530 */   113,  114,  189,  189,  189,  189,  189,  189,  177,  178,
 /*  3540 */   179,  180,  189,   46,   47,  189,   49,   50,  189,   52,
 /*  3550 */   189,  189,  135,   56,    1,    2,    3,    4,    5,  189,
 /*  3560 */   189,  144,  145,  146,  147,  148,  149,  150,  151,  152,
 /*  3570 */   153,  154,  155,  156,  157,  158,  159,  160,  161,  162,
 /*  3580 */   163,  189,  189,  189,  167,  189,  189,  189,  113,  114,
 /*  3590 */   189,  189,  189,  189,  177,  178,  179,  180,  189,   46,
 /*  3600 */    47,  189,   49,   50,  189,   52,  189,  189,  189,   56,
 /*  3610 */   135,  189,    1,    2,    3,    4,    5,  189,  189,  144,
 /*  3620 */   145,  146,  147,  148,  149,  150,  151,  152,  153,  154,
 /*  3630 */   155,  156,  157,  158,  159,  160,  161,  162,  163,  189,
 /*  3640 */   189,  189,  167,  189,  113,  114,  189,  189,  189,  189,
 /*  3650 */   189,  189,  177,  178,  179,  180,  189,   46,   47,  189,
 /*  3660 */    49,   50,  189,   52,  189,  189,  135,   56,    1,    2,
 /*  3670 */     3,    4,    5,  189,  189,  144,  145,  146,  147,  148,
 /*  3680 */   149,  150,  151,  152,  153,  154,  155,  156,  157,  158,
 /*  3690 */   159,  160,  161,  162,  163,  189,  189,  189,  167,  189,
 /*  3700 */   189,  189,  113,  114,  189,  189,  189,  189,  177,  178,
 /*  3710 */   179,  180,  189,   46,   47,  189,   49,   50,  189,   52,
 /*  3720 */   189,  189,  189,   56,  135,  189,    1,    2,    3,    4,
 /*  3730 */     5,  189,  189,  144,  145,  146,  147,  148,  149,  150,
 /*  3740 */   151,  152,  153,  154,  155,  156,  157,  158,  159,  160,
 /*  3750 */   161,  162,  163,  189,  189,  189,  167,  189,  113,  114,
 /*  3760 */   189,  189,  189,  189,  189,  189,  177,  178,  179,  180,
 /*  3770 */   189,   46,   47,  189,   49,   50,  189,   52,  189,  189,
 /*  3780 */   135,   56,    1,    2,    3,    4,    5,  189,  189,  144,
 /*  3790 */   145,  146,  147,  148,  149,  150,  151,  152,  153,  154,
 /*  3800 */   155,  156,  157,  158,  159,  160,  161,  162,  163,  189,
 /*  3810 */   189,  189,  167,  189,  189,  189,  113,  114,  189,  189,
 /*  3820 */   189,  189,  177,  178,  179,  180,  189,   46,   47,  189,
 /*  3830 */    49,   50,  189,  189,  189,  189,  189,   56,  135,  189,
 /*  3840 */     1,    2,    3,    4,    5,  189,  189,  144,  145,  146,
 /*  3850 */   147,  148,  149,  150,  151,  152,  153,  154,  155,  156,
 /*  3860 */   157,  158,  159,  160,  161,  162,  163,  189,  189,  189,
 /*  3870 */   167,  189,  113,  114,  189,  189,  189,  189,  189,  189,
 /*  3880 */   177,  178,  179,  180,  189,   46,   47,  189,   49,   50,
 /*  3890 */   189,  189,  189,  189,  135,   56,    1,    2,    3,    4,
 /*  3900 */     5,  189,  189,  144,  145,  146,  147,  148,  149,  150,
 /*  3910 */   151,  152,  153,  154,  155,  156,  157,  158,  159,  160,
 /*  3920 */   161,  162,  163,  189,  189,  189,  167,  189,  189,  189,
 /*  3930 */   113,  114,  189,  189,  189,  189,  177,  178,  179,  180,
 /*  3940 */   189,   46,   47,  189,   49,   50,  189,  189,  189,  189,
 /*  3950 */   189,   56,  135,  189,    1,    2,    3,    4,    5,  189,
 /*  3960 */   189,  144,  145,  146,  147,  148,  149,  150,  151,  152,
 /*  3970 */   153,  154,  155,  156,  157,  158,  159,  160,  161,  162,
 /*  3980 */   163,  189,  189,  189,  167,  189,  113,  114,  189,  189,
 /*  3990 */   189,  189,  189,  189,  177,  178,  179,  180,  189,   46,
 /*  4000 */    47,  189,   49,   50,  189,  189,  189,  189,  135,   56,
 /*  4010 */     1,    2,    3,    4,    5,  189,  189,  144,  145,  146,
 /*  4020 */   147,  148,  149,  150,  151,  152,  153,  154,  155,  156,
 /*  4030 */   157,  158,  159,  160,  161,  162,  163,  189,  189,  189,
 /*  4040 */   167,  189,  189,  189,  113,  114,  189,  189,  189,  189,
 /*  4050 */   177,  178,  179,  180,  189,   46,   47,  189,   49,   50,
 /*  4060 */   189,  189,  189,  189,  189,   56,  135,  189,    1,    2,
 /*  4070 */     3,    4,    5,  189,  189,  144,  145,  146,  147,  148,
 /*  4080 */   149,  150,  151,  152,  153,  154,  155,  156,  157,  158,
 /*  4090 */   159,  160,  161,  162,  163,  189,  189,  189,  167,  189,
 /*  4100 */   113,  114,  189,  189,  189,  189,  189,  189,  177,  178,
 /*  4110 */   179,  180,  189,   46,   47,  189,   49,   50,  189,  189,
 /*  4120 */   189,  189,  135,   56,    1,    2,    3,    4,    5,  189,
 /*  4130 */   189,  144,  145,  146,  147,  148,  149,  150,  151,  152,
 /*  4140 */   153,  154,  155,  156,  157,  158,  159,  160,  161,  162,
 /*  4150 */   163,  189,  189,  189,  167,  189,  189,  189,  113,  114,
 /*  4160 */   189,  189,  189,  189,  177,  178,  179,  180,  189,   46,
 /*  4170 */    47,  167,   49,   50,    1,    2,    3,    4,    5,   56,
 /*  4180 */   135,  177,  178,  179,  180,  189,  189,  189,  189,  144,
 /*  4190 */   145,  146,  147,  148,  149,  150,  151,  152,  153,  154,
 /*  4200 */   155,  156,  157,  158,  159,  160,  161,  162,  163,  189,
 /*  4210 */   189,  189,  167,   40,  113,  114,  189,  189,  189,   46,
 /*  4220 */    47,  167,  177,  178,  179,  180,  189,   54,  189,   56,
 /*  4230 */   189,  177,  178,  179,  180,  189,  135,  189,    1,    2,
 /*  4240 */     3,    4,    5,  189,  189,  144,  145,  146,  147,  148,
 /*  4250 */   149,  150,  151,  152,  153,  154,  155,  156,  157,  158,
 /*  4260 */   159,  160,  161,  162,  163,  189,  189,  189,  167,  189,
 /*  4270 */   189,  189,  113,  114,  189,  189,  189,  189,  177,  178,
 /*  4280 */   179,  180,  189,   46,   47,  189,   49,   50,  189,  189,
 /*  4290 */   189,  189,  189,   56,  135,  189,    1,    2,    3,    4,
 /*  4300 */     5,  189,  189,  144,  145,  146,  147,  148,  149,  150,
 /*  4310 */   151,  152,  153,  154,  155,  156,  157,  158,  159,  160,
 /*  4320 */   161,  162,  163,  189,  189,  189,  167,  189,  113,  114,
 /*  4330 */   189,  189,  189,  167,  189,  189,  177,  178,  179,  180,
 /*  4340 */   189,   46,   47,  177,  178,  179,  180,   52,  189,  183,
 /*  4350 */   135,   56,  186,  187,  188,  189,  189,  189,  189,  144,
 /*  4360 */   145,  146,  147,  148,  149,  150,  151,  152,  153,  154,
 /*  4370 */   155,  156,  157,  158,  159,  160,  161,  162,  163,  189,
 /*  4380 */   189,  189,  167,  189,  189,  189,  113,  114,  189,  189,
 /*  4390 */   189,  189,  177,  178,  179,  180,  189,  118,  189,  189,
 /*  4400 */   121,  122,  123,  189,  125,  126,  189,  189,  135,  189,
 /*  4410 */   131,  132,  189,  189,  189,  189,  137,  144,  145,  146,
 /*  4420 */   147,  148,  149,  150,  151,  152,  153,  154,  155,  156,
 /*  4430 */   157,  158,  159,  160,  161,  162,  163,  189,  189,  189,
 /*  4440 */   167,  189,  113,  114,  189,  189,  189,  167,  189,  189,
 /*  4450 */   177,  178,  179,  180,  189,  189,  189,  177,  178,  179,
 /*  4460 */   180,  189,  189,  189,  189,  189,  186,  187,  188,  189,
 /*  4470 */   189,  189,  189,  144,  145,  146,  147,  148,  149,  150,
 /*  4480 */   151,  152,  153,  154,  155,  156,  157,  158,  159,  160,
 /*  4490 */   161,  162,  163,  189,  189,  189,  167,  189,    7,    8,
 /*  4500 */     9,   10,   11,  189,  167,  189,  177,  178,  179,  180,
 /*  4510 */    19,  189,  189,  189,  177,  178,  179,  180,  189,  189,
 /*  4520 */   189,  189,   31,   32,   33,   34,   35,  189,   37,  189,
 /*  4530 */   167,   40,  189,    1,    2,    3,    4,    5,  189,   48,
 /*  4540 */   177,  178,  179,  180,   53,  189,   55,  189,  189,   58,
 /*  4550 */   189,  188,  189,  189,  189,  189,  189,   12,   13,   14,
 /*  4560 */   189,  189,   17,   18,   19,   20,   21,   22,   23,   24,
 /*  4570 */    25,   26,   27,   28,   29,   30,  189,  189,   46,   47,
 /*  4580 */   189,   90,   37,   38,  189,  189,   54,  189,   56,   98,
 /*  4590 */    99,  100,  101,  102,  103,  167,  189,    7,    8,    9,
 /*  4600 */    10,   11,    0,   58,  167,  177,  178,  179,  180,   19,
 /*  4610 */   189,  189,  189,  189,  177,  178,  179,  180,  189,  189,
 /*  4620 */   189,   31,   32,   33,   34,   35,   36,   37,  189,  189,
 /*  4630 */    40,  189,  189,  189,  189,  189,  189,  189,   48,  189,
 /*  4640 */   189,   39,  189,   53,   42,   55,   44,  189,   46,   47,
 /*  4650 */   189,  189,   50,  189,  189,  189,   12,   13,   14,  189,
 /*  4660 */   189,   17,   18,   19,   20,   21,   22,   23,   24,   25,
 /*  4670 */    26,   27,   28,   29,   30,  189,  189,  189,  189,  189,
 /*  4680 */    90,   37,   38,  189,  189,   41,  189,  189,   98,   99,
 /*  4690 */   100,  101,  102,  103,  189,  189,    7,    8,    9,   10,
 /*  4700 */    11,  167,  189,  189,  189,  167,  104,  189,   19,  189,
 /*  4710 */   189,  177,  178,  179,  180,  177,  178,  179,  180,  189,
 /*  4720 */    31,   32,   33,   34,   35,  189,   37,  189,  189,   40,
 /*  4730 */   189,  189,  189,  189,  189,  167,  189,   48,  189,  189,
 /*  4740 */   189,  189,   53,  189,   55,  177,  178,  179,  180,  189,
 /*  4750 */    12,   13,   14,  189,  189,   17,   18,   19,   20,   21,
 /*  4760 */    22,   23,   24,   25,   26,   27,   28,   29,   30,  189,
 /*  4770 */   167,  189,   83,  189,  189,   37,   38,  189,  189,   90,
 /*  4780 */   177,  178,  179,  180,  189,  189,   48,   98,   99,  100,
 /*  4790 */   101,  102,  103,  167,  189,    7,    8,    9,   10,   11,
 /*  4800 */   189,  189,  189,  177,  178,  179,  180,   19,  189,  189,
 /*  4810 */   189,  189,  189,  189,  189,  189,  189,  189,  189,   31,
 /*  4820 */    32,   33,   34,   35,  189,   37,  189,  189,   40,  189,
 /*  4830 */   189,  189,  189,  189,  167,  189,   48,  189,  189,  189,
 /*  4840 */   189,   53,  189,   55,  177,  178,  179,  180,  189,   12,
 /*  4850 */    13,   14,  189,  189,   17,   18,   19,   20,   21,   22,
 /*  4860 */    23,   24,   25,   26,   27,   28,   29,   30,  189,  189,
 /*  4870 */   189,   83,  189,  189,   37,   38,  189,  189,   90,  189,
 /*  4880 */   189,  189,  189,  189,  189,   48,   98,   99,  100,  101,
 /*  4890 */   102,  103,  189,  189,    7,    8,    9,   10,   11,  189,
 /*  4900 */   189,  189,  189,  189,  189,  189,   19,  189,  189,  189,
 /*  4910 */   189,  189,  189,  189,  189,  189,  189,  189,   31,   32,
 /*  4920 */    33,   34,   35,  189,   37,  189,  189,   40,  189,  189,
 /*  4930 */   189,  189,  189,  189,  189,   48,  189,  189,  189,  189,
 /*  4940 */    53,  189,   55,  189,  189,   58,  189,  189,   12,   13,
 /*  4950 */    14,  189,  189,   17,   18,   19,   20,   21,   22,   23,
 /*  4960 */    24,   25,   26,   27,   28,   29,   30,  189,  189,  189,
 /*  4970 */   189,  189,  189,   37,   38,  189,  189,   90,  189,  189,
 /*  4980 */   189,  189,  189,  189,   48,   98,   99,  100,  101,  102,
 /*  4990 */   103,  189,  189,    7,    8,    9,   10,   11,  189,  189,
 /*  5000 */   189,  189,  189,  189,  189,   19,  189,  189,  189,  189,
 /*  5010 */   189,  189,  189,  189,  189,  189,  189,   31,   32,   33,
 /*  5020 */    34,   35,  189,   37,  189,  189,   40,  189,    1,    2,
 /*  5030 */     3,    4,    5,  189,   48,  189,  189,  189,  189,   53,
 /*  5040 */   189,   55,  189,  189,   58,  189,  189,  189,  189,  189,
 /*  5050 */   189,  189,   12,   13,   14,  189,  189,   17,   18,   19,
 /*  5060 */    20,   21,   22,   23,   24,   25,   26,   27,   28,   29,
 /*  5070 */    30,  189,  189,   46,   47,  189,   90,   37,   38,  189,
 /*  5080 */   189,   54,  189,   56,   98,   99,  100,  101,  102,  103,
 /*  5090 */   189,  189,    7,    8,    9,   10,   11,  189,   58,  189,
 /*  5100 */   189,  189,  189,  189,   19,  189,  189,  189,  189,  189,
 /*  5110 */   189,  189,  189,  189,  189,  189,   31,   32,   33,   34,
 /*  5120 */    35,   36,   37,  189,  189,   40,  189,  189,  189,  189,
 /*  5130 */   189,  189,  189,   48,  189,  189,  189,  189,   53,  189,
 /*  5140 */    55,  189,  189,  189,  189,  189,  189,  189,  189,  189,
 /*  5150 */   189,   12,   13,   14,  189,  189,   17,   18,   19,   20,
 /*  5160 */    21,   22,   23,   24,   25,   26,   27,   28,   29,   30,
 /*  5170 */   189,  189,  189,  189,  189,   90,   37,   38,  189,  189,
 /*  5180 */    41,  189,  189,   98,   99,  100,  101,  102,  103,  189,
 /*  5190 */   189,    7,    8,    9,   10,   11,  189,  189,  189,  189,
 /*  5200 */   189,  189,  189,   19,  189,  189,  189,  189,  189,  189,
 /*  5210 */   189,  189,  189,  189,  189,   31,   32,   33,   34,   35,
 /*  5220 */    36,   37,  189,  189,   40,  189,  189,  189,  189,  189,
 /*  5230 */   189,  189,   48,  189,  189,  189,  189,   53,  189,   55,
 /*  5240 */   189,  189,  189,  189,  189,   12,   13,   14,  189,  189,
 /*  5250 */    17,   18,   19,   20,   21,   22,   23,   24,   25,   26,
 /*  5260 */    27,   28,   29,   30,  189,  189,  189,  189,  189,  189,
 /*  5270 */    37,   38,  189,  189,   90,  189,  189,  189,  189,  189,
 /*  5280 */   189,   48,   98,   99,  100,  101,  102,  103,  189,  189,
 /*  5290 */     7,    8,    9,   10,   11,  189,  189,  189,  189,  189,
 /*  5300 */   189,  189,   19,  189,  189,  189,  189,  189,  189,  189,
 /*  5310 */   189,  189,  189,  189,   31,   32,   33,   34,   35,   36,
 /*  5320 */    37,  189,  189,   40,  189,  189,  189,  189,  189,  189,
 /*  5330 */   189,   48,  189,  189,  189,  189,   53,  189,   55,  189,
 /*  5340 */   189,  189,  189,  189,  189,  189,  189,  189,  189,   12,
 /*  5350 */    13,   14,  189,  189,   17,   18,   19,   20,   21,   22,
 /*  5360 */    23,   24,   25,   26,   27,   28,   29,   30,  189,  189,
 /*  5370 */   189,  189,  189,   90,   37,   38,  189,  189,   41,  189,
 /*  5380 */   189,   98,   99,  100,  101,  102,  103,  189,  189,    7,
 /*  5390 */     8,    9,   10,   11,  189,  189,  189,  189,  189,  189,
 /*  5400 */   189,   19,  189,  189,  189,  189,  189,  189,  189,  189,
 /*  5410 */   189,  189,  189,   31,   32,   33,   34,   35,  189,   37,
 /*  5420 */   189,  189,   40,   41,  189,  189,  189,  189,  189,  189,
 /*  5430 */    48,  189,  189,  189,  189,   53,  189,   55,  189,  189,
 /*  5440 */   189,  189,  189,   12,   13,   14,  189,  189,   17,   18,
 /*  5450 */    19,   20,   21,   22,   23,   24,   25,   26,   27,   28,
 /*  5460 */    29,   30,  189,  189,  189,  189,  189,   36,   37,   38,
 /*  5470 */   189,  189,   90,  189,  189,  189,  189,  189,  189,  189,
 /*  5480 */    98,   99,  100,  101,  102,  103,  189,  189,    7,    8,
 /*  5490 */     9,   10,   11,  189,  189,  189,  189,  189,  189,  189,
 /*  5500 */    19,  189,  189,  189,  189,  189,  189,  189,  189,  189,
 /*  5510 */   189,  189,   31,   32,   33,   34,   35,   36,   37,  189,
 /*  5520 */   189,   40,  189,  189,  189,  189,  189,  189,  189,   48,
 /*  5530 */   189,  189,  189,  189,   53,  189,   55,  189,  189,  189,
 /*  5540 */   189,  189,   12,   13,   14,  189,  189,   17,   18,   19,
 /*  5550 */    20,   21,   22,   23,   24,   25,   26,   27,   28,   29,
 /*  5560 */    30,  189,  189,  189,  189,  189,  189,   37,   38,  189,
 /*  5570 */   189,   90,  189,  189,  189,  189,  189,  189,  189,   98,
 /*  5580 */    99,  100,  101,  102,  103,  189,  189,    7,    8,    9,
 /*  5590 */    10,   11,  189,  189,  189,  189,  189,  189,  189,   19,
 /*  5600 */   189,  189,  189,  189,  189,  189,  189,  189,  189,  189,
 /*  5610 */   189,   31,   32,   33,   34,   35,   36,   37,  189,  189,
 /*  5620 */    40,  189,  189,  189,  189,  189,  189,  189,   48,  189,
 /*  5630 */   189,  189,  189,   53,  189,   55,  189,  189,  189,  189,
 /*  5640 */   189,  189,   13,   14,  189,  189,   17,   18,   19,   20,
 /*  5650 */    21,   22,   23,   24,   25,   26,   27,   28,   29,   30,
 /*  5660 */   189,  189,  189,  189,  189,  189,   37,   38,  189,  189,
 /*  5670 */    90,  189,  189,  189,  189,  189,  189,  189,   98,   99,
 /*  5680 */   100,  101,  102,  103,  189,  189,    7,    8,    9,   10,
 /*  5690 */    11,  189,  189,  189,  189,  189,  189,  189,   19,  189,
 /*  5700 */   189,  189,  189,  189,  189,  189,  189,  189,  189,  189,
 /*  5710 */    31,   32,   33,   34,   35,   36,   37,  189,  189,   40,
 /*  5720 */   189,  189,  189,  189,  189,  189,  189,   48,  189,  189,
 /*  5730 */   189,  189,   53,  189,   55,  189,  189,   17,   18,   19,
 /*  5740 */    20,   21,   22,   23,   24,   25,   26,   27,   28,   29,
 /*  5750 */    30,  189,   37,   38,  189,  189,  189,   37,   38,  189,
 /*  5760 */   189,  189,  189,  189,  189,  189,   51,  189,  189,   90,
 /*  5770 */   189,  189,  189,  189,  189,  189,  189,   98,   99,  100,
 /*  5780 */   101,  102,  103,  189,  189,    7,    8,    9,   10,   11,
 /*  5790 */   189,  189,  189,  189,  189,  189,  189,   19,  189,   19,
 /*  5800 */    85,   86,   87,   88,   89,  189,   91,   92,   93,   31,
 /*  5810 */    32,   33,   34,   35,  189,   37,   36,   19,   40,  189,
 /*  5820 */    40,  189,  189,  189,  189,  189,   48,  189,  189,  189,
 /*  5830 */   189,   53,  189,   55,   36,  189,   58,  189,   40,   59,
 /*  5840 */    60,   61,   62,   63,   64,   65,   66,   67,   68,   69,
 /*  5850 */   189,  189,  189,  189,  189,  189,  189,   59,   60,   61,
 /*  5860 */    62,   63,   64,   65,   66,   67,   68,   69,   90,  189,
 /*  5870 */   189,  189,  189,  189,  189,  189,   98,   99,  100,  101,
 /*  5880 */   102,  103,  189,  189,    7,    8,    9,   10,   11,  189,
 /*  5890 */   189,  189,  189,  189,  189,  189,   19,  189,   19,  189,
 /*  5900 */   189,  189,  189,  189,  189,  189,  189,  189,   31,   32,
 /*  5910 */    33,   34,   35,   36,   37,   36,   19,   40,  189,   40,
 /*  5920 */   189,  189,  189,  189,  189,   48,  189,  189,  189,  189,
 /*  5930 */    53,  189,   55,   36,  189,  189,  189,   40,   59,   60,
 /*  5940 */    61,   62,   63,   64,   65,   66,   67,   68,   69,  189,
 /*  5950 */   189,  189,  189,  189,  189,  189,   59,   60,   61,   62,
 /*  5960 */    63,   64,   65,   66,   67,   68,   69,   90,  189,  189,
 /*  5970 */   189,  189,  189,  189,  189,   98,   99,  100,  101,  102,
 /*  5980 */   103,  189,  189,    7,    8,    9,   10,   11,  189,  189,
 /*  5990 */   189,  189,  189,  189,  189,   19,   19,  189,  189,  189,
 /*  6000 */    37,  189,  189,  189,  189,  189,  189,   31,   32,   33,
 /*  6010 */    34,   35,   36,   37,   51,  189,   40,  189,    1,    2,
 /*  6020 */     3,    4,    5,  189,   48,  189,  189,  189,  189,   53,
 /*  6030 */   189,   55,  189,  189,   57,  189,   59,   60,   61,   62,
 /*  6040 */    63,   64,   65,   66,   67,   68,   69,  189,   85,   86,
 /*  6050 */    87,   88,   89,  189,  189,   92,   93,  189,  189,  189,
 /*  6060 */   189,  189,  189,   46,   47,  189,   90,  189,  189,  189,
 /*  6070 */   189,   54,  189,   56,   98,   99,  100,  101,  102,  103,
 /*  6080 */   189,  189,    7,    8,    9,   10,   11,  189,  189,  189,
 /*  6090 */   189,  189,  189,  189,   19,   19,  189,  189,  189,  189,
 /*  6100 */   189,  189,  189,  189,  189,  189,   31,   32,   33,   34,
 /*  6110 */    35,   36,   37,  189,  189,   40,   40,  189,  189,  189,
 /*  6120 */   189,  189,  189,   48,  189,  189,  189,  189,   53,  189,
 /*  6130 */    55,  189,  189,  189,  189,   59,   60,   61,   62,   63,
 /*  6140 */    64,   65,   66,   67,   68,   69,  189,  189,  189,  189,
 /*  6150 */   189,  189,  189,  189,  189,  189,  189,  189,  189,  189,
 /*  6160 */   189,  189,  189,  189,  189,   90,  189,  189,  189,  189,
 /*  6170 */   189,  189,  189,   98,   99,  100,  101,  102,  103,  189,
 /*  6180 */   189,    7,    8,    9,   10,   11,  189,  189,  189,  189,
 /*  6190 */   189,  189,  189,   19,  189,  189,  189,  189,  189,  189,
 /*  6200 */   189,  189,  189,  189,  189,   31,   32,   33,   34,   35,
 /*  6210 */   189,   37,  189,  189,   40,  189,  189,  189,  189,  189,
 /*  6220 */   189,  189,   48,  189,  189,  189,  189,   53,  189,   55,
 /*  6230 */   189,  189,  189,  189,  189,  189,  189,  189,  189,  189,
 /*  6240 */   189,  189,  189,  189,  189,  189,  189,  189,  189,  189,
 /*  6250 */   189,  189,  189,  189,  189,  189,  189,  189,  189,  189,
 /*  6260 */   189,  189,  189,  189,   90,  189,  189,  189,  189,  189,
 /*  6270 */   189,  189,   98,   99,  100,  101,  102,  103,  189,  189,
 /*  6280 */     7,    8,    9,   10,   11,  189,  189,  189,  189,  189,
 /*  6290 */   189,  189,   19,  189,  189,  189,  189,  189,  189,  189,
 /*  6300 */   189,  189,  189,  189,   31,   32,   33,   34,   35,  189,
 /*  6310 */    37,  189,  189,   40,  189,  189,  189,  189,  189,  189,
 /*  6320 */   189,   48,  189,  189,  189,  189,   53,  189,   55,  189,
 /*  6330 */   189,  189,  189,  189,  189,  189,  189,  189,  189,  189,
 /*  6340 */   189,  189,  189,  189,  189,  189,  189,  189,  189,  189,
 /*  6350 */   189,  189,  189,  189,  189,  189,  189,  189,  189,  189,
 /*  6360 */   189,  189,  189,   90,  189,  189,  189,  189,  189,  189,
 /*  6370 */   189,   98,   99,  100,  101,  102,  103,
};
#define YY_SHIFT_USE_DFLT (-55)
static short yy_shift_ofst[] = {
 /*     0 */   363, 1025, 4602,  -55,  -55,  -55,  -55,  -55,  -55,  951,
 /*    10 */  1060,  -55,  891,  626,  -55, 1082,  780,  -55, 3553,  -55,
 /*    20 */   945,  -55, 3497, 4237,  -55, 6017,  791,  550, 5879,  632,
 /*    30 */  5977,  825,  -55,  -55,  975, 3425,  -55,  981,  -55,  -55,
 /*    40 */   -55,  286,  779,  -55,  771,  531,  -55,  -55,  -55,  -55,
 /*    50 */   -55,  -55,  -55,  -55,  -55,  -55,  -55,  -55,  -55,  -55,
 /*    60 */   729,  420, 5977,  877,  -55,  -55, 6076,  -55,  703, 3210,
 /*    70 */   -55,  -55,  -55,  -55,  -55,  -55,  -55,  -55,  909,  946,
 /*    80 */   -55,  -55,  231,  997, 3210,  -55,  273,  952, 3210,  -55,
 /*    90 */  1049,  964, 3210,  -55,  862,  870,  -55,  -55,  -55,  -55,
 /*   100 */   -55,  -55,  -55,  -55,  -55,  -55, 3375, 5027,  863,  864,
 /*   110 */  5780,  374, 5977,  917,  -55,  -55,  319,  723, 5977,  970,
 /*   120 */   -55,  -55,  -55,  -55,  -55, 4295, 1038, 1048, 3210, 1122,
 /*   130 */   -55,  815,   24, 3210, 1098,  -55, 3895,  -55,  984,  159,
 /*   140 */   -55,  953,  474,  -55,  217,  358,  -55, 3611,  -55,  785,
 /*   150 */   -55, 3725, 4123,  -55, 4532,  457,  448, 5798,  587, 1071,
 /*   160 */   -55,  973,  -55,  -55,  -55,  -55,  -55,  -55,  -55,  -55,
 /*   170 */   -55,  -55,  -55,  -55,  -55,  -55,  -55,  -55,  -55,  -55,
 /*   180 */   -55,  -55,  -55,  -55,  -55,  -55, 1047,  -55, 1056,  -55,
 /*   190 */  6174, 1075,  581, 2933, 1083, 1106, 2247, 2443,  -55, 6174,
 /*   200 */   885,  -26,  -55,  170,  -55,  -55, 3210, 1072, 3129, 3129,
 /*   210 */  1081, 1365, 1561,  -55, 6174, 1059,  287,  483,  -55, 1037,
 /*   220 */   189, 1855,  950, 6174,  979,  -55, 6174,   32, 6174, 6174,
 /*   230 */    32, 6174, 6174, 5530, 6174, 6174, 5530, 6174, 6174, 5530,
 /*   240 */  6174, 6174, 5530, 6174, 6174, 5530, 6174,   78, 6174,   78,
 /*   250 */    73,  856, 5580, 5530, 6174, 4545,  -55, 6174,  105, 6174,
 /*   260 */   105, 6174,   32, 6174,   32, 6174,   32, 6174,  105, 6174,
 /*   270 */  5720, 6174, 5720, 6174, 5629, 3227, 5431,  -55,  656, 6174,
 /*   280 */    32,  402,  383, 6174,   78,  413,  669,  577, 5481,  659,
 /*   290 */   711, 6174,   32,  -55, 6174,   32,  -55,  -55,  -55,  -55,
 /*   300 */   -55,  -55,  -55,  -55, 4887, 5530,  -15, 5679,   90,  858,
 /*   310 */  6075,  -55,  372,  -55, 6174,  903,  908,  572, 5085,  695,
 /*   320 */   -55,  -55,  -55,  -55,  -55,  -55,  624,  963, 5283,  405,
 /*   330 */   -55,  -55, 1010, 1007, 1011, 5877,  799,  -55,  -55,  -55,
 /*   340 */  1058, 1061, 1062, 4590,  -55,  529,  -55,  620,  -55,  -55,
 /*   350 */  1114,  -54,  -55,  712,  -55, 6273,  -55,  925, 6174,  -55,
 /*   360 */   -55,  -55,  694,  -55,  -55,  -55,  597,  -55,  -55,  436,
 /*   370 */  1079, 1070, 5184,  417,  -55,  -55, 1067, 1063, 5976,  272,
 /*   380 */   -55,  -55,   78,   78,   78,   78,   78,   78, 5530, 1024,
 /*   390 */     0, 4689, 4837, 3129, 1659,  -55,  977,  519,  -55,  977,
 /*   400 */   -55, 5715,  -55,  -55,  -55,  -55,  -55,  -55, 6174,  -55,
 /*   410 */  5530,  707, 5963, 6174,  -55, 4491,  312, 6174,  -55,  928,
 /*   420 */   -55, 5040,  715, 6174,  -55,  -55, 6174,  -55,  -55,  -55,
 /*   430 */   119,  883,  312, 6174,  -55,  873,  312, 6174,  -55,  880,
 /*   440 */   617, 6174,  -55, 4986,  312, 6174,  -55,  715, 6174,  -55,
 /*   450 */  5778,  312, 6174,  -55,  715, 6174,  -55,  -55,  -55,  -55,
 /*   460 */  6174,  421,  -55, 6174,  -55, 5530,  -55,  851,  -55,  850,
 /*   470 */   -55,  848,  -55,  819,  -55, 5382, 4644,  -55,  -55, 6174,
 /*   480 */  5139,  -55, 6174, 5337,  -55,  396,  756,  -55,  396,  -55,
 /*   490 */   716, 3210,  -55,  -55,  396,  731,  -55,  396,  274,  -55,
 /*   500 */   396,  553,  -55,  396,  108,  -55,  396,  730,  -55,  396,
 /*   510 */   123,  -55,  396,  792,  -55,  396,  727,  -55,  396,  335,
 /*   520 */   -55, 5530,  -55,  -55,  -55, 6174, 4936, 3129, 1267,  -55,
 /*   530 */   781,  742, 4788, 4738, 3129, 2051,  -55, 6174, 5233, 3129,
 /*   540 */  2541,  -55,  774, 6174,  809,  -55,  -55,  -55,  812, 3129,
 /*   550 */  3129,  -55,  -55,  820,  853, 1953,   91,  -55,  -55, 5977,
 /*   560 */   857,  777,  -55,  679,  -55,  784,  775, 1757,  -55, 3031,
 /*   570 */   -55, 5977,  879, 2737,  -55,  875,  -55, 3375, 3251,  911,
 /*   580 */   897, 5897,  518, 2345,  -55, 1463,  -55, 5977,  940, 2149,
 /*   590 */   -55,  385,  -55,  505,  601, 1169,  -55,   -7,  -55, 5977,
 /*   600 */   954, 2639,  -55, 2835,  -55, 3383, 3953,  -55, 4295,  -55,
 /*   610 */  4295, 3318,  177,  -55, 3210,  266,  -55,  990,  -55,   87,
 /*   620 */  1014,   60, 1017,   -3,  -55,  -55, 1046,  -55,  -55, 1051,
 /*   630 */   -55, 4173,  373,  -55, 3210,   71,  -55, 1073,  -55, 1074,
 /*   640 */   -55, 3667, 4009, 3839, 3375, 4067,  -55, 3781,  217,  -55,
 /*   650 */   -55,  -55,  217,  358,  -55, 1120, 1151,  453,  -55,  203,
 /*   660 */    36,  -55,  217,  358,  -55,  301,  320,  486,  -55,  886,
 /*   670 */    36,  -55,  -55,
};
#define YY_REDUCE_USE_DFLT (-142)
static short yy_reduce_ofst[] = {
 /*     0 */   -93, -142,  191, -142, -142, -142, -142, -142, -142, -142,
 /*    10 */  -142, -142, -142,  904, -142, -142,  739, -142,  473, -142,
 /*    20 */  -142, -142,  306,  197, -142,  337, -142, -142,  468, -142,
 /*    30 */   555, -142, -142, -142, -142,   74, -142, -142, -142, -142,
 /*    40 */  -142, -142, -142, -142, -142, -142, -142, -142, -142, -142,
 /*    50 */  -142, -142, -142, -142, -142, -142, -142, -142, -142, -142,
 /*    60 */  -142, -142, -131, -142, -142, -142,  288, -142, -142,  718,
 /*    70 */  -142, -142, -142, -142, -142, -142, -142, -142, -142, -142,
 /*    80 */  -142, -142, -142, -142,  957, -142, -142, -142, 1000, -142,
 /*    90 */  -142, -142,  837, -142, -142, -142, -142, -142, -142, -142,
 /*   100 */  -142, -142, -142, -142, -142, -142,  589,  337, -142, -142,
 /*   110 */   -23, -142, 2123, -142, -142, -142, -142, -142, 2229, -142,
 /*   120 */  -142, -142, -142, -142, -142,  589, -142, -142, 1035, -142,
 /*   130 */  -142, -142, -142, 1084, -142, -142,  197, -142, -142,  893,
 /*   140 */  -142, -142,  832, -142,   63,  788, -142, 4279, -142, -142,
 /*   150 */  -142,  -87,  207, -142,  337, -142, -142,   93, -142, 3189,
 /*   160 */  -142, 4329, -142, -142, -142, -142, -142, -142, -142, -142,
 /*   170 */  -142, -142, -142, -142, -142, -142, -142, -142, -142, -142,
 /*   180 */  -142, -142, -142, -142, -142, -142, -142, -142, -142, -142,
 /*   190 */  1372, -142, 3987, 4329, -142, -142, 3247, 4329, -142, 1470,
 /*   200 */  -142,  204, -142,   65, -142, -142, 1026, -142, 3817, 4329,
 /*   210 */  -142, 3531, 4329, -142,  882, -142, 3645, 4329, -142, -142,
 /*   220 */  3759, 4329, -142,  980, -142, -142, 2386, -142, 4437, 4428,
 /*   230 */  -142, 4004, 2876, -142, 1567, 2645, -142, 2778, 2841, -142,
 /*   240 */  3037, 3099, -142, 4538, 4626, -142, 2155, -142, 2351, -142,
 /*   250 */  -142, -142,  293, -142, 4054, -142, -142, 2939, -142, 2743,
 /*   260 */  -142, 2449, -142, 2974, -142, 4534, -142, 1861, -142, 2288,
 /*   270 */  -142, 1959, -142, 2547, -142, 1994, -142, -142, -142, 2680,
 /*   280 */  -142, -142, -142, 1665, -142, -142, -142, -142,   -1, -142,
 /*   290 */  -142, 2582, -142, -142, 4568, -142, -142, -142, -142, -142,
 /*   300 */  -142, -142, -142, -142, 4166, -142, -142,  132, -142, -142,
 /*   310 */   685, -142, -142, -142, 1273, -142, -142, -142,  -99, -142,
 /*   320 */  -142, -142, -142, -142, -142, -142, -142, -142,  391, -142,
 /*   330 */  -142, -142, -142, -142, -142,  195, -142, -142, -142, -142,
 /*   340 */  -142, -142, -142,   97, -142, -142, -142, -142, -142, -142,
 /*   350 */  -142, -142, -142, -142, -142, 4280, -142, -142, 4363, -142,
 /*   360 */  -142, -142, -142, -142, -142, -142, -142, -142, -142, -142,
 /*   370 */  -142, -142,  783, -142, -142, -142, -142, -142,  328, -142,
 /*   380 */  -142, -142, -142, -142, -142, -142, -142, -142, -142, -142,
 /*   390 */  -142, 1763, -142, 3873, 4329, -142,  341, -142, -142,  843,
 /*   400 */  -142, -141, -142, -142, -142, -142, -142, -142,  818, -142,
 /*   410 */  -142, -142,  367,  916, -142, 1015,  796, 1112, -142, -142,
 /*   420 */  -142, -142,  428, 1210, -142, -142, 1015, -142, -142, -142,
 /*   430 */  -142, -142,  759, 1308, -142, -142,  751, 1406, -142, -142,
 /*   440 */   -25, 1504, -142, 1015,  747, 1602, -142,  649, 1700, -142,
 /*   450 */  1015,  744, 1798, -142,   92, 1896, -142, -142, -142, -142,
 /*   460 */   720, -142, -142, 1078, -142, -142, -142, -142, -142, -142,
 /*   470 */  -142, -142, -142, -142, -142, 4337, -142, -142, -142, 4603,
 /*   480 */  -142, -142, 2253, -142, -142,  625, -142, -142,  633, -142,
 /*   490 */  -142,  629, -142, -142,  543, -142, -142,  526, -142, -142,
 /*   500 */   476, -142, -142,  -14, -142, -142,  485, -142, -142,  456,
 /*   510 */  -142, -142,  442, -142, -142,  438, -142, -142,  318, -142,
 /*   520 */  -142, -142, -142, -142, -142, 4667, -142, 4045, 4329, -142,
 /*   530 */  -142, -142, 2057, -142, 4215, 4329, -142, 2484, -142, 3417,
 /*   540 */  4329, -142, -142, 1176, -142, -142, -142, -142, -142, 3475,
 /*   550 */  4329, -142, -142, -142, -142, 3589, 4329, -142, -142,  653,
 /*   560 */  -142, 3703, -142, 4329, -142, -142, -142, 3931, -142, 4329,
 /*   570 */  -142, 1045, -142, 4159, -142, 4329, -142,  491,  337, -142,
 /*   580 */  -142,  387, -142, 3303, -142, 4329, -142, 2131, -142, 3361,
 /*   590 */  -142, 4329, -142, -142, -142, 4273, -142, 4329, -142, 2221,
 /*   600 */  -142, 4101, -142, 4329, -142,  109,  207, -142,  491, -142,
 /*   610 */   470,  337,  899, -142,  914,  902, -142, -142, -142,   44,
 /*   620 */  -142, -142, -142,  935, -142, -142, -142, -142, -142, -142,
 /*   630 */  -142,  337,  972, -142,  982,  985, -142, -142, -142, -142,
 /*   640 */  -142,   80,  206,  207,  470,  207, -142,  207, 1027, -142,
 /*   650 */  -142, -142,  455, 1080, -142, -142, -142, 1089, -142, -142,
 /*   660 */    94, -142,  604,  156, -142, -142, -142,  743, -142, -142,
 /*   670 */   871, -142, -142,
};
static YYACTIONTYPE yy_default[] = {
 /*     0 */  1001, 1001, 1001,  675,  677,  678,  679,  680,  681, 1001,
 /*    10 */  1001,  682, 1001, 1001,  683, 1001, 1001,  684, 1001,  699,
 /*    20 */  1001,  700, 1001, 1001,  733, 1001, 1001, 1001, 1001, 1001,
 /*    30 */  1001, 1001,  753,  771,  772, 1001,  773,  775,  776,  777,
 /*    40 */   778, 1001, 1001,  789, 1001, 1001,  790,  791,  792,  793,
 /*    50 */   794,  795,  796,  797,  798,  799,  800,  801,  774,  757,
 /*    60 */  1001, 1001, 1001, 1001,  754,  758, 1001,  779,  781, 1001,
 /*    70 */   785,  989,  990,  991,  992,  993,  994,  995, 1001, 1001,
 /*    80 */   996,  997, 1001,  782, 1001,  786, 1001,  783, 1001,  787,
 /*    90 */  1001,  784, 1001,  788, 1001, 1001,  780,  761,  763,  764,
 /*   100 */   765,  766,  767,  768,  769,  770, 1001, 1001, 1001, 1001,
 /*   110 */  1001, 1001, 1001, 1001,  755,  759, 1001, 1001, 1001, 1001,
 /*   120 */   756,  760,  762,  729,  734, 1001, 1001, 1001, 1001, 1001,
 /*   130 */   735, 1001, 1001, 1001, 1001,  736, 1001,  730, 1001, 1001,
 /*   140 */   685, 1001, 1001,  686, 1001, 1001,  688, 1001,  694, 1001,
 /*   150 */   695, 1001, 1001,  731, 1001, 1001, 1001, 1001, 1001, 1001,
 /*   160 */   737, 1001,  739,  802,  804,  805,  806,  807,  808,  809,
 /*   170 */   810,  811,  812,  813,  814,  815,  816,  817,  818,  819,
 /*   180 */   820,  821,  822,  823,  824,  825, 1001,  826, 1001,  827,
 /*   190 */  1001, 1001, 1001, 1001,  830, 1001, 1001, 1001,  831, 1001,
 /*   200 */  1001, 1001,  834, 1001,  835,  836, 1001, 1001,  838,  839,
 /*   210 */  1001, 1001, 1001,  842, 1001, 1001, 1001, 1001,  844, 1001,
 /*   220 */  1001, 1001, 1001, 1001, 1001,  846, 1001,  906, 1001, 1001,
 /*   230 */   907, 1001, 1001,  908, 1001, 1001,  909, 1001, 1001,  910,
 /*   240 */  1001, 1001,  911, 1001, 1001,  912, 1001,  919, 1001,  920,
 /*   250 */  1001,  924, 1001,  980, 1001, 1001,  929, 1001,  930, 1001,
 /*   260 */   931, 1001,  932, 1001,  933, 1001,  934, 1001,  935, 1001,
 /*   270 */   936, 1001,  937, 1001,  938, 1001, 1001,  921, 1001, 1001,
 /*   280 */   922, 1001, 1001, 1001,  923,  942, 1001,  927, 1001, 1001,
 /*   290 */  1001, 1001,  939,  940, 1001,  941,  943,  944,  945,  946,
 /*   300 */   947,  948,  949,  950, 1001,  988,  942, 1001, 1001,  953,
 /*   310 */  1001,  954, 1001,  955, 1001, 1001, 1001, 1001, 1001, 1001,
 /*   320 */   961,  962,  975,  976,  977,  979, 1001, 1001, 1001, 1001,
 /*   330 */   965,  966, 1001, 1001, 1001, 1001, 1001,  967,  968,  978,
 /*   340 */  1001, 1001,  956, 1001,  957, 1001,  958, 1001,  959,  960,
 /*   350 */   943,  944,  951, 1001,  952, 1001,  981, 1001, 1001,  983,
 /*   360 */   984,  982, 1001,  963,  964,  928, 1001,  969,  970, 1001,
 /*   370 */  1001,  925, 1001, 1001,  971,  972, 1001,  926, 1001, 1001,
 /*   380 */   973,  974,  918,  917,  916,  915,  914,  913,  998, 1001,
 /*   390 */  1001, 1001, 1001, 1001, 1001,  847, 1001, 1001,  851, 1001,
 /*   400 */   852, 1001,  854,  855,  856,  857,  858,  859, 1001,  860,
 /*   410 */   905, 1001, 1001, 1001,  861, 1001, 1001, 1001,  864, 1001,
 /*   420 */   873, 1001, 1001, 1001,  865,  871, 1001,  872,  874,  875,
 /*   430 */  1001, 1001, 1001, 1001,  862, 1001, 1001, 1001,  863, 1001,
 /*   440 */  1001, 1001,  866, 1001, 1001, 1001,  867, 1001, 1001,  868,
 /*   450 */  1001, 1001, 1001,  869, 1001, 1001,  870,  876,  877,  853,
 /*   460 */  1001, 1001,  879, 1001,  880,  882,  881,  975,  883,  977,
 /*   470 */   884,  976,  885,  940,  886, 1001, 1001,  887,  888, 1001,
 /*   480 */  1001,  889, 1001, 1001,  890, 1001, 1001,  891, 1001,  901,
 /*   490 */   903, 1001,  904,  902, 1001, 1001,  892, 1001, 1001,  893,
 /*   500 */  1001, 1001,  894, 1001, 1001,  895, 1001, 1001,  896, 1001,
 /*   510 */  1001,  897, 1001, 1001,  898, 1001, 1001,  899, 1001, 1001,
 /*   520 */   900, 1001,  999, 1000,  803, 1001, 1001, 1001, 1001,  848,
 /*   530 */  1001, 1001, 1001, 1001, 1001, 1001,  849, 1001, 1001, 1001,
 /*   540 */  1001,  850, 1001, 1001, 1001,  845,  843,  841, 1001, 1001,
 /*   550 */   840,  837,  832,  828, 1001, 1001, 1001,  833,  829, 1001,
 /*   560 */  1001, 1001,  745, 1001,  747, 1001, 1001, 1001,  738, 1001,
 /*   570 */   740, 1001, 1001, 1001,  746, 1001,  748, 1001, 1001, 1001,
 /*   580 */  1001, 1001, 1001, 1001,  741, 1001,  743, 1001, 1001, 1001,
 /*   590 */   749, 1001,  751, 1001, 1001, 1001,  742, 1001,  744, 1001,
 /*   600 */  1001, 1001,  750, 1001,  752, 1001, 1001,  732, 1001,  713,
 /*   610 */  1001, 1001, 1001,  715, 1001, 1001,  717, 1001,  721, 1001,
 /*   620 */  1001, 1001, 1001, 1001,  725,  727, 1001,  728,  726, 1001,
 /*   630 */   719, 1001, 1001,  716, 1001, 1001,  718, 1001,  722, 1001,
 /*   640 */   720, 1001, 1001, 1001, 1001, 1001,  714, 1001, 1001,  696,
 /*   650 */   698,  697, 1001, 1001,  687, 1001, 1001, 1001,  689, 1001,
 /*   660 */  1001,  690, 1001, 1001,  691, 1001, 1001, 1001,  692, 1001,
 /*   670 */  1001,  693,  676,
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
  "PRIVATE",       "SCOPED",        "COMMA",         "REQUIRE",     
  "LIKELY",        "UNLIKELY",      "CLONE",         "EMPTY",       
  "INSTANCEOF",    "AND",           "OR",            "BITWISE_OR",  
  "BITWISE_AND",   "EQUALS",        "IDENTICAL",     "LESS",        
  "GREATER",       "LESSEQUAL",     "GREATEREQUAL",  "NOTIDENTICAL",
  "NOTEQUALS",     "ADD",           "SUB",           "CONCAT",      
  "MUL",           "DIV",           "MOD",           "ISSET",       
  "FETCH",         "TYPEOF",        "NEW",           "NOT",         
  "PARENTHESES_CLOSE",  "SBRACKET_OPEN",  "ARROW",         "NAMESPACE",   
  "IDENTIFIER",    "DOTCOMMA",      "INTERFACE",     "EXTENDS",     
  "CLASS",         "IMPLEMENTS",    "ABSTRACT",      "FINAL",       
  "BRACKET_OPEN",  "BRACKET_CLOSE",  "COMMENT",       "ASSIGN",      
  "CONST",         "CONSTANT",      "FUNCTION",      "PARENTHESES_OPEN",
  "INLINE",        "VOID",          "SBRACKET_CLOSE",  "TYPE_INTEGER",
  "TYPE_UINTEGER",  "TYPE_LONG",     "TYPE_ULONG",    "TYPE_CHAR",   
  "TYPE_UCHAR",    "TYPE_DOUBLE",   "TYPE_BOOL",     "TYPE_STRING", 
  "TYPE_ARRAY",    "TYPE_VAR",      "BREAK",         "CONTINUE",    
  "IF",            "ELSE",          "SWITCH",        "CASE",        
  "COLON",         "DEFAULT",       "LOOP",          "WHILE",       
  "DO",            "FOR",           "IN",            "REVERSE",     
  "LET",           "ADDASSIGN",     "SUBASSIGN",     "MULASSIGN",   
  "DIVASSIGN",     "CONCATASSIGN",  "STRING",        "DOUBLECOLON", 
  "INCR",          "DECR",          "ECHO",          "RETURN",      
  "UNSET",         "THROW",         "INTEGER",       "CHAR",        
  "DOUBLE",        "NULL",          "TRUE",          "FALSE",       
  "CBLOCK",        "error",         "program",       "xx_language", 
  "xx_top_statement_list",  "xx_top_statement",  "xx_namespace_def",  "xx_class_def",
  "xx_interface_def",  "xx_comment",    "xx_cblock",     "xx_interface_body",
  "xx_class_body",  "xx_implements_list",  "xx_class_definition",  "xx_implements",
  "xx_interface_definition",  "xx_class_properties_definition",  "xx_class_consts_definition",  "xx_class_methods_definition",
  "xx_interface_methods_definition",  "xx_class_property_definition",  "xx_visibility_list",  "xx_literal_expr",
  "xx_class_property_shortcuts",  "xx_class_property_shortcuts_list",  "xx_class_property_shortcut",  "xx_class_const_definition",
  "xx_class_method_definition",  "xx_interface_method_definition",  "xx_parameter_list",  "xx_statement_list",
  "xx_method_return_type",  "xx_visibility",  "xx_method_return_type_list",  "xx_method_return_type_item",
  "xx_parameter_type",  "xx_parameter_cast",  "xx_parameter_cast_collection",  "xx_parameter",
  "xx_statement",  "xx_let_statement",  "xx_if_statement",  "xx_loop_statement",
  "xx_echo_statement",  "xx_return_statement",  "xx_fetch_statement",  "xx_fcall_statement",
  "xx_mcall_statement",  "xx_scall_statement",  "xx_unset_statement",  "xx_throw_statement",
  "xx_declare_statement",  "xx_break_statement",  "xx_continue_statement",  "xx_while_statement",
  "xx_do_while_statement",  "xx_switch_statement",  "xx_for_statement",  "xx_empty_statement",
  "xx_eval_expr",  "xx_case_clauses",  "xx_case_clause",  "xx_common_expr",
  "xx_let_assignments",  "xx_let_assignment",  "xx_assignment_operator",  "xx_assign_expr",
  "xx_array_offset_list",  "xx_array_offset",  "xx_index_expr",  "xx_echo_expressions",
  "xx_echo_expression",  "xx_mcall_expr",  "xx_fcall_expr",  "xx_scall_expr",
  "xx_fetch_expr",  "xx_declare_variable_list",  "xx_declare_variable",  "xx_array_list",
  "xx_call_parameters",  "xx_call_parameter",  "xx_array_item",  "xx_array_key",
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
 /*   6 */ "xx_top_statement ::= xx_interface_def",
 /*   7 */ "xx_top_statement ::= xx_comment",
 /*   8 */ "xx_top_statement ::= xx_cblock",
 /*   9 */ "xx_namespace_def ::= NAMESPACE IDENTIFIER DOTCOMMA",
 /*  10 */ "xx_interface_def ::= INTERFACE IDENTIFIER xx_interface_body",
 /*  11 */ "xx_interface_def ::= INTERFACE IDENTIFIER EXTENDS IDENTIFIER xx_interface_body",
 /*  12 */ "xx_class_def ::= CLASS IDENTIFIER xx_class_body",
 /*  13 */ "xx_class_def ::= CLASS IDENTIFIER EXTENDS IDENTIFIER xx_class_body",
 /*  14 */ "xx_class_def ::= CLASS IDENTIFIER IMPLEMENTS xx_implements_list xx_class_body",
 /*  15 */ "xx_class_def ::= CLASS IDENTIFIER EXTENDS IDENTIFIER IMPLEMENTS xx_implements_list xx_class_body",
 /*  16 */ "xx_class_def ::= ABSTRACT CLASS IDENTIFIER xx_class_body",
 /*  17 */ "xx_class_def ::= ABSTRACT CLASS IDENTIFIER EXTENDS IDENTIFIER xx_class_body",
 /*  18 */ "xx_class_def ::= ABSTRACT CLASS IDENTIFIER IMPLEMENTS xx_implements_list xx_class_body",
 /*  19 */ "xx_class_def ::= FINAL CLASS IDENTIFIER xx_class_body",
 /*  20 */ "xx_class_def ::= FINAL CLASS IDENTIFIER EXTENDS IDENTIFIER xx_class_body",
 /*  21 */ "xx_class_body ::= BRACKET_OPEN BRACKET_CLOSE",
 /*  22 */ "xx_class_body ::= BRACKET_OPEN xx_class_definition BRACKET_CLOSE",
 /*  23 */ "xx_implements_list ::= xx_implements_list COMMA xx_implements",
 /*  24 */ "xx_implements_list ::= xx_implements",
 /*  25 */ "xx_implements ::= IDENTIFIER",
 /*  26 */ "xx_interface_body ::= BRACKET_OPEN BRACKET_CLOSE",
 /*  27 */ "xx_interface_body ::= BRACKET_OPEN xx_interface_definition BRACKET_CLOSE",
 /*  28 */ "xx_class_definition ::= xx_class_properties_definition",
 /*  29 */ "xx_class_definition ::= xx_class_consts_definition",
 /*  30 */ "xx_class_definition ::= xx_class_methods_definition",
 /*  31 */ "xx_class_definition ::= xx_class_properties_definition xx_class_methods_definition",
 /*  32 */ "xx_class_definition ::= xx_class_properties_definition xx_class_consts_definition",
 /*  33 */ "xx_class_definition ::= xx_class_consts_definition xx_class_properties_definition",
 /*  34 */ "xx_class_definition ::= xx_class_consts_definition xx_class_methods_definition",
 /*  35 */ "xx_class_definition ::= xx_class_properties_definition xx_class_consts_definition xx_class_methods_definition",
 /*  36 */ "xx_class_definition ::= xx_class_consts_definition xx_class_properties_definition xx_class_methods_definition",
 /*  37 */ "xx_interface_definition ::= xx_class_consts_definition",
 /*  38 */ "xx_interface_definition ::= xx_interface_methods_definition",
 /*  39 */ "xx_interface_definition ::= xx_class_consts_definition xx_interface_methods_definition",
 /*  40 */ "xx_class_properties_definition ::= xx_class_properties_definition xx_class_property_definition",
 /*  41 */ "xx_class_properties_definition ::= xx_class_property_definition",
 /*  42 */ "xx_class_property_definition ::= COMMENT xx_visibility_list IDENTIFIER DOTCOMMA",
 /*  43 */ "xx_class_property_definition ::= xx_visibility_list IDENTIFIER DOTCOMMA",
 /*  44 */ "xx_class_property_definition ::= COMMENT xx_visibility_list IDENTIFIER ASSIGN xx_literal_expr DOTCOMMA",
 /*  45 */ "xx_class_property_definition ::= xx_visibility_list IDENTIFIER ASSIGN xx_literal_expr DOTCOMMA",
 /*  46 */ "xx_class_property_definition ::= COMMENT xx_visibility_list IDENTIFIER xx_class_property_shortcuts DOTCOMMA",
 /*  47 */ "xx_class_property_definition ::= xx_visibility_list IDENTIFIER xx_class_property_shortcuts DOTCOMMA",
 /*  48 */ "xx_class_property_definition ::= COMMENT xx_visibility_list IDENTIFIER ASSIGN xx_literal_expr xx_class_property_shortcuts DOTCOMMA",
 /*  49 */ "xx_class_property_definition ::= xx_visibility_list IDENTIFIER ASSIGN xx_literal_expr xx_class_property_shortcuts DOTCOMMA",
 /*  50 */ "xx_class_property_shortcuts ::= BRACKET_OPEN BRACKET_CLOSE",
 /*  51 */ "xx_class_property_shortcuts ::= BRACKET_OPEN xx_class_property_shortcuts_list BRACKET_CLOSE",
 /*  52 */ "xx_class_property_shortcuts_list ::= xx_class_property_shortcuts_list COMMA xx_class_property_shortcut",
 /*  53 */ "xx_class_property_shortcuts_list ::= xx_class_property_shortcut",
 /*  54 */ "xx_class_property_shortcut ::= IDENTIFIER",
 /*  55 */ "xx_class_property_shortcut ::= COMMENT IDENTIFIER",
 /*  56 */ "xx_class_consts_definition ::= xx_class_consts_definition xx_class_const_definition",
 /*  57 */ "xx_class_consts_definition ::= xx_class_const_definition",
 /*  58 */ "xx_class_methods_definition ::= xx_class_methods_definition xx_class_method_definition",
 /*  59 */ "xx_class_methods_definition ::= xx_class_method_definition",
 /*  60 */ "xx_interface_methods_definition ::= xx_interface_methods_definition xx_interface_method_definition",
 /*  61 */ "xx_interface_methods_definition ::= xx_interface_method_definition",
 /*  62 */ "xx_class_const_definition ::= COMMENT CONST CONSTANT ASSIGN xx_literal_expr DOTCOMMA",
 /*  63 */ "xx_class_const_definition ::= CONST CONSTANT ASSIGN xx_literal_expr DOTCOMMA",
 /*  64 */ "xx_class_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  65 */ "xx_class_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  66 */ "xx_class_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  67 */ "xx_class_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  68 */ "xx_class_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  69 */ "xx_class_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  70 */ "xx_class_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  71 */ "xx_class_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  72 */ "xx_class_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE ARROW xx_method_return_type BRACKET_OPEN BRACKET_CLOSE",
 /*  73 */ "xx_class_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE ARROW xx_method_return_type BRACKET_OPEN BRACKET_CLOSE",
 /*  74 */ "xx_class_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE ARROW xx_method_return_type BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  75 */ "xx_class_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE ARROW xx_method_return_type BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  76 */ "xx_class_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE ARROW xx_method_return_type BRACKET_OPEN BRACKET_CLOSE",
 /*  77 */ "xx_class_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE ARROW xx_method_return_type BRACKET_OPEN BRACKET_CLOSE",
 /*  78 */ "xx_class_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE ARROW xx_method_return_type BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  79 */ "xx_class_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE ARROW xx_method_return_type BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  80 */ "xx_interface_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE ARROW xx_method_return_type DOTCOMMA",
 /*  81 */ "xx_interface_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE ARROW xx_method_return_type DOTCOMMA",
 /*  82 */ "xx_interface_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE ARROW xx_method_return_type DOTCOMMA",
 /*  83 */ "xx_interface_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE ARROW xx_method_return_type DOTCOMMA",
 /*  84 */ "xx_interface_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE DOTCOMMA",
 /*  85 */ "xx_interface_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE DOTCOMMA",
 /*  86 */ "xx_interface_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE DOTCOMMA",
 /*  87 */ "xx_interface_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE DOTCOMMA",
 /*  88 */ "xx_visibility_list ::= xx_visibility_list xx_visibility",
 /*  89 */ "xx_visibility_list ::= xx_visibility",
 /*  90 */ "xx_visibility ::= PUBLIC",
 /*  91 */ "xx_visibility ::= PROTECTED",
 /*  92 */ "xx_visibility ::= PRIVATE",
 /*  93 */ "xx_visibility ::= STATIC",
 /*  94 */ "xx_visibility ::= SCOPED",
 /*  95 */ "xx_visibility ::= INLINE",
 /*  96 */ "xx_visibility ::= ABSTRACT",
 /*  97 */ "xx_visibility ::= FINAL",
 /*  98 */ "xx_method_return_type ::= VOID",
 /*  99 */ "xx_method_return_type ::= xx_method_return_type_list",
 /* 100 */ "xx_method_return_type_list ::= xx_method_return_type_list BITWISE_OR xx_method_return_type_item",
 /* 101 */ "xx_method_return_type_list ::= xx_method_return_type_item",
 /* 102 */ "xx_method_return_type_item ::= xx_parameter_type",
 /* 103 */ "xx_method_return_type_item ::= xx_parameter_type NOT",
 /* 104 */ "xx_method_return_type_item ::= xx_parameter_cast",
 /* 105 */ "xx_method_return_type_item ::= xx_parameter_cast_collection",
 /* 106 */ "xx_parameter_list ::= xx_parameter_list COMMA xx_parameter",
 /* 107 */ "xx_parameter_list ::= xx_parameter",
 /* 108 */ "xx_parameter ::= IDENTIFIER",
 /* 109 */ "xx_parameter ::= xx_parameter_type IDENTIFIER",
 /* 110 */ "xx_parameter ::= xx_parameter_type NOT IDENTIFIER",
 /* 111 */ "xx_parameter ::= xx_parameter_cast IDENTIFIER",
 /* 112 */ "xx_parameter ::= IDENTIFIER ASSIGN xx_literal_expr",
 /* 113 */ "xx_parameter ::= xx_parameter_type IDENTIFIER ASSIGN xx_literal_expr",
 /* 114 */ "xx_parameter ::= xx_parameter_type NOT IDENTIFIER ASSIGN xx_literal_expr",
 /* 115 */ "xx_parameter ::= xx_parameter_cast IDENTIFIER ASSIGN xx_literal_expr",
 /* 116 */ "xx_parameter_cast ::= LESS IDENTIFIER GREATER",
 /* 117 */ "xx_parameter_cast_collection ::= LESS IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE GREATER",
 /* 118 */ "xx_parameter_type ::= TYPE_INTEGER",
 /* 119 */ "xx_parameter_type ::= TYPE_UINTEGER",
 /* 120 */ "xx_parameter_type ::= TYPE_LONG",
 /* 121 */ "xx_parameter_type ::= TYPE_ULONG",
 /* 122 */ "xx_parameter_type ::= TYPE_CHAR",
 /* 123 */ "xx_parameter_type ::= TYPE_UCHAR",
 /* 124 */ "xx_parameter_type ::= TYPE_DOUBLE",
 /* 125 */ "xx_parameter_type ::= TYPE_BOOL",
 /* 126 */ "xx_parameter_type ::= TYPE_STRING",
 /* 127 */ "xx_parameter_type ::= TYPE_ARRAY",
 /* 128 */ "xx_parameter_type ::= TYPE_VAR",
 /* 129 */ "xx_statement_list ::= xx_statement_list xx_statement",
 /* 130 */ "xx_statement_list ::= xx_statement",
 /* 131 */ "xx_statement ::= xx_cblock",
 /* 132 */ "xx_statement ::= xx_let_statement",
 /* 133 */ "xx_statement ::= xx_if_statement",
 /* 134 */ "xx_statement ::= xx_loop_statement",
 /* 135 */ "xx_statement ::= xx_echo_statement",
 /* 136 */ "xx_statement ::= xx_return_statement",
 /* 137 */ "xx_statement ::= xx_fetch_statement",
 /* 138 */ "xx_statement ::= xx_fcall_statement",
 /* 139 */ "xx_statement ::= xx_mcall_statement",
 /* 140 */ "xx_statement ::= xx_scall_statement",
 /* 141 */ "xx_statement ::= xx_unset_statement",
 /* 142 */ "xx_statement ::= xx_throw_statement",
 /* 143 */ "xx_statement ::= xx_declare_statement",
 /* 144 */ "xx_statement ::= xx_break_statement",
 /* 145 */ "xx_statement ::= xx_continue_statement",
 /* 146 */ "xx_statement ::= xx_while_statement",
 /* 147 */ "xx_statement ::= xx_do_while_statement",
 /* 148 */ "xx_statement ::= xx_switch_statement",
 /* 149 */ "xx_statement ::= xx_for_statement",
 /* 150 */ "xx_statement ::= xx_comment",
 /* 151 */ "xx_statement ::= xx_empty_statement",
 /* 152 */ "xx_empty_statement ::= DOTCOMMA",
 /* 153 */ "xx_break_statement ::= BREAK DOTCOMMA",
 /* 154 */ "xx_continue_statement ::= CONTINUE DOTCOMMA",
 /* 155 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /* 156 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE ELSE BRACKET_OPEN BRACKET_CLOSE",
 /* 157 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 158 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE ELSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 159 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE ELSE BRACKET_OPEN BRACKET_CLOSE",
 /* 160 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE ELSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 161 */ "xx_switch_statement ::= SWITCH xx_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /* 162 */ "xx_switch_statement ::= SWITCH xx_eval_expr BRACKET_OPEN xx_case_clauses BRACKET_CLOSE",
 /* 163 */ "xx_case_clauses ::= xx_case_clauses xx_case_clause",
 /* 164 */ "xx_case_clauses ::= xx_case_clause",
 /* 165 */ "xx_case_clause ::= CASE xx_literal_expr COLON",
 /* 166 */ "xx_case_clause ::= CASE xx_literal_expr COLON xx_statement_list",
 /* 167 */ "xx_case_clause ::= DEFAULT COLON xx_statement_list",
 /* 168 */ "xx_loop_statement ::= LOOP BRACKET_OPEN BRACKET_CLOSE",
 /* 169 */ "xx_loop_statement ::= LOOP BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 170 */ "xx_while_statement ::= WHILE xx_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /* 171 */ "xx_while_statement ::= WHILE xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 172 */ "xx_do_while_statement ::= DO BRACKET_OPEN BRACKET_CLOSE WHILE xx_eval_expr DOTCOMMA",
 /* 173 */ "xx_do_while_statement ::= DO BRACKET_OPEN xx_statement_list BRACKET_CLOSE WHILE xx_eval_expr DOTCOMMA",
 /* 174 */ "xx_for_statement ::= FOR IDENTIFIER IN xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 175 */ "xx_for_statement ::= FOR IDENTIFIER IN REVERSE xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 176 */ "xx_for_statement ::= FOR IDENTIFIER COMMA IDENTIFIER IN xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 177 */ "xx_for_statement ::= FOR IDENTIFIER COMMA IDENTIFIER IN REVERSE xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 178 */ "xx_let_statement ::= LET xx_let_assignments DOTCOMMA",
 /* 179 */ "xx_let_assignments ::= xx_let_assignments COMMA xx_let_assignment",
 /* 180 */ "xx_let_assignments ::= xx_let_assignment",
 /* 181 */ "xx_assignment_operator ::= ASSIGN",
 /* 182 */ "xx_assignment_operator ::= ADDASSIGN",
 /* 183 */ "xx_assignment_operator ::= SUBASSIGN",
 /* 184 */ "xx_assignment_operator ::= MULASSIGN",
 /* 185 */ "xx_assignment_operator ::= DIVASSIGN",
 /* 186 */ "xx_assignment_operator ::= CONCATASSIGN",
 /* 187 */ "xx_let_assignment ::= IDENTIFIER xx_assignment_operator xx_assign_expr",
 /* 188 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER xx_assignment_operator xx_assign_expr",
 /* 189 */ "xx_let_assignment ::= IDENTIFIER ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 190 */ "xx_let_assignment ::= IDENTIFIER ARROW BRACKET_OPEN STRING BRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 191 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 192 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER xx_array_offset_list xx_assignment_operator xx_assign_expr",
 /* 193 */ "xx_let_assignment ::= IDENTIFIER DOUBLECOLON IDENTIFIER xx_assignment_operator xx_assign_expr",
 /* 194 */ "xx_let_assignment ::= IDENTIFIER DOUBLECOLON IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 195 */ "xx_let_assignment ::= IDENTIFIER DOUBLECOLON IDENTIFIER xx_array_offset_list xx_assignment_operator xx_assign_expr",
 /* 196 */ "xx_let_assignment ::= IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 197 */ "xx_let_assignment ::= IDENTIFIER xx_array_offset_list xx_assignment_operator xx_assign_expr",
 /* 198 */ "xx_array_offset_list ::= xx_array_offset_list xx_array_offset",
 /* 199 */ "xx_array_offset_list ::= xx_array_offset",
 /* 200 */ "xx_array_offset ::= SBRACKET_OPEN xx_index_expr SBRACKET_CLOSE",
 /* 201 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER INCR",
 /* 202 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER DECR",
 /* 203 */ "xx_let_assignment ::= IDENTIFIER INCR",
 /* 204 */ "xx_let_assignment ::= IDENTIFIER DECR",
 /* 205 */ "xx_index_expr ::= xx_common_expr",
 /* 206 */ "xx_echo_statement ::= ECHO xx_echo_expressions DOTCOMMA",
 /* 207 */ "xx_echo_expressions ::= xx_echo_expressions COMMA xx_echo_expression",
 /* 208 */ "xx_echo_expressions ::= xx_echo_expression",
 /* 209 */ "xx_echo_expression ::= xx_common_expr",
 /* 210 */ "xx_mcall_statement ::= xx_mcall_expr DOTCOMMA",
 /* 211 */ "xx_fcall_statement ::= xx_fcall_expr DOTCOMMA",
 /* 212 */ "xx_scall_statement ::= xx_scall_expr DOTCOMMA",
 /* 213 */ "xx_fetch_statement ::= xx_fetch_expr DOTCOMMA",
 /* 214 */ "xx_return_statement ::= RETURN xx_common_expr DOTCOMMA",
 /* 215 */ "xx_return_statement ::= RETURN DOTCOMMA",
 /* 216 */ "xx_unset_statement ::= UNSET xx_common_expr DOTCOMMA",
 /* 217 */ "xx_throw_statement ::= THROW xx_common_expr DOTCOMMA",
 /* 218 */ "xx_declare_statement ::= TYPE_INTEGER xx_declare_variable_list DOTCOMMA",
 /* 219 */ "xx_declare_statement ::= TYPE_UINTEGER xx_declare_variable_list DOTCOMMA",
 /* 220 */ "xx_declare_statement ::= TYPE_CHAR xx_declare_variable_list DOTCOMMA",
 /* 221 */ "xx_declare_statement ::= TYPE_UCHAR xx_declare_variable_list DOTCOMMA",
 /* 222 */ "xx_declare_statement ::= TYPE_LONG xx_declare_variable_list DOTCOMMA",
 /* 223 */ "xx_declare_statement ::= TYPE_ULONG xx_declare_variable_list DOTCOMMA",
 /* 224 */ "xx_declare_statement ::= TYPE_DOUBLE xx_declare_variable_list DOTCOMMA",
 /* 225 */ "xx_declare_statement ::= TYPE_STRING xx_declare_variable_list DOTCOMMA",
 /* 226 */ "xx_declare_statement ::= TYPE_BOOL xx_declare_variable_list DOTCOMMA",
 /* 227 */ "xx_declare_statement ::= TYPE_VAR xx_declare_variable_list DOTCOMMA",
 /* 228 */ "xx_declare_variable_list ::= xx_declare_variable_list COMMA xx_declare_variable",
 /* 229 */ "xx_declare_variable_list ::= xx_declare_variable",
 /* 230 */ "xx_declare_variable ::= IDENTIFIER",
 /* 231 */ "xx_declare_variable ::= IDENTIFIER ASSIGN xx_literal_expr",
 /* 232 */ "xx_assign_expr ::= xx_common_expr",
 /* 233 */ "xx_common_expr ::= NOT xx_common_expr",
 /* 234 */ "xx_common_expr ::= ISSET xx_common_expr",
 /* 235 */ "xx_common_expr ::= REQUIRE xx_common_expr",
 /* 236 */ "xx_common_expr ::= CLONE xx_common_expr",
 /* 237 */ "xx_common_expr ::= EMPTY xx_common_expr",
 /* 238 */ "xx_common_expr ::= LIKELY xx_common_expr",
 /* 239 */ "xx_common_expr ::= UNLIKELY xx_common_expr",
 /* 240 */ "xx_common_expr ::= xx_common_expr EQUALS xx_common_expr",
 /* 241 */ "xx_common_expr ::= xx_common_expr NOTEQUALS xx_common_expr",
 /* 242 */ "xx_common_expr ::= xx_common_expr IDENTICAL xx_common_expr",
 /* 243 */ "xx_common_expr ::= xx_common_expr NOTIDENTICAL xx_common_expr",
 /* 244 */ "xx_common_expr ::= xx_common_expr LESS xx_common_expr",
 /* 245 */ "xx_common_expr ::= xx_common_expr GREATER xx_common_expr",
 /* 246 */ "xx_common_expr ::= xx_common_expr LESSEQUAL xx_common_expr",
 /* 247 */ "xx_common_expr ::= xx_common_expr GREATEREQUAL xx_common_expr",
 /* 248 */ "xx_common_expr ::= PARENTHESES_OPEN xx_common_expr PARENTHESES_CLOSE",
 /* 249 */ "xx_common_expr ::= PARENTHESES_OPEN xx_parameter_type PARENTHESES_CLOSE xx_common_expr",
 /* 250 */ "xx_common_expr ::= LESS IDENTIFIER GREATER xx_common_expr",
 /* 251 */ "xx_common_expr ::= xx_common_expr ARROW IDENTIFIER",
 /* 252 */ "xx_common_expr ::= xx_common_expr ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE",
 /* 253 */ "xx_common_expr ::= xx_common_expr ARROW BRACKET_OPEN STRING BRACKET_CLOSE",
 /* 254 */ "xx_common_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER",
 /* 255 */ "xx_common_expr ::= IDENTIFIER DOUBLECOLON CONSTANT",
 /* 256 */ "xx_common_expr ::= xx_common_expr SBRACKET_OPEN xx_common_expr SBRACKET_CLOSE",
 /* 257 */ "xx_common_expr ::= xx_common_expr ADD xx_common_expr",
 /* 258 */ "xx_common_expr ::= xx_common_expr SUB xx_common_expr",
 /* 259 */ "xx_common_expr ::= xx_common_expr MUL xx_common_expr",
 /* 260 */ "xx_common_expr ::= xx_common_expr DIV xx_common_expr",
 /* 261 */ "xx_common_expr ::= xx_common_expr MOD xx_common_expr",
 /* 262 */ "xx_common_expr ::= xx_common_expr CONCAT xx_common_expr",
 /* 263 */ "xx_common_expr ::= xx_common_expr AND xx_common_expr",
 /* 264 */ "xx_common_expr ::= xx_common_expr OR xx_common_expr",
 /* 265 */ "xx_common_expr ::= xx_common_expr INSTANCEOF xx_common_expr",
 /* 266 */ "xx_fetch_expr ::= FETCH IDENTIFIER COMMA xx_common_expr",
 /* 267 */ "xx_common_expr ::= xx_fetch_expr",
 /* 268 */ "xx_common_expr ::= TYPEOF xx_common_expr",
 /* 269 */ "xx_common_expr ::= IDENTIFIER",
 /* 270 */ "xx_common_expr ::= INTEGER",
 /* 271 */ "xx_common_expr ::= STRING",
 /* 272 */ "xx_common_expr ::= CHAR",
 /* 273 */ "xx_common_expr ::= DOUBLE",
 /* 274 */ "xx_common_expr ::= NULL",
 /* 275 */ "xx_common_expr ::= TRUE",
 /* 276 */ "xx_common_expr ::= FALSE",
 /* 277 */ "xx_common_expr ::= CONSTANT",
 /* 278 */ "xx_common_expr ::= SBRACKET_OPEN SBRACKET_CLOSE",
 /* 279 */ "xx_common_expr ::= SBRACKET_OPEN xx_array_list SBRACKET_CLOSE",
 /* 280 */ "xx_common_expr ::= NEW IDENTIFIER",
 /* 281 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 282 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 283 */ "xx_common_expr ::= NEW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE",
 /* 284 */ "xx_common_expr ::= NEW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 285 */ "xx_common_expr ::= NEW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 286 */ "xx_fcall_expr ::= IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 287 */ "xx_fcall_expr ::= IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 288 */ "xx_fcall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 289 */ "xx_fcall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 290 */ "xx_scall_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 291 */ "xx_scall_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 292 */ "xx_scall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE DOUBLECOLON IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 293 */ "xx_scall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE DOUBLECOLON IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 294 */ "xx_scall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE DOUBLECOLON BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 295 */ "xx_scall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE DOUBLECOLON BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 296 */ "xx_mcall_expr ::= xx_common_expr ARROW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 297 */ "xx_mcall_expr ::= xx_common_expr ARROW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 298 */ "xx_mcall_expr ::= xx_common_expr ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 299 */ "xx_mcall_expr ::= xx_common_expr ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 300 */ "xx_mcall_expr ::= xx_common_expr ARROW BRACKET_OPEN STRING BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 301 */ "xx_mcall_expr ::= xx_common_expr ARROW BRACKET_OPEN STRING BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 302 */ "xx_common_expr ::= xx_mcall_expr",
 /* 303 */ "xx_common_expr ::= xx_scall_expr",
 /* 304 */ "xx_common_expr ::= xx_fcall_expr",
 /* 305 */ "xx_call_parameters ::= xx_call_parameters COMMA xx_call_parameter",
 /* 306 */ "xx_call_parameters ::= xx_call_parameter",
 /* 307 */ "xx_call_parameter ::= xx_common_expr",
 /* 308 */ "xx_array_list ::= xx_array_list COMMA xx_array_item",
 /* 309 */ "xx_array_list ::= xx_array_item",
 /* 310 */ "xx_array_item ::= xx_array_key COLON xx_array_value",
 /* 311 */ "xx_array_item ::= xx_array_value",
 /* 312 */ "xx_array_key ::= IDENTIFIER",
 /* 313 */ "xx_array_key ::= STRING",
 /* 314 */ "xx_array_key ::= INTEGER",
 /* 315 */ "xx_array_value ::= xx_common_expr",
 /* 316 */ "xx_literal_expr ::= INTEGER",
 /* 317 */ "xx_literal_expr ::= CHAR",
 /* 318 */ "xx_literal_expr ::= STRING",
 /* 319 */ "xx_literal_expr ::= DOUBLE",
 /* 320 */ "xx_literal_expr ::= NULL",
 /* 321 */ "xx_literal_expr ::= FALSE",
 /* 322 */ "xx_literal_expr ::= TRUE",
 /* 323 */ "xx_literal_expr ::= IDENTIFIER DOUBLECOLON CONSTANT",
 /* 324 */ "xx_literal_expr ::= CONSTANT",
 /* 325 */ "xx_eval_expr ::= xx_common_expr",
 /* 326 */ "xx_comment ::= COMMENT",
 /* 327 */ "xx_cblock ::= CBLOCK",
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
    case 94:
    case 95:
    case 96:
    case 97:
    case 98:
    case 99:
    case 100:
    case 101:
    case 102:
    case 103:
    case 104:
// 1021 "parser.lemon"
{
	/*if ((yypminor->yy0)) {
		if ((yypminor->yy0)->free_flag) {
			efree((yypminor->yy0)->token);
		}
		efree((yypminor->yy0));
	}*/
}
// 3190 "parser.c"
      break;
    case 107:
// 1034 "parser.lemon"
{ json_object_put((yypminor->yy259)); }
// 3195 "parser.c"
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
  { 106, 1 },
  { 107, 1 },
  { 108, 2 },
  { 108, 1 },
  { 109, 1 },
  { 109, 1 },
  { 109, 1 },
  { 109, 1 },
  { 109, 1 },
  { 110, 3 },
  { 112, 3 },
  { 112, 5 },
  { 111, 3 },
  { 111, 5 },
  { 111, 5 },
  { 111, 7 },
  { 111, 4 },
  { 111, 6 },
  { 111, 6 },
  { 111, 4 },
  { 111, 6 },
  { 116, 2 },
  { 116, 3 },
  { 117, 3 },
  { 117, 1 },
  { 119, 1 },
  { 115, 2 },
  { 115, 3 },
  { 118, 1 },
  { 118, 1 },
  { 118, 1 },
  { 118, 2 },
  { 118, 2 },
  { 118, 2 },
  { 118, 2 },
  { 118, 3 },
  { 118, 3 },
  { 120, 1 },
  { 120, 1 },
  { 120, 2 },
  { 121, 2 },
  { 121, 1 },
  { 125, 4 },
  { 125, 3 },
  { 125, 6 },
  { 125, 5 },
  { 125, 5 },
  { 125, 4 },
  { 125, 7 },
  { 125, 6 },
  { 128, 2 },
  { 128, 3 },
  { 129, 3 },
  { 129, 1 },
  { 130, 1 },
  { 130, 2 },
  { 122, 2 },
  { 122, 1 },
  { 123, 2 },
  { 123, 1 },
  { 124, 2 },
  { 124, 1 },
  { 131, 6 },
  { 131, 5 },
  { 132, 7 },
  { 132, 8 },
  { 132, 8 },
  { 132, 9 },
  { 132, 8 },
  { 132, 9 },
  { 132, 9 },
  { 132, 10 },
  { 132, 9 },
  { 132, 10 },
  { 132, 10 },
  { 132, 11 },
  { 132, 10 },
  { 132, 11 },
  { 132, 11 },
  { 132, 12 },
  { 133, 8 },
  { 133, 9 },
  { 133, 9 },
  { 133, 10 },
  { 133, 6 },
  { 133, 7 },
  { 133, 7 },
  { 133, 8 },
  { 126, 2 },
  { 126, 1 },
  { 137, 1 },
  { 137, 1 },
  { 137, 1 },
  { 137, 1 },
  { 137, 1 },
  { 137, 1 },
  { 137, 1 },
  { 137, 1 },
  { 136, 1 },
  { 136, 1 },
  { 138, 3 },
  { 138, 1 },
  { 139, 1 },
  { 139, 2 },
  { 139, 1 },
  { 139, 1 },
  { 134, 3 },
  { 134, 1 },
  { 143, 1 },
  { 143, 2 },
  { 143, 3 },
  { 143, 2 },
  { 143, 3 },
  { 143, 4 },
  { 143, 5 },
  { 143, 4 },
  { 141, 3 },
  { 142, 5 },
  { 140, 1 },
  { 140, 1 },
  { 140, 1 },
  { 140, 1 },
  { 140, 1 },
  { 140, 1 },
  { 140, 1 },
  { 140, 1 },
  { 140, 1 },
  { 140, 1 },
  { 140, 1 },
  { 135, 2 },
  { 135, 1 },
  { 144, 1 },
  { 144, 1 },
  { 144, 1 },
  { 144, 1 },
  { 144, 1 },
  { 144, 1 },
  { 144, 1 },
  { 144, 1 },
  { 144, 1 },
  { 144, 1 },
  { 144, 1 },
  { 144, 1 },
  { 144, 1 },
  { 144, 1 },
  { 144, 1 },
  { 144, 1 },
  { 144, 1 },
  { 144, 1 },
  { 144, 1 },
  { 144, 1 },
  { 144, 1 },
  { 163, 1 },
  { 157, 2 },
  { 158, 2 },
  { 146, 4 },
  { 146, 7 },
  { 146, 5 },
  { 146, 9 },
  { 146, 8 },
  { 146, 8 },
  { 161, 4 },
  { 161, 5 },
  { 165, 2 },
  { 165, 1 },
  { 166, 3 },
  { 166, 4 },
  { 166, 3 },
  { 147, 3 },
  { 147, 4 },
  { 159, 4 },
  { 159, 5 },
  { 160, 6 },
  { 160, 7 },
  { 162, 7 },
  { 162, 8 },
  { 162, 9 },
  { 162, 10 },
  { 145, 3 },
  { 168, 3 },
  { 168, 1 },
  { 170, 1 },
  { 170, 1 },
  { 170, 1 },
  { 170, 1 },
  { 170, 1 },
  { 170, 1 },
  { 169, 3 },
  { 169, 5 },
  { 169, 7 },
  { 169, 7 },
  { 169, 7 },
  { 169, 6 },
  { 169, 5 },
  { 169, 7 },
  { 169, 6 },
  { 169, 5 },
  { 169, 4 },
  { 172, 2 },
  { 172, 1 },
  { 173, 3 },
  { 169, 4 },
  { 169, 4 },
  { 169, 2 },
  { 169, 2 },
  { 174, 1 },
  { 148, 3 },
  { 175, 3 },
  { 175, 1 },
  { 176, 1 },
  { 152, 2 },
  { 151, 2 },
  { 153, 2 },
  { 150, 2 },
  { 149, 3 },
  { 149, 2 },
  { 154, 3 },
  { 155, 3 },
  { 156, 3 },
  { 156, 3 },
  { 156, 3 },
  { 156, 3 },
  { 156, 3 },
  { 156, 3 },
  { 156, 3 },
  { 156, 3 },
  { 156, 3 },
  { 156, 3 },
  { 181, 3 },
  { 181, 1 },
  { 182, 1 },
  { 182, 3 },
  { 171, 1 },
  { 167, 2 },
  { 167, 2 },
  { 167, 2 },
  { 167, 2 },
  { 167, 2 },
  { 167, 2 },
  { 167, 2 },
  { 167, 3 },
  { 167, 3 },
  { 167, 3 },
  { 167, 3 },
  { 167, 3 },
  { 167, 3 },
  { 167, 3 },
  { 167, 3 },
  { 167, 3 },
  { 167, 4 },
  { 167, 4 },
  { 167, 3 },
  { 167, 5 },
  { 167, 5 },
  { 167, 3 },
  { 167, 3 },
  { 167, 4 },
  { 167, 3 },
  { 167, 3 },
  { 167, 3 },
  { 167, 3 },
  { 167, 3 },
  { 167, 3 },
  { 167, 3 },
  { 167, 3 },
  { 167, 3 },
  { 180, 4 },
  { 167, 1 },
  { 167, 2 },
  { 167, 1 },
  { 167, 1 },
  { 167, 1 },
  { 167, 1 },
  { 167, 1 },
  { 167, 1 },
  { 167, 1 },
  { 167, 1 },
  { 167, 1 },
  { 167, 2 },
  { 167, 3 },
  { 167, 2 },
  { 167, 4 },
  { 167, 5 },
  { 167, 4 },
  { 167, 6 },
  { 167, 7 },
  { 178, 4 },
  { 178, 3 },
  { 178, 6 },
  { 178, 5 },
  { 179, 6 },
  { 179, 5 },
  { 179, 8 },
  { 179, 7 },
  { 179, 10 },
  { 179, 9 },
  { 177, 6 },
  { 177, 5 },
  { 177, 8 },
  { 177, 7 },
  { 177, 8 },
  { 177, 7 },
  { 167, 1 },
  { 167, 1 },
  { 167, 1 },
  { 184, 3 },
  { 184, 1 },
  { 185, 1 },
  { 183, 3 },
  { 183, 1 },
  { 186, 3 },
  { 186, 1 },
  { 187, 1 },
  { 187, 1 },
  { 187, 1 },
  { 188, 1 },
  { 127, 1 },
  { 127, 1 },
  { 127, 1 },
  { 127, 1 },
  { 127, 1 },
  { 127, 1 },
  { 127, 1 },
  { 127, 3 },
  { 127, 1 },
  { 164, 1 },
  { 113, 1 },
  { 114, 1 },
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
// 1030 "parser.lemon"
{
	status->ret = yymsp[0].minor.yy259;
}
// 3740 "parser.c"
        break;
      case 1:
      case 4:
      case 5:
      case 6:
      case 7:
      case 8:
      case 131:
      case 132:
      case 133:
      case 134:
      case 135:
      case 136:
      case 137:
      case 138:
      case 139:
      case 140:
      case 141:
      case 142:
      case 143:
      case 144:
      case 145:
      case 146:
      case 147:
      case 148:
      case 149:
      case 150:
      case 151:
      case 205:
      case 232:
      case 267:
      case 302:
      case 303:
      case 304:
      case 307:
      case 315:
      case 325:
// 1036 "parser.lemon"
{
	yygotominor.yy259 = yymsp[0].minor.yy259;
}
// 3782 "parser.c"
        break;
      case 2:
      case 40:
      case 56:
      case 58:
      case 60:
      case 88:
      case 129:
      case 163:
      case 198:
// 1040 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_list(yymsp[-1].minor.yy259, yymsp[0].minor.yy259);
}
// 3797 "parser.c"
        break;
      case 3:
      case 24:
      case 41:
      case 53:
      case 57:
      case 59:
      case 61:
      case 89:
      case 101:
      case 107:
      case 130:
      case 164:
      case 180:
      case 199:
      case 208:
      case 229:
      case 306:
      case 309:
// 1044 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_list(NULL, yymsp[0].minor.yy259);
}
// 3821 "parser.c"
        break;
      case 9:
// 1069 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_namespace(yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(39,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 3830 "parser.c"
        break;
      case 10:
// 1073 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_interface(yymsp[-1].minor.yy0, yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(42,&yymsp[-2].minor);
}
// 3838 "parser.c"
        break;
      case 11:
// 1077 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_interface(yymsp[-3].minor.yy0, yymsp[0].minor.yy259, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
}
// 3847 "parser.c"
        break;
      case 12:
// 1081 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy259, 0, 0, NULL, NULL, status->scanner_state);
  yy_destructor(44,&yymsp[-2].minor);
}
// 3855 "parser.c"
        break;
      case 13:
// 1085 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy259, 0, 0, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(44,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
}
// 3864 "parser.c"
        break;
      case 14:
// 1089 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy259, 0, 0, NULL, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(44,&yymsp[-4].minor);
  yy_destructor(45,&yymsp[-2].minor);
}
// 3873 "parser.c"
        break;
      case 15:
// 1093 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class(yymsp[-5].minor.yy0, yymsp[0].minor.yy259, 0, 0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(44,&yymsp[-6].minor);
  yy_destructor(43,&yymsp[-4].minor);
  yy_destructor(45,&yymsp[-2].minor);
}
// 3883 "parser.c"
        break;
      case 16:
// 1097 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy259, 1, 0, NULL, NULL, status->scanner_state);
  yy_destructor(46,&yymsp[-3].minor);
  yy_destructor(44,&yymsp[-2].minor);
}
// 3892 "parser.c"
        break;
      case 17:
// 1101 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy259, 1, 0, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(46,&yymsp[-5].minor);
  yy_destructor(44,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
}
// 3902 "parser.c"
        break;
      case 18:
// 1105 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy259, 1, 0, NULL, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(46,&yymsp[-5].minor);
  yy_destructor(44,&yymsp[-4].minor);
  yy_destructor(45,&yymsp[-2].minor);
}
// 3912 "parser.c"
        break;
      case 19:
// 1109 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy259, 0, 1, NULL, NULL, status->scanner_state);
  yy_destructor(47,&yymsp[-3].minor);
  yy_destructor(44,&yymsp[-2].minor);
}
// 3921 "parser.c"
        break;
      case 20:
// 1113 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy259, 0, 1, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(47,&yymsp[-5].minor);
  yy_destructor(44,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
}
// 3931 "parser.c"
        break;
      case 21:
      case 50:
// 1117 "parser.lemon"
{
	yygotominor.yy259 = NULL;
  yy_destructor(48,&yymsp[-1].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 3941 "parser.c"
        break;
      case 22:
      case 51:
// 1121 "parser.lemon"
{
	yygotominor.yy259 = yymsp[-1].minor.yy259;
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 3951 "parser.c"
        break;
      case 23:
      case 52:
      case 106:
      case 179:
      case 207:
      case 228:
      case 305:
      case 308:
// 1125 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_list(yymsp[-2].minor.yy259, yymsp[0].minor.yy259);
  yy_destructor(6,&yymsp[-1].minor);
}
// 3966 "parser.c"
        break;
      case 25:
      case 269:
      case 312:
// 1133 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state);
}
// 3975 "parser.c"
        break;
      case 26:
// 1137 "parser.lemon"
{
  yygotominor.yy259 = NULL;
  yy_destructor(48,&yymsp[-1].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 3984 "parser.c"
        break;
      case 27:
// 1141 "parser.lemon"
{
  yygotominor.yy259 = yymsp[-1].minor.yy259;
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 3993 "parser.c"
        break;
      case 28:
// 1145 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_definition(yymsp[0].minor.yy259, NULL, NULL, status->scanner_state);
}
// 4000 "parser.c"
        break;
      case 29:
// 1149 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_definition(NULL, NULL, yymsp[0].minor.yy259, status->scanner_state);
}
// 4007 "parser.c"
        break;
      case 30:
// 1153 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_definition(NULL, yymsp[0].minor.yy259, NULL, status->scanner_state);
}
// 4014 "parser.c"
        break;
      case 31:
// 1157 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_definition(yymsp[-1].minor.yy259, yymsp[0].minor.yy259, NULL, status->scanner_state);
}
// 4021 "parser.c"
        break;
      case 32:
// 1161 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_definition(yymsp[-1].minor.yy259, NULL, yymsp[0].minor.yy259, status->scanner_state);
}
// 4028 "parser.c"
        break;
      case 33:
// 1165 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_definition(yymsp[0].minor.yy259, NULL, yymsp[-1].minor.yy259, status->scanner_state);
}
// 4035 "parser.c"
        break;
      case 34:
// 1169 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_definition(NULL, yymsp[0].minor.yy259, yymsp[-1].minor.yy259, status->scanner_state);
}
// 4042 "parser.c"
        break;
      case 35:
// 1173 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_definition(yymsp[-2].minor.yy259, yymsp[0].minor.yy259, yymsp[-1].minor.yy259, status->scanner_state);
}
// 4049 "parser.c"
        break;
      case 36:
// 1177 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_definition(yymsp[-1].minor.yy259, yymsp[0].minor.yy259, yymsp[-2].minor.yy259, status->scanner_state);
}
// 4056 "parser.c"
        break;
      case 37:
// 1181 "parser.lemon"
{
  yygotominor.yy259 = xx_ret_interface_definition(NULL, yymsp[0].minor.yy259, status->scanner_state);
}
// 4063 "parser.c"
        break;
      case 38:
// 1185 "parser.lemon"
{
  yygotominor.yy259 = xx_ret_interface_definition(yymsp[0].minor.yy259, NULL, status->scanner_state);
}
// 4070 "parser.c"
        break;
      case 39:
// 1189 "parser.lemon"
{
  yygotominor.yy259 = xx_ret_interface_definition(yymsp[0].minor.yy259, yymsp[-1].minor.yy259, status->scanner_state);
}
// 4077 "parser.c"
        break;
      case 42:
// 1202 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_property(yymsp[-2].minor.yy259, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(41,&yymsp[0].minor);
}
// 4085 "parser.c"
        break;
      case 43:
// 1206 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_property(yymsp[-2].minor.yy259, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(41,&yymsp[0].minor);
}
// 4093 "parser.c"
        break;
      case 44:
// 1210 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_property(yymsp[-4].minor.yy259, yymsp[-3].minor.yy0, yymsp[-1].minor.yy259, yymsp[-5].minor.yy0, NULL, status->scanner_state);
  yy_destructor(51,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4102 "parser.c"
        break;
      case 45:
// 1214 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_property(yymsp[-4].minor.yy259, yymsp[-3].minor.yy0, yymsp[-1].minor.yy259, NULL, NULL, status->scanner_state);
  yy_destructor(51,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4111 "parser.c"
        break;
      case 46:
// 1218 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_property(yymsp[-3].minor.yy259, yymsp[-2].minor.yy0, NULL, yymsp[-4].minor.yy0, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(41,&yymsp[0].minor);
}
// 4119 "parser.c"
        break;
      case 47:
// 1222 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_property(yymsp[-3].minor.yy259, yymsp[-2].minor.yy0, NULL, NULL, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(41,&yymsp[0].minor);
}
// 4127 "parser.c"
        break;
      case 48:
// 1226 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_property(yymsp[-5].minor.yy259, yymsp[-4].minor.yy0, yymsp[-2].minor.yy259, yymsp[-6].minor.yy0, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(51,&yymsp[-3].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4136 "parser.c"
        break;
      case 49:
// 1230 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_property(yymsp[-5].minor.yy259, yymsp[-4].minor.yy0, yymsp[-2].minor.yy259, NULL, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(51,&yymsp[-3].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4145 "parser.c"
        break;
      case 54:
// 1250 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_property_shortcut(NULL, yymsp[0].minor.yy0, status->scanner_state);
}
// 4152 "parser.c"
        break;
      case 55:
// 1254 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_property_shortcut(yymsp[-1].minor.yy0, yymsp[0].minor.yy0, status->scanner_state);
}
// 4159 "parser.c"
        break;
      case 62:
// 1283 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy259, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(52,&yymsp[-4].minor);
  yy_destructor(51,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4169 "parser.c"
        break;
      case 63:
// 1287 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy259, NULL, status->scanner_state);
  yy_destructor(52,&yymsp[-4].minor);
  yy_destructor(51,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4179 "parser.c"
        break;
      case 64:
// 1292 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_method(yymsp[-6].minor.yy259, yymsp[-4].minor.yy0, NULL, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-5].minor);
  yy_destructor(55,&yymsp[-3].minor);
  yy_destructor(36,&yymsp[-2].minor);
  yy_destructor(48,&yymsp[-1].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4191 "parser.c"
        break;
      case 65:
// 1296 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_method(yymsp[-7].minor.yy259, yymsp[-5].minor.yy0, yymsp[-3].minor.yy259, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-6].minor);
  yy_destructor(55,&yymsp[-4].minor);
  yy_destructor(36,&yymsp[-2].minor);
  yy_destructor(48,&yymsp[-1].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4203 "parser.c"
        break;
      case 66:
// 1300 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_method(yymsp[-7].minor.yy259, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy259, NULL, NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-6].minor);
  yy_destructor(55,&yymsp[-4].minor);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4215 "parser.c"
        break;
      case 67:
// 1304 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_method(yymsp[-8].minor.yy259, yymsp[-6].minor.yy0, yymsp[-4].minor.yy259, yymsp[-1].minor.yy259, NULL, NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-7].minor);
  yy_destructor(55,&yymsp[-5].minor);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4227 "parser.c"
        break;
      case 68:
// 1308 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_method(yymsp[-6].minor.yy259, yymsp[-4].minor.yy0, NULL, NULL, yymsp[-7].minor.yy0, NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-5].minor);
  yy_destructor(55,&yymsp[-3].minor);
  yy_destructor(36,&yymsp[-2].minor);
  yy_destructor(48,&yymsp[-1].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4239 "parser.c"
        break;
      case 69:
// 1312 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_method(yymsp[-7].minor.yy259, yymsp[-5].minor.yy0, yymsp[-3].minor.yy259, NULL, yymsp[-8].minor.yy0, NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-6].minor);
  yy_destructor(55,&yymsp[-4].minor);
  yy_destructor(36,&yymsp[-2].minor);
  yy_destructor(48,&yymsp[-1].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4251 "parser.c"
        break;
      case 70:
// 1316 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_method(yymsp[-7].minor.yy259, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy259, yymsp[-8].minor.yy0, NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-6].minor);
  yy_destructor(55,&yymsp[-4].minor);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4263 "parser.c"
        break;
      case 71:
// 1320 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_method(yymsp[-8].minor.yy259, yymsp[-6].minor.yy0, yymsp[-4].minor.yy259, yymsp[-1].minor.yy259, yymsp[-9].minor.yy0, NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-7].minor);
  yy_destructor(55,&yymsp[-5].minor);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4275 "parser.c"
        break;
      case 72:
// 1324 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_method(yymsp[-8].minor.yy259, yymsp[-6].minor.yy0, NULL, NULL, NULL, yymsp[-2].minor.yy259, status->scanner_state);
  yy_destructor(54,&yymsp[-7].minor);
  yy_destructor(55,&yymsp[-5].minor);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-1].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4288 "parser.c"
        break;
      case 73:
// 1328 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_method(yymsp[-9].minor.yy259, yymsp[-7].minor.yy0, yymsp[-5].minor.yy259, NULL, NULL, yymsp[-2].minor.yy259, status->scanner_state);
  yy_destructor(54,&yymsp[-8].minor);
  yy_destructor(55,&yymsp[-6].minor);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-1].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4301 "parser.c"
        break;
      case 74:
// 1332 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_method(yymsp[-9].minor.yy259, yymsp[-7].minor.yy0, NULL, yymsp[-1].minor.yy259, NULL, yymsp[-3].minor.yy259, status->scanner_state);
  yy_destructor(54,&yymsp[-8].minor);
  yy_destructor(55,&yymsp[-6].minor);
  yy_destructor(36,&yymsp[-5].minor);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4314 "parser.c"
        break;
      case 75:
// 1336 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_method(yymsp[-10].minor.yy259, yymsp[-8].minor.yy0, yymsp[-6].minor.yy259, yymsp[-1].minor.yy259, NULL, yymsp[-3].minor.yy259, status->scanner_state);
  yy_destructor(54,&yymsp[-9].minor);
  yy_destructor(55,&yymsp[-7].minor);
  yy_destructor(36,&yymsp[-5].minor);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4327 "parser.c"
        break;
      case 76:
// 1340 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_method(yymsp[-8].minor.yy259, yymsp[-6].minor.yy0, NULL, NULL, yymsp[-9].minor.yy0, yymsp[-2].minor.yy259, status->scanner_state);
  yy_destructor(54,&yymsp[-7].minor);
  yy_destructor(55,&yymsp[-5].minor);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-1].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4340 "parser.c"
        break;
      case 77:
// 1344 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_method(yymsp[-9].minor.yy259, yymsp[-7].minor.yy0, yymsp[-5].minor.yy259, NULL, yymsp[-10].minor.yy0, yymsp[-2].minor.yy259, status->scanner_state);
  yy_destructor(54,&yymsp[-8].minor);
  yy_destructor(55,&yymsp[-6].minor);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-1].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4353 "parser.c"
        break;
      case 78:
// 1348 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_method(yymsp[-9].minor.yy259, yymsp[-7].minor.yy0, NULL, yymsp[-1].minor.yy259, yymsp[-10].minor.yy0, yymsp[-3].minor.yy259, status->scanner_state);
  yy_destructor(54,&yymsp[-8].minor);
  yy_destructor(55,&yymsp[-6].minor);
  yy_destructor(36,&yymsp[-5].minor);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4366 "parser.c"
        break;
      case 79:
// 1352 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_method(yymsp[-10].minor.yy259, yymsp[-8].minor.yy0, yymsp[-6].minor.yy259, yymsp[-1].minor.yy259, yymsp[-11].minor.yy0, yymsp[-3].minor.yy259, status->scanner_state);
  yy_destructor(54,&yymsp[-9].minor);
  yy_destructor(55,&yymsp[-7].minor);
  yy_destructor(36,&yymsp[-5].minor);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4379 "parser.c"
        break;
      case 80:
// 1357 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_method(yymsp[-7].minor.yy259, yymsp[-5].minor.yy0, NULL, NULL, NULL, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(54,&yymsp[-6].minor);
  yy_destructor(55,&yymsp[-4].minor);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4391 "parser.c"
        break;
      case 81:
// 1361 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_method(yymsp[-8].minor.yy259, yymsp[-6].minor.yy0, yymsp[-4].minor.yy259, NULL, NULL, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(54,&yymsp[-7].minor);
  yy_destructor(55,&yymsp[-5].minor);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4403 "parser.c"
        break;
      case 82:
// 1365 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_method(yymsp[-7].minor.yy259, yymsp[-5].minor.yy0, NULL, NULL, yymsp[-8].minor.yy0, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(54,&yymsp[-6].minor);
  yy_destructor(55,&yymsp[-4].minor);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4415 "parser.c"
        break;
      case 83:
// 1369 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_method(yymsp[-8].minor.yy259, yymsp[-6].minor.yy0, yymsp[-4].minor.yy259, NULL, yymsp[-9].minor.yy0, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(54,&yymsp[-7].minor);
  yy_destructor(55,&yymsp[-5].minor);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4427 "parser.c"
        break;
      case 84:
// 1373 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_method(yymsp[-5].minor.yy259, yymsp[-3].minor.yy0, NULL, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-4].minor);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4438 "parser.c"
        break;
      case 85:
// 1377 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_method(yymsp[-6].minor.yy259, yymsp[-4].minor.yy0, yymsp[-2].minor.yy259, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-5].minor);
  yy_destructor(55,&yymsp[-3].minor);
  yy_destructor(36,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4449 "parser.c"
        break;
      case 86:
// 1381 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_method(yymsp[-5].minor.yy259, yymsp[-3].minor.yy0, NULL, NULL, yymsp[-6].minor.yy0, NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-4].minor);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4460 "parser.c"
        break;
      case 87:
// 1385 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_class_method(yymsp[-6].minor.yy259, yymsp[-4].minor.yy0, yymsp[-2].minor.yy259, NULL, yymsp[-7].minor.yy0, NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-5].minor);
  yy_destructor(55,&yymsp[-3].minor);
  yy_destructor(36,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4471 "parser.c"
        break;
      case 90:
// 1398 "parser.lemon"
{
	yygotominor.yy259 = json_object_new_string("public");
  yy_destructor(1,&yymsp[0].minor);
}
// 4479 "parser.c"
        break;
      case 91:
// 1402 "parser.lemon"
{
	yygotominor.yy259 = json_object_new_string("protected");
  yy_destructor(2,&yymsp[0].minor);
}
// 4487 "parser.c"
        break;
      case 92:
// 1406 "parser.lemon"
{
	yygotominor.yy259 = json_object_new_string("private");
  yy_destructor(4,&yymsp[0].minor);
}
// 4495 "parser.c"
        break;
      case 93:
// 1410 "parser.lemon"
{
	yygotominor.yy259 = json_object_new_string("static");
  yy_destructor(3,&yymsp[0].minor);
}
// 4503 "parser.c"
        break;
      case 94:
// 1414 "parser.lemon"
{
	yygotominor.yy259 = json_object_new_string("scoped");
  yy_destructor(5,&yymsp[0].minor);
}
// 4511 "parser.c"
        break;
      case 95:
// 1418 "parser.lemon"
{
	yygotominor.yy259 = json_object_new_string("inline");
  yy_destructor(56,&yymsp[0].minor);
}
// 4519 "parser.c"
        break;
      case 96:
// 1422 "parser.lemon"
{
	yygotominor.yy259 = json_object_new_string("abstract");
  yy_destructor(46,&yymsp[0].minor);
}
// 4527 "parser.c"
        break;
      case 97:
// 1426 "parser.lemon"
{
	yygotominor.yy259 = json_object_new_string("final");
  yy_destructor(47,&yymsp[0].minor);
}
// 4535 "parser.c"
        break;
      case 98:
// 1431 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_return_type(1, NULL, status->scanner_state);
  yy_destructor(57,&yymsp[0].minor);
}
// 4543 "parser.c"
        break;
      case 99:
// 1435 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_return_type(0, yymsp[0].minor.yy259, status->scanner_state);
}
// 4550 "parser.c"
        break;
      case 100:
// 1439 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_list(yymsp[-2].minor.yy259, yymsp[0].minor.yy259);
  yy_destructor(15,&yymsp[-1].minor);
}
// 4558 "parser.c"
        break;
      case 102:
// 1447 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_return_type_item(yymsp[0].minor.yy259, NULL, 0, 0, status->scanner_state);
}
// 4565 "parser.c"
        break;
      case 103:
// 1451 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_return_type_item(yymsp[-1].minor.yy259, NULL, 1, 0, status->scanner_state);
  yy_destructor(35,&yymsp[0].minor);
}
// 4573 "parser.c"
        break;
      case 104:
// 1455 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_return_type_item(NULL, yymsp[0].minor.yy259, 0, 0, status->scanner_state);
}
// 4580 "parser.c"
        break;
      case 105:
// 1459 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_return_type_item(NULL, yymsp[0].minor.yy259, 0, 1, status->scanner_state);
}
// 4587 "parser.c"
        break;
      case 108:
// 1472 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_parameter(NULL, NULL, yymsp[0].minor.yy0, NULL, 0, status->scanner_state);
}
// 4594 "parser.c"
        break;
      case 109:
// 1476 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_parameter(yymsp[-1].minor.yy259, NULL, yymsp[0].minor.yy0, NULL, 0, status->scanner_state);
}
// 4601 "parser.c"
        break;
      case 110:
// 1480 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_parameter(yymsp[-2].minor.yy259, NULL, yymsp[0].minor.yy0, NULL, 1, status->scanner_state);
  yy_destructor(35,&yymsp[-1].minor);
}
// 4609 "parser.c"
        break;
      case 111:
// 1484 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_parameter(NULL, yymsp[-1].minor.yy259, yymsp[0].minor.yy0, NULL, 0, status->scanner_state);
}
// 4616 "parser.c"
        break;
      case 112:
// 1488 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_parameter(NULL, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy259, 0, status->scanner_state);
  yy_destructor(51,&yymsp[-1].minor);
}
// 4624 "parser.c"
        break;
      case 113:
// 1492 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_parameter(yymsp[-3].minor.yy259, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy259, 0, status->scanner_state);
  yy_destructor(51,&yymsp[-1].minor);
}
// 4632 "parser.c"
        break;
      case 114:
// 1496 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_parameter(yymsp[-4].minor.yy259, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy259, 1, status->scanner_state);
  yy_destructor(35,&yymsp[-3].minor);
  yy_destructor(51,&yymsp[-1].minor);
}
// 4641 "parser.c"
        break;
      case 115:
// 1500 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_parameter(NULL, yymsp[-3].minor.yy259, yymsp[-2].minor.yy0, yymsp[0].minor.yy259, 0, status->scanner_state);
  yy_destructor(51,&yymsp[-1].minor);
}
// 4649 "parser.c"
        break;
      case 116:
// 1504 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(19,&yymsp[-2].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 4658 "parser.c"
        break;
      case 117:
// 1508 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(19,&yymsp[-4].minor);
  yy_destructor(37,&yymsp[-2].minor);
  yy_destructor(58,&yymsp[-1].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 4669 "parser.c"
        break;
      case 118:
// 1512 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_type(XX_TYPE_INTEGER);
  yy_destructor(59,&yymsp[0].minor);
}
// 4677 "parser.c"
        break;
      case 119:
// 1516 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_type(XX_TYPE_UINTEGER);
  yy_destructor(60,&yymsp[0].minor);
}
// 4685 "parser.c"
        break;
      case 120:
// 1520 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_type(XX_TYPE_LONG);
  yy_destructor(61,&yymsp[0].minor);
}
// 4693 "parser.c"
        break;
      case 121:
// 1524 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_type(XX_TYPE_ULONG);
  yy_destructor(62,&yymsp[0].minor);
}
// 4701 "parser.c"
        break;
      case 122:
// 1528 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_type(XX_TYPE_CHAR);
  yy_destructor(63,&yymsp[0].minor);
}
// 4709 "parser.c"
        break;
      case 123:
// 1532 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_type(XX_TYPE_UCHAR);
  yy_destructor(64,&yymsp[0].minor);
}
// 4717 "parser.c"
        break;
      case 124:
// 1536 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_type(XX_TYPE_DOUBLE);
  yy_destructor(65,&yymsp[0].minor);
}
// 4725 "parser.c"
        break;
      case 125:
// 1540 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_type(XX_TYPE_BOOL);
  yy_destructor(66,&yymsp[0].minor);
}
// 4733 "parser.c"
        break;
      case 126:
// 1544 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_type(XX_TYPE_STRING);
  yy_destructor(67,&yymsp[0].minor);
}
// 4741 "parser.c"
        break;
      case 127:
// 1548 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_type(XX_TYPE_ARRAY);
  yy_destructor(68,&yymsp[0].minor);
}
// 4749 "parser.c"
        break;
      case 128:
// 1552 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_type(XX_TYPE_VAR);
  yy_destructor(69,&yymsp[0].minor);
}
// 4757 "parser.c"
        break;
      case 152:
// 1650 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_empty_statement(status->scanner_state);
  yy_destructor(41,&yymsp[0].minor);
}
// 4765 "parser.c"
        break;
      case 153:
// 1654 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_break_statement(status->scanner_state);
  yy_destructor(70,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4774 "parser.c"
        break;
      case 154:
// 1658 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_continue_statement(status->scanner_state);
  yy_destructor(71,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4783 "parser.c"
        break;
      case 155:
// 1662 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_if_statement(yymsp[-2].minor.yy259, NULL, NULL, status->scanner_state);
  yy_destructor(72,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-1].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4793 "parser.c"
        break;
      case 156:
// 1666 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_if_statement(yymsp[-5].minor.yy259, NULL, NULL, status->scanner_state);
  yy_destructor(72,&yymsp[-6].minor);
  yy_destructor(48,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(73,&yymsp[-2].minor);
  yy_destructor(48,&yymsp[-1].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4806 "parser.c"
        break;
      case 157:
// 1670 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_if_statement(yymsp[-3].minor.yy259, yymsp[-1].minor.yy259, NULL, status->scanner_state);
  yy_destructor(72,&yymsp[-4].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4816 "parser.c"
        break;
      case 158:
// 1674 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_if_statement(yymsp[-7].minor.yy259, yymsp[-5].minor.yy259, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(72,&yymsp[-8].minor);
  yy_destructor(48,&yymsp[-6].minor);
  yy_destructor(49,&yymsp[-4].minor);
  yy_destructor(73,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4829 "parser.c"
        break;
      case 159:
// 1678 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_if_statement(yymsp[-6].minor.yy259, yymsp[-4].minor.yy259, NULL, status->scanner_state);
  yy_destructor(72,&yymsp[-7].minor);
  yy_destructor(48,&yymsp[-5].minor);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(73,&yymsp[-2].minor);
  yy_destructor(48,&yymsp[-1].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4842 "parser.c"
        break;
      case 160:
// 1682 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_if_statement(yymsp[-6].minor.yy259, NULL, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(72,&yymsp[-7].minor);
  yy_destructor(48,&yymsp[-5].minor);
  yy_destructor(49,&yymsp[-4].minor);
  yy_destructor(73,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4855 "parser.c"
        break;
      case 161:
// 1686 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_switch_statement(yymsp[-2].minor.yy259, NULL, status->scanner_state);
  yy_destructor(74,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-1].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4865 "parser.c"
        break;
      case 162:
// 1690 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_switch_statement(yymsp[-3].minor.yy259, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(74,&yymsp[-4].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4875 "parser.c"
        break;
      case 165:
// 1702 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_case_clause(yymsp[-1].minor.yy259, NULL, status->scanner_state);
  yy_destructor(75,&yymsp[-2].minor);
  yy_destructor(76,&yymsp[0].minor);
}
// 4884 "parser.c"
        break;
      case 166:
// 1706 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_case_clause(yymsp[-2].minor.yy259, yymsp[0].minor.yy259, status->scanner_state);
  yy_destructor(75,&yymsp[-3].minor);
  yy_destructor(76,&yymsp[-1].minor);
}
// 4893 "parser.c"
        break;
      case 167:
// 1710 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_case_clause(NULL, yymsp[0].minor.yy259, status->scanner_state);
  yy_destructor(77,&yymsp[-2].minor);
  yy_destructor(76,&yymsp[-1].minor);
}
// 4902 "parser.c"
        break;
      case 168:
// 1714 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_loop_statement(NULL, status->scanner_state);
  yy_destructor(78,&yymsp[-2].minor);
  yy_destructor(48,&yymsp[-1].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4912 "parser.c"
        break;
      case 169:
// 1718 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_loop_statement(yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(78,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4922 "parser.c"
        break;
      case 170:
// 1722 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_while_statement(yymsp[-2].minor.yy259, NULL, status->scanner_state);
  yy_destructor(79,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-1].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4932 "parser.c"
        break;
      case 171:
// 1726 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_while_statement(yymsp[-3].minor.yy259, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(79,&yymsp[-4].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4942 "parser.c"
        break;
      case 172:
// 1730 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_do_while_statement(yymsp[-1].minor.yy259, NULL, status->scanner_state);
  yy_destructor(80,&yymsp[-5].minor);
  yy_destructor(48,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(79,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4954 "parser.c"
        break;
      case 173:
// 1734 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_do_while_statement(yymsp[-1].minor.yy259, yymsp[-4].minor.yy259, status->scanner_state);
  yy_destructor(80,&yymsp[-6].minor);
  yy_destructor(48,&yymsp[-5].minor);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(79,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 4966 "parser.c"
        break;
      case 174:
// 1738 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_for_statement(yymsp[-3].minor.yy259, NULL, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(81,&yymsp[-6].minor);
  yy_destructor(82,&yymsp[-4].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4977 "parser.c"
        break;
      case 175:
// 1742 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_for_statement(yymsp[-3].minor.yy259, NULL, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(81,&yymsp[-7].minor);
  yy_destructor(82,&yymsp[-5].minor);
  yy_destructor(83,&yymsp[-4].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 4989 "parser.c"
        break;
      case 176:
// 1746 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_for_statement(yymsp[-3].minor.yy259, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(81,&yymsp[-8].minor);
  yy_destructor(6,&yymsp[-6].minor);
  yy_destructor(82,&yymsp[-4].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 5001 "parser.c"
        break;
      case 177:
// 1750 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_for_statement(yymsp[-3].minor.yy259, yymsp[-8].minor.yy0, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(81,&yymsp[-9].minor);
  yy_destructor(6,&yymsp[-7].minor);
  yy_destructor(82,&yymsp[-5].minor);
  yy_destructor(83,&yymsp[-4].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 5014 "parser.c"
        break;
      case 178:
// 1754 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_let_statement(yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(84,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 5023 "parser.c"
        break;
      case 181:
// 1767 "parser.lemon"
{
	yygotominor.yy259 = json_object_new_string("assign");
  yy_destructor(51,&yymsp[0].minor);
}
// 5031 "parser.c"
        break;
      case 182:
// 1772 "parser.lemon"
{
	yygotominor.yy259 = json_object_new_string("add-assign");
  yy_destructor(85,&yymsp[0].minor);
}
// 5039 "parser.c"
        break;
      case 183:
// 1777 "parser.lemon"
{
	yygotominor.yy259 = json_object_new_string("sub-assign");
  yy_destructor(86,&yymsp[0].minor);
}
// 5047 "parser.c"
        break;
      case 184:
// 1781 "parser.lemon"
{
	yygotominor.yy259 = json_object_new_string("mult-assign");
  yy_destructor(87,&yymsp[0].minor);
}
// 5055 "parser.c"
        break;
      case 185:
// 1785 "parser.lemon"
{
	yygotominor.yy259 = json_object_new_string("div-assign");
  yy_destructor(88,&yymsp[0].minor);
}
// 5063 "parser.c"
        break;
      case 186:
// 1789 "parser.lemon"
{
	yygotominor.yy259 = json_object_new_string("concat-assign");
  yy_destructor(89,&yymsp[0].minor);
}
// 5071 "parser.c"
        break;
      case 187:
// 1794 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_let_assignment("variable", yymsp[-1].minor.yy259, yymsp[-2].minor.yy0, NULL, NULL, yymsp[0].minor.yy259, status->scanner_state);
}
// 5078 "parser.c"
        break;
      case 188:
// 1799 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_let_assignment("object-property", yymsp[-1].minor.yy259, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy259, status->scanner_state);
  yy_destructor(38,&yymsp[-3].minor);
}
// 5086 "parser.c"
        break;
      case 189:
// 1804 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_let_assignment("variable-dynamic-object-property", yymsp[-1].minor.yy259, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, yymsp[0].minor.yy259, status->scanner_state);
  yy_destructor(38,&yymsp[-5].minor);
  yy_destructor(48,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
}
// 5096 "parser.c"
        break;
      case 190:
// 1809 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_let_assignment("string-dynamic-object-property", yymsp[-1].minor.yy259, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, yymsp[0].minor.yy259, status->scanner_state);
  yy_destructor(38,&yymsp[-5].minor);
  yy_destructor(48,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
}
// 5106 "parser.c"
        break;
      case 191:
// 1814 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_let_assignment("object-property-append", yymsp[-1].minor.yy259, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy259, status->scanner_state);
  yy_destructor(38,&yymsp[-5].minor);
  yy_destructor(37,&yymsp[-3].minor);
  yy_destructor(58,&yymsp[-2].minor);
}
// 5116 "parser.c"
        break;
      case 192:
// 1819 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_let_assignment("object-property-array-index", yymsp[-1].minor.yy259, yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-2].minor.yy259, yymsp[0].minor.yy259, status->scanner_state);
  yy_destructor(38,&yymsp[-4].minor);
}
// 5124 "parser.c"
        break;
      case 193:
// 1824 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_let_assignment("static-property", yymsp[-1].minor.yy259, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy259, status->scanner_state);
  yy_destructor(91,&yymsp[-3].minor);
}
// 5132 "parser.c"
        break;
      case 194:
// 1829 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_let_assignment("static-property-append", yymsp[-1].minor.yy259, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy259, status->scanner_state);
  yy_destructor(91,&yymsp[-5].minor);
  yy_destructor(37,&yymsp[-3].minor);
  yy_destructor(58,&yymsp[-2].minor);
}
// 5142 "parser.c"
        break;
      case 195:
// 1834 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_let_assignment("static-property-array-index", yymsp[-1].minor.yy259, yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-2].minor.yy259, yymsp[0].minor.yy259, status->scanner_state);
  yy_destructor(91,&yymsp[-4].minor);
}
// 5150 "parser.c"
        break;
      case 196:
// 1839 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_let_assignment("variable-append", yymsp[-1].minor.yy259, yymsp[-4].minor.yy0, NULL, NULL, yymsp[0].minor.yy259, status->scanner_state);
  yy_destructor(37,&yymsp[-3].minor);
  yy_destructor(58,&yymsp[-2].minor);
}
// 5159 "parser.c"
        break;
      case 197:
// 1844 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_let_assignment("array-index", yymsp[-1].minor.yy259, yymsp[-3].minor.yy0, NULL, yymsp[-2].minor.yy259, yymsp[0].minor.yy259, status->scanner_state);
}
// 5166 "parser.c"
        break;
      case 200:
// 1856 "parser.lemon"
{
	yygotominor.yy259 = yymsp[-1].minor.yy259;
  yy_destructor(37,&yymsp[-2].minor);
  yy_destructor(58,&yymsp[0].minor);
}
// 5175 "parser.c"
        break;
      case 201:
// 1861 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_let_assignment("object-property-incr", NULL, yymsp[-3].minor.yy0, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(92,&yymsp[0].minor);
}
// 5184 "parser.c"
        break;
      case 202:
// 1866 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_let_assignment("object-property-decr", NULL, yymsp[-3].minor.yy0, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(93,&yymsp[0].minor);
}
// 5193 "parser.c"
        break;
      case 203:
// 1871 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_let_assignment("incr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(92,&yymsp[0].minor);
}
// 5201 "parser.c"
        break;
      case 204:
// 1876 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_let_assignment("decr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(93,&yymsp[0].minor);
}
// 5209 "parser.c"
        break;
      case 206:
// 1884 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_echo_statement(yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(94,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 5218 "parser.c"
        break;
      case 209:
// 1896 "parser.lemon"
{
	yygotominor.yy259 = yymsp[0].minor.yy259;;
}
// 5225 "parser.c"
        break;
      case 210:
// 1901 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_mcall_statement(yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(41,&yymsp[0].minor);
}
// 5233 "parser.c"
        break;
      case 211:
// 1906 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_fcall_statement(yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(41,&yymsp[0].minor);
}
// 5241 "parser.c"
        break;
      case 212:
// 1911 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_scall_statement(yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(41,&yymsp[0].minor);
}
// 5249 "parser.c"
        break;
      case 213:
// 1916 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_fetch_statement(yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(41,&yymsp[0].minor);
}
// 5257 "parser.c"
        break;
      case 214:
// 1921 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_return_statement(yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(95,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 5266 "parser.c"
        break;
      case 215:
// 1926 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_return_statement(NULL, status->scanner_state);
  yy_destructor(95,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 5275 "parser.c"
        break;
      case 216:
// 1931 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_unset_statement(yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(96,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 5284 "parser.c"
        break;
      case 217:
// 1936 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_throw_exception(yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(97,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 5293 "parser.c"
        break;
      case 218:
// 1940 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_declare_statement(XX_T_TYPE_INTEGER, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(59,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 5302 "parser.c"
        break;
      case 219:
// 1944 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_declare_statement(XX_T_TYPE_UINTEGER, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(60,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 5311 "parser.c"
        break;
      case 220:
// 1948 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_declare_statement(XX_T_TYPE_CHAR, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(63,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 5320 "parser.c"
        break;
      case 221:
// 1952 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_declare_statement(XX_T_TYPE_UCHAR, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(64,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 5329 "parser.c"
        break;
      case 222:
// 1956 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_declare_statement(XX_T_TYPE_LONG, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(61,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 5338 "parser.c"
        break;
      case 223:
// 1960 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_declare_statement(XX_T_TYPE_ULONG, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(62,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 5347 "parser.c"
        break;
      case 224:
// 1964 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_declare_statement(XX_T_TYPE_DOUBLE, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(65,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 5356 "parser.c"
        break;
      case 225:
// 1968 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_declare_statement(XX_T_TYPE_STRING, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(67,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 5365 "parser.c"
        break;
      case 226:
// 1972 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_declare_statement(XX_T_TYPE_BOOL, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(66,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 5374 "parser.c"
        break;
      case 227:
// 1976 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_declare_statement(XX_T_TYPE_VAR, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(69,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 5383 "parser.c"
        break;
      case 230:
// 1988 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_declare_variable(yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 5390 "parser.c"
        break;
      case 231:
// 1992 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_declare_variable(yymsp[-2].minor.yy0, yymsp[0].minor.yy259, status->scanner_state);
  yy_destructor(51,&yymsp[-1].minor);
}
// 5398 "parser.c"
        break;
      case 233:
// 2000 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("not", yymsp[0].minor.yy259, NULL, NULL, status->scanner_state);
  yy_destructor(35,&yymsp[-1].minor);
}
// 5406 "parser.c"
        break;
      case 234:
// 2004 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("isset", yymsp[0].minor.yy259, NULL, NULL, status->scanner_state);
  yy_destructor(31,&yymsp[-1].minor);
}
// 5414 "parser.c"
        break;
      case 235:
// 2008 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("require", yymsp[0].minor.yy259, NULL, NULL, status->scanner_state);
  yy_destructor(7,&yymsp[-1].minor);
}
// 5422 "parser.c"
        break;
      case 236:
// 2012 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("clone", yymsp[0].minor.yy259, NULL, NULL, status->scanner_state);
  yy_destructor(10,&yymsp[-1].minor);
}
// 5430 "parser.c"
        break;
      case 237:
// 2016 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("empty", yymsp[0].minor.yy259, NULL, NULL, status->scanner_state);
  yy_destructor(11,&yymsp[-1].minor);
}
// 5438 "parser.c"
        break;
      case 238:
// 2020 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("likely", yymsp[0].minor.yy259, NULL, NULL, status->scanner_state);
  yy_destructor(8,&yymsp[-1].minor);
}
// 5446 "parser.c"
        break;
      case 239:
// 2024 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("unlikely", yymsp[0].minor.yy259, NULL, NULL, status->scanner_state);
  yy_destructor(9,&yymsp[-1].minor);
}
// 5454 "parser.c"
        break;
      case 240:
// 2028 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("equals", yymsp[-2].minor.yy259, yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(17,&yymsp[-1].minor);
}
// 5462 "parser.c"
        break;
      case 241:
// 2032 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("not-equals", yymsp[-2].minor.yy259, yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(24,&yymsp[-1].minor);
}
// 5470 "parser.c"
        break;
      case 242:
// 2036 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("identical", yymsp[-2].minor.yy259, yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(18,&yymsp[-1].minor);
}
// 5478 "parser.c"
        break;
      case 243:
// 2040 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("not-identical", yymsp[-2].minor.yy259, yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(23,&yymsp[-1].minor);
}
// 5486 "parser.c"
        break;
      case 244:
// 2044 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("less", yymsp[-2].minor.yy259, yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(19,&yymsp[-1].minor);
}
// 5494 "parser.c"
        break;
      case 245:
// 2048 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("greater", yymsp[-2].minor.yy259, yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(20,&yymsp[-1].minor);
}
// 5502 "parser.c"
        break;
      case 246:
// 2052 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("less-equal", yymsp[-2].minor.yy259, yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(21,&yymsp[-1].minor);
}
// 5510 "parser.c"
        break;
      case 247:
// 2056 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("greater-equal", yymsp[-2].minor.yy259, yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(22,&yymsp[-1].minor);
}
// 5518 "parser.c"
        break;
      case 248:
// 2060 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("list", yymsp[-1].minor.yy259, NULL, NULL, status->scanner_state);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5527 "parser.c"
        break;
      case 249:
// 2064 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("cast", yymsp[-2].minor.yy259, yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(55,&yymsp[-3].minor);
  yy_destructor(36,&yymsp[-1].minor);
}
// 5536 "parser.c"
        break;
      case 250:
// 2068 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("type-hint", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(19,&yymsp[-3].minor);
  yy_destructor(20,&yymsp[-1].minor);
}
// 5545 "parser.c"
        break;
      case 251:
// 2072 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("property-access", yymsp[-2].minor.yy259, xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-1].minor);
}
// 5553 "parser.c"
        break;
      case 252:
// 2076 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("property-dynamic-access", yymsp[-4].minor.yy259, xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 5563 "parser.c"
        break;
      case 253:
// 2080 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("property-string-access", yymsp[-4].minor.yy259, xx_ret_literal(XX_T_STRING, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 5573 "parser.c"
        break;
      case 254:
// 2084 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("static-property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(91,&yymsp[-1].minor);
}
// 5581 "parser.c"
        break;
      case 255:
      case 323:
// 2088 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("static-constant-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(91,&yymsp[-1].minor);
}
// 5590 "parser.c"
        break;
      case 256:
// 2097 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("array-access", yymsp[-3].minor.yy259, yymsp[-1].minor.yy259, NULL, status->scanner_state);
  yy_destructor(37,&yymsp[-2].minor);
  yy_destructor(58,&yymsp[0].minor);
}
// 5599 "parser.c"
        break;
      case 257:
// 2102 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("add", yymsp[-2].minor.yy259, yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(25,&yymsp[-1].minor);
}
// 5607 "parser.c"
        break;
      case 258:
// 2107 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("sub", yymsp[-2].minor.yy259, yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(26,&yymsp[-1].minor);
}
// 5615 "parser.c"
        break;
      case 259:
// 2112 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("mul", yymsp[-2].minor.yy259, yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-1].minor);
}
// 5623 "parser.c"
        break;
      case 260:
// 2117 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("div", yymsp[-2].minor.yy259, yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(29,&yymsp[-1].minor);
}
// 5631 "parser.c"
        break;
      case 261:
// 2122 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("mod", yymsp[-2].minor.yy259, yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(30,&yymsp[-1].minor);
}
// 5639 "parser.c"
        break;
      case 262:
// 2127 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("concat", yymsp[-2].minor.yy259, yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(27,&yymsp[-1].minor);
}
// 5647 "parser.c"
        break;
      case 263:
// 2132 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("and", yymsp[-2].minor.yy259, yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(13,&yymsp[-1].minor);
}
// 5655 "parser.c"
        break;
      case 264:
// 2137 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("or", yymsp[-2].minor.yy259, yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(14,&yymsp[-1].minor);
}
// 5663 "parser.c"
        break;
      case 265:
// 2142 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("instanceof", yymsp[-2].minor.yy259, yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(12,&yymsp[-1].minor);
}
// 5671 "parser.c"
        break;
      case 266:
// 2147 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("fetch", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy259, NULL, status->scanner_state);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(6,&yymsp[-1].minor);
}
// 5680 "parser.c"
        break;
      case 268:
// 2157 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("typeof", yymsp[0].minor.yy259, NULL, NULL, status->scanner_state);
  yy_destructor(33,&yymsp[-1].minor);
}
// 5688 "parser.c"
        break;
      case 270:
      case 314:
      case 316:
// 2167 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_literal(XX_T_INTEGER, yymsp[0].minor.yy0, status->scanner_state);
}
// 5697 "parser.c"
        break;
      case 271:
      case 313:
      case 318:
// 2172 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_literal(XX_T_STRING, yymsp[0].minor.yy0, status->scanner_state);
}
// 5706 "parser.c"
        break;
      case 272:
      case 317:
// 2177 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_literal(XX_T_CHAR, yymsp[0].minor.yy0, status->scanner_state);
}
// 5714 "parser.c"
        break;
      case 273:
      case 319:
// 2182 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_literal(XX_T_DOUBLE, yymsp[0].minor.yy0, status->scanner_state);
}
// 5722 "parser.c"
        break;
      case 274:
      case 320:
// 2187 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_literal(XX_T_NULL, NULL, status->scanner_state);
  yy_destructor(101,&yymsp[0].minor);
}
// 5731 "parser.c"
        break;
      case 275:
      case 322:
// 2192 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_literal(XX_T_TRUE, NULL, status->scanner_state);
  yy_destructor(102,&yymsp[0].minor);
}
// 5740 "parser.c"
        break;
      case 276:
      case 321:
// 2197 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_literal(XX_T_FALSE, NULL, status->scanner_state);
  yy_destructor(103,&yymsp[0].minor);
}
// 5749 "parser.c"
        break;
      case 277:
      case 324:
// 2202 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_literal(XX_T_CONSTANT, yymsp[0].minor.yy0, status->scanner_state);
}
// 5757 "parser.c"
        break;
      case 278:
// 2207 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("empty-array", NULL, NULL, NULL, status->scanner_state);
  yy_destructor(37,&yymsp[-1].minor);
  yy_destructor(58,&yymsp[0].minor);
}
// 5766 "parser.c"
        break;
      case 279:
// 2212 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_expr("array", yymsp[-1].minor.yy259, NULL, NULL, status->scanner_state);
  yy_destructor(37,&yymsp[-2].minor);
  yy_destructor(58,&yymsp[0].minor);
}
// 5775 "parser.c"
        break;
      case 280:
// 2217 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_new_instance(0, yymsp[0].minor.yy0, NULL, status->scanner_state);
  yy_destructor(34,&yymsp[-1].minor);
}
// 5783 "parser.c"
        break;
      case 281:
// 2222 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_new_instance(0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(34,&yymsp[-3].minor);
  yy_destructor(55,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5793 "parser.c"
        break;
      case 282:
// 2227 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_new_instance(0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(34,&yymsp[-4].minor);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5803 "parser.c"
        break;
      case 283:
// 2232 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_new_instance(1, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(34,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[0].minor);
}
// 5813 "parser.c"
        break;
      case 284:
// 2237 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_new_instance(1, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(34,&yymsp[-5].minor);
  yy_destructor(48,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(55,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5825 "parser.c"
        break;
      case 285:
// 2242 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_new_instance(1, yymsp[-4].minor.yy0, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(34,&yymsp[-6].minor);
  yy_destructor(48,&yymsp[-5].minor);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5837 "parser.c"
        break;
      case 286:
// 2247 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_fcall(1, yymsp[-3].minor.yy0, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5846 "parser.c"
        break;
      case 287:
// 2252 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_fcall(1, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(55,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5855 "parser.c"
        break;
      case 288:
// 2257 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_fcall(2, yymsp[-4].minor.yy0, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(48,&yymsp[-5].minor);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5866 "parser.c"
        break;
      case 289:
// 2262 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_fcall(2, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(55,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5877 "parser.c"
        break;
      case 290:
// 2267 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_scall(0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(91,&yymsp[-4].minor);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5887 "parser.c"
        break;
      case 291:
// 2272 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_scall(0, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(91,&yymsp[-3].minor);
  yy_destructor(55,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5897 "parser.c"
        break;
      case 292:
// 2277 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_scall(1, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(48,&yymsp[-7].minor);
  yy_destructor(49,&yymsp[-5].minor);
  yy_destructor(91,&yymsp[-4].minor);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5909 "parser.c"
        break;
      case 293:
// 2282 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_scall(1, yymsp[-5].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-6].minor);
  yy_destructor(49,&yymsp[-4].minor);
  yy_destructor(91,&yymsp[-3].minor);
  yy_destructor(55,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5921 "parser.c"
        break;
      case 294:
// 2287 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_scall(1, yymsp[-8].minor.yy0, yymsp[-4].minor.yy0, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(48,&yymsp[-9].minor);
  yy_destructor(49,&yymsp[-7].minor);
  yy_destructor(91,&yymsp[-6].minor);
  yy_destructor(48,&yymsp[-5].minor);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5935 "parser.c"
        break;
      case 295:
// 2292 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_scall(1, yymsp[-7].minor.yy0, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-8].minor);
  yy_destructor(49,&yymsp[-6].minor);
  yy_destructor(91,&yymsp[-5].minor);
  yy_destructor(48,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(55,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5949 "parser.c"
        break;
      case 296:
// 2297 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_mcall(1, yymsp[-5].minor.yy259, yymsp[-3].minor.yy0, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5959 "parser.c"
        break;
      case 297:
// 2302 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_mcall(1, yymsp[-4].minor.yy259, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(55,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5969 "parser.c"
        break;
      case 298:
// 2307 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_mcall(2, yymsp[-7].minor.yy259, yymsp[-4].minor.yy0, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(38,&yymsp[-6].minor);
  yy_destructor(48,&yymsp[-5].minor);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5981 "parser.c"
        break;
      case 299:
// 2312 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_mcall(2, yymsp[-6].minor.yy259, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-5].minor);
  yy_destructor(48,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(55,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 5993 "parser.c"
        break;
      case 300:
// 2317 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_mcall(3, yymsp[-7].minor.yy259, yymsp[-4].minor.yy0, yymsp[-1].minor.yy259, status->scanner_state);
  yy_destructor(38,&yymsp[-6].minor);
  yy_destructor(48,&yymsp[-5].minor);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 6005 "parser.c"
        break;
      case 301:
// 2322 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_mcall(3, yymsp[-6].minor.yy259, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-5].minor);
  yy_destructor(48,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(55,&yymsp[-1].minor);
  yy_destructor(36,&yymsp[0].minor);
}
// 6017 "parser.c"
        break;
      case 310:
// 2361 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_array_item(yymsp[-2].minor.yy259, yymsp[0].minor.yy259, status->scanner_state);
  yy_destructor(76,&yymsp[-1].minor);
}
// 6025 "parser.c"
        break;
      case 311:
// 2365 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_array_item(NULL, yymsp[0].minor.yy259, status->scanner_state);
}
// 6032 "parser.c"
        break;
      case 326:
// 2425 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_comment(yymsp[0].minor.yy0, status->scanner_state);
}
// 6039 "parser.c"
        break;
      case 327:
// 2429 "parser.lemon"
{
	yygotominor.yy259 = xx_ret_cblock(yymsp[0].minor.yy0, status->scanner_state);
}
// 6046 "parser.c"
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
// 987 "parser.lemon"


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

// 6122 "parser.c"
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
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013 Zephir Team and contributors                          |
 +--------------------------------------------------------------------------+
 | This source file is subject the MIT license, that is bundled with        |
 | this package in the file LICENSE, and is available through the           |
 | world-wide-web at the following url:                                     |
 | http://zephir-lang.com/license.html                                      |
 |                                                                          |
 | If you did not receive a copy of the MIT license and are unable          |
 | to obtain it through the world-wide-web, please send a note to           |
 | license@zephir-lang.com so we can mail you a copy immediately.           |
 +--------------------------------------------------------------------------+
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
			case XX_T_INTERFACE:
				xx_(xx_parser, XX_INTERFACE, NULL, parser_status);
				break;
			case XX_T_EXTENDS:
				xx_(xx_parser, XX_EXTENDS, NULL, parser_status);
				break;
			case XX_T_IMPLEMENTS:
				xx_(xx_parser, XX_IMPLEMENTS, NULL, parser_status);
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
			case XX_T_REQUIRE:
				xx_(xx_parser, XX_REQUIRE, NULL, parser_status);
				break;
			case XX_T_CLONE:
				xx_(xx_parser, XX_CLONE, NULL, parser_status);
				break;
			case XX_T_EMPTY:
				xx_(xx_parser, XX_EMPTY, NULL, parser_status);
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
			case XX_T_CBLOCK:
				xx_parse_with_token(xx_parser, XX_T_CBLOCK, XX_CBLOCK, &token, parser_status);
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
			case XX_T_BITWISE_AND:
				xx_(xx_parser, XX_BITWISE_AND, NULL, parser_status);
				break;
			case XX_T_BITWISE_OR:
				xx_(xx_parser, XX_BITWISE_OR, NULL, parser_status);
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

			case XX_T_VOID:
				xx_(xx_parser, XX_VOID, NULL, parser_status);
				break;
			case XX_T_LIKELY:
				xx_(xx_parser, XX_LIKELY, NULL, parser_status);
				break;
			case XX_T_UNLIKELY:
				xx_(xx_parser, XX_UNLIKELY, NULL, parser_status);
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
