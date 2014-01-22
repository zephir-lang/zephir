/** The author disclaims copyright to this source code.
*/
/* First off, code is include which follows the "include" declaration
** in the input file. */
#include <stdio.h>
// 52 "parser.lemon"


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

static json_object *xx_ret_require_statement(json_object *expr, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("require"));
	json_object_object_add(ret, "expr", expr);

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


// 957 "parser.c"
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
#define YYNOCODE 196
#define YYACTIONTYPE unsigned short int
#define xx_TOKENTYPE xx_parser_token*
typedef union {
  xx_TOKENTYPE yy0;
  json_object* yy25;
  int yy391;
} YYMINORTYPE;
#define YYSTACKDEPTH 100
#define xx_ARG_SDECL xx_parser_status *status;
#define xx_ARG_PDECL ,xx_parser_status *status
#define xx_ARG_FETCH xx_parser_status *status = yypParser->status
#define xx_ARG_STORE yypParser->status = status
#define YYNSTATE 699
#define YYNRULE 339
#define YYERRORSYMBOL 110
#define YYERRSYMDT yy391
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
 /*     0 */   502,  388,  244,  247, 1039,    1,    2,  698,    4,    5,
 /*    10 */     6,    7,    8,  586,  298,   34,   58,   37,   39,   40,
 /*    20 */    82,  149,  426,   78,  472,  445,  238,  241,  232,  306,
 /*    30 */   311,  325,  229,  147,  321,  677,   81,  302,  188,  260,
 /*    40 */   262,  270,  264,  266,  268,  332,  610,  548,   84,   92,
 /*    50 */   320,   67,  292,  257,  253,  597,  511,  520,  529,  532,
 /*    60 */   523,  526,  535,  541,  538,  593,  544,  189,  191,  193,
 /*    70 */   651,  202,  257,  253,   73,  213,  217,  222,  406,  646,
 /*    80 */   652,  413,   71,   72,   74,   75,   77,   76,  314,  514,
 /*    90 */   131,  304,  483,  498,  505,  508,  313,  315,  316,  317,
 /*   100 */   318,  319,  549,  502,  382,  244,  247,  151,  539,  519,
 /*   110 */   153,  667,  673,  156,  672,  657,  619,  298,  479,  155,
 /*   120 */   139,  633,   84,   92,  124,   98,  124,  615,  540,  238,
 /*   130 */   241,  232,  306,  311,  325,  229,  459,  321,  465,  445,
 /*   140 */   302,  188,  631,  154,  516,  635,  657,   25,  332,  162,
 /*   150 */   548,  139,  633,  320,   24,  292,  324,  124,  124,  511,
 /*   160 */   520,  529,  532,  523,  526,  535,  541,  538,  416,  544,
 /*   170 */   189,  191,  193,  433,  202,  659,  419, 1021,  213,  217,
 /*   180 */   222,  406,  645,  591,  413,  660,  418,  419,  557,   84,
 /*   190 */    92,  314,   98,  303,  477,  483,  498,  505,  508,  313,
 /*   200 */   315,  316,  317,  318,  319,  549,  502,  415,  244,  247,
 /*   210 */   420,  421,  422,  423,  424,  425,   36,   37,   39,   40,
 /*   220 */   298,  420,  421,  422,  423,  424,  425,  414,  482,  446,
 /*   230 */   447,  207,  238,  241,  232,  306,  311,  325,  229,  431,
 /*   240 */   321,  440,  445,  302,  188,    3,    4,    5,    6,    7,
 /*   250 */     8,  332,  616,  548,  556,   79,  320,  209,  292,  574,
 /*   260 */   536,  519,  511,  520,  529,  532,  523,  526,  535,  541,
 /*   270 */   538,  514,  544,  189,  191,  193,  444,  202,  639,  533,
 /*   280 */   519,  213,  217,  222,  406,  645,   91,  413,  640,  254,
 /*   290 */   419,  143,  295,  678,  314,   43,  149,  386,  483,  498,
 /*   300 */   505,  508,  313,  315,  316,  317,  318,  319,  549,  502,
 /*   310 */   537,  244,  247,   31,   44,   34,   58,   37,   39,   40,
 /*   320 */   293,   21,  205,  298,  420,  421,  422,  423,  424,  425,
 /*   330 */   339,  341,  340,  310,  408,  238,  241,  232,  306,  311,
 /*   340 */   325,  229,  674,  321,  530,  519,  302,  188,  209,   66,
 /*   350 */   574,  264,  266,  268,  332,  630,  548,  514,  331,  320,
 /*   360 */   514,  292,  257,  253,   80,  511,  520,  529,  532,  523,
 /*   370 */   526,  535,  541,  538,  642,  544,  189,  191,  193,  461,
 /*   380 */   202,  645,  514,  592,  213,  217,  222,  406,  149,  685,
 /*   390 */   413,  688,  397,  419,  149,   30,  534,  314,   59,  531,
 /*   400 */   387,  483,  498,  505,  508,  313,  315,  316,  317,  318,
 /*   410 */   319,  549,  502,  109,  244,  247,  527,  519,  524,  519,
 /*   420 */   227,  528,  114,  405,  124,  117,  298,  420,  421,  422,
 /*   430 */   423,  424,  425,  339,  341,  340,  310,  604,  238,  241,
 /*   440 */   232,  306,  311,  325,  229,  514,  321,  118,  124,  302,
 /*   450 */   188,  393,  632,   84,   92,  156,   98,  332,  612,  548,
 /*   460 */   125,  633,  320,  676,  292,  331,  124, 1022,  511,  520,
 /*   470 */   529,  532,  523,  526,  535,  541,  538,  695,  544,  189,
 /*   480 */   191,  193,  149,  202,  525,  256,  326,  213,  217,  222,
 /*   490 */   406,  521,  519,  413,  357,  339,  341,  340,  310,  391,
 /*   500 */   314,  514,  362,  342,  483,  498,  505,  508,  313,  315,
 /*   510 */   316,  317,  318,  319,  549,  502,  514,  244,  247,  121,
 /*   520 */   256,   34,   58,   37,   39,   40,  613,  512,  519,  298,
 /*   530 */   339,  341,  340,  310,  331,   66,  609,  346,  342,  372,
 /*   540 */   513,  238,  241,  232,  306,  311,  325,  229,  625,  321,
 /*   550 */   335,  331,  302,  188,  689,  522,  677,   23,  621,   25,
 /*   560 */   332,  567,  548,  486,  125,  320,  126,  292,  384,  620,
 /*   570 */   124,  511,  520,  529,  532,  523,  526,  535,  541,  538,
 /*   580 */    90,  544,  189,  191,  193,  330,  202,  343,  256,   15,
 /*   590 */   213,  217,  222,  406,   18,  371,  413,   66,  339,  341,
 /*   600 */   340,  310,  485,  314,  331,  390,  342,  483,  498,  505,
 /*   610 */   508,  313,  315,  316,  317,  318,  319,  549,  502,  331,
 /*   620 */   244,  247,    9,  146,  256,   12,  149,  140,  331,  681,
 /*   630 */   691,  119,  298,  548,  339,  341,  340,  310,  380,  647,
 /*   640 */   654,  396,  342,  430,  238,  241,  232,  306,  311,  325,
 /*   650 */   229,  448,  321,  365,  120,  302,  188,  123,  669,  585,
 /*   660 */   635,  657,  337,  332,  583,  548,   88,  633,  320,  161,
 /*   670 */   292,   85,  124,  651,  511,  520,  529,  532,  523,  526,
 /*   680 */   535,  541,  538,  652,  544,  189,  191,  193,  549,  202,
 /*   690 */   214,  256,  344,  213,  217,  222,  406,  206,  577,  413,
 /*   700 */   349,  339,  341,  340,  310,  675,  314,  331,  336,  342,
 /*   710 */   483,  498,  505,  508,  313,  315,  316,  317,  318,  319,
 /*   720 */   549,  502,  649,  244,  247,  626,  256,   34,   58,   37,
 /*   730 */    39,   40,  679,  662,  677,  298,  339,  341,  340,  310,
 /*   740 */   645,  347,  331,  364,  342,  637,  333,  238,  241,  232,
 /*   750 */   306,  311,  325,  229,   60,  321,  124,   66,  302,  188,
 /*   760 */    84,   92,  466,   98,  324,  443,  332,  594,  548,  648,
 /*   770 */   375,  320,  441,  292,  331,  443,  354,  511,  520,  529,
 /*   780 */   532,  523,  526,  535,  541,  538,  449,  544,  189,  191,
 /*   790 */   193,   61,  202,  305,  256,  142,  213,  217,  222,  406,
 /*   800 */   473,  303,  413,  443,  339,  341,  340,  310,  363,  314,
 /*   810 */   514,  379,  342,  483,  498,  505,  508,  313,  315,  316,
 /*   820 */   317,  318,  319,  549,  502,  514,  244,  247,  614,  256,
 /*   830 */    34,   58,   37,   39,   40,  211,  334,  453,  298,  339,
 /*   840 */   341,  340,  310,  358,  542,  519,  353,  342,  438,  543,
 /*   850 */   238,  241,  232,  306,  311,  325,  229,  210,  321,  545,
 /*   860 */   519,  302,  188,   62,  546,  680,   65,  478,   38,  332,
 /*   870 */   584,  548,  627,  690,  320,  137,  292,  136,  296,  198,
 /*   880 */   511,  520,  529,  532,  523,  526,  535,  541,  538,   95,
 /*   890 */   544,  189,  191,  193,  145,  202,  359,  256,   14,  213,
 /*   900 */   217,  222,  406,  435,  458,  413,  308,  339,  341,  340,
 /*   910 */   310,  135,  314,   94,  329,  342,  483,  498,  505,  508,
 /*   920 */   313,  315,  316,  317,  318,  319,  549,  502,  497,  244,
 /*   930 */   247,  115,  256,   34,   58,   37,   39,   40,   97,  699,
 /*   940 */   122,  298,  339,  341,  340,  310,   16,  360, 1023,  383,
 /*   950 */   342,   99,  307,  238,  241,  232,  306,  311,  325,  229,
 /*   960 */   655,  321,  495,   83,  302,  188,  687,  641,  300,  255,
 /*   970 */   643,  493,  332,  555,  548,  148,  644,  320,   11,  292,
 /*   980 */   749,  455,   89,  511,  520,  529,  532,  523,  526,  535,
 /*   990 */   541,  538,   86,  544,  189,  191,  193,  750,  202,  345,
 /*  1000 */   683,  488,  213,  217,  222,  406,  650,   42,  413,  484,
 /*  1010 */   489,  339,  341,  340,  310,  314,  653,  450,  491,  483,
 /*  1020 */   498,  505,  508,  313,  315,  316,  317,  318,  319,  549,
 /*  1030 */   502,  204,  244,  247,  190,  668,  696,  671,  129,  672,
 /*  1040 */   657,  656,  350,   87,  298,  125,  633,  351,   27,  199,
 /*  1050 */   159,  124,  665,  192,   17,  152,  238,  241,  232,  306,
 /*  1060 */   311,  325,  229,  575,  321,  352,  228,  302,  188,  407,
 /*  1070 */   682,   32,   69,  661,  130,  332,  578,  548,  663,  581,
 /*  1080 */   320,   13,  292,   64,  580,  208,  511,  520,  529,  532,
 /*  1090 */   523,  526,  535,  541,  538,  694,  544,  189,  191,  193,
 /*  1100 */   664,  202,  158,  439,  686,  213,  217,  222,  406,  587,
 /*  1110 */   437,  413,  226,  339,  341,  340,  310,  111,  314,  451,
 /*  1120 */   454,   35,  483,  498,  505,  508,  313,  315,  316,  317,
 /*  1130 */   318,  319,  549,  502,  112,  244,  247,   20,  666,   22,
 /*  1140 */   141,  138,   10,   25,  463,  571,  223,  298,  139,   93,
 /*  1150 */   126,  132,  144,   43,  124,  684,  395,  693,  134,  238,
 /*  1160 */   241,  232,  306,  311,  325,  229,  697,  321,   18,  599,
 /*  1170 */   302,  188,   70,  394,  219,  417,  558,  480,  332,  590,
 /*  1180 */   548,  299,  518,  320,   28,  292,   45,  606,  327,  511,
 /*  1190 */   520,  529,  532,  523,  526,  535,  541,  538,  607,  544,
 /*  1200 */   189,  191,  193,  570,  202,  517,  405,  470,  213,  217,
 /*  1210 */   222,  406,  515,  569,  413,  389,  339,  341,  340,  310,
 /*  1220 */   692,  314,   46,  116,  536,  483,  498,  505,  508,  313,
 /*  1230 */   315,  316,  317,  318,  319,  549,  502,  195,  244,  247,
 /*  1240 */   536,  428,  536,  536,  536,  436,  536,  536,  536,  536,
 /*  1250 */   298,  339,  341,  340,  310,  598,  536,   34,   58,   37,
 /*  1260 */    39,   40,  238,  241,  232,  306,  311,  325,  229,  536,
 /*  1270 */   321,  536,  536,  302,  188,  536,  536,  536,  536,  536,
 /*  1280 */   536,  332,  225,  548,  536,  536,  320,  536,  292,  536,
 /*  1290 */   536,  536,  511,  520,  529,  532,  523,  526,  535,  541,
 /*  1300 */   538,  536,  544,  189,  191,  193,  194,  202,  536,  405,
 /*  1310 */   536,  213,  217,  222,  406,  536,  536,  413,  536,  339,
 /*  1320 */   341,  340,  310,  536,  314,  536,  536,  536,  483,  498,
 /*  1330 */   505,  508,  313,  315,  316,  317,  318,  319,  549,  502,
 /*  1340 */   536,  244,  247,  536,  428,  536,  536,  536,  427,  536,
 /*  1350 */   536,  536,  536,  298,  339,  341,  340,  310,   63,  536,
 /*  1360 */    34,   58,   37,   39,   40,  238,  241,  232,  306,  311,
 /*  1370 */   325,  229,  536,  321,  536,  536,  302,  188,  536,  536,
 /*  1380 */   536,  536,  536,  536,  332,  596,  548,  536,  536,  320,
 /*  1390 */   536,  292,  536,  536,  536,  511,  520,  529,  532,  523,
 /*  1400 */   526,  535,  541,  538,  536,  544,  189,  191,  193,  203,
 /*  1410 */   202,  536,  405,  536,  213,  217,  222,  406,  536,  536,
 /*  1420 */   413,  536,  339,  341,  340,  310,  536,  314,  536,  536,
 /*  1430 */   536,  483,  498,  505,  508,  313,  315,  316,  317,  318,
 /*  1440 */   319,  549,  502,  536,  244,  247,  536,  428,  536,  536,
 /*  1450 */   536,  442,  536,  536,  536,  536,  298,  339,  341,  340,
 /*  1460 */   310,  536,  536,  536,  536,  536,  536,  536,  238,  241,
 /*  1470 */   232,  306,  311,  325,  229,  536,  321,  536,  536,  302,
 /*  1480 */   188,  536,  536,  536,  536,  536,  536,  332,  164,  548,
 /*  1490 */   536,  536,  320,  536,  292,  536,  536,  536,  511,  520,
 /*  1500 */   529,  532,  523,  526,  535,  541,  538,  536,  544,  189,
 /*  1510 */   191,  193,  536,  202,  536,  488,  536,  213,  217,  222,
 /*  1520 */   406,  536,  536,  413,  487,  339,  341,  340,  310,  536,
 /*  1530 */   314,  536,  536,  536,  483,  498,  505,  508,  313,  315,
 /*  1540 */   316,  317,  318,  319,  549,  502,  536,  244,  247,  536,
 /*  1550 */   428,  536,  536,  536,  464,  536,  536,  536,  536,  298,
 /*  1560 */   339,  341,  340,  310,  536,  536,  536,  536,  536,  536,
 /*  1570 */   536,  238,  241,  232,  306,  311,  325,  229,  536,  321,
 /*  1580 */   536,  536,  302,  188,  536,  536,  536,  536,  536,  536,
 /*  1590 */   332,  588,  548,  536,  536,  320,  536,  292,  536,  536,
 /*  1600 */   536,  511,  520,  529,  532,  523,  526,  535,  541,  538,
 /*  1610 */   536,  544,  189,  191,  193,  536,  202,  536,  256,  536,
 /*  1620 */   213,  217,  222,  406,  536,  536,  413,  536,  339,  341,
 /*  1630 */   340,  310,  536,  314,  536,  536,  356,  483,  498,  505,
 /*  1640 */   508,  313,  315,  316,  317,  318,  319,  549,  502,  536,
 /*  1650 */   244,  247,  536,  428,  536,  536,  536,  481,  536,  536,
 /*  1660 */   536,  536,  298,  339,  341,  340,  310,  536,  536,  536,
 /*  1670 */   536,  536,  536,  536,  238,  241,  232,  306,  311,  325,
 /*  1680 */   229,  536,  321,  536,  536,  302,  188,  536,  536,  536,
 /*  1690 */   536,  536,  536,  332,  197,  548,  536,  536,  320,  536,
 /*  1700 */   292,  536,  536,  536,  511,  520,  529,  532,  523,  526,
 /*  1710 */   535,  541,  538,  536,  544,  189,  191,  193,  218,  202,
 /*  1720 */   536,  405,  536,  213,  217,  222,  406,  536,  536,  413,
 /*  1730 */   536,  339,  341,  340,  310,  536,  314,  536,  536,  536,
 /*  1740 */   483,  498,  505,  508,  313,  315,  316,  317,  318,  319,
 /*  1750 */   549,  502,  536,  244,  247,  536,  428,  536,  536,  536,
 /*  1760 */   460,  536,  536,  536,  536,  298,  339,  341,  340,  310,
 /*  1770 */   536,  536,  536,  536,  536,  536,  536,  238,  241,  232,
 /*  1780 */   306,  311,  325,  229,  536,  321,  536,  536,  302,  188,
 /*  1790 */   536,  536,  536,  536,  536,  536,  332,  628,  548,  536,
 /*  1800 */   536,  320,  536,  292,  536,  536,  536,  511,  520,  529,
 /*  1810 */   532,  523,  526,  535,  541,  538,  536,  544,  189,  191,
 /*  1820 */   193,  536,  202,  536,  269,  536,  213,  217,  222,  406,
 /*  1830 */   536,  536,  413,  536,  339,  341,  340,  310,  536,  314,
 /*  1840 */   536,  536,  536,  483,  498,  505,  508,  313,  315,  316,
 /*  1850 */   317,  318,  319,  549,  502,  536,  244,  247,  536,  428,
 /*  1860 */   536,  536,  536,  432,  536,  536,  536,  536,  298,  339,
 /*  1870 */   341,  340,  310,  536,  536,  536,  536,  536,  536,  536,
 /*  1880 */   238,  241,  232,  306,  311,  325,  229,  536,  321,  536,
 /*  1890 */   536,  302,  188,  536,  536,  536,  536,  536,  536,  332,
 /*  1900 */   562,  548,  536,  536,  320,  536,  292,  536,  536,  536,
 /*  1910 */   511,  520,  529,  532,  523,  526,  535,  541,  538,  536,
 /*  1920 */   544,  189,  191,  193,  536,  202,  536,  258,  536,  213,
 /*  1930 */   217,  222,  406,  536,  536,  413,  536,  339,  341,  340,
 /*  1940 */   310,  536,  314,  536,  536,  536,  483,  498,  505,  508,
 /*  1950 */   313,  315,  316,  317,  318,  319,  549,  502,  536,  244,
 /*  1960 */   247,  536,  428,  536,  536,  536,  456,  536,  536,  536,
 /*  1970 */   536,  298,  339,  341,  340,  310,  536,  536,  536,  536,
 /*  1980 */   536,  536,  536,  238,  241,  232,  306,  311,  325,  229,
 /*  1990 */   536,  321,  536,  536,  302,  188,  536,  536,  536,  536,
 /*  2000 */   536,  536,  332,  221,  548,  536,  536,  320,  536,  292,
 /*  2010 */   536,  536,  536,  511,  520,  529,  532,  523,  526,  535,
 /*  2020 */   541,  538,  536,  544,  189,  191,  193,  536,  202,  536,
 /*  2030 */   242,  536,  213,  217,  222,  406,  536,  536,  413,  536,
 /*  2040 */   339,  341,  340,  310,  536,  314,  536,  536,  536,  483,
 /*  2050 */   498,  505,  508,  313,  315,  316,  317,  318,  319,  549,
 /*  2060 */   502,  536,  244,  247,  536,  428,  536,  536,  536,  467,
 /*  2070 */   536,  536,  536,  536,  298,  339,  341,  340,  310,  536,
 /*  2080 */   536,  536,  536,  536,  536,  536,  238,  241,  232,  306,
 /*  2090 */   311,  325,  229,  536,  321,  536,  536,  302,  188,  536,
 /*  2100 */   536,  536,  536,  536,  536,  332,  624,  548,  536,  536,
 /*  2110 */   320,  536,  292,  536,  536,  536,  511,  520,  529,  532,
 /*  2120 */   523,  526,  535,  541,  538,  536,  544,  189,  191,  193,
 /*  2130 */   536,  202,  536,  261,  536,  213,  217,  222,  406,  536,
 /*  2140 */   536,  413,  536,  339,  341,  340,  310,  536,  314,  536,
 /*  2150 */   536,  536,  483,  498,  505,  508,  313,  315,  316,  317,
 /*  2160 */   318,  319,  549,  502,  536,  244,  247,  536,  428,  536,
 /*  2170 */   536,  536,  452,  536,  536,  536,  536,  298,  339,  341,
 /*  2180 */   340,  310,  536,  536,  536,  536,  536,  536,  536,  238,
 /*  2190 */   241,  232,  306,  311,  325,  229,  536,  321,  536,  536,
 /*  2200 */   302,  188,  536,  536,  536,  536,  536,  536,  332,  600,
 /*  2210 */   548,  536,  536,  320,  536,  292,  536,  536,  536,  511,
 /*  2220 */   520,  529,  532,  523,  526,  535,  541,  538,  536,  544,
 /*  2230 */   189,  191,  193,  536,  202,  536,  263,  536,  213,  217,
 /*  2240 */   222,  406,  536,  536,  413,  536,  339,  341,  340,  310,
 /*  2250 */   536,  314,  536,  536,  536,  483,  498,  505,  508,  313,
 /*  2260 */   315,  316,  317,  318,  319,  549,  502,  536,  244,  247,
 /*  2270 */   536,  428,  536,  536,  536,  471,  536,  536,  536,  536,
 /*  2280 */   298,  339,  341,  340,  310,  536,  536,  536,  536,  536,
 /*  2290 */   536,  536,  238,  241,  232,  306,  311,  325,  229,  536,
 /*  2300 */   321,  536,  536,  302,  188,  536,  536,  536,  536,  536,
 /*  2310 */   536,  332,  622,  548,  536,  536,  320,  536,  292,  536,
 /*  2320 */   536,  536,  511,  520,  529,  532,  523,  526,  535,  541,
 /*  2330 */   538,  536,  544,  189,  191,  193,  536,  202,  536,  265,
 /*  2340 */   536,  213,  217,  222,  406,  536,  536,  413,  536,  339,
 /*  2350 */   341,  340,  310,  536,  314,  536,  536,  536,  483,  498,
 /*  2360 */   505,  508,  313,  315,  316,  317,  318,  319,  549,  502,
 /*  2370 */   536,  244,  247,  536,  428,  536,  536,  536,  474,  536,
 /*  2380 */   536,  536,  536,  298,  339,  341,  340,  310,  536,  536,
 /*  2390 */   536,  536,  536,  536,  536,  238,  241,  232,  306,  311,
 /*  2400 */   325,  229,  536,  321,  536,  536,  302,  188,  536,  536,
 /*  2410 */   536,  536,  536,  536,  332,  572,  548,  536,  536,  320,
 /*  2420 */   536,  292,  536,  536,  536,  511,  520,  529,  532,  523,
 /*  2430 */   526,  535,  541,  538,  536,  544,  189,  191,  193,  536,
 /*  2440 */   202,  536,  267,  536,  213,  217,  222,  406,  536,  536,
 /*  2450 */   413,  536,  339,  341,  340,  310,  536,  314,  536,  536,
 /*  2460 */   536,  483,  498,  505,  508,  313,  315,  316,  317,  318,
 /*  2470 */   319,  549,  502,  536,  244,  247,  536,  271,  536,  536,
 /*  2480 */   536,  536,  536,  536,  536,  536,  298,  339,  341,  340,
 /*  2490 */   310,  536,  536,  536,  536,  536,  536,  536,  238,  241,
 /*  2500 */   232,  306,  311,  325,  229,  536,  321,  536,  536,  302,
 /*  2510 */   188,  536,  536,  536,  536,  536,  536,  332,  602,  548,
 /*  2520 */   536,  536,  320,  536,  292,  536,  536,  536,  511,  520,
 /*  2530 */   529,  532,  523,  526,  535,  541,  538,  536,  544,  189,
 /*  2540 */   191,  193,  536,  202,  536,  273,  536,  213,  217,  222,
 /*  2550 */   406,  536,  536,  413,  536,  339,  341,  340,  310,  536,
 /*  2560 */   314,  536,  536,  536,  483,  498,  505,  508,  313,  315,
 /*  2570 */   316,  317,  318,  319,  549,  502,  536,  244,  247,  536,
 /*  2580 */   499,  536,  536,  536,  536,  536,  536,  536,  536,  298,
 /*  2590 */   339,  341,  340,  310,  536,  536,  536,  536,  536,  536,
 /*  2600 */   536,  238,  241,  232,  306,  311,  325,  229,  536,  321,
 /*  2610 */   536,  536,  302,  188,  536,  536,  536,  536,  536,  536,
 /*  2620 */   332,  568,  548,  536,  536,  320,  536,  292,  536,  536,
 /*  2630 */   536,  511,  520,  529,  532,  523,  526,  535,  541,  538,
 /*  2640 */   536,  544,  189,  191,  193,  536,  202,  536,  275,  536,
 /*  2650 */   213,  217,  222,  406,  536,  536,  413,  536,  339,  341,
 /*  2660 */   340,  310,  536,  314,  536,  536,  536,  483,  498,  505,
 /*  2670 */   508,  313,  315,  316,  317,  318,  319,  549,  502,  536,
 /*  2680 */   244,  247,  536,  503,  536,  536,  536,  536,  536,  536,
 /*  2690 */   536,  536,  298,  339,  341,  340,  310,  536,  536,  536,
 /*  2700 */   536,  536,  536,  536,  238,  241,  232,  306,  311,  325,
 /*  2710 */   229,  536,  321,  536,  536,  302,  188,  536,  536,  536,
 /*  2720 */   536,  536,  536,  332,  579,  548,  536,  536,  320,  536,
 /*  2730 */   292,  536,  536,  536,  511,  520,  529,  532,  523,  526,
 /*  2740 */   535,  541,  538,  536,  544,  189,  191,  193,  536,  202,
 /*  2750 */   536,  401,  536,  213,  217,  222,  406,  536,  536,  413,
 /*  2760 */   536,  339,  341,  340,  310,  536,  314,  536,  536,  536,
 /*  2770 */   483,  498,  505,  508,  313,  315,  316,  317,  318,  319,
 /*  2780 */   549,  502,  536,  244,  247,  536,  277,  536,  536,  536,
 /*  2790 */   536,  536,  536,  536,  536,  298,  339,  341,  340,  310,
 /*  2800 */   536,  536,  536,  536,  536,  536,  536,  238,  241,  232,
 /*  2810 */   306,  311,  325,  229,  536,  321,  536,  536,  302,  188,
 /*  2820 */   536,  536,  536,  536,  536,  536,  332,  201,  548,  536,
 /*  2830 */   536,  320,  536,  292,  536,  536,  536,  511,  520,  529,
 /*  2840 */   532,  523,  526,  535,  541,  538,  536,  544,  189,  191,
 /*  2850 */   193,  536,  202,  536,  506,  536,  213,  217,  222,  406,
 /*  2860 */   536,  536,  413,  536,  339,  341,  340,  310,  536,  314,
 /*  2870 */   536,  536,  536,  483,  498,  505,  508,  313,  315,  316,
 /*  2880 */   317,  318,  319,  549,  502,  536,  244,  247,  536,  279,
 /*  2890 */   536,  536,  536,  536,  536,  536,  536,  536,  298,  339,
 /*  2900 */   341,  340,  310,  536,  536,  536,  536,  536,  536,  536,
 /*  2910 */   238,  241,  232,  306,  311,  325,  229,  536,  321,  536,
 /*  2920 */   536,  302,  188,  536,  536,  536,  536,  536,  536,  332,
 /*  2930 */   618,  548,  536,  536,  320,  536,  292,  536,  536,  536,
 /*  2940 */   511,  520,  529,  532,  523,  526,  535,  541,  538,  536,
 /*  2950 */   544,  189,  191,  193,  536,  202,  536,  509,  536,  213,
 /*  2960 */   217,  222,  406,  536,  536,  413,  536,  339,  341,  340,
 /*  2970 */   310,  536,  314,  536,  536,  536,  483,  498,  505,  508,
 /*  2980 */   313,  315,  316,  317,  318,  319,  549,  502,  536,  244,
 /*  2990 */   247,  536,  281,  536,  536,  536,  536,  536,  536,  536,
 /*  3000 */   536,  298,  339,  341,  340,  310,  536,  536,  536,  536,
 /*  3010 */   536,  536,  536,  238,  241,  232,  306,  311,  325,  229,
 /*  3020 */   536,  321,  536,  536,  302,  188,  536,  536,  536,  536,
 /*  3030 */   536,  536,  332,  412,  548,  536,  536,  320,  536,  292,
 /*  3040 */   536,  536,  536,  511,  520,  529,  532,  523,  526,  535,
 /*  3050 */   541,  538,  536,  544,  189,  191,  193,  536,  202,  536,
 /*  3060 */   283,  536,  213,  217,  222,  406,  536,  536,  413,  536,
 /*  3070 */   339,  341,  340,  310,  536,  314,  536,  536,  536,  483,
 /*  3080 */   498,  505,  508,  313,  315,  316,  317,  318,  319,  549,
 /*  3090 */   502,  536,  244,  247,  536,  285,  536,  536,  536,  536,
 /*  3100 */   536,  536,  536,  536,  298,  339,  341,  340,  310,  536,
 /*  3110 */   536,  536,  536,  536,  536,  536,  238,  241,  232,  306,
 /*  3120 */   311,  325,  229,  536,  321,  536,  536,  302,  188,  536,
 /*  3130 */   536,  536,  536,  536,  536,  332,  573,  548,  536,  536,
 /*  3140 */   320,  536,  292,  536,  536,  536,  511,  520,  529,  532,
 /*  3150 */   523,  526,  535,  541,  538,  536,  544,  189,  191,  193,
 /*  3160 */   536,  202,  536,  287,  536,  213,  217,  222,  406,  536,
 /*  3170 */   536,  413,  536,  339,  341,  340,  310,  536,  314,  536,
 /*  3180 */   536,  536,  483,  498,  505,  508,  313,  315,  316,  317,
 /*  3190 */   318,  319,  549,  502,  536,  244,  247,  536,  289,  536,
 /*  3200 */   536,  536,  536,  536,  536,  536,  536,  298,  339,  341,
 /*  3210 */   340,  310,  536,  536,  536,  536,  536,  536,  536,  238,
 /*  3220 */   241,  232,  306,  311,  325,  229,  536,  321,  536,  536,
 /*  3230 */   302,  188,  536,  536,  536,  536,  536,  536,  332,  216,
 /*  3240 */   548,  536,  536,  320,  536,  292,  536,  536,  536,  511,
 /*  3250 */   520,  529,  532,  523,  526,  535,  541,  538,  536,  544,
 /*  3260 */   189,  191,  193,  536,  202,  536,  291,  536,  213,  217,
 /*  3270 */   222,  406,  536,  536,  413,  536,  339,  341,  340,  310,
 /*  3280 */   536,  314,  536,  536,  536,  483,  498,  505,  508,  313,
 /*  3290 */   315,  316,  317,  318,  319,  549,  502,  536,  244,  247,
 /*  3300 */   536,  239,  536,  536,  536,  536,  536,  536,  536,  536,
 /*  3310 */   298,  339,  341,  340,  310,  536,  536,  536,  536,  536,
 /*  3320 */   536,  536,  238,  241,  232,  306,  311,  325,  229,  536,
 /*  3330 */   321,  536,  536,  302,  188,  536,  536,  536,  536,  536,
 /*  3340 */   536,  332,  536,  548,  536,  536,  320,  536,  292,  536,
 /*  3350 */   536,  536,  511,  520,  529,  532,  523,  526,  535,  541,
 /*  3360 */   538,  700,  544,  189,  191,  193,  536,  202,  536,  402,
 /*  3370 */   536,  213,  217,  222,  406,  536,  536,  413,  536,  339,
 /*  3380 */   341,  340,  310,  536,  314,  536,  536,  536,  483,  498,
 /*  3390 */   505,  508,  313,  315,  316,  317,  318,  319,  549,  235,
 /*  3400 */   536,  244,  247,  536,    9,  536,  536,   12,  536,  140,
 /*  3410 */   536,  681,  691,  298,  536,  548,  536,  100,  101,  103,
 /*  3420 */   102,  104,  536,  536,  536,  238,  241,  232,  306,  311,
 /*  3430 */   325,  229,  536,  321,  536,  297,  302,  536,  536,  536,
 /*  3440 */   536,  536,  536,  536,  332,  339,  341,  340,  310,  320,
 /*  3450 */   536,  292,  536,  536,  536,   47,   48,   49,   50,   51,
 /*  3460 */    52,   53,   54,   55,   56,   57,  106,  107,  536,  322,
 /*  3470 */   549,  186,  166,  536,  536,  536,  105,  536,  536,  339,
 /*  3480 */   341,  340,  310,  536,  536,  370,  536,  314,  378,  374,
 /*  3490 */   377,  536,  536,  411,  536,  313,  315,  316,  317,  318,
 /*  3500 */   319,  536,  550,  167,  168,  169,  170,  171,  172,  173,
 /*  3510 */   174,  175,  176,  177,  178,  179,  180,  181,  182,  183,
 /*  3520 */   184,  185,  187,  536,  536,  536,  547,  536,  536,  536,
 /*  3530 */   322,  536,  186,  166,  536,  536,  490,  492,  494,  496,
 /*  3540 */   339,  341,  340,  310,  100,  101,  103,  102,  104,  373,
 /*  3550 */   374,  377,  536,  536,  196,  536,  536,  536,  536,  536,
 /*  3560 */   536,  536,  536,  550,  167,  168,  169,  170,  171,  172,
 /*  3570 */   173,  174,  175,  176,  177,  178,  179,  180,  181,  182,
 /*  3580 */   183,  184,  185,  187,  186,  166,  536,  547,  536,  536,
 /*  3590 */   536,  536,  536,  106,  107,  536,  536,  490,  492,  494,
 /*  3600 */   496,  236,  536,  105,  536,  536,  212,  536,  536,  536,
 /*  3610 */   536,  339,  341,  340,  310,  550,  167,  168,  169,  170,
 /*  3620 */   171,  172,  173,  174,  175,  176,  177,  178,  179,  180,
 /*  3630 */   181,  182,  183,  184,  185,  187,  186,  166,  536,  547,
 /*  3640 */   536,  536,  536,  322,  536,  536,  536,  536,  536,  490,
 /*  3650 */   492,  494,  496,  339,  341,  340,  310,  536,  589,  536,
 /*  3660 */   536,  536,  536,  536,  376,  536,  536,  550,  167,  168,
 /*  3670 */   169,  170,  171,  172,  173,  174,  175,  176,  177,  178,
 /*  3680 */   179,  180,  181,  182,  183,  184,  185,  187,  186,  166,
 /*  3690 */   536,  547,  536,  536,  536,  301,  536,  536,  536,  536,
 /*  3700 */   536,  490,  492,  494,  496,  339,  341,  340,  310,  536,
 /*  3710 */   601,  536,  536,  536,  536,  536,  536,  536,  536,  550,
 /*  3720 */   167,  168,  169,  170,  171,  172,  173,  174,  175,  176,
 /*  3730 */   177,  178,  179,  180,  181,  182,  183,  184,  185,  187,
 /*  3740 */   186,  166,  536,  547,  536,  536,  536,  403,  536,  536,
 /*  3750 */   536,  536,  536,  490,  492,  494,  496,  339,  341,  340,
 /*  3760 */   310,  536,  224,  536,  536,  536,  536,  536,  536,  536,
 /*  3770 */   536,  550,  167,  168,  169,  170,  171,  172,  173,  174,
 /*  3780 */   175,  176,  177,  178,  179,  180,  181,  182,  183,  184,
 /*  3790 */   185,  187,  186,  166,  536,  547,  536,  536,  536,  552,
 /*  3800 */   536,  536,  536,  536,  536,  490,  492,  494,  496,  339,
 /*  3810 */   341,  340,  310,  536,  220,  536,  536,  536,  536,  536,
 /*  3820 */   536,  536,  536,  550,  167,  168,  169,  170,  171,  172,
 /*  3830 */   173,  174,  175,  176,  177,  178,  179,  180,  181,  182,
 /*  3840 */   183,  184,  185,  187,  186,  166,  536,  547,  536,  536,
 /*  3850 */   536,  233,  536,  536,  536,  536,  536,  490,  492,  494,
 /*  3860 */   496,  339,  341,  340,  310,  536,  623,  536,  536,  536,
 /*  3870 */   536,  536,  536,  536,  536,  550,  167,  168,  169,  170,
 /*  3880 */   171,  172,  173,  174,  175,  176,  177,  178,  179,  180,
 /*  3890 */   181,  182,  183,  184,  185,  187,  186,  166,  536,  547,
 /*  3900 */   536,  536,  536,  404,  536,  536,  536,  536,  536,  490,
 /*  3910 */   492,  494,  496,  339,  341,  340,  310,  536,  163,  536,
 /*  3920 */   536,  536,  536,  536,  536,  536,  536,  550,  167,  168,
 /*  3930 */   169,  170,  171,  172,  173,  174,  175,  176,  177,  178,
 /*  3940 */   179,  180,  181,  182,  183,  184,  185,  187,  186,  166,
 /*  3950 */   536,  547,  536,  536,  536,  559,  536,  536,  536,  536,
 /*  3960 */   536,  490,  492,  494,  496,  339,  341,  340,  310,  536,
 /*  3970 */   561,  536,  536,  536,  536,  536,  536,  536,  536,  550,
 /*  3980 */   167,  168,  169,  170,  171,  172,  173,  174,  175,  176,
 /*  3990 */   177,  178,  179,  180,  181,  182,  183,  184,  185,  187,
 /*  4000 */   186,  166,  536,  547,  536,  536,  536,  230,  536,  536,
 /*  4010 */   536,  536,  536,  490,  492,  494,  496,  339,  341,  340,
 /*  4020 */   310,  536,  554,  536,  536,  536,  536,  536,  536,  536,
 /*  4030 */   536,  550,  167,  168,  169,  170,  171,  172,  173,  174,
 /*  4040 */   175,  176,  177,  178,  179,  180,  181,  182,  183,  184,
 /*  4050 */   185,  187,  186,  166,  536,  547,  536,  536,  536,  564,
 /*  4060 */   536,  536,  536,  536,  536,  490,  492,  494,  496,  339,
 /*  4070 */   341,  340,  310,  536,  576,  536,  536,  536,  536,  536,
 /*  4080 */   536,  536,  536,  550,  167,  168,  169,  170,  171,  172,
 /*  4090 */   173,  174,  175,  176,  177,  178,  179,  180,  181,  182,
 /*  4100 */   183,  184,  185,  187,  186,  166,  536,  547,  536,  536,
 /*  4110 */   536,  409,  536,  536,  536,  536,  536,  490,  492,  494,
 /*  4120 */   496,  339,  341,  340,  310,  536,  566,  536,  536,  536,
 /*  4130 */   536,  536,  536,  536,  536,  550,  167,  168,  169,  170,
 /*  4140 */   171,  172,  173,  174,  175,  176,  177,  178,  179,  180,
 /*  4150 */   181,  182,  183,  184,  185,  187,  186,  166,  536,  547,
 /*  4160 */   536,  536,  536,  309,  536,  536,  536,  536,  536,  490,
 /*  4170 */   492,  494,  496,  339,  341,  340,  310,  536,  595,  536,
 /*  4180 */   536,  536,  536,  536,  536,  536,  536,  550,  167,  168,
 /*  4190 */   169,  170,  171,  172,  173,  174,  175,  176,  177,  178,
 /*  4200 */   179,  180,  181,  182,  183,  184,  185,  187,  186,  166,
 /*  4210 */   536,  547,  536,  536,  536,  312,  536,  536,  536,  536,
 /*  4220 */   536,  490,  492,  494,  496,  339,  341,  340,  310,  536,
 /*  4230 */   582,  536,  536,  536,  536,  536,  536,  536,  536,  550,
 /*  4240 */   167,  168,  169,  170,  171,  172,  173,  174,  175,  176,
 /*  4250 */   177,  178,  179,  180,  181,  182,  183,  184,  185,  187,
 /*  4260 */   186,  166,  536,  547,  536,  536,  536,  400,  536,  536,
 /*  4270 */   536,  536,  536,  490,  492,  494,  496,  339,  341,  340,
 /*  4280 */   310,  536,  629,  536,  536,  536,  536,  536,  536,  536,
 /*  4290 */   536,  550,  167,  168,  169,  170,  171,  172,  173,  174,
 /*  4300 */   175,  176,  177,  178,  179,  180,  181,  182,  183,  184,
 /*  4310 */   185,  187,  186,  166,  536,  547,  536,  536,  536,  245,
 /*  4320 */   536,  536,  536,  536,  536,  490,  492,  494,  496,  339,
 /*  4330 */   341,  340,  310,  536,  611,  536,  536,  536,  536,  536,
 /*  4340 */   536,  536,  536,  550,  167,  168,  169,  170,  171,  172,
 /*  4350 */   173,  174,  175,  176,  177,  178,  179,  180,  181,  182,
 /*  4360 */   183,  184,  185,  187,  186,  166,  536,  547,  536,  536,
 /*  4370 */   536,  399,  536,  536,  536,  536,  536,  490,  492,  494,
 /*  4380 */   496,  339,  341,  340,  310,  536,  215,  536,  536,  536,
 /*  4390 */   536,  536,  536,  536,  536,  550,  167,  168,  169,  170,
 /*  4400 */   171,  172,  173,  174,  175,  176,  177,  178,  179,  180,
 /*  4410 */   181,  182,  183,  184,  185,  187,  186,  166,  536,  547,
 /*  4420 */   536,  536,  536,  248,  536,  536,  536,  536,  536,  490,
 /*  4430 */   492,  494,  496,  339,  341,  340,  310,  536,  617,  536,
 /*  4440 */   536,  536,  536,  536,  536,  536,  536,  550,  167,  168,
 /*  4450 */   169,  170,  171,  172,  173,  174,  175,  176,  177,  178,
 /*  4460 */   179,  180,  181,  182,  183,  184,  185,  187,  186,  166,
 /*  4470 */   536,  547,  536,  536,  536,  250,  536,  536,  536,  536,
 /*  4480 */   536,  490,  492,  494,  496,  339,  341,  340,  310,  536,
 /*  4490 */   200,  536,  536,  536,  536,  536,  536,  536,  536,  550,
 /*  4500 */   167,  168,  169,  170,  171,  172,  173,  174,  175,  176,
 /*  4510 */   177,  178,  179,  180,  181,  182,  183,  184,  185,  187,
 /*  4520 */   186,  166,  536,  547,  536,  536,  536,  252,  536,  536,
 /*  4530 */   536,  536,  536,  490,  492,  494,  496,  339,  341,  340,
 /*  4540 */   310,  536,  536,  536,  536,  536,  536,  536,  536,  536,
 /*  4550 */   536,  165,  167,  168,  169,  170,  171,  172,  173,  174,
 /*  4560 */   175,  176,  177,  178,  179,  180,  181,  182,  183,  184,
 /*  4570 */   185,  187,  536,  536,  536,  547,  536,  536,  536,  536,
 /*  4580 */   536,  536,  536,  536,  536,  490,  492,  494,  496,  536,
 /*  4590 */   536,  536,  536,  536,  288,  536,  536,  286,  272,  274,
 /*  4600 */   278,  276,  280,  282,  284,  231,  237,  243,  246,  249,
 /*  4610 */   251,  240,  234,  260,  262,  270,  264,  266,  268,  536,
 /*  4620 */   536,  536,  536,  536,  536,  536,  536,  257,  253,  536,
 /*  4630 */   536,  536,  536,  536,  536,  536,  288,  536,  560,  286,
 /*  4640 */   272,  274,  278,  276,  280,  282,  284,  231,  237,  243,
 /*  4650 */   246,  249,  251,  240,  234,  260,  262,  270,  264,  266,
 /*  4660 */   268,  536,  536,  536,  536,  536,  536,  536,  536,  257,
 /*  4670 */   253,  231,  237,  243,  246,  249,  251,  240,  234,  260,
 /*  4680 */   262,  270,  264,  266,  268,  536,  536,  536,  536,  536,
 /*  4690 */   259,  536,  536,  257,  253,  536,  536,  536,  536,  288,
 /*  4700 */   536,  536,  286,  272,  274,  278,  276,  280,  282,  284,
 /*  4710 */   231,  237,  243,  246,  249,  251,  240,  234,  260,  262,
 /*  4720 */   270,  264,  266,  268,  536,  536,  536,  536,  536,  536,
 /*  4730 */   536,  536,  257,  253,  536,  536,  510,  536,  536,  536,
 /*  4740 */   536,  288,  536,  536,  286,  272,  274,  278,  276,  280,
 /*  4750 */   282,  284,  231,  237,  243,  246,  249,  251,  240,  234,
 /*  4760 */   260,  262,  270,  264,  266,  268,  536,  536,  536,  536,
 /*  4770 */   536,  536,  536,  294,  257,  253,  536,  536,  536,  536,
 /*  4780 */   536,  536,  288,  536,  536,  286,  272,  274,  278,  276,
 /*  4790 */   280,  282,  284,  231,  237,  243,  246,  249,  251,  240,
 /*  4800 */   234,  260,  262,  270,  264,  266,  268,  536,  536,  536,
 /*  4810 */   536,  536,  536,  536,  536,  257,  253,  536,  536,  536,
 /*  4820 */   536,  536,  536,  536,  288,  536,  565,  286,  272,  274,
 /*  4830 */   278,  276,  280,  282,  284,  231,  237,  243,  246,  249,
 /*  4840 */   251,  240,  234,  260,  262,  270,  264,  266,  268,  536,
 /*  4850 */   536,  536,  536,  536,  536,  536,  536,  257,  253,  536,
 /*  4860 */   536,  500,  536,  536,  536,  536,  288,  536,  536,  286,
 /*  4870 */   272,  274,  278,  276,  280,  282,  284,  231,  237,  243,
 /*  4880 */   246,  249,  251,  240,  234,  260,  262,  270,  264,  266,
 /*  4890 */   268,  536,  536,  536,  536,  536,  536,  536,  536,  257,
 /*  4900 */   253,  536,  536,  504,   41,  536,  536,  536,  288,  536,
 /*  4910 */   536,  286,  272,  274,  278,  276,  280,  282,  284,  231,
 /*  4920 */   237,  243,  246,  249,  251,  240,  234,  260,  262,  270,
 /*  4930 */   264,  266,  268,  536,  536,  536,  536,  536,  536,  536,
 /*  4940 */   536,  257,  253,  536,   33,  536,   47,   48,   49,   50,
 /*  4950 */    51,   52,   53,   54,   55,   56,   57,  536,  536,  536,
 /*  4960 */   536,  536,  907,  536,  536,  536,  536,  536,  536,  536,
 /*  4970 */   536,  288,  536,  536,  286,  272,  274,  278,  276,  280,
 /*  4980 */   282,  284,  231,  237,  243,  246,  249,  251,  240,  234,
 /*  4990 */   260,  262,  270,  264,  266,  268,  536,  536,  536,  536,
 /*  5000 */   536,  536,  536,  536,  257,  253,  536,  536,  536,  536,
 /*  5010 */   536,  536,  536,  288,  536,  410,  286,  272,  274,  278,
 /*  5020 */   276,  280,  282,  284,  231,  237,  243,  246,  249,  251,
 /*  5030 */   240,  234,  260,  262,  270,  264,  266,  268,  536,  536,
 /*  5040 */   536,  536,  536,  536,  536,  536,  257,  253,  536,  536,
 /*  5050 */   536,  536,  536,  536,  536,  288,  536,  553,  286,  272,
 /*  5060 */   274,  278,  276,  280,  282,  284,  231,  237,  243,  246,
 /*  5070 */   249,  251,  240,  234,  260,  262,  270,  264,  266,  268,
 /*  5080 */   536,  536,  536,  536,  536,  536,  536,  288,  257,  253,
 /*  5090 */   286,  272,  274,  278,  276,  280,  282,  284,  231,  237,
 /*  5100 */   243,  246,  249,  251,  240,  234,  260,  262,  270,  264,
 /*  5110 */   266,  268,  536,  536,  536,  536,  536,  536,  536,  536,
 /*  5120 */   257,  253,  536,  536,  507,  536,  536,  290,  536,  288,
 /*  5130 */   536,  536,  286,  272,  274,  278,  276,  280,  282,  284,
 /*  5140 */   231,  237,  243,  246,  249,  251,  240,  234,  260,  262,
 /*  5150 */   270,  264,  266,  268,  235,  536,  244,  247,  536,  536,
 /*  5160 */   536,  536,  257,  253,  536,  536,   96,  536,  298,  536,
 /*  5170 */   536,  536,  536,  536,  536,  536,  536,  536,  536,  536,
 /*  5180 */   238,  241,  232,  306,  311,  325,  229,  348,  321,   68,
 /*  5190 */   536,  302,  536,  235,  536,  244,  247,  536,  536,  332,
 /*  5200 */   536,  536,  536,  536,  320,  536,  292,  298,   47,   48,
 /*  5210 */    49,   50,   51,   52,   53,   54,   55,   56,   57,  238,
 /*  5220 */   241,  232,  306,  311,  325,  229,  366,  321,  536,  536,
 /*  5230 */   302,  536,  235,  536,  244,  247,  536,  536,  332,  536,
 /*  5240 */   536,  536,  314,  320,  536,  292,  298,  536,  536,  536,
 /*  5250 */   313,  315,  316,  317,  318,  319,  536,  536,  238,  241,
 /*  5260 */   232,  306,  311,  325,  229,  328,  321,  536,  536,  302,
 /*  5270 */   536,  235,  536,  244,  247,  536,  536,  332,  536,  536,
 /*  5280 */   536,  314,  320,  536,  292,  298,  536,  536,  536,  313,
 /*  5290 */   315,  316,  317,  318,  319,  536,  536,  238,  241,  232,
 /*  5300 */   306,  311,  325,  229,  392,  321,  536,  536,  302,  536,
 /*  5310 */   235,  536,  244,  247,  536,  536,  332,  536,  536,  536,
 /*  5320 */   314,  320,  536,  292,  298,  536,  536,  536,  313,  315,
 /*  5330 */   316,  317,  318,  319,  536,  536,  238,  241,  232,  306,
 /*  5340 */   311,  325,  229,  361,  321,  536,  536,  302,  536,  235,
 /*  5350 */   536,  244,  247,  536,  536,  332,  536,  536,  536,  314,
 /*  5360 */   320,  536,  292,  298,  536,  536,  536,  313,  315,  316,
 /*  5370 */   317,  318,  319,  536,  536,  238,  241,  232,  306,  311,
 /*  5380 */   325,  229,  338,  321,  536,  536,  302,  536,  536,  536,
 /*  5390 */   536,  536,  536,  536,  332,  536,  536,  536,  314,  320,
 /*  5400 */   536,  292,  536,  536,  536,  536,  313,  315,  316,  317,
 /*  5410 */   318,  319,  536,  286,  272,  274,  278,  276,  280,  282,
 /*  5420 */   284,  231,  237,  243,  246,  249,  251,  240,  234,  260,
 /*  5430 */   262,  270,  264,  266,  268,  536,  536,  314,  235,  536,
 /*  5440 */   244,  247,  536,  257,  253,  313,  315,  316,  317,  318,
 /*  5450 */   319,  536,  298,  536,  536,  536,  536,  536,  536,  536,
 /*  5460 */   536,  536,  536,  536,  238,  241,  232,  306,  311,  325,
 /*  5470 */   229,  536,  321,  536,  536,  302,  536,  235,  536,  244,
 /*  5480 */   247,  536,  536,  332,  536,  536,  536,  536,  320,  536,
 /*  5490 */   292,  298,  536,  536,  536,  536,  536,  536,  536,  536,
 /*  5500 */   536,  536,  536,  238,  241,  232,  306,  311,  325,  229,
 /*  5510 */   381,  321,  536,  536,  302,  536,  536,  235,  563,  244,
 /*  5520 */   247,  536,  332,  536,  536,  536,  314,  320,  536,  292,
 /*  5530 */   536,  298,  536,  536,  313,  315,  316,  317,  318,  319,
 /*  5540 */   536,  536,  536,  238,  241,  232,  306,  311,  325,  229,
 /*  5550 */   536,  321,  536,  536,  302,  536,  235,  536,  244,  247,
 /*  5560 */   536,  536,  332,  536,  536,  314,  536,  320,  536,  292,
 /*  5570 */   298,  536,  462,  313,  315,  316,  317,  318,  319,  536,
 /*  5580 */   536,  536,  238,  241,  232,  306,  311,  325,  229,  398,
 /*  5590 */   321,  536,  536,  302,  536,  235,  536,  244,  247,  536,
 /*  5600 */   536,  332,  536,  536,  536,  314,  320,  536,  292,  298,
 /*  5610 */   536,  536,  536,  313,  315,  316,  317,  318,  319,  536,
 /*  5620 */   536,  238,  241,  232,  306,  311,  325,  229,  385,  321,
 /*  5630 */   536,  536,  302,  536,  235,  536,  244,  247,  536,  536,
 /*  5640 */   332,  536,  536,  536,  314,  320,  536,  292,  298,  536,
 /*  5650 */   536,  536,  313,  315,  316,  317,  318,  319,  536,  536,
 /*  5660 */   238,  241,  232,  306,  311,  325,  229,  536,  321,  536,
 /*  5670 */   536,  302,  536,  235,  536,  244,  247,  536,  536,  332,
 /*  5680 */   536,  536,  536,  314,  320,  536,  292,  298,  536,  469,
 /*  5690 */   536,  313,  315,  316,  317,  318,  319,  536,  536,  238,
 /*  5700 */   241,  232,  306,  311,  325,  229,  536,  321,  536,  536,
 /*  5710 */   302,  536,  235,  536,  244,  247,  536,  536,  332,  536,
 /*  5720 */   536,  536,  314,  320,  536,  292,  298,  536,  434,  536,
 /*  5730 */   313,  315,  316,  317,  318,  319,  536,  536,  238,  241,
 /*  5740 */   232,  306,  311,  325,  229,  536,  321,  536,  536,  302,
 /*  5750 */   501,  235,  536,  244,  247,  536,  536,  332,  536,  536,
 /*  5760 */   536,  314,  320,  536,  292,  298,  536,  536,  536,  313,
 /*  5770 */   315,  316,  317,  318,  319,  536,  536,  238,  241,  232,
 /*  5780 */   306,  311,  325,  229,  355,  321,  536,  536,  302,  536,
 /*  5790 */   235,  536,  244,  247,  536,  536,  332,  536,  536,  536,
 /*  5800 */   314,  320,  536,  292,  298,  536,  536,  536,  313,  315,
 /*  5810 */   316,  317,  318,  319,  536,  536,  238,  241,  232,  306,
 /*  5820 */   311,  325,  229,  536,  321,  536,  536,  323,  536,  235,
 /*  5830 */   536,  244,  247,  536,  536,  332,  536,  536,  536,  314,
 /*  5840 */   320,  536,  292,  298,  536,  369,  536,  313,  315,  316,
 /*  5850 */   317,  318,  319,  536,  536,  238,  241,  232,  306,  311,
 /*  5860 */   325,  229,  536,  321,  536,  536,  302,  536,  235,  536,
 /*  5870 */   244,  247,  536,  536,  332,  536,  536,  536,  368,  320,
 /*  5880 */   536,  292,  298,  536,  536,  536,  367,  315,  316,  317,
 /*  5890 */   318,  319,  536,  536,  238,  241,  232,  306,  311,  325,
 /*  5900 */   229,  536,  321,  536,  536,  302,  536,  536,  235,  551,
 /*  5910 */   244,  247,  536,  332,  536,  536,  536,  314,  320,  536,
 /*  5920 */   292,  536,  298,  536,  536,  313,  315,  316,  317,  318,
 /*  5930 */   319,  536,  536,  536,  238,  241,  232,  306,  311,  325,
 /*  5940 */   229,  536,  321,  536,  536,  323,  536,  536,  536,  536,
 /*  5950 */   536,  536,  536,  332,  536,  536,  314,  536,  320,  536,
 /*  5960 */   292,  536,  536,  536,  313,  315,  316,  317,  318,  319,
 /*  5970 */   536,  272,  274,  278,  276,  280,  282,  284,  231,  237,
 /*  5980 */   243,  246,  249,  251,  240,  234,  260,  262,  270,  264,
 /*  5990 */   266,  268,  536,   96,  536,  536,  368,  536,  536,  536,
 /*  6000 */   257,  253,  536,  536,  367,  315,  316,  317,  318,  319,
 /*  6010 */    96,  536,  113,  536,  536,  536,   68,  536,  536,  536,
 /*  6020 */   536,  536,  536,  536,  536,  536,  536,   96,  536,  608,
 /*  6030 */   536,  536,  536,   68,  536,   47,   48,   49,   50,   51,
 /*  6040 */    52,   53,   54,   55,   56,   57,   29,  536,  536,   41,
 /*  6050 */    68,  536,   47,   48,   49,   50,   51,   52,   53,   54,
 /*  6060 */    55,   56,   57,   96,  536,  536,  536,  536,  536,   47,
 /*  6070 */    48,   49,   50,   51,   52,   53,   54,   55,   56,   57,
 /*  6080 */   536,  536,  160,  536,  536,  536,   68,  536,  468,  429,
 /*  6090 */   536,   47,   48,   49,   50,   51,   52,   53,   54,   55,
 /*  6100 */    56,   57,  419,  536,  536,   47,   48,   49,   50,   51,
 /*  6110 */    52,   53,   54,   55,   56,   57,  100,  101,  103,  102,
 /*  6120 */   104,  536,  536,  536,  536,  536,  536,  536,  536,  536,
 /*  6130 */   536,  536,  536,  536,  536,  536,  420,  421,  422,  423,
 /*  6140 */   424,  425,  536,  457,  475,  476,  100,  101,  103,  102,
 /*  6150 */   104,  536,  536,  536,  536,  536,  536,  536,  536,  536,
 /*  6160 */   536,  536,  536,  536,  536,  106,  107,  536,  736,  127,
 /*  6170 */   536,  133,  536,  536,  536,  105,  536,  100,  101,  103,
 /*  6180 */   102,  104,  536,  536,  536,  536,  536,  536,  536,  536,
 /*  6190 */   536,  536,  536,  536,  536,  106,  107,  536,   19,  127,
 /*  6200 */   536,  133,  536,  536,  536,  105,  536,  100,  101,  103,
 /*  6210 */   102,  104,  536,  536,  536,  536,  536,  536,  536,  536,
 /*  6220 */   536,  536,  536,  536,  536,  536,  106,  107,  536,  727,
 /*  6230 */   636,  536,  133,  536,  536,  536,  105,  536,  100,  101,
 /*  6240 */   103,  102,  104,  536,  536,  536,  536,  536,  536,  536,
 /*  6250 */   536,  536,  536,  536,  536,  536,  106,  107,  536,  731,
 /*  6260 */   634,  536,  133,  536,  536,  536,  105,  536,  100,  101,
 /*  6270 */   103,  102,  104,  536,  536,  536,  536,  536,  536,  536,
 /*  6280 */   536,  536,  536,  536,  536,  536,  536,  106,  107,  536,
 /*  6290 */   728,  636,  536,  133,  536,  536,  536,  105,  536,  100,
 /*  6300 */   101,  103,  102,  104,  100,  101,  103,  102,  104,  536,
 /*  6310 */   536,  536,  536,  536,  536,  536,  536,  106,  107,  536,
 /*  6320 */   150,  636,  536,  133,  536,  536,  536,  105,  536,  100,
 /*  6330 */   101,  103,  102,  104,  100,  101,  103,  102,  104,  536,
 /*  6340 */   536,  536,  536,  536,  536,  536,  536,  536,  106,  107,
 /*  6350 */   536,  737,  108,  106,  107,  536,  729,  603,  105,  536,
 /*  6360 */   536,  536,  536,  105,  536,  100,  101,  103,  102,  104,
 /*  6370 */   100,  101,  103,  102,  104,  536,  536,  536,  106,  107,
 /*  6380 */   536,  733,  603,  106,  107,  536,  735,  603,  105,  536,
 /*  6390 */   536,  536,  536,  105,  536,  100,  101,  103,  102,  104,
 /*  6400 */   100,  101,  103,  102,  104,  536,  100,  101,  103,  102,
 /*  6410 */   104,  536,  536,  658,  106,  107,  536,  732,  670,  106,
 /*  6420 */   107,  536,  536,  536,  105,  536,  536,  157,  536,  105,
 /*  6430 */   536,  100,  101,  103,  102,  104,  100,  101,  103,  102,
 /*  6440 */   104,  536,  536,  638,  106,  107,  536,  738,  108,  106,
 /*  6450 */   107,  536,  536,  536,  105,  106,  107,  605,  536,  105,
 /*  6460 */   536,  536,  536,   26,  536,  105,  536,  100,  101,  103,
 /*  6470 */   102,  104,  536,  536,  536,  536,  536,  536,  536,  536,
 /*  6480 */   106,  107,  536,  734,  603,  106,  107,  536,  730,  603,
 /*  6490 */   105,  536,  536,  536,  536,  105,  536,  100,  101,  103,
 /*  6500 */   102,  104,  100,  101,  103,  102,  104,  536,  100,  101,
 /*  6510 */   103,  102,  104,  536,  536,  536,  106,  107,  536,  536,
 /*  6520 */   536,  536,  128,  536,  536,  536,  105,  536,  536,  536,
 /*  6530 */   536,  536,  536,  536,  536,  536,  536,  536,  536,  536,
 /*  6540 */   536,  536,  536,  536,  536,  536,  106,  107,  536,  536,
 /*  6550 */   536,  106,  107,  536,  605,  536,  105,  106,  107,  110,
 /*  6560 */   536,  105,  536,  536,  536,  157,  536,  105,
};
static YYCODETYPE yy_lookahead[] = {
 /*     0 */     7,   53,    9,   10,  111,  112,  113,  114,  115,  116,
 /*    10 */   117,  118,  119,  141,   21,  143,  144,  145,  146,  147,
 /*    20 */    41,   52,  176,   44,  178,  179,   33,   34,   35,   36,
 /*    30 */    37,   38,   39,  122,   41,  124,   57,   44,   45,   27,
 /*    40 */    28,   29,   30,   31,   32,   52,   53,   54,  145,  146,
 /*    50 */    57,  148,   59,   41,   42,   42,   63,   64,   65,   66,
 /*    60 */    67,   68,   69,   70,   71,   52,   73,   74,   75,   76,
 /*    70 */    44,   78,   41,   42,   95,   82,   83,   84,   85,   53,
 /*    80 */    54,   88,  103,  104,  105,  106,  107,  108,   95,    6,
 /*    90 */   132,   44,   99,  100,  101,  102,  103,  104,  105,  106,
 /*   100 */   107,  108,  109,    7,   57,    9,   10,  123,  187,  188,
 /*   110 */   126,  127,  128,  131,  130,  131,  139,   21,   53,  137,
 /*   120 */   136,  137,  145,  146,  142,  148,  142,   52,   45,   33,
 /*   130 */    34,   35,   36,   37,   38,   39,  176,   41,  178,  179,
 /*   140 */    44,   45,  127,  128,   44,  130,  131,  131,   52,   53,
 /*   150 */    54,  136,  137,   57,  138,   59,   59,  142,  142,   63,
 /*   160 */    64,   65,   66,   67,   68,   69,   70,   71,    6,   73,
 /*   170 */    74,   75,   76,   41,   78,   45,   55,   80,   82,   83,
 /*   180 */    84,   85,   52,  139,   88,   55,   44,   55,   44,  145,
 /*   190 */   146,   95,  148,   96,   52,   99,  100,  101,  102,  103,
 /*   200 */   104,  105,  106,  107,  108,  109,    7,   45,    9,   10,
 /*   210 */    89,   90,   91,   92,   93,   94,  144,  145,  146,  147,
 /*   220 */    21,   89,   90,   91,   92,   93,   94,  174,  175,   97,
 /*   230 */    98,   53,   33,   34,   35,   36,   37,   38,   39,  176,
 /*   240 */    41,  178,  179,   44,   45,  114,  115,  116,  117,  118,
 /*   250 */   119,   52,   53,   54,    6,   96,   57,   79,   59,   81,
 /*   260 */   187,  188,   63,   64,   65,   66,   67,   68,   69,   70,
 /*   270 */    71,    6,   73,   74,   75,   76,   41,   78,   45,  187,
 /*   280 */   188,   82,   83,   84,   85,   52,  132,   88,   55,   44,
 /*   290 */    55,   47,  145,   49,   95,   22,   52,   52,   99,  100,
 /*   300 */   101,  102,  103,  104,  105,  106,  107,  108,  109,    7,
 /*   310 */    45,    9,   10,  141,   41,  143,  144,  145,  146,  147,
 /*   320 */   173,   53,   53,   21,   89,   90,   91,   92,   93,   94,
 /*   330 */   183,  184,  185,  186,   86,   33,   34,   35,   36,   37,
 /*   340 */    38,   39,    6,   41,  187,  188,   44,   45,   79,    6,
 /*   350 */    81,   30,   31,   32,   52,   53,   54,    6,    6,   57,
 /*   360 */     6,   59,   41,   42,   57,   63,   64,   65,   66,   67,
 /*   370 */    68,   69,   70,   71,   45,   73,   74,   75,   76,   41,
 /*   380 */    78,   52,    6,   40,   82,   83,   84,   85,   52,   47,
 /*   390 */    88,   49,   40,   55,   52,   42,   45,   95,   45,   45,
 /*   400 */    44,   99,  100,  101,  102,  103,  104,  105,  106,  107,
 /*   410 */   108,  109,    7,  131,    9,   10,  187,  188,  187,  188,
 /*   420 */   170,   45,   42,  173,  142,   45,   21,   89,   90,   91,
 /*   430 */    92,   93,   94,  183,  184,  185,  186,  131,   33,   34,
 /*   440 */    35,   36,   37,   38,   39,    6,   41,  139,  142,   44,
 /*   450 */    45,   95,  128,  145,  146,  131,  148,   52,   53,   54,
 /*   460 */   136,  137,   57,   44,   59,    6,  142,   80,   63,   64,
 /*   470 */    65,   66,   67,   68,   69,   70,   71,   47,   73,   74,
 /*   480 */    75,   76,   52,   78,   45,  173,   44,   82,   83,   84,
 /*   490 */    85,  187,  188,   88,   52,  183,  184,  185,  186,   40,
 /*   500 */    95,    6,  190,  191,   99,  100,  101,  102,  103,  104,
 /*   510 */   105,  106,  107,  108,  109,    7,    6,    9,   10,  141,
 /*   520 */   173,  143,  144,  145,  146,  147,   42,  187,  188,   21,
 /*   530 */   183,  184,  185,  186,    6,    6,   52,  190,  191,    6,
 /*   540 */    45,   33,   34,   35,   36,   37,   38,   39,   42,   41,
 /*   550 */    59,    6,   44,   45,  122,   45,  124,  129,   52,  131,
 /*   560 */    52,   53,   54,    6,  136,   57,  138,   59,   40,   40,
 /*   570 */   142,   63,   64,   65,   66,   67,   68,   69,   70,   71,
 /*   580 */    55,   73,   74,   75,   76,   40,   78,   96,  173,   47,
 /*   590 */    82,   83,   84,   85,   52,   62,   88,    6,  183,  184,
 /*   600 */   185,  186,   45,   95,    6,  190,  191,   99,  100,  101,
 /*   610 */   102,  103,  104,  105,  106,  107,  108,  109,    7,    6,
 /*   620 */     9,   10,   43,   49,  173,   46,   52,   48,    6,   50,
 /*   630 */    51,   40,   21,   54,  183,  184,  185,  186,   40,  134,
 /*   640 */   135,  190,  191,   44,   33,   34,   35,   36,   37,   38,
 /*   650 */    39,   52,   41,   40,   42,   44,   45,   45,  128,   42,
 /*   660 */   130,  131,   40,   52,   53,   54,   39,  137,   57,   52,
 /*   670 */    59,   44,  142,   44,   63,   64,   65,   66,   67,   68,
 /*   680 */    69,   70,   71,   54,   73,   74,   75,   76,  109,   78,
 /*   690 */    52,  173,   44,   82,   83,   84,   85,  171,  172,   88,
 /*   700 */    52,  183,  184,  185,  186,  124,   95,    6,  190,  191,
 /*   710 */    99,  100,  101,  102,  103,  104,  105,  106,  107,  108,
 /*   720 */   109,    7,    6,    9,   10,  141,  173,  143,  144,  145,
 /*   730 */   146,  147,  122,   45,  124,   21,  183,  184,  185,  186,
 /*   740 */    52,   40,    6,  190,  191,  131,   44,   33,   34,   35,
 /*   750 */    36,   37,   38,   39,  139,   41,  142,    6,   44,   45,
 /*   760 */   145,  146,  176,  148,   59,  179,   52,   53,   54,   53,
 /*   770 */    80,   57,  176,   59,    6,  179,   40,   63,   64,   65,
 /*   780 */    66,   67,   68,   69,   70,   71,   44,   73,   74,   75,
 /*   790 */    76,   40,   78,   59,  173,  121,   82,   83,   84,   85,
 /*   800 */   176,   96,   88,  179,  183,  184,  185,  186,   40,   95,
 /*   810 */     6,  190,  191,   99,  100,  101,  102,  103,  104,  105,
 /*   820 */   106,  107,  108,  109,    7,    6,    9,   10,  141,  173,
 /*   830 */   143,  144,  145,  146,  147,   80,   53,   95,   21,  183,
 /*   840 */   184,  185,  186,   44,  187,  188,  190,  191,   62,   45,
 /*   850 */    33,   34,   35,   36,   37,   38,   39,  132,   41,  187,
 /*   860 */   188,   44,   45,   42,   45,  121,   45,   44,   39,   52,
 /*   870 */    53,   54,   52,  121,   57,   45,   59,  132,   40,   77,
 /*   880 */    63,   64,   65,   66,   67,   68,   69,   70,   71,  132,
 /*   890 */    73,   74,   75,   76,  121,   78,   53,  173,  120,   82,
 /*   900 */    83,   84,   85,  176,   44,   88,    6,  183,  184,  185,
 /*   910 */   186,   55,   95,   55,  190,  191,   99,  100,  101,  102,
 /*   920 */   103,  104,  105,  106,  107,  108,  109,    7,   45,    9,
 /*   930 */    10,  141,  173,  143,  144,  145,  146,  147,   44,    0,
 /*   940 */    45,   21,  183,  184,  185,  186,   44,   59,   80,  190,
 /*   950 */   191,  142,   44,   33,   34,   35,   36,   37,   38,   39,
 /*   960 */   133,   41,   45,   62,   44,   45,  121,  132,   22,   59,
 /*   970 */   133,   45,   52,   53,   54,  121,   45,   57,   45,   59,
 /*   980 */    45,  176,   44,   63,   64,   65,   66,   67,   68,   69,
 /*   990 */    70,   71,   55,   73,   74,   75,   76,   45,   78,   59,
 /*  1000 */    44,  173,   82,   83,   84,   85,  135,   44,   88,  181,
 /*  1010 */   182,  183,  184,  185,  186,   95,   44,   53,   45,   99,
 /*  1020 */   100,  101,  102,  103,  104,  105,  106,  107,  108,  109,
 /*  1030 */     7,   52,    9,   10,   45,  126,   44,  128,   57,  130,
 /*  1040 */   131,   45,   44,  132,   21,  136,  137,   53,   44,   52,
 /*  1050 */    59,  142,  133,   45,  120,   53,   33,   34,   35,   36,
 /*  1060 */    37,   38,   39,   80,   41,   59,   45,   44,   45,   44,
 /*  1070 */    48,   45,   55,  132,   55,   52,   53,   54,  133,   52,
 /*  1080 */    57,   44,   59,   45,   77,  172,   63,   64,   65,   66,
 /*  1090 */    67,   68,   69,   70,   71,  121,   73,   74,   75,   76,
 /*  1100 */    45,   78,   44,  173,   44,   82,   83,   84,   85,   52,
 /*  1110 */   180,   88,   83,  183,  184,  185,  186,   44,   95,  176,
 /*  1120 */    53,   14,   99,  100,  101,  102,  103,  104,  105,  106,
 /*  1130 */   107,  108,  109,    7,   59,    9,   10,  125,   45,  127,
 /*  1140 */    44,  129,   44,  131,  176,   45,   52,   21,  136,   44,
 /*  1150 */   138,   45,   44,   22,  142,  121,   59,   44,   57,   33,
 /*  1160 */    34,   35,   36,   37,   38,   39,  121,   41,   52,   52,
 /*  1170 */    44,   45,  132,   53,   52,  175,   86,  176,   52,   53,
 /*  1180 */    54,   44,  132,   57,   59,   59,   62,   44,   59,   63,
 /*  1190 */    64,   65,   66,   67,   68,   69,   70,   71,   59,   73,
 /*  1200 */    74,   75,   76,  170,   78,   55,  173,  176,   82,   83,
 /*  1210 */    84,   85,  188,   83,   88,   59,  183,  184,  185,  186,
 /*  1220 */    48,   95,   22,   45,  195,   99,  100,  101,  102,  103,
 /*  1230 */   104,  105,  106,  107,  108,  109,    7,   52,    9,   10,
 /*  1240 */   195,  173,  195,  195,  195,  177,  195,  195,  195,  195,
 /*  1250 */    21,  183,  184,  185,  186,  141,  195,  143,  144,  145,
 /*  1260 */   146,  147,   33,   34,   35,   36,   37,   38,   39,  195,
 /*  1270 */    41,  195,  195,   44,   45,  195,  195,  195,  195,  195,
 /*  1280 */   195,   52,   53,   54,  195,  195,   57,  195,   59,  195,
 /*  1290 */   195,  195,   63,   64,   65,   66,   67,   68,   69,   70,
 /*  1300 */    71,  195,   73,   74,   75,   76,  170,   78,  195,  173,
 /*  1310 */   195,   82,   83,   84,   85,  195,  195,   88,  195,  183,
 /*  1320 */   184,  185,  186,  195,   95,  195,  195,  195,   99,  100,
 /*  1330 */   101,  102,  103,  104,  105,  106,  107,  108,  109,    7,
 /*  1340 */   195,    9,   10,  195,  173,  195,  195,  195,  177,  195,
 /*  1350 */   195,  195,  195,   21,  183,  184,  185,  186,  141,  195,
 /*  1360 */   143,  144,  145,  146,  147,   33,   34,   35,   36,   37,
 /*  1370 */    38,   39,  195,   41,  195,  195,   44,   45,  195,  195,
 /*  1380 */   195,  195,  195,  195,   52,   53,   54,  195,  195,   57,
 /*  1390 */   195,   59,  195,  195,  195,   63,   64,   65,   66,   67,
 /*  1400 */    68,   69,   70,   71,  195,   73,   74,   75,   76,  170,
 /*  1410 */    78,  195,  173,  195,   82,   83,   84,   85,  195,  195,
 /*  1420 */    88,  195,  183,  184,  185,  186,  195,   95,  195,  195,
 /*  1430 */   195,   99,  100,  101,  102,  103,  104,  105,  106,  107,
 /*  1440 */   108,  109,    7,  195,    9,   10,  195,  173,  195,  195,
 /*  1450 */   195,  177,  195,  195,  195,  195,   21,  183,  184,  185,
 /*  1460 */   186,  195,  195,  195,  195,  195,  195,  195,   33,   34,
 /*  1470 */    35,   36,   37,   38,   39,  195,   41,  195,  195,   44,
 /*  1480 */    45,  195,  195,  195,  195,  195,  195,   52,   53,   54,
 /*  1490 */   195,  195,   57,  195,   59,  195,  195,  195,   63,   64,
 /*  1500 */    65,   66,   67,   68,   69,   70,   71,  195,   73,   74,
 /*  1510 */    75,   76,  195,   78,  195,  173,  195,   82,   83,   84,
 /*  1520 */    85,  195,  195,   88,  182,  183,  184,  185,  186,  195,
 /*  1530 */    95,  195,  195,  195,   99,  100,  101,  102,  103,  104,
 /*  1540 */   105,  106,  107,  108,  109,    7,  195,    9,   10,  195,
 /*  1550 */   173,  195,  195,  195,  177,  195,  195,  195,  195,   21,
 /*  1560 */   183,  184,  185,  186,  195,  195,  195,  195,  195,  195,
 /*  1570 */   195,   33,   34,   35,   36,   37,   38,   39,  195,   41,
 /*  1580 */   195,  195,   44,   45,  195,  195,  195,  195,  195,  195,
 /*  1590 */    52,   53,   54,  195,  195,   57,  195,   59,  195,  195,
 /*  1600 */   195,   63,   64,   65,   66,   67,   68,   69,   70,   71,
 /*  1610 */   195,   73,   74,   75,   76,  195,   78,  195,  173,  195,
 /*  1620 */    82,   83,   84,   85,  195,  195,   88,  195,  183,  184,
 /*  1630 */   185,  186,  195,   95,  195,  195,  191,   99,  100,  101,
 /*  1640 */   102,  103,  104,  105,  106,  107,  108,  109,    7,  195,
 /*  1650 */     9,   10,  195,  173,  195,  195,  195,  177,  195,  195,
 /*  1660 */   195,  195,   21,  183,  184,  185,  186,  195,  195,  195,
 /*  1670 */   195,  195,  195,  195,   33,   34,   35,   36,   37,   38,
 /*  1680 */    39,  195,   41,  195,  195,   44,   45,  195,  195,  195,
 /*  1690 */   195,  195,  195,   52,   53,   54,  195,  195,   57,  195,
 /*  1700 */    59,  195,  195,  195,   63,   64,   65,   66,   67,   68,
 /*  1710 */    69,   70,   71,  195,   73,   74,   75,   76,  170,   78,
 /*  1720 */   195,  173,  195,   82,   83,   84,   85,  195,  195,   88,
 /*  1730 */   195,  183,  184,  185,  186,  195,   95,  195,  195,  195,
 /*  1740 */    99,  100,  101,  102,  103,  104,  105,  106,  107,  108,
 /*  1750 */   109,    7,  195,    9,   10,  195,  173,  195,  195,  195,
 /*  1760 */   177,  195,  195,  195,  195,   21,  183,  184,  185,  186,
 /*  1770 */   195,  195,  195,  195,  195,  195,  195,   33,   34,   35,
 /*  1780 */    36,   37,   38,   39,  195,   41,  195,  195,   44,   45,
 /*  1790 */   195,  195,  195,  195,  195,  195,   52,   53,   54,  195,
 /*  1800 */   195,   57,  195,   59,  195,  195,  195,   63,   64,   65,
 /*  1810 */    66,   67,   68,   69,   70,   71,  195,   73,   74,   75,
 /*  1820 */    76,  195,   78,  195,  173,  195,   82,   83,   84,   85,
 /*  1830 */   195,  195,   88,  195,  183,  184,  185,  186,  195,   95,
 /*  1840 */   195,  195,  195,   99,  100,  101,  102,  103,  104,  105,
 /*  1850 */   106,  107,  108,  109,    7,  195,    9,   10,  195,  173,
 /*  1860 */   195,  195,  195,  177,  195,  195,  195,  195,   21,  183,
 /*  1870 */   184,  185,  186,  195,  195,  195,  195,  195,  195,  195,
 /*  1880 */    33,   34,   35,   36,   37,   38,   39,  195,   41,  195,
 /*  1890 */   195,   44,   45,  195,  195,  195,  195,  195,  195,   52,
 /*  1900 */    53,   54,  195,  195,   57,  195,   59,  195,  195,  195,
 /*  1910 */    63,   64,   65,   66,   67,   68,   69,   70,   71,  195,
 /*  1920 */    73,   74,   75,   76,  195,   78,  195,  173,  195,   82,
 /*  1930 */    83,   84,   85,  195,  195,   88,  195,  183,  184,  185,
 /*  1940 */   186,  195,   95,  195,  195,  195,   99,  100,  101,  102,
 /*  1950 */   103,  104,  105,  106,  107,  108,  109,    7,  195,    9,
 /*  1960 */    10,  195,  173,  195,  195,  195,  177,  195,  195,  195,
 /*  1970 */   195,   21,  183,  184,  185,  186,  195,  195,  195,  195,
 /*  1980 */   195,  195,  195,   33,   34,   35,   36,   37,   38,   39,
 /*  1990 */   195,   41,  195,  195,   44,   45,  195,  195,  195,  195,
 /*  2000 */   195,  195,   52,   53,   54,  195,  195,   57,  195,   59,
 /*  2010 */   195,  195,  195,   63,   64,   65,   66,   67,   68,   69,
 /*  2020 */    70,   71,  195,   73,   74,   75,   76,  195,   78,  195,
 /*  2030 */   173,  195,   82,   83,   84,   85,  195,  195,   88,  195,
 /*  2040 */   183,  184,  185,  186,  195,   95,  195,  195,  195,   99,
 /*  2050 */   100,  101,  102,  103,  104,  105,  106,  107,  108,  109,
 /*  2060 */     7,  195,    9,   10,  195,  173,  195,  195,  195,  177,
 /*  2070 */   195,  195,  195,  195,   21,  183,  184,  185,  186,  195,
 /*  2080 */   195,  195,  195,  195,  195,  195,   33,   34,   35,   36,
 /*  2090 */    37,   38,   39,  195,   41,  195,  195,   44,   45,  195,
 /*  2100 */   195,  195,  195,  195,  195,   52,   53,   54,  195,  195,
 /*  2110 */    57,  195,   59,  195,  195,  195,   63,   64,   65,   66,
 /*  2120 */    67,   68,   69,   70,   71,  195,   73,   74,   75,   76,
 /*  2130 */   195,   78,  195,  173,  195,   82,   83,   84,   85,  195,
 /*  2140 */   195,   88,  195,  183,  184,  185,  186,  195,   95,  195,
 /*  2150 */   195,  195,   99,  100,  101,  102,  103,  104,  105,  106,
 /*  2160 */   107,  108,  109,    7,  195,    9,   10,  195,  173,  195,
 /*  2170 */   195,  195,  177,  195,  195,  195,  195,   21,  183,  184,
 /*  2180 */   185,  186,  195,  195,  195,  195,  195,  195,  195,   33,
 /*  2190 */    34,   35,   36,   37,   38,   39,  195,   41,  195,  195,
 /*  2200 */    44,   45,  195,  195,  195,  195,  195,  195,   52,   53,
 /*  2210 */    54,  195,  195,   57,  195,   59,  195,  195,  195,   63,
 /*  2220 */    64,   65,   66,   67,   68,   69,   70,   71,  195,   73,
 /*  2230 */    74,   75,   76,  195,   78,  195,  173,  195,   82,   83,
 /*  2240 */    84,   85,  195,  195,   88,  195,  183,  184,  185,  186,
 /*  2250 */   195,   95,  195,  195,  195,   99,  100,  101,  102,  103,
 /*  2260 */   104,  105,  106,  107,  108,  109,    7,  195,    9,   10,
 /*  2270 */   195,  173,  195,  195,  195,  177,  195,  195,  195,  195,
 /*  2280 */    21,  183,  184,  185,  186,  195,  195,  195,  195,  195,
 /*  2290 */   195,  195,   33,   34,   35,   36,   37,   38,   39,  195,
 /*  2300 */    41,  195,  195,   44,   45,  195,  195,  195,  195,  195,
 /*  2310 */   195,   52,   53,   54,  195,  195,   57,  195,   59,  195,
 /*  2320 */   195,  195,   63,   64,   65,   66,   67,   68,   69,   70,
 /*  2330 */    71,  195,   73,   74,   75,   76,  195,   78,  195,  173,
 /*  2340 */   195,   82,   83,   84,   85,  195,  195,   88,  195,  183,
 /*  2350 */   184,  185,  186,  195,   95,  195,  195,  195,   99,  100,
 /*  2360 */   101,  102,  103,  104,  105,  106,  107,  108,  109,    7,
 /*  2370 */   195,    9,   10,  195,  173,  195,  195,  195,  177,  195,
 /*  2380 */   195,  195,  195,   21,  183,  184,  185,  186,  195,  195,
 /*  2390 */   195,  195,  195,  195,  195,   33,   34,   35,   36,   37,
 /*  2400 */    38,   39,  195,   41,  195,  195,   44,   45,  195,  195,
 /*  2410 */   195,  195,  195,  195,   52,   53,   54,  195,  195,   57,
 /*  2420 */   195,   59,  195,  195,  195,   63,   64,   65,   66,   67,
 /*  2430 */    68,   69,   70,   71,  195,   73,   74,   75,   76,  195,
 /*  2440 */    78,  195,  173,  195,   82,   83,   84,   85,  195,  195,
 /*  2450 */    88,  195,  183,  184,  185,  186,  195,   95,  195,  195,
 /*  2460 */   195,   99,  100,  101,  102,  103,  104,  105,  106,  107,
 /*  2470 */   108,  109,    7,  195,    9,   10,  195,  173,  195,  195,
 /*  2480 */   195,  195,  195,  195,  195,  195,   21,  183,  184,  185,
 /*  2490 */   186,  195,  195,  195,  195,  195,  195,  195,   33,   34,
 /*  2500 */    35,   36,   37,   38,   39,  195,   41,  195,  195,   44,
 /*  2510 */    45,  195,  195,  195,  195,  195,  195,   52,   53,   54,
 /*  2520 */   195,  195,   57,  195,   59,  195,  195,  195,   63,   64,
 /*  2530 */    65,   66,   67,   68,   69,   70,   71,  195,   73,   74,
 /*  2540 */    75,   76,  195,   78,  195,  173,  195,   82,   83,   84,
 /*  2550 */    85,  195,  195,   88,  195,  183,  184,  185,  186,  195,
 /*  2560 */    95,  195,  195,  195,   99,  100,  101,  102,  103,  104,
 /*  2570 */   105,  106,  107,  108,  109,    7,  195,    9,   10,  195,
 /*  2580 */   173,  195,  195,  195,  195,  195,  195,  195,  195,   21,
 /*  2590 */   183,  184,  185,  186,  195,  195,  195,  195,  195,  195,
 /*  2600 */   195,   33,   34,   35,   36,   37,   38,   39,  195,   41,
 /*  2610 */   195,  195,   44,   45,  195,  195,  195,  195,  195,  195,
 /*  2620 */    52,   53,   54,  195,  195,   57,  195,   59,  195,  195,
 /*  2630 */   195,   63,   64,   65,   66,   67,   68,   69,   70,   71,
 /*  2640 */   195,   73,   74,   75,   76,  195,   78,  195,  173,  195,
 /*  2650 */    82,   83,   84,   85,  195,  195,   88,  195,  183,  184,
 /*  2660 */   185,  186,  195,   95,  195,  195,  195,   99,  100,  101,
 /*  2670 */   102,  103,  104,  105,  106,  107,  108,  109,    7,  195,
 /*  2680 */     9,   10,  195,  173,  195,  195,  195,  195,  195,  195,
 /*  2690 */   195,  195,   21,  183,  184,  185,  186,  195,  195,  195,
 /*  2700 */   195,  195,  195,  195,   33,   34,   35,   36,   37,   38,
 /*  2710 */    39,  195,   41,  195,  195,   44,   45,  195,  195,  195,
 /*  2720 */   195,  195,  195,   52,   53,   54,  195,  195,   57,  195,
 /*  2730 */    59,  195,  195,  195,   63,   64,   65,   66,   67,   68,
 /*  2740 */    69,   70,   71,  195,   73,   74,   75,   76,  195,   78,
 /*  2750 */   195,  173,  195,   82,   83,   84,   85,  195,  195,   88,
 /*  2760 */   195,  183,  184,  185,  186,  195,   95,  195,  195,  195,
 /*  2770 */    99,  100,  101,  102,  103,  104,  105,  106,  107,  108,
 /*  2780 */   109,    7,  195,    9,   10,  195,  173,  195,  195,  195,
 /*  2790 */   195,  195,  195,  195,  195,   21,  183,  184,  185,  186,
 /*  2800 */   195,  195,  195,  195,  195,  195,  195,   33,   34,   35,
 /*  2810 */    36,   37,   38,   39,  195,   41,  195,  195,   44,   45,
 /*  2820 */   195,  195,  195,  195,  195,  195,   52,   53,   54,  195,
 /*  2830 */   195,   57,  195,   59,  195,  195,  195,   63,   64,   65,
 /*  2840 */    66,   67,   68,   69,   70,   71,  195,   73,   74,   75,
 /*  2850 */    76,  195,   78,  195,  173,  195,   82,   83,   84,   85,
 /*  2860 */   195,  195,   88,  195,  183,  184,  185,  186,  195,   95,
 /*  2870 */   195,  195,  195,   99,  100,  101,  102,  103,  104,  105,
 /*  2880 */   106,  107,  108,  109,    7,  195,    9,   10,  195,  173,
 /*  2890 */   195,  195,  195,  195,  195,  195,  195,  195,   21,  183,
 /*  2900 */   184,  185,  186,  195,  195,  195,  195,  195,  195,  195,
 /*  2910 */    33,   34,   35,   36,   37,   38,   39,  195,   41,  195,
 /*  2920 */   195,   44,   45,  195,  195,  195,  195,  195,  195,   52,
 /*  2930 */    53,   54,  195,  195,   57,  195,   59,  195,  195,  195,
 /*  2940 */    63,   64,   65,   66,   67,   68,   69,   70,   71,  195,
 /*  2950 */    73,   74,   75,   76,  195,   78,  195,  173,  195,   82,
 /*  2960 */    83,   84,   85,  195,  195,   88,  195,  183,  184,  185,
 /*  2970 */   186,  195,   95,  195,  195,  195,   99,  100,  101,  102,
 /*  2980 */   103,  104,  105,  106,  107,  108,  109,    7,  195,    9,
 /*  2990 */    10,  195,  173,  195,  195,  195,  195,  195,  195,  195,
 /*  3000 */   195,   21,  183,  184,  185,  186,  195,  195,  195,  195,
 /*  3010 */   195,  195,  195,   33,   34,   35,   36,   37,   38,   39,
 /*  3020 */   195,   41,  195,  195,   44,   45,  195,  195,  195,  195,
 /*  3030 */   195,  195,   52,   53,   54,  195,  195,   57,  195,   59,
 /*  3040 */   195,  195,  195,   63,   64,   65,   66,   67,   68,   69,
 /*  3050 */    70,   71,  195,   73,   74,   75,   76,  195,   78,  195,
 /*  3060 */   173,  195,   82,   83,   84,   85,  195,  195,   88,  195,
 /*  3070 */   183,  184,  185,  186,  195,   95,  195,  195,  195,   99,
 /*  3080 */   100,  101,  102,  103,  104,  105,  106,  107,  108,  109,
 /*  3090 */     7,  195,    9,   10,  195,  173,  195,  195,  195,  195,
 /*  3100 */   195,  195,  195,  195,   21,  183,  184,  185,  186,  195,
 /*  3110 */   195,  195,  195,  195,  195,  195,   33,   34,   35,   36,
 /*  3120 */    37,   38,   39,  195,   41,  195,  195,   44,   45,  195,
 /*  3130 */   195,  195,  195,  195,  195,   52,   53,   54,  195,  195,
 /*  3140 */    57,  195,   59,  195,  195,  195,   63,   64,   65,   66,
 /*  3150 */    67,   68,   69,   70,   71,  195,   73,   74,   75,   76,
 /*  3160 */   195,   78,  195,  173,  195,   82,   83,   84,   85,  195,
 /*  3170 */   195,   88,  195,  183,  184,  185,  186,  195,   95,  195,
 /*  3180 */   195,  195,   99,  100,  101,  102,  103,  104,  105,  106,
 /*  3190 */   107,  108,  109,    7,  195,    9,   10,  195,  173,  195,
 /*  3200 */   195,  195,  195,  195,  195,  195,  195,   21,  183,  184,
 /*  3210 */   185,  186,  195,  195,  195,  195,  195,  195,  195,   33,
 /*  3220 */    34,   35,   36,   37,   38,   39,  195,   41,  195,  195,
 /*  3230 */    44,   45,  195,  195,  195,  195,  195,  195,   52,   53,
 /*  3240 */    54,  195,  195,   57,  195,   59,  195,  195,  195,   63,
 /*  3250 */    64,   65,   66,   67,   68,   69,   70,   71,  195,   73,
 /*  3260 */    74,   75,   76,  195,   78,  195,  173,  195,   82,   83,
 /*  3270 */    84,   85,  195,  195,   88,  195,  183,  184,  185,  186,
 /*  3280 */   195,   95,  195,  195,  195,   99,  100,  101,  102,  103,
 /*  3290 */   104,  105,  106,  107,  108,  109,    7,  195,    9,   10,
 /*  3300 */   195,  173,  195,  195,  195,  195,  195,  195,  195,  195,
 /*  3310 */    21,  183,  184,  185,  186,  195,  195,  195,  195,  195,
 /*  3320 */   195,  195,   33,   34,   35,   36,   37,   38,   39,  195,
 /*  3330 */    41,  195,  195,   44,   45,  195,  195,  195,  195,  195,
 /*  3340 */   195,   52,  195,   54,  195,  195,   57,  195,   59,  195,
 /*  3350 */   195,  195,   63,   64,   65,   66,   67,   68,   69,   70,
 /*  3360 */    71,    0,   73,   74,   75,   76,  195,   78,  195,  173,
 /*  3370 */   195,   82,   83,   84,   85,  195,  195,   88,  195,  183,
 /*  3380 */   184,  185,  186,  195,   95,  195,  195,  195,   99,  100,
 /*  3390 */   101,  102,  103,  104,  105,  106,  107,  108,  109,    7,
 /*  3400 */   195,    9,   10,  195,   43,  195,  195,   46,  195,   48,
 /*  3410 */   195,   50,   51,   21,  195,   54,  195,    1,    2,    3,
 /*  3420 */     4,    5,  195,  195,  195,   33,   34,   35,   36,   37,
 /*  3430 */    38,   39,  195,   41,  195,  173,   44,  195,  195,  195,
 /*  3440 */   195,  195,  195,  195,   52,  183,  184,  185,  186,   57,
 /*  3450 */   195,   59,  195,  195,  195,   63,   64,   65,   66,   67,
 /*  3460 */    68,   69,   70,   71,   72,   73,   50,   51,  195,  173,
 /*  3470 */   109,  118,  119,  195,  195,  195,   60,  195,  195,  183,
 /*  3480 */   184,  185,  186,  195,  195,  189,  195,   95,  192,  193,
 /*  3490 */   194,  195,  195,  140,  195,  103,  104,  105,  106,  107,
 /*  3500 */   108,  195,  149,  150,  151,  152,  153,  154,  155,  156,
 /*  3510 */   157,  158,  159,  160,  161,  162,  163,  164,  165,  166,
 /*  3520 */   167,  168,  169,  195,  195,  195,  173,  195,  195,  195,
 /*  3530 */   173,  195,  118,  119,  195,  195,  183,  184,  185,  186,
 /*  3540 */   183,  184,  185,  186,    1,    2,    3,    4,    5,  192,
 /*  3550 */   193,  194,  195,  195,  140,  195,  195,  195,  195,  195,
 /*  3560 */   195,  195,  195,  149,  150,  151,  152,  153,  154,  155,
 /*  3570 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  165,
 /*  3580 */   166,  167,  168,  169,  118,  119,  195,  173,  195,  195,
 /*  3590 */   195,  195,  195,   50,   51,  195,  195,  183,  184,  185,
 /*  3600 */   186,  173,  195,   60,  195,  195,  140,  195,  195,  195,
 /*  3610 */   195,  183,  184,  185,  186,  149,  150,  151,  152,  153,
 /*  3620 */   154,  155,  156,  157,  158,  159,  160,  161,  162,  163,
 /*  3630 */   164,  165,  166,  167,  168,  169,  118,  119,  195,  173,
 /*  3640 */   195,  195,  195,  173,  195,  195,  195,  195,  195,  183,
 /*  3650 */   184,  185,  186,  183,  184,  185,  186,  195,  140,  195,
 /*  3660 */   195,  195,  195,  195,  194,  195,  195,  149,  150,  151,
 /*  3670 */   152,  153,  154,  155,  156,  157,  158,  159,  160,  161,
 /*  3680 */   162,  163,  164,  165,  166,  167,  168,  169,  118,  119,
 /*  3690 */   195,  173,  195,  195,  195,  173,  195,  195,  195,  195,
 /*  3700 */   195,  183,  184,  185,  186,  183,  184,  185,  186,  195,
 /*  3710 */   140,  195,  195,  195,  195,  195,  195,  195,  195,  149,
 /*  3720 */   150,  151,  152,  153,  154,  155,  156,  157,  158,  159,
 /*  3730 */   160,  161,  162,  163,  164,  165,  166,  167,  168,  169,
 /*  3740 */   118,  119,  195,  173,  195,  195,  195,  173,  195,  195,
 /*  3750 */   195,  195,  195,  183,  184,  185,  186,  183,  184,  185,
 /*  3760 */   186,  195,  140,  195,  195,  195,  195,  195,  195,  195,
 /*  3770 */   195,  149,  150,  151,  152,  153,  154,  155,  156,  157,
 /*  3780 */   158,  159,  160,  161,  162,  163,  164,  165,  166,  167,
 /*  3790 */   168,  169,  118,  119,  195,  173,  195,  195,  195,  173,
 /*  3800 */   195,  195,  195,  195,  195,  183,  184,  185,  186,  183,
 /*  3810 */   184,  185,  186,  195,  140,  195,  195,  195,  195,  195,
 /*  3820 */   195,  195,  195,  149,  150,  151,  152,  153,  154,  155,
 /*  3830 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  165,
 /*  3840 */   166,  167,  168,  169,  118,  119,  195,  173,  195,  195,
 /*  3850 */   195,  173,  195,  195,  195,  195,  195,  183,  184,  185,
 /*  3860 */   186,  183,  184,  185,  186,  195,  140,  195,  195,  195,
 /*  3870 */   195,  195,  195,  195,  195,  149,  150,  151,  152,  153,
 /*  3880 */   154,  155,  156,  157,  158,  159,  160,  161,  162,  163,
 /*  3890 */   164,  165,  166,  167,  168,  169,  118,  119,  195,  173,
 /*  3900 */   195,  195,  195,  173,  195,  195,  195,  195,  195,  183,
 /*  3910 */   184,  185,  186,  183,  184,  185,  186,  195,  140,  195,
 /*  3920 */   195,  195,  195,  195,  195,  195,  195,  149,  150,  151,
 /*  3930 */   152,  153,  154,  155,  156,  157,  158,  159,  160,  161,
 /*  3940 */   162,  163,  164,  165,  166,  167,  168,  169,  118,  119,
 /*  3950 */   195,  173,  195,  195,  195,  173,  195,  195,  195,  195,
 /*  3960 */   195,  183,  184,  185,  186,  183,  184,  185,  186,  195,
 /*  3970 */   140,  195,  195,  195,  195,  195,  195,  195,  195,  149,
 /*  3980 */   150,  151,  152,  153,  154,  155,  156,  157,  158,  159,
 /*  3990 */   160,  161,  162,  163,  164,  165,  166,  167,  168,  169,
 /*  4000 */   118,  119,  195,  173,  195,  195,  195,  173,  195,  195,
 /*  4010 */   195,  195,  195,  183,  184,  185,  186,  183,  184,  185,
 /*  4020 */   186,  195,  140,  195,  195,  195,  195,  195,  195,  195,
 /*  4030 */   195,  149,  150,  151,  152,  153,  154,  155,  156,  157,
 /*  4040 */   158,  159,  160,  161,  162,  163,  164,  165,  166,  167,
 /*  4050 */   168,  169,  118,  119,  195,  173,  195,  195,  195,  173,
 /*  4060 */   195,  195,  195,  195,  195,  183,  184,  185,  186,  183,
 /*  4070 */   184,  185,  186,  195,  140,  195,  195,  195,  195,  195,
 /*  4080 */   195,  195,  195,  149,  150,  151,  152,  153,  154,  155,
 /*  4090 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  165,
 /*  4100 */   166,  167,  168,  169,  118,  119,  195,  173,  195,  195,
 /*  4110 */   195,  173,  195,  195,  195,  195,  195,  183,  184,  185,
 /*  4120 */   186,  183,  184,  185,  186,  195,  140,  195,  195,  195,
 /*  4130 */   195,  195,  195,  195,  195,  149,  150,  151,  152,  153,
 /*  4140 */   154,  155,  156,  157,  158,  159,  160,  161,  162,  163,
 /*  4150 */   164,  165,  166,  167,  168,  169,  118,  119,  195,  173,
 /*  4160 */   195,  195,  195,  173,  195,  195,  195,  195,  195,  183,
 /*  4170 */   184,  185,  186,  183,  184,  185,  186,  195,  140,  195,
 /*  4180 */   195,  195,  195,  195,  195,  195,  195,  149,  150,  151,
 /*  4190 */   152,  153,  154,  155,  156,  157,  158,  159,  160,  161,
 /*  4200 */   162,  163,  164,  165,  166,  167,  168,  169,  118,  119,
 /*  4210 */   195,  173,  195,  195,  195,  173,  195,  195,  195,  195,
 /*  4220 */   195,  183,  184,  185,  186,  183,  184,  185,  186,  195,
 /*  4230 */   140,  195,  195,  195,  195,  195,  195,  195,  195,  149,
 /*  4240 */   150,  151,  152,  153,  154,  155,  156,  157,  158,  159,
 /*  4250 */   160,  161,  162,  163,  164,  165,  166,  167,  168,  169,
 /*  4260 */   118,  119,  195,  173,  195,  195,  195,  173,  195,  195,
 /*  4270 */   195,  195,  195,  183,  184,  185,  186,  183,  184,  185,
 /*  4280 */   186,  195,  140,  195,  195,  195,  195,  195,  195,  195,
 /*  4290 */   195,  149,  150,  151,  152,  153,  154,  155,  156,  157,
 /*  4300 */   158,  159,  160,  161,  162,  163,  164,  165,  166,  167,
 /*  4310 */   168,  169,  118,  119,  195,  173,  195,  195,  195,  173,
 /*  4320 */   195,  195,  195,  195,  195,  183,  184,  185,  186,  183,
 /*  4330 */   184,  185,  186,  195,  140,  195,  195,  195,  195,  195,
 /*  4340 */   195,  195,  195,  149,  150,  151,  152,  153,  154,  155,
 /*  4350 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  165,
 /*  4360 */   166,  167,  168,  169,  118,  119,  195,  173,  195,  195,
 /*  4370 */   195,  173,  195,  195,  195,  195,  195,  183,  184,  185,
 /*  4380 */   186,  183,  184,  185,  186,  195,  140,  195,  195,  195,
 /*  4390 */   195,  195,  195,  195,  195,  149,  150,  151,  152,  153,
 /*  4400 */   154,  155,  156,  157,  158,  159,  160,  161,  162,  163,
 /*  4410 */   164,  165,  166,  167,  168,  169,  118,  119,  195,  173,
 /*  4420 */   195,  195,  195,  173,  195,  195,  195,  195,  195,  183,
 /*  4430 */   184,  185,  186,  183,  184,  185,  186,  195,  140,  195,
 /*  4440 */   195,  195,  195,  195,  195,  195,  195,  149,  150,  151,
 /*  4450 */   152,  153,  154,  155,  156,  157,  158,  159,  160,  161,
 /*  4460 */   162,  163,  164,  165,  166,  167,  168,  169,  118,  119,
 /*  4470 */   195,  173,  195,  195,  195,  173,  195,  195,  195,  195,
 /*  4480 */   195,  183,  184,  185,  186,  183,  184,  185,  186,  195,
 /*  4490 */   140,  195,  195,  195,  195,  195,  195,  195,  195,  149,
 /*  4500 */   150,  151,  152,  153,  154,  155,  156,  157,  158,  159,
 /*  4510 */   160,  161,  162,  163,  164,  165,  166,  167,  168,  169,
 /*  4520 */   118,  119,  195,  173,  195,  195,  195,  173,  195,  195,
 /*  4530 */   195,  195,  195,  183,  184,  185,  186,  183,  184,  185,
 /*  4540 */   186,  195,  195,  195,  195,  195,  195,  195,  195,  195,
 /*  4550 */   195,  149,  150,  151,  152,  153,  154,  155,  156,  157,
 /*  4560 */   158,  159,  160,  161,  162,  163,  164,  165,  166,  167,
 /*  4570 */   168,  169,  195,  195,  195,  173,  195,  195,  195,  195,
 /*  4580 */   195,  195,  195,  195,  195,  183,  184,  185,  186,  195,
 /*  4590 */   195,  195,  195,  195,    8,  195,  195,   11,   12,   13,
 /*  4600 */    14,   15,   16,   17,   18,   19,   20,   21,   22,   23,
 /*  4610 */    24,   25,   26,   27,   28,   29,   30,   31,   32,  195,
 /*  4620 */   195,  195,  195,  195,  195,  195,  195,   41,   42,  195,
 /*  4630 */   195,  195,  195,  195,  195,  195,    8,  195,   52,   11,
 /*  4640 */    12,   13,   14,   15,   16,   17,   18,   19,   20,   21,
 /*  4650 */    22,   23,   24,   25,   26,   27,   28,   29,   30,   31,
 /*  4660 */    32,  195,  195,  195,  195,  195,  195,  195,  195,   41,
 /*  4670 */    42,   19,   20,   21,   22,   23,   24,   25,   26,   27,
 /*  4680 */    28,   29,   30,   31,   32,  195,  195,  195,  195,  195,
 /*  4690 */    62,  195,  195,   41,   42,  195,  195,  195,  195,    8,
 /*  4700 */   195,  195,   11,   12,   13,   14,   15,   16,   17,   18,
 /*  4710 */    19,   20,   21,   22,   23,   24,   25,   26,   27,   28,
 /*  4720 */    29,   30,   31,   32,  195,  195,  195,  195,  195,  195,
 /*  4730 */   195,  195,   41,   42,  195,  195,   45,  195,  195,  195,
 /*  4740 */   195,    8,  195,  195,   11,   12,   13,   14,   15,   16,
 /*  4750 */    17,   18,   19,   20,   21,   22,   23,   24,   25,   26,
 /*  4760 */    27,   28,   29,   30,   31,   32,  195,  195,  195,  195,
 /*  4770 */   195,  195,  195,   40,   41,   42,  195,  195,  195,  195,
 /*  4780 */   195,  195,    8,  195,  195,   11,   12,   13,   14,   15,
 /*  4790 */    16,   17,   18,   19,   20,   21,   22,   23,   24,   25,
 /*  4800 */    26,   27,   28,   29,   30,   31,   32,  195,  195,  195,
 /*  4810 */   195,  195,  195,  195,  195,   41,   42,  195,  195,  195,
 /*  4820 */   195,  195,  195,  195,    8,  195,   52,   11,   12,   13,
 /*  4830 */    14,   15,   16,   17,   18,   19,   20,   21,   22,   23,
 /*  4840 */    24,   25,   26,   27,   28,   29,   30,   31,   32,  195,
 /*  4850 */   195,  195,  195,  195,  195,  195,  195,   41,   42,  195,
 /*  4860 */   195,   45,  195,  195,  195,  195,    8,  195,  195,   11,
 /*  4870 */    12,   13,   14,   15,   16,   17,   18,   19,   20,   21,
 /*  4880 */    22,   23,   24,   25,   26,   27,   28,   29,   30,   31,
 /*  4890 */    32,  195,  195,  195,  195,  195,  195,  195,  195,   41,
 /*  4900 */    42,  195,  195,   45,   21,  195,  195,  195,    8,  195,
 /*  4910 */   195,   11,   12,   13,   14,   15,   16,   17,   18,   19,
 /*  4920 */    20,   21,   22,   23,   24,   25,   26,   27,   28,   29,
 /*  4930 */    30,   31,   32,  195,  195,  195,  195,  195,  195,  195,
 /*  4940 */   195,   41,   42,  195,   61,  195,   63,   64,   65,   66,
 /*  4950 */    67,   68,   69,   70,   71,   72,   73,  195,  195,  195,
 /*  4960 */   195,  195,   62,  195,  195,  195,  195,  195,  195,  195,
 /*  4970 */   195,    8,  195,  195,   11,   12,   13,   14,   15,   16,
 /*  4980 */    17,   18,   19,   20,   21,   22,   23,   24,   25,   26,
 /*  4990 */    27,   28,   29,   30,   31,   32,  195,  195,  195,  195,
 /*  5000 */   195,  195,  195,  195,   41,   42,  195,  195,  195,  195,
 /*  5010 */   195,  195,  195,    8,  195,   52,   11,   12,   13,   14,
 /*  5020 */    15,   16,   17,   18,   19,   20,   21,   22,   23,   24,
 /*  5030 */    25,   26,   27,   28,   29,   30,   31,   32,  195,  195,
 /*  5040 */   195,  195,  195,  195,  195,  195,   41,   42,  195,  195,
 /*  5050 */   195,  195,  195,  195,  195,    8,  195,   52,   11,   12,
 /*  5060 */    13,   14,   15,   16,   17,   18,   19,   20,   21,   22,
 /*  5070 */    23,   24,   25,   26,   27,   28,   29,   30,   31,   32,
 /*  5080 */   195,  195,  195,  195,  195,  195,  195,    8,   41,   42,
 /*  5090 */    11,   12,   13,   14,   15,   16,   17,   18,   19,   20,
 /*  5100 */    21,   22,   23,   24,   25,   26,   27,   28,   29,   30,
 /*  5110 */    31,   32,  195,  195,  195,  195,  195,  195,  195,  195,
 /*  5120 */    41,   42,  195,  195,   45,  195,  195,   80,  195,    8,
 /*  5130 */   195,  195,   11,   12,   13,   14,   15,   16,   17,   18,
 /*  5140 */    19,   20,   21,   22,   23,   24,   25,   26,   27,   28,
 /*  5150 */    29,   30,   31,   32,    7,  195,    9,   10,  195,  195,
 /*  5160 */   195,  195,   41,   42,  195,  195,   21,  195,   21,  195,
 /*  5170 */   195,  195,  195,  195,  195,  195,  195,  195,  195,  195,
 /*  5180 */    33,   34,   35,   36,   37,   38,   39,   40,   41,   44,
 /*  5190 */   195,   44,  195,    7,  195,    9,   10,  195,  195,   52,
 /*  5200 */   195,  195,  195,  195,   57,  195,   59,   21,   63,   64,
 /*  5210 */    65,   66,   67,   68,   69,   70,   71,   72,   73,   33,
 /*  5220 */    34,   35,   36,   37,   38,   39,   40,   41,  195,  195,
 /*  5230 */    44,  195,    7,  195,    9,   10,  195,  195,   52,  195,
 /*  5240 */   195,  195,   95,   57,  195,   59,   21,  195,  195,  195,
 /*  5250 */   103,  104,  105,  106,  107,  108,  195,  195,   33,   34,
 /*  5260 */    35,   36,   37,   38,   39,   40,   41,  195,  195,   44,
 /*  5270 */   195,    7,  195,    9,   10,  195,  195,   52,  195,  195,
 /*  5280 */   195,   95,   57,  195,   59,   21,  195,  195,  195,  103,
 /*  5290 */   104,  105,  106,  107,  108,  195,  195,   33,   34,   35,
 /*  5300 */    36,   37,   38,   39,   40,   41,  195,  195,   44,  195,
 /*  5310 */     7,  195,    9,   10,  195,  195,   52,  195,  195,  195,
 /*  5320 */    95,   57,  195,   59,   21,  195,  195,  195,  103,  104,
 /*  5330 */   105,  106,  107,  108,  195,  195,   33,   34,   35,   36,
 /*  5340 */    37,   38,   39,   40,   41,  195,  195,   44,  195,    7,
 /*  5350 */   195,    9,   10,  195,  195,   52,  195,  195,  195,   95,
 /*  5360 */    57,  195,   59,   21,  195,  195,  195,  103,  104,  105,
 /*  5370 */   106,  107,  108,  195,  195,   33,   34,   35,   36,   37,
 /*  5380 */    38,   39,   40,   41,  195,  195,   44,  195,  195,  195,
 /*  5390 */   195,  195,  195,  195,   52,  195,  195,  195,   95,   57,
 /*  5400 */   195,   59,  195,  195,  195,  195,  103,  104,  105,  106,
 /*  5410 */   107,  108,  195,   11,   12,   13,   14,   15,   16,   17,
 /*  5420 */    18,   19,   20,   21,   22,   23,   24,   25,   26,   27,
 /*  5430 */    28,   29,   30,   31,   32,  195,  195,   95,    7,  195,
 /*  5440 */     9,   10,  195,   41,   42,  103,  104,  105,  106,  107,
 /*  5450 */   108,  195,   21,  195,  195,  195,  195,  195,  195,  195,
 /*  5460 */   195,  195,  195,  195,   33,   34,   35,   36,   37,   38,
 /*  5470 */    39,  195,   41,  195,  195,   44,  195,    7,  195,    9,
 /*  5480 */    10,  195,  195,   52,  195,  195,  195,  195,   57,  195,
 /*  5490 */    59,   21,  195,  195,  195,  195,  195,  195,  195,  195,
 /*  5500 */   195,  195,  195,   33,   34,   35,   36,   37,   38,   39,
 /*  5510 */    40,   41,  195,  195,   44,  195,  195,    7,   87,    9,
 /*  5520 */    10,  195,   52,  195,  195,  195,   95,   57,  195,   59,
 /*  5530 */   195,   21,  195,  195,  103,  104,  105,  106,  107,  108,
 /*  5540 */   195,  195,  195,   33,   34,   35,   36,   37,   38,   39,
 /*  5550 */   195,   41,  195,  195,   44,  195,    7,  195,    9,   10,
 /*  5560 */   195,  195,   52,  195,  195,   95,  195,   57,  195,   59,
 /*  5570 */    21,  195,   62,  103,  104,  105,  106,  107,  108,  195,
 /*  5580 */   195,  195,   33,   34,   35,   36,   37,   38,   39,   40,
 /*  5590 */    41,  195,  195,   44,  195,    7,  195,    9,   10,  195,
 /*  5600 */   195,   52,  195,  195,  195,   95,   57,  195,   59,   21,
 /*  5610 */   195,  195,  195,  103,  104,  105,  106,  107,  108,  195,
 /*  5620 */   195,   33,   34,   35,   36,   37,   38,   39,   40,   41,
 /*  5630 */   195,  195,   44,  195,    7,  195,    9,   10,  195,  195,
 /*  5640 */    52,  195,  195,  195,   95,   57,  195,   59,   21,  195,
 /*  5650 */   195,  195,  103,  104,  105,  106,  107,  108,  195,  195,
 /*  5660 */    33,   34,   35,   36,   37,   38,   39,  195,   41,  195,
 /*  5670 */   195,   44,  195,    7,  195,    9,   10,  195,  195,   52,
 /*  5680 */   195,  195,  195,   95,   57,  195,   59,   21,  195,   62,
 /*  5690 */   195,  103,  104,  105,  106,  107,  108,  195,  195,   33,
 /*  5700 */    34,   35,   36,   37,   38,   39,  195,   41,  195,  195,
 /*  5710 */    44,  195,    7,  195,    9,   10,  195,  195,   52,  195,
 /*  5720 */   195,  195,   95,   57,  195,   59,   21,  195,   62,  195,
 /*  5730 */   103,  104,  105,  106,  107,  108,  195,  195,   33,   34,
 /*  5740 */    35,   36,   37,   38,   39,  195,   41,  195,  195,   44,
 /*  5750 */    45,    7,  195,    9,   10,  195,  195,   52,  195,  195,
 /*  5760 */   195,   95,   57,  195,   59,   21,  195,  195,  195,  103,
 /*  5770 */   104,  105,  106,  107,  108,  195,  195,   33,   34,   35,
 /*  5780 */    36,   37,   38,   39,   40,   41,  195,  195,   44,  195,
 /*  5790 */     7,  195,    9,   10,  195,  195,   52,  195,  195,  195,
 /*  5800 */    95,   57,  195,   59,   21,  195,  195,  195,  103,  104,
 /*  5810 */   105,  106,  107,  108,  195,  195,   33,   34,   35,   36,
 /*  5820 */    37,   38,   39,  195,   41,  195,  195,   44,  195,    7,
 /*  5830 */   195,    9,   10,  195,  195,   52,  195,  195,  195,   95,
 /*  5840 */    57,  195,   59,   21,  195,   62,  195,  103,  104,  105,
 /*  5850 */   106,  107,  108,  195,  195,   33,   34,   35,   36,   37,
 /*  5860 */    38,   39,  195,   41,  195,  195,   44,  195,    7,  195,
 /*  5870 */     9,   10,  195,  195,   52,  195,  195,  195,   95,   57,
 /*  5880 */   195,   59,   21,  195,  195,  195,  103,  104,  105,  106,
 /*  5890 */   107,  108,  195,  195,   33,   34,   35,   36,   37,   38,
 /*  5900 */    39,  195,   41,  195,  195,   44,  195,  195,    7,   87,
 /*  5910 */     9,   10,  195,   52,  195,  195,  195,   95,   57,  195,
 /*  5920 */    59,  195,   21,  195,  195,  103,  104,  105,  106,  107,
 /*  5930 */   108,  195,  195,  195,   33,   34,   35,   36,   37,   38,
 /*  5940 */    39,  195,   41,  195,  195,   44,  195,  195,  195,  195,
 /*  5950 */   195,  195,  195,   52,  195,  195,   95,  195,   57,  195,
 /*  5960 */    59,  195,  195,  195,  103,  104,  105,  106,  107,  108,
 /*  5970 */   195,   12,   13,   14,   15,   16,   17,   18,   19,   20,
 /*  5980 */    21,   22,   23,   24,   25,   26,   27,   28,   29,   30,
 /*  5990 */    31,   32,  195,   21,  195,  195,   95,  195,  195,  195,
 /*  6000 */    41,   42,  195,  195,  103,  104,  105,  106,  107,  108,
 /*  6010 */    21,  195,   40,  195,  195,  195,   44,  195,  195,  195,
 /*  6020 */   195,  195,  195,  195,  195,  195,  195,   21,  195,   40,
 /*  6030 */   195,  195,  195,   44,  195,   63,   64,   65,   66,   67,
 /*  6040 */    68,   69,   70,   71,   72,   73,   40,  195,  195,   21,
 /*  6050 */    44,  195,   63,   64,   65,   66,   67,   68,   69,   70,
 /*  6060 */    71,   72,   73,   21,  195,  195,  195,  195,  195,   63,
 /*  6070 */    64,   65,   66,   67,   68,   69,   70,   71,   72,   73,
 /*  6080 */   195,  195,   40,  195,  195,  195,   44,  195,   41,   42,
 /*  6090 */   195,   63,   64,   65,   66,   67,   68,   69,   70,   71,
 /*  6100 */    72,   73,   55,  195,  195,   63,   64,   65,   66,   67,
 /*  6110 */    68,   69,   70,   71,   72,   73,    1,    2,    3,    4,
 /*  6120 */     5,  195,  195,  195,  195,  195,  195,  195,  195,  195,
 /*  6130 */   195,  195,  195,  195,  195,  195,   89,   90,   91,   92,
 /*  6140 */    93,   94,  195,   96,   97,   98,    1,    2,    3,    4,
 /*  6150 */     5,  195,  195,  195,  195,  195,  195,  195,  195,  195,
 /*  6160 */   195,  195,  195,  195,  195,   50,   51,  195,   53,   54,
 /*  6170 */   195,   56,  195,  195,  195,   60,  195,    1,    2,    3,
 /*  6180 */     4,    5,  195,  195,  195,  195,  195,  195,  195,  195,
 /*  6190 */   195,  195,  195,  195,  195,   50,   51,  195,   53,   54,
 /*  6200 */   195,   56,  195,  195,  195,   60,  195,    1,    2,    3,
 /*  6210 */     4,    5,  195,  195,  195,  195,  195,  195,  195,  195,
 /*  6220 */   195,  195,  195,  195,  195,  195,   50,   51,  195,   53,
 /*  6230 */    54,  195,   56,  195,  195,  195,   60,  195,    1,    2,
 /*  6240 */     3,    4,    5,  195,  195,  195,  195,  195,  195,  195,
 /*  6250 */   195,  195,  195,  195,  195,  195,   50,   51,  195,   53,
 /*  6260 */    54,  195,   56,  195,  195,  195,   60,  195,    1,    2,
 /*  6270 */     3,    4,    5,  195,  195,  195,  195,  195,  195,  195,
 /*  6280 */   195,  195,  195,  195,  195,  195,  195,   50,   51,  195,
 /*  6290 */    53,   54,  195,   56,  195,  195,  195,   60,  195,    1,
 /*  6300 */     2,    3,    4,    5,    1,    2,    3,    4,    5,  195,
 /*  6310 */   195,  195,  195,  195,  195,  195,  195,   50,   51,  195,
 /*  6320 */    53,   54,  195,   56,  195,  195,  195,   60,  195,    1,
 /*  6330 */     2,    3,    4,    5,    1,    2,    3,    4,    5,  195,
 /*  6340 */   195,  195,  195,  195,  195,  195,  195,  195,   50,   51,
 /*  6350 */   195,   53,   54,   50,   51,  195,   53,   54,   60,  195,
 /*  6360 */   195,  195,  195,   60,  195,    1,    2,    3,    4,    5,
 /*  6370 */     1,    2,    3,    4,    5,  195,  195,  195,   50,   51,
 /*  6380 */   195,   53,   54,   50,   51,  195,   53,   54,   60,  195,
 /*  6390 */   195,  195,  195,   60,  195,    1,    2,    3,    4,    5,
 /*  6400 */     1,    2,    3,    4,    5,  195,    1,    2,    3,    4,
 /*  6410 */     5,  195,  195,   44,   50,   51,  195,   53,   54,   50,
 /*  6420 */    51,  195,  195,  195,   60,  195,  195,   58,  195,   60,
 /*  6430 */   195,    1,    2,    3,    4,    5,    1,    2,    3,    4,
 /*  6440 */     5,  195,  195,   44,   50,   51,  195,   53,   54,   50,
 /*  6450 */    51,  195,  195,  195,   60,   50,   51,   58,  195,   60,
 /*  6460 */   195,  195,  195,   58,  195,   60,  195,    1,    2,    3,
 /*  6470 */     4,    5,  195,  195,  195,  195,  195,  195,  195,  195,
 /*  6480 */    50,   51,  195,   53,   54,   50,   51,  195,   53,   54,
 /*  6490 */    60,  195,  195,  195,  195,   60,  195,    1,    2,    3,
 /*  6500 */     4,    5,    1,    2,    3,    4,    5,  195,    1,    2,
 /*  6510 */     3,    4,    5,  195,  195,  195,   50,   51,  195,  195,
 /*  6520 */   195,  195,   56,  195,  195,  195,   60,  195,  195,  195,
 /*  6530 */   195,  195,  195,  195,  195,  195,  195,  195,  195,  195,
 /*  6540 */   195,  195,  195,  195,  195,  195,   50,   51,  195,  195,
 /*  6550 */   195,   50,   51,  195,   58,  195,   60,   50,   51,   58,
 /*  6560 */   195,   60,  195,  195,  195,   58,  195,   60,
};
#define YY_SHIFT_USE_DFLT (-53)
static short yy_shift_ofst[] = {
 /*     0 */   579,  939, 3361,  -53,  -53,  -53,  -53,  -53,  -53, 1098,
 /*    10 */   933,  -53, 1037,  542,  -53,  902, 1116,  -53, 6145,  -53,
 /*    20 */   268,  -53, 6115, 6394,  -53, 6405, 1004, 1125, 6006,  353,
 /*    30 */  4883, 1026,  -53,  -53, 1107, 6028,  -53,  829,  -53,  -53,
 /*    40 */   -53,  963,  273,  -53, 1124, 1200,  -53,  -53,  -53,  -53,
 /*    50 */   -53,  -53,  -53,  -53,  -53,  -53,  -53,  -53,  -53,  -53,
 /*    60 */   751,  821, 4883, 1038,  -53,  -53, 5145,  -53, 1017,  -21,
 /*    70 */   -53,  -53,  -53,  -53,  -53,  -53,  -53,  -53,  159,  307,
 /*    80 */   -53,  -53,  901,  -53,  627,  937,  -21,  -53,  938,  525,
 /*    90 */   -21,  -53, 1105,  858,  -21,  -53,  894, 1131,  -53,  -53,
 /*   100 */   -53,  -53,  -53,  -53,  -53,  -53,  -53,  -53, 3543, 6501,
 /*   110 */  1073, 1075, 5972,  380, 4883, 1178,  -53,  -53,  591,  612,
 /*   120 */  4883,  895,  -53,  -53,  -53,  -53,  -53, 6466,  981, 1019,
 /*   130 */   -21, 1106,  -53, 1101,  856,  -21,  830,  -53, 6298,  -53,
 /*   140 */  1096,  244,  -53, 1108,  574,  -53,  419,  336,  -53, 6267,
 /*   150 */   -53, 1002,  -53, 6176, 6435,  -53, 6507, 1058,  991, 6042,
 /*   160 */   617,   96,  -53, 1435,  -53,  -53,  -53,  -53,  -53,  -53,
 /*   170 */   -53,  -53,  -53,  -53,  -53,  -53,  -53,  -53,  -53,  -53,
 /*   180 */   -53,  -53,  -53,  -53,  -53,  -53,  -53,  -53,  -53,  989,
 /*   190 */   -53, 1008,  -53, 5861, 1185, 2671, 1641,  802,  997, 1023,
 /*   200 */  2774,  -53, 5861,  979,  269,  -53,  178,  -53,  -53,  -21,
 /*   210 */   755, 3289, 3289,  638, 3083, 3186,  -53, 5861, 1122, 2362,
 /*   220 */  1950,  -53, 1094, 2568, 1229, 1029, 5861, 1021,  -53, 5861,
 /*   230 */    31, 5861, 5861,   31, 5861, 5861, 5121, 5861, 5861,   31,
 /*   240 */  5861, 5861,   31, 5861, 5861, 5402, 5861, 5861, 5402, 5861,
 /*   250 */    12, 5861,   12,  245,  910, 5588, 5121, 5861, 4628,  -53,
 /*   260 */  5861,  321, 5861,  321, 5861,   31, 5861,   31, 5861,   31,
 /*   270 */  5861,  321, 5861, 4652, 5861, 4652, 5861, 4652, 5861, 4652,
 /*   280 */  5861, 4652, 5861, 4652, 5861, 4652, 5861, 5959, 5861, 5047,
 /*   290 */  5861, 5121, 3392, 4733,  -53,  838, 5861,   31, 1137,  946,
 /*   300 */  5861,   12,  705,   47,  734, 5470,  908,  900, 5861,   31,
 /*   310 */   -53, 5861,   31,  -53,  -53,  -53,  -53,  -53,  -53,  -53,
 /*   320 */   -53, 5783, 5121,   97, 5186,  442, 1129, 5225,  -53,  545,
 /*   330 */   -53, 5861,  702,  783,  491, 5342,  622,  -53,  -53,  -53,
 /*   340 */   -53,  -53,  -53,  648,  940, 5147,  701,  -53,  -53,  998,
 /*   350 */   994, 1006, 5744,  736,  -53,  -53,  -53,  799,  843,  888,
 /*   360 */  5303,  -53,  768,  -53,  613,  -53,  -53,  868,  387,  -53,
 /*   370 */   533,  -53, 5901,  -53,  690, 5861,  -53,  -53,  -53,  598,
 /*   380 */   -53,  -53,  -53,  528,  -53,  -53,  356,  -52, 1156, 5264,
 /*   390 */   459,  -53,  -53, 1120, 1097, 5549,  352,  -53,  -53,   12,
 /*   400 */    12,   12,   12,   12,   12, 5121, 1025,  248, 5822, 4963,
 /*   410 */  3289, 2980,  -53,  142,  162,  -53,  142,  -53, 6047,  -53,
 /*   420 */   -53,  -53,  -53,  -53,  -53,  -53, 5861,  -53, 5121,  599,
 /*   430 */   132, 5861,  -53, 5666,  121, 5861,  -53,  786,  -53, 4900,
 /*   440 */   235, 5861,  -53,  -53, 5861,  -53,  -53,  -53,  742,  964,
 /*   450 */   121, 5861,  -53, 1067,  121, 5861,  -53,  860,  338, 5861,
 /*   460 */   -53, 5510,  121, 5861,  -53,  235, 5861,  -53, 5627,  121,
 /*   470 */  5861,  -53,  235, 5861,  -53,  -53,  -53,  823,   65,  121,
 /*   480 */  5861,  -53,  -53, 5861,  557,  -53, 5861,  -53, 5121,  -53,
 /*   490 */   973,  -53,  926,  -53,  917,  -53,  883,  -53, 5705, 4816,
 /*   500 */   -53,  -53, 5861, 4858,  -53, 5861, 5079,  -53, 5861, 4691,
 /*   510 */   -53,  100,  495,  -53,  100,  -53, 1150,  -21,  -53,  -53,
 /*   520 */   100,  510,  -53,  100,  439,  -53,  100,  376,  -53,  100,
 /*   530 */   354,  -53,  100,  351,  -53,  100,  265,  -53,  100,   83,
 /*   540 */   -53,  100,  804,  -53,  100,  819,  -53, 5121,  -53,  -53,
 /*   550 */   -53, 5861, 5005, 3289,  920,  -53,  144, 1090, 5431, 4586,
 /*   560 */  3289, 1847,  -53, 5861, 4774, 3289,  508,  -53, 1130, 5861,
 /*   570 */  1100,  -53,  -53,  -53,  983, 3289, 3289,  -53,  -53, 1007,
 /*   580 */  1027,  817,  611,  -53,  -53, 4883, 1057, 1538,  -53, 1126,
 /*   590 */   -53,  343,   13,  714,  -53, 1332,  -53, 4883, 1117, 2156,
 /*   600 */   -53, 2465,  -53, 3416, 6496, 1143, 1139, 5989,  484,   -7,
 /*   610 */   -53,  405,  -53, 4883,   75,  199,  -53, 2877,  -53,  529,
 /*   620 */   506, 2259,  -53, 2053,  -53, 4883,  820, 1744,  -53,  302,
 /*   630 */   -53, 6206, 6430,  -53, 6466,  -53, 6466, 6399,  233,  -53,
 /*   640 */   -21,  329,  -53,  931,  -53,   26,  935,  716,  952,  629,
 /*   650 */   -53,  -53,  972,  -53,  -53,  996,  -53, 6369,  130,  -53,
 /*   660 */   -21,  688,  -53, 1055,  -53, 1093,  -53, 6237, 6364, 6333,
 /*   670 */  3416, 6328,  -53, 6303,  419,  -53,  -53,  -53,  419,  336,
 /*   680 */   -53, 1022,  956,  342,  -53, 1060,  -31,  -53,  419,  336,
 /*   690 */   -53, 1172, 1113,  430,  -53,  992,  -31,  -53,  -53,
};
#define YY_REDUCE_USE_DFLT (-155)
static short yy_reduce_ofst[] = {
 /*     0 */  -107, -155,  131, -155, -155, -155, -155, -155, -155, -155,
 /*    10 */  -155, -155, -155,  778, -155, -155,  934, -155, 1012, -155,
 /*    20 */  -155, -155,  428,   16, -155,  809, -155, -155,  615, -155,
 /*    30 */   172, -155, -155, -155, -155,   72, -155, -155, -155, -155,
 /*    40 */  -155, -155, -155, -155, -155, -155, -155, -155, -155, -155,
 /*    50 */  -155, -155, -155, -155, -155, -155, -155, -155, -155, -155,
 /*    60 */  -155, -155, 1217, -155, -155, -155,  -97, -155, -155, 1040,
 /*    70 */  -155, -155, -155, -155, -155, -155, -155, -155, -155, -155,
 /*    80 */  -155, -155, -155, -155, -155, -155,  911, -155, -155, -155,
 /*    90 */   154, -155, -155, -155,  757, -155, -155, -155, -155, -155,
 /*   100 */  -155, -155, -155, -155, -155, -155, -155, -155,  282,  809,
 /*   110 */  -155, -155,  308, -155,  790, -155, -155, -155, -155, -155,
 /*   120 */   378, -155, -155, -155, -155, -155, -155,  282, -155, -155,
 /*   130 */   -42, -155, -155, -155, -155,  745, -155, -155,   16, -155,
 /*   140 */  -155,  674, -155, -155,  773, -155,  -89,  854, -155,  -16,
 /*   150 */  -155, -155, -155,   15,  -18, -155,  809, -155, -155,   44,
 /*   160 */  -155, 3778, -155, 4402, -155, -155, -155, -155, -155, -155,
 /*   170 */  -155, -155, -155, -155, -155, -155, -155, -155, -155, -155,
 /*   180 */  -155, -155, -155, -155, -155, -155, -155, -155, -155, -155,
 /*   190 */  -155, -155, -155, 1136, -155, 3414, 4402, -155, -155, 4350,
 /*   200 */  4402, -155, 1239, -155,  526, -155,  913, -155, -155,  725,
 /*   210 */  -155, 3466, 4402, -155, 4246, 4402, -155, 1548, -155, 3674,
 /*   220 */  4402, -155, -155, 3622, 4402, -155,  250, -155, -155, 3834,
 /*   230 */  -155, 3730, 3678, -155, 3574, 3428, -155, 3196, 3128, -155,
 /*   240 */  2578, 1857, -155, 4094, 4146, -155, 4198, 4250, -155, 4302,
 /*   250 */  -155, 4354, -155, -155, -155,  759, -155, 1754, -155, -155,
 /*   260 */  1960, -155, 2063, -155, 2166, -155, 2269, -155, 1651, -155,
 /*   270 */  2304, -155, 2372, -155, 2475, -155, 2613, -155, 2716, -155,
 /*   280 */  2819, -155, 2887, -155, 2922, -155, 2990, -155, 3025, -155,
 /*   290 */  3093, -155,  147, -155, -155, -155, 3262, -155, -155, -155,
 /*   300 */  3522, -155, -155, -155, -155,  621, -155, -155, 3990, -155,
 /*   310 */  -155, 4042, -155, -155, -155, -155, -155, -155, -155, -155,
 /*   320 */  -155, 3296, -155, -155,  553, -155, -155,  724, -155, -155,
 /*   330 */  -155, 1445, -155, -155, -155,  518, -155, -155, -155, -155,
 /*   340 */  -155, -155, -155, -155, -155,  347, -155, -155, -155, -155,
 /*   350 */  -155, -155,  656, -155, -155, -155, -155, -155, -155, -155,
 /*   360 */   312, -155, -155, -155, -155, -155, -155, -155, -155, -155,
 /*   370 */  -155, -155, 3357, -155, -155, 3470, -155, -155, -155, -155,
 /*   380 */  -155, -155, -155, -155, -155, -155, -155, -155, -155,  415,
 /*   390 */  -155, -155, -155, -155, -155,  451, -155, -155, -155, -155,
 /*   400 */  -155, -155, -155, -155, -155, -155, -155, -155, 3938, -155,
 /*   410 */  3353, 4402, -155,   53, -155, -155, 1000, -155, -154, -155,
 /*   420 */  -155, -155, -155, -155, -155, -155, 1171, -155, -155, -155,
 /*   430 */    63, 1686, -155,  930,  727, 1068, -155, -155, -155, -155,
 /*   440 */   596, 1274, -155, -155,  930, -155, -155, -155, -155, -155,
 /*   450 */   943, 1995, -155, -155,  805, 1789, -155, -155,  -40, 1583,
 /*   460 */  -155,  930,  968, 1377, -155,  586, 1892, -155,  930, 1031,
 /*   470 */  2098, -155,  624, 2201, -155, -155, -155, -155, -155, 1001,
 /*   480 */  1480, -155, -155,  828, -155, -155, 1342, -155, -155, -155,
 /*   490 */  -155, -155, -155, -155, -155, -155, -155, -155, 2407, -155,
 /*   500 */  -155, -155, 2510, -155, -155, 2681, -155, -155, 2784, -155,
 /*   510 */  -155,  340, -155, -155, 1024, -155, -155, 1050, -155, -155,
 /*   520 */   304, -155, -155,  231, -155, -155,  229, -155, -155,  157,
 /*   530 */  -155, -155,   92, -155, -155,   73, -155, -155,  -79, -155,
 /*   540 */  -155,  657, -155, -155,  672, -155, -155, -155, -155, -155,
 /*   550 */  -155, 3626, -155, 3882, 4402, -155, -155, -155, 3782, -155,
 /*   560 */  3830, 4402, -155, 3886, -155, 3986, 4402, -155, -155, 1033,
 /*   570 */  -155, -155, -155, -155, -155, 3934, 4402, -155, -155, -155,
 /*   580 */  -155, 4090, 4402, -155, -155, -128, -155, 3518, -155, 4402,
 /*   590 */  -155, -155, -155, 4038, -155, 4402, -155, 1114, -155, 3570,
 /*   600 */  -155, 4402, -155,  306,  809, -155, -155,  -23, -155, 4194,
 /*   610 */  -155, 4402, -155,  687, -155, 4298, -155, 4402, -155, -155,
 /*   620 */  -155, 3726, -155, 4402, -155,  584, -155, 4142, -155, 4402,
 /*   630 */  -155,  324,  -18, -155,  306, -155,  614,  809,  827, -155,
 /*   640 */   835,  837, -155, -155, -155,  505, -155, -155, -155,  871,
 /*   650 */  -155, -155, -155, -155, -155, -155, -155,  809,  919, -155,
 /*   660 */   941,  945, -155, -155, -155, -155, -155,  909,  530,  -18,
 /*   670 */   614,  -18, -155,  -18,  581, -155, -155, -155,  610,  744,
 /*   680 */  -155, -155, -155, 1034, -155, -155,  845, -155,  432,  752,
 /*   690 */  -155, -155, -155,  974, -155, -155, 1045, -155, -155,
};
static YYACTIONTYPE yy_default[] = {
 /*     0 */  1038, 1038, 1038,  701,  703,  704,  705,  706,  707, 1038,
 /*    10 */  1038,  708, 1038, 1038,  709, 1038, 1038,  710, 1038,  725,
 /*    20 */  1038,  726, 1038, 1038,  759, 1038, 1038, 1038, 1038, 1038,
 /*    30 */  1038, 1038,  779,  797,  798, 1038,  799,  801,  802,  803,
 /*    40 */   804, 1038, 1038,  815, 1038, 1038,  816,  817,  818,  819,
 /*    50 */   820,  821,  822,  823,  824,  825,  826,  827,  800,  783,
 /*    60 */  1038, 1038, 1038, 1038,  780,  784, 1038,  805,  807, 1038,
 /*    70 */   811, 1025, 1026, 1027, 1028, 1029, 1030, 1031, 1038, 1038,
 /*    80 */  1032, 1033, 1038, 1034, 1038,  808, 1038,  812, 1038,  809,
 /*    90 */  1038,  813, 1038,  810, 1038,  814, 1038, 1038,  806,  787,
 /*   100 */   789,  790,  791,  792,  793,  794,  795,  796, 1038, 1038,
 /*   110 */  1038, 1038, 1038, 1038, 1038, 1038,  781,  785, 1038, 1038,
 /*   120 */  1038, 1038,  782,  786,  788,  755,  760, 1038, 1038, 1038,
 /*   130 */  1038, 1038,  761, 1038, 1038, 1038, 1038,  762, 1038,  756,
 /*   140 */  1038, 1038,  711, 1038, 1038,  712, 1038, 1038,  714, 1038,
 /*   150 */   720, 1038,  721, 1038, 1038,  757, 1038, 1038, 1038, 1038,
 /*   160 */  1038, 1038,  763, 1038,  765,  828,  830,  831,  832,  833,
 /*   170 */   834,  835,  836,  837,  838,  839,  840,  841,  842,  843,
 /*   180 */   844,  845,  846,  847,  848,  849,  850,  851,  852, 1038,
 /*   190 */   853, 1038,  854, 1038, 1038, 1038, 1038,  857, 1038, 1038,
 /*   200 */  1038,  858, 1038, 1038, 1038,  861, 1038,  862,  863, 1038,
 /*   210 */  1038,  865,  866, 1038, 1038, 1038,  869, 1038, 1038, 1038,
 /*   220 */  1038,  871, 1038, 1038, 1038, 1038, 1038, 1038,  873, 1038,
 /*   230 */   936, 1038, 1038,  937, 1038, 1038,  938, 1038, 1038,  939,
 /*   240 */  1038, 1038,  940, 1038, 1038,  941, 1038, 1038,  942, 1038,
 /*   250 */   949, 1038,  950, 1038,  954, 1038, 1016, 1038, 1038,  959,
 /*   260 */  1038,  960, 1038,  961, 1038,  962, 1038,  963, 1038,  964,
 /*   270 */  1038,  965, 1038,  966, 1038,  967, 1038,  968, 1038,  969,
 /*   280 */  1038,  970, 1038,  971, 1038,  972, 1038,  973, 1038, 1038,
 /*   290 */  1038, 1013, 1038, 1038,  951, 1038, 1038,  952, 1038, 1038,
 /*   300 */  1038,  953,  977, 1038,  957, 1038, 1038, 1038, 1038,  974,
 /*   310 */   975, 1038,  976,  978,  979,  980,  981,  982,  983,  984,
 /*   320 */   985, 1038, 1024,  977, 1038, 1038,  988, 1038,  989, 1038,
 /*   330 */   990, 1038, 1038, 1038, 1038, 1038, 1038,  996,  997, 1010,
 /*   340 */  1011, 1012, 1015, 1038, 1038, 1038, 1038, 1000, 1001, 1038,
 /*   350 */  1038, 1038, 1038, 1038, 1002, 1003, 1014, 1038, 1038,  991,
 /*   360 */  1038,  992, 1038,  993, 1038,  994,  995,  978,  979,  986,
 /*   370 */  1038,  987, 1038, 1017, 1038, 1038, 1019, 1020, 1018, 1038,
 /*   380 */   998,  999,  958, 1038, 1004, 1005, 1038, 1038,  955, 1038,
 /*   390 */  1038, 1006, 1007, 1038,  956, 1038, 1038, 1008, 1009,  948,
 /*   400 */   947,  946,  945,  944,  943, 1035, 1038, 1038, 1038, 1038,
 /*   410 */  1038, 1038,  874, 1038, 1038,  878, 1038,  879, 1038,  881,
 /*   420 */   882,  883,  884,  885,  886,  887, 1038,  888,  935, 1038,
 /*   430 */  1038, 1038,  889, 1038, 1038, 1038,  892, 1038,  901, 1038,
 /*   440 */  1038, 1038,  893,  899, 1038,  900,  902,  903, 1038, 1038,
 /*   450 */  1038, 1038,  890, 1038, 1038, 1038,  891, 1038, 1038, 1038,
 /*   460 */   894, 1038, 1038, 1038,  895, 1038, 1038,  896, 1038, 1038,
 /*   470 */  1038,  897, 1038, 1038,  898,  904,  905, 1038, 1038, 1038,
 /*   480 */  1038,  906,  880, 1038, 1038,  908, 1038,  909,  911,  910,
 /*   490 */  1010,  912, 1012,  913, 1011,  914,  975,  915, 1038, 1038,
 /*   500 */   916,  917, 1038, 1038,  918, 1038, 1038,  919, 1038, 1038,
 /*   510 */   920, 1038, 1038,  921, 1038,  931,  933, 1038,  934,  932,
 /*   520 */  1038, 1038,  922, 1038, 1038,  923, 1038, 1038,  924, 1038,
 /*   530 */  1038,  925, 1038, 1038,  926, 1038, 1038,  927, 1038, 1038,
 /*   540 */   928, 1038, 1038,  929, 1038, 1038,  930, 1038, 1036, 1037,
 /*   550 */   829, 1038, 1038, 1038, 1038,  875, 1038, 1038, 1038, 1038,
 /*   560 */  1038, 1038,  876, 1038, 1038, 1038, 1038,  877, 1038, 1038,
 /*   570 */  1038,  872,  870,  868, 1038, 1038,  867,  864,  859,  855,
 /*   580 */  1038, 1038, 1038,  860,  856, 1038, 1038, 1038,  771, 1038,
 /*   590 */   773, 1038, 1038, 1038,  764, 1038,  766, 1038, 1038, 1038,
 /*   600 */   772, 1038,  774, 1038, 1038, 1038, 1038, 1038, 1038, 1038,
 /*   610 */   767, 1038,  769, 1038, 1038, 1038,  775, 1038,  777, 1038,
 /*   620 */  1038, 1038,  768, 1038,  770, 1038, 1038, 1038,  776, 1038,
 /*   630 */   778, 1038, 1038,  758, 1038,  739, 1038, 1038, 1038,  741,
 /*   640 */  1038, 1038,  743, 1038,  747, 1038, 1038, 1038, 1038, 1038,
 /*   650 */   751,  753, 1038,  754,  752, 1038,  745, 1038, 1038,  742,
 /*   660 */  1038, 1038,  744, 1038,  748, 1038,  746, 1038, 1038, 1038,
 /*   670 */  1038, 1038,  740, 1038, 1038,  722,  724,  723, 1038, 1038,
 /*   680 */   713, 1038, 1038, 1038,  715, 1038, 1038,  716, 1038, 1038,
 /*   690 */   717, 1038, 1038, 1038,  718, 1038, 1038,  719,  702,
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
  "QUESTION",      "LIKELY",        "UNLIKELY",      "INSTANCEOF",  
  "AND",           "OR",            "BITWISE_OR",    "BITWISE_AND", 
  "BITWISE_XOR",   "BITWISE_SHIFTLEFT",  "BITWISE_SHIFTRIGHT",  "EQUALS",      
  "IDENTICAL",     "LESS",          "GREATER",       "LESSEQUAL",   
  "GREATEREQUAL",  "NOTIDENTICAL",  "NOTEQUALS",     "ADD",         
  "SUB",           "CONCAT",        "MUL",           "DIV",         
  "MOD",           "CLONE",         "EMPTY",         "ISSET",       
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
  "DIVASSIGN",     "CONCATASSIGN",  "MODASSIGN",     "STRING",      
  "DOUBLECOLON",   "INCR",          "DECR",          "ECHO",        
  "RETURN",        "UNSET",         "THROW",         "INTEGER",     
  "CHAR",          "DOUBLE",        "NULL",          "TRUE",        
  "FALSE",         "CBLOCK",        "error",         "program",     
  "xx_language",   "xx_top_statement_list",  "xx_top_statement",  "xx_namespace_def",
  "xx_class_def",  "xx_interface_def",  "xx_comment",    "xx_cblock",   
  "xx_interface_body",  "xx_class_body",  "xx_implements_list",  "xx_class_definition",
  "xx_implements",  "xx_interface_definition",  "xx_class_properties_definition",  "xx_class_consts_definition",
  "xx_class_methods_definition",  "xx_interface_methods_definition",  "xx_class_property_definition",  "xx_visibility_list",
  "xx_literal_expr",  "xx_class_property_shortcuts",  "xx_class_property_shortcuts_list",  "xx_class_property_shortcut",
  "xx_class_const_definition",  "xx_class_method_definition",  "xx_interface_method_definition",  "xx_parameter_list",
  "xx_statement_list",  "xx_method_return_type",  "xx_visibility",  "xx_method_return_type_list",
  "xx_method_return_type_item",  "xx_parameter_type",  "xx_parameter_cast",  "xx_parameter_cast_collection",
  "xx_parameter",  "xx_statement",  "xx_let_statement",  "xx_if_statement",
  "xx_loop_statement",  "xx_echo_statement",  "xx_return_statement",  "xx_require_statement",
  "xx_fetch_statement",  "xx_fcall_statement",  "xx_mcall_statement",  "xx_scall_statement",
  "xx_unset_statement",  "xx_throw_statement",  "xx_declare_statement",  "xx_break_statement",
  "xx_continue_statement",  "xx_while_statement",  "xx_do_while_statement",  "xx_switch_statement",
  "xx_for_statement",  "xx_empty_statement",  "xx_eval_expr",  "xx_case_clauses",
  "xx_case_clause",  "xx_common_expr",  "xx_let_assignments",  "xx_let_assignment",
  "xx_assignment_operator",  "xx_assign_expr",  "xx_array_offset_list",  "xx_array_offset",
  "xx_index_expr",  "xx_echo_expressions",  "xx_echo_expression",  "xx_mcall_expr",
  "xx_fcall_expr",  "xx_scall_expr",  "xx_fetch_expr",  "xx_declare_variable_list",
  "xx_declare_variable",  "xx_array_list",  "xx_call_parameters",  "xx_call_parameter",
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
 /* 137 */ "xx_statement ::= xx_require_statement",
 /* 138 */ "xx_statement ::= xx_fetch_statement",
 /* 139 */ "xx_statement ::= xx_fcall_statement",
 /* 140 */ "xx_statement ::= xx_mcall_statement",
 /* 141 */ "xx_statement ::= xx_scall_statement",
 /* 142 */ "xx_statement ::= xx_unset_statement",
 /* 143 */ "xx_statement ::= xx_throw_statement",
 /* 144 */ "xx_statement ::= xx_declare_statement",
 /* 145 */ "xx_statement ::= xx_break_statement",
 /* 146 */ "xx_statement ::= xx_continue_statement",
 /* 147 */ "xx_statement ::= xx_while_statement",
 /* 148 */ "xx_statement ::= xx_do_while_statement",
 /* 149 */ "xx_statement ::= xx_switch_statement",
 /* 150 */ "xx_statement ::= xx_for_statement",
 /* 151 */ "xx_statement ::= xx_comment",
 /* 152 */ "xx_statement ::= xx_empty_statement",
 /* 153 */ "xx_empty_statement ::= DOTCOMMA",
 /* 154 */ "xx_break_statement ::= BREAK DOTCOMMA",
 /* 155 */ "xx_continue_statement ::= CONTINUE DOTCOMMA",
 /* 156 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /* 157 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE ELSE BRACKET_OPEN BRACKET_CLOSE",
 /* 158 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 159 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE ELSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 160 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE ELSE BRACKET_OPEN BRACKET_CLOSE",
 /* 161 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE ELSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 162 */ "xx_switch_statement ::= SWITCH xx_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /* 163 */ "xx_switch_statement ::= SWITCH xx_eval_expr BRACKET_OPEN xx_case_clauses BRACKET_CLOSE",
 /* 164 */ "xx_case_clauses ::= xx_case_clauses xx_case_clause",
 /* 165 */ "xx_case_clauses ::= xx_case_clause",
 /* 166 */ "xx_case_clause ::= CASE xx_literal_expr COLON",
 /* 167 */ "xx_case_clause ::= CASE xx_literal_expr COLON xx_statement_list",
 /* 168 */ "xx_case_clause ::= DEFAULT COLON xx_statement_list",
 /* 169 */ "xx_loop_statement ::= LOOP BRACKET_OPEN BRACKET_CLOSE",
 /* 170 */ "xx_loop_statement ::= LOOP BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 171 */ "xx_while_statement ::= WHILE xx_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /* 172 */ "xx_while_statement ::= WHILE xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 173 */ "xx_do_while_statement ::= DO BRACKET_OPEN BRACKET_CLOSE WHILE xx_eval_expr DOTCOMMA",
 /* 174 */ "xx_do_while_statement ::= DO BRACKET_OPEN xx_statement_list BRACKET_CLOSE WHILE xx_eval_expr DOTCOMMA",
 /* 175 */ "xx_for_statement ::= FOR IDENTIFIER IN xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 176 */ "xx_for_statement ::= FOR IDENTIFIER IN REVERSE xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 177 */ "xx_for_statement ::= FOR IDENTIFIER COMMA IDENTIFIER IN xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 178 */ "xx_for_statement ::= FOR IDENTIFIER COMMA IDENTIFIER IN REVERSE xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 179 */ "xx_let_statement ::= LET xx_let_assignments DOTCOMMA",
 /* 180 */ "xx_let_assignments ::= xx_let_assignments COMMA xx_let_assignment",
 /* 181 */ "xx_let_assignments ::= xx_let_assignment",
 /* 182 */ "xx_assignment_operator ::= ASSIGN",
 /* 183 */ "xx_assignment_operator ::= ADDASSIGN",
 /* 184 */ "xx_assignment_operator ::= SUBASSIGN",
 /* 185 */ "xx_assignment_operator ::= MULASSIGN",
 /* 186 */ "xx_assignment_operator ::= DIVASSIGN",
 /* 187 */ "xx_assignment_operator ::= CONCATASSIGN",
 /* 188 */ "xx_assignment_operator ::= MODASSIGN",
 /* 189 */ "xx_let_assignment ::= IDENTIFIER xx_assignment_operator xx_assign_expr",
 /* 190 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER xx_assignment_operator xx_assign_expr",
 /* 191 */ "xx_let_assignment ::= IDENTIFIER ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 192 */ "xx_let_assignment ::= IDENTIFIER ARROW BRACKET_OPEN STRING BRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 193 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 194 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER xx_array_offset_list xx_assignment_operator xx_assign_expr",
 /* 195 */ "xx_let_assignment ::= IDENTIFIER DOUBLECOLON IDENTIFIER xx_assignment_operator xx_assign_expr",
 /* 196 */ "xx_let_assignment ::= IDENTIFIER DOUBLECOLON IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 197 */ "xx_let_assignment ::= IDENTIFIER DOUBLECOLON IDENTIFIER xx_array_offset_list xx_assignment_operator xx_assign_expr",
 /* 198 */ "xx_let_assignment ::= IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 199 */ "xx_let_assignment ::= IDENTIFIER xx_array_offset_list xx_assignment_operator xx_assign_expr",
 /* 200 */ "xx_array_offset_list ::= xx_array_offset_list xx_array_offset",
 /* 201 */ "xx_array_offset_list ::= xx_array_offset",
 /* 202 */ "xx_array_offset ::= SBRACKET_OPEN xx_index_expr SBRACKET_CLOSE",
 /* 203 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER INCR",
 /* 204 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER DECR",
 /* 205 */ "xx_let_assignment ::= IDENTIFIER INCR",
 /* 206 */ "xx_let_assignment ::= IDENTIFIER DECR",
 /* 207 */ "xx_let_assignment ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 208 */ "xx_index_expr ::= xx_common_expr",
 /* 209 */ "xx_echo_statement ::= ECHO xx_echo_expressions DOTCOMMA",
 /* 210 */ "xx_echo_expressions ::= xx_echo_expressions COMMA xx_echo_expression",
 /* 211 */ "xx_echo_expressions ::= xx_echo_expression",
 /* 212 */ "xx_echo_expression ::= xx_common_expr",
 /* 213 */ "xx_mcall_statement ::= xx_mcall_expr DOTCOMMA",
 /* 214 */ "xx_fcall_statement ::= xx_fcall_expr DOTCOMMA",
 /* 215 */ "xx_scall_statement ::= xx_scall_expr DOTCOMMA",
 /* 216 */ "xx_fetch_statement ::= xx_fetch_expr DOTCOMMA",
 /* 217 */ "xx_return_statement ::= RETURN xx_common_expr DOTCOMMA",
 /* 218 */ "xx_return_statement ::= RETURN DOTCOMMA",
 /* 219 */ "xx_require_statement ::= REQUIRE xx_common_expr DOTCOMMA",
 /* 220 */ "xx_unset_statement ::= UNSET xx_common_expr DOTCOMMA",
 /* 221 */ "xx_throw_statement ::= THROW xx_common_expr DOTCOMMA",
 /* 222 */ "xx_declare_statement ::= TYPE_INTEGER xx_declare_variable_list DOTCOMMA",
 /* 223 */ "xx_declare_statement ::= TYPE_UINTEGER xx_declare_variable_list DOTCOMMA",
 /* 224 */ "xx_declare_statement ::= TYPE_CHAR xx_declare_variable_list DOTCOMMA",
 /* 225 */ "xx_declare_statement ::= TYPE_UCHAR xx_declare_variable_list DOTCOMMA",
 /* 226 */ "xx_declare_statement ::= TYPE_LONG xx_declare_variable_list DOTCOMMA",
 /* 227 */ "xx_declare_statement ::= TYPE_ULONG xx_declare_variable_list DOTCOMMA",
 /* 228 */ "xx_declare_statement ::= TYPE_DOUBLE xx_declare_variable_list DOTCOMMA",
 /* 229 */ "xx_declare_statement ::= TYPE_STRING xx_declare_variable_list DOTCOMMA",
 /* 230 */ "xx_declare_statement ::= TYPE_BOOL xx_declare_variable_list DOTCOMMA",
 /* 231 */ "xx_declare_statement ::= TYPE_VAR xx_declare_variable_list DOTCOMMA",
 /* 232 */ "xx_declare_variable_list ::= xx_declare_variable_list COMMA xx_declare_variable",
 /* 233 */ "xx_declare_variable_list ::= xx_declare_variable",
 /* 234 */ "xx_declare_variable ::= IDENTIFIER",
 /* 235 */ "xx_declare_variable ::= IDENTIFIER ASSIGN xx_literal_expr",
 /* 236 */ "xx_assign_expr ::= xx_common_expr",
 /* 237 */ "xx_common_expr ::= NOT xx_common_expr",
 /* 238 */ "xx_common_expr ::= ISSET xx_common_expr",
 /* 239 */ "xx_common_expr ::= REQUIRE xx_common_expr",
 /* 240 */ "xx_common_expr ::= CLONE xx_common_expr",
 /* 241 */ "xx_common_expr ::= EMPTY xx_common_expr",
 /* 242 */ "xx_common_expr ::= LIKELY xx_common_expr",
 /* 243 */ "xx_common_expr ::= UNLIKELY xx_common_expr",
 /* 244 */ "xx_common_expr ::= xx_common_expr EQUALS xx_common_expr",
 /* 245 */ "xx_common_expr ::= xx_common_expr NOTEQUALS xx_common_expr",
 /* 246 */ "xx_common_expr ::= xx_common_expr IDENTICAL xx_common_expr",
 /* 247 */ "xx_common_expr ::= xx_common_expr NOTIDENTICAL xx_common_expr",
 /* 248 */ "xx_common_expr ::= xx_common_expr LESS xx_common_expr",
 /* 249 */ "xx_common_expr ::= xx_common_expr GREATER xx_common_expr",
 /* 250 */ "xx_common_expr ::= xx_common_expr LESSEQUAL xx_common_expr",
 /* 251 */ "xx_common_expr ::= xx_common_expr GREATEREQUAL xx_common_expr",
 /* 252 */ "xx_common_expr ::= PARENTHESES_OPEN xx_common_expr PARENTHESES_CLOSE",
 /* 253 */ "xx_common_expr ::= PARENTHESES_OPEN xx_parameter_type PARENTHESES_CLOSE xx_common_expr",
 /* 254 */ "xx_common_expr ::= LESS IDENTIFIER GREATER xx_common_expr",
 /* 255 */ "xx_common_expr ::= xx_common_expr ARROW IDENTIFIER",
 /* 256 */ "xx_common_expr ::= xx_common_expr ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE",
 /* 257 */ "xx_common_expr ::= xx_common_expr ARROW BRACKET_OPEN STRING BRACKET_CLOSE",
 /* 258 */ "xx_common_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER",
 /* 259 */ "xx_common_expr ::= IDENTIFIER DOUBLECOLON CONSTANT",
 /* 260 */ "xx_common_expr ::= xx_common_expr SBRACKET_OPEN xx_common_expr SBRACKET_CLOSE",
 /* 261 */ "xx_common_expr ::= xx_common_expr ADD xx_common_expr",
 /* 262 */ "xx_common_expr ::= xx_common_expr SUB xx_common_expr",
 /* 263 */ "xx_common_expr ::= xx_common_expr MUL xx_common_expr",
 /* 264 */ "xx_common_expr ::= xx_common_expr DIV xx_common_expr",
 /* 265 */ "xx_common_expr ::= xx_common_expr MOD xx_common_expr",
 /* 266 */ "xx_common_expr ::= xx_common_expr CONCAT xx_common_expr",
 /* 267 */ "xx_common_expr ::= xx_common_expr AND xx_common_expr",
 /* 268 */ "xx_common_expr ::= xx_common_expr OR xx_common_expr",
 /* 269 */ "xx_common_expr ::= xx_common_expr BITWISE_AND xx_common_expr",
 /* 270 */ "xx_common_expr ::= xx_common_expr BITWISE_OR xx_common_expr",
 /* 271 */ "xx_common_expr ::= xx_common_expr BITWISE_XOR xx_common_expr",
 /* 272 */ "xx_common_expr ::= xx_common_expr BITWISE_SHIFTLEFT xx_common_expr",
 /* 273 */ "xx_common_expr ::= xx_common_expr BITWISE_SHIFTRIGHT xx_common_expr",
 /* 274 */ "xx_common_expr ::= xx_common_expr INSTANCEOF xx_common_expr",
 /* 275 */ "xx_fetch_expr ::= FETCH IDENTIFIER COMMA xx_common_expr",
 /* 276 */ "xx_common_expr ::= xx_fetch_expr",
 /* 277 */ "xx_common_expr ::= TYPEOF xx_common_expr",
 /* 278 */ "xx_common_expr ::= IDENTIFIER",
 /* 279 */ "xx_common_expr ::= INTEGER",
 /* 280 */ "xx_common_expr ::= STRING",
 /* 281 */ "xx_common_expr ::= CHAR",
 /* 282 */ "xx_common_expr ::= DOUBLE",
 /* 283 */ "xx_common_expr ::= NULL",
 /* 284 */ "xx_common_expr ::= TRUE",
 /* 285 */ "xx_common_expr ::= FALSE",
 /* 286 */ "xx_common_expr ::= CONSTANT",
 /* 287 */ "xx_common_expr ::= SBRACKET_OPEN SBRACKET_CLOSE",
 /* 288 */ "xx_common_expr ::= SBRACKET_OPEN xx_array_list SBRACKET_CLOSE",
 /* 289 */ "xx_common_expr ::= NEW IDENTIFIER",
 /* 290 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 291 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 292 */ "xx_common_expr ::= NEW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE",
 /* 293 */ "xx_common_expr ::= NEW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 294 */ "xx_common_expr ::= NEW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 295 */ "xx_fcall_expr ::= IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 296 */ "xx_fcall_expr ::= IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 297 */ "xx_fcall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 298 */ "xx_fcall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 299 */ "xx_scall_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 300 */ "xx_scall_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 301 */ "xx_scall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE DOUBLECOLON IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 302 */ "xx_scall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE DOUBLECOLON IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 303 */ "xx_scall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE DOUBLECOLON BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 304 */ "xx_scall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE DOUBLECOLON BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 305 */ "xx_mcall_expr ::= xx_common_expr ARROW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 306 */ "xx_mcall_expr ::= xx_common_expr ARROW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 307 */ "xx_mcall_expr ::= xx_common_expr ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 308 */ "xx_mcall_expr ::= xx_common_expr ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 309 */ "xx_mcall_expr ::= xx_common_expr ARROW BRACKET_OPEN STRING BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 310 */ "xx_mcall_expr ::= xx_common_expr ARROW BRACKET_OPEN STRING BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 311 */ "xx_common_expr ::= xx_mcall_expr",
 /* 312 */ "xx_common_expr ::= xx_scall_expr",
 /* 313 */ "xx_common_expr ::= xx_fcall_expr",
 /* 314 */ "xx_common_expr ::= xx_common_expr QUESTION xx_common_expr COLON xx_common_expr",
 /* 315 */ "xx_call_parameters ::= xx_call_parameters COMMA xx_call_parameter",
 /* 316 */ "xx_call_parameters ::= xx_call_parameter",
 /* 317 */ "xx_call_parameter ::= xx_common_expr",
 /* 318 */ "xx_array_list ::= xx_array_list COMMA xx_array_item",
 /* 319 */ "xx_array_list ::= xx_array_item",
 /* 320 */ "xx_array_item ::= xx_array_key COLON xx_array_value",
 /* 321 */ "xx_array_item ::= xx_array_value",
 /* 322 */ "xx_array_key ::= IDENTIFIER",
 /* 323 */ "xx_array_key ::= STRING",
 /* 324 */ "xx_array_key ::= INTEGER",
 /* 325 */ "xx_array_value ::= xx_common_expr",
 /* 326 */ "xx_literal_expr ::= INTEGER",
 /* 327 */ "xx_literal_expr ::= CHAR",
 /* 328 */ "xx_literal_expr ::= STRING",
 /* 329 */ "xx_literal_expr ::= DOUBLE",
 /* 330 */ "xx_literal_expr ::= NULL",
 /* 331 */ "xx_literal_expr ::= FALSE",
 /* 332 */ "xx_literal_expr ::= TRUE",
 /* 333 */ "xx_literal_expr ::= IDENTIFIER DOUBLECOLON CONSTANT",
 /* 334 */ "xx_literal_expr ::= CONSTANT",
 /* 335 */ "xx_literal_expr ::= SBRACKET_OPEN SBRACKET_CLOSE",
 /* 336 */ "xx_eval_expr ::= xx_common_expr",
 /* 337 */ "xx_comment ::= COMMENT",
 /* 338 */ "xx_cblock ::= CBLOCK",
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
    case 105:
    case 106:
    case 107:
    case 108:
    case 109:
// 1036 "parser.lemon"
{
	/*if ((yypminor->yy0)) {
		if ((yypminor->yy0)->free_flag) {
			efree((yypminor->yy0)->token);
		}
		efree((yypminor->yy0));
	}*/
}
// 3265 "parser.c"
      break;
    case 112:
// 1049 "parser.lemon"
{ json_object_put((yypminor->yy25)); }
// 3270 "parser.c"
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
  { 111, 1 },
  { 112, 1 },
  { 113, 2 },
  { 113, 1 },
  { 114, 1 },
  { 114, 1 },
  { 114, 1 },
  { 114, 1 },
  { 114, 1 },
  { 115, 3 },
  { 117, 3 },
  { 117, 5 },
  { 116, 3 },
  { 116, 5 },
  { 116, 5 },
  { 116, 7 },
  { 116, 4 },
  { 116, 6 },
  { 116, 6 },
  { 116, 4 },
  { 116, 6 },
  { 121, 2 },
  { 121, 3 },
  { 122, 3 },
  { 122, 1 },
  { 124, 1 },
  { 120, 2 },
  { 120, 3 },
  { 123, 1 },
  { 123, 1 },
  { 123, 1 },
  { 123, 2 },
  { 123, 2 },
  { 123, 2 },
  { 123, 2 },
  { 123, 3 },
  { 123, 3 },
  { 125, 1 },
  { 125, 1 },
  { 125, 2 },
  { 126, 2 },
  { 126, 1 },
  { 130, 4 },
  { 130, 3 },
  { 130, 6 },
  { 130, 5 },
  { 130, 5 },
  { 130, 4 },
  { 130, 7 },
  { 130, 6 },
  { 133, 2 },
  { 133, 3 },
  { 134, 3 },
  { 134, 1 },
  { 135, 1 },
  { 135, 2 },
  { 127, 2 },
  { 127, 1 },
  { 128, 2 },
  { 128, 1 },
  { 129, 2 },
  { 129, 1 },
  { 136, 6 },
  { 136, 5 },
  { 137, 7 },
  { 137, 8 },
  { 137, 8 },
  { 137, 9 },
  { 137, 8 },
  { 137, 9 },
  { 137, 9 },
  { 137, 10 },
  { 137, 9 },
  { 137, 10 },
  { 137, 10 },
  { 137, 11 },
  { 137, 10 },
  { 137, 11 },
  { 137, 11 },
  { 137, 12 },
  { 138, 8 },
  { 138, 9 },
  { 138, 9 },
  { 138, 10 },
  { 138, 6 },
  { 138, 7 },
  { 138, 7 },
  { 138, 8 },
  { 131, 2 },
  { 131, 1 },
  { 142, 1 },
  { 142, 1 },
  { 142, 1 },
  { 142, 1 },
  { 142, 1 },
  { 142, 1 },
  { 142, 1 },
  { 142, 1 },
  { 141, 1 },
  { 141, 1 },
  { 143, 3 },
  { 143, 1 },
  { 144, 1 },
  { 144, 2 },
  { 144, 1 },
  { 144, 1 },
  { 139, 3 },
  { 139, 1 },
  { 148, 1 },
  { 148, 2 },
  { 148, 3 },
  { 148, 2 },
  { 148, 3 },
  { 148, 4 },
  { 148, 5 },
  { 148, 4 },
  { 146, 3 },
  { 147, 5 },
  { 145, 1 },
  { 145, 1 },
  { 145, 1 },
  { 145, 1 },
  { 145, 1 },
  { 145, 1 },
  { 145, 1 },
  { 145, 1 },
  { 145, 1 },
  { 145, 1 },
  { 145, 1 },
  { 140, 2 },
  { 140, 1 },
  { 149, 1 },
  { 149, 1 },
  { 149, 1 },
  { 149, 1 },
  { 149, 1 },
  { 149, 1 },
  { 149, 1 },
  { 149, 1 },
  { 149, 1 },
  { 149, 1 },
  { 149, 1 },
  { 149, 1 },
  { 149, 1 },
  { 149, 1 },
  { 149, 1 },
  { 149, 1 },
  { 149, 1 },
  { 149, 1 },
  { 149, 1 },
  { 149, 1 },
  { 149, 1 },
  { 149, 1 },
  { 169, 1 },
  { 163, 2 },
  { 164, 2 },
  { 151, 4 },
  { 151, 7 },
  { 151, 5 },
  { 151, 9 },
  { 151, 8 },
  { 151, 8 },
  { 167, 4 },
  { 167, 5 },
  { 171, 2 },
  { 171, 1 },
  { 172, 3 },
  { 172, 4 },
  { 172, 3 },
  { 152, 3 },
  { 152, 4 },
  { 165, 4 },
  { 165, 5 },
  { 166, 6 },
  { 166, 7 },
  { 168, 7 },
  { 168, 8 },
  { 168, 9 },
  { 168, 10 },
  { 150, 3 },
  { 174, 3 },
  { 174, 1 },
  { 176, 1 },
  { 176, 1 },
  { 176, 1 },
  { 176, 1 },
  { 176, 1 },
  { 176, 1 },
  { 176, 1 },
  { 175, 3 },
  { 175, 5 },
  { 175, 7 },
  { 175, 7 },
  { 175, 7 },
  { 175, 6 },
  { 175, 5 },
  { 175, 7 },
  { 175, 6 },
  { 175, 5 },
  { 175, 4 },
  { 178, 2 },
  { 178, 1 },
  { 179, 3 },
  { 175, 4 },
  { 175, 4 },
  { 175, 2 },
  { 175, 2 },
  { 175, 5 },
  { 180, 1 },
  { 153, 3 },
  { 181, 3 },
  { 181, 1 },
  { 182, 1 },
  { 158, 2 },
  { 157, 2 },
  { 159, 2 },
  { 156, 2 },
  { 154, 3 },
  { 154, 2 },
  { 155, 3 },
  { 160, 3 },
  { 161, 3 },
  { 162, 3 },
  { 162, 3 },
  { 162, 3 },
  { 162, 3 },
  { 162, 3 },
  { 162, 3 },
  { 162, 3 },
  { 162, 3 },
  { 162, 3 },
  { 162, 3 },
  { 187, 3 },
  { 187, 1 },
  { 188, 1 },
  { 188, 3 },
  { 177, 1 },
  { 173, 2 },
  { 173, 2 },
  { 173, 2 },
  { 173, 2 },
  { 173, 2 },
  { 173, 2 },
  { 173, 2 },
  { 173, 3 },
  { 173, 3 },
  { 173, 3 },
  { 173, 3 },
  { 173, 3 },
  { 173, 3 },
  { 173, 3 },
  { 173, 3 },
  { 173, 3 },
  { 173, 4 },
  { 173, 4 },
  { 173, 3 },
  { 173, 5 },
  { 173, 5 },
  { 173, 3 },
  { 173, 3 },
  { 173, 4 },
  { 173, 3 },
  { 173, 3 },
  { 173, 3 },
  { 173, 3 },
  { 173, 3 },
  { 173, 3 },
  { 173, 3 },
  { 173, 3 },
  { 173, 3 },
  { 173, 3 },
  { 173, 3 },
  { 173, 3 },
  { 173, 3 },
  { 173, 3 },
  { 186, 4 },
  { 173, 1 },
  { 173, 2 },
  { 173, 1 },
  { 173, 1 },
  { 173, 1 },
  { 173, 1 },
  { 173, 1 },
  { 173, 1 },
  { 173, 1 },
  { 173, 1 },
  { 173, 1 },
  { 173, 2 },
  { 173, 3 },
  { 173, 2 },
  { 173, 4 },
  { 173, 5 },
  { 173, 4 },
  { 173, 6 },
  { 173, 7 },
  { 184, 4 },
  { 184, 3 },
  { 184, 6 },
  { 184, 5 },
  { 185, 6 },
  { 185, 5 },
  { 185, 8 },
  { 185, 7 },
  { 185, 10 },
  { 185, 9 },
  { 183, 6 },
  { 183, 5 },
  { 183, 8 },
  { 183, 7 },
  { 183, 8 },
  { 183, 7 },
  { 173, 1 },
  { 173, 1 },
  { 173, 1 },
  { 173, 5 },
  { 190, 3 },
  { 190, 1 },
  { 191, 1 },
  { 189, 3 },
  { 189, 1 },
  { 192, 3 },
  { 192, 1 },
  { 193, 1 },
  { 193, 1 },
  { 193, 1 },
  { 194, 1 },
  { 132, 1 },
  { 132, 1 },
  { 132, 1 },
  { 132, 1 },
  { 132, 1 },
  { 132, 1 },
  { 132, 1 },
  { 132, 3 },
  { 132, 1 },
  { 132, 2 },
  { 170, 1 },
  { 118, 1 },
  { 119, 1 },
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
// 1045 "parser.lemon"
{
	status->ret = yymsp[0].minor.yy25;
}
// 3826 "parser.c"
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
      case 152:
      case 208:
      case 236:
      case 276:
      case 311:
      case 312:
      case 313:
      case 317:
      case 325:
      case 336:
// 1051 "parser.lemon"
{
	yygotominor.yy25 = yymsp[0].minor.yy25;
}
// 3869 "parser.c"
        break;
      case 2:
      case 40:
      case 56:
      case 58:
      case 60:
      case 88:
      case 129:
      case 164:
      case 200:
// 1055 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_list(yymsp[-1].minor.yy25, yymsp[0].minor.yy25);
}
// 3884 "parser.c"
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
      case 165:
      case 181:
      case 201:
      case 211:
      case 233:
      case 316:
      case 319:
// 1059 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_list(NULL, yymsp[0].minor.yy25);
}
// 3908 "parser.c"
        break;
      case 9:
// 1084 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_namespace(yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 3917 "parser.c"
        break;
      case 10:
// 1088 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_interface(yymsp[-1].minor.yy0, yymsp[0].minor.yy25, NULL, status->scanner_state);
  yy_destructor(46,&yymsp[-2].minor);
}
// 3925 "parser.c"
        break;
      case 11:
// 1092 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_interface(yymsp[-3].minor.yy0, yymsp[0].minor.yy25, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(46,&yymsp[-4].minor);
  yy_destructor(47,&yymsp[-2].minor);
}
// 3934 "parser.c"
        break;
      case 12:
// 1096 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy25, 0, 0, NULL, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-2].minor);
}
// 3942 "parser.c"
        break;
      case 13:
// 1100 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy25, 0, 0, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-4].minor);
  yy_destructor(47,&yymsp[-2].minor);
}
// 3951 "parser.c"
        break;
      case 14:
// 1104 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy25, 0, 0, NULL, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(48,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
}
// 3960 "parser.c"
        break;
      case 15:
// 1108 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class(yymsp[-5].minor.yy0, yymsp[0].minor.yy25, 0, 0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(48,&yymsp[-6].minor);
  yy_destructor(47,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
}
// 3970 "parser.c"
        break;
      case 16:
// 1112 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy25, 1, 0, NULL, NULL, status->scanner_state);
  yy_destructor(50,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-2].minor);
}
// 3979 "parser.c"
        break;
      case 17:
// 1116 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy25, 1, 0, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(50,&yymsp[-5].minor);
  yy_destructor(48,&yymsp[-4].minor);
  yy_destructor(47,&yymsp[-2].minor);
}
// 3989 "parser.c"
        break;
      case 18:
// 1120 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy25, 1, 0, NULL, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(50,&yymsp[-5].minor);
  yy_destructor(48,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
}
// 3999 "parser.c"
        break;
      case 19:
// 1124 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy25, 0, 1, NULL, NULL, status->scanner_state);
  yy_destructor(51,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-2].minor);
}
// 4008 "parser.c"
        break;
      case 20:
// 1128 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy25, 0, 1, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(51,&yymsp[-5].minor);
  yy_destructor(48,&yymsp[-4].minor);
  yy_destructor(47,&yymsp[-2].minor);
}
// 4018 "parser.c"
        break;
      case 21:
      case 50:
// 1132 "parser.lemon"
{
	yygotominor.yy25 = NULL;
  yy_destructor(52,&yymsp[-1].minor);
  yy_destructor(53,&yymsp[0].minor);
}
// 4028 "parser.c"
        break;
      case 22:
      case 51:
// 1136 "parser.lemon"
{
	yygotominor.yy25 = yymsp[-1].minor.yy25;
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(53,&yymsp[0].minor);
}
// 4038 "parser.c"
        break;
      case 23:
      case 52:
      case 106:
      case 180:
      case 210:
      case 232:
      case 315:
      case 318:
// 1140 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_list(yymsp[-2].minor.yy25, yymsp[0].minor.yy25);
  yy_destructor(6,&yymsp[-1].minor);
}
// 4053 "parser.c"
        break;
      case 25:
      case 278:
      case 322:
// 1148 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state);
}
// 4062 "parser.c"
        break;
      case 26:
// 1152 "parser.lemon"
{
  yygotominor.yy25 = NULL;
  yy_destructor(52,&yymsp[-1].minor);
  yy_destructor(53,&yymsp[0].minor);
}
// 4071 "parser.c"
        break;
      case 27:
// 1156 "parser.lemon"
{
  yygotominor.yy25 = yymsp[-1].minor.yy25;
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(53,&yymsp[0].minor);
}
// 4080 "parser.c"
        break;
      case 28:
// 1160 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_definition(yymsp[0].minor.yy25, NULL, NULL, status->scanner_state);
}
// 4087 "parser.c"
        break;
      case 29:
// 1164 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_definition(NULL, NULL, yymsp[0].minor.yy25, status->scanner_state);
}
// 4094 "parser.c"
        break;
      case 30:
// 1168 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_definition(NULL, yymsp[0].minor.yy25, NULL, status->scanner_state);
}
// 4101 "parser.c"
        break;
      case 31:
// 1172 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_definition(yymsp[-1].minor.yy25, yymsp[0].minor.yy25, NULL, status->scanner_state);
}
// 4108 "parser.c"
        break;
      case 32:
// 1176 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_definition(yymsp[-1].minor.yy25, NULL, yymsp[0].minor.yy25, status->scanner_state);
}
// 4115 "parser.c"
        break;
      case 33:
// 1180 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_definition(yymsp[0].minor.yy25, NULL, yymsp[-1].minor.yy25, status->scanner_state);
}
// 4122 "parser.c"
        break;
      case 34:
// 1184 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_definition(NULL, yymsp[0].minor.yy25, yymsp[-1].minor.yy25, status->scanner_state);
}
// 4129 "parser.c"
        break;
      case 35:
// 1188 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_definition(yymsp[-2].minor.yy25, yymsp[0].minor.yy25, yymsp[-1].minor.yy25, status->scanner_state);
}
// 4136 "parser.c"
        break;
      case 36:
// 1192 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_definition(yymsp[-1].minor.yy25, yymsp[0].minor.yy25, yymsp[-2].minor.yy25, status->scanner_state);
}
// 4143 "parser.c"
        break;
      case 37:
// 1196 "parser.lemon"
{
  yygotominor.yy25 = xx_ret_interface_definition(NULL, yymsp[0].minor.yy25, status->scanner_state);
}
// 4150 "parser.c"
        break;
      case 38:
// 1200 "parser.lemon"
{
  yygotominor.yy25 = xx_ret_interface_definition(yymsp[0].minor.yy25, NULL, status->scanner_state);
}
// 4157 "parser.c"
        break;
      case 39:
// 1204 "parser.lemon"
{
  yygotominor.yy25 = xx_ret_interface_definition(yymsp[0].minor.yy25, yymsp[-1].minor.yy25, status->scanner_state);
}
// 4164 "parser.c"
        break;
      case 42:
// 1217 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_property(yymsp[-2].minor.yy25, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(45,&yymsp[0].minor);
}
// 4172 "parser.c"
        break;
      case 43:
// 1221 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_property(yymsp[-2].minor.yy25, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(45,&yymsp[0].minor);
}
// 4180 "parser.c"
        break;
      case 44:
// 1225 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_property(yymsp[-4].minor.yy25, yymsp[-3].minor.yy0, yymsp[-1].minor.yy25, yymsp[-5].minor.yy0, NULL, status->scanner_state);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 4189 "parser.c"
        break;
      case 45:
// 1229 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_property(yymsp[-4].minor.yy25, yymsp[-3].minor.yy0, yymsp[-1].minor.yy25, NULL, NULL, status->scanner_state);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 4198 "parser.c"
        break;
      case 46:
// 1233 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_property(yymsp[-3].minor.yy25, yymsp[-2].minor.yy0, NULL, yymsp[-4].minor.yy0, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(45,&yymsp[0].minor);
}
// 4206 "parser.c"
        break;
      case 47:
// 1237 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_property(yymsp[-3].minor.yy25, yymsp[-2].minor.yy0, NULL, NULL, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(45,&yymsp[0].minor);
}
// 4214 "parser.c"
        break;
      case 48:
// 1241 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_property(yymsp[-5].minor.yy25, yymsp[-4].minor.yy0, yymsp[-2].minor.yy25, yymsp[-6].minor.yy0, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(55,&yymsp[-3].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 4223 "parser.c"
        break;
      case 49:
// 1245 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_property(yymsp[-5].minor.yy25, yymsp[-4].minor.yy0, yymsp[-2].minor.yy25, NULL, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(55,&yymsp[-3].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 4232 "parser.c"
        break;
      case 54:
// 1265 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_property_shortcut(NULL, yymsp[0].minor.yy0, status->scanner_state);
}
// 4239 "parser.c"
        break;
      case 55:
// 1269 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_property_shortcut(yymsp[-1].minor.yy0, yymsp[0].minor.yy0, status->scanner_state);
}
// 4246 "parser.c"
        break;
      case 62:
// 1298 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy25, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(56,&yymsp[-4].minor);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 4256 "parser.c"
        break;
      case 63:
// 1302 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy25, NULL, status->scanner_state);
  yy_destructor(56,&yymsp[-4].minor);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 4266 "parser.c"
        break;
      case 64:
// 1307 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_method(yymsp[-6].minor.yy25, yymsp[-4].minor.yy0, NULL, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(58,&yymsp[-5].minor);
  yy_destructor(59,&yymsp[-3].minor);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(52,&yymsp[-1].minor);
  yy_destructor(53,&yymsp[0].minor);
}
// 4278 "parser.c"
        break;
      case 65:
// 1311 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_method(yymsp[-7].minor.yy25, yymsp[-5].minor.yy0, yymsp[-3].minor.yy25, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(58,&yymsp[-6].minor);
  yy_destructor(59,&yymsp[-4].minor);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(52,&yymsp[-1].minor);
  yy_destructor(53,&yymsp[0].minor);
}
// 4290 "parser.c"
        break;
      case 66:
// 1315 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_method(yymsp[-7].minor.yy25, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy25, NULL, NULL, status->scanner_state);
  yy_destructor(58,&yymsp[-6].minor);
  yy_destructor(59,&yymsp[-4].minor);
  yy_destructor(40,&yymsp[-3].minor);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(53,&yymsp[0].minor);
}
// 4302 "parser.c"
        break;
      case 67:
// 1319 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_method(yymsp[-8].minor.yy25, yymsp[-6].minor.yy0, yymsp[-4].minor.yy25, yymsp[-1].minor.yy25, NULL, NULL, status->scanner_state);
  yy_destructor(58,&yymsp[-7].minor);
  yy_destructor(59,&yymsp[-5].minor);
  yy_destructor(40,&yymsp[-3].minor);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(53,&yymsp[0].minor);
}
// 4314 "parser.c"
        break;
      case 68:
// 1323 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_method(yymsp[-6].minor.yy25, yymsp[-4].minor.yy0, NULL, NULL, yymsp[-7].minor.yy0, NULL, status->scanner_state);
  yy_destructor(58,&yymsp[-5].minor);
  yy_destructor(59,&yymsp[-3].minor);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(52,&yymsp[-1].minor);
  yy_destructor(53,&yymsp[0].minor);
}
// 4326 "parser.c"
        break;
      case 69:
// 1327 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_method(yymsp[-7].minor.yy25, yymsp[-5].minor.yy0, yymsp[-3].minor.yy25, NULL, yymsp[-8].minor.yy0, NULL, status->scanner_state);
  yy_destructor(58,&yymsp[-6].minor);
  yy_destructor(59,&yymsp[-4].minor);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(52,&yymsp[-1].minor);
  yy_destructor(53,&yymsp[0].minor);
}
// 4338 "parser.c"
        break;
      case 70:
// 1331 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_method(yymsp[-7].minor.yy25, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy25, yymsp[-8].minor.yy0, NULL, status->scanner_state);
  yy_destructor(58,&yymsp[-6].minor);
  yy_destructor(59,&yymsp[-4].minor);
  yy_destructor(40,&yymsp[-3].minor);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(53,&yymsp[0].minor);
}
// 4350 "parser.c"
        break;
      case 71:
// 1335 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_method(yymsp[-8].minor.yy25, yymsp[-6].minor.yy0, yymsp[-4].minor.yy25, yymsp[-1].minor.yy25, yymsp[-9].minor.yy0, NULL, status->scanner_state);
  yy_destructor(58,&yymsp[-7].minor);
  yy_destructor(59,&yymsp[-5].minor);
  yy_destructor(40,&yymsp[-3].minor);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(53,&yymsp[0].minor);
}
// 4362 "parser.c"
        break;
      case 72:
// 1339 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_method(yymsp[-8].minor.yy25, yymsp[-6].minor.yy0, NULL, NULL, NULL, yymsp[-2].minor.yy25, status->scanner_state);
  yy_destructor(58,&yymsp[-7].minor);
  yy_destructor(59,&yymsp[-5].minor);
  yy_destructor(40,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-3].minor);
  yy_destructor(52,&yymsp[-1].minor);
  yy_destructor(53,&yymsp[0].minor);
}
// 4375 "parser.c"
        break;
      case 73:
// 1343 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_method(yymsp[-9].minor.yy25, yymsp[-7].minor.yy0, yymsp[-5].minor.yy25, NULL, NULL, yymsp[-2].minor.yy25, status->scanner_state);
  yy_destructor(58,&yymsp[-8].minor);
  yy_destructor(59,&yymsp[-6].minor);
  yy_destructor(40,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-3].minor);
  yy_destructor(52,&yymsp[-1].minor);
  yy_destructor(53,&yymsp[0].minor);
}
// 4388 "parser.c"
        break;
      case 74:
// 1347 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_method(yymsp[-9].minor.yy25, yymsp[-7].minor.yy0, NULL, yymsp[-1].minor.yy25, NULL, yymsp[-3].minor.yy25, status->scanner_state);
  yy_destructor(58,&yymsp[-8].minor);
  yy_destructor(59,&yymsp[-6].minor);
  yy_destructor(40,&yymsp[-5].minor);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(53,&yymsp[0].minor);
}
// 4401 "parser.c"
        break;
      case 75:
// 1351 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_method(yymsp[-10].minor.yy25, yymsp[-8].minor.yy0, yymsp[-6].minor.yy25, yymsp[-1].minor.yy25, NULL, yymsp[-3].minor.yy25, status->scanner_state);
  yy_destructor(58,&yymsp[-9].minor);
  yy_destructor(59,&yymsp[-7].minor);
  yy_destructor(40,&yymsp[-5].minor);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(53,&yymsp[0].minor);
}
// 4414 "parser.c"
        break;
      case 76:
// 1355 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_method(yymsp[-8].minor.yy25, yymsp[-6].minor.yy0, NULL, NULL, yymsp[-9].minor.yy0, yymsp[-2].minor.yy25, status->scanner_state);
  yy_destructor(58,&yymsp[-7].minor);
  yy_destructor(59,&yymsp[-5].minor);
  yy_destructor(40,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-3].minor);
  yy_destructor(52,&yymsp[-1].minor);
  yy_destructor(53,&yymsp[0].minor);
}
// 4427 "parser.c"
        break;
      case 77:
// 1359 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_method(yymsp[-9].minor.yy25, yymsp[-7].minor.yy0, yymsp[-5].minor.yy25, NULL, yymsp[-10].minor.yy0, yymsp[-2].minor.yy25, status->scanner_state);
  yy_destructor(58,&yymsp[-8].minor);
  yy_destructor(59,&yymsp[-6].minor);
  yy_destructor(40,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-3].minor);
  yy_destructor(52,&yymsp[-1].minor);
  yy_destructor(53,&yymsp[0].minor);
}
// 4440 "parser.c"
        break;
      case 78:
// 1363 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_method(yymsp[-9].minor.yy25, yymsp[-7].minor.yy0, NULL, yymsp[-1].minor.yy25, yymsp[-10].minor.yy0, yymsp[-3].minor.yy25, status->scanner_state);
  yy_destructor(58,&yymsp[-8].minor);
  yy_destructor(59,&yymsp[-6].minor);
  yy_destructor(40,&yymsp[-5].minor);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(53,&yymsp[0].minor);
}
// 4453 "parser.c"
        break;
      case 79:
// 1367 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_method(yymsp[-10].minor.yy25, yymsp[-8].minor.yy0, yymsp[-6].minor.yy25, yymsp[-1].minor.yy25, yymsp[-11].minor.yy0, yymsp[-3].minor.yy25, status->scanner_state);
  yy_destructor(58,&yymsp[-9].minor);
  yy_destructor(59,&yymsp[-7].minor);
  yy_destructor(40,&yymsp[-5].minor);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(53,&yymsp[0].minor);
}
// 4466 "parser.c"
        break;
      case 80:
// 1372 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_method(yymsp[-7].minor.yy25, yymsp[-5].minor.yy0, NULL, NULL, NULL, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(58,&yymsp[-6].minor);
  yy_destructor(59,&yymsp[-4].minor);
  yy_destructor(40,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 4478 "parser.c"
        break;
      case 81:
// 1376 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_method(yymsp[-8].minor.yy25, yymsp[-6].minor.yy0, yymsp[-4].minor.yy25, NULL, NULL, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(58,&yymsp[-7].minor);
  yy_destructor(59,&yymsp[-5].minor);
  yy_destructor(40,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 4490 "parser.c"
        break;
      case 82:
// 1380 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_method(yymsp[-7].minor.yy25, yymsp[-5].minor.yy0, NULL, NULL, yymsp[-8].minor.yy0, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(58,&yymsp[-6].minor);
  yy_destructor(59,&yymsp[-4].minor);
  yy_destructor(40,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 4502 "parser.c"
        break;
      case 83:
// 1384 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_method(yymsp[-8].minor.yy25, yymsp[-6].minor.yy0, yymsp[-4].minor.yy25, NULL, yymsp[-9].minor.yy0, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(58,&yymsp[-7].minor);
  yy_destructor(59,&yymsp[-5].minor);
  yy_destructor(40,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 4514 "parser.c"
        break;
      case 84:
// 1388 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_method(yymsp[-5].minor.yy25, yymsp[-3].minor.yy0, NULL, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(58,&yymsp[-4].minor);
  yy_destructor(59,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[-1].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 4525 "parser.c"
        break;
      case 85:
// 1392 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_method(yymsp[-6].minor.yy25, yymsp[-4].minor.yy0, yymsp[-2].minor.yy25, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(58,&yymsp[-5].minor);
  yy_destructor(59,&yymsp[-3].minor);
  yy_destructor(40,&yymsp[-1].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 4536 "parser.c"
        break;
      case 86:
// 1396 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_method(yymsp[-5].minor.yy25, yymsp[-3].minor.yy0, NULL, NULL, yymsp[-6].minor.yy0, NULL, status->scanner_state);
  yy_destructor(58,&yymsp[-4].minor);
  yy_destructor(59,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[-1].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 4547 "parser.c"
        break;
      case 87:
// 1400 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_method(yymsp[-6].minor.yy25, yymsp[-4].minor.yy0, yymsp[-2].minor.yy25, NULL, yymsp[-7].minor.yy0, NULL, status->scanner_state);
  yy_destructor(58,&yymsp[-5].minor);
  yy_destructor(59,&yymsp[-3].minor);
  yy_destructor(40,&yymsp[-1].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 4558 "parser.c"
        break;
      case 90:
// 1413 "parser.lemon"
{
	yygotominor.yy25 = json_object_new_string("public");
  yy_destructor(1,&yymsp[0].minor);
}
// 4566 "parser.c"
        break;
      case 91:
// 1417 "parser.lemon"
{
	yygotominor.yy25 = json_object_new_string("protected");
  yy_destructor(2,&yymsp[0].minor);
}
// 4574 "parser.c"
        break;
      case 92:
// 1421 "parser.lemon"
{
	yygotominor.yy25 = json_object_new_string("private");
  yy_destructor(4,&yymsp[0].minor);
}
// 4582 "parser.c"
        break;
      case 93:
// 1425 "parser.lemon"
{
	yygotominor.yy25 = json_object_new_string("static");
  yy_destructor(3,&yymsp[0].minor);
}
// 4590 "parser.c"
        break;
      case 94:
// 1429 "parser.lemon"
{
	yygotominor.yy25 = json_object_new_string("scoped");
  yy_destructor(5,&yymsp[0].minor);
}
// 4598 "parser.c"
        break;
      case 95:
// 1433 "parser.lemon"
{
	yygotominor.yy25 = json_object_new_string("inline");
  yy_destructor(60,&yymsp[0].minor);
}
// 4606 "parser.c"
        break;
      case 96:
// 1437 "parser.lemon"
{
	yygotominor.yy25 = json_object_new_string("abstract");
  yy_destructor(50,&yymsp[0].minor);
}
// 4614 "parser.c"
        break;
      case 97:
// 1441 "parser.lemon"
{
	yygotominor.yy25 = json_object_new_string("final");
  yy_destructor(51,&yymsp[0].minor);
}
// 4622 "parser.c"
        break;
      case 98:
// 1446 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_return_type(1, NULL, status->scanner_state);
  yy_destructor(61,&yymsp[0].minor);
}
// 4630 "parser.c"
        break;
      case 99:
// 1450 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_return_type(0, yymsp[0].minor.yy25, status->scanner_state);
}
// 4637 "parser.c"
        break;
      case 100:
// 1454 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_list(yymsp[-2].minor.yy25, yymsp[0].minor.yy25);
  yy_destructor(14,&yymsp[-1].minor);
}
// 4645 "parser.c"
        break;
      case 102:
// 1462 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_return_type_item(yymsp[0].minor.yy25, NULL, 0, 0, status->scanner_state);
}
// 4652 "parser.c"
        break;
      case 103:
// 1466 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_return_type_item(yymsp[-1].minor.yy25, NULL, 1, 0, status->scanner_state);
  yy_destructor(39,&yymsp[0].minor);
}
// 4660 "parser.c"
        break;
      case 104:
// 1470 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_return_type_item(NULL, yymsp[0].minor.yy25, 0, 0, status->scanner_state);
}
// 4667 "parser.c"
        break;
      case 105:
// 1474 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_return_type_item(NULL, yymsp[0].minor.yy25, 0, 1, status->scanner_state);
}
// 4674 "parser.c"
        break;
      case 108:
// 1487 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_parameter(NULL, NULL, yymsp[0].minor.yy0, NULL, 0, status->scanner_state);
}
// 4681 "parser.c"
        break;
      case 109:
// 1491 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_parameter(yymsp[-1].minor.yy25, NULL, yymsp[0].minor.yy0, NULL, 0, status->scanner_state);
}
// 4688 "parser.c"
        break;
      case 110:
// 1495 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_parameter(yymsp[-2].minor.yy25, NULL, yymsp[0].minor.yy0, NULL, 1, status->scanner_state);
  yy_destructor(39,&yymsp[-1].minor);
}
// 4696 "parser.c"
        break;
      case 111:
// 1499 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_parameter(NULL, yymsp[-1].minor.yy25, yymsp[0].minor.yy0, NULL, 0, status->scanner_state);
}
// 4703 "parser.c"
        break;
      case 112:
// 1503 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_parameter(NULL, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy25, 0, status->scanner_state);
  yy_destructor(55,&yymsp[-1].minor);
}
// 4711 "parser.c"
        break;
      case 113:
// 1507 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_parameter(yymsp[-3].minor.yy25, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy25, 0, status->scanner_state);
  yy_destructor(55,&yymsp[-1].minor);
}
// 4719 "parser.c"
        break;
      case 114:
// 1511 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_parameter(yymsp[-4].minor.yy25, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy25, 1, status->scanner_state);
  yy_destructor(39,&yymsp[-3].minor);
  yy_destructor(55,&yymsp[-1].minor);
}
// 4728 "parser.c"
        break;
      case 115:
// 1515 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_parameter(NULL, yymsp[-3].minor.yy25, yymsp[-2].minor.yy0, yymsp[0].minor.yy25, 0, status->scanner_state);
  yy_destructor(55,&yymsp[-1].minor);
}
// 4736 "parser.c"
        break;
      case 116:
// 1519 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 4745 "parser.c"
        break;
      case 117:
// 1523 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(21,&yymsp[-4].minor);
  yy_destructor(41,&yymsp[-2].minor);
  yy_destructor(62,&yymsp[-1].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 4756 "parser.c"
        break;
      case 118:
// 1527 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_type(XX_TYPE_INTEGER);
  yy_destructor(63,&yymsp[0].minor);
}
// 4764 "parser.c"
        break;
      case 119:
// 1531 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_type(XX_TYPE_UINTEGER);
  yy_destructor(64,&yymsp[0].minor);
}
// 4772 "parser.c"
        break;
      case 120:
// 1535 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_type(XX_TYPE_LONG);
  yy_destructor(65,&yymsp[0].minor);
}
// 4780 "parser.c"
        break;
      case 121:
// 1539 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_type(XX_TYPE_ULONG);
  yy_destructor(66,&yymsp[0].minor);
}
// 4788 "parser.c"
        break;
      case 122:
// 1543 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_type(XX_TYPE_CHAR);
  yy_destructor(67,&yymsp[0].minor);
}
// 4796 "parser.c"
        break;
      case 123:
// 1547 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_type(XX_TYPE_UCHAR);
  yy_destructor(68,&yymsp[0].minor);
}
// 4804 "parser.c"
        break;
      case 124:
// 1551 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_type(XX_TYPE_DOUBLE);
  yy_destructor(69,&yymsp[0].minor);
}
// 4812 "parser.c"
        break;
      case 125:
// 1555 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_type(XX_TYPE_BOOL);
  yy_destructor(70,&yymsp[0].minor);
}
// 4820 "parser.c"
        break;
      case 126:
// 1559 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_type(XX_TYPE_STRING);
  yy_destructor(71,&yymsp[0].minor);
}
// 4828 "parser.c"
        break;
      case 127:
// 1563 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_type(XX_TYPE_ARRAY);
  yy_destructor(72,&yymsp[0].minor);
}
// 4836 "parser.c"
        break;
      case 128:
// 1567 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_type(XX_TYPE_VAR);
  yy_destructor(73,&yymsp[0].minor);
}
// 4844 "parser.c"
        break;
      case 153:
// 1669 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_empty_statement(status->scanner_state);
  yy_destructor(45,&yymsp[0].minor);
}
// 4852 "parser.c"
        break;
      case 154:
// 1673 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_break_statement(status->scanner_state);
  yy_destructor(74,&yymsp[-1].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 4861 "parser.c"
        break;
      case 155:
// 1677 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_continue_statement(status->scanner_state);
  yy_destructor(75,&yymsp[-1].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 4870 "parser.c"
        break;
      case 156:
// 1681 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_if_statement(yymsp[-2].minor.yy25, NULL, NULL, status->scanner_state);
  yy_destructor(76,&yymsp[-3].minor);
  yy_destructor(52,&yymsp[-1].minor);
  yy_destructor(53,&yymsp[0].minor);
}
// 4880 "parser.c"
        break;
      case 157:
// 1685 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_if_statement(yymsp[-5].minor.yy25, NULL, NULL, status->scanner_state);
  yy_destructor(76,&yymsp[-6].minor);
  yy_destructor(52,&yymsp[-4].minor);
  yy_destructor(53,&yymsp[-3].minor);
  yy_destructor(77,&yymsp[-2].minor);
  yy_destructor(52,&yymsp[-1].minor);
  yy_destructor(53,&yymsp[0].minor);
}
// 4893 "parser.c"
        break;
      case 158:
// 1689 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_if_statement(yymsp[-3].minor.yy25, yymsp[-1].minor.yy25, NULL, status->scanner_state);
  yy_destructor(76,&yymsp[-4].minor);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(53,&yymsp[0].minor);
}
// 4903 "parser.c"
        break;
      case 159:
// 1693 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_if_statement(yymsp[-7].minor.yy25, yymsp[-5].minor.yy25, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(76,&yymsp[-8].minor);
  yy_destructor(52,&yymsp[-6].minor);
  yy_destructor(53,&yymsp[-4].minor);
  yy_destructor(77,&yymsp[-3].minor);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(53,&yymsp[0].minor);
}
// 4916 "parser.c"
        break;
      case 160:
// 1697 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_if_statement(yymsp[-6].minor.yy25, yymsp[-4].minor.yy25, NULL, status->scanner_state);
  yy_destructor(76,&yymsp[-7].minor);
  yy_destructor(52,&yymsp[-5].minor);
  yy_destructor(53,&yymsp[-3].minor);
  yy_destructor(77,&yymsp[-2].minor);
  yy_destructor(52,&yymsp[-1].minor);
  yy_destructor(53,&yymsp[0].minor);
}
// 4929 "parser.c"
        break;
      case 161:
// 1701 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_if_statement(yymsp[-6].minor.yy25, NULL, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(76,&yymsp[-7].minor);
  yy_destructor(52,&yymsp[-5].minor);
  yy_destructor(53,&yymsp[-4].minor);
  yy_destructor(77,&yymsp[-3].minor);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(53,&yymsp[0].minor);
}
// 4942 "parser.c"
        break;
      case 162:
// 1705 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_switch_statement(yymsp[-2].minor.yy25, NULL, status->scanner_state);
  yy_destructor(78,&yymsp[-3].minor);
  yy_destructor(52,&yymsp[-1].minor);
  yy_destructor(53,&yymsp[0].minor);
}
// 4952 "parser.c"
        break;
      case 163:
// 1709 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_switch_statement(yymsp[-3].minor.yy25, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(78,&yymsp[-4].minor);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(53,&yymsp[0].minor);
}
// 4962 "parser.c"
        break;
      case 166:
// 1721 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_case_clause(yymsp[-1].minor.yy25, NULL, status->scanner_state);
  yy_destructor(79,&yymsp[-2].minor);
  yy_destructor(80,&yymsp[0].minor);
}
// 4971 "parser.c"
        break;
      case 167:
// 1725 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_case_clause(yymsp[-2].minor.yy25, yymsp[0].minor.yy25, status->scanner_state);
  yy_destructor(79,&yymsp[-3].minor);
  yy_destructor(80,&yymsp[-1].minor);
}
// 4980 "parser.c"
        break;
      case 168:
// 1729 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_case_clause(NULL, yymsp[0].minor.yy25, status->scanner_state);
  yy_destructor(81,&yymsp[-2].minor);
  yy_destructor(80,&yymsp[-1].minor);
}
// 4989 "parser.c"
        break;
      case 169:
// 1733 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_loop_statement(NULL, status->scanner_state);
  yy_destructor(82,&yymsp[-2].minor);
  yy_destructor(52,&yymsp[-1].minor);
  yy_destructor(53,&yymsp[0].minor);
}
// 4999 "parser.c"
        break;
      case 170:
// 1737 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_loop_statement(yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(82,&yymsp[-3].minor);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(53,&yymsp[0].minor);
}
// 5009 "parser.c"
        break;
      case 171:
// 1741 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_while_statement(yymsp[-2].minor.yy25, NULL, status->scanner_state);
  yy_destructor(83,&yymsp[-3].minor);
  yy_destructor(52,&yymsp[-1].minor);
  yy_destructor(53,&yymsp[0].minor);
}
// 5019 "parser.c"
        break;
      case 172:
// 1745 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_while_statement(yymsp[-3].minor.yy25, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(83,&yymsp[-4].minor);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(53,&yymsp[0].minor);
}
// 5029 "parser.c"
        break;
      case 173:
// 1749 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_do_while_statement(yymsp[-1].minor.yy25, NULL, status->scanner_state);
  yy_destructor(84,&yymsp[-5].minor);
  yy_destructor(52,&yymsp[-4].minor);
  yy_destructor(53,&yymsp[-3].minor);
  yy_destructor(83,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 5041 "parser.c"
        break;
      case 174:
// 1753 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_do_while_statement(yymsp[-1].minor.yy25, yymsp[-4].minor.yy25, status->scanner_state);
  yy_destructor(84,&yymsp[-6].minor);
  yy_destructor(52,&yymsp[-5].minor);
  yy_destructor(53,&yymsp[-3].minor);
  yy_destructor(83,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 5053 "parser.c"
        break;
      case 175:
// 1757 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_for_statement(yymsp[-3].minor.yy25, NULL, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(85,&yymsp[-6].minor);
  yy_destructor(86,&yymsp[-4].minor);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(53,&yymsp[0].minor);
}
// 5064 "parser.c"
        break;
      case 176:
// 1761 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_for_statement(yymsp[-3].minor.yy25, NULL, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(85,&yymsp[-7].minor);
  yy_destructor(86,&yymsp[-5].minor);
  yy_destructor(87,&yymsp[-4].minor);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(53,&yymsp[0].minor);
}
// 5076 "parser.c"
        break;
      case 177:
// 1765 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_for_statement(yymsp[-3].minor.yy25, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(85,&yymsp[-8].minor);
  yy_destructor(6,&yymsp[-6].minor);
  yy_destructor(86,&yymsp[-4].minor);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(53,&yymsp[0].minor);
}
// 5088 "parser.c"
        break;
      case 178:
// 1769 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_for_statement(yymsp[-3].minor.yy25, yymsp[-8].minor.yy0, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(85,&yymsp[-9].minor);
  yy_destructor(6,&yymsp[-7].minor);
  yy_destructor(86,&yymsp[-5].minor);
  yy_destructor(87,&yymsp[-4].minor);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(53,&yymsp[0].minor);
}
// 5101 "parser.c"
        break;
      case 179:
// 1773 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_let_statement(yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(88,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 5110 "parser.c"
        break;
      case 182:
// 1786 "parser.lemon"
{
	yygotominor.yy25 = json_object_new_string("assign");
  yy_destructor(55,&yymsp[0].minor);
}
// 5118 "parser.c"
        break;
      case 183:
// 1791 "parser.lemon"
{
	yygotominor.yy25 = json_object_new_string("add-assign");
  yy_destructor(89,&yymsp[0].minor);
}
// 5126 "parser.c"
        break;
      case 184:
// 1796 "parser.lemon"
{
	yygotominor.yy25 = json_object_new_string("sub-assign");
  yy_destructor(90,&yymsp[0].minor);
}
// 5134 "parser.c"
        break;
      case 185:
// 1800 "parser.lemon"
{
	yygotominor.yy25 = json_object_new_string("mult-assign");
  yy_destructor(91,&yymsp[0].minor);
}
// 5142 "parser.c"
        break;
      case 186:
// 1804 "parser.lemon"
{
	yygotominor.yy25 = json_object_new_string("div-assign");
  yy_destructor(92,&yymsp[0].minor);
}
// 5150 "parser.c"
        break;
      case 187:
// 1808 "parser.lemon"
{
	yygotominor.yy25 = json_object_new_string("concat-assign");
  yy_destructor(93,&yymsp[0].minor);
}
// 5158 "parser.c"
        break;
      case 188:
// 1812 "parser.lemon"
{
	yygotominor.yy25 = json_object_new_string("mod-assign");
  yy_destructor(94,&yymsp[0].minor);
}
// 5166 "parser.c"
        break;
      case 189:
// 1817 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_let_assignment("variable", yymsp[-1].minor.yy25, yymsp[-2].minor.yy0, NULL, NULL, yymsp[0].minor.yy25, status->scanner_state);
}
// 5173 "parser.c"
        break;
      case 190:
// 1822 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_let_assignment("object-property", yymsp[-1].minor.yy25, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy25, status->scanner_state);
  yy_destructor(42,&yymsp[-3].minor);
}
// 5181 "parser.c"
        break;
      case 191:
// 1827 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_let_assignment("variable-dynamic-object-property", yymsp[-1].minor.yy25, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, yymsp[0].minor.yy25, status->scanner_state);
  yy_destructor(42,&yymsp[-5].minor);
  yy_destructor(52,&yymsp[-4].minor);
  yy_destructor(53,&yymsp[-2].minor);
}
// 5191 "parser.c"
        break;
      case 192:
// 1832 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_let_assignment("string-dynamic-object-property", yymsp[-1].minor.yy25, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, yymsp[0].minor.yy25, status->scanner_state);
  yy_destructor(42,&yymsp[-5].minor);
  yy_destructor(52,&yymsp[-4].minor);
  yy_destructor(53,&yymsp[-2].minor);
}
// 5201 "parser.c"
        break;
      case 193:
// 1837 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_let_assignment("object-property-append", yymsp[-1].minor.yy25, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy25, status->scanner_state);
  yy_destructor(42,&yymsp[-5].minor);
  yy_destructor(41,&yymsp[-3].minor);
  yy_destructor(62,&yymsp[-2].minor);
}
// 5211 "parser.c"
        break;
      case 194:
// 1842 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_let_assignment("object-property-array-index", yymsp[-1].minor.yy25, yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-2].minor.yy25, yymsp[0].minor.yy25, status->scanner_state);
  yy_destructor(42,&yymsp[-4].minor);
}
// 5219 "parser.c"
        break;
      case 195:
// 1847 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_let_assignment("static-property", yymsp[-1].minor.yy25, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy25, status->scanner_state);
  yy_destructor(96,&yymsp[-3].minor);
}
// 5227 "parser.c"
        break;
      case 196:
// 1852 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_let_assignment("static-property-append", yymsp[-1].minor.yy25, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy25, status->scanner_state);
  yy_destructor(96,&yymsp[-5].minor);
  yy_destructor(41,&yymsp[-3].minor);
  yy_destructor(62,&yymsp[-2].minor);
}
// 5237 "parser.c"
        break;
      case 197:
// 1857 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_let_assignment("static-property-array-index", yymsp[-1].minor.yy25, yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-2].minor.yy25, yymsp[0].minor.yy25, status->scanner_state);
  yy_destructor(96,&yymsp[-4].minor);
}
// 5245 "parser.c"
        break;
      case 198:
// 1862 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_let_assignment("variable-append", yymsp[-1].minor.yy25, yymsp[-4].minor.yy0, NULL, NULL, yymsp[0].minor.yy25, status->scanner_state);
  yy_destructor(41,&yymsp[-3].minor);
  yy_destructor(62,&yymsp[-2].minor);
}
// 5254 "parser.c"
        break;
      case 199:
// 1867 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_let_assignment("array-index", yymsp[-1].minor.yy25, yymsp[-3].minor.yy0, NULL, yymsp[-2].minor.yy25, yymsp[0].minor.yy25, status->scanner_state);
}
// 5261 "parser.c"
        break;
      case 202:
// 1879 "parser.lemon"
{
	yygotominor.yy25 = yymsp[-1].minor.yy25;
  yy_destructor(41,&yymsp[-2].minor);
  yy_destructor(62,&yymsp[0].minor);
}
// 5270 "parser.c"
        break;
      case 203:
// 1884 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_let_assignment("object-property-incr", NULL, yymsp[-3].minor.yy0, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(97,&yymsp[0].minor);
}
// 5279 "parser.c"
        break;
      case 204:
// 1889 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_let_assignment("object-property-decr", NULL, yymsp[-3].minor.yy0, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(98,&yymsp[0].minor);
}
// 5288 "parser.c"
        break;
      case 205:
// 1894 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_let_assignment("incr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(97,&yymsp[0].minor);
}
// 5296 "parser.c"
        break;
      case 206:
// 1899 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_let_assignment("decr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(98,&yymsp[0].minor);
}
// 5304 "parser.c"
        break;
      case 207:
// 1904 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_let_assignment("dynamic-variable", yymsp[-1].minor.yy25, yymsp[-3].minor.yy0, NULL, NULL, yymsp[0].minor.yy25, status->scanner_state);
  yy_destructor(52,&yymsp[-4].minor);
  yy_destructor(53,&yymsp[-2].minor);
}
// 5313 "parser.c"
        break;
      case 209:
// 1912 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_echo_statement(yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(99,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 5322 "parser.c"
        break;
      case 212:
// 1924 "parser.lemon"
{
	yygotominor.yy25 = yymsp[0].minor.yy25;;
}
// 5329 "parser.c"
        break;
      case 213:
// 1929 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_mcall_statement(yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(45,&yymsp[0].minor);
}
// 5337 "parser.c"
        break;
      case 214:
// 1934 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_fcall_statement(yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(45,&yymsp[0].minor);
}
// 5345 "parser.c"
        break;
      case 215:
// 1939 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_scall_statement(yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(45,&yymsp[0].minor);
}
// 5353 "parser.c"
        break;
      case 216:
// 1944 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_fetch_statement(yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(45,&yymsp[0].minor);
}
// 5361 "parser.c"
        break;
      case 217:
// 1949 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_return_statement(yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(100,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 5370 "parser.c"
        break;
      case 218:
// 1954 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_return_statement(NULL, status->scanner_state);
  yy_destructor(100,&yymsp[-1].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 5379 "parser.c"
        break;
      case 219:
// 1959 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_require_statement(yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(7,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 5388 "parser.c"
        break;
      case 220:
// 1964 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_unset_statement(yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(101,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 5397 "parser.c"
        break;
      case 221:
// 1969 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_throw_exception(yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(102,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 5406 "parser.c"
        break;
      case 222:
// 1973 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_declare_statement(XX_T_TYPE_INTEGER, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(63,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 5415 "parser.c"
        break;
      case 223:
// 1977 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_declare_statement(XX_T_TYPE_UINTEGER, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(64,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 5424 "parser.c"
        break;
      case 224:
// 1981 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_declare_statement(XX_T_TYPE_CHAR, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(67,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 5433 "parser.c"
        break;
      case 225:
// 1985 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_declare_statement(XX_T_TYPE_UCHAR, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(68,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 5442 "parser.c"
        break;
      case 226:
// 1989 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_declare_statement(XX_T_TYPE_LONG, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(65,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 5451 "parser.c"
        break;
      case 227:
// 1993 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_declare_statement(XX_T_TYPE_ULONG, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(66,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 5460 "parser.c"
        break;
      case 228:
// 1997 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_declare_statement(XX_T_TYPE_DOUBLE, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(69,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 5469 "parser.c"
        break;
      case 229:
// 2001 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_declare_statement(XX_T_TYPE_STRING, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(71,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 5478 "parser.c"
        break;
      case 230:
// 2005 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_declare_statement(XX_T_TYPE_BOOL, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(70,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 5487 "parser.c"
        break;
      case 231:
// 2009 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_declare_statement(XX_T_TYPE_VAR, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(73,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 5496 "parser.c"
        break;
      case 234:
// 2021 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_declare_variable(yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 5503 "parser.c"
        break;
      case 235:
// 2025 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_declare_variable(yymsp[-2].minor.yy0, yymsp[0].minor.yy25, status->scanner_state);
  yy_destructor(55,&yymsp[-1].minor);
}
// 5511 "parser.c"
        break;
      case 237:
// 2033 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("not", yymsp[0].minor.yy25, NULL, NULL, status->scanner_state);
  yy_destructor(39,&yymsp[-1].minor);
}
// 5519 "parser.c"
        break;
      case 238:
// 2037 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("isset", yymsp[0].minor.yy25, NULL, NULL, status->scanner_state);
  yy_destructor(35,&yymsp[-1].minor);
}
// 5527 "parser.c"
        break;
      case 239:
// 2041 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("require", yymsp[0].minor.yy25, NULL, NULL, status->scanner_state);
  yy_destructor(7,&yymsp[-1].minor);
}
// 5535 "parser.c"
        break;
      case 240:
// 2045 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("clone", yymsp[0].minor.yy25, NULL, NULL, status->scanner_state);
  yy_destructor(33,&yymsp[-1].minor);
}
// 5543 "parser.c"
        break;
      case 241:
// 2049 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("empty", yymsp[0].minor.yy25, NULL, NULL, status->scanner_state);
  yy_destructor(34,&yymsp[-1].minor);
}
// 5551 "parser.c"
        break;
      case 242:
// 2053 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("likely", yymsp[0].minor.yy25, NULL, NULL, status->scanner_state);
  yy_destructor(9,&yymsp[-1].minor);
}
// 5559 "parser.c"
        break;
      case 243:
// 2057 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("unlikely", yymsp[0].minor.yy25, NULL, NULL, status->scanner_state);
  yy_destructor(10,&yymsp[-1].minor);
}
// 5567 "parser.c"
        break;
      case 244:
// 2061 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("equals", yymsp[-2].minor.yy25, yymsp[0].minor.yy25, NULL, status->scanner_state);
  yy_destructor(19,&yymsp[-1].minor);
}
// 5575 "parser.c"
        break;
      case 245:
// 2065 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("not-equals", yymsp[-2].minor.yy25, yymsp[0].minor.yy25, NULL, status->scanner_state);
  yy_destructor(26,&yymsp[-1].minor);
}
// 5583 "parser.c"
        break;
      case 246:
// 2069 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("identical", yymsp[-2].minor.yy25, yymsp[0].minor.yy25, NULL, status->scanner_state);
  yy_destructor(20,&yymsp[-1].minor);
}
// 5591 "parser.c"
        break;
      case 247:
// 2073 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("not-identical", yymsp[-2].minor.yy25, yymsp[0].minor.yy25, NULL, status->scanner_state);
  yy_destructor(25,&yymsp[-1].minor);
}
// 5599 "parser.c"
        break;
      case 248:
// 2077 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("less", yymsp[-2].minor.yy25, yymsp[0].minor.yy25, NULL, status->scanner_state);
  yy_destructor(21,&yymsp[-1].minor);
}
// 5607 "parser.c"
        break;
      case 249:
// 2081 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("greater", yymsp[-2].minor.yy25, yymsp[0].minor.yy25, NULL, status->scanner_state);
  yy_destructor(22,&yymsp[-1].minor);
}
// 5615 "parser.c"
        break;
      case 250:
// 2085 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("less-equal", yymsp[-2].minor.yy25, yymsp[0].minor.yy25, NULL, status->scanner_state);
  yy_destructor(23,&yymsp[-1].minor);
}
// 5623 "parser.c"
        break;
      case 251:
// 2089 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("greater-equal", yymsp[-2].minor.yy25, yymsp[0].minor.yy25, NULL, status->scanner_state);
  yy_destructor(24,&yymsp[-1].minor);
}
// 5631 "parser.c"
        break;
      case 252:
// 2093 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("list", yymsp[-1].minor.yy25, NULL, NULL, status->scanner_state);
  yy_destructor(59,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 5640 "parser.c"
        break;
      case 253:
// 2097 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("cast", yymsp[-2].minor.yy25, yymsp[0].minor.yy25, NULL, status->scanner_state);
  yy_destructor(59,&yymsp[-3].minor);
  yy_destructor(40,&yymsp[-1].minor);
}
// 5649 "parser.c"
        break;
      case 254:
// 2101 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("type-hint", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy25, NULL, status->scanner_state);
  yy_destructor(21,&yymsp[-3].minor);
  yy_destructor(22,&yymsp[-1].minor);
}
// 5658 "parser.c"
        break;
      case 255:
// 2105 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("property-access", yymsp[-2].minor.yy25, xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(42,&yymsp[-1].minor);
}
// 5666 "parser.c"
        break;
      case 256:
// 2109 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("property-dynamic-access", yymsp[-4].minor.yy25, xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(42,&yymsp[-3].minor);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(53,&yymsp[0].minor);
}
// 5676 "parser.c"
        break;
      case 257:
// 2113 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("property-string-access", yymsp[-4].minor.yy25, xx_ret_literal(XX_T_STRING, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(42,&yymsp[-3].minor);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(53,&yymsp[0].minor);
}
// 5686 "parser.c"
        break;
      case 258:
// 2117 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("static-property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(96,&yymsp[-1].minor);
}
// 5694 "parser.c"
        break;
      case 259:
      case 333:
// 2121 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("static-constant-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(96,&yymsp[-1].minor);
}
// 5703 "parser.c"
        break;
      case 260:
// 2130 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("array-access", yymsp[-3].minor.yy25, yymsp[-1].minor.yy25, NULL, status->scanner_state);
  yy_destructor(41,&yymsp[-2].minor);
  yy_destructor(62,&yymsp[0].minor);
}
// 5712 "parser.c"
        break;
      case 261:
// 2135 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("add", yymsp[-2].minor.yy25, yymsp[0].minor.yy25, NULL, status->scanner_state);
  yy_destructor(27,&yymsp[-1].minor);
}
// 5720 "parser.c"
        break;
      case 262:
// 2140 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("sub", yymsp[-2].minor.yy25, yymsp[0].minor.yy25, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-1].minor);
}
// 5728 "parser.c"
        break;
      case 263:
// 2145 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("mul", yymsp[-2].minor.yy25, yymsp[0].minor.yy25, NULL, status->scanner_state);
  yy_destructor(30,&yymsp[-1].minor);
}
// 5736 "parser.c"
        break;
      case 264:
// 2150 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("div", yymsp[-2].minor.yy25, yymsp[0].minor.yy25, NULL, status->scanner_state);
  yy_destructor(31,&yymsp[-1].minor);
}
// 5744 "parser.c"
        break;
      case 265:
// 2155 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("mod", yymsp[-2].minor.yy25, yymsp[0].minor.yy25, NULL, status->scanner_state);
  yy_destructor(32,&yymsp[-1].minor);
}
// 5752 "parser.c"
        break;
      case 266:
// 2160 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("concat", yymsp[-2].minor.yy25, yymsp[0].minor.yy25, NULL, status->scanner_state);
  yy_destructor(29,&yymsp[-1].minor);
}
// 5760 "parser.c"
        break;
      case 267:
// 2165 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("and", yymsp[-2].minor.yy25, yymsp[0].minor.yy25, NULL, status->scanner_state);
  yy_destructor(12,&yymsp[-1].minor);
}
// 5768 "parser.c"
        break;
      case 268:
// 2170 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("or", yymsp[-2].minor.yy25, yymsp[0].minor.yy25, NULL, status->scanner_state);
  yy_destructor(13,&yymsp[-1].minor);
}
// 5776 "parser.c"
        break;
      case 269:
// 2175 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("bitwise_and", yymsp[-2].minor.yy25, yymsp[0].minor.yy25, NULL, status->scanner_state);
  yy_destructor(15,&yymsp[-1].minor);
}
// 5784 "parser.c"
        break;
      case 270:
// 2180 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("bitwise_or", yymsp[-2].minor.yy25, yymsp[0].minor.yy25, NULL, status->scanner_state);
  yy_destructor(14,&yymsp[-1].minor);
}
// 5792 "parser.c"
        break;
      case 271:
// 2185 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("bitwise_xor", yymsp[-2].minor.yy25, yymsp[0].minor.yy25, NULL, status->scanner_state);
  yy_destructor(16,&yymsp[-1].minor);
}
// 5800 "parser.c"
        break;
      case 272:
// 2190 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("bitwise_shiftleft", yymsp[-2].minor.yy25, yymsp[0].minor.yy25, NULL, status->scanner_state);
  yy_destructor(17,&yymsp[-1].minor);
}
// 5808 "parser.c"
        break;
      case 273:
// 2195 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("bitwise_shiftright", yymsp[-2].minor.yy25, yymsp[0].minor.yy25, NULL, status->scanner_state);
  yy_destructor(18,&yymsp[-1].minor);
}
// 5816 "parser.c"
        break;
      case 274:
// 2200 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("instanceof", yymsp[-2].minor.yy25, yymsp[0].minor.yy25, NULL, status->scanner_state);
  yy_destructor(11,&yymsp[-1].minor);
}
// 5824 "parser.c"
        break;
      case 275:
// 2205 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("fetch", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy25, NULL, status->scanner_state);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(6,&yymsp[-1].minor);
}
// 5833 "parser.c"
        break;
      case 277:
// 2215 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("typeof", yymsp[0].minor.yy25, NULL, NULL, status->scanner_state);
  yy_destructor(37,&yymsp[-1].minor);
}
// 5841 "parser.c"
        break;
      case 279:
      case 324:
      case 326:
// 2225 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_literal(XX_T_INTEGER, yymsp[0].minor.yy0, status->scanner_state);
}
// 5850 "parser.c"
        break;
      case 280:
      case 323:
      case 328:
// 2230 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_literal(XX_T_STRING, yymsp[0].minor.yy0, status->scanner_state);
}
// 5859 "parser.c"
        break;
      case 281:
      case 327:
// 2235 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_literal(XX_T_CHAR, yymsp[0].minor.yy0, status->scanner_state);
}
// 5867 "parser.c"
        break;
      case 282:
      case 329:
// 2240 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_literal(XX_T_DOUBLE, yymsp[0].minor.yy0, status->scanner_state);
}
// 5875 "parser.c"
        break;
      case 283:
      case 330:
// 2245 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_literal(XX_T_NULL, NULL, status->scanner_state);
  yy_destructor(106,&yymsp[0].minor);
}
// 5884 "parser.c"
        break;
      case 284:
      case 332:
// 2250 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_literal(XX_T_TRUE, NULL, status->scanner_state);
  yy_destructor(107,&yymsp[0].minor);
}
// 5893 "parser.c"
        break;
      case 285:
      case 331:
// 2255 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_literal(XX_T_FALSE, NULL, status->scanner_state);
  yy_destructor(108,&yymsp[0].minor);
}
// 5902 "parser.c"
        break;
      case 286:
      case 334:
// 2260 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_literal(XX_T_CONSTANT, yymsp[0].minor.yy0, status->scanner_state);
}
// 5910 "parser.c"
        break;
      case 287:
// 2265 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("empty-array", NULL, NULL, NULL, status->scanner_state);
  yy_destructor(41,&yymsp[-1].minor);
  yy_destructor(62,&yymsp[0].minor);
}
// 5919 "parser.c"
        break;
      case 288:
// 2270 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("array", yymsp[-1].minor.yy25, NULL, NULL, status->scanner_state);
  yy_destructor(41,&yymsp[-2].minor);
  yy_destructor(62,&yymsp[0].minor);
}
// 5928 "parser.c"
        break;
      case 289:
// 2275 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_new_instance(0, yymsp[0].minor.yy0, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-1].minor);
}
// 5936 "parser.c"
        break;
      case 290:
// 2280 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_new_instance(0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(59,&yymsp[-1].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 5946 "parser.c"
        break;
      case 291:
// 2285 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_new_instance(0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(59,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 5956 "parser.c"
        break;
      case 292:
// 2290 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_new_instance(1, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(53,&yymsp[0].minor);
}
// 5966 "parser.c"
        break;
      case 293:
// 2295 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_new_instance(1, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-5].minor);
  yy_destructor(52,&yymsp[-4].minor);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(59,&yymsp[-1].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 5978 "parser.c"
        break;
      case 294:
// 2300 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_new_instance(1, yymsp[-4].minor.yy0, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(38,&yymsp[-6].minor);
  yy_destructor(52,&yymsp[-5].minor);
  yy_destructor(53,&yymsp[-3].minor);
  yy_destructor(59,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 5990 "parser.c"
        break;
      case 295:
// 2305 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_fcall(1, yymsp[-3].minor.yy0, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(59,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 5999 "parser.c"
        break;
      case 296:
// 2310 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_fcall(1, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(59,&yymsp[-1].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 6008 "parser.c"
        break;
      case 297:
// 2315 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_fcall(2, yymsp[-4].minor.yy0, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(52,&yymsp[-5].minor);
  yy_destructor(53,&yymsp[-3].minor);
  yy_destructor(59,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 6019 "parser.c"
        break;
      case 298:
// 2320 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_fcall(2, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(52,&yymsp[-4].minor);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(59,&yymsp[-1].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 6030 "parser.c"
        break;
      case 299:
// 2325 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_scall(0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(96,&yymsp[-4].minor);
  yy_destructor(59,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 6040 "parser.c"
        break;
      case 300:
// 2330 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_scall(0, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(96,&yymsp[-3].minor);
  yy_destructor(59,&yymsp[-1].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 6050 "parser.c"
        break;
      case 301:
// 2335 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_scall(1, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(52,&yymsp[-7].minor);
  yy_destructor(53,&yymsp[-5].minor);
  yy_destructor(96,&yymsp[-4].minor);
  yy_destructor(59,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 6062 "parser.c"
        break;
      case 302:
// 2340 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_scall(1, yymsp[-5].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(52,&yymsp[-6].minor);
  yy_destructor(53,&yymsp[-4].minor);
  yy_destructor(96,&yymsp[-3].minor);
  yy_destructor(59,&yymsp[-1].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 6074 "parser.c"
        break;
      case 303:
// 2345 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_scall(1, yymsp[-8].minor.yy0, yymsp[-4].minor.yy0, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(52,&yymsp[-9].minor);
  yy_destructor(53,&yymsp[-7].minor);
  yy_destructor(96,&yymsp[-6].minor);
  yy_destructor(52,&yymsp[-5].minor);
  yy_destructor(53,&yymsp[-3].minor);
  yy_destructor(59,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 6088 "parser.c"
        break;
      case 304:
// 2350 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_scall(1, yymsp[-7].minor.yy0, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(52,&yymsp[-8].minor);
  yy_destructor(53,&yymsp[-6].minor);
  yy_destructor(96,&yymsp[-5].minor);
  yy_destructor(52,&yymsp[-4].minor);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(59,&yymsp[-1].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 6102 "parser.c"
        break;
      case 305:
// 2355 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_mcall(1, yymsp[-5].minor.yy25, yymsp[-3].minor.yy0, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(59,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 6112 "parser.c"
        break;
      case 306:
// 2360 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_mcall(1, yymsp[-4].minor.yy25, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(42,&yymsp[-3].minor);
  yy_destructor(59,&yymsp[-1].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 6122 "parser.c"
        break;
      case 307:
// 2365 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_mcall(2, yymsp[-7].minor.yy25, yymsp[-4].minor.yy0, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(42,&yymsp[-6].minor);
  yy_destructor(52,&yymsp[-5].minor);
  yy_destructor(53,&yymsp[-3].minor);
  yy_destructor(59,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 6134 "parser.c"
        break;
      case 308:
// 2370 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_mcall(2, yymsp[-6].minor.yy25, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(42,&yymsp[-5].minor);
  yy_destructor(52,&yymsp[-4].minor);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(59,&yymsp[-1].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 6146 "parser.c"
        break;
      case 309:
// 2375 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_mcall(3, yymsp[-7].minor.yy25, yymsp[-4].minor.yy0, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(42,&yymsp[-6].minor);
  yy_destructor(52,&yymsp[-5].minor);
  yy_destructor(53,&yymsp[-3].minor);
  yy_destructor(59,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 6158 "parser.c"
        break;
      case 310:
// 2380 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_mcall(3, yymsp[-6].minor.yy25, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(42,&yymsp[-5].minor);
  yy_destructor(52,&yymsp[-4].minor);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(59,&yymsp[-1].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 6170 "parser.c"
        break;
      case 314:
// 2400 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("ternary", yymsp[-4].minor.yy25, yymsp[-2].minor.yy25, yymsp[0].minor.yy25, status->scanner_state);
  yy_destructor(8,&yymsp[-3].minor);
  yy_destructor(80,&yymsp[-1].minor);
}
// 6179 "parser.c"
        break;
      case 320:
// 2424 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_array_item(yymsp[-2].minor.yy25, yymsp[0].minor.yy25, status->scanner_state);
  yy_destructor(80,&yymsp[-1].minor);
}
// 6187 "parser.c"
        break;
      case 321:
// 2428 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_array_item(NULL, yymsp[0].minor.yy25, status->scanner_state);
}
// 6194 "parser.c"
        break;
      case 335:
// 2484 "parser.lemon"
{
  yygotominor.yy25 = xx_ret_expr("empty-array", NULL, NULL, NULL, status->scanner_state);
  yy_destructor(41,&yymsp[-1].minor);
  yy_destructor(62,&yymsp[0].minor);
}
// 6203 "parser.c"
        break;
      case 337:
// 2492 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_comment(yymsp[0].minor.yy0, status->scanner_state);
}
// 6210 "parser.c"
        break;
      case 338:
// 2496 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_cblock(yymsp[0].minor.yy0, status->scanner_state);
}
// 6217 "parser.c"
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
// 1002 "parser.lemon"


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

// 6293 "parser.c"
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
 | Copyright (c) 2013-2014 Zephir Team and contributors                          |
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
			case XX_T_DIVASSIGN:
				xx_(xx_parser, XX_DIVASSIGN, NULL, parser_status);
				break;
			case XX_T_MULASSIGN:
				xx_(xx_parser, XX_MULASSIGN, NULL, parser_status);
				break;
			case XX_T_CONCATASSIGN:
				xx_(xx_parser, XX_CONCATASSIGN, NULL, parser_status);
				break;
			case XX_T_MODASSIGN:
				xx_(xx_parser, XX_MODASSIGN, NULL, parser_status);
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
			case XX_T_QUESTION:
				xx_(xx_parser, XX_QUESTION, NULL, parser_status);
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
			case XX_T_BITWISE_XOR:
				xx_(xx_parser, XX_BITWISE_XOR, NULL, parser_status);
				break;
			case XX_T_BITWISE_SHIFTLEFT:
				xx_(xx_parser, XX_BITWISE_SHIFTLEFT, NULL, parser_status);
				break;
			case XX_T_BITWISE_SHIFTRIGHT:
				xx_(xx_parser, XX_BITWISE_SHIFTRIGHT, NULL, parser_status);
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
