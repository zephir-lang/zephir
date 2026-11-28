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

namespace Zephir\Expression;

use Zephir\CompilationContext;

use function ltrim;
use function strtolower;

/**
 * Maps the class constants whose values depend on the PHP that compiles the
 * C to the engine macros that define them, with the header each macro needs.
 *
 * A value read from the PHP that runs `zephir generate` is wrong wherever the
 * C is compiled against a different PHP, which is the normal case for a
 * committed `ext/` or a PECL package. php-src declares these constants from
 * the same macros (`@cvalue` in its stubs), so the macro is emitted instead.
 *
 * - `Attribute::*`: PHP 8.5 added `TARGET_CONSTANT` and renumbered
 *   `TARGET_ALL` and `IS_REPEATABLE` (#2738). `ZEND_ATTRIBUTE_TARGET_CONST`
 *   exists only on PHP 8.5+. Compiled against an older PHP it is an
 *   undeclared identifier, which is the closest a build can get to PHP's own
 *   "Undefined constant Attribute::TARGET_CONSTANT".
 * - `PDO::FETCH_*` flags: PHP 8.5 renumbered them (#2869) and defines them
 *   unsigned (`1u << 5u`). They are cast like php-src's `LONG_CONST()`, or
 *   `~PDO::FETCH_GROUP` would be a large positive number.
 * - `FilesystemIterator` flags: PHP 8.1 renumbered them (#2869).
 */
final class EngineConstants
{
    /**
     * Keyed by the lowercase name of the class that declares the constants
     * (PHP class names are case-insensitive), then by the constant name, which
     * is case-sensitive. A null header is one that `php.h` already includes.
     */
    private const CLASSES = [
        'attribute'          => [
            'class'  => 'Attribute',
            'header' => null,
            'macros' => [
                'TARGET_CLASS'          => 'ZEND_ATTRIBUTE_TARGET_CLASS',
                'TARGET_FUNCTION'       => 'ZEND_ATTRIBUTE_TARGET_FUNCTION',
                'TARGET_METHOD'         => 'ZEND_ATTRIBUTE_TARGET_METHOD',
                'TARGET_PROPERTY'       => 'ZEND_ATTRIBUTE_TARGET_PROPERTY',
                'TARGET_CLASS_CONSTANT' => 'ZEND_ATTRIBUTE_TARGET_CLASS_CONST',
                'TARGET_PARAMETER'      => 'ZEND_ATTRIBUTE_TARGET_PARAMETER',
                'TARGET_CONSTANT'       => 'ZEND_ATTRIBUTE_TARGET_CONST',
                'TARGET_ALL'            => 'ZEND_ATTRIBUTE_TARGET_ALL',
                'IS_REPEATABLE'         => 'ZEND_ATTRIBUTE_IS_REPEATABLE',
            ],
        ],
        'pdo'                => [
            'class'  => 'PDO',
            'header' => 'ext/pdo/php_pdo_driver',
            'macros' => [
                'FETCH_GROUP'      => '((zend_long) PDO_FETCH_GROUP)',
                'FETCH_UNIQUE'     => '((zend_long) PDO_FETCH_UNIQUE)',
                'FETCH_CLASSTYPE'  => '((zend_long) PDO_FETCH_CLASSTYPE)',
                'FETCH_PROPS_LATE' => '((zend_long) PDO_FETCH_PROPS_LATE)',
                'FETCH_SERIALIZE'  => '((zend_long) PDO_FETCH_SERIALIZE)',
            ],
        ],
        'filesystemiterator' => [
            'class'  => 'FilesystemIterator',
            'header' => 'ext/spl/spl_directory',
            'macros' => [
                'FOLLOW_SYMLINKS' => 'SPL_FILE_DIR_FOLLOW_SYMLINKS',
                'OTHER_MODE_MASK' => 'SPL_FILE_DIR_OTHERS_MASK',
            ],
        ],
    ];

    /**
     * The C macro, the fully-qualified PHP spelling and the headers the macro
     * needs, or null when the constant has no macro.
     *
     * @param string|null $className the class that declares the constant
     *
     * @return array{c: string, qualified: string, headers: list<string>}|null
     */
    public static function lookup(?string $className, string $constant): ?array
    {
        $class = self::CLASSES[strtolower(ltrim((string) $className, '\\'))] ?? null;
        $macro = $class['macros'][$constant] ?? null;

        if (null === $macro) {
            return null;
        }

        return [
            'c'         => $macro,
            'qualified' => '\\' . $class['class'] . '::' . $constant,
            'headers'   => null === $class['header'] ? [] : [$class['header']],
        ];
    }

    /**
     * Includes $headers in the file being compiled.
     *
     * @param list<string> $headers
     */
    public static function addHeaders(array $headers, CompilationContext $compilationContext): void
    {
        foreach ($headers as $header) {
            $compilationContext->headersManager?->add($header);
        }
    }

    /**
     * Includes the headers of every macro $node carries, alone or inside an
     * array, in the file being compiled. A folded node is cached on its
     * constant and may be emitted in a file other than the one it was folded
     * in, so each file that emits it asks for the headers again.
     */
    public static function addNodeHeaders(array $node, CompilationContext $compilationContext): void
    {
        self::addHeaders($node['native']['headers'] ?? [], $compilationContext);

        if ('array' !== ($node['type'] ?? null)) {
            return;
        }

        foreach ($node['left'] as $item) {
            if (isset($item['key'])) {
                self::addNodeHeaders($item['key'], $compilationContext);
            }

            self::addNodeHeaders($item['value'], $compilationContext);
        }
    }
}
