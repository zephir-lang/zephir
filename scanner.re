

#include "string.h"
#include "scanner.h"

#define YYCTYPE unsigned char
#define YYCURSOR (s->start)
#define YYLIMIT (s->end)
#define YYMARKER q

int xx_get_token(xx_scanner_state *s, xx_scanner_token *token) {

	char next, *q = YYCURSOR, *start = YYCURSOR;
	int status = XX_SCANNER_RETCODE_IMPOSSIBLE;

	while (XX_SCANNER_RETCODE_IMPOSSIBLE == status) {

		/*!re2c
		re2c:indent:top = 2;
		re2c:yyfill:enable = 0;

		INTEGER = [\-]?[0-9]+;
		INTEGER {
			token->opcode = XX_T_INTEGER;
			token->value = strndup(start, YYCURSOR - start);
			token->len = YYCURSOR - start;
			q = YYCURSOR;
			return 0;
		}

		DOUBLE = ([\-]?[0-9]+[\.][0-9]+);
		DOUBLE {
			token->opcode = XX_T_DOUBLE;
			token->value = strndup(start, YYCURSOR - start);
			token->len = YYCURSOR - start;
			q = YYCURSOR;
			return 0;
		}

		'null' {
			token->opcode = XX_T_NULL;
			return 0;
		}

		'false' {
			token->opcode = XX_T_FALSE;
			return 0;
		}

		'true' {
			token->opcode = XX_T_TRUE;
			return 0;
		}

		'namespace' {
			token->opcode = XX_T_NAMESPACE;
			return 0;
		}

		'class' {
			token->opcode = XX_T_CLASS;
			return 0;
		}

		'extends' {
			token->opcode = XX_T_EXTENDS;
			return 0;
		}

		'public' {
			token->opcode = XX_T_PUBLIC;
			return 0;
		}

		'protected' {
			token->opcode = XX_T_PROTECTED;
			return 0;
		}

		'function' {
			token->opcode = XX_T_FUNCTION;
			return 0;
		}

		'fn' {
			token->opcode = XX_T_FUNCTION;
			return 0;
		}

		'let' {
			token->opcode = XX_T_LET;
			return 0;
		}

		STRING = (["] ([\\]["]|[\\].|[\001-\377]\[\\"])* ["])|(['] ([\\][']|[\\].|[\001-\377]\[\\'])* [']);
		STRING {
			token->opcode = XX_T_STRING;
			token->value = strndup(q, YYCURSOR - q - 1);
			token->len = YYCURSOR - q - 1;
			q = YYCURSOR;
			return 0;
		}

		COMMENT = ([/][\*] ([\001-\377]\[\\\*])* [\*][/]);
		COMMENT {
			token->opcode = XX_T_COMMENT;
			token->value = strndup(q, YYCURSOR - q - 1);
			token->len = YYCURSOR - q - 1;
			q = YYCURSOR;
			return 0;
		}

		IDENTIFIER = [\\\_]?[\_a-zA-Z\\][a-zA-Z0-9\_\\]*;
		IDENTIFIER {
			token->opcode = XX_T_IDENTIFIER;
			token->value = strndup(start, YYCURSOR - start);
			token->len = YYCURSOR - start;
			q = YYCURSOR;
			return 0;
		}

		"(" {
			token->opcode = XX_T_PARENTHESES_OPEN;
			return 0;
		}

		")" {
			token->opcode = XX_T_PARENTHESES_CLOSE;
			return 0;
		}

		"{" {
			token->opcode = XX_T_BRACKET_OPEN;
			return 0;
		}

		"}" {
			token->opcode = XX_T_BRACKET_CLOSE;
			return 0;
		}

		"[" {
			token->opcode = XX_T_SBRACKET_OPEN;
			return 0;
		}

		"]" {
			token->opcode = XX_T_SBRACKET_CLOSE;
			return 0;
		}

		"@" {
			token->opcode = XX_T_AT;
			return 0;
		}

		"=" {
			token->opcode = XX_T_ASSIGN;
			return 0;
		}

		":" {
			token->opcode = XX_T_COLON;
			return 0;
		}

		";" {
			token->opcode = XX_T_DOTCOMMA;
			return 0;
		}

		"," {
			token->opcode = XX_T_COMMA;
			return 0;
		}

		[ \t\r]+ {
			token->opcode = XX_T_IGNORE;
			return 0;
		}

		[\n] {
			s->active_line++;
			token->opcode = XX_T_IGNORE;
			return 0;
		}

		"\000" {
			status = XX_SCANNER_RETCODE_EOF;
			break;
		}

		[^] {
			status = XX_SCANNER_RETCODE_ERR;
			break;
		}

		*/
	}

	return status;
}
