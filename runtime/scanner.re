
/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2015 Zephir Team and contributors                     |
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <php.h>
#include "php_zephir.h"
#include "scanner.h"
#include "zephir.h"

#define YYCTYPE unsigned char
#define YYCURSOR (s->start)
#define YYLIMIT (s->end)
#define YYMARKER q

int zephir_get_token(zephir_scanner_state *s, zephir_scanner_token *token) {

	char next, *q = YYCURSOR, *start = YYCURSOR;
	int status = ZEPHIR_SCANNER_RETCODE_IMPOSSIBLE;
	int is_constant = 0, j;

	while (ZEPHIR_SCANNER_RETCODE_IMPOSSIBLE == status) {

		/*!re2c
		re2c:indent:top = 2;
		re2c:yyfill:enable = 0;

		INTEGER = ([\-]?[0-9]+)|([\-]?[0][x][0-9A-Fa-f]+);
		INTEGER {
			token->opcode = ZEPHIR_T_INTEGER;
			token->value = estrndup(start, YYCURSOR - start);
			token->len = YYCURSOR - start;
			s->active_char += (YYCURSOR - start);
			q = YYCURSOR;
			return 0;
		}

		DOUBLE = ([\-]?[0-9]+[\.][0-9]+);
		DOUBLE {
			token->opcode = ZEPHIR_T_DOUBLE;
			token->value = estrndup(start, YYCURSOR - start);
			token->len = YYCURSOR - start;
			s->active_char += (YYCURSOR - start);
			q = YYCURSOR;
			return 0;
		}

		'null' {
			s->active_char += sizeof("null")-1;
			token->opcode = ZEPHIR_T_NULL;
			return 0;
		}

		'false' {
			s->active_char += sizeof("false")-1;
			token->opcode = ZEPHIR_T_FALSE;
			return 0;
		}

		'true' {
			s->active_char += sizeof("true")-1;
			token->opcode = ZEPHIR_T_TRUE;
			return 0;
		}

		'namespace' {
			s->active_char += sizeof("namespace")-1;
			token->opcode = ZEPHIR_T_NAMESPACE;
			return 0;
		}

		'use' {
			s->active_char += sizeof("use")-1;
			token->opcode = ZEPHIR_T_USE;
			return 0;
		}

		'as' {
			s->active_char += sizeof("as")-1;
			token->opcode = ZEPHIR_T_AS;
			return 0;
		}

		'abstract' {
			s->active_char += sizeof("abstract")-1;
			token->opcode = ZEPHIR_T_ABSTRACT;
			return 0;
		}

		'interface' {
			s->active_char += sizeof("interface")-1;
			token->opcode = ZEPHIR_T_INTERFACE;
			return 0;
		}

		'class' {
			s->active_char += sizeof("class")-1;
			token->opcode = ZEPHIR_T_CLASS;
			return 0;
		}

		'extends' {
			s->active_char += sizeof("extends")-1;
			token->opcode = ZEPHIR_T_EXTENDS;
			return 0;
		}

		'implements' {
			s->active_char += sizeof("implements")-1;
			token->opcode = ZEPHIR_T_IMPLEMENTS;
			return 0;
		}

		'public' {
			s->active_char += sizeof("public")-1;
			token->opcode = ZEPHIR_T_PUBLIC;
			return 0;
		}

		'protected' {
			s->active_char += sizeof("protected")-1;
			token->opcode = ZEPHIR_T_PROTECTED;
			return 0;
		}

		'private' {
			s->active_char += sizeof("private")-1;
			token->opcode = ZEPHIR_T_PRIVATE;
			return 0;
		}

		'static' {
			s->active_char += sizeof("static")-1;
			token->opcode = ZEPHIR_T_STATIC;
			return 0;
		}

		'inline' {
			s->active_char += sizeof("inline")-1;
			token->opcode = ZEPHIR_T_INLINE;
			return 0;
		}

		'final' {
			s->active_char += sizeof("final")-1;
			token->opcode = ZEPHIR_T_FINAL;
			return 0;
		}

		'abstract' {
			s->active_char += sizeof("abstract")-1;
			token->opcode = ZEPHIR_T_ABSTRACT;
			return 0;
		}

		'function' {
			s->active_char += sizeof("function")-1;
			token->opcode = ZEPHIR_T_FUNCTION;
			return 0;
		}

		'fn' {
			s->active_char += sizeof("fn")-1;
			token->opcode = ZEPHIR_T_FUNCTION;
			return 0;
		}

		'let' {
			s->active_char += sizeof("let")-1;
			token->opcode = ZEPHIR_T_LET;
			return 0;
		}

		'echo' {
			s->active_char += sizeof("echo")-1;
			token->opcode = ZEPHIR_T_ECHO;
			return 0;
		}

		'const' {
			s->active_char += sizeof("const")-1;
			token->opcode = ZEPHIR_T_CONST;
			return 0;
		}

		'int' {
			s->active_char += sizeof("int")-1;
			token->opcode = ZEPHIR_T_TYPE_INTEGER;
			return 0;
		}

		'uint' {
			s->active_char += sizeof("uint")-1;
			token->opcode = ZEPHIR_T_TYPE_UINTEGER;
			return 0;
		}

		'long' {
			s->active_char += sizeof("long")-1;
			token->opcode = ZEPHIR_T_TYPE_LONG;
			return 0;
		}

		'ulong' {
			s->active_char += sizeof("ulong")-1;
			token->opcode = ZEPHIR_T_TYPE_ULONG;
			return 0;
		}

		'char' {
			s->active_char += sizeof("char")-1;
			token->opcode = ZEPHIR_T_TYPE_CHAR;
			return 0;
		}

		'uchar' {
			s->active_char += sizeof("uchar")-1;
			token->opcode = ZEPHIR_T_TYPE_UCHAR;
			return 0;
		}

		'double' {
			s->active_char += sizeof("double")-1;
			token->opcode = ZEPHIR_T_TYPE_DOUBLE;
			return 0;
		}

		'float' {
			s->active_char += sizeof("float")-1;
			token->opcode = ZEPHIR_T_TYPE_DOUBLE;
			return 0;
		}

		'bool' {
			s->active_char += sizeof("bool")-1;
			token->opcode = ZEPHIR_T_TYPE_BOOL;
			return 0;
		}

		'boolean' {
			s->active_char += sizeof("boolean")-1;
			token->opcode = ZEPHIR_T_TYPE_BOOL;
			return 0;
		}

		'string' {
			s->active_char += sizeof("string")-1;
			token->opcode = ZEPHIR_T_TYPE_STRING;
			return 0;
		}

		'array' {
			s->active_char += sizeof("array")-1;
			token->opcode = ZEPHIR_T_TYPE_ARRAY;
			return 0;
		}

		'var' {
			s->active_char += sizeof("var")-1;
			token->opcode = ZEPHIR_T_TYPE_VAR;
			return 0;
		}

		'object' {
            s->active_char += sizeof("object")-1;
            token->opcode = ZEPHIR_T_TYPE_OBJECT;
            return 0;
        }

        'callable' {
            s->active_char += sizeof("callable")-1;
            token->opcode = ZEPHIR_T_TYPE_CALLABLE;
            return 0;
        }

        'resource' {
            s->active_char += sizeof("resource")-1;
            token->opcode = ZEPHIR_T_TYPE_RESOURCE;
            return 0;
        }

		'if' {
			s->active_char += sizeof("if")-1;
			token->opcode = ZEPHIR_T_IF;
			return 0;
		}

		'else' {
			s->active_char += sizeof("else")-1;
			token->opcode = ZEPHIR_T_ELSE;
			return 0;
		}

		'do' {
			s->active_char += sizeof("do")-1;
			token->opcode = ZEPHIR_T_DO;
			return 0;
		}

		'while' {
			s->active_char += sizeof("while")-1;
			token->opcode = ZEPHIR_T_WHILE;
			return 0;
		}

		'for' {
			s->active_char += sizeof("for")-1;
			token->opcode = ZEPHIR_T_FOR;
			return 0;
		}

		'in' {
			s->active_char += sizeof("in")-1;
			token->opcode = ZEPHIR_T_IN;
			return 0;
		}

		'new' {
			s->active_char += sizeof("new")-1;
			token->opcode = ZEPHIR_T_NEW;
			return 0;
		}

		'return' {
			s->active_char += sizeof("return")-1;
			token->opcode = ZEPHIR_T_RETURN;
			return 0;
		}

		'require' {
			s->active_char += sizeof("require")-1;
			token->opcode = ZEPHIR_T_REQUIRE;
			return 0;
		}

		'clone' {
			s->active_char += sizeof("clone")-1;
			token->opcode = ZEPHIR_T_CLONE;
			return 0;
		}

		'empty' {
			s->active_char += sizeof("empty")-1;
			token->opcode = ZEPHIR_T_EMPTY;
			return 0;
		}

		'void' {
			s->active_char += sizeof("void")-1;
			token->opcode = ZEPHIR_T_VOID;
			return 0;
		}

		'loop' {
			token->opcode = ZEPHIR_T_LOOP;
			s->active_char += sizeof("loop")-1;
			return 0;
		}

		'break' {
			token->opcode = ZEPHIR_T_BREAK;
			s->active_char += sizeof("break")-1;
			return 0;
		}

		'continue' {
			token->opcode = ZEPHIR_T_CONTINUE;
			s->active_char += sizeof("continue")-1;
			return 0;
		}

		'typeof' {
			token->opcode = ZEPHIR_T_TYPEOF;
			s->active_char += sizeof("typeof")-1;
			return 0;
		}

		'instanceof' {
			token->opcode = ZEPHIR_T_INSTANCEOF;
			s->active_char += sizeof("instanceof")-1;
			return 0;
		}

		'likely' {
			s->active_char += sizeof("likely")-1;
			token->opcode = ZEPHIR_T_LIKELY;
			return 0;
		}

		'unlikely' {
			s->active_char += sizeof("unlikely")-1;
			token->opcode = ZEPHIR_T_UNLIKELY;
			return 0;
		}

		'isset' {
			token->opcode = ZEPHIR_T_ISSET;
			s->active_char += sizeof("isset")-1;
			return 0;
		}

		'unset' {
			token->opcode = ZEPHIR_T_UNSET;
			s->active_char += sizeof("unset")-1;
			return 0;
		}

		'throw' {
			token->opcode = ZEPHIR_T_THROW;
			s->active_char += sizeof("throw")-1;
			return 0;
		}

		'fetch' {
			token->opcode = ZEPHIR_T_FETCH;
			s->active_char += sizeof("fetch")-1;
			return 0;
		}

		'switch' {
			token->opcode = ZEPHIR_T_SWITCH;
			s->active_char += sizeof("switch")-1;
			return 0;
		}

		'case' {
			token->opcode = ZEPHIR_T_CASE;
			s->active_char += sizeof("case")-1;
			return 0;
		}

		'default' {
			token->opcode = ZEPHIR_T_DEFAULT;
			s->active_char += sizeof("default")-1;
			return 0;
		}

		'reverse' {
			token->opcode = ZEPHIR_T_REVERSE;
			s->active_char += sizeof("reverse")-1;
			return 0;
		}

		'try' {
			s->active_char += sizeof("try")-1;
			token->opcode = ZEPHIR_T_TRY;
			return 0;
		}

		'catch' {
			s->active_char += sizeof("catch")-1;
			token->opcode = ZEPHIR_T_CATCH;
			return 0;
		}

		SCHAR = (['] ([\\][']|[\\].|[\001-\377]\[\\'])* [']);
		SCHAR {
			token->opcode = ZEPHIR_T_CHAR;
			token->value = estrndup(q, YYCURSOR - q - 1);
			token->len = YYCURSOR - q - 1;
			s->active_char += (YYCURSOR - start);
			q = YYCURSOR;
			return 0;
		}

		STRING = (["] ([\\]["]|[\\].|[\001-\377]\[\\"])* ["]);
		STRING {
			token->opcode = ZEPHIR_T_STRING;
			token->value = estrndup(q, YYCURSOR - q - 1);
			token->len = YYCURSOR - q - 1;
			s->active_char += (YYCURSOR - start);
			q = YYCURSOR;
			return 0;
		}

		COMMENT = ("/*"([^*]+|[*]+[^/*])*[*]*"*/");
		COMMENT {
			s->active_char += (YYCURSOR - start);
			token->opcode = ZEPHIR_T_IGNORE;
			return 0;
		}

		SLCOMMENT = ("//"[^\r\n]*);
		SLCOMMENT {
			s->active_char += (YYCURSOR - start);
			token->opcode = ZEPHIR_T_IGNORE;
			return 0;
		}

		CBLOCK = ("%{"([^}]+|[}]+[^%{])*"}%");
		CBLOCK {
			token->opcode = ZEPHIR_T_CBLOCK;
			token->value = estrndup(q+1, YYCURSOR - q - 3 );
			token->len = YYCURSOR - q - 3;
			{
				int k, ch = s->active_char;
				for (k = 0; k < (token->len - 1); k++) {
					if (token->value[k] == '\n') {
						ch = 1;
						s->active_line++;
					} else {
						ch++;
					}
				}
				s->active_char = ch;
			}
			q = YYCURSOR;
			return 0;
		}

		IDENTIFIER = [\\_\$]?[\_a-zA-Z\\][a-zA-Z0-9\_\\]*;
		IDENTIFIER {

			if (start[0] == '$') {
				token->value = estrndup(start + 1, YYCURSOR - start - 1);
				token->len = YYCURSOR - start - 1;
				s->active_char += (YYCURSOR - start - 1);
			} else {
				token->value = estrndup(start, YYCURSOR - start);
				token->len = YYCURSOR - start;
				s->active_char += (YYCURSOR - start);
			}
			q = YYCURSOR;

			if (!memcmp(token->value, "_GET", sizeof("_GET")-1)) {
				token->opcode = ZEPHIR_T_IDENTIFIER;
				return 0;
			}

			if (!memcmp(token->value, "_POST", sizeof("_POST")-1)) {
				token->opcode = ZEPHIR_T_IDENTIFIER;
				return 0;
			}

			if (!memcmp(token->value, "_REQUEST", sizeof("_REQUEST")-1)) {
				token->opcode = ZEPHIR_T_IDENTIFIER;
				return 0;
			}

			if (!memcmp(token->value, "_COOKIE", sizeof("_COOKIE")-1)) {
				token->opcode = ZEPHIR_T_IDENTIFIER;
				return 0;
			}

			if (!memcmp(token->value, "_SERVER", sizeof("_SERVER")-1)) {
				token->opcode = ZEPHIR_T_IDENTIFIER;
				return 0;
			}

			if (!memcmp(token->value, "_SESSION", sizeof("_SESSION")-1)) {
				token->opcode = ZEPHIR_T_IDENTIFIER;
				return 0;
			}

			if (!memcmp(token->value, "_FILES", sizeof("_FILES")-1)) {
				token->opcode = ZEPHIR_T_IDENTIFIER;
				return 0;
			}

			is_constant = 1;
			for (j = 0; j < token->len; j++) {
				if (!((token->value[j] >= 'A' && token->value[j] <= 'Z') || (token->value[j] >= '0' && token->value[j] <= '9') || token->value[j] == '_')) {
					is_constant = 0;
					break;
				}
			};
			if (is_constant) {
				token->opcode = ZEPHIR_T_CONSTANT;
			} else {
				token->opcode = ZEPHIR_T_IDENTIFIER;
			}
			return 0;
		}

		"(" {
			s->active_char++;
			token->opcode = ZEPHIR_T_PARENTHESES_OPEN;
			return 0;
		}

		")" {
			s->active_char++;
			token->opcode = ZEPHIR_T_PARENTHESES_CLOSE;
			return 0;
		}

		"{" {
			s->active_char++;
			token->opcode = ZEPHIR_T_BRACKET_OPEN;
			return 0;
		}

		"}" {
			s->active_char++;
			token->opcode = ZEPHIR_T_BRACKET_CLOSE;
			return 0;
		}

		"[" {
			s->active_char++;
			token->opcode = ZEPHIR_T_SBRACKET_OPEN;
			return 0;
		}

		"]" {
			s->active_char++;
			token->opcode = ZEPHIR_T_SBRACKET_CLOSE;
			return 0;
		}

		"@" {
			s->active_char++;
			token->opcode = ZEPHIR_T_AT;
			return 0;
		}

		"!" {
			s->active_char++;
			token->opcode = ZEPHIR_T_NOT;
			return 0;
		}

		"&&" {
			s->active_char += 2;
			token->opcode = ZEPHIR_T_AND;
			return 0;
		}

		"||" {
			s->active_char += 2;
			token->opcode = ZEPHIR_T_OR;
			return 0;
		}

		"&" {
			s->active_char++;
			token->opcode = ZEPHIR_T_BITWISE_AND;
			return 0;
		}

		"|" {
			s->active_char++;
			token->opcode = ZEPHIR_T_BITWISE_OR;
			return 0;
		}

		"^" {
			s->active_char++;
			token->opcode = ZEPHIR_T_BITWISE_XOR;
			return 0;
		}

		"<<" {
			s->active_char += 2;
			token->opcode = ZEPHIR_T_BITWISE_SHIFTLEFT;
			return 0;
		}

		">>" {
			s->active_char += 2;
			token->opcode = ZEPHIR_T_BITWISE_SHIFTRIGHT;
			return 0;
		}

		"=" {
			s->active_char++;
			token->opcode = ZEPHIR_T_ASSIGN;
			return 0;
		}

		"+=" {
			s->active_char++;
			token->opcode = ZEPHIR_T_ADDASSIGN;
			return 0;
		}

		"-=" {
			s->active_char++;
			token->opcode = ZEPHIR_T_SUBASSIGN;
			return 0;
		}

		"*=" {
			s->active_char++;
			token->opcode = ZEPHIR_T_MULASSIGN;
			return 0;
		}

		"/=" {
			s->active_char++;
			token->opcode = ZEPHIR_T_DIVASSIGN;
			return 0;
		}

		"%=" {
			s->active_char++;
			token->opcode = ZEPHIR_T_MODASSIGN;
			return 0;
		}

		".=" {
			s->active_char++;
			token->opcode = ZEPHIR_T_CONCATASSIGN;
			return 0;
		}

		"==" {
			s->active_char += 2;
			token->opcode = ZEPHIR_T_EQUALS;
			return 0;
		}

		"!=" {
			s->active_char += 2;
			token->opcode = ZEPHIR_T_NOTEQUALS;
			return 0;
		}

		"===" {
			s->active_char += 3;
			token->opcode = ZEPHIR_T_IDENTICAL;
			return 0;
		}

		"!==" {
			s->active_char += 3;
			token->opcode = ZEPHIR_T_NOTIDENTICAL;
			return 0;
		}

		"<=" {
			s->active_char++;
			token->opcode = ZEPHIR_T_LESSEQUAL;
			return 0;
		}

		">=" {
			s->active_char++;
			token->opcode = ZEPHIR_T_GREATEREQUAL;
			return 0;
		}

		"<" {
			s->active_char++;
			token->opcode = ZEPHIR_T_LESS;
			return 0;
		}

		">" {
			s->active_char++;
			token->opcode = ZEPHIR_T_GREATER;
			return 0;
		}

		"->" {
			s->active_char += 2;
			token->opcode = ZEPHIR_T_ARROW;
			return 0;
		}

		"::" {
			s->active_char += 2;
			token->opcode = ZEPHIR_T_DOUBLECOLON;
			return 0;
		}

		"." {
			s->active_char++;
			token->opcode = ZEPHIR_T_DOT;
			return 0;
		}

		"+" {
			s->active_char++;
			token->opcode = ZEPHIR_T_ADD;
			return 0;
		}

		"-" {
			s->active_char++;
			token->opcode = ZEPHIR_T_SUB;
			return 0;
		}

		"*" {
			s->active_char++;
			token->opcode = ZEPHIR_T_MUL;
			return 0;
		}

		"/" {
			s->active_char++;
			token->opcode = ZEPHIR_T_DIV;
			return 0;
		}

		"%" {
			s->active_char++;
			token->opcode = ZEPHIR_T_MOD;
			return 0;
		}

		"++" {
			s->active_char += 2;
			token->opcode = ZEPHIR_T_INCR;
			return 0;
		}

		"--" {
			s->active_char += 2;
			token->opcode = ZEPHIR_T_DECR;
			return 0;
		}

		":" {
			s->active_char++;
			token->opcode = ZEPHIR_T_COLON;
			return 0;
		}

		";" {
			s->active_char++;
			token->opcode = ZEPHIR_T_DOTCOMMA;
			return 0;
		}

		"," {
			s->active_char++;
			token->opcode = ZEPHIR_T_COMMA;
			return 0;
		}

		"?" {
			s->active_char++;
			token->opcode = ZEPHIR_T_QUESTION;
			return 0;
		}

		[ \t\r]+ {
			s->active_char += (YYCURSOR - start);
			token->opcode = ZEPHIR_T_IGNORE;
			return 0;
		}

		[\n] {
			s->active_line++;
			s->active_char = 0;
			token->opcode = ZEPHIR_T_IGNORE;
			return 0;
		}

		"\000" {
			status = ZEPHIR_SCANNER_RETCODE_EOF;
			break;
		}

		[^] {
			status = ZEPHIR_SCANNER_RETCODE_ERR;
			break;
		}

		*/
	}

	return status;
}
