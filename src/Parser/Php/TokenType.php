<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

declare(strict_types=1);

namespace Zephir\Parser\Php;

/**
 * Token type identifiers, mirroring `parser/scanner.h` of the C extension
 * (php-zephir-parser) one-to-one. Single-character operators use their ASCII
 * code as the id (e.g. '.', '='); multi-character operators and keywords use
 * the 300+ range. Keeping these identical to the C source keeps the port
 * verifiable against the reference implementation.
 */
final class TokenType
{
    /* Scanner return codes (xx_get_token) */
    public const RETCODE_EOF        = -1;
    public const RETCODE_ERR        = -2;
    public const RETCODE_IMPOSSIBLE = -3;

    /* Modes */
    public const T_IGNORE = 297;

    /* Literals & Identifiers */
    public const T_INTEGER    = 301;
    public const T_DOUBLE     = 302;
    public const T_STRING     = 303;
    public const T_NULL       = 304;
    public const T_FALSE      = 305;
    public const T_TRUE       = 306;
    public const T_IDENTIFIER = 307;
    public const T_ARRAY      = 308;
    public const T_CHAR       = 309;
    public const T_ISTRING    = 310;

    /* Type keywords */
    public const T_TYPE_INTEGER  = 320;
    public const T_TYPE_DOUBLE   = 321;
    public const T_TYPE_BOOL     = 322;
    public const T_TYPE_STRING   = 323;
    public const T_TYPE_VAR      = 324;
    public const T_TYPE_LONG     = 325;
    public const T_TYPE_ULONG    = 326;
    public const T_TYPE_CHAR     = 327;
    public const T_TYPE_UCHAR    = 328;
    public const T_TYPE_UINTEGER = 329;
    public const T_TYPE_ARRAY    = 330;
    public const T_TYPE_CALLABLE = 331;
    public const T_TYPE_OBJECT   = 332;
    public const T_TYPE_RESOURCE = 333;
    public const T_TYPE_NULL     = 334;
    public const T_TYPE_THIS     = 335;
    public const T_TYPE_MIXED    = 336;
    public const T_TYPE_FALSE    = 337;

    /* Keywords */
    public const T_NAMESPACE    = 350;
    public const T_CLASS        = 351;
    public const T_PUBLIC       = 352;
    public const T_PROTECTED    = 353;
    public const T_EXTENDS      = 354;
    public const T_FUNCTION     = 355;
    public const T_LET          = 356;
    public const T_COMMENT      = 357;
    public const T_ECHO         = 358;
    public const T_CONST        = 359;
    public const T_ABSTRACT     = 360;
    public const T_IMPLEMENTS   = 361;
    public const T_INTERFACE    = 362;
    public const T_IF           = 363;
    public const T_ELSE         = 364;
    public const T_WHILE        = 365;
    public const T_NEW          = 366;
    public const T_RETURN       = 367;
    public const T_LOOP         = 368;
    public const T_BREAK        = 369;
    public const T_CONTINUE     = 370;
    public const T_INSTANCEOF   = 371;
    public const T_TYPEOF       = 372;
    public const T_ISSET        = 373;
    public const T_UNSET        = 374;
    public const T_THROW        = 375;
    public const T_FOR          = 376;
    public const T_IN           = 377;
    public const T_FETCH        = 378;
    public const T_SWITCH       = 379;
    public const T_CASE         = 380;
    public const T_DEFAULT      = 381;
    public const T_REVERSE      = 382;
    public const T_PRIVATE      = 383;
    public const T_STATIC       = 384;
    public const T_INLINE       = 385;
    public const T_FINAL        = 386;
    public const T_CONSTANT     = 387;
    public const T_DO           = 388;
    public const T_REQUIRE      = 389;
    public const T_CLONE        = 390;
    public const T_EMPTY        = 391;
    public const T_VOID         = 392;
    public const T_LIKELY       = 393;
    public const T_UNLIKELY     = 394;
    public const T_USE          = 395;
    public const T_AS           = 396;
    public const T_TRY          = 397;
    public const T_CATCH        = 398;
    public const T_DEPRECATED   = 399;
    public const T_REQUIRE_ONCE = 459;
    public const T_YIELD        = 460;
    public const T_ELSEIF       = 452;
    public const T_INTERNAL     = 453;

    /* Single-character operators (ASCII code == id) */
    public const T_AT                = 64;  /* @ */
    public const T_DOT               = 46;  /* . */
    public const T_COMMA             = 44;  /* , */
    public const T_ASSIGN            = 61;  /* = */
    public const T_LESS              = 60;  /* < */
    public const T_GREATER           = 62;  /* > */
    public const T_COLON             = 58;  /* : */
    public const T_DOTCOMMA          = 59;  /* ; */
    public const T_QUESTION          = 63;  /* ? */
    public const T_BRACKET_OPEN      = 123; /* { */
    public const T_BRACKET_CLOSE     = 125; /* } */
    public const T_SBRACKET_OPEN     = 91;  /* [ */
    public const T_SBRACKET_CLOSE    = 93;  /* ] */
    public const T_PARENTHESES_OPEN  = 40;  /* ( */
    public const T_PARENTHESES_CLOSE = 41;  /* ) */
    public const T_BITWISE_OR        = 124; /* | */
    public const T_BITWISE_AND       = 38;  /* & */
    public const T_BITWISE_XOR       = 94;  /* ^ */
    public const T_ADD               = 43;  /* + */
    public const T_SUB               = 45;  /* - */
    public const T_MUL               = 42;  /* * */
    public const T_DIV               = 47;  /* / */
    public const T_MOD               = 37;  /* % */
    public const T_BITWISE_NOT       = 126; /* ~ */

    /* Multi-character operators */
    public const T_ARROW                       = 400;
    public const T_EQUALS                      = 401;
    public const T_IDENTICAL                   = 402;
    public const T_INCR                        = 403;
    public const T_DECR                        = 404;
    public const T_NOTEQUALS                   = 405;
    public const T_NOTIDENTICAL                = 406;
    public const T_NOT                         = 407;
    public const T_GREATEREQUAL                = 408;
    public const T_LESSEQUAL                   = 409;
    public const T_ASSIGN_ADD                  = 410;
    public const T_ASSIGN_SUB                  = 411;
    public const T_ASSIGN_MUL                  = 412;
    public const T_ASSIGN_DIV                  = 413;
    public const T_ASSIGN_CONCAT               = 414;
    public const T_AND                         = 415;
    public const T_OR                          = 416;
    public const T_DOUBLECOLON                 = 417;
    public const T_ASSIGN_MOD                  = 418;
    public const T_BITWISE_SHIFTLEFT           = 419;
    public const T_BITWISE_SHIFTRIGHT          = 420;
    public const T_DOUBLEARROW                 = 440;
    public const T_INCLUSIVE_RANGE             = 441;
    public const T_EXCLUSIVE_RANGE             = 442;
    public const T_CBLOCK                      = 451;
    public const T_ASSIGN_BITWISE_AND          = 454;
    public const T_ASSIGN_BITWISE_OR           = 455;
    public const T_ASSIGN_BITWISE_XOR          = 456;
    public const T_ASSIGN_BITWISE_SHIFTLEFT    = 457;
    public const T_ASSIGN_BITWISE_SHIFTRIGHT   = 458;

    private function __construct()
    {
    }
}
