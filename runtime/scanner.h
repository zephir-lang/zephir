
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

#ifndef PHP_ZEPHIR_RUNTIME_SCANNER_H
#define PHP_ZEPHIR_RUNTIME_SCANNER_H 1

#define ZEPHIR_SCANNER_RETCODE_EOF -1
#define ZEPHIR_SCANNER_RETCODE_ERR -2
#define ZEPHIR_SCANNER_RETCODE_IMPOSSIBLE -3

/** Modes */
#define ZEPHIR_T_IGNORE 297

/* Literals & Identifiers */
#define ZEPHIR_T_INTEGER 301
#define ZEPHIR_T_DOUBLE 302
#define ZEPHIR_T_STRING 303
#define ZEPHIR_T_NULL 304
#define ZEPHIR_T_FALSE 305
#define ZEPHIR_T_TRUE 306
#define ZEPHIR_T_IDENTIFIER 307
#define ZEPHIR_T_VARIABLE 307
#define ZEPHIR_T_ARRAY 308
#define ZEPHIR_T_CHAR 309

#define ZEPHIR_T_TYPE_INTEGER 320
#define ZEPHIR_T_TYPE_DOUBLE 321
#define ZEPHIR_T_TYPE_BOOL 322
#define ZEPHIR_T_TYPE_STRING 323
#define ZEPHIR_T_TYPE_VAR 324
#define ZEPHIR_T_TYPE_LONG 325
#define ZEPHIR_T_TYPE_ULONG 326
#define ZEPHIR_T_TYPE_CHAR 327
#define ZEPHIR_T_TYPE_UCHAR 328
#define ZEPHIR_T_TYPE_UINTEGER 329
#define ZEPHIR_T_TYPE_ARRAY 330
#define ZEPHIR_T_TYPE_CALLABLE 331
#define ZEPHIR_T_TYPE_OBJECT 332
#define ZEPHIR_T_TYPE_RESOURCE 333
#define ZEPHIR_T_TYPE_NULL 334
#define ZEPHIR_T_TYPE_THIS 335

#define ZEPHIR_T_NAMESPACE 350
#define ZEPHIR_T_CLASS 351
#define ZEPHIR_T_PUBLIC 352
#define ZEPHIR_T_PROTECTED 353
#define ZEPHIR_T_EXTENDS 354
#define ZEPHIR_T_FUNCTION 355
#define ZEPHIR_T_LET 356
#define ZEPHIR_T_COMMENT 357
#define ZEPHIR_T_ECHO 358
#define ZEPHIR_T_CONST 359
#define ZEPHIR_T_ABSTRACT 360
#define ZEPHIR_T_IMPLEMENTS 361
#define ZEPHIR_T_INTERFACE 362
#define ZEPHIR_T_IF 363
#define ZEPHIR_T_ELSE 364
#define ZEPHIR_T_WHILE 365
#define ZEPHIR_T_NEW 366
#define ZEPHIR_T_RETURN 367
#define ZEPHIR_T_LOOP 368
#define ZEPHIR_T_BREAK 369
#define ZEPHIR_T_CONTINUE 370
#define ZEPHIR_T_INSTANCEOF 371
#define ZEPHIR_T_TYPEOF 372
#define ZEPHIR_T_ISSET 373
#define ZEPHIR_T_UNSET 374
#define ZEPHIR_T_THROW 375
#define ZEPHIR_T_FOR 376
#define ZEPHIR_T_IN 377
#define ZEPHIR_T_FETCH 378
#define ZEPHIR_T_SWITCH 379
#define ZEPHIR_T_CASE 380
#define ZEPHIR_T_DEFAULT 381
#define ZEPHIR_T_REVERSE 382
#define ZEPHIR_T_PRIVATE 383
#define ZEPHIR_T_STATIC 384
#define ZEPHIR_T_INLINE 385
#define ZEPHIR_T_FINAL 386
#define ZEPHIR_T_CONSTANT 387
#define ZEPHIR_T_DO 388
#define ZEPHIR_T_REQUIRE 389
#define ZEPHIR_T_CLONE 390
#define ZEPHIR_T_EMPTY 391
#define ZEPHIR_T_VOID 392
#define ZEPHIR_T_LIKELY 393
#define ZEPHIR_T_UNLIKELY 394
#define ZEPHIR_T_USE 395
#define ZEPHIR_T_AS 396
#define ZEPHIR_T_TRY 397
#define ZEPHIR_T_CATCH 398

/* Operators */
#define ZEPHIR_T_AT '@'
#define ZEPHIR_T_DOT '.'
#define ZEPHIR_T_COMMA ','
#define ZEPHIR_T_ASSIGN '='
#define ZEPHIR_T_LESS '<'
#define ZEPHIR_T_GREATER '>'
#define ZEPHIR_T_COLON ':'
#define ZEPHIR_T_DOTCOMMA ';'
#define ZEPHIR_T_QUESTION '?'
#define ZEPHIR_T_BRACKET_OPEN '{'
#define ZEPHIR_T_BRACKET_CLOSE '}'
#define ZEPHIR_T_SBRACKET_OPEN '['
#define ZEPHIR_T_SBRACKET_CLOSE ']'
#define ZEPHIR_T_PARENTHESES_OPEN '('
#define ZEPHIR_T_PARENTHESES_CLOSE ')'
#define ZEPHIR_T_BITWISE_OR '|'
#define ZEPHIR_T_BITWISE_AND '&'
#define ZEPHIR_T_BITWISE_XOR '^'
#define ZEPHIR_T_ARROW 400
#define ZEPHIR_T_EQUALS 401
#define ZEPHIR_T_IDENTICAL 402
#define ZEPHIR_T_ADD '+'
#define ZEPHIR_T_SUB '-'
#define ZEPHIR_T_MUL '*'
#define ZEPHIR_T_DIV '/'
#define ZEPHIR_T_MOD '%'
#define ZEPHIR_T_INCR 403
#define ZEPHIR_T_DECR 404
#define ZEPHIR_T_NOTEQUALS 405
#define ZEPHIR_T_NOTIDENTICAL 406
#define ZEPHIR_T_NOT 407
#define ZEPHIR_T_GREATEREQUAL 408
#define ZEPHIR_T_LESSEQUAL 409
#define ZEPHIR_T_ADDASSIGN 410
#define ZEPHIR_T_SUBASSIGN 411
#define ZEPHIR_T_MULASSIGN 412
#define ZEPHIR_T_DIVASSIGN 413
#define ZEPHIR_T_CONCATASSIGN 414
#define ZEPHIR_T_AND 415
#define ZEPHIR_T_OR 416
#define ZEPHIR_T_DOUBLECOLON 417
#define ZEPHIR_T_MODASSIGN 418
#define ZEPHIR_T_BITWISE_SHIFTLEFT 419
#define ZEPHIR_T_BITWISE_SHIFTRIGHT 420

#define ZEPHIR_T_CBLOCK 451

/* List of tokens and their names */
typedef struct _zephir_token_names {
	unsigned int code;
	char *name;
} zephir_token_names;

/* Active token state */
typedef struct _zephir_scanner_state {
	int active_token;
	char* start;
	char* end;
	unsigned int start_length;
	int mode;
	unsigned int active_line;
	unsigned int active_char;
	char *active_file;
} zephir_scanner_state;

/* Extra information tokens */
typedef struct _zephir_scanner_token {
	int opcode;
	char *value;
	int len;
} zephir_scanner_token;

int zephir_get_token(zephir_scanner_state *s, zephir_scanner_token *token);

extern const zephir_token_names zephir_tokens[];

#endif
