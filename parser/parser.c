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

static json_object *xx_ret_parameter(int const_param, json_object *type, json_object *cast, xx_parser_token *N, json_object *default_value,
	int mandatory, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("parameter"));
	json_object_object_add(ret, "name", json_object_new_string(N->token));
	json_object_object_add(ret, "const", json_object_new_int(const_param));

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


// 958 "parser.c"
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
#define YYNSTATE 715
#define YYNRULE 347
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
 /*     0 */   518,   66,  260,  263, 1063,    1,    2,  714,    4,    5,
 /*    10 */     6,    7,    8,  642,  314,   34,   58,   37,   39,   40,
 /*    20 */    82,   66,  447,   78,  456,  461,  254,  257,  248,  322,
 /*    30 */   327,  341,  245,  502,  337,  135,   81,  318,  204,  276,
 /*    40 */   278,  286,  280,  282,  284,  348,  644,  564,  102,  110,
 /*    50 */   336,   67,  308,  273,  269,   61,  527,  536,  545,  548,
 /*    60 */   539,  542,  551,  557,  554,  342,  560,  205,  207,  209,
 /*    70 */   667,  218,  501,  373,   73,  229,  233,  238,  422,  662,
 /*    80 */   668,  429,   71,   72,   74,   75,   77,   76,  330,  273,
 /*    90 */   269,  320,  499,  514,  521,  524,  329,  331,  332,  333,
 /*   100 */   334,  335,  565,  518,  398,  260,  263,  167,  435,   90,
 /*   110 */   169,  683,  689,  172,  688,  673,  134,  314,  530,  171,
 /*   120 */   155,  649,  102,  110,  140,  114,  140,  572,  221,  254,
 /*   130 */   257,  248,  322,  327,  341,  245,  442,  337,  488,  461,
 /*   140 */   318,  204,  436,  437,  438,  439,  440,  441,  348,  610,
 /*   150 */   564,  658,  715,  336,  225,  308,  590,  529,  661,  527,
 /*   160 */   536,  545,  548,  539,  542,  551,  557,  554,  465,  560,
 /*   170 */   205,  207,  209,  449,  218,  125,  635,   66,  229,  233,
 /*   180 */   238,  422,  102,  110,  429,  114,  140,  435,  701,  482,
 /*   190 */   704,  330,  459,  165,   11,  499,  514,  521,  524,  329,
 /*   200 */   331,  332,  333,  334,  335,  565,  518,  424,  260,  263,
 /*   210 */   630,  608,   34,   58,   37,   39,   40,  663,  670,  469,
 /*   220 */   314,  436,  437,  438,  439,  440,  441,   10,  136,  462,
 /*   230 */   463,  139,  254,  257,  248,  322,  327,  341,  245,  475,
 /*   240 */   337,  481,  461,  318,  204,  647,  170,  130,  651,  673,
 /*   250 */   133,  348,  640,  564,  155,  649,  336,  705,  308,  693,
 /*   260 */   140,  667,  527,  536,  545,  548,  539,  542,  551,  557,
 /*   270 */   554,  668,  560,  205,  207,  209,  477,  218,  528,  535,
 /*   280 */   530,  229,  233,  238,  422,   25,  159,  429,  694,  388,
 /*   290 */   435,  165,   24,   13,  330,   14,  140,  340,  499,  514,
 /*   300 */   521,  524,  329,  331,  332,  333,  334,  335,  565,  518,
 /*   310 */   489,  260,  263,  459,  690,  665,  223,  234, 1045,  538,
 /*   320 */   421,  222,  593,  314,  436,  437,  438,  439,  440,  441,
 /*   330 */   355,  357,  356,  326,  319,  254,  257,  248,  322,  327,
 /*   340 */   341,  245,  225,  337,  590,  387,  318,  204,    3,    4,
 /*   350 */     5,    6,    7,    8,  348,  618,  564,  530,  106,  336,
 /*   360 */   165,  308,  664,  103,  613,  527,  536,  545,  548,  539,
 /*   370 */   542,  551,  557,  554,  609,  560,  205,  207,  209,  460,
 /*   380 */   218,  675,  655,  530,  229,  233,  238,  422,  661,  661,
 /*   390 */   429,  676,  656,  435,  537,  535,  541,  330,  540,  535,
 /*   400 */   340,  499,  514,  521,  524,  329,  331,  332,  333,  334,
 /*   410 */   335,  565,  518,  270,  260,  263,  543,  535,  546,  535,
 /*   420 */   243,  402,  544,  421,  530,   16,  314,  436,  437,  438,
 /*   430 */   439,  440,  441,  355,  357,  356,  326,  319,  254,  257,
 /*   440 */   248,  322,  327,  341,  245,  530,  337,  549,  535,  318,
 /*   450 */   204,  552,  535,  678,  280,  282,  284,  348,  583,  564,
 /*   460 */   661,   18,  336,  547,  308,  273,  269,   66,  527,  536,
 /*   470 */   545,  548,  539,  542,  551,  557,  554,  530,  560,  205,
 /*   480 */   207,  209,  530,  218,  550,  272,   17,  229,  233,  238,
 /*   490 */   422,  555,  535,  429,   92,  355,  357,  356,  326,   89,
 /*   500 */   330,  636,  380,  358,  499,  514,  521,  524,  329,  331,
 /*   510 */   332,  333,  334,  335,  565,  518,  553,  260,  263,   21,
 /*   520 */   684,  556,  687,   28,  688,  673,   60,  455,  115,  314,
 /*   530 */   141,  649,  102,  110,  453,  114,  140,  355,  357,  356,
 /*   540 */   326,  254,  257,  248,  322,  327,  341,  245,  653,  337,
 /*   550 */   558,  535,  318,  204,   27,  685,  530,  651,  673,  140,
 /*   560 */   348,  178,  564,   15,  649,  336,   43,  308,   18,  140,
 /*   570 */   347,  527,  536,  545,  548,  539,  542,  551,  557,  554,
 /*   580 */   530,  560,  205,  207,  209,   44,  218,  629,  272,  711,
 /*   590 */   229,  233,  238,  422,  165,  559,  429,  625,  355,  357,
 /*   600 */   356,  326,   32,  330,  413,  362,  358,  499,  514,  521,
 /*   610 */   524,  329,  331,  332,  333,  334,  335,  565,  518,  562,
 /*   620 */   260,  263,   20,   38,   22,  347,  154,  620,   25,   35,
 /*   630 */   279,   62,  314,  155,   65,  142,  561,  535,  140,  140,
 /*   640 */   355,  357,  356,  326,  254,  257,  248,  322,  327,  341,
 /*   650 */   245,  641,  337,  430,  498,  318,  204,   88,   96,  407,
 /*   660 */    23,  637,   25,  348,  213,  564,  432,  141,  336,  142,
 /*   670 */   308,  403,   42,  140,  527,  536,  545,  548,  539,  542,
 /*   680 */   551,  557,  554,   45,  560,  205,  207,  209,  601,  218,
 /*   690 */    46,  272,  434,  229,  233,  238,  422,  351,  177,  429,
 /*   700 */   493,  355,  357,  356,  326,  431,  330,  347,  412,  358,
 /*   710 */   499,  514,  521,  524,  329,  331,  332,  333,  334,  335,
 /*   720 */   565,  518,  409,  260,  263,    9,  347,  272,   12,  163,
 /*   730 */   156,  693,  697,  707,  359,  314,  564,  355,  357,  356,
 /*   740 */   326,  400,  347,   64,  345,  358,  446,  254,  257,  248,
 /*   750 */   322,  327,  341,  245,  464,  337,  607,   70,  318,  204,
 /*   760 */   396,  648,  102,  110,  172,  114,  348,  578,  564,  141,
 /*   770 */   649,  336,  695,  308,  693,  140,  381,  527,  536,  545,
 /*   780 */   548,  539,  542,  551,  557,  554,  360,  560,  205,  207,
 /*   790 */   209,  565,  218,   69,  365,  504,  229,  233,  238,  422,
 /*   800 */    79,  347,  429,  500,  505,  355,  357,  356,  326,  330,
 /*   810 */   347,   80,   83,  499,  514,  521,  524,  329,  331,  332,
 /*   820 */   333,  334,  335,  565,  518,  347,  260,  263,   31,  272,
 /*   830 */    34,   58,   37,   39,   40,  353,   86,   87,  314,  355,
 /*   840 */   357,  356,  326,   91,  379,  162,  399,  358,  165,   93,
 /*   850 */   254,  257,  248,  322,  327,  341,  245,   94,  337,  370,
 /*   860 */   347,  318,  204,   36,   37,   39,   40,   95,   97,  348,
 /*   870 */   180,  564,   30,   98,  336,   59,  308,  347,   99,  101,
 /*   880 */   527,  536,  545,  548,  539,  542,  551,  557,  554,   43,
 /*   890 */   560,  205,  207,  209,  363,  218,  104,  272,  105,  229,
 /*   900 */   233,  238,  422,  457,  107,  429,  459,  355,  357,  356,
 /*   910 */   326,  346,  330,  108,  378,  358,  499,  514,  521,  524,
 /*   920 */   329,  331,  332,  333,  334,  335,  565,  518,  111,  260,
 /*   930 */   263,  131,  272,   34,   58,   37,   39,   40,  109,  113,
 /*   940 */   112,  314,  355,  357,  356,  326,  127,  128,  132,  395,
 /*   950 */   358,  138,  145,  254,  257,  248,  322,  327,  341,  245,
 /*   960 */   146,  337,  148,  147,  318,  204,  150,  151,  152,  153,
 /*   970 */   157,  158,  348,  616,  564,  160,  161,  336,  692,  308,
 /*   980 */   164,  168,  174,  527,  536,  545,  548,  539,  542,  551,
 /*   990 */   557,  554,  175,  560,  205,  207,  209,  206,  218,  208,
 /*  1000 */   272,  211,  229,  233,  238,  422,  214,  215,  429,  220,
 /*  1010 */   355,  357,  356,  326,  224,  330,  227,  369,  358,  499,
 /*  1020 */   514,  521,  524,  329,  331,  332,  333,  334,  335,  565,
 /*  1030 */   518,  226,  260,  263,  137,  272,   34,   58,   37,   39,
 /*  1040 */    40,  230,  235,  239,  314,  355,  357,  356,  326,  242,
 /*  1050 */   244,  271,  352,  358,  312,  321,  254,  257,  248,  322,
 /*  1060 */   327,  341,  245,  315,  337,  316,  323,  318,  204,  324,
 /*  1070 */   349,  343,  361,  350,  366,  348,  612,  564,  367,  374,
 /*  1080 */   336,  375,  308,  368, 1047,  423,  527,  536,  545,  548,
 /*  1090 */   539,  542,  551,  557,  554,  376,  560,  205,  207,  209,
 /*  1100 */   404,  218,  410,  272,  405,  229,  233,  238,  422, 1046,
 /*  1110 */   411,  429,  391,  355,  357,  356,  326,  433,  330,  451,
 /*  1120 */   406,  358,  499,  514,  521,  524,  329,  331,  332,  333,
 /*  1130 */   334,  335,  565,  518,  454,  260,  263,  466,  444,  467,
 /*  1140 */   470,  474,  452,  471,  479,  486,  494,  314,  355,  357,
 /*  1150 */   356,  326,  614,  495,   34,   58,   37,   39,   40,  254,
 /*  1160 */   257,  248,  322,  327,  341,  245,  507,  337,  509,  496,
 /*  1170 */   318,  204,  511,  513,  532,  531,  534,  533,  348,  241,
 /*  1180 */   564,  573,  574,  336,  585,  308,  587,  591,  596,  527,
 /*  1190 */   536,  545,  548,  539,  542,  551,  557,  554,  597,  560,
 /*  1200 */   205,  207,  209,  603,  218,  615,  455,  622,  229,  233,
 /*  1210 */   238,  422,  631,  453,  429,  623,  355,  357,  356,  326,
 /*  1220 */   643,  330,  671,  657,  660,  499,  514,  521,  524,  329,
 /*  1230 */   331,  332,  333,  334,  335,  565,  518,  659,  260,  263,
 /*  1240 */   765,  444,  766,  666,  669,  483,  672,  680,  681,  677,
 /*  1250 */   314,  355,  357,  356,  326,  602,  682,   34,   58,   37,
 /*  1260 */    39,   40,  254,  257,  248,  322,  327,  341,  245,  691,
 /*  1270 */   337,  679,  698,  318,  204,  696,  699,  702,  700,  165,
 /*  1280 */   703,  348,  237,  564,  708,  706,  336,  709,  308,  712,
 /*  1290 */   710,  713,  527,  536,  545,  548,  539,  542,  551,  557,
 /*  1300 */   554,  544,  560,  205,  207,  209,  210,  218,  544,  421,
 /*  1310 */   544,  229,  233,  238,  422,  544,  544,  429,  544,  355,
 /*  1320 */   357,  356,  326,  544,  330,  544,  544,  544,  499,  514,
 /*  1330 */   521,  524,  329,  331,  332,  333,  334,  335,  565,  518,
 /*  1340 */   544,  260,  263,  544,  444,  544,  544,  544,  480,  544,
 /*  1350 */   544,  544,  544,  314,  355,  357,  356,  326,   63,  544,
 /*  1360 */    34,   58,   37,   39,   40,  254,  257,  248,  322,  327,
 /*  1370 */   341,  245,  544,  337,  544,  544,  318,  204,  544,  544,
 /*  1380 */   544,  544,  544,  544,  348,  638,  564,  544,  544,  336,
 /*  1390 */   544,  308,  544,  544,  544,  527,  536,  545,  548,  539,
 /*  1400 */   542,  551,  557,  554,  544,  560,  205,  207,  209,  544,
 /*  1410 */   218,  544,  272,  544,  229,  233,  238,  422,  544,  544,
 /*  1420 */   429,  544,  355,  357,  356,  326,  544,  330,  544,  544,
 /*  1430 */   372,  499,  514,  521,  524,  329,  331,  332,  333,  334,
 /*  1440 */   335,  565,  518,  544,  260,  263,  544,  444,  544,  544,
 /*  1450 */   544,  448,  544,  544,  544,  544,  314,  355,  357,  356,
 /*  1460 */   326,  544,  544,  544,  544,  544,  544,  544,  254,  257,
 /*  1470 */   248,  322,  327,  341,  245,  544,  337,  544,  544,  318,
 /*  1480 */   204,  544,  544,  544,  544,  544,  544,  348,  600,  564,
 /*  1490 */   544,  544,  336,  544,  308,  544,  544,  544,  527,  536,
 /*  1500 */   545,  548,  539,  542,  551,  557,  554,  544,  560,  205,
 /*  1510 */   207,  209,  219,  218,  544,  421,  544,  229,  233,  238,
 /*  1520 */   422,  544,  311,  429,  544,  355,  357,  356,  326,  544,
 /*  1530 */   330,  544,  544,  544,  499,  514,  521,  524,  329,  331,
 /*  1540 */   332,  333,  334,  335,  565,  518,  544,  260,  263,  544,
 /*  1550 */   309,  544,  544,  544,  544,  544,  544,  544,  544,  314,
 /*  1560 */   355,  357,  356,  326,  544,  544,  544,  544,  544,  544,
 /*  1570 */   544,  254,  257,  248,  322,  327,  341,  245,  544,  337,
 /*  1580 */   544,  544,  318,  204,  544,  544,  544,  544,  544,  544,
 /*  1590 */   348,  606,  564,  544,  544,  336,  544,  308,  544,  544,
 /*  1600 */   544,  527,  536,  545,  548,  539,  542,  551,  557,  554,
 /*  1610 */   544,  560,  205,  207,  209,  544,  218,  544,  504,  544,
 /*  1620 */   229,  233,  238,  422,  544,  544,  429,  503,  355,  357,
 /*  1630 */   356,  326,  544,  330,  544,  544,  544,  499,  514,  521,
 /*  1640 */   524,  329,  331,  332,  333,  334,  335,  565,  518,  544,
 /*  1650 */   260,  263,  544,  444,  544,  544,  544,  476,  544,  544,
 /*  1660 */   544,  544,  314,  355,  357,  356,  326,  544,  544,  544,
 /*  1670 */   544,  544,  544,  544,  254,  257,  248,  322,  327,  341,
 /*  1680 */   245,  544,  337,  544,  544,  318,  204,  544,  544,  544,
 /*  1690 */   544,  544,  544,  348,  599,  564,  544,  544,  336,  544,
 /*  1700 */   308,  544,  544,  544,  527,  536,  545,  548,  539,  542,
 /*  1710 */   551,  557,  554,  544,  560,  205,  207,  209,  586,  218,
 /*  1720 */   544,  421,  544,  229,  233,  238,  422,  544,  544,  429,
 /*  1730 */   544,  355,  357,  356,  326,  544,  330,  544,  544,  544,
 /*  1740 */   499,  514,  521,  524,  329,  331,  332,  333,  334,  335,
 /*  1750 */   565,  518,  544,  260,  263,  544,  444,  544,  544,  544,
 /*  1760 */   443,  544,  544,  544,  544,  314,  355,  357,  356,  326,
 /*  1770 */   544,  544,  544,  544,  544,  544,  544,  254,  257,  248,
 /*  1780 */   322,  327,  341,  245,  544,  337,  544,  544,  318,  204,
 /*  1790 */   544,  544,  544,  544,  544,  544,  348,  594,  564,  544,
 /*  1800 */   544,  336,  544,  308,  544,  544,  544,  527,  536,  545,
 /*  1810 */   548,  539,  542,  551,  557,  554,  544,  560,  205,  207,
 /*  1820 */   209,  544,  218,  544,  246,  544,  229,  233,  238,  422,
 /*  1830 */   544,  544,  429,  544,  355,  357,  356,  326,  544,  330,
 /*  1840 */   544,  544,  544,  499,  514,  521,  524,  329,  331,  332,
 /*  1850 */   333,  334,  335,  565,  518,  544,  260,  263,  544,  444,
 /*  1860 */   544,  544,  544,  487,  544,  544,  544,  544,  314,  355,
 /*  1870 */   357,  356,  326,  544,  544,  544,  544,  544,  544,  544,
 /*  1880 */   254,  257,  248,  322,  327,  341,  245,  544,  337,  544,
 /*  1890 */   544,  318,  204,  544,  544,  544,  544,  544,  544,  348,
 /*  1900 */   588,  564,  544,  544,  336,  544,  308,  544,  544,  544,
 /*  1910 */   527,  536,  545,  548,  539,  542,  551,  557,  554,  544,
 /*  1920 */   560,  205,  207,  209,  544,  218,  544,  420,  544,  229,
 /*  1930 */   233,  238,  422,  544,  544,  429,  544,  355,  357,  356,
 /*  1940 */   326,  544,  330,  544,  544,  544,  499,  514,  521,  524,
 /*  1950 */   329,  331,  332,  333,  334,  335,  565,  518,  544,  260,
 /*  1960 */   263,  544,  444,  544,  544,  544,  472,  544,  544,  544,
 /*  1970 */   544,  314,  355,  357,  356,  326,  544,  544,  544,  544,
 /*  1980 */   544,  544,  544,  254,  257,  248,  322,  327,  341,  245,
 /*  1990 */   544,  337,  544,  544,  318,  204,  544,  544,  544,  544,
 /*  2000 */   544,  544,  348,  646,  564,  544,  544,  336,  544,  308,
 /*  2010 */   544,  544,  544,  527,  536,  545,  548,  539,  542,  551,
 /*  2020 */   557,  554,  544,  560,  205,  207,  209,  544,  218,  544,
 /*  2030 */   249,  544,  229,  233,  238,  422,  544,  544,  429,  544,
 /*  2040 */   355,  357,  356,  326,  544,  330,  544,  544,  544,  499,
 /*  2050 */   514,  521,  524,  329,  331,  332,  333,  334,  335,  565,
 /*  2060 */   518,  544,  260,  263,  544,  444,  544,  544,  544,  468,
 /*  2070 */   544,  544,  544,  544,  314,  355,  357,  356,  326,  544,
 /*  2080 */   544,  544,  544,  544,  544,  544,  254,  257,  248,  322,
 /*  2090 */   327,  341,  245,  544,  337,  544,  544,  318,  204,  544,
 /*  2100 */   544,  544,  544,  544,  544,  348,  604,  564,  544,  544,
 /*  2110 */   336,  544,  308,  544,  544,  544,  527,  536,  545,  548,
 /*  2120 */   539,  542,  551,  557,  554,  544,  560,  205,  207,  209,
 /*  2130 */   544,  218,  544,  419,  544,  229,  233,  238,  422,  544,
 /*  2140 */   544,  429,  544,  355,  357,  356,  326,  544,  330,  544,
 /*  2150 */   544,  544,  499,  514,  521,  524,  329,  331,  332,  333,
 /*  2160 */   334,  335,  565,  518,  544,  260,  263,  544,  444,  544,
 /*  2170 */   544,  544,  490,  544,  544,  544,  544,  314,  355,  357,
 /*  2180 */   356,  326,  544,  544,  544,  544,  544,  544,  544,  254,
 /*  2190 */   257,  248,  322,  327,  341,  245,  544,  337,  544,  544,
 /*  2200 */   318,  204,  544,  544,  544,  544,  544,  544,  348,  632,
 /*  2210 */   564,  544,  544,  336,  544,  308,  544,  544,  544,  527,
 /*  2220 */   536,  545,  548,  539,  542,  551,  557,  554,  544,  560,
 /*  2230 */   205,  207,  209,  544,  218,  544,  252,  544,  229,  233,
 /*  2240 */   238,  422,  544,  544,  429,  544,  355,  357,  356,  326,
 /*  2250 */   544,  330,  544,  544,  544,  499,  514,  521,  524,  329,
 /*  2260 */   331,  332,  333,  334,  335,  565,  518,  544,  260,  263,
 /*  2270 */   544,  444,  544,  544,  544,  497,  544,  544,  544,  544,
 /*  2280 */   314,  355,  357,  356,  326,  544,  544,  544,  544,  544,
 /*  2290 */   544,  544,  254,  257,  248,  322,  327,  341,  245,  544,
 /*  2300 */   337,  544,  544,  318,  204,  544,  544,  544,  544,  544,
 /*  2310 */   544,  348,  634,  564,  544,  544,  336,  544,  308,  544,
 /*  2320 */   544,  544,  527,  536,  545,  548,  539,  542,  551,  557,
 /*  2330 */   554,  544,  560,  205,  207,  209,  544,  218,  544,  299,
 /*  2340 */   544,  229,  233,  238,  422,  544,  544,  429,  544,  355,
 /*  2350 */   357,  356,  326,  544,  330,  544,  544,  544,  499,  514,
 /*  2360 */   521,  524,  329,  331,  332,  333,  334,  335,  565,  518,
 /*  2370 */   544,  260,  263,  544,  444,  544,  544,  544,  458,  544,
 /*  2380 */   544,  544,  544,  314,  355,  357,  356,  326,  544,  544,
 /*  2390 */   544,  544,  544,  544,  544,  254,  257,  248,  322,  327,
 /*  2400 */   341,  245,  544,  337,  544,  544,  318,  204,  544,  544,
 /*  2410 */   544,  544,  544,  544,  348,  589,  564,  544,  544,  336,
 /*  2420 */   544,  308,  544,  544,  544,  527,  536,  545,  548,  539,
 /*  2430 */   542,  551,  557,  554,  544,  560,  205,  207,  209,  544,
 /*  2440 */   218,  544,  418,  544,  229,  233,  238,  422,  544,  544,
 /*  2450 */   429,  544,  355,  357,  356,  326,  544,  330,  544,  544,
 /*  2460 */   544,  499,  514,  521,  524,  329,  331,  332,  333,  334,
 /*  2470 */   335,  565,  518,  544,  260,  263,  544,  255,  544,  544,
 /*  2480 */   544,  544,  544,  544,  544,  544,  314,  355,  357,  356,
 /*  2490 */   326,  544,  544,  544,  544,  544,  544,  544,  254,  257,
 /*  2500 */   248,  322,  327,  341,  245,  544,  337,  544,  544,  318,
 /*  2510 */   204,  544,  544,  544,  544,  544,  544,  348,  584,  564,
 /*  2520 */   544,  544,  336,  544,  308,  544,  544,  544,  527,  536,
 /*  2530 */   545,  548,  539,  542,  551,  557,  554,  544,  560,  205,
 /*  2540 */   207,  209,  544,  218,  544,  417,  544,  229,  233,  238,
 /*  2550 */   422,  544,  544,  429,  544,  355,  357,  356,  326,  544,
 /*  2560 */   330,  544,  544,  544,  499,  514,  521,  524,  329,  331,
 /*  2570 */   332,  333,  334,  335,  565,  518,  544,  260,  263,  544,
 /*  2580 */   258,  544,  544,  544,  544,  544,  544,  544,  544,  314,
 /*  2590 */   355,  357,  356,  326,  544,  544,  544,  544,  544,  544,
 /*  2600 */   544,  254,  257,  248,  322,  327,  341,  245,  544,  337,
 /*  2610 */   544,  544,  318,  204,  544,  544,  544,  544,  544,  544,
 /*  2620 */   348,  232,  564,  544,  544,  336,  544,  308,  544,  544,
 /*  2630 */   544,  527,  536,  545,  548,  539,  542,  551,  557,  554,
 /*  2640 */   544,  560,  205,  207,  209,  544,  218,  544,  416,  544,
 /*  2650 */   229,  233,  238,  422,  544,  544,  429,  544,  355,  357,
 /*  2660 */   356,  326,  544,  330,  544,  544,  544,  499,  514,  521,
 /*  2670 */   524,  329,  331,  332,  333,  334,  335,  565,  518,  544,
 /*  2680 */   260,  263,  544,  261,  544,  544,  544,  544,  544,  544,
 /*  2690 */   544,  544,  314,  355,  357,  356,  326,  544,  544,  544,
 /*  2700 */   544,  544,  544,  544,  254,  257,  248,  322,  327,  341,
 /*  2710 */   245,  544,  337,  544,  544,  318,  204,  544,  544,  544,
 /*  2720 */   544,  544,  544,  348,  628,  564,  544,  544,  336,  544,
 /*  2730 */   308,  544,  544,  544,  527,  536,  545,  548,  539,  542,
 /*  2740 */   551,  557,  554,  544,  560,  205,  207,  209,  544,  218,
 /*  2750 */   544,  297,  544,  229,  233,  238,  422,  544,  544,  429,
 /*  2760 */   544,  355,  357,  356,  326,  544,  330,  544,  544,  544,
 /*  2770 */   499,  514,  521,  524,  329,  331,  332,  333,  334,  335,
 /*  2780 */   565,  518,  544,  260,  263,  544,  415,  544,  544,  544,
 /*  2790 */   544,  544,  544,  544,  544,  314,  355,  357,  356,  326,
 /*  2800 */   544,  544,  544,  544,  544,  544,  544,  254,  257,  248,
 /*  2810 */   322,  327,  341,  245,  544,  337,  544,  544,  318,  204,
 /*  2820 */   544,  544,  544,  544,  544,  544,  348,  626,  564,  544,
 /*  2830 */   544,  336,  544,  308,  544,  544,  544,  527,  536,  545,
 /*  2840 */   548,  539,  542,  551,  557,  554,  544,  560,  205,  207,
 /*  2850 */   209,  544,  218,  544,  264,  544,  229,  233,  238,  422,
 /*  2860 */   544,  544,  429,  544,  355,  357,  356,  326,  544,  330,
 /*  2870 */   544,  544,  544,  499,  514,  521,  524,  329,  331,  332,
 /*  2880 */   333,  334,  335,  565,  518,  544,  260,  263,  544,  266,
 /*  2890 */   544,  544,  544,  544,  544,  544,  544,  544,  314,  355,
 /*  2900 */   357,  356,  326,  544,  544,  544,  544,  544,  544,  544,
 /*  2910 */   254,  257,  248,  322,  327,  341,  245,  544,  337,  544,
 /*  2920 */   544,  318,  204,  544,  544,  544,  544,  544,  544,  348,
 /*  2930 */   595,  564,  544,  544,  336,  544,  308,  544,  544,  544,
 /*  2940 */   527,  536,  545,  548,  539,  542,  551,  557,  554,  544,
 /*  2950 */   560,  205,  207,  209,  544,  218,  544,  268,  544,  229,
 /*  2960 */   233,  238,  422,  544,  544,  429,  544,  355,  357,  356,
 /*  2970 */   326,  544,  330,  544,  544,  544,  499,  514,  521,  524,
 /*  2980 */   329,  331,  332,  333,  334,  335,  565,  518,  544,  260,
 /*  2990 */   263,  544,  295,  544,  544,  544,  544,  544,  544,  544,
 /*  3000 */   544,  314,  355,  357,  356,  326,  544,  544,  544,  544,
 /*  3010 */   544,  544,  544,  254,  257,  248,  322,  327,  341,  245,
 /*  3020 */   544,  337,  544,  544,  318,  204,  544,  544,  544,  544,
 /*  3030 */   544,  544,  348,  217,  564,  544,  544,  336,  544,  308,
 /*  3040 */   544,  544,  544,  527,  536,  545,  548,  539,  542,  551,
 /*  3050 */   557,  554,  544,  560,  205,  207,  209,  544,  218,  544,
 /*  3060 */   293,  544,  229,  233,  238,  422,  544,  544,  429,  544,
 /*  3070 */   355,  357,  356,  326,  544,  330,  544,  544,  544,  499,
 /*  3080 */   514,  521,  524,  329,  331,  332,  333,  334,  335,  565,
 /*  3090 */   518,  544,  260,  263,  544,  274,  544,  544,  544,  544,
 /*  3100 */   544,  544,  544,  544,  314,  355,  357,  356,  326,  544,
 /*  3110 */   544,  544,  544,  544,  544,  544,  254,  257,  248,  322,
 /*  3120 */   327,  341,  245,  544,  337,  544,  544,  318,  204,  544,
 /*  3130 */   544,  544,  544,  544,  544,  348,  428,  564,  544,  544,
 /*  3140 */   336,  544,  308,  544,  544,  544,  527,  536,  545,  548,
 /*  3150 */   539,  542,  551,  557,  554,  544,  560,  205,  207,  209,
 /*  3160 */   544,  218,  544,  291,  544,  229,  233,  238,  422,  544,
 /*  3170 */   544,  429,  544,  355,  357,  356,  326,  544,  330,  544,
 /*  3180 */   544,  544,  499,  514,  521,  524,  329,  331,  332,  333,
 /*  3190 */   334,  335,  565,  518,  544,  260,  263,  544,  277,  544,
 /*  3200 */   544,  544,  544,  544,  544,  544,  544,  314,  355,  357,
 /*  3210 */   356,  326,  544,  544,  544,  544,  544,  544,  544,  254,
 /*  3220 */   257,  248,  322,  327,  341,  245,  544,  337,  544,  544,
 /*  3230 */   318,  204,  544,  544,  544,  544,  544,  544,  348,  571,
 /*  3240 */   564,  544,  544,  336,  544,  308,  544,  544,  544,  527,
 /*  3250 */   536,  545,  548,  539,  542,  551,  557,  554,  544,  560,
 /*  3260 */   205,  207,  209,  544,  218,  544,  281,  544,  229,  233,
 /*  3270 */   238,  422,  544,  544,  429,  544,  355,  357,  356,  326,
 /*  3280 */   544,  330,  544,  544,  544,  499,  514,  521,  524,  329,
 /*  3290 */   331,  332,  333,  334,  335,  565,  518,  544,  260,  263,
 /*  3300 */   544,  289,  544,  544,  544,  544,  544,  544,  544,  544,
 /*  3310 */   314,  355,  357,  356,  326,  544,  544,  544,  544,  544,
 /*  3320 */   544,  544,  254,  257,  248,  322,  327,  341,  245,  544,
 /*  3330 */   337,  544,  544,  318,  204,  544,  544,  544,  544,  544,
 /*  3340 */   544,  348,  544,  564,  544,  544,  336,  544,  308,  544,
 /*  3350 */   544,  544,  527,  536,  545,  548,  539,  542,  551,  557,
 /*  3360 */   554,  716,  560,  205,  207,  209,  544,  218,  544,  283,
 /*  3370 */   544,  229,  233,  238,  422,  544,  544,  429,  544,  355,
 /*  3380 */   357,  356,  326,  544,  330,  544,  544,  544,  499,  514,
 /*  3390 */   521,  524,  329,  331,  332,  333,  334,  335,  565,  251,
 /*  3400 */   544,  260,  263,  544,    9,  544,  544,   12,  544,  156,
 /*  3410 */   544,  697,  707,  314,  544,  564,  544,  116,  117,  119,
 /*  3420 */   118,  120,  544,  544,  544,  254,  257,  248,  322,  327,
 /*  3430 */   341,  245,  544,  337,  544,  285,  318,  544,  544,  544,
 /*  3440 */   544,  544,  544,  544,  348,  355,  357,  356,  326,  336,
 /*  3450 */   544,  308,  544,  544,  544,   47,   48,   49,   50,   51,
 /*  3460 */    52,   53,   54,   55,   56,   57,  122,  123,  544,  338,
 /*  3470 */   565,  202,  182,  544,  544,  544,  121,  544,  544,  355,
 /*  3480 */   357,  356,  326,  544,  544,  386,  544,  330,  394,  390,
 /*  3490 */   393,  544,  544,  598,  544,  329,  331,  332,  333,  334,
 /*  3500 */   335,  544,  566,  183,  184,  185,  186,  187,  188,  189,
 /*  3510 */   190,  191,  192,  193,  194,  195,  196,  197,  198,  199,
 /*  3520 */   200,  201,  203,  544,  544,  544,  563,  544,  544,  544,
 /*  3530 */   338,  544,  202,  182,  544,  338,  506,  508,  510,  512,
 /*  3540 */   355,  357,  356,  326,  544,  355,  357,  356,  326,  389,
 /*  3550 */   390,  393,  544,  544,  240,  544,  392,  544,  544,  544,
 /*  3560 */   544,  544,  544,  566,  183,  184,  185,  186,  187,  188,
 /*  3570 */   189,  190,  191,  192,  193,  194,  195,  196,  197,  198,
 /*  3580 */   199,  200,  201,  203,  202,  182,  544,  563,  544,  544,
 /*  3590 */   544,  287,  544,  544,  544,  544,  544,  506,  508,  510,
 /*  3600 */   512,  355,  357,  356,  326,  544,  216,  544,  544,  544,
 /*  3610 */   544,  544,  544,  544,  544,  566,  183,  184,  185,  186,
 /*  3620 */   187,  188,  189,  190,  191,  192,  193,  194,  195,  196,
 /*  3630 */   197,  198,  199,  200,  201,  203,  202,  182,  544,  563,
 /*  3640 */   544,  544,  544,  580,  544,  544,  544,  544,  544,  506,
 /*  3650 */   508,  510,  512,  355,  357,  356,  326,  544,  228,  544,
 /*  3660 */   544,  544,  544,  544,  544,  544,  544,  566,  183,  184,
 /*  3670 */   185,  186,  187,  188,  189,  190,  191,  192,  193,  194,
 /*  3680 */   195,  196,  197,  198,  199,  200,  201,  203,  202,  182,
 /*  3690 */   544,  563,  544,  544,  544,  575,  544,  544,  544,  544,
 /*  3700 */   544,  506,  508,  510,  512,  355,  357,  356,  326,  544,
 /*  3710 */   212,  544,  544,  544,  544,  544,  544,  544,  544,  566,
 /*  3720 */   183,  184,  185,  186,  187,  188,  189,  190,  191,  192,
 /*  3730 */   193,  194,  195,  196,  197,  198,  199,  200,  201,  203,
 /*  3740 */   202,  182,  544,  563,  544,  544,  544,  568,  544,  544,
 /*  3750 */   544,  544,  544,  506,  508,  510,  512,  355,  357,  356,
 /*  3760 */   326,  544,  231,  544,  544,  544,  544,  544,  544,  544,
 /*  3770 */   544,  566,  183,  184,  185,  186,  187,  188,  189,  190,
 /*  3780 */   191,  192,  193,  194,  195,  196,  197,  198,  199,  200,
 /*  3790 */   201,  203,  202,  182,  544,  563,  544,  544,  544,  525,
 /*  3800 */   544,  544,  544,  544,  544,  506,  508,  510,  512,  355,
 /*  3810 */   357,  356,  326,  544,  570,  544,  544,  544,  544,  544,
 /*  3820 */   544,  544,  544,  566,  183,  184,  185,  186,  187,  188,
 /*  3830 */   189,  190,  191,  192,  193,  194,  195,  196,  197,  198,
 /*  3840 */   199,  200,  201,  203,  202,  182,  544,  563,  544,  544,
 /*  3850 */   544,  522,  544,  544,  544,  544,  544,  506,  508,  510,
 /*  3860 */   512,  355,  357,  356,  326,  544,  427,  544,  544,  544,
 /*  3870 */   544,  544,  544,  544,  544,  566,  183,  184,  185,  186,
 /*  3880 */   187,  188,  189,  190,  191,  192,  193,  194,  195,  196,
 /*  3890 */   197,  198,  199,  200,  201,  203,  202,  182,  544,  563,
 /*  3900 */   544,  544,  544,  519,  544,  544,  544,  544,  544,  506,
 /*  3910 */   508,  510,  512,  355,  357,  356,  326,  544,  577,  544,
 /*  3920 */   544,  544,  544,  544,  544,  544,  544,  566,  183,  184,
 /*  3930 */   185,  186,  187,  188,  189,  190,  191,  192,  193,  194,
 /*  3940 */   195,  196,  197,  198,  199,  200,  201,  203,  202,  182,
 /*  3950 */   544,  563,  544,  544,  544,  515,  544,  544,  544,  544,
 /*  3960 */   544,  506,  508,  510,  512,  355,  357,  356,  326,  544,
 /*  3970 */   179,  544,  544,  544,  544,  544,  544,  544,  544,  566,
 /*  3980 */   183,  184,  185,  186,  187,  188,  189,  190,  191,  192,
 /*  3990 */   193,  194,  195,  196,  197,  198,  199,  200,  201,  203,
 /*  4000 */   202,  182,  544,  563,  544,  544,  544,  425,  544,  544,
 /*  4010 */   544,  544,  544,  506,  508,  510,  512,  355,  357,  356,
 /*  4020 */   326,  544,  645,  544,  544,  544,  544,  544,  544,  544,
 /*  4030 */   544,  566,  183,  184,  185,  186,  187,  188,  189,  190,
 /*  4040 */   191,  192,  193,  194,  195,  196,  197,  198,  199,  200,
 /*  4050 */   201,  203,  202,  182,  544,  563,  544,  544,  544,  328,
 /*  4060 */   544,  544,  544,  544,  544,  506,  508,  510,  512,  355,
 /*  4070 */   357,  356,  326,  544,  582,  544,  544,  544,  544,  544,
 /*  4080 */   544,  544,  544,  566,  183,  184,  185,  186,  187,  188,
 /*  4090 */   189,  190,  191,  192,  193,  194,  195,  196,  197,  198,
 /*  4100 */   199,  200,  201,  203,  202,  182,  544,  563,  544,  544,
 /*  4110 */   544,  325,  544,  544,  544,  544,  544,  506,  508,  510,
 /*  4120 */   512,  355,  357,  356,  326,  544,  592,  544,  544,  544,
 /*  4130 */   544,  544,  544,  544,  544,  566,  183,  184,  185,  186,
 /*  4140 */   187,  188,  189,  190,  191,  192,  193,  194,  195,  196,
 /*  4150 */   197,  198,  199,  200,  201,  203,  202,  182,  544,  563,
 /*  4160 */   544,  544,  544,  317,  544,  544,  544,  544,  544,  506,
 /*  4170 */   508,  510,  512,  355,  357,  356,  326,  544,  639,  544,
 /*  4180 */   544,  544,  544,  544,  544,  544,  544,  566,  183,  184,
 /*  4190 */   185,  186,  187,  188,  189,  190,  191,  192,  193,  194,
 /*  4200 */   195,  196,  197,  198,  199,  200,  201,  203,  202,  182,
 /*  4210 */   544,  563,  544,  544,  544,  313,  544,  544,  544,  544,
 /*  4220 */   544,  506,  508,  510,  512,  355,  357,  356,  326,  544,
 /*  4230 */   236,  544,  544,  544,  544,  544,  544,  544,  544,  566,
 /*  4240 */   183,  184,  185,  186,  187,  188,  189,  190,  191,  192,
 /*  4250 */   193,  194,  195,  196,  197,  198,  199,  200,  201,  203,
 /*  4260 */   202,  182,  544,  563,  544,  544,  544,  307,  544,  544,
 /*  4270 */   544,  544,  544,  506,  508,  510,  512,  355,  357,  356,
 /*  4280 */   326,  544,  605,  544,  544,  544,  544,  544,  544,  544,
 /*  4290 */   544,  566,  183,  184,  185,  186,  187,  188,  189,  190,
 /*  4300 */   191,  192,  193,  194,  195,  196,  197,  198,  199,  200,
 /*  4310 */   201,  203,  202,  182,  544,  563,  544,  544,  544,  305,
 /*  4320 */   544,  544,  544,  544,  544,  506,  508,  510,  512,  355,
 /*  4330 */   357,  356,  326,  544,  611,  544,  544,  544,  544,  544,
 /*  4340 */   544,  544,  544,  566,  183,  184,  185,  186,  187,  188,
 /*  4350 */   189,  190,  191,  192,  193,  194,  195,  196,  197,  198,
 /*  4360 */   199,  200,  201,  203,  202,  182,  544,  563,  544,  544,
 /*  4370 */   544,  303,  544,  544,  544,  544,  544,  506,  508,  510,
 /*  4380 */   512,  355,  357,  356,  326,  544,  617,  544,  544,  544,
 /*  4390 */   544,  544,  544,  544,  544,  566,  183,  184,  185,  186,
 /*  4400 */   187,  188,  189,  190,  191,  192,  193,  194,  195,  196,
 /*  4410 */   197,  198,  199,  200,  201,  203,  202,  182,  544,  563,
 /*  4420 */   544,  544,  544,  301,  544,  544,  544,  544,  544,  506,
 /*  4430 */   508,  510,  512,  355,  357,  356,  326,  544,  627,  544,
 /*  4440 */   544,  544,  544,  544,  544,  544,  544,  566,  183,  184,
 /*  4450 */   185,  186,  187,  188,  189,  190,  191,  192,  193,  194,
 /*  4460 */   195,  196,  197,  198,  199,  200,  201,  203,  202,  182,
 /*  4470 */   544,  563,  544,  544,  544,  544,  544,  544,  544,  544,
 /*  4480 */   544,  506,  508,  510,  512,  544,  544,  544,  544,  544,
 /*  4490 */   633,  544,  544,  544,  544,  544,  544,  544,  544,  566,
 /*  4500 */   183,  184,  185,  186,  187,  188,  189,  190,  191,  192,
 /*  4510 */   193,  194,  195,  196,  197,  198,  199,  200,  201,  203,
 /*  4520 */   202,  182,  544,  563,  544,  544,  544,  544,  544,  544,
 /*  4530 */   544,  544,  544,  506,  508,  510,  512,  544,  544,  544,
 /*  4540 */   544,  544,  544,  544,  544,  544,  544,  544,  544,  544,
 /*  4550 */   544,  181,  183,  184,  185,  186,  187,  188,  189,  190,
 /*  4560 */   191,  192,  193,  194,  195,  196,  197,  198,  199,  200,
 /*  4570 */   201,  203,  544,  544,  544,  563,  544,  544,  544,  544,
 /*  4580 */   544,  544,  544,  544,  544,  506,  508,  510,  512,  544,
 /*  4590 */   544,  544,  544,  544,  304,  544,  544,  302,  288,  290,
 /*  4600 */   294,  292,  296,  298,  300,  247,  253,  259,  262,  265,
 /*  4610 */   267,  256,  250,  276,  278,  286,  280,  282,  284,  544,
 /*  4620 */   544,  544,  544,  544,  544,  544,  544,  273,  269,  544,
 /*  4630 */   544,  526,  544,  544,  544,  544,  304,  544,  544,  302,
 /*  4640 */   288,  290,  294,  292,  296,  298,  300,  247,  253,  259,
 /*  4650 */   262,  265,  267,  256,  250,  276,  278,  286,  280,  282,
 /*  4660 */   284,  544,  544,  544,  544,  544,  544,  544,  544,  273,
 /*  4670 */   269,  247,  253,  259,  262,  265,  267,  256,  250,  276,
 /*  4680 */   278,  286,  280,  282,  284,  544,  544,  544,  544,  544,
 /*  4690 */   931,  544,  544,  273,  269,  544,  544,  544,  544,  304,
 /*  4700 */   544,  544,  302,  288,  290,  294,  292,  296,  298,  300,
 /*  4710 */   247,  253,  259,  262,  265,  267,  256,  250,  276,  278,
 /*  4720 */   286,  280,  282,  284,  544,  544,  544,  544,  544,  544,
 /*  4730 */   544,  544,  273,  269,  544,  544,   41,  544,  544,  544,
 /*  4740 */   544,  304,  544,  426,  302,  288,  290,  294,  292,  296,
 /*  4750 */   298,  300,  247,  253,  259,  262,  265,  267,  256,  250,
 /*  4760 */   276,  278,  286,  280,  282,  284,  544,  544,  544,  544,
 /*  4770 */   544,  544,  544,  544,  273,  269,   33,  544,   47,   48,
 /*  4780 */    49,   50,   51,   52,   53,   54,   55,   56,   57,  544,
 /*  4790 */   544,  544,  544,  544,  544,  275,  544,  544,  544,  544,
 /*  4800 */   544,  544,  544,  544,  304,  544,  544,  302,  288,  290,
 /*  4810 */   294,  292,  296,  298,  300,  247,  253,  259,  262,  265,
 /*  4820 */   267,  256,  250,  276,  278,  286,  280,  282,  284,  544,
 /*  4830 */   544,  544,  544,  544,  544,  544,  304,  273,  269,  302,
 /*  4840 */   288,  290,  294,  292,  296,  298,  300,  247,  253,  259,
 /*  4850 */   262,  265,  267,  256,  250,  276,  278,  286,  280,  282,
 /*  4860 */   284,  544,  544,  544,  544,  544,  544,  544,  544,  273,
 /*  4870 */   269,  544,  544,  544,  544,  544,  306,  544,  304,  544,
 /*  4880 */   581,  302,  288,  290,  294,  292,  296,  298,  300,  247,
 /*  4890 */   253,  259,  262,  265,  267,  256,  250,  276,  278,  286,
 /*  4900 */   280,  282,  284,  544,  544,  544,  544,  544,  544,  544,
 /*  4910 */   544,  273,  269,  544,  544,  544,  544,  544,  544,  544,
 /*  4920 */   304,  544,  576,  302,  288,  290,  294,  292,  296,  298,
 /*  4930 */   300,  247,  253,  259,  262,  265,  267,  256,  250,  276,
 /*  4940 */   278,  286,  280,  282,  284,  544,  544,  544,  544,  544,
 /*  4950 */   544,  544,  544,  273,  269,  544,  544,  516,  544,  544,
 /*  4960 */   544,  544,  304,  544,  544,  302,  288,  290,  294,  292,
 /*  4970 */   296,  298,  300,  247,  253,  259,  262,  265,  267,  256,
 /*  4980 */   250,  276,  278,  286,  280,  282,  284,  544,  544,  544,
 /*  4990 */   544,  544,  544,  544,  544,  273,  269,  544,  544,  544,
 /*  5000 */   544,  544,  544,  544,  304,  544,  569,  302,  288,  290,
 /*  5010 */   294,  292,  296,  298,  300,  247,  253,  259,  262,  265,
 /*  5020 */   267,  256,  250,  276,  278,  286,  280,  282,  284,  544,
 /*  5030 */   544,  544,  544,  544,  544,  544,  544,  273,  269,  544,
 /*  5040 */   544,  520,  544,  544,  544,  544,  304,  544,  544,  302,
 /*  5050 */   288,  290,  294,  292,  296,  298,  300,  247,  253,  259,
 /*  5060 */   262,  265,  267,  256,  250,  276,  278,  286,  280,  282,
 /*  5070 */   284,  544,  544,  544,  544,  544,  544,  544,  544,  273,
 /*  5080 */   269,  544,  544,  523,  544,  544,  544,  544,  304,  544,
 /*  5090 */   544,  302,  288,  290,  294,  292,  296,  298,  300,  247,
 /*  5100 */   253,  259,  262,  265,  267,  256,  250,  276,  278,  286,
 /*  5110 */   280,  282,  284,  544,  544,  544,  544,  544,  544,  544,
 /*  5120 */   310,  273,  269,  544,  544,  544,  544,   41,  544,  304,
 /*  5130 */   544,  544,  302,  288,  290,  294,  292,  296,  298,  300,
 /*  5140 */   247,  253,  259,  262,  265,  267,  256,  250,  276,  278,
 /*  5150 */   286,  280,  282,  284,  251,  544,  260,  263,  544,  544,
 /*  5160 */   544,  544,  273,  269,  544,  544,  100,  544,  314,   47,
 /*  5170 */    48,   49,   50,   51,   52,   53,   54,   55,   56,   57,
 /*  5180 */   254,  257,  248,  322,  327,  341,  245,  344,  337,   68,
 /*  5190 */   544,  318,  544,  251,  544,  260,  263,  544,  544,  348,
 /*  5200 */   544,   84,  544,  544,  336,  544,  308,  314,   47,   48,
 /*  5210 */    49,   50,   51,   52,   53,   54,   55,   56,   57,  254,
 /*  5220 */   257,  248,  322,  327,  341,  245,  397,  337,  544,  544,
 /*  5230 */   318,  544,  251,  544,  260,  263,  544,  544,  348,  544,
 /*  5240 */   544,  544,  330,  336,  544,  308,  314,  544,  544,  544,
 /*  5250 */   329,  331,  332,  333,  334,  335,  544,  544,  254,  257,
 /*  5260 */   248,  322,  327,  341,  245,  354,  337,  544,  544,  318,
 /*  5270 */   544,  251,  544,  260,  263,  544,  544,  348,  544,  544,
 /*  5280 */   544,  330,  336,  544,  308,  314,  544,  544,  544,  329,
 /*  5290 */   331,  332,  333,  334,  335,  544,  544,  254,  257,  248,
 /*  5300 */   322,  327,  341,  245,  544,  337,  544,  544,  318,  544,
 /*  5310 */   251,  544,  260,  263,  544,  544,  348,  544,  544,  544,
 /*  5320 */   330,  336,  544,  308,  314,  544,  450,  544,  329,  331,
 /*  5330 */   332,  333,  334,  335,  544,  544,  254,  257,  248,  322,
 /*  5340 */   327,  341,  245,  371,  337,  544,  544,  318,  544,  251,
 /*  5350 */   544,  260,  263,  544,  544,  348,  544,  544,  544,  330,
 /*  5360 */   336,  544,  308,  314,  544,  544,  544,  329,  331,  332,
 /*  5370 */   333,  334,  335,  544,  544,  254,  257,  248,  322,  327,
 /*  5380 */   341,  245,  544,  337,  544,  544,  318,  517,  251,  544,
 /*  5390 */   260,  263,  544,  544,  348,  544,  544,  544,  330,  336,
 /*  5400 */   544,  308,  314,  544,  544,  544,  329,  331,  332,  333,
 /*  5410 */   334,  335,  544,  544,  254,  257,  248,  322,  327,  341,
 /*  5420 */   245,  377,  337,  544,  544,  318,  544,  544,  544,  544,
 /*  5430 */   544,  544,  544,  348,  544,  544,  544,  330,  336,  544,
 /*  5440 */   308,  544,  544,  544,  544,  329,  331,  332,  333,  334,
 /*  5450 */   335,  544,  302,  288,  290,  294,  292,  296,  298,  300,
 /*  5460 */   247,  253,  259,  262,  265,  267,  256,  250,  276,  278,
 /*  5470 */   286,  280,  282,  284,  544,  544,  330,  251,  544,  260,
 /*  5480 */   263,  544,  273,  269,  329,  331,  332,  333,  334,  335,
 /*  5490 */   544,  314,  544,  544,  544,  544,  544,  544,  544,  544,
 /*  5500 */   544,  544,  544,  254,  257,  248,  322,  327,  341,  245,
 /*  5510 */   544,  337,  544,  544,  318,  544,  251,  544,  260,  263,
 /*  5520 */   544,  544,  348,  544,  544,  544,  544,  336,  544,  308,
 /*  5530 */   314,  544,  485,  544,  544,  544,  544,  544,  544,  544,
 /*  5540 */   544,  544,  254,  257,  248,  322,  327,  341,  245,  401,
 /*  5550 */   337,  544,  544,  318,  544,  251,  544,  260,  263,  544,
 /*  5560 */   544,  348,  544,  544,  544,  330,  336,  544,  308,  314,
 /*  5570 */   544,  544,  544,  329,  331,  332,  333,  334,  335,  544,
 /*  5580 */   544,  254,  257,  248,  322,  327,  341,  245,  382,  337,
 /*  5590 */   544,  544,  318,  544,  251,  544,  260,  263,  544,  544,
 /*  5600 */   348,  544,  544,  544,  330,  336,  544,  308,  314,  544,
 /*  5610 */   544,  544,  329,  331,  332,  333,  334,  335,  544,  544,
 /*  5620 */   254,  257,  248,  322,  327,  341,  245,  408,  337,  544,
 /*  5630 */   544,  318,  544,  251,  544,  260,  263,  544,  544,  348,
 /*  5640 */   544,  544,  544,  330,  336,  544,  308,  314,  544,  544,
 /*  5650 */   544,  329,  331,  332,  333,  334,  335,  544,  544,  254,
 /*  5660 */   257,  248,  322,  327,  341,  245,  544,  337,  544,  544,
 /*  5670 */   318,  544,  251,  544,  260,  263,  544,  544,  348,  544,
 /*  5680 */   544,  544,  330,  336,  544,  308,  314,  544,  544,  544,
 /*  5690 */   329,  331,  332,  333,  334,  335,  544,  544,  254,  257,
 /*  5700 */   248,  322,  327,  341,  245,  544,  337,  544,  544,  339,
 /*  5710 */   544,  544,  251,  567,  260,  263,  544,  348,  544,  544,
 /*  5720 */   544,  330,  336,  544,  308,  544,  314,  385,  544,  329,
 /*  5730 */   331,  332,  333,  334,  335,  544,  544,  544,  254,  257,
 /*  5740 */   248,  322,  327,  341,  245,  544,  337,  544,  544,  318,
 /*  5750 */   544,  251,  544,  260,  263,  544,  544,  348,  544,  544,
 /*  5760 */   384,  544,  336,  544,  308,  314,  544,  478,  383,  331,
 /*  5770 */   332,  333,  334,  335,  544,  544,  544,  254,  257,  248,
 /*  5780 */   322,  327,  341,  245,  414,  337,  544,  544,  318,  544,
 /*  5790 */   251,  544,  260,  263,  544,  544,  348,  544,  544,  544,
 /*  5800 */   330,  336,  544,  308,  314,  544,  544,  544,  329,  331,
 /*  5810 */   332,  333,  334,  335,  544,  544,  254,  257,  248,  322,
 /*  5820 */   327,  341,  245,  364,  337,  544,  544,  318,  544,  251,
 /*  5830 */   544,  260,  263,  544,  544,  348,  544,  544,  544,  330,
 /*  5840 */   336,  544,  308,  314,  544,  544,  544,  329,  331,  332,
 /*  5850 */   333,  334,  335,  544,  544,  254,  257,  248,  322,  327,
 /*  5860 */   341,  245,  544,  337,  544,  544,  318,  544,  251,  544,
 /*  5870 */   260,  263,  544,  544,  348,  544,  544,  544,  330,  336,
 /*  5880 */   544,  308,  314,  544,  544,  544,  329,  331,  332,  333,
 /*  5890 */   334,  335,  544,  544,  254,  257,  248,  322,  327,  341,
 /*  5900 */   245,  544,  337,  544,  544,  318,  544,  544,  544,  579,
 /*  5910 */   544,  544,  544,  348,  544,  544,  544,  330,  336,  544,
 /*  5920 */   308,  544,  544,  544,  544,  329,  331,  332,  333,  334,
 /*  5930 */   335,  288,  290,  294,  292,  296,  298,  300,  247,  253,
 /*  5940 */   259,  262,  265,  267,  256,  250,  276,  278,  286,  280,
 /*  5950 */   282,  284,  544,  544,  544,  544,  330,  544,  544,  544,
 /*  5960 */   273,  269,  544,  544,  329,  331,  332,  333,  334,  335,
 /*  5970 */   544,  544,  251,  544,  260,  263,  544,  544,  544,  544,
 /*  5980 */   544,  544,  544,  544,  544,  544,  314,  544,  100,  544,
 /*  5990 */   544,  544,  544,  544,  544,  544,  544,  544,  254,  257,
 /*  6000 */   248,  322,  327,  341,  245,  544,  337,   29,  544,  339,
 /*  6010 */   544,   68,  544,  544,  544,  544,  544,  348,  544,  544,
 /*  6020 */   544,  544,  336,   84,  308,  116,  117,  119,  118,  120,
 /*  6030 */    47,   48,   49,   50,   51,   52,   53,   54,   55,   56,
 /*  6040 */    57,  544,  544,  544,  100,  544,  544,  544,  544,  544,
 /*  6050 */   544,  544,  544,  544,  544,  544,  544,  544,  544,  544,
 /*  6060 */   384,  544,  100,  176,  544,  544,  544,   68,  383,  331,
 /*  6070 */   332,  333,  334,  335,  122,  123,  544,  747,  650,   84,
 /*  6080 */   149,  129,  544,  544,  121,   68,   47,   48,   49,   50,
 /*  6090 */    51,   52,   53,   54,   55,   56,   57,   84,  544,  544,
 /*  6100 */   544,  100,  544,  544,   47,   48,   49,   50,   51,   52,
 /*  6110 */    53,   54,   55,   56,   57,  100,  544,  544,  544,  544,
 /*  6120 */   624,  544,  544,  544,   68,  544,  544,  484,  445,  544,
 /*  6130 */   544,  544,  544,  544,  544,  544,   84,  544,   85,  544,
 /*  6140 */   544,  435,  544,   47,   48,   49,   50,   51,   52,   53,
 /*  6150 */    54,   55,   56,   57,  544,  544,  544,   47,   48,   49,
 /*  6160 */    50,   51,   52,   53,   54,   55,   56,   57,  116,  117,
 /*  6170 */   119,  118,  120,  544,  544,  436,  437,  438,  439,  440,
 /*  6180 */   441,  544,  473,  491,  492,  116,  117,  119,  118,  120,
 /*  6190 */   544,  544,  544,  544,  544,  544,  544,  544,  544,  544,
 /*  6200 */   544,  544,  544,  544,  544,  544,  544,  544,  544,  544,
 /*  6210 */   544,  544,  544,  544,  544,  544,  544,  122,  123,  544,
 /*  6220 */   743,  652,  544,  149,  544,  544,  544,  121,  544,  116,
 /*  6230 */   117,  119,  118,  120,  122,  123,  544,  744,  652,  544,
 /*  6240 */   149,  544,  544,  544,  121,  544,  116,  117,  119,  118,
 /*  6250 */   120,  544,  544,  544,  544,  544,  544,  544,  544,  544,
 /*  6260 */   544,  544,  544,  544,  544,  544,  544,  544,  544,  544,
 /*  6270 */   544,  544,  544,  544,  544,  544,  544,  544,  122,  123,
 /*  6280 */   544,  752,  143,  544,  149,  544,  544,  544,  121,  544,
 /*  6290 */   116,  117,  119,  118,  120,  122,  123,  544,   19,  143,
 /*  6300 */   544,  149,  544,  544,  544,  121,  544,  116,  117,  119,
 /*  6310 */   118,  120,  116,  117,  119,  118,  120,  544,  544,  544,
 /*  6320 */   544,  544,  544,  544,  544,  544,  544,  544,  544,  544,
 /*  6330 */   544,  544,  544,  544,  544,  544,  544,  544,  544,  122,
 /*  6340 */   123,  544,  166,  652,  544,  149,  544,  544,  544,  121,
 /*  6350 */   544,  116,  117,  119,  118,  120,  122,  123,  544,  753,
 /*  6360 */   124,  122,  123,  544,  754,  124,  121,  544,  544,  544,
 /*  6370 */   544,  121,  544,  116,  117,  119,  118,  120,  116,  117,
 /*  6380 */   119,  118,  120,  544,  544,  544,  544,  544,  544,  544,
 /*  6390 */   544,  544,  544,  544,  544,  544,  544,  544,  544,  544,
 /*  6400 */   122,  123,  544,  745,  619,  116,  117,  119,  118,  120,
 /*  6410 */   121,  544,  544,  544,  544,  544,  544,  544,  544,  544,
 /*  6420 */   544,  544,  122,  123,  544,  751,  619,  122,  123,  544,
 /*  6430 */   746,  619,  121,  544,  544,  544,  544,  121,  544,  116,
 /*  6440 */   117,  119,  118,  120,  544,  544,  544,  544,  654,  544,
 /*  6450 */   544,  544,  544,  544,  122,  123,  116,  117,  119,  118,
 /*  6460 */   120,  544,  621,  544,  121,  544,  544,  544,  544,  544,
 /*  6470 */   544,  544,  544,  544,  544,  544,  544,  544,  544,  544,
 /*  6480 */   544,  544,  544,  544,  544,  544,  544,  544,  122,  123,
 /*  6490 */   544,  750,  619,  116,  117,  119,  118,  120,  121,  116,
 /*  6500 */   117,  119,  118,  120,  544,  122,  123,  544,  749,  619,
 /*  6510 */   116,  117,  119,  118,  120,  121,  116,  117,  119,  118,
 /*  6520 */   120,  544,  544,  544,  544,  544,  544,  544,  544,  544,
 /*  6530 */   544,  544,  544,  544,  544,  544,  674,  544,  544,  544,
 /*  6540 */   544,  544,  122,  123,  544,  544,  544,  544,  122,  123,
 /*  6550 */   173,  544,  121,  544,  544,  544,  621,  544,  121,  122,
 /*  6560 */   123,  544,  748,  686,  544,  122,  123,  544,  544,  121,
 /*  6570 */   544,  144,  544,  544,  544,  121,  544,  116,  117,  119,
 /*  6580 */   118,  120,  116,  117,  119,  118,  120,  544,  116,  117,
 /*  6590 */   119,  118,  120,  544,  544,  544,  544,  544,  544,  544,
 /*  6600 */   544,  544,  544,  544,  544,  544,  544,  544,  544,  544,
 /*  6610 */   544,  544,  544,  544,  544,  544,  544,  544,  544,  544,
 /*  6620 */   544,  544,  544,  544,  544,  544,  122,  123,  544,  544,
 /*  6630 */   544,  122,  123,  544,   26,  544,  121,  122,  123,  126,
 /*  6640 */   544,  121,  544,  544,  544,  173,  544,  121,
};
static YYCODETYPE yy_lookahead[] = {
 /*     0 */     7,    6,    9,   10,  111,  112,  113,  114,  115,  116,
 /*    10 */   117,  118,  119,  141,   21,  143,  144,  145,  146,  147,
 /*    20 */    41,    6,  176,   44,  178,  179,   33,   34,   35,   36,
 /*    30 */    37,   38,   39,    6,   41,   40,   57,   44,   45,   27,
 /*    40 */    28,   29,   30,   31,   32,   52,   53,   54,  145,  146,
 /*    50 */    57,  148,   59,   41,   42,   40,   63,   64,   65,   66,
 /*    60 */    67,   68,   69,   70,   71,   44,   73,   74,   75,   76,
 /*    70 */    44,   78,   45,   52,   95,   82,   83,   84,   85,   53,
 /*    80 */    54,   88,  103,  104,  105,  106,  107,  108,   95,   41,
 /*    90 */    42,   44,   99,  100,  101,  102,  103,  104,  105,  106,
 /*   100 */   107,  108,  109,    7,   57,    9,   10,  123,   55,   55,
 /*   110 */   126,  127,  128,  131,  130,  131,  139,   21,    6,  137,
 /*   120 */   136,  137,  145,  146,  142,  148,  142,    6,   53,   33,
 /*   130 */    34,   35,   36,   37,   38,   39,  176,   41,  178,  179,
 /*   140 */    44,   45,   89,   90,   91,   92,   93,   94,   52,   53,
 /*   150 */    54,   45,    0,   57,   79,   59,   81,   45,   52,   63,
 /*   160 */    64,   65,   66,   67,   68,   69,   70,   71,   44,   73,
 /*   170 */    74,   75,   76,   41,   78,  131,  139,    6,   82,   83,
 /*   180 */    84,   85,  145,  146,   88,  148,  142,   55,   47,  176,
 /*   190 */    49,   95,  179,   52,   45,   99,  100,  101,  102,  103,
 /*   200 */   104,  105,  106,  107,  108,  109,    7,   86,    9,   10,
 /*   210 */   141,   40,  143,  144,  145,  146,  147,  134,  135,   95,
 /*   220 */    21,   89,   90,   91,   92,   93,   94,   44,   42,   97,
 /*   230 */    98,   45,   33,   34,   35,   36,   37,   38,   39,  176,
 /*   240 */    41,  178,  179,   44,   45,  127,  128,   42,  130,  131,
 /*   250 */    45,   52,   53,   54,  136,  137,   57,  122,   59,  124,
 /*   260 */   142,   44,   63,   64,   65,   66,   67,   68,   69,   70,
 /*   270 */    71,   54,   73,   74,   75,   76,   41,   78,  187,  188,
 /*   280 */     6,   82,   83,   84,   85,  131,   47,   88,   49,    6,
 /*   290 */    55,   52,  138,   44,   95,  120,  142,   59,   99,  100,
 /*   300 */   101,  102,  103,  104,  105,  106,  107,  108,  109,    7,
 /*   310 */   176,    9,   10,  179,    6,    6,   53,  170,   80,   45,
 /*   320 */   173,  171,  172,   21,   89,   90,   91,   92,   93,   94,
 /*   330 */   183,  184,  185,  186,   96,   33,   34,   35,   36,   37,
 /*   340 */    38,   39,   79,   41,   81,   62,   44,   45,  114,  115,
 /*   350 */   116,  117,  118,  119,   52,   53,   54,    6,   39,   57,
 /*   360 */    52,   59,   53,   44,   42,   63,   64,   65,   66,   67,
 /*   370 */    68,   69,   70,   71,   52,   73,   74,   75,   76,   41,
 /*   380 */    78,   45,   45,    6,   82,   83,   84,   85,   52,   52,
 /*   390 */    88,   55,   55,   55,  187,  188,   45,   95,  187,  188,
 /*   400 */    59,   99,  100,  101,  102,  103,  104,  105,  106,  107,
 /*   410 */   108,  109,    7,   44,    9,   10,  187,  188,  187,  188,
 /*   420 */   170,   52,   45,  173,    6,   44,   21,   89,   90,   91,
 /*   430 */    92,   93,   94,  183,  184,  185,  186,   96,   33,   34,
 /*   440 */    35,   36,   37,   38,   39,    6,   41,  187,  188,   44,
 /*   450 */    45,  187,  188,   45,   30,   31,   32,   52,   53,   54,
 /*   460 */    52,   52,   57,   45,   59,   41,   42,    6,   63,   64,
 /*   470 */    65,   66,   67,   68,   69,   70,   71,    6,   73,   74,
 /*   480 */    75,   76,    6,   78,   45,  173,  120,   82,   83,   84,
 /*   490 */    85,  187,  188,   88,   39,  183,  184,  185,  186,   44,
 /*   500 */    95,   40,  190,  191,   99,  100,  101,  102,  103,  104,
 /*   510 */   105,  106,  107,  108,  109,    7,   45,    9,   10,   53,
 /*   520 */   126,   45,  128,   59,  130,  131,  139,  173,  142,   21,
 /*   530 */   136,  137,  145,  146,  180,  148,  142,  183,  184,  185,
 /*   540 */   186,   33,   34,   35,   36,   37,   38,   39,  131,   41,
 /*   550 */   187,  188,   44,   45,   44,  128,    6,  130,  131,  142,
 /*   560 */    52,   53,   54,   47,  137,   57,   22,   59,   52,  142,
 /*   570 */     6,   63,   64,   65,   66,   67,   68,   69,   70,   71,
 /*   580 */     6,   73,   74,   75,   76,   41,   78,   42,  173,   47,
 /*   590 */    82,   83,   84,   85,   52,   45,   88,   52,  183,  184,
 /*   600 */   185,  186,   45,   95,   40,  190,  191,   99,  100,  101,
 /*   610 */   102,  103,  104,  105,  106,  107,  108,  109,    7,   45,
 /*   620 */     9,   10,  125,   39,  127,    6,  129,  131,  131,   14,
 /*   630 */   173,   42,   21,  136,   45,  138,  187,  188,  142,  142,
 /*   640 */   183,  184,  185,  186,   33,   34,   35,   36,   37,   38,
 /*   650 */    39,   42,   41,  174,  175,   44,   45,  145,  146,   40,
 /*   660 */   129,   52,  131,   52,   53,   54,    6,  136,   57,  138,
 /*   670 */    59,   44,   44,  142,   63,   64,   65,   66,   67,   68,
 /*   680 */    69,   70,   71,   62,   73,   74,   75,   76,   42,   78,
 /*   690 */    22,  173,   44,   82,   83,   84,   85,   59,   52,   88,
 /*   700 */    52,  183,  184,  185,  186,   45,   95,    6,  190,  191,
 /*   710 */    99,  100,  101,  102,  103,  104,  105,  106,  107,  108,
 /*   720 */   109,    7,   95,    9,   10,   43,    6,  173,   46,  122,
 /*   730 */    48,  124,   50,   51,   96,   21,   54,  183,  184,  185,
 /*   740 */   186,   40,    6,   45,  190,  191,   44,   33,   34,   35,
 /*   750 */    36,   37,   38,   39,   52,   41,  139,  132,   44,   45,
 /*   760 */    40,  128,  145,  146,  131,  148,   52,   53,   54,  136,
 /*   770 */   137,   57,  122,   59,  124,  142,   40,   63,   64,   65,
 /*   780 */    66,   67,   68,   69,   70,   71,   44,   73,   74,   75,
 /*   790 */    76,  109,   78,   55,   52,  173,   82,   83,   84,   85,
 /*   800 */    96,    6,   88,  181,  182,  183,  184,  185,  186,   95,
 /*   810 */     6,   57,   62,   99,  100,  101,  102,  103,  104,  105,
 /*   820 */   106,  107,  108,  109,    7,    6,    9,   10,  141,  173,
 /*   830 */   143,  144,  145,  146,  147,   40,   55,  132,   21,  183,
 /*   840 */   184,  185,  186,  132,   40,   49,  190,  191,   52,   44,
 /*   850 */    33,   34,   35,   36,   37,   38,   39,   55,   41,   40,
 /*   860 */     6,   44,   45,  144,  145,  146,  147,  132,   44,   52,
 /*   870 */    53,   54,   42,   55,   57,   45,   59,    6,  132,   44,
 /*   880 */    63,   64,   65,   66,   67,   68,   69,   70,   71,   22,
 /*   890 */    73,   74,   75,   76,   40,   78,   55,  173,  132,   82,
 /*   900 */    83,   84,   85,  176,   44,   88,  179,  183,  184,  185,
 /*   910 */   186,   40,   95,   55,  190,  191,   99,  100,  101,  102,
 /*   920 */   103,  104,  105,  106,  107,  108,  109,    7,   44,    9,
 /*   930 */    10,  141,  173,  143,  144,  145,  146,  147,  132,  132,
 /*   940 */    55,   21,  183,  184,  185,  186,   44,   59,   45,  190,
 /*   950 */   191,   45,   57,   33,   34,   35,   36,   37,   38,   39,
 /*   960 */    55,   41,   45,  132,   44,   45,   57,   55,  132,   45,
 /*   970 */    44,  121,   52,   53,   54,   44,  121,   57,   44,   59,
 /*   980 */   121,   53,   44,   63,   64,   65,   66,   67,   68,   69,
 /*   990 */    70,   71,   59,   73,   74,   75,   76,   45,   78,   45,
 /*  1000 */   173,   52,   82,   83,   84,   85,   77,   52,   88,   52,
 /*  1010 */   183,  184,  185,  186,  172,   95,   80,  190,  191,   99,
 /*  1020 */   100,  101,  102,  103,  104,  105,  106,  107,  108,  109,
 /*  1030 */     7,  132,    9,   10,  141,  173,  143,  144,  145,  146,
 /*  1040 */   147,   52,   52,   52,   21,  183,  184,  185,  186,   83,
 /*  1050 */    45,   59,  190,  191,   40,   59,   33,   34,   35,   36,
 /*  1060 */    37,   38,   39,   44,   41,   22,   44,   44,   45,    6,
 /*  1070 */    44,   59,   59,   53,   44,   52,   53,   54,   53,   44,
 /*  1080 */    57,   53,   59,   59,   80,   44,   63,   64,   65,   66,
 /*  1090 */    67,   68,   69,   70,   71,   59,   73,   74,   75,   76,
 /*  1100 */    53,   78,   53,  173,   59,   82,   83,   84,   85,   80,
 /*  1110 */    59,   88,   80,  183,  184,  185,  186,  175,   95,  176,
 /*  1120 */   190,  191,   99,  100,  101,  102,  103,  104,  105,  106,
 /*  1130 */   107,  108,  109,    7,   62,    9,   10,   53,  173,  176,
 /*  1140 */    53,   44,  177,  176,  176,  176,   44,   21,  183,  184,
 /*  1150 */   185,  186,  141,   53,  143,  144,  145,  146,  147,   33,
 /*  1160 */    34,   35,   36,   37,   38,   39,   45,   41,   45,  176,
 /*  1170 */    44,   45,   45,   45,   44,  188,  132,   55,   52,   53,
 /*  1180 */    54,   44,   86,   57,   83,   59,   45,   80,   77,   63,
 /*  1190 */    64,   65,   66,   67,   68,   69,   70,   71,   52,   73,
 /*  1200 */    74,   75,   76,   52,   78,   52,  173,   44,   82,   83,
 /*  1210 */    84,   85,   52,  180,   88,   59,  183,  184,  185,  186,
 /*  1220 */    52,   95,  133,  132,   45,   99,  100,  101,  102,  103,
 /*  1230 */   104,  105,  106,  107,  108,  109,    7,  133,    9,   10,
 /*  1240 */    45,  173,   45,  135,   44,  177,   45,   45,  133,  132,
 /*  1250 */    21,  183,  184,  185,  186,  141,   45,  143,  144,  145,
 /*  1260 */   146,  147,   33,   34,   35,   36,   37,   38,   39,  124,
 /*  1270 */    41,  133,   48,   44,   45,  121,   44,   44,  121,   52,
 /*  1280 */   121,   52,   53,   54,   48,  121,   57,   44,   59,   44,
 /*  1290 */   121,  121,   63,   64,   65,   66,   67,   68,   69,   70,
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
 /*  1400 */    68,   69,   70,   71,  195,   73,   74,   75,   76,  195,
 /*  1410 */    78,  195,  173,  195,   82,   83,   84,   85,  195,  195,
 /*  1420 */    88,  195,  183,  184,  185,  186,  195,   95,  195,  195,
 /*  1430 */   191,   99,  100,  101,  102,  103,  104,  105,  106,  107,
 /*  1440 */   108,  109,    7,  195,    9,   10,  195,  173,  195,  195,
 /*  1450 */   195,  177,  195,  195,  195,  195,   21,  183,  184,  185,
 /*  1460 */   186,  195,  195,  195,  195,  195,  195,  195,   33,   34,
 /*  1470 */    35,   36,   37,   38,   39,  195,   41,  195,  195,   44,
 /*  1480 */    45,  195,  195,  195,  195,  195,  195,   52,   53,   54,
 /*  1490 */   195,  195,   57,  195,   59,  195,  195,  195,   63,   64,
 /*  1500 */    65,   66,   67,   68,   69,   70,   71,  195,   73,   74,
 /*  1510 */    75,   76,  170,   78,  195,  173,  195,   82,   83,   84,
 /*  1520 */    85,  195,  145,   88,  195,  183,  184,  185,  186,  195,
 /*  1530 */    95,  195,  195,  195,   99,  100,  101,  102,  103,  104,
 /*  1540 */   105,  106,  107,  108,  109,    7,  195,    9,   10,  195,
 /*  1550 */   173,  195,  195,  195,  195,  195,  195,  195,  195,   21,
 /*  1560 */   183,  184,  185,  186,  195,  195,  195,  195,  195,  195,
 /*  1570 */   195,   33,   34,   35,   36,   37,   38,   39,  195,   41,
 /*  1580 */   195,  195,   44,   45,  195,  195,  195,  195,  195,  195,
 /*  1590 */    52,   53,   54,  195,  195,   57,  195,   59,  195,  195,
 /*  1600 */   195,   63,   64,   65,   66,   67,   68,   69,   70,   71,
 /*  1610 */   195,   73,   74,   75,   76,  195,   78,  195,  173,  195,
 /*  1620 */    82,   83,   84,   85,  195,  195,   88,  182,  183,  184,
 /*  1630 */   185,  186,  195,   95,  195,  195,  195,   99,  100,  101,
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
 /*  3530 */   173,  195,  118,  119,  195,  173,  183,  184,  185,  186,
 /*  3540 */   183,  184,  185,  186,  195,  183,  184,  185,  186,  192,
 /*  3550 */   193,  194,  195,  195,  140,  195,  194,  195,  195,  195,
 /*  3560 */   195,  195,  195,  149,  150,  151,  152,  153,  154,  155,
 /*  3570 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  165,
 /*  3580 */   166,  167,  168,  169,  118,  119,  195,  173,  195,  195,
 /*  3590 */   195,  173,  195,  195,  195,  195,  195,  183,  184,  185,
 /*  3600 */   186,  183,  184,  185,  186,  195,  140,  195,  195,  195,
 /*  3610 */   195,  195,  195,  195,  195,  149,  150,  151,  152,  153,
 /*  3620 */   154,  155,  156,  157,  158,  159,  160,  161,  162,  163,
 /*  3630 */   164,  165,  166,  167,  168,  169,  118,  119,  195,  173,
 /*  3640 */   195,  195,  195,  173,  195,  195,  195,  195,  195,  183,
 /*  3650 */   184,  185,  186,  183,  184,  185,  186,  195,  140,  195,
 /*  3660 */   195,  195,  195,  195,  195,  195,  195,  149,  150,  151,
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
 /*  4470 */   195,  173,  195,  195,  195,  195,  195,  195,  195,  195,
 /*  4480 */   195,  183,  184,  185,  186,  195,  195,  195,  195,  195,
 /*  4490 */   140,  195,  195,  195,  195,  195,  195,  195,  195,  149,
 /*  4500 */   150,  151,  152,  153,  154,  155,  156,  157,  158,  159,
 /*  4510 */   160,  161,  162,  163,  164,  165,  166,  167,  168,  169,
 /*  4520 */   118,  119,  195,  173,  195,  195,  195,  195,  195,  195,
 /*  4530 */   195,  195,  195,  183,  184,  185,  186,  195,  195,  195,
 /*  4540 */   195,  195,  195,  195,  195,  195,  195,  195,  195,  195,
 /*  4550 */   195,  149,  150,  151,  152,  153,  154,  155,  156,  157,
 /*  4560 */   158,  159,  160,  161,  162,  163,  164,  165,  166,  167,
 /*  4570 */   168,  169,  195,  195,  195,  173,  195,  195,  195,  195,
 /*  4580 */   195,  195,  195,  195,  195,  183,  184,  185,  186,  195,
 /*  4590 */   195,  195,  195,  195,    8,  195,  195,   11,   12,   13,
 /*  4600 */    14,   15,   16,   17,   18,   19,   20,   21,   22,   23,
 /*  4610 */    24,   25,   26,   27,   28,   29,   30,   31,   32,  195,
 /*  4620 */   195,  195,  195,  195,  195,  195,  195,   41,   42,  195,
 /*  4630 */   195,   45,  195,  195,  195,  195,    8,  195,  195,   11,
 /*  4640 */    12,   13,   14,   15,   16,   17,   18,   19,   20,   21,
 /*  4650 */    22,   23,   24,   25,   26,   27,   28,   29,   30,   31,
 /*  4660 */    32,  195,  195,  195,  195,  195,  195,  195,  195,   41,
 /*  4670 */    42,   19,   20,   21,   22,   23,   24,   25,   26,   27,
 /*  4680 */    28,   29,   30,   31,   32,  195,  195,  195,  195,  195,
 /*  4690 */    62,  195,  195,   41,   42,  195,  195,  195,  195,    8,
 /*  4700 */   195,  195,   11,   12,   13,   14,   15,   16,   17,   18,
 /*  4710 */    19,   20,   21,   22,   23,   24,   25,   26,   27,   28,
 /*  4720 */    29,   30,   31,   32,  195,  195,  195,  195,  195,  195,
 /*  4730 */   195,  195,   41,   42,  195,  195,   21,  195,  195,  195,
 /*  4740 */   195,    8,  195,   52,   11,   12,   13,   14,   15,   16,
 /*  4750 */    17,   18,   19,   20,   21,   22,   23,   24,   25,   26,
 /*  4760 */    27,   28,   29,   30,   31,   32,  195,  195,  195,  195,
 /*  4770 */   195,  195,  195,  195,   41,   42,   61,  195,   63,   64,
 /*  4780 */    65,   66,   67,   68,   69,   70,   71,   72,   73,  195,
 /*  4790 */   195,  195,  195,  195,  195,   62,  195,  195,  195,  195,
 /*  4800 */   195,  195,  195,  195,    8,  195,  195,   11,   12,   13,
 /*  4810 */    14,   15,   16,   17,   18,   19,   20,   21,   22,   23,
 /*  4820 */    24,   25,   26,   27,   28,   29,   30,   31,   32,  195,
 /*  4830 */   195,  195,  195,  195,  195,  195,    8,   41,   42,   11,
 /*  4840 */    12,   13,   14,   15,   16,   17,   18,   19,   20,   21,
 /*  4850 */    22,   23,   24,   25,   26,   27,   28,   29,   30,   31,
 /*  4860 */    32,  195,  195,  195,  195,  195,  195,  195,  195,   41,
 /*  4870 */    42,  195,  195,  195,  195,  195,   80,  195,    8,  195,
 /*  4880 */    52,   11,   12,   13,   14,   15,   16,   17,   18,   19,
 /*  4890 */    20,   21,   22,   23,   24,   25,   26,   27,   28,   29,
 /*  4900 */    30,   31,   32,  195,  195,  195,  195,  195,  195,  195,
 /*  4910 */   195,   41,   42,  195,  195,  195,  195,  195,  195,  195,
 /*  4920 */     8,  195,   52,   11,   12,   13,   14,   15,   16,   17,
 /*  4930 */    18,   19,   20,   21,   22,   23,   24,   25,   26,   27,
 /*  4940 */    28,   29,   30,   31,   32,  195,  195,  195,  195,  195,
 /*  4950 */   195,  195,  195,   41,   42,  195,  195,   45,  195,  195,
 /*  4960 */   195,  195,    8,  195,  195,   11,   12,   13,   14,   15,
 /*  4970 */    16,   17,   18,   19,   20,   21,   22,   23,   24,   25,
 /*  4980 */    26,   27,   28,   29,   30,   31,   32,  195,  195,  195,
 /*  4990 */   195,  195,  195,  195,  195,   41,   42,  195,  195,  195,
 /*  5000 */   195,  195,  195,  195,    8,  195,   52,   11,   12,   13,
 /*  5010 */    14,   15,   16,   17,   18,   19,   20,   21,   22,   23,
 /*  5020 */    24,   25,   26,   27,   28,   29,   30,   31,   32,  195,
 /*  5030 */   195,  195,  195,  195,  195,  195,  195,   41,   42,  195,
 /*  5040 */   195,   45,  195,  195,  195,  195,    8,  195,  195,   11,
 /*  5050 */    12,   13,   14,   15,   16,   17,   18,   19,   20,   21,
 /*  5060 */    22,   23,   24,   25,   26,   27,   28,   29,   30,   31,
 /*  5070 */    32,  195,  195,  195,  195,  195,  195,  195,  195,   41,
 /*  5080 */    42,  195,  195,   45,  195,  195,  195,  195,    8,  195,
 /*  5090 */   195,   11,   12,   13,   14,   15,   16,   17,   18,   19,
 /*  5100 */    20,   21,   22,   23,   24,   25,   26,   27,   28,   29,
 /*  5110 */    30,   31,   32,  195,  195,  195,  195,  195,  195,  195,
 /*  5120 */    40,   41,   42,  195,  195,  195,  195,   21,  195,    8,
 /*  5130 */   195,  195,   11,   12,   13,   14,   15,   16,   17,   18,
 /*  5140 */    19,   20,   21,   22,   23,   24,   25,   26,   27,   28,
 /*  5150 */    29,   30,   31,   32,    7,  195,    9,   10,  195,  195,
 /*  5160 */   195,  195,   41,   42,  195,  195,   21,  195,   21,   63,
 /*  5170 */    64,   65,   66,   67,   68,   69,   70,   71,   72,   73,
 /*  5180 */    33,   34,   35,   36,   37,   38,   39,   40,   41,   44,
 /*  5190 */   195,   44,  195,    7,  195,    9,   10,  195,  195,   52,
 /*  5200 */   195,   56,  195,  195,   57,  195,   59,   21,   63,   64,
 /*  5210 */    65,   66,   67,   68,   69,   70,   71,   72,   73,   33,
 /*  5220 */    34,   35,   36,   37,   38,   39,   40,   41,  195,  195,
 /*  5230 */    44,  195,    7,  195,    9,   10,  195,  195,   52,  195,
 /*  5240 */   195,  195,   95,   57,  195,   59,   21,  195,  195,  195,
 /*  5250 */   103,  104,  105,  106,  107,  108,  195,  195,   33,   34,
 /*  5260 */    35,   36,   37,   38,   39,   40,   41,  195,  195,   44,
 /*  5270 */   195,    7,  195,    9,   10,  195,  195,   52,  195,  195,
 /*  5280 */   195,   95,   57,  195,   59,   21,  195,  195,  195,  103,
 /*  5290 */   104,  105,  106,  107,  108,  195,  195,   33,   34,   35,
 /*  5300 */    36,   37,   38,   39,  195,   41,  195,  195,   44,  195,
 /*  5310 */     7,  195,    9,   10,  195,  195,   52,  195,  195,  195,
 /*  5320 */    95,   57,  195,   59,   21,  195,   62,  195,  103,  104,
 /*  5330 */   105,  106,  107,  108,  195,  195,   33,   34,   35,   36,
 /*  5340 */    37,   38,   39,   40,   41,  195,  195,   44,  195,    7,
 /*  5350 */   195,    9,   10,  195,  195,   52,  195,  195,  195,   95,
 /*  5360 */    57,  195,   59,   21,  195,  195,  195,  103,  104,  105,
 /*  5370 */   106,  107,  108,  195,  195,   33,   34,   35,   36,   37,
 /*  5380 */    38,   39,  195,   41,  195,  195,   44,   45,    7,  195,
 /*  5390 */     9,   10,  195,  195,   52,  195,  195,  195,   95,   57,
 /*  5400 */   195,   59,   21,  195,  195,  195,  103,  104,  105,  106,
 /*  5410 */   107,  108,  195,  195,   33,   34,   35,   36,   37,   38,
 /*  5420 */    39,   40,   41,  195,  195,   44,  195,  195,  195,  195,
 /*  5430 */   195,  195,  195,   52,  195,  195,  195,   95,   57,  195,
 /*  5440 */    59,  195,  195,  195,  195,  103,  104,  105,  106,  107,
 /*  5450 */   108,  195,   11,   12,   13,   14,   15,   16,   17,   18,
 /*  5460 */    19,   20,   21,   22,   23,   24,   25,   26,   27,   28,
 /*  5470 */    29,   30,   31,   32,  195,  195,   95,    7,  195,    9,
 /*  5480 */    10,  195,   41,   42,  103,  104,  105,  106,  107,  108,
 /*  5490 */   195,   21,  195,  195,  195,  195,  195,  195,  195,  195,
 /*  5500 */   195,  195,  195,   33,   34,   35,   36,   37,   38,   39,
 /*  5510 */   195,   41,  195,  195,   44,  195,    7,  195,    9,   10,
 /*  5520 */   195,  195,   52,  195,  195,  195,  195,   57,  195,   59,
 /*  5530 */    21,  195,   62,  195,  195,  195,  195,  195,  195,  195,
 /*  5540 */   195,  195,   33,   34,   35,   36,   37,   38,   39,   40,
 /*  5550 */    41,  195,  195,   44,  195,    7,  195,    9,   10,  195,
 /*  5560 */   195,   52,  195,  195,  195,   95,   57,  195,   59,   21,
 /*  5570 */   195,  195,  195,  103,  104,  105,  106,  107,  108,  195,
 /*  5580 */   195,   33,   34,   35,   36,   37,   38,   39,   40,   41,
 /*  5590 */   195,  195,   44,  195,    7,  195,    9,   10,  195,  195,
 /*  5600 */    52,  195,  195,  195,   95,   57,  195,   59,   21,  195,
 /*  5610 */   195,  195,  103,  104,  105,  106,  107,  108,  195,  195,
 /*  5620 */    33,   34,   35,   36,   37,   38,   39,   40,   41,  195,
 /*  5630 */   195,   44,  195,    7,  195,    9,   10,  195,  195,   52,
 /*  5640 */   195,  195,  195,   95,   57,  195,   59,   21,  195,  195,
 /*  5650 */   195,  103,  104,  105,  106,  107,  108,  195,  195,   33,
 /*  5660 */    34,   35,   36,   37,   38,   39,  195,   41,  195,  195,
 /*  5670 */    44,  195,    7,  195,    9,   10,  195,  195,   52,  195,
 /*  5680 */   195,  195,   95,   57,  195,   59,   21,  195,  195,  195,
 /*  5690 */   103,  104,  105,  106,  107,  108,  195,  195,   33,   34,
 /*  5700 */    35,   36,   37,   38,   39,  195,   41,  195,  195,   44,
 /*  5710 */   195,  195,    7,   87,    9,   10,  195,   52,  195,  195,
 /*  5720 */   195,   95,   57,  195,   59,  195,   21,   62,  195,  103,
 /*  5730 */   104,  105,  106,  107,  108,  195,  195,  195,   33,   34,
 /*  5740 */    35,   36,   37,   38,   39,  195,   41,  195,  195,   44,
 /*  5750 */   195,    7,  195,    9,   10,  195,  195,   52,  195,  195,
 /*  5760 */    95,  195,   57,  195,   59,   21,  195,   62,  103,  104,
 /*  5770 */   105,  106,  107,  108,  195,  195,  195,   33,   34,   35,
 /*  5780 */    36,   37,   38,   39,   40,   41,  195,  195,   44,  195,
 /*  5790 */     7,  195,    9,   10,  195,  195,   52,  195,  195,  195,
 /*  5800 */    95,   57,  195,   59,   21,  195,  195,  195,  103,  104,
 /*  5810 */   105,  106,  107,  108,  195,  195,   33,   34,   35,   36,
 /*  5820 */    37,   38,   39,   40,   41,  195,  195,   44,  195,    7,
 /*  5830 */   195,    9,   10,  195,  195,   52,  195,  195,  195,   95,
 /*  5840 */    57,  195,   59,   21,  195,  195,  195,  103,  104,  105,
 /*  5850 */   106,  107,  108,  195,  195,   33,   34,   35,   36,   37,
 /*  5860 */    38,   39,  195,   41,  195,  195,   44,  195,    7,  195,
 /*  5870 */     9,   10,  195,  195,   52,  195,  195,  195,   95,   57,
 /*  5880 */   195,   59,   21,  195,  195,  195,  103,  104,  105,  106,
 /*  5890 */   107,  108,  195,  195,   33,   34,   35,   36,   37,   38,
 /*  5900 */    39,  195,   41,  195,  195,   44,  195,  195,  195,   87,
 /*  5910 */   195,  195,  195,   52,  195,  195,  195,   95,   57,  195,
 /*  5920 */    59,  195,  195,  195,  195,  103,  104,  105,  106,  107,
 /*  5930 */   108,   12,   13,   14,   15,   16,   17,   18,   19,   20,
 /*  5940 */    21,   22,   23,   24,   25,   26,   27,   28,   29,   30,
 /*  5950 */    31,   32,  195,  195,  195,  195,   95,  195,  195,  195,
 /*  5960 */    41,   42,  195,  195,  103,  104,  105,  106,  107,  108,
 /*  5970 */   195,  195,    7,  195,    9,   10,  195,  195,  195,  195,
 /*  5980 */   195,  195,  195,  195,  195,  195,   21,  195,   21,  195,
 /*  5990 */   195,  195,  195,  195,  195,  195,  195,  195,   33,   34,
 /*  6000 */    35,   36,   37,   38,   39,  195,   41,   40,  195,   44,
 /*  6010 */   195,   44,  195,  195,  195,  195,  195,   52,  195,  195,
 /*  6020 */   195,  195,   57,   56,   59,    1,    2,    3,    4,    5,
 /*  6030 */    63,   64,   65,   66,   67,   68,   69,   70,   71,   72,
 /*  6040 */    73,  195,  195,  195,   21,  195,  195,  195,  195,  195,
 /*  6050 */   195,  195,  195,  195,  195,  195,  195,  195,  195,  195,
 /*  6060 */    95,  195,   21,   40,  195,  195,  195,   44,  103,  104,
 /*  6070 */   105,  106,  107,  108,   50,   51,  195,   53,   54,   56,
 /*  6080 */    56,   40,  195,  195,   60,   44,   63,   64,   65,   66,
 /*  6090 */    67,   68,   69,   70,   71,   72,   73,   56,  195,  195,
 /*  6100 */   195,   21,  195,  195,   63,   64,   65,   66,   67,   68,
 /*  6110 */    69,   70,   71,   72,   73,   21,  195,  195,  195,  195,
 /*  6120 */    40,  195,  195,  195,   44,  195,  195,   41,   42,  195,
 /*  6130 */   195,  195,  195,  195,  195,  195,   56,  195,   44,  195,
 /*  6140 */   195,   55,  195,   63,   64,   65,   66,   67,   68,   69,
 /*  6150 */    70,   71,   72,   73,  195,  195,  195,   63,   64,   65,
 /*  6160 */    66,   67,   68,   69,   70,   71,   72,   73,    1,    2,
 /*  6170 */     3,    4,    5,  195,  195,   89,   90,   91,   92,   93,
 /*  6180 */    94,  195,   96,   97,   98,    1,    2,    3,    4,    5,
 /*  6190 */   195,  195,  195,  195,  195,  195,  195,  195,  195,  195,
 /*  6200 */   195,  195,  195,  195,  195,  195,  195,  195,  195,  195,
 /*  6210 */   195,  195,  195,  195,  195,  195,  195,   50,   51,  195,
 /*  6220 */    53,   54,  195,   56,  195,  195,  195,   60,  195,    1,
 /*  6230 */     2,    3,    4,    5,   50,   51,  195,   53,   54,  195,
 /*  6240 */    56,  195,  195,  195,   60,  195,    1,    2,    3,    4,
 /*  6250 */     5,  195,  195,  195,  195,  195,  195,  195,  195,  195,
 /*  6260 */   195,  195,  195,  195,  195,  195,  195,  195,  195,  195,
 /*  6270 */   195,  195,  195,  195,  195,  195,  195,  195,   50,   51,
 /*  6280 */   195,   53,   54,  195,   56,  195,  195,  195,   60,  195,
 /*  6290 */     1,    2,    3,    4,    5,   50,   51,  195,   53,   54,
 /*  6300 */   195,   56,  195,  195,  195,   60,  195,    1,    2,    3,
 /*  6310 */     4,    5,    1,    2,    3,    4,    5,  195,  195,  195,
 /*  6320 */   195,  195,  195,  195,  195,  195,  195,  195,  195,  195,
 /*  6330 */   195,  195,  195,  195,  195,  195,  195,  195,  195,   50,
 /*  6340 */    51,  195,   53,   54,  195,   56,  195,  195,  195,   60,
 /*  6350 */   195,    1,    2,    3,    4,    5,   50,   51,  195,   53,
 /*  6360 */    54,   50,   51,  195,   53,   54,   60,  195,  195,  195,
 /*  6370 */   195,   60,  195,    1,    2,    3,    4,    5,    1,    2,
 /*  6380 */     3,    4,    5,  195,  195,  195,  195,  195,  195,  195,
 /*  6390 */   195,  195,  195,  195,  195,  195,  195,  195,  195,  195,
 /*  6400 */    50,   51,  195,   53,   54,    1,    2,    3,    4,    5,
 /*  6410 */    60,  195,  195,  195,  195,  195,  195,  195,  195,  195,
 /*  6420 */   195,  195,   50,   51,  195,   53,   54,   50,   51,  195,
 /*  6430 */    53,   54,   60,  195,  195,  195,  195,   60,  195,    1,
 /*  6440 */     2,    3,    4,    5,  195,  195,  195,  195,   44,  195,
 /*  6450 */   195,  195,  195,  195,   50,   51,    1,    2,    3,    4,
 /*  6460 */     5,  195,   58,  195,   60,  195,  195,  195,  195,  195,
 /*  6470 */   195,  195,  195,  195,  195,  195,  195,  195,  195,  195,
 /*  6480 */   195,  195,  195,  195,  195,  195,  195,  195,   50,   51,
 /*  6490 */   195,   53,   54,    1,    2,    3,    4,    5,   60,    1,
 /*  6500 */     2,    3,    4,    5,  195,   50,   51,  195,   53,   54,
 /*  6510 */     1,    2,    3,    4,    5,   60,    1,    2,    3,    4,
 /*  6520 */     5,  195,  195,  195,  195,  195,  195,  195,  195,  195,
 /*  6530 */   195,  195,  195,  195,  195,  195,   44,  195,  195,  195,
 /*  6540 */   195,  195,   50,   51,  195,  195,  195,  195,   50,   51,
 /*  6550 */    58,  195,   60,  195,  195,  195,   58,  195,   60,   50,
 /*  6560 */    51,  195,   53,   54,  195,   50,   51,  195,  195,   60,
 /*  6570 */   195,   56,  195,  195,  195,   60,  195,    1,    2,    3,
 /*  6580 */     4,    5,    1,    2,    3,    4,    5,  195,    1,    2,
 /*  6590 */     3,    4,    5,  195,  195,  195,  195,  195,  195,  195,
 /*  6600 */   195,  195,  195,  195,  195,  195,  195,  195,  195,  195,
 /*  6610 */   195,  195,  195,  195,  195,  195,  195,  195,  195,  195,
 /*  6620 */   195,  195,  195,  195,  195,  195,   50,   51,  195,  195,
 /*  6630 */   195,   50,   51,  195,   58,  195,   60,   50,   51,   58,
 /*  6640 */   195,   60,  195,  195,  195,   58,  195,   60,
};
#define YY_SHIFT_USE_DFLT (-22)
static short yy_shift_ofst[] = {
 /*     0 */   682,  152, 3361,  -22,  -22,  -22,  -22,  -22,  -22,  183,
 /*    10 */   149,  -22,  249,  516,  -22,  381,  409,  -22, 6245,  -22,
 /*    20 */   466,  -22, 6228, 6311,  -22, 6576,  510,  464, 5967,  830,
 /*    30 */  4715,  557,  -22,  -22,  615, 5106,  -22,  584,  -22,  -22,
 /*    40 */   -22,  628,  544,  -22,  621,  668,  -22,  -22,  -22,  -22,
 /*    50 */   -22,  -22,  -22,  -22,  -22,  -22,  -22,  -22,  -22,  -22,
 /*    60 */    15,  589, 4715,  698,  -22,  -22, 5145,  -22,  738,  -21,
 /*    70 */   -22,  -22,  -22,  -22,  -22,  -22,  -22,  -22,  704,  754,
 /*    80 */   -22,  -22,  750,  -22, 6094,  781,  -21,  -22,  455,   54,
 /*    90 */   -21,  -22,  805,  802,  -21,  -22,  824,  818,  -21,  -22,
 /*   100 */   835,  867,  319,  841,  -21,  -22,  860,  858,  -21,  -22,
 /*   110 */   884,  885,  -21,  -22,  -22,  -22,  -22,  -22,  -22,  -22,
 /*   120 */   -22,  -22,  -22,  -22, 3416, 6581,  902,  888, 6041,  205,
 /*   130 */  4715,  903,  -22,  -22,   -5,  186, 4715,  906,  -22,  -22,
 /*   140 */   -22,  -22,  -22, 6515,  895,  905,  -21,  917,  -22,  909,
 /*   150 */   912,  -21,  924,  -22, 6306,  -22,  926,  239,  -22,  931,
 /*   160 */   796,  -22,  934,  308,  -22, 6289,  -22,  928,  -22, 6167,
 /*   170 */  6377,  -22, 6587,  938,  933, 6023,  646,  508,  -22,  817,
 /*   180 */   -22,  -22,  -22,  -22,  -22,  -22,  -22,  -22,  -22,  -22,
 /*   190 */   -22,  -22,  -22,  -22,  -22,  -22,  -22,  -22,  -22,  -22,
 /*   200 */   -22,  -22,  -22,  -22,  -22,  952,  -22,  954,  -22, 5861,
 /*   210 */   949, 2877,  611,  929,  955, 1744, 2980,  -22, 5861,  957,
 /*   220 */    75,  -22,  263,  -22,  -22,  -21,  936, 3289, 3289,  989,
 /*   230 */  2362, 2568,  -22, 5861,  990, 1847, 1229,  -22,  991, 2465,
 /*   240 */  1126,  966, 5861, 1005,  -22, 5861,   48, 5861, 5861,   48,
 /*   250 */  5861, 5861, 5121, 5861, 5861,   48, 5861, 5861,   48, 5861,
 /*   260 */  5861, 5441, 5861, 5861, 5441, 5861,   12, 5861,   12,  369,
 /*   270 */   992, 5509, 5121, 5861, 4733,  -22, 5861,  424, 5861,  424,
 /*   280 */  5861,   48, 5861,   48, 5861,   48, 5861,  424, 5861, 4652,
 /*   290 */  5861, 4652, 5861, 4652, 5861, 4652, 5861, 4652, 5861, 4652,
 /*   300 */  5861, 4652, 5861, 5919, 5861, 4796, 5861, 5121, 3392, 5080,
 /*   310 */   -22, 1014, 5861,   48, 1019, 1043, 5861,   12,  341,   47,
 /*   320 */   996, 5186, 1022, 1063, 5861,   48,  -22, 5861,   48,  -22,
 /*   330 */   -22,  -22,  -22,  -22,  -22,  -22,  -22, 5665, 5121,  238,
 /*   340 */  5548,   21, 1012, 5147,  -22,  871,  -22, 5861, 1026, 1020,
 /*   350 */   638, 5225,  795,  -22,  -22,  -22,  -22,  -22,  -22,  742,
 /*   360 */  1013, 5783,  854,  -22,  -22, 1030, 1025, 1024, 5303,  819,
 /*   370 */   -22,  -22,  -22, 1035, 1028, 1036, 5381,  -22,  804,  -22,
 /*   380 */   736,  -22,  -22, 1004, 1029,  -22,  283,  -22, 5965,  -22,
 /*   390 */  1032, 5861,  -22,  -22,  -22,  720,  -22,  -22,  -22,  701,
 /*   400 */   -22,  -22,  627, 1047, 1045, 5587,  619,  -22,  -22, 1049,
 /*   410 */  1051, 5744,  564,  -22,  -22,   12,   12,   12,   12,   12,
 /*   420 */    12, 5121, 1041,  121, 5626, 4691, 3289, 3083,  -22,  648,
 /*   430 */   660,  -22,  648,  -22, 6086,  -22,  -22,  -22,  -22,  -22,
 /*   440 */   -22,  -22, 5861,  -22, 5121,  702,  132, 5861,  -22, 5264,
 /*   450 */    53, 5861,  -22, 1072,  -22, 4628,  338, 5861,  -22,  -22,
 /*   460 */  5861,  -22,  -22,  -22,  124, 1084,   53, 5861,  -22, 1087,
 /*   470 */    53, 5861,  -22, 1097,  235, 5861,  -22, 5705,   53, 5861,
 /*   480 */   -22,  338, 5861,  -22, 5470,   53, 5861,  -22,  338, 5861,
 /*   490 */   -22,  -22,  -22, 1102, 1100,   53, 5861,  -22,  -22, 5861,
 /*   500 */    27,  -22, 5861,  -22, 5121,  -22, 1121,  -22, 1123,  -22,
 /*   510 */  1127,  -22, 1128,  -22, 5342, 4912,  -22,  -22, 5861, 4996,
 /*   520 */   -22, 5861, 5038,  -22, 5861, 4586,  -22, 1130,  112,  -22,
 /*   530 */  1130,  -22, 1122,  -21,  -22,  -22, 1130,  274,  -22, 1130,
 /*   540 */   351,  -22, 1130,  377,  -22, 1130,  418,  -22, 1130,  439,
 /*   550 */   -22, 1130,  471,  -22, 1130,  476,  -22, 1130,  550,  -22,
 /*   560 */  1130,  574,  -22, 5121,  -22,  -22,  -22, 5861, 4954, 3289,
 /*   570 */  3186,  -22, 1137, 1096, 5822, 4870, 3289,  714,  -22, 5861,
 /*   580 */  4828, 3289,  405,  -22, 1101, 5861, 1141,  -22,  -22,  -22,
 /*   590 */  1107, 3289, 3289,  -22,  -22, 1111, 1146, 1435, 1641,  -22,
 /*   600 */   -22, 4715, 1151, 2053,  -22, 1538,  -22,  171,  322,   96,
 /*   610 */   -22, 1023,  -22, 4715, 1153,  920,  -22,  302,  -22, 3416,
 /*   620 */  6498, 1163, 1156, 6080,  545, 2774,  -22, 2671,  -22, 4715,
 /*   630 */  1160, 2156,  -22, 2259,  -22,  461,  609, 1332,  -22,  199,
 /*   640 */   -22, 4715, 1168,   -7,  -22, 1950,  -22, 6024, 6438,  -22,
 /*   650 */  6515,  -22, 6515, 6404,  337,  -22,  -21,  106,  -22, 1179,
 /*   660 */   -22,   26, 1195,  309, 1197,  217,  -22,  -22, 1200,  -22,
 /*   670 */   -22, 1201,  -22, 6492,  336,  -22,  -21,  408,  -22, 1202,
 /*   680 */   -22, 1211,  -22, 6184, 6509, 6372, 3416, 6455,  -22, 6350,
 /*   690 */   934,  -22,  -22,  -22,  934,  308,  -22, 1224, 1232,  141,
 /*   700 */   -22, 1233, 1227,  -22,  934,  308,  -22, 1236, 1243,  542,
 /*   710 */   -22, 1245, 1227,  -22,  -22,
};
#define YY_REDUCE_USE_DFLT (-155)
static short yy_reduce_ofst[] = {
 /*     0 */  -107, -155,  234, -155, -155, -155, -155, -155, -155, -155,
 /*    10 */  -155, -155, -155,  175, -155, -155,  366, -155,  497, -155,
 /*    20 */  -155, -155,  531,  154, -155,  386, -155, -155,  387, -155,
 /*    30 */   687, -155, -155, -155, -155,  719, -155, -155, -155, -155,
 /*    40 */  -155, -155, -155, -155, -155, -155, -155, -155, -155, -155,
 /*    50 */  -155, -155, -155, -155, -155, -155, -155, -155, -155, -155,
 /*    60 */  -155, -155, 1217, -155, -155, -155,  -97, -155, -155,  625,
 /*    70 */  -155, -155, -155, -155, -155, -155, -155, -155, -155, -155,
 /*    80 */  -155, -155, -155, -155,  512, -155,  705, -155, -155, -155,
 /*    90 */   711, -155, -155, -155,  735, -155, -155, -155,  746, -155,
 /*   100 */  -155, -155, -155, -155,  766, -155, -155, -155,  806, -155,
 /*   110 */  -155, -155,  807, -155, -155, -155, -155, -155, -155, -155,
 /*   120 */  -155, -155, -155, -155,   44,  386, -155, -155,  -23, -155,
 /*   130 */   790, -155, -155, -155, -155, -155,  893, -155, -155, -155,
 /*   140 */  -155, -155, -155,   44, -155, -155,  831, -155, -155, -155,
 /*   150 */  -155,  836, -155, -155,  154, -155, -155,  850, -155, -155,
 /*   160 */   855, -155,  607,  859, -155,  -16, -155, -155, -155,  118,
 /*   170 */   -18, -155,  386, -155, -155,  617, -155, 3830, -155, 4402,
 /*   180 */  -155, -155, -155, -155, -155, -155, -155, -155, -155, -155,
 /*   190 */  -155, -155, -155, -155, -155, -155, -155, -155, -155, -155,
 /*   200 */  -155, -155, -155, -155, -155, -155, -155, -155, -155, 1136,
 /*   210 */  -155, 3570, 4402, -155, -155, 3466, 4402, -155, 1342, -155,
 /*   220 */   150, -155,  842, -155, -155,  899, -155, 3518, 4402, -155,
 /*   230 */  3622, 4402, -155,  147, -155, 4090, 4402, -155, -155, 3414,
 /*   240 */  4402, -155,  250, -155, -155, 1651, -155, 1754, 1857, -155,
 /*   250 */  1960, 2063, -155, 2269, 2304, -155, 2372, 2407, -155, 2475,
 /*   260 */  2510, -155, 2613, 2681, -155, 2716, -155, 2784, -155, -155,
 /*   270 */  -155,  656, -155, 2922, -155, -155, 3025, -155,  457, -155,
 /*   280 */  3093, -155, 3196, -155, 3262, -155, 3418, -155, 3128, -155,
 /*   290 */  2990, -155, 2887, -155, 2819, -155, 2578, -155, 2166, -155,
 /*   300 */  4250, -155, 4198, -155, 4146, -155, 4094, -155, 1377, -155,
 /*   310 */  -155, -155, 4042, -155, -155, -155, 3990, -155, -155, -155,
 /*   320 */  -155,  759, -155, -155, 3938, -155, -155, 3886, -155, -155,
 /*   330 */  -155, -155, -155, -155, -155, -155, -155, 3296, -155, -155,
 /*   340 */   312, -155, -155,  554, -155, -155, -155, 1239, -155, -155,
 /*   350 */  -155,  862, -155, -155, -155, -155, -155, -155, -155, -155,
 /*   360 */  -155,  415, -155, -155, -155, -155, -155, -155,  827, -155,
 /*   370 */  -155, -155, -155, -155, -155, -155,  724, -155, -155, -155,
 /*   380 */  -155, -155, -155, -155, -155, -155, -155, -155, 3357, -155,
 /*   390 */  -155, 3362, -155, -155, -155, -155, -155, -155, -155, -155,
 /*   400 */  -155, -155, -155, -155, -155,  930, -155, -155, -155, -155,
 /*   410 */  -155,  518, -155, -155, -155, -155, -155, -155, -155, -155,
 /*   420 */  -155, -155, -155, -155, 3834, -155, 3726, 4402, -155,  479,
 /*   430 */  -155, -155,  942, -155,  -40, -155, -155, -155, -155, -155,
 /*   440 */  -155, -155, 1583, -155, -155, -155, -154, 1274, -155, 1033,
 /*   450 */   943,  965, -155, -155, -155, -155,  727, 2201, -155, -155,
 /*   460 */   354, -155, -155, -155, -155, -155,  963, 1892, -155, -155,
 /*   470 */   967, 1789, -155, -155,   63, 1480, -155,  354,  968, 1171,
 /*   480 */  -155,   13, 1068, -155,  354,  969, 1686, -155,  134, 1995,
 /*   490 */  -155, -155, -155, -155, -155,  993, 2098, -155, -155,  622,
 /*   500 */  -155, -155, 1445, -155, -155, -155, -155, -155, -155, -155,
 /*   510 */  -155, -155, -155, -155, 3782, -155, -155, -155, 3730, -155,
 /*   520 */  -155, 3678, -155, -155, 3626, -155, -155,   91, -155, -155,
 /*   530 */   987, -155, -155, 1044, -155, -155,  207, -155, -155,  211,
 /*   540 */  -155, -155,  229, -155, -155,  231, -155, -155,  260, -155,
 /*   550 */  -155,  264, -155, -155,  304, -155, -155,  363, -155, -155,
 /*   560 */   449, -155, -155, -155, -155, -155, -155, 3574, -155, 3674,
 /*   570 */  4402, -155, -155, -155, 3522, -155, 3778, 4402, -155, 3470,
 /*   580 */  -155, 3934, 4402, -155, -155, 1548, -155, -155, -155, -155,
 /*   590 */  -155, 3986, 4402, -155, -155, -155, -155, 3353, 4402, -155,
 /*   600 */  -155, 1114, -155, 4142, -155, 4402, -155, -155, -155, 4194,
 /*   610 */  -155, 4402, -155, 1011, -155, 4246, -155, 4402, -155,  496,
 /*   620 */   386, -155, -155,   37, -155, 4298, -155, 4402, -155,   69,
 /*   630 */  -155, 4350, -155, 4402, -155, -155, -155, 4038, -155, 4402,
 /*   640 */  -155, -128, -155, 3882, -155, 4402, -155,  633,  -18, -155,
 /*   650 */   496, -155,  417,  386, 1089, -155, 1091, 1104, -155, -155,
 /*   660 */  -155,   83, -155, -155, -155, 1108, -155, -155, -155, -155,
 /*   670 */  -155, -155, -155,  386, 1115, -155, 1117, 1138, -155, -155,
 /*   680 */  -155, -155, -155,  394,  427,  -18,  417,  -18, -155,  -18,
 /*   690 */  1145, -155, -155, -155,  650, 1154, -155, -155, -155, 1157,
 /*   700 */  -155, -155, 1159, -155,  135, 1164, -155, -155, -155, 1169,
 /*   710 */  -155, -155, 1170, -155, -155,
};
static YYACTIONTYPE yy_default[] = {
 /*     0 */  1062, 1062, 1062,  717,  719,  720,  721,  722,  723, 1062,
 /*    10 */  1062,  724, 1062, 1062,  725, 1062, 1062,  726, 1062,  741,
 /*    20 */  1062,  742, 1062, 1062,  775, 1062, 1062, 1062, 1062, 1062,
 /*    30 */  1062, 1062,  795,  813,  814, 1062,  815,  817,  818,  819,
 /*    40 */   820, 1062, 1062,  839, 1062, 1062,  840,  841,  842,  843,
 /*    50 */   844,  845,  846,  847,  848,  849,  850,  851,  816,  799,
 /*    60 */  1062, 1062, 1062, 1062,  796,  800, 1062,  821,  823, 1062,
 /*    70 */   831, 1049, 1050, 1051, 1052, 1053, 1054, 1055, 1062, 1062,
 /*    80 */  1056, 1057, 1062, 1058, 1062,  824, 1062,  832, 1062,  826,
 /*    90 */  1062,  834, 1062,  828, 1062,  836, 1062,  830, 1062,  838,
 /*   100 */  1062, 1062, 1062,  825, 1062,  833, 1062,  827, 1062,  835,
 /*   110 */  1062,  829, 1062,  837,  822,  803,  805,  806,  807,  808,
 /*   120 */   809,  810,  811,  812, 1062, 1062, 1062, 1062, 1062, 1062,
 /*   130 */  1062, 1062,  797,  801, 1062, 1062, 1062, 1062,  798,  802,
 /*   140 */   804,  771,  776, 1062, 1062, 1062, 1062, 1062,  777, 1062,
 /*   150 */  1062, 1062, 1062,  778, 1062,  772, 1062, 1062,  727, 1062,
 /*   160 */  1062,  728, 1062, 1062,  730, 1062,  736, 1062,  737, 1062,
 /*   170 */  1062,  773, 1062, 1062, 1062, 1062, 1062, 1062,  779, 1062,
 /*   180 */   781,  852,  854,  855,  856,  857,  858,  859,  860,  861,
 /*   190 */   862,  863,  864,  865,  866,  867,  868,  869,  870,  871,
 /*   200 */   872,  873,  874,  875,  876, 1062,  877, 1062,  878, 1062,
 /*   210 */  1062, 1062, 1062,  881, 1062, 1062, 1062,  882, 1062, 1062,
 /*   220 */  1062,  885, 1062,  886,  887, 1062, 1062,  889,  890, 1062,
 /*   230 */  1062, 1062,  893, 1062, 1062, 1062, 1062,  895, 1062, 1062,
 /*   240 */  1062, 1062, 1062, 1062,  897, 1062,  960, 1062, 1062,  961,
 /*   250 */  1062, 1062,  962, 1062, 1062,  963, 1062, 1062,  964, 1062,
 /*   260 */  1062,  965, 1062, 1062,  966, 1062,  973, 1062,  974, 1062,
 /*   270 */   978, 1062, 1040, 1062, 1062,  983, 1062,  984, 1062,  985,
 /*   280 */  1062,  986, 1062,  987, 1062,  988, 1062,  989, 1062,  990,
 /*   290 */  1062,  991, 1062,  992, 1062,  993, 1062,  994, 1062,  995,
 /*   300 */  1062,  996, 1062,  997, 1062, 1062, 1062, 1037, 1062, 1062,
 /*   310 */   975, 1062, 1062,  976, 1062, 1062, 1062,  977, 1001, 1062,
 /*   320 */   981, 1062, 1062, 1062, 1062,  998,  999, 1062, 1000, 1002,
 /*   330 */  1003, 1004, 1005, 1006, 1007, 1008, 1009, 1062, 1048, 1001,
 /*   340 */  1062, 1062, 1012, 1062, 1013, 1062, 1014, 1062, 1062, 1062,
 /*   350 */  1062, 1062, 1062, 1020, 1021, 1034, 1035, 1036, 1039, 1062,
 /*   360 */  1062, 1062, 1062, 1024, 1025, 1062, 1062, 1062, 1062, 1062,
 /*   370 */  1026, 1027, 1038, 1062, 1062, 1015, 1062, 1016, 1062, 1017,
 /*   380 */  1062, 1018, 1019, 1002, 1003, 1010, 1062, 1011, 1062, 1041,
 /*   390 */  1062, 1062, 1043, 1044, 1042, 1062, 1022, 1023,  982, 1062,
 /*   400 */  1028, 1029, 1062, 1062,  979, 1062, 1062, 1030, 1031, 1062,
 /*   410 */   980, 1062, 1062, 1032, 1033,  972,  971,  970,  969,  968,
 /*   420 */   967, 1059, 1062, 1062, 1062, 1062, 1062, 1062,  898, 1062,
 /*   430 */  1062,  902, 1062,  903, 1062,  905,  906,  907,  908,  909,
 /*   440 */   910,  911, 1062,  912,  959, 1062, 1062, 1062,  913, 1062,
 /*   450 */  1062, 1062,  916, 1062,  925, 1062, 1062, 1062,  917,  923,
 /*   460 */  1062,  924,  926,  927, 1062, 1062, 1062, 1062,  914, 1062,
 /*   470 */  1062, 1062,  915, 1062, 1062, 1062,  918, 1062, 1062, 1062,
 /*   480 */   919, 1062, 1062,  920, 1062, 1062, 1062,  921, 1062, 1062,
 /*   490 */   922,  928,  929, 1062, 1062, 1062, 1062,  930,  904, 1062,
 /*   500 */  1062,  932, 1062,  933,  935,  934, 1034,  936, 1036,  937,
 /*   510 */  1035,  938,  999,  939, 1062, 1062,  940,  941, 1062, 1062,
 /*   520 */   942, 1062, 1062,  943, 1062, 1062,  944, 1062, 1062,  945,
 /*   530 */  1062,  955,  957, 1062,  958,  956, 1062, 1062,  946, 1062,
 /*   540 */  1062,  947, 1062, 1062,  948, 1062, 1062,  949, 1062, 1062,
 /*   550 */   950, 1062, 1062,  951, 1062, 1062,  952, 1062, 1062,  953,
 /*   560 */  1062, 1062,  954, 1062, 1060, 1061,  853, 1062, 1062, 1062,
 /*   570 */  1062,  899, 1062, 1062, 1062, 1062, 1062, 1062,  900, 1062,
 /*   580 */  1062, 1062, 1062,  901, 1062, 1062, 1062,  896,  894,  892,
 /*   590 */  1062, 1062,  891,  888,  883,  879, 1062, 1062, 1062,  884,
 /*   600 */   880, 1062, 1062, 1062,  787, 1062,  789, 1062, 1062, 1062,
 /*   610 */   780, 1062,  782, 1062, 1062, 1062,  788, 1062,  790, 1062,
 /*   620 */  1062, 1062, 1062, 1062, 1062, 1062,  783, 1062,  785, 1062,
 /*   630 */  1062, 1062,  791, 1062,  793, 1062, 1062, 1062,  784, 1062,
 /*   640 */   786, 1062, 1062, 1062,  792, 1062,  794, 1062, 1062,  774,
 /*   650 */  1062,  755, 1062, 1062, 1062,  757, 1062, 1062,  759, 1062,
 /*   660 */   763, 1062, 1062, 1062, 1062, 1062,  767,  769, 1062,  770,
 /*   670 */   768, 1062,  761, 1062, 1062,  758, 1062, 1062,  760, 1062,
 /*   680 */   764, 1062,  762, 1062, 1062, 1062, 1062, 1062,  756, 1062,
 /*   690 */  1062,  738,  740,  739, 1062, 1062,  729, 1062, 1062, 1062,
 /*   700 */   731, 1062, 1062,  732, 1062, 1062,  733, 1062, 1062, 1062,
 /*   710 */   734, 1062, 1062,  735,  718,
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
 /* 109 */ "xx_parameter ::= CONST IDENTIFIER",
 /* 110 */ "xx_parameter ::= xx_parameter_type IDENTIFIER",
 /* 111 */ "xx_parameter ::= CONST xx_parameter_type IDENTIFIER",
 /* 112 */ "xx_parameter ::= xx_parameter_type NOT IDENTIFIER",
 /* 113 */ "xx_parameter ::= CONST xx_parameter_type NOT IDENTIFIER",
 /* 114 */ "xx_parameter ::= xx_parameter_cast IDENTIFIER",
 /* 115 */ "xx_parameter ::= CONST xx_parameter_cast IDENTIFIER",
 /* 116 */ "xx_parameter ::= IDENTIFIER ASSIGN xx_literal_expr",
 /* 117 */ "xx_parameter ::= CONST IDENTIFIER ASSIGN xx_literal_expr",
 /* 118 */ "xx_parameter ::= xx_parameter_type IDENTIFIER ASSIGN xx_literal_expr",
 /* 119 */ "xx_parameter ::= CONST xx_parameter_type IDENTIFIER ASSIGN xx_literal_expr",
 /* 120 */ "xx_parameter ::= xx_parameter_type NOT IDENTIFIER ASSIGN xx_literal_expr",
 /* 121 */ "xx_parameter ::= CONST xx_parameter_type NOT IDENTIFIER ASSIGN xx_literal_expr",
 /* 122 */ "xx_parameter ::= xx_parameter_cast IDENTIFIER ASSIGN xx_literal_expr",
 /* 123 */ "xx_parameter ::= CONST xx_parameter_cast IDENTIFIER ASSIGN xx_literal_expr",
 /* 124 */ "xx_parameter_cast ::= LESS IDENTIFIER GREATER",
 /* 125 */ "xx_parameter_cast_collection ::= LESS IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE GREATER",
 /* 126 */ "xx_parameter_type ::= TYPE_INTEGER",
 /* 127 */ "xx_parameter_type ::= TYPE_UINTEGER",
 /* 128 */ "xx_parameter_type ::= TYPE_LONG",
 /* 129 */ "xx_parameter_type ::= TYPE_ULONG",
 /* 130 */ "xx_parameter_type ::= TYPE_CHAR",
 /* 131 */ "xx_parameter_type ::= TYPE_UCHAR",
 /* 132 */ "xx_parameter_type ::= TYPE_DOUBLE",
 /* 133 */ "xx_parameter_type ::= TYPE_BOOL",
 /* 134 */ "xx_parameter_type ::= TYPE_STRING",
 /* 135 */ "xx_parameter_type ::= TYPE_ARRAY",
 /* 136 */ "xx_parameter_type ::= TYPE_VAR",
 /* 137 */ "xx_statement_list ::= xx_statement_list xx_statement",
 /* 138 */ "xx_statement_list ::= xx_statement",
 /* 139 */ "xx_statement ::= xx_cblock",
 /* 140 */ "xx_statement ::= xx_let_statement",
 /* 141 */ "xx_statement ::= xx_if_statement",
 /* 142 */ "xx_statement ::= xx_loop_statement",
 /* 143 */ "xx_statement ::= xx_echo_statement",
 /* 144 */ "xx_statement ::= xx_return_statement",
 /* 145 */ "xx_statement ::= xx_require_statement",
 /* 146 */ "xx_statement ::= xx_fetch_statement",
 /* 147 */ "xx_statement ::= xx_fcall_statement",
 /* 148 */ "xx_statement ::= xx_mcall_statement",
 /* 149 */ "xx_statement ::= xx_scall_statement",
 /* 150 */ "xx_statement ::= xx_unset_statement",
 /* 151 */ "xx_statement ::= xx_throw_statement",
 /* 152 */ "xx_statement ::= xx_declare_statement",
 /* 153 */ "xx_statement ::= xx_break_statement",
 /* 154 */ "xx_statement ::= xx_continue_statement",
 /* 155 */ "xx_statement ::= xx_while_statement",
 /* 156 */ "xx_statement ::= xx_do_while_statement",
 /* 157 */ "xx_statement ::= xx_switch_statement",
 /* 158 */ "xx_statement ::= xx_for_statement",
 /* 159 */ "xx_statement ::= xx_comment",
 /* 160 */ "xx_statement ::= xx_empty_statement",
 /* 161 */ "xx_empty_statement ::= DOTCOMMA",
 /* 162 */ "xx_break_statement ::= BREAK DOTCOMMA",
 /* 163 */ "xx_continue_statement ::= CONTINUE DOTCOMMA",
 /* 164 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /* 165 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE ELSE BRACKET_OPEN BRACKET_CLOSE",
 /* 166 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 167 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE ELSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 168 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE ELSE BRACKET_OPEN BRACKET_CLOSE",
 /* 169 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE ELSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 170 */ "xx_switch_statement ::= SWITCH xx_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /* 171 */ "xx_switch_statement ::= SWITCH xx_eval_expr BRACKET_OPEN xx_case_clauses BRACKET_CLOSE",
 /* 172 */ "xx_case_clauses ::= xx_case_clauses xx_case_clause",
 /* 173 */ "xx_case_clauses ::= xx_case_clause",
 /* 174 */ "xx_case_clause ::= CASE xx_literal_expr COLON",
 /* 175 */ "xx_case_clause ::= CASE xx_literal_expr COLON xx_statement_list",
 /* 176 */ "xx_case_clause ::= DEFAULT COLON xx_statement_list",
 /* 177 */ "xx_loop_statement ::= LOOP BRACKET_OPEN BRACKET_CLOSE",
 /* 178 */ "xx_loop_statement ::= LOOP BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 179 */ "xx_while_statement ::= WHILE xx_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /* 180 */ "xx_while_statement ::= WHILE xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 181 */ "xx_do_while_statement ::= DO BRACKET_OPEN BRACKET_CLOSE WHILE xx_eval_expr DOTCOMMA",
 /* 182 */ "xx_do_while_statement ::= DO BRACKET_OPEN xx_statement_list BRACKET_CLOSE WHILE xx_eval_expr DOTCOMMA",
 /* 183 */ "xx_for_statement ::= FOR IDENTIFIER IN xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 184 */ "xx_for_statement ::= FOR IDENTIFIER IN REVERSE xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 185 */ "xx_for_statement ::= FOR IDENTIFIER COMMA IDENTIFIER IN xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 186 */ "xx_for_statement ::= FOR IDENTIFIER COMMA IDENTIFIER IN REVERSE xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 187 */ "xx_let_statement ::= LET xx_let_assignments DOTCOMMA",
 /* 188 */ "xx_let_assignments ::= xx_let_assignments COMMA xx_let_assignment",
 /* 189 */ "xx_let_assignments ::= xx_let_assignment",
 /* 190 */ "xx_assignment_operator ::= ASSIGN",
 /* 191 */ "xx_assignment_operator ::= ADDASSIGN",
 /* 192 */ "xx_assignment_operator ::= SUBASSIGN",
 /* 193 */ "xx_assignment_operator ::= MULASSIGN",
 /* 194 */ "xx_assignment_operator ::= DIVASSIGN",
 /* 195 */ "xx_assignment_operator ::= CONCATASSIGN",
 /* 196 */ "xx_assignment_operator ::= MODASSIGN",
 /* 197 */ "xx_let_assignment ::= IDENTIFIER xx_assignment_operator xx_assign_expr",
 /* 198 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER xx_assignment_operator xx_assign_expr",
 /* 199 */ "xx_let_assignment ::= IDENTIFIER ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 200 */ "xx_let_assignment ::= IDENTIFIER ARROW BRACKET_OPEN STRING BRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 201 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 202 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER xx_array_offset_list xx_assignment_operator xx_assign_expr",
 /* 203 */ "xx_let_assignment ::= IDENTIFIER DOUBLECOLON IDENTIFIER xx_assignment_operator xx_assign_expr",
 /* 204 */ "xx_let_assignment ::= IDENTIFIER DOUBLECOLON IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 205 */ "xx_let_assignment ::= IDENTIFIER DOUBLECOLON IDENTIFIER xx_array_offset_list xx_assignment_operator xx_assign_expr",
 /* 206 */ "xx_let_assignment ::= IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 207 */ "xx_let_assignment ::= IDENTIFIER xx_array_offset_list xx_assignment_operator xx_assign_expr",
 /* 208 */ "xx_array_offset_list ::= xx_array_offset_list xx_array_offset",
 /* 209 */ "xx_array_offset_list ::= xx_array_offset",
 /* 210 */ "xx_array_offset ::= SBRACKET_OPEN xx_index_expr SBRACKET_CLOSE",
 /* 211 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER INCR",
 /* 212 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER DECR",
 /* 213 */ "xx_let_assignment ::= IDENTIFIER INCR",
 /* 214 */ "xx_let_assignment ::= IDENTIFIER DECR",
 /* 215 */ "xx_let_assignment ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 216 */ "xx_index_expr ::= xx_common_expr",
 /* 217 */ "xx_echo_statement ::= ECHO xx_echo_expressions DOTCOMMA",
 /* 218 */ "xx_echo_expressions ::= xx_echo_expressions COMMA xx_echo_expression",
 /* 219 */ "xx_echo_expressions ::= xx_echo_expression",
 /* 220 */ "xx_echo_expression ::= xx_common_expr",
 /* 221 */ "xx_mcall_statement ::= xx_mcall_expr DOTCOMMA",
 /* 222 */ "xx_fcall_statement ::= xx_fcall_expr DOTCOMMA",
 /* 223 */ "xx_scall_statement ::= xx_scall_expr DOTCOMMA",
 /* 224 */ "xx_fetch_statement ::= xx_fetch_expr DOTCOMMA",
 /* 225 */ "xx_return_statement ::= RETURN xx_common_expr DOTCOMMA",
 /* 226 */ "xx_return_statement ::= RETURN DOTCOMMA",
 /* 227 */ "xx_require_statement ::= REQUIRE xx_common_expr DOTCOMMA",
 /* 228 */ "xx_unset_statement ::= UNSET xx_common_expr DOTCOMMA",
 /* 229 */ "xx_throw_statement ::= THROW xx_common_expr DOTCOMMA",
 /* 230 */ "xx_declare_statement ::= TYPE_INTEGER xx_declare_variable_list DOTCOMMA",
 /* 231 */ "xx_declare_statement ::= TYPE_UINTEGER xx_declare_variable_list DOTCOMMA",
 /* 232 */ "xx_declare_statement ::= TYPE_CHAR xx_declare_variable_list DOTCOMMA",
 /* 233 */ "xx_declare_statement ::= TYPE_UCHAR xx_declare_variable_list DOTCOMMA",
 /* 234 */ "xx_declare_statement ::= TYPE_LONG xx_declare_variable_list DOTCOMMA",
 /* 235 */ "xx_declare_statement ::= TYPE_ULONG xx_declare_variable_list DOTCOMMA",
 /* 236 */ "xx_declare_statement ::= TYPE_DOUBLE xx_declare_variable_list DOTCOMMA",
 /* 237 */ "xx_declare_statement ::= TYPE_STRING xx_declare_variable_list DOTCOMMA",
 /* 238 */ "xx_declare_statement ::= TYPE_BOOL xx_declare_variable_list DOTCOMMA",
 /* 239 */ "xx_declare_statement ::= TYPE_VAR xx_declare_variable_list DOTCOMMA",
 /* 240 */ "xx_declare_variable_list ::= xx_declare_variable_list COMMA xx_declare_variable",
 /* 241 */ "xx_declare_variable_list ::= xx_declare_variable",
 /* 242 */ "xx_declare_variable ::= IDENTIFIER",
 /* 243 */ "xx_declare_variable ::= IDENTIFIER ASSIGN xx_literal_expr",
 /* 244 */ "xx_assign_expr ::= xx_common_expr",
 /* 245 */ "xx_common_expr ::= NOT xx_common_expr",
 /* 246 */ "xx_common_expr ::= ISSET xx_common_expr",
 /* 247 */ "xx_common_expr ::= REQUIRE xx_common_expr",
 /* 248 */ "xx_common_expr ::= CLONE xx_common_expr",
 /* 249 */ "xx_common_expr ::= EMPTY xx_common_expr",
 /* 250 */ "xx_common_expr ::= LIKELY xx_common_expr",
 /* 251 */ "xx_common_expr ::= UNLIKELY xx_common_expr",
 /* 252 */ "xx_common_expr ::= xx_common_expr EQUALS xx_common_expr",
 /* 253 */ "xx_common_expr ::= xx_common_expr NOTEQUALS xx_common_expr",
 /* 254 */ "xx_common_expr ::= xx_common_expr IDENTICAL xx_common_expr",
 /* 255 */ "xx_common_expr ::= xx_common_expr NOTIDENTICAL xx_common_expr",
 /* 256 */ "xx_common_expr ::= xx_common_expr LESS xx_common_expr",
 /* 257 */ "xx_common_expr ::= xx_common_expr GREATER xx_common_expr",
 /* 258 */ "xx_common_expr ::= xx_common_expr LESSEQUAL xx_common_expr",
 /* 259 */ "xx_common_expr ::= xx_common_expr GREATEREQUAL xx_common_expr",
 /* 260 */ "xx_common_expr ::= PARENTHESES_OPEN xx_common_expr PARENTHESES_CLOSE",
 /* 261 */ "xx_common_expr ::= PARENTHESES_OPEN xx_parameter_type PARENTHESES_CLOSE xx_common_expr",
 /* 262 */ "xx_common_expr ::= LESS IDENTIFIER GREATER xx_common_expr",
 /* 263 */ "xx_common_expr ::= xx_common_expr ARROW IDENTIFIER",
 /* 264 */ "xx_common_expr ::= xx_common_expr ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE",
 /* 265 */ "xx_common_expr ::= xx_common_expr ARROW BRACKET_OPEN STRING BRACKET_CLOSE",
 /* 266 */ "xx_common_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER",
 /* 267 */ "xx_common_expr ::= IDENTIFIER DOUBLECOLON CONSTANT",
 /* 268 */ "xx_common_expr ::= xx_common_expr SBRACKET_OPEN xx_common_expr SBRACKET_CLOSE",
 /* 269 */ "xx_common_expr ::= xx_common_expr ADD xx_common_expr",
 /* 270 */ "xx_common_expr ::= xx_common_expr SUB xx_common_expr",
 /* 271 */ "xx_common_expr ::= xx_common_expr MUL xx_common_expr",
 /* 272 */ "xx_common_expr ::= xx_common_expr DIV xx_common_expr",
 /* 273 */ "xx_common_expr ::= xx_common_expr MOD xx_common_expr",
 /* 274 */ "xx_common_expr ::= xx_common_expr CONCAT xx_common_expr",
 /* 275 */ "xx_common_expr ::= xx_common_expr AND xx_common_expr",
 /* 276 */ "xx_common_expr ::= xx_common_expr OR xx_common_expr",
 /* 277 */ "xx_common_expr ::= xx_common_expr BITWISE_AND xx_common_expr",
 /* 278 */ "xx_common_expr ::= xx_common_expr BITWISE_OR xx_common_expr",
 /* 279 */ "xx_common_expr ::= xx_common_expr BITWISE_XOR xx_common_expr",
 /* 280 */ "xx_common_expr ::= xx_common_expr BITWISE_SHIFTLEFT xx_common_expr",
 /* 281 */ "xx_common_expr ::= xx_common_expr BITWISE_SHIFTRIGHT xx_common_expr",
 /* 282 */ "xx_common_expr ::= xx_common_expr INSTANCEOF xx_common_expr",
 /* 283 */ "xx_fetch_expr ::= FETCH IDENTIFIER COMMA xx_common_expr",
 /* 284 */ "xx_common_expr ::= xx_fetch_expr",
 /* 285 */ "xx_common_expr ::= TYPEOF xx_common_expr",
 /* 286 */ "xx_common_expr ::= IDENTIFIER",
 /* 287 */ "xx_common_expr ::= INTEGER",
 /* 288 */ "xx_common_expr ::= STRING",
 /* 289 */ "xx_common_expr ::= CHAR",
 /* 290 */ "xx_common_expr ::= DOUBLE",
 /* 291 */ "xx_common_expr ::= NULL",
 /* 292 */ "xx_common_expr ::= TRUE",
 /* 293 */ "xx_common_expr ::= FALSE",
 /* 294 */ "xx_common_expr ::= CONSTANT",
 /* 295 */ "xx_common_expr ::= SBRACKET_OPEN SBRACKET_CLOSE",
 /* 296 */ "xx_common_expr ::= SBRACKET_OPEN xx_array_list SBRACKET_CLOSE",
 /* 297 */ "xx_common_expr ::= NEW IDENTIFIER",
 /* 298 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 299 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 300 */ "xx_common_expr ::= NEW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE",
 /* 301 */ "xx_common_expr ::= NEW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 302 */ "xx_common_expr ::= NEW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 303 */ "xx_fcall_expr ::= IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 304 */ "xx_fcall_expr ::= IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 305 */ "xx_fcall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 306 */ "xx_fcall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 307 */ "xx_scall_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 308 */ "xx_scall_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 309 */ "xx_scall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE DOUBLECOLON IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 310 */ "xx_scall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE DOUBLECOLON IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 311 */ "xx_scall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE DOUBLECOLON BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 312 */ "xx_scall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE DOUBLECOLON BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 313 */ "xx_mcall_expr ::= xx_common_expr ARROW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 314 */ "xx_mcall_expr ::= xx_common_expr ARROW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 315 */ "xx_mcall_expr ::= xx_common_expr ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 316 */ "xx_mcall_expr ::= xx_common_expr ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 317 */ "xx_mcall_expr ::= xx_common_expr ARROW BRACKET_OPEN STRING BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 318 */ "xx_mcall_expr ::= xx_common_expr ARROW BRACKET_OPEN STRING BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 319 */ "xx_common_expr ::= xx_mcall_expr",
 /* 320 */ "xx_common_expr ::= xx_scall_expr",
 /* 321 */ "xx_common_expr ::= xx_fcall_expr",
 /* 322 */ "xx_common_expr ::= xx_common_expr QUESTION xx_common_expr COLON xx_common_expr",
 /* 323 */ "xx_call_parameters ::= xx_call_parameters COMMA xx_call_parameter",
 /* 324 */ "xx_call_parameters ::= xx_call_parameter",
 /* 325 */ "xx_call_parameter ::= xx_common_expr",
 /* 326 */ "xx_array_list ::= xx_array_list COMMA xx_array_item",
 /* 327 */ "xx_array_list ::= xx_array_item",
 /* 328 */ "xx_array_item ::= xx_array_key COLON xx_array_value",
 /* 329 */ "xx_array_item ::= xx_array_value",
 /* 330 */ "xx_array_key ::= IDENTIFIER",
 /* 331 */ "xx_array_key ::= STRING",
 /* 332 */ "xx_array_key ::= INTEGER",
 /* 333 */ "xx_array_value ::= xx_common_expr",
 /* 334 */ "xx_literal_expr ::= INTEGER",
 /* 335 */ "xx_literal_expr ::= CHAR",
 /* 336 */ "xx_literal_expr ::= STRING",
 /* 337 */ "xx_literal_expr ::= DOUBLE",
 /* 338 */ "xx_literal_expr ::= NULL",
 /* 339 */ "xx_literal_expr ::= FALSE",
 /* 340 */ "xx_literal_expr ::= TRUE",
 /* 341 */ "xx_literal_expr ::= IDENTIFIER DOUBLECOLON CONSTANT",
 /* 342 */ "xx_literal_expr ::= CONSTANT",
 /* 343 */ "xx_literal_expr ::= SBRACKET_OPEN SBRACKET_CLOSE",
 /* 344 */ "xx_eval_expr ::= xx_common_expr",
 /* 345 */ "xx_comment ::= COMMENT",
 /* 346 */ "xx_cblock ::= CBLOCK",
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
// 1037 "parser.lemon"
{
	/*if ((yypminor->yy0)) {
		if ((yypminor->yy0)->free_flag) {
			efree((yypminor->yy0)->token);
		}
		efree((yypminor->yy0));
	}*/
}
// 3296 "parser.c"
      break;
    case 112:
// 1050 "parser.lemon"
{ json_object_put((yypminor->yy25)); }
// 3301 "parser.c"
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
  { 148, 2 },
  { 148, 3 },
  { 148, 3 },
  { 148, 4 },
  { 148, 2 },
  { 148, 3 },
  { 148, 3 },
  { 148, 4 },
  { 148, 4 },
  { 148, 5 },
  { 148, 5 },
  { 148, 6 },
  { 148, 4 },
  { 148, 5 },
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
// 1046 "parser.lemon"
{
	status->ret = yymsp[0].minor.yy25;
}
// 3865 "parser.c"
        break;
      case 1:
      case 4:
      case 5:
      case 6:
      case 7:
      case 8:
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
      case 153:
      case 154:
      case 155:
      case 156:
      case 157:
      case 158:
      case 159:
      case 160:
      case 216:
      case 244:
      case 284:
      case 319:
      case 320:
      case 321:
      case 325:
      case 333:
      case 344:
// 1052 "parser.lemon"
{
	yygotominor.yy25 = yymsp[0].minor.yy25;
}
// 3908 "parser.c"
        break;
      case 2:
      case 40:
      case 56:
      case 58:
      case 60:
      case 88:
      case 137:
      case 172:
      case 208:
// 1056 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_list(yymsp[-1].minor.yy25, yymsp[0].minor.yy25);
}
// 3923 "parser.c"
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
      case 138:
      case 173:
      case 189:
      case 209:
      case 219:
      case 241:
      case 324:
      case 327:
// 1060 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_list(NULL, yymsp[0].minor.yy25);
}
// 3947 "parser.c"
        break;
      case 9:
// 1085 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_namespace(yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 3956 "parser.c"
        break;
      case 10:
// 1089 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_interface(yymsp[-1].minor.yy0, yymsp[0].minor.yy25, NULL, status->scanner_state);
  yy_destructor(46,&yymsp[-2].minor);
}
// 3964 "parser.c"
        break;
      case 11:
// 1093 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_interface(yymsp[-3].minor.yy0, yymsp[0].minor.yy25, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(46,&yymsp[-4].minor);
  yy_destructor(47,&yymsp[-2].minor);
}
// 3973 "parser.c"
        break;
      case 12:
// 1097 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy25, 0, 0, NULL, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-2].minor);
}
// 3981 "parser.c"
        break;
      case 13:
// 1101 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy25, 0, 0, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-4].minor);
  yy_destructor(47,&yymsp[-2].minor);
}
// 3990 "parser.c"
        break;
      case 14:
// 1105 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy25, 0, 0, NULL, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(48,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
}
// 3999 "parser.c"
        break;
      case 15:
// 1109 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class(yymsp[-5].minor.yy0, yymsp[0].minor.yy25, 0, 0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(48,&yymsp[-6].minor);
  yy_destructor(47,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
}
// 4009 "parser.c"
        break;
      case 16:
// 1113 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy25, 1, 0, NULL, NULL, status->scanner_state);
  yy_destructor(50,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-2].minor);
}
// 4018 "parser.c"
        break;
      case 17:
// 1117 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy25, 1, 0, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(50,&yymsp[-5].minor);
  yy_destructor(48,&yymsp[-4].minor);
  yy_destructor(47,&yymsp[-2].minor);
}
// 4028 "parser.c"
        break;
      case 18:
// 1121 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy25, 1, 0, NULL, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(50,&yymsp[-5].minor);
  yy_destructor(48,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
}
// 4038 "parser.c"
        break;
      case 19:
// 1125 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy25, 0, 1, NULL, NULL, status->scanner_state);
  yy_destructor(51,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-2].minor);
}
// 4047 "parser.c"
        break;
      case 20:
// 1129 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy25, 0, 1, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(51,&yymsp[-5].minor);
  yy_destructor(48,&yymsp[-4].minor);
  yy_destructor(47,&yymsp[-2].minor);
}
// 4057 "parser.c"
        break;
      case 21:
      case 50:
// 1133 "parser.lemon"
{
	yygotominor.yy25 = NULL;
  yy_destructor(52,&yymsp[-1].minor);
  yy_destructor(53,&yymsp[0].minor);
}
// 4067 "parser.c"
        break;
      case 22:
      case 51:
// 1137 "parser.lemon"
{
	yygotominor.yy25 = yymsp[-1].minor.yy25;
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(53,&yymsp[0].minor);
}
// 4077 "parser.c"
        break;
      case 23:
      case 52:
      case 106:
      case 188:
      case 218:
      case 240:
      case 323:
      case 326:
// 1141 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_list(yymsp[-2].minor.yy25, yymsp[0].minor.yy25);
  yy_destructor(6,&yymsp[-1].minor);
}
// 4092 "parser.c"
        break;
      case 25:
      case 286:
      case 330:
// 1149 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state);
}
// 4101 "parser.c"
        break;
      case 26:
// 1153 "parser.lemon"
{
  yygotominor.yy25 = NULL;
  yy_destructor(52,&yymsp[-1].minor);
  yy_destructor(53,&yymsp[0].minor);
}
// 4110 "parser.c"
        break;
      case 27:
// 1157 "parser.lemon"
{
  yygotominor.yy25 = yymsp[-1].minor.yy25;
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(53,&yymsp[0].minor);
}
// 4119 "parser.c"
        break;
      case 28:
// 1161 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_definition(yymsp[0].minor.yy25, NULL, NULL, status->scanner_state);
}
// 4126 "parser.c"
        break;
      case 29:
// 1165 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_definition(NULL, NULL, yymsp[0].minor.yy25, status->scanner_state);
}
// 4133 "parser.c"
        break;
      case 30:
// 1169 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_definition(NULL, yymsp[0].minor.yy25, NULL, status->scanner_state);
}
// 4140 "parser.c"
        break;
      case 31:
// 1173 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_definition(yymsp[-1].minor.yy25, yymsp[0].minor.yy25, NULL, status->scanner_state);
}
// 4147 "parser.c"
        break;
      case 32:
// 1177 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_definition(yymsp[-1].minor.yy25, NULL, yymsp[0].minor.yy25, status->scanner_state);
}
// 4154 "parser.c"
        break;
      case 33:
// 1181 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_definition(yymsp[0].minor.yy25, NULL, yymsp[-1].minor.yy25, status->scanner_state);
}
// 4161 "parser.c"
        break;
      case 34:
// 1185 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_definition(NULL, yymsp[0].minor.yy25, yymsp[-1].minor.yy25, status->scanner_state);
}
// 4168 "parser.c"
        break;
      case 35:
// 1189 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_definition(yymsp[-2].minor.yy25, yymsp[0].minor.yy25, yymsp[-1].minor.yy25, status->scanner_state);
}
// 4175 "parser.c"
        break;
      case 36:
// 1193 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_definition(yymsp[-1].minor.yy25, yymsp[0].minor.yy25, yymsp[-2].minor.yy25, status->scanner_state);
}
// 4182 "parser.c"
        break;
      case 37:
// 1197 "parser.lemon"
{
  yygotominor.yy25 = xx_ret_interface_definition(NULL, yymsp[0].minor.yy25, status->scanner_state);
}
// 4189 "parser.c"
        break;
      case 38:
// 1201 "parser.lemon"
{
  yygotominor.yy25 = xx_ret_interface_definition(yymsp[0].minor.yy25, NULL, status->scanner_state);
}
// 4196 "parser.c"
        break;
      case 39:
// 1205 "parser.lemon"
{
  yygotominor.yy25 = xx_ret_interface_definition(yymsp[0].minor.yy25, yymsp[-1].minor.yy25, status->scanner_state);
}
// 4203 "parser.c"
        break;
      case 42:
// 1218 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_property(yymsp[-2].minor.yy25, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(45,&yymsp[0].minor);
}
// 4211 "parser.c"
        break;
      case 43:
// 1222 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_property(yymsp[-2].minor.yy25, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(45,&yymsp[0].minor);
}
// 4219 "parser.c"
        break;
      case 44:
// 1226 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_property(yymsp[-4].minor.yy25, yymsp[-3].minor.yy0, yymsp[-1].minor.yy25, yymsp[-5].minor.yy0, NULL, status->scanner_state);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 4228 "parser.c"
        break;
      case 45:
// 1230 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_property(yymsp[-4].minor.yy25, yymsp[-3].minor.yy0, yymsp[-1].minor.yy25, NULL, NULL, status->scanner_state);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 4237 "parser.c"
        break;
      case 46:
// 1234 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_property(yymsp[-3].minor.yy25, yymsp[-2].minor.yy0, NULL, yymsp[-4].minor.yy0, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(45,&yymsp[0].minor);
}
// 4245 "parser.c"
        break;
      case 47:
// 1238 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_property(yymsp[-3].minor.yy25, yymsp[-2].minor.yy0, NULL, NULL, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(45,&yymsp[0].minor);
}
// 4253 "parser.c"
        break;
      case 48:
// 1242 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_property(yymsp[-5].minor.yy25, yymsp[-4].minor.yy0, yymsp[-2].minor.yy25, yymsp[-6].minor.yy0, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(55,&yymsp[-3].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 4262 "parser.c"
        break;
      case 49:
// 1246 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_property(yymsp[-5].minor.yy25, yymsp[-4].minor.yy0, yymsp[-2].minor.yy25, NULL, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(55,&yymsp[-3].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 4271 "parser.c"
        break;
      case 54:
// 1266 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_property_shortcut(NULL, yymsp[0].minor.yy0, status->scanner_state);
}
// 4278 "parser.c"
        break;
      case 55:
// 1270 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_property_shortcut(yymsp[-1].minor.yy0, yymsp[0].minor.yy0, status->scanner_state);
}
// 4285 "parser.c"
        break;
      case 62:
// 1299 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy25, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(56,&yymsp[-4].minor);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 4295 "parser.c"
        break;
      case 63:
// 1303 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy25, NULL, status->scanner_state);
  yy_destructor(56,&yymsp[-4].minor);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 4305 "parser.c"
        break;
      case 64:
// 1308 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_method(yymsp[-6].minor.yy25, yymsp[-4].minor.yy0, NULL, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(58,&yymsp[-5].minor);
  yy_destructor(59,&yymsp[-3].minor);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(52,&yymsp[-1].minor);
  yy_destructor(53,&yymsp[0].minor);
}
// 4317 "parser.c"
        break;
      case 65:
// 1312 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_method(yymsp[-7].minor.yy25, yymsp[-5].minor.yy0, yymsp[-3].minor.yy25, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(58,&yymsp[-6].minor);
  yy_destructor(59,&yymsp[-4].minor);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(52,&yymsp[-1].minor);
  yy_destructor(53,&yymsp[0].minor);
}
// 4329 "parser.c"
        break;
      case 66:
// 1316 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_method(yymsp[-7].minor.yy25, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy25, NULL, NULL, status->scanner_state);
  yy_destructor(58,&yymsp[-6].minor);
  yy_destructor(59,&yymsp[-4].minor);
  yy_destructor(40,&yymsp[-3].minor);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(53,&yymsp[0].minor);
}
// 4341 "parser.c"
        break;
      case 67:
// 1320 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_method(yymsp[-8].minor.yy25, yymsp[-6].minor.yy0, yymsp[-4].minor.yy25, yymsp[-1].minor.yy25, NULL, NULL, status->scanner_state);
  yy_destructor(58,&yymsp[-7].minor);
  yy_destructor(59,&yymsp[-5].minor);
  yy_destructor(40,&yymsp[-3].minor);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(53,&yymsp[0].minor);
}
// 4353 "parser.c"
        break;
      case 68:
// 1324 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_method(yymsp[-6].minor.yy25, yymsp[-4].minor.yy0, NULL, NULL, yymsp[-7].minor.yy0, NULL, status->scanner_state);
  yy_destructor(58,&yymsp[-5].minor);
  yy_destructor(59,&yymsp[-3].minor);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(52,&yymsp[-1].minor);
  yy_destructor(53,&yymsp[0].minor);
}
// 4365 "parser.c"
        break;
      case 69:
// 1328 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_method(yymsp[-7].minor.yy25, yymsp[-5].minor.yy0, yymsp[-3].minor.yy25, NULL, yymsp[-8].minor.yy0, NULL, status->scanner_state);
  yy_destructor(58,&yymsp[-6].minor);
  yy_destructor(59,&yymsp[-4].minor);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(52,&yymsp[-1].minor);
  yy_destructor(53,&yymsp[0].minor);
}
// 4377 "parser.c"
        break;
      case 70:
// 1332 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_method(yymsp[-7].minor.yy25, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy25, yymsp[-8].minor.yy0, NULL, status->scanner_state);
  yy_destructor(58,&yymsp[-6].minor);
  yy_destructor(59,&yymsp[-4].minor);
  yy_destructor(40,&yymsp[-3].minor);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(53,&yymsp[0].minor);
}
// 4389 "parser.c"
        break;
      case 71:
// 1336 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_method(yymsp[-8].minor.yy25, yymsp[-6].minor.yy0, yymsp[-4].minor.yy25, yymsp[-1].minor.yy25, yymsp[-9].minor.yy0, NULL, status->scanner_state);
  yy_destructor(58,&yymsp[-7].minor);
  yy_destructor(59,&yymsp[-5].minor);
  yy_destructor(40,&yymsp[-3].minor);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(53,&yymsp[0].minor);
}
// 4401 "parser.c"
        break;
      case 72:
// 1340 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_method(yymsp[-8].minor.yy25, yymsp[-6].minor.yy0, NULL, NULL, NULL, yymsp[-2].minor.yy25, status->scanner_state);
  yy_destructor(58,&yymsp[-7].minor);
  yy_destructor(59,&yymsp[-5].minor);
  yy_destructor(40,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-3].minor);
  yy_destructor(52,&yymsp[-1].minor);
  yy_destructor(53,&yymsp[0].minor);
}
// 4414 "parser.c"
        break;
      case 73:
// 1344 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_method(yymsp[-9].minor.yy25, yymsp[-7].minor.yy0, yymsp[-5].minor.yy25, NULL, NULL, yymsp[-2].minor.yy25, status->scanner_state);
  yy_destructor(58,&yymsp[-8].minor);
  yy_destructor(59,&yymsp[-6].minor);
  yy_destructor(40,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-3].minor);
  yy_destructor(52,&yymsp[-1].minor);
  yy_destructor(53,&yymsp[0].minor);
}
// 4427 "parser.c"
        break;
      case 74:
// 1348 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_method(yymsp[-9].minor.yy25, yymsp[-7].minor.yy0, NULL, yymsp[-1].minor.yy25, NULL, yymsp[-3].minor.yy25, status->scanner_state);
  yy_destructor(58,&yymsp[-8].minor);
  yy_destructor(59,&yymsp[-6].minor);
  yy_destructor(40,&yymsp[-5].minor);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(53,&yymsp[0].minor);
}
// 4440 "parser.c"
        break;
      case 75:
// 1352 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_method(yymsp[-10].minor.yy25, yymsp[-8].minor.yy0, yymsp[-6].minor.yy25, yymsp[-1].minor.yy25, NULL, yymsp[-3].minor.yy25, status->scanner_state);
  yy_destructor(58,&yymsp[-9].minor);
  yy_destructor(59,&yymsp[-7].minor);
  yy_destructor(40,&yymsp[-5].minor);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(53,&yymsp[0].minor);
}
// 4453 "parser.c"
        break;
      case 76:
// 1356 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_method(yymsp[-8].minor.yy25, yymsp[-6].minor.yy0, NULL, NULL, yymsp[-9].minor.yy0, yymsp[-2].minor.yy25, status->scanner_state);
  yy_destructor(58,&yymsp[-7].minor);
  yy_destructor(59,&yymsp[-5].minor);
  yy_destructor(40,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-3].minor);
  yy_destructor(52,&yymsp[-1].minor);
  yy_destructor(53,&yymsp[0].minor);
}
// 4466 "parser.c"
        break;
      case 77:
// 1360 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_method(yymsp[-9].minor.yy25, yymsp[-7].minor.yy0, yymsp[-5].minor.yy25, NULL, yymsp[-10].minor.yy0, yymsp[-2].minor.yy25, status->scanner_state);
  yy_destructor(58,&yymsp[-8].minor);
  yy_destructor(59,&yymsp[-6].minor);
  yy_destructor(40,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-3].minor);
  yy_destructor(52,&yymsp[-1].minor);
  yy_destructor(53,&yymsp[0].minor);
}
// 4479 "parser.c"
        break;
      case 78:
// 1364 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_method(yymsp[-9].minor.yy25, yymsp[-7].minor.yy0, NULL, yymsp[-1].minor.yy25, yymsp[-10].minor.yy0, yymsp[-3].minor.yy25, status->scanner_state);
  yy_destructor(58,&yymsp[-8].minor);
  yy_destructor(59,&yymsp[-6].minor);
  yy_destructor(40,&yymsp[-5].minor);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(53,&yymsp[0].minor);
}
// 4492 "parser.c"
        break;
      case 79:
// 1368 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_method(yymsp[-10].minor.yy25, yymsp[-8].minor.yy0, yymsp[-6].minor.yy25, yymsp[-1].minor.yy25, yymsp[-11].minor.yy0, yymsp[-3].minor.yy25, status->scanner_state);
  yy_destructor(58,&yymsp[-9].minor);
  yy_destructor(59,&yymsp[-7].minor);
  yy_destructor(40,&yymsp[-5].minor);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(53,&yymsp[0].minor);
}
// 4505 "parser.c"
        break;
      case 80:
// 1373 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_method(yymsp[-7].minor.yy25, yymsp[-5].minor.yy0, NULL, NULL, NULL, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(58,&yymsp[-6].minor);
  yy_destructor(59,&yymsp[-4].minor);
  yy_destructor(40,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 4517 "parser.c"
        break;
      case 81:
// 1377 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_method(yymsp[-8].minor.yy25, yymsp[-6].minor.yy0, yymsp[-4].minor.yy25, NULL, NULL, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(58,&yymsp[-7].minor);
  yy_destructor(59,&yymsp[-5].minor);
  yy_destructor(40,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 4529 "parser.c"
        break;
      case 82:
// 1381 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_method(yymsp[-7].minor.yy25, yymsp[-5].minor.yy0, NULL, NULL, yymsp[-8].minor.yy0, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(58,&yymsp[-6].minor);
  yy_destructor(59,&yymsp[-4].minor);
  yy_destructor(40,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 4541 "parser.c"
        break;
      case 83:
// 1385 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_method(yymsp[-8].minor.yy25, yymsp[-6].minor.yy0, yymsp[-4].minor.yy25, NULL, yymsp[-9].minor.yy0, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(58,&yymsp[-7].minor);
  yy_destructor(59,&yymsp[-5].minor);
  yy_destructor(40,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 4553 "parser.c"
        break;
      case 84:
// 1389 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_method(yymsp[-5].minor.yy25, yymsp[-3].minor.yy0, NULL, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(58,&yymsp[-4].minor);
  yy_destructor(59,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[-1].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 4564 "parser.c"
        break;
      case 85:
// 1393 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_method(yymsp[-6].minor.yy25, yymsp[-4].minor.yy0, yymsp[-2].minor.yy25, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(58,&yymsp[-5].minor);
  yy_destructor(59,&yymsp[-3].minor);
  yy_destructor(40,&yymsp[-1].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 4575 "parser.c"
        break;
      case 86:
// 1397 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_method(yymsp[-5].minor.yy25, yymsp[-3].minor.yy0, NULL, NULL, yymsp[-6].minor.yy0, NULL, status->scanner_state);
  yy_destructor(58,&yymsp[-4].minor);
  yy_destructor(59,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[-1].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 4586 "parser.c"
        break;
      case 87:
// 1401 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_class_method(yymsp[-6].minor.yy25, yymsp[-4].minor.yy0, yymsp[-2].minor.yy25, NULL, yymsp[-7].minor.yy0, NULL, status->scanner_state);
  yy_destructor(58,&yymsp[-5].minor);
  yy_destructor(59,&yymsp[-3].minor);
  yy_destructor(40,&yymsp[-1].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 4597 "parser.c"
        break;
      case 90:
// 1414 "parser.lemon"
{
	yygotominor.yy25 = json_object_new_string("public");
  yy_destructor(1,&yymsp[0].minor);
}
// 4605 "parser.c"
        break;
      case 91:
// 1418 "parser.lemon"
{
	yygotominor.yy25 = json_object_new_string("protected");
  yy_destructor(2,&yymsp[0].minor);
}
// 4613 "parser.c"
        break;
      case 92:
// 1422 "parser.lemon"
{
	yygotominor.yy25 = json_object_new_string("private");
  yy_destructor(4,&yymsp[0].minor);
}
// 4621 "parser.c"
        break;
      case 93:
// 1426 "parser.lemon"
{
	yygotominor.yy25 = json_object_new_string("static");
  yy_destructor(3,&yymsp[0].minor);
}
// 4629 "parser.c"
        break;
      case 94:
// 1430 "parser.lemon"
{
	yygotominor.yy25 = json_object_new_string("scoped");
  yy_destructor(5,&yymsp[0].minor);
}
// 4637 "parser.c"
        break;
      case 95:
// 1434 "parser.lemon"
{
	yygotominor.yy25 = json_object_new_string("inline");
  yy_destructor(60,&yymsp[0].minor);
}
// 4645 "parser.c"
        break;
      case 96:
// 1438 "parser.lemon"
{
	yygotominor.yy25 = json_object_new_string("abstract");
  yy_destructor(50,&yymsp[0].minor);
}
// 4653 "parser.c"
        break;
      case 97:
// 1442 "parser.lemon"
{
	yygotominor.yy25 = json_object_new_string("final");
  yy_destructor(51,&yymsp[0].minor);
}
// 4661 "parser.c"
        break;
      case 98:
// 1447 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_return_type(1, NULL, status->scanner_state);
  yy_destructor(61,&yymsp[0].minor);
}
// 4669 "parser.c"
        break;
      case 99:
// 1451 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_return_type(0, yymsp[0].minor.yy25, status->scanner_state);
}
// 4676 "parser.c"
        break;
      case 100:
// 1455 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_list(yymsp[-2].minor.yy25, yymsp[0].minor.yy25);
  yy_destructor(14,&yymsp[-1].minor);
}
// 4684 "parser.c"
        break;
      case 102:
// 1463 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_return_type_item(yymsp[0].minor.yy25, NULL, 0, 0, status->scanner_state);
}
// 4691 "parser.c"
        break;
      case 103:
// 1467 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_return_type_item(yymsp[-1].minor.yy25, NULL, 1, 0, status->scanner_state);
  yy_destructor(39,&yymsp[0].minor);
}
// 4699 "parser.c"
        break;
      case 104:
// 1471 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_return_type_item(NULL, yymsp[0].minor.yy25, 0, 0, status->scanner_state);
}
// 4706 "parser.c"
        break;
      case 105:
// 1475 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_return_type_item(NULL, yymsp[0].minor.yy25, 0, 1, status->scanner_state);
}
// 4713 "parser.c"
        break;
      case 108:
// 1489 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_parameter(0, NULL, NULL, yymsp[0].minor.yy0, NULL, 0, status->scanner_state);
}
// 4720 "parser.c"
        break;
      case 109:
// 1493 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_parameter(1, NULL, NULL, yymsp[0].minor.yy0, NULL, 0, status->scanner_state);
  yy_destructor(56,&yymsp[-1].minor);
}
// 4728 "parser.c"
        break;
      case 110:
// 1497 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_parameter(0, yymsp[-1].minor.yy25, NULL, yymsp[0].minor.yy0, NULL, 0, status->scanner_state);
}
// 4735 "parser.c"
        break;
      case 111:
// 1501 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_parameter(1, yymsp[-1].minor.yy25, NULL, yymsp[0].minor.yy0, NULL, 0, status->scanner_state);
  yy_destructor(56,&yymsp[-2].minor);
}
// 4743 "parser.c"
        break;
      case 112:
// 1505 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_parameter(0, yymsp[-2].minor.yy25, NULL, yymsp[0].minor.yy0, NULL, 1, status->scanner_state);
  yy_destructor(39,&yymsp[-1].minor);
}
// 4751 "parser.c"
        break;
      case 113:
// 1509 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_parameter(1, yymsp[-2].minor.yy25, NULL, yymsp[0].minor.yy0, NULL, 1, status->scanner_state);
  yy_destructor(56,&yymsp[-3].minor);
  yy_destructor(39,&yymsp[-1].minor);
}
// 4760 "parser.c"
        break;
      case 114:
// 1513 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_parameter(0, NULL, yymsp[-1].minor.yy25, yymsp[0].minor.yy0, NULL, 0, status->scanner_state);
}
// 4767 "parser.c"
        break;
      case 115:
// 1517 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_parameter(1, NULL, yymsp[-1].minor.yy25, yymsp[0].minor.yy0, NULL, 0, status->scanner_state);
  yy_destructor(56,&yymsp[-2].minor);
}
// 4775 "parser.c"
        break;
      case 116:
// 1521 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_parameter(0, NULL, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy25, 0, status->scanner_state);
  yy_destructor(55,&yymsp[-1].minor);
}
// 4783 "parser.c"
        break;
      case 117:
// 1525 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_parameter(1, NULL, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy25, 0, status->scanner_state);
  yy_destructor(56,&yymsp[-3].minor);
  yy_destructor(55,&yymsp[-1].minor);
}
// 4792 "parser.c"
        break;
      case 118:
// 1529 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_parameter(0, yymsp[-3].minor.yy25, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy25, 0, status->scanner_state);
  yy_destructor(55,&yymsp[-1].minor);
}
// 4800 "parser.c"
        break;
      case 119:
// 1533 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_parameter(1, yymsp[-3].minor.yy25, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy25, 0, status->scanner_state);
  yy_destructor(56,&yymsp[-4].minor);
  yy_destructor(55,&yymsp[-1].minor);
}
// 4809 "parser.c"
        break;
      case 120:
// 1537 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_parameter(0, yymsp[-4].minor.yy25, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy25, 1, status->scanner_state);
  yy_destructor(39,&yymsp[-3].minor);
  yy_destructor(55,&yymsp[-1].minor);
}
// 4818 "parser.c"
        break;
      case 121:
// 1541 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_parameter(1, yymsp[-4].minor.yy25, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy25, 1, status->scanner_state);
  yy_destructor(56,&yymsp[-5].minor);
  yy_destructor(39,&yymsp[-3].minor);
  yy_destructor(55,&yymsp[-1].minor);
}
// 4828 "parser.c"
        break;
      case 122:
// 1545 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_parameter(0, NULL, yymsp[-3].minor.yy25, yymsp[-2].minor.yy0, yymsp[0].minor.yy25, 0, status->scanner_state);
  yy_destructor(55,&yymsp[-1].minor);
}
// 4836 "parser.c"
        break;
      case 123:
// 1549 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_parameter(1, NULL, yymsp[-3].minor.yy25, yymsp[-2].minor.yy0, yymsp[0].minor.yy25, 0, status->scanner_state);
  yy_destructor(56,&yymsp[-4].minor);
  yy_destructor(55,&yymsp[-1].minor);
}
// 4845 "parser.c"
        break;
      case 124:
// 1554 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 4854 "parser.c"
        break;
      case 125:
// 1558 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(21,&yymsp[-4].minor);
  yy_destructor(41,&yymsp[-2].minor);
  yy_destructor(62,&yymsp[-1].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 4865 "parser.c"
        break;
      case 126:
// 1562 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_type(XX_TYPE_INTEGER);
  yy_destructor(63,&yymsp[0].minor);
}
// 4873 "parser.c"
        break;
      case 127:
// 1566 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_type(XX_TYPE_UINTEGER);
  yy_destructor(64,&yymsp[0].minor);
}
// 4881 "parser.c"
        break;
      case 128:
// 1570 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_type(XX_TYPE_LONG);
  yy_destructor(65,&yymsp[0].minor);
}
// 4889 "parser.c"
        break;
      case 129:
// 1574 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_type(XX_TYPE_ULONG);
  yy_destructor(66,&yymsp[0].minor);
}
// 4897 "parser.c"
        break;
      case 130:
// 1578 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_type(XX_TYPE_CHAR);
  yy_destructor(67,&yymsp[0].minor);
}
// 4905 "parser.c"
        break;
      case 131:
// 1582 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_type(XX_TYPE_UCHAR);
  yy_destructor(68,&yymsp[0].minor);
}
// 4913 "parser.c"
        break;
      case 132:
// 1586 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_type(XX_TYPE_DOUBLE);
  yy_destructor(69,&yymsp[0].minor);
}
// 4921 "parser.c"
        break;
      case 133:
// 1590 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_type(XX_TYPE_BOOL);
  yy_destructor(70,&yymsp[0].minor);
}
// 4929 "parser.c"
        break;
      case 134:
// 1594 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_type(XX_TYPE_STRING);
  yy_destructor(71,&yymsp[0].minor);
}
// 4937 "parser.c"
        break;
      case 135:
// 1598 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_type(XX_TYPE_ARRAY);
  yy_destructor(72,&yymsp[0].minor);
}
// 4945 "parser.c"
        break;
      case 136:
// 1602 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_type(XX_TYPE_VAR);
  yy_destructor(73,&yymsp[0].minor);
}
// 4953 "parser.c"
        break;
      case 161:
// 1704 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_empty_statement(status->scanner_state);
  yy_destructor(45,&yymsp[0].minor);
}
// 4961 "parser.c"
        break;
      case 162:
// 1708 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_break_statement(status->scanner_state);
  yy_destructor(74,&yymsp[-1].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 4970 "parser.c"
        break;
      case 163:
// 1712 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_continue_statement(status->scanner_state);
  yy_destructor(75,&yymsp[-1].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 4979 "parser.c"
        break;
      case 164:
// 1716 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_if_statement(yymsp[-2].minor.yy25, NULL, NULL, status->scanner_state);
  yy_destructor(76,&yymsp[-3].minor);
  yy_destructor(52,&yymsp[-1].minor);
  yy_destructor(53,&yymsp[0].minor);
}
// 4989 "parser.c"
        break;
      case 165:
// 1720 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_if_statement(yymsp[-5].minor.yy25, NULL, NULL, status->scanner_state);
  yy_destructor(76,&yymsp[-6].minor);
  yy_destructor(52,&yymsp[-4].minor);
  yy_destructor(53,&yymsp[-3].minor);
  yy_destructor(77,&yymsp[-2].minor);
  yy_destructor(52,&yymsp[-1].minor);
  yy_destructor(53,&yymsp[0].minor);
}
// 5002 "parser.c"
        break;
      case 166:
// 1724 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_if_statement(yymsp[-3].minor.yy25, yymsp[-1].minor.yy25, NULL, status->scanner_state);
  yy_destructor(76,&yymsp[-4].minor);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(53,&yymsp[0].minor);
}
// 5012 "parser.c"
        break;
      case 167:
// 1728 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_if_statement(yymsp[-7].minor.yy25, yymsp[-5].minor.yy25, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(76,&yymsp[-8].minor);
  yy_destructor(52,&yymsp[-6].minor);
  yy_destructor(53,&yymsp[-4].minor);
  yy_destructor(77,&yymsp[-3].minor);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(53,&yymsp[0].minor);
}
// 5025 "parser.c"
        break;
      case 168:
// 1732 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_if_statement(yymsp[-6].minor.yy25, yymsp[-4].minor.yy25, NULL, status->scanner_state);
  yy_destructor(76,&yymsp[-7].minor);
  yy_destructor(52,&yymsp[-5].minor);
  yy_destructor(53,&yymsp[-3].minor);
  yy_destructor(77,&yymsp[-2].minor);
  yy_destructor(52,&yymsp[-1].minor);
  yy_destructor(53,&yymsp[0].minor);
}
// 5038 "parser.c"
        break;
      case 169:
// 1736 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_if_statement(yymsp[-6].minor.yy25, NULL, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(76,&yymsp[-7].minor);
  yy_destructor(52,&yymsp[-5].minor);
  yy_destructor(53,&yymsp[-4].minor);
  yy_destructor(77,&yymsp[-3].minor);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(53,&yymsp[0].minor);
}
// 5051 "parser.c"
        break;
      case 170:
// 1740 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_switch_statement(yymsp[-2].minor.yy25, NULL, status->scanner_state);
  yy_destructor(78,&yymsp[-3].minor);
  yy_destructor(52,&yymsp[-1].minor);
  yy_destructor(53,&yymsp[0].minor);
}
// 5061 "parser.c"
        break;
      case 171:
// 1744 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_switch_statement(yymsp[-3].minor.yy25, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(78,&yymsp[-4].minor);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(53,&yymsp[0].minor);
}
// 5071 "parser.c"
        break;
      case 174:
// 1756 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_case_clause(yymsp[-1].minor.yy25, NULL, status->scanner_state);
  yy_destructor(79,&yymsp[-2].minor);
  yy_destructor(80,&yymsp[0].minor);
}
// 5080 "parser.c"
        break;
      case 175:
// 1760 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_case_clause(yymsp[-2].minor.yy25, yymsp[0].minor.yy25, status->scanner_state);
  yy_destructor(79,&yymsp[-3].minor);
  yy_destructor(80,&yymsp[-1].minor);
}
// 5089 "parser.c"
        break;
      case 176:
// 1764 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_case_clause(NULL, yymsp[0].minor.yy25, status->scanner_state);
  yy_destructor(81,&yymsp[-2].minor);
  yy_destructor(80,&yymsp[-1].minor);
}
// 5098 "parser.c"
        break;
      case 177:
// 1768 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_loop_statement(NULL, status->scanner_state);
  yy_destructor(82,&yymsp[-2].minor);
  yy_destructor(52,&yymsp[-1].minor);
  yy_destructor(53,&yymsp[0].minor);
}
// 5108 "parser.c"
        break;
      case 178:
// 1772 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_loop_statement(yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(82,&yymsp[-3].minor);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(53,&yymsp[0].minor);
}
// 5118 "parser.c"
        break;
      case 179:
// 1776 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_while_statement(yymsp[-2].minor.yy25, NULL, status->scanner_state);
  yy_destructor(83,&yymsp[-3].minor);
  yy_destructor(52,&yymsp[-1].minor);
  yy_destructor(53,&yymsp[0].minor);
}
// 5128 "parser.c"
        break;
      case 180:
// 1780 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_while_statement(yymsp[-3].minor.yy25, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(83,&yymsp[-4].minor);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(53,&yymsp[0].minor);
}
// 5138 "parser.c"
        break;
      case 181:
// 1784 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_do_while_statement(yymsp[-1].minor.yy25, NULL, status->scanner_state);
  yy_destructor(84,&yymsp[-5].minor);
  yy_destructor(52,&yymsp[-4].minor);
  yy_destructor(53,&yymsp[-3].minor);
  yy_destructor(83,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 5150 "parser.c"
        break;
      case 182:
// 1788 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_do_while_statement(yymsp[-1].minor.yy25, yymsp[-4].minor.yy25, status->scanner_state);
  yy_destructor(84,&yymsp[-6].minor);
  yy_destructor(52,&yymsp[-5].minor);
  yy_destructor(53,&yymsp[-3].minor);
  yy_destructor(83,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 5162 "parser.c"
        break;
      case 183:
// 1792 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_for_statement(yymsp[-3].minor.yy25, NULL, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(85,&yymsp[-6].minor);
  yy_destructor(86,&yymsp[-4].minor);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(53,&yymsp[0].minor);
}
// 5173 "parser.c"
        break;
      case 184:
// 1796 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_for_statement(yymsp[-3].minor.yy25, NULL, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(85,&yymsp[-7].minor);
  yy_destructor(86,&yymsp[-5].minor);
  yy_destructor(87,&yymsp[-4].minor);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(53,&yymsp[0].minor);
}
// 5185 "parser.c"
        break;
      case 185:
// 1800 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_for_statement(yymsp[-3].minor.yy25, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(85,&yymsp[-8].minor);
  yy_destructor(6,&yymsp[-6].minor);
  yy_destructor(86,&yymsp[-4].minor);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(53,&yymsp[0].minor);
}
// 5197 "parser.c"
        break;
      case 186:
// 1804 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_for_statement(yymsp[-3].minor.yy25, yymsp[-8].minor.yy0, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(85,&yymsp[-9].minor);
  yy_destructor(6,&yymsp[-7].minor);
  yy_destructor(86,&yymsp[-5].minor);
  yy_destructor(87,&yymsp[-4].minor);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(53,&yymsp[0].minor);
}
// 5210 "parser.c"
        break;
      case 187:
// 1808 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_let_statement(yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(88,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 5219 "parser.c"
        break;
      case 190:
// 1821 "parser.lemon"
{
	yygotominor.yy25 = json_object_new_string("assign");
  yy_destructor(55,&yymsp[0].minor);
}
// 5227 "parser.c"
        break;
      case 191:
// 1826 "parser.lemon"
{
	yygotominor.yy25 = json_object_new_string("add-assign");
  yy_destructor(89,&yymsp[0].minor);
}
// 5235 "parser.c"
        break;
      case 192:
// 1831 "parser.lemon"
{
	yygotominor.yy25 = json_object_new_string("sub-assign");
  yy_destructor(90,&yymsp[0].minor);
}
// 5243 "parser.c"
        break;
      case 193:
// 1835 "parser.lemon"
{
	yygotominor.yy25 = json_object_new_string("mult-assign");
  yy_destructor(91,&yymsp[0].minor);
}
// 5251 "parser.c"
        break;
      case 194:
// 1839 "parser.lemon"
{
	yygotominor.yy25 = json_object_new_string("div-assign");
  yy_destructor(92,&yymsp[0].minor);
}
// 5259 "parser.c"
        break;
      case 195:
// 1843 "parser.lemon"
{
	yygotominor.yy25 = json_object_new_string("concat-assign");
  yy_destructor(93,&yymsp[0].minor);
}
// 5267 "parser.c"
        break;
      case 196:
// 1847 "parser.lemon"
{
	yygotominor.yy25 = json_object_new_string("mod-assign");
  yy_destructor(94,&yymsp[0].minor);
}
// 5275 "parser.c"
        break;
      case 197:
// 1852 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_let_assignment("variable", yymsp[-1].minor.yy25, yymsp[-2].minor.yy0, NULL, NULL, yymsp[0].minor.yy25, status->scanner_state);
}
// 5282 "parser.c"
        break;
      case 198:
// 1857 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_let_assignment("object-property", yymsp[-1].minor.yy25, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy25, status->scanner_state);
  yy_destructor(42,&yymsp[-3].minor);
}
// 5290 "parser.c"
        break;
      case 199:
// 1862 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_let_assignment("variable-dynamic-object-property", yymsp[-1].minor.yy25, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, yymsp[0].minor.yy25, status->scanner_state);
  yy_destructor(42,&yymsp[-5].minor);
  yy_destructor(52,&yymsp[-4].minor);
  yy_destructor(53,&yymsp[-2].minor);
}
// 5300 "parser.c"
        break;
      case 200:
// 1867 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_let_assignment("string-dynamic-object-property", yymsp[-1].minor.yy25, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, yymsp[0].minor.yy25, status->scanner_state);
  yy_destructor(42,&yymsp[-5].minor);
  yy_destructor(52,&yymsp[-4].minor);
  yy_destructor(53,&yymsp[-2].minor);
}
// 5310 "parser.c"
        break;
      case 201:
// 1872 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_let_assignment("object-property-append", yymsp[-1].minor.yy25, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy25, status->scanner_state);
  yy_destructor(42,&yymsp[-5].minor);
  yy_destructor(41,&yymsp[-3].minor);
  yy_destructor(62,&yymsp[-2].minor);
}
// 5320 "parser.c"
        break;
      case 202:
// 1877 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_let_assignment("object-property-array-index", yymsp[-1].minor.yy25, yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-2].minor.yy25, yymsp[0].minor.yy25, status->scanner_state);
  yy_destructor(42,&yymsp[-4].minor);
}
// 5328 "parser.c"
        break;
      case 203:
// 1882 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_let_assignment("static-property", yymsp[-1].minor.yy25, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy25, status->scanner_state);
  yy_destructor(96,&yymsp[-3].minor);
}
// 5336 "parser.c"
        break;
      case 204:
// 1887 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_let_assignment("static-property-append", yymsp[-1].minor.yy25, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy25, status->scanner_state);
  yy_destructor(96,&yymsp[-5].minor);
  yy_destructor(41,&yymsp[-3].minor);
  yy_destructor(62,&yymsp[-2].minor);
}
// 5346 "parser.c"
        break;
      case 205:
// 1892 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_let_assignment("static-property-array-index", yymsp[-1].minor.yy25, yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-2].minor.yy25, yymsp[0].minor.yy25, status->scanner_state);
  yy_destructor(96,&yymsp[-4].minor);
}
// 5354 "parser.c"
        break;
      case 206:
// 1897 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_let_assignment("variable-append", yymsp[-1].minor.yy25, yymsp[-4].minor.yy0, NULL, NULL, yymsp[0].minor.yy25, status->scanner_state);
  yy_destructor(41,&yymsp[-3].minor);
  yy_destructor(62,&yymsp[-2].minor);
}
// 5363 "parser.c"
        break;
      case 207:
// 1902 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_let_assignment("array-index", yymsp[-1].minor.yy25, yymsp[-3].minor.yy0, NULL, yymsp[-2].minor.yy25, yymsp[0].minor.yy25, status->scanner_state);
}
// 5370 "parser.c"
        break;
      case 210:
// 1914 "parser.lemon"
{
	yygotominor.yy25 = yymsp[-1].minor.yy25;
  yy_destructor(41,&yymsp[-2].minor);
  yy_destructor(62,&yymsp[0].minor);
}
// 5379 "parser.c"
        break;
      case 211:
// 1919 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_let_assignment("object-property-incr", NULL, yymsp[-3].minor.yy0, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(97,&yymsp[0].minor);
}
// 5388 "parser.c"
        break;
      case 212:
// 1924 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_let_assignment("object-property-decr", NULL, yymsp[-3].minor.yy0, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(98,&yymsp[0].minor);
}
// 5397 "parser.c"
        break;
      case 213:
// 1929 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_let_assignment("incr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(97,&yymsp[0].minor);
}
// 5405 "parser.c"
        break;
      case 214:
// 1934 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_let_assignment("decr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(98,&yymsp[0].minor);
}
// 5413 "parser.c"
        break;
      case 215:
// 1939 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_let_assignment("dynamic-variable", yymsp[-1].minor.yy25, yymsp[-3].minor.yy0, NULL, NULL, yymsp[0].minor.yy25, status->scanner_state);
  yy_destructor(52,&yymsp[-4].minor);
  yy_destructor(53,&yymsp[-2].minor);
}
// 5422 "parser.c"
        break;
      case 217:
// 1947 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_echo_statement(yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(99,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 5431 "parser.c"
        break;
      case 220:
// 1959 "parser.lemon"
{
	yygotominor.yy25 = yymsp[0].minor.yy25;;
}
// 5438 "parser.c"
        break;
      case 221:
// 1964 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_mcall_statement(yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(45,&yymsp[0].minor);
}
// 5446 "parser.c"
        break;
      case 222:
// 1969 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_fcall_statement(yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(45,&yymsp[0].minor);
}
// 5454 "parser.c"
        break;
      case 223:
// 1974 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_scall_statement(yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(45,&yymsp[0].minor);
}
// 5462 "parser.c"
        break;
      case 224:
// 1979 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_fetch_statement(yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(45,&yymsp[0].minor);
}
// 5470 "parser.c"
        break;
      case 225:
// 1984 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_return_statement(yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(100,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 5479 "parser.c"
        break;
      case 226:
// 1989 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_return_statement(NULL, status->scanner_state);
  yy_destructor(100,&yymsp[-1].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 5488 "parser.c"
        break;
      case 227:
// 1994 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_require_statement(yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(7,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 5497 "parser.c"
        break;
      case 228:
// 1999 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_unset_statement(yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(101,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 5506 "parser.c"
        break;
      case 229:
// 2004 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_throw_exception(yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(102,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 5515 "parser.c"
        break;
      case 230:
// 2008 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_declare_statement(XX_T_TYPE_INTEGER, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(63,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 5524 "parser.c"
        break;
      case 231:
// 2012 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_declare_statement(XX_T_TYPE_UINTEGER, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(64,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 5533 "parser.c"
        break;
      case 232:
// 2016 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_declare_statement(XX_T_TYPE_CHAR, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(67,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 5542 "parser.c"
        break;
      case 233:
// 2020 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_declare_statement(XX_T_TYPE_UCHAR, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(68,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 5551 "parser.c"
        break;
      case 234:
// 2024 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_declare_statement(XX_T_TYPE_LONG, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(65,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 5560 "parser.c"
        break;
      case 235:
// 2028 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_declare_statement(XX_T_TYPE_ULONG, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(66,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 5569 "parser.c"
        break;
      case 236:
// 2032 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_declare_statement(XX_T_TYPE_DOUBLE, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(69,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 5578 "parser.c"
        break;
      case 237:
// 2036 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_declare_statement(XX_T_TYPE_STRING, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(71,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 5587 "parser.c"
        break;
      case 238:
// 2040 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_declare_statement(XX_T_TYPE_BOOL, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(70,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 5596 "parser.c"
        break;
      case 239:
// 2044 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_declare_statement(XX_T_TYPE_VAR, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(73,&yymsp[-2].minor);
  yy_destructor(45,&yymsp[0].minor);
}
// 5605 "parser.c"
        break;
      case 242:
// 2056 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_declare_variable(yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 5612 "parser.c"
        break;
      case 243:
// 2060 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_declare_variable(yymsp[-2].minor.yy0, yymsp[0].minor.yy25, status->scanner_state);
  yy_destructor(55,&yymsp[-1].minor);
}
// 5620 "parser.c"
        break;
      case 245:
// 2068 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("not", yymsp[0].minor.yy25, NULL, NULL, status->scanner_state);
  yy_destructor(39,&yymsp[-1].minor);
}
// 5628 "parser.c"
        break;
      case 246:
// 2072 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("isset", yymsp[0].minor.yy25, NULL, NULL, status->scanner_state);
  yy_destructor(35,&yymsp[-1].minor);
}
// 5636 "parser.c"
        break;
      case 247:
// 2076 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("require", yymsp[0].minor.yy25, NULL, NULL, status->scanner_state);
  yy_destructor(7,&yymsp[-1].minor);
}
// 5644 "parser.c"
        break;
      case 248:
// 2080 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("clone", yymsp[0].minor.yy25, NULL, NULL, status->scanner_state);
  yy_destructor(33,&yymsp[-1].minor);
}
// 5652 "parser.c"
        break;
      case 249:
// 2084 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("empty", yymsp[0].minor.yy25, NULL, NULL, status->scanner_state);
  yy_destructor(34,&yymsp[-1].minor);
}
// 5660 "parser.c"
        break;
      case 250:
// 2088 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("likely", yymsp[0].minor.yy25, NULL, NULL, status->scanner_state);
  yy_destructor(9,&yymsp[-1].minor);
}
// 5668 "parser.c"
        break;
      case 251:
// 2092 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("unlikely", yymsp[0].minor.yy25, NULL, NULL, status->scanner_state);
  yy_destructor(10,&yymsp[-1].minor);
}
// 5676 "parser.c"
        break;
      case 252:
// 2096 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("equals", yymsp[-2].minor.yy25, yymsp[0].minor.yy25, NULL, status->scanner_state);
  yy_destructor(19,&yymsp[-1].minor);
}
// 5684 "parser.c"
        break;
      case 253:
// 2100 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("not-equals", yymsp[-2].minor.yy25, yymsp[0].minor.yy25, NULL, status->scanner_state);
  yy_destructor(26,&yymsp[-1].minor);
}
// 5692 "parser.c"
        break;
      case 254:
// 2104 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("identical", yymsp[-2].minor.yy25, yymsp[0].minor.yy25, NULL, status->scanner_state);
  yy_destructor(20,&yymsp[-1].minor);
}
// 5700 "parser.c"
        break;
      case 255:
// 2108 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("not-identical", yymsp[-2].minor.yy25, yymsp[0].minor.yy25, NULL, status->scanner_state);
  yy_destructor(25,&yymsp[-1].minor);
}
// 5708 "parser.c"
        break;
      case 256:
// 2112 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("less", yymsp[-2].minor.yy25, yymsp[0].minor.yy25, NULL, status->scanner_state);
  yy_destructor(21,&yymsp[-1].minor);
}
// 5716 "parser.c"
        break;
      case 257:
// 2116 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("greater", yymsp[-2].minor.yy25, yymsp[0].minor.yy25, NULL, status->scanner_state);
  yy_destructor(22,&yymsp[-1].minor);
}
// 5724 "parser.c"
        break;
      case 258:
// 2120 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("less-equal", yymsp[-2].minor.yy25, yymsp[0].minor.yy25, NULL, status->scanner_state);
  yy_destructor(23,&yymsp[-1].minor);
}
// 5732 "parser.c"
        break;
      case 259:
// 2124 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("greater-equal", yymsp[-2].minor.yy25, yymsp[0].minor.yy25, NULL, status->scanner_state);
  yy_destructor(24,&yymsp[-1].minor);
}
// 5740 "parser.c"
        break;
      case 260:
// 2128 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("list", yymsp[-1].minor.yy25, NULL, NULL, status->scanner_state);
  yy_destructor(59,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 5749 "parser.c"
        break;
      case 261:
// 2132 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("cast", yymsp[-2].minor.yy25, yymsp[0].minor.yy25, NULL, status->scanner_state);
  yy_destructor(59,&yymsp[-3].minor);
  yy_destructor(40,&yymsp[-1].minor);
}
// 5758 "parser.c"
        break;
      case 262:
// 2136 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("type-hint", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy25, NULL, status->scanner_state);
  yy_destructor(21,&yymsp[-3].minor);
  yy_destructor(22,&yymsp[-1].minor);
}
// 5767 "parser.c"
        break;
      case 263:
// 2140 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("property-access", yymsp[-2].minor.yy25, xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(42,&yymsp[-1].minor);
}
// 5775 "parser.c"
        break;
      case 264:
// 2144 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("property-dynamic-access", yymsp[-4].minor.yy25, xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(42,&yymsp[-3].minor);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(53,&yymsp[0].minor);
}
// 5785 "parser.c"
        break;
      case 265:
// 2148 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("property-string-access", yymsp[-4].minor.yy25, xx_ret_literal(XX_T_STRING, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(42,&yymsp[-3].minor);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(53,&yymsp[0].minor);
}
// 5795 "parser.c"
        break;
      case 266:
// 2152 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("static-property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(96,&yymsp[-1].minor);
}
// 5803 "parser.c"
        break;
      case 267:
      case 341:
// 2156 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("static-constant-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(96,&yymsp[-1].minor);
}
// 5812 "parser.c"
        break;
      case 268:
// 2165 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("array-access", yymsp[-3].minor.yy25, yymsp[-1].minor.yy25, NULL, status->scanner_state);
  yy_destructor(41,&yymsp[-2].minor);
  yy_destructor(62,&yymsp[0].minor);
}
// 5821 "parser.c"
        break;
      case 269:
// 2170 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("add", yymsp[-2].minor.yy25, yymsp[0].minor.yy25, NULL, status->scanner_state);
  yy_destructor(27,&yymsp[-1].minor);
}
// 5829 "parser.c"
        break;
      case 270:
// 2175 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("sub", yymsp[-2].minor.yy25, yymsp[0].minor.yy25, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-1].minor);
}
// 5837 "parser.c"
        break;
      case 271:
// 2180 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("mul", yymsp[-2].minor.yy25, yymsp[0].minor.yy25, NULL, status->scanner_state);
  yy_destructor(30,&yymsp[-1].minor);
}
// 5845 "parser.c"
        break;
      case 272:
// 2185 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("div", yymsp[-2].minor.yy25, yymsp[0].minor.yy25, NULL, status->scanner_state);
  yy_destructor(31,&yymsp[-1].minor);
}
// 5853 "parser.c"
        break;
      case 273:
// 2190 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("mod", yymsp[-2].minor.yy25, yymsp[0].minor.yy25, NULL, status->scanner_state);
  yy_destructor(32,&yymsp[-1].minor);
}
// 5861 "parser.c"
        break;
      case 274:
// 2195 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("concat", yymsp[-2].minor.yy25, yymsp[0].minor.yy25, NULL, status->scanner_state);
  yy_destructor(29,&yymsp[-1].minor);
}
// 5869 "parser.c"
        break;
      case 275:
// 2200 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("and", yymsp[-2].minor.yy25, yymsp[0].minor.yy25, NULL, status->scanner_state);
  yy_destructor(12,&yymsp[-1].minor);
}
// 5877 "parser.c"
        break;
      case 276:
// 2205 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("or", yymsp[-2].minor.yy25, yymsp[0].minor.yy25, NULL, status->scanner_state);
  yy_destructor(13,&yymsp[-1].minor);
}
// 5885 "parser.c"
        break;
      case 277:
// 2210 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("bitwise_and", yymsp[-2].minor.yy25, yymsp[0].minor.yy25, NULL, status->scanner_state);
  yy_destructor(15,&yymsp[-1].minor);
}
// 5893 "parser.c"
        break;
      case 278:
// 2215 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("bitwise_or", yymsp[-2].minor.yy25, yymsp[0].minor.yy25, NULL, status->scanner_state);
  yy_destructor(14,&yymsp[-1].minor);
}
// 5901 "parser.c"
        break;
      case 279:
// 2220 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("bitwise_xor", yymsp[-2].minor.yy25, yymsp[0].minor.yy25, NULL, status->scanner_state);
  yy_destructor(16,&yymsp[-1].minor);
}
// 5909 "parser.c"
        break;
      case 280:
// 2225 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("bitwise_shiftleft", yymsp[-2].minor.yy25, yymsp[0].minor.yy25, NULL, status->scanner_state);
  yy_destructor(17,&yymsp[-1].minor);
}
// 5917 "parser.c"
        break;
      case 281:
// 2230 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("bitwise_shiftright", yymsp[-2].minor.yy25, yymsp[0].minor.yy25, NULL, status->scanner_state);
  yy_destructor(18,&yymsp[-1].minor);
}
// 5925 "parser.c"
        break;
      case 282:
// 2235 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("instanceof", yymsp[-2].minor.yy25, yymsp[0].minor.yy25, NULL, status->scanner_state);
  yy_destructor(11,&yymsp[-1].minor);
}
// 5933 "parser.c"
        break;
      case 283:
// 2240 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("fetch", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy25, NULL, status->scanner_state);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(6,&yymsp[-1].minor);
}
// 5942 "parser.c"
        break;
      case 285:
// 2250 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("typeof", yymsp[0].minor.yy25, NULL, NULL, status->scanner_state);
  yy_destructor(37,&yymsp[-1].minor);
}
// 5950 "parser.c"
        break;
      case 287:
      case 332:
      case 334:
// 2260 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_literal(XX_T_INTEGER, yymsp[0].minor.yy0, status->scanner_state);
}
// 5959 "parser.c"
        break;
      case 288:
      case 331:
      case 336:
// 2265 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_literal(XX_T_STRING, yymsp[0].minor.yy0, status->scanner_state);
}
// 5968 "parser.c"
        break;
      case 289:
      case 335:
// 2270 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_literal(XX_T_CHAR, yymsp[0].minor.yy0, status->scanner_state);
}
// 5976 "parser.c"
        break;
      case 290:
      case 337:
// 2275 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_literal(XX_T_DOUBLE, yymsp[0].minor.yy0, status->scanner_state);
}
// 5984 "parser.c"
        break;
      case 291:
      case 338:
// 2280 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_literal(XX_T_NULL, NULL, status->scanner_state);
  yy_destructor(106,&yymsp[0].minor);
}
// 5993 "parser.c"
        break;
      case 292:
      case 340:
// 2285 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_literal(XX_T_TRUE, NULL, status->scanner_state);
  yy_destructor(107,&yymsp[0].minor);
}
// 6002 "parser.c"
        break;
      case 293:
      case 339:
// 2290 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_literal(XX_T_FALSE, NULL, status->scanner_state);
  yy_destructor(108,&yymsp[0].minor);
}
// 6011 "parser.c"
        break;
      case 294:
      case 342:
// 2295 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_literal(XX_T_CONSTANT, yymsp[0].minor.yy0, status->scanner_state);
}
// 6019 "parser.c"
        break;
      case 295:
// 2300 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("empty-array", NULL, NULL, NULL, status->scanner_state);
  yy_destructor(41,&yymsp[-1].minor);
  yy_destructor(62,&yymsp[0].minor);
}
// 6028 "parser.c"
        break;
      case 296:
// 2305 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("array", yymsp[-1].minor.yy25, NULL, NULL, status->scanner_state);
  yy_destructor(41,&yymsp[-2].minor);
  yy_destructor(62,&yymsp[0].minor);
}
// 6037 "parser.c"
        break;
      case 297:
// 2310 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_new_instance(0, yymsp[0].minor.yy0, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-1].minor);
}
// 6045 "parser.c"
        break;
      case 298:
// 2315 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_new_instance(0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(59,&yymsp[-1].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 6055 "parser.c"
        break;
      case 299:
// 2320 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_new_instance(0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(59,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 6065 "parser.c"
        break;
      case 300:
// 2325 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_new_instance(1, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(53,&yymsp[0].minor);
}
// 6075 "parser.c"
        break;
      case 301:
// 2330 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_new_instance(1, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-5].minor);
  yy_destructor(52,&yymsp[-4].minor);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(59,&yymsp[-1].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 6087 "parser.c"
        break;
      case 302:
// 2335 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_new_instance(1, yymsp[-4].minor.yy0, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(38,&yymsp[-6].minor);
  yy_destructor(52,&yymsp[-5].minor);
  yy_destructor(53,&yymsp[-3].minor);
  yy_destructor(59,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 6099 "parser.c"
        break;
      case 303:
// 2340 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_fcall(1, yymsp[-3].minor.yy0, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(59,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 6108 "parser.c"
        break;
      case 304:
// 2345 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_fcall(1, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(59,&yymsp[-1].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 6117 "parser.c"
        break;
      case 305:
// 2350 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_fcall(2, yymsp[-4].minor.yy0, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(52,&yymsp[-5].minor);
  yy_destructor(53,&yymsp[-3].minor);
  yy_destructor(59,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 6128 "parser.c"
        break;
      case 306:
// 2355 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_fcall(2, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(52,&yymsp[-4].minor);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(59,&yymsp[-1].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 6139 "parser.c"
        break;
      case 307:
// 2360 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_scall(0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(96,&yymsp[-4].minor);
  yy_destructor(59,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 6149 "parser.c"
        break;
      case 308:
// 2365 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_scall(0, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(96,&yymsp[-3].minor);
  yy_destructor(59,&yymsp[-1].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 6159 "parser.c"
        break;
      case 309:
// 2370 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_scall(1, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(52,&yymsp[-7].minor);
  yy_destructor(53,&yymsp[-5].minor);
  yy_destructor(96,&yymsp[-4].minor);
  yy_destructor(59,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 6171 "parser.c"
        break;
      case 310:
// 2375 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_scall(1, yymsp[-5].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(52,&yymsp[-6].minor);
  yy_destructor(53,&yymsp[-4].minor);
  yy_destructor(96,&yymsp[-3].minor);
  yy_destructor(59,&yymsp[-1].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 6183 "parser.c"
        break;
      case 311:
// 2380 "parser.lemon"
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
// 6197 "parser.c"
        break;
      case 312:
// 2385 "parser.lemon"
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
// 6211 "parser.c"
        break;
      case 313:
// 2390 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_mcall(1, yymsp[-5].minor.yy25, yymsp[-3].minor.yy0, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(59,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 6221 "parser.c"
        break;
      case 314:
// 2395 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_mcall(1, yymsp[-4].minor.yy25, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(42,&yymsp[-3].minor);
  yy_destructor(59,&yymsp[-1].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 6231 "parser.c"
        break;
      case 315:
// 2400 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_mcall(2, yymsp[-7].minor.yy25, yymsp[-4].minor.yy0, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(42,&yymsp[-6].minor);
  yy_destructor(52,&yymsp[-5].minor);
  yy_destructor(53,&yymsp[-3].minor);
  yy_destructor(59,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 6243 "parser.c"
        break;
      case 316:
// 2405 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_mcall(2, yymsp[-6].minor.yy25, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(42,&yymsp[-5].minor);
  yy_destructor(52,&yymsp[-4].minor);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(59,&yymsp[-1].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 6255 "parser.c"
        break;
      case 317:
// 2410 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_mcall(3, yymsp[-7].minor.yy25, yymsp[-4].minor.yy0, yymsp[-1].minor.yy25, status->scanner_state);
  yy_destructor(42,&yymsp[-6].minor);
  yy_destructor(52,&yymsp[-5].minor);
  yy_destructor(53,&yymsp[-3].minor);
  yy_destructor(59,&yymsp[-2].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 6267 "parser.c"
        break;
      case 318:
// 2415 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_mcall(3, yymsp[-6].minor.yy25, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(42,&yymsp[-5].minor);
  yy_destructor(52,&yymsp[-4].minor);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(59,&yymsp[-1].minor);
  yy_destructor(40,&yymsp[0].minor);
}
// 6279 "parser.c"
        break;
      case 322:
// 2435 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_expr("ternary", yymsp[-4].minor.yy25, yymsp[-2].minor.yy25, yymsp[0].minor.yy25, status->scanner_state);
  yy_destructor(8,&yymsp[-3].minor);
  yy_destructor(80,&yymsp[-1].minor);
}
// 6288 "parser.c"
        break;
      case 328:
// 2459 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_array_item(yymsp[-2].minor.yy25, yymsp[0].minor.yy25, status->scanner_state);
  yy_destructor(80,&yymsp[-1].minor);
}
// 6296 "parser.c"
        break;
      case 329:
// 2463 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_array_item(NULL, yymsp[0].minor.yy25, status->scanner_state);
}
// 6303 "parser.c"
        break;
      case 343:
// 2519 "parser.lemon"
{
  yygotominor.yy25 = xx_ret_expr("empty-array", NULL, NULL, NULL, status->scanner_state);
  yy_destructor(41,&yymsp[-1].minor);
  yy_destructor(62,&yymsp[0].minor);
}
// 6312 "parser.c"
        break;
      case 345:
// 2527 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_comment(yymsp[0].minor.yy0, status->scanner_state);
}
// 6319 "parser.c"
        break;
      case 346:
// 2531 "parser.lemon"
{
	yygotominor.yy25 = xx_ret_cblock(yymsp[0].minor.yy0, status->scanner_state);
}
// 6326 "parser.c"
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
// 1003 "parser.lemon"


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

// 6402 "parser.c"
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
			case XX_T_TYPE_ARRAY:
				xx_(xx_parser, XX_TYPE_ARRAY, NULL, parser_status);
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
