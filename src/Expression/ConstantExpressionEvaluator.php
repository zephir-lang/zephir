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

use Throwable;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception\CompilerException;
use Zephir\Operators\AbstractOperator;

use function in_array;
use function is_bool;
use function is_finite;
use function is_float;
use function is_int;
use function is_string;
use function ord;
use function sprintf;
use function strlen;
use function var_export;

/**
 * Folds a constant expression in declaration position — a class constant
 * initializer or a class/trait property default — down to a single literal
 * node the backends can emit.
 *
 * Both parsers accept a full expression there (see zephir#2061), so
 * `const INT8_MIN = -0x7f - 1;` arrives as `sub(int -0x7f, int 1)` and has to
 * be reduced to `int -128` before `ZEPHIR_INIT_CLASS` is written.
 *
 * Semantics come from PHP itself: every leaf is decoded to a real PHP value,
 * the native PHP operator is applied, and the result is re-encoded as a node.
 * That is what makes `4 / 2` an `int`, `PHP_INT_MAX + 1` a `double` and
 * `1 == "1"` true without reimplementing any of those rules here.
 *
 * This is not the optional {@see \Zephir\Operators\Arithmetical\ArithmeticalBaseOperator::optimizeConstantFolding()}
 * peephole: a declaration has no runtime fallback, so folding here cannot be
 * gated on the `constant-folding` optimization and must recurse.
 */
final class ConstantExpressionEvaluator
{
    /**
     * Node types a caller can hand to a backend as-is.
     *
     * `constant` and `static-constant-access` are in the list because every
     * caller already resolves those two itself; they are still resolved when
     * they appear as a leaf *inside* an expression.
     */
    private const DECLARATION_TYPES = [
        'int',
        'uint',
        'long',
        'ulong',
        'double',
        'float',
        'string',
        'istring',
        'char',
        'bool',
        'null',
        'array',
        'empty-array',
        'constant',
        'static-constant-access',
    ];

    /** Unary operators, reading `left` only. */
    private const UNARY = ['minus', 'plus', 'not', 'bitwise_not'];

    /** Binary operators, reading `left` and `right`. */
    private const BINARY = [
        'add',
        'sub',
        'mul',
        'div',
        'mod',
        'concat',
        'bitwise_and',
        'bitwise_or',
        'bitwise_xor',
        'bitwise_shiftleft',
        'bitwise_shiftright',
        'equals',
        'not-equals',
        'identical',
        'not-identical',
        'less',
        'greater',
        'less-equal',
        'greater-equal',
    ];

    /**
     * Whether $node has to be folded before a backend can emit it.
     */
    public static function needsFolding(array $node): bool
    {
        if (!in_array($node['type'], self::DECLARATION_TYPES, true)) {
            return true;
        }

        // An array is emittable only when every element is.
        if ('array' === $node['type']) {
            foreach ($node['left'] as $item) {
                if (isset($item['key']) && self::needsFolding($item['key'])) {
                    return true;
                }

                if (self::needsFolding($item['value'])) {
                    return true;
                }
            }
        }

        return false;
    }

    /**
     * Folds $node to a literal node. Nodes that need no folding are returned
     * untouched, which also makes repeated folding a no-op.
     *
     * @throws CompilerException when $node is not a compile-time constant
     */
    public function fold(array $node, CompilationContext $compilationContext): array
    {
        // `(expr)` parses to a single-child `list` node. Unwrap before
        // dispatching, so a parenthesised array still takes the array path.
        while ('list' === $node['type']) {
            $node = $node['left'];
        }

        if (!self::needsFolding($node)) {
            return $node;
        }

        if ('array' === $node['type']) {
            return $this->foldArray($node, $compilationContext);
        }

        return $this->toNode($this->evaluate($node, $compilationContext), $node);
    }

    /**
     * Rebuilds an array node with every key and value folded, preserving the
     * shape {@see \Zephir\Backend\Backend::declareArrayConstant()} expects.
     */
    private function foldArray(array $node, CompilationContext $compilationContext): array
    {
        foreach ($node['left'] as $index => $item) {
            if (isset($item['key'])) {
                $node['left'][$index]['key'] = $this->fold($item['key'], $compilationContext);
            }

            $node['left'][$index]['value'] = $this->fold($item['value'], $compilationContext);
        }

        return $node;
    }

    /**
     * Evaluates $node to a PHP value.
     *
     * @throws CompilerException
     */
    private function evaluate(array $node, CompilationContext $compilationContext): mixed
    {
        $type = $node['type'];

        // `(expr)` parses to a single-child `list` node.
        if ('list' === $type) {
            return $this->evaluate($node['left'], $compilationContext);
        }

        if (in_array($type, self::DECLARATION_TYPES, true)) {
            return $this->leafValue($node, $compilationContext);
        }

        // Short-circuiting operators must not evaluate the dead branch, so that
        // `true ? 1 : 1 / 0` folds instead of reporting a division by zero.
        switch ($type) {
            case 'and':
                return $this->evaluate($node['left'], $compilationContext)
                    && $this->evaluate($node['right'], $compilationContext);

            case 'or':
                return $this->evaluate($node['left'], $compilationContext)
                    || $this->evaluate($node['right'], $compilationContext);

            case 'ternary':
                return $this->evaluate($node['left'], $compilationContext)
                    ? $this->evaluate($node['right'], $compilationContext)
                    : $this->evaluate($node['extra'], $compilationContext);

            case 'short-ternary':
                $condition = $this->evaluate($node['left'], $compilationContext);

                return $condition ?: $this->evaluate($node['extra'], $compilationContext);

            case 'concat':
                // A char is a byte value everywhere else in Zephir, so
                // concatenating one would silently produce "9798".
                $this->rejectChar($node['left']);
                $this->rejectChar($node['right']);
                break;
        }

        if (!in_array($type, self::UNARY, true) && !in_array($type, self::BINARY, true)) {
            throw new CompilerException(
                sprintf('Expression of type "%s" is not a compile-time constant expression', $type),
                $node
            );
        }

        $left  = $this->evaluate($node['left'], $compilationContext);
        $right = in_array($type, self::BINARY, true)
            ? $this->evaluate($node['right'], $compilationContext)
            : null;

        try {
            return $this->apply($type, $left, $right);
        } catch (Throwable $e) {
            // PHP's own diagnostics for division by zero, a negative shift or
            // unsupported operand types are better than anything reworded here.
            throw new CompilerException(
                'Cannot evaluate constant expression: ' . $e->getMessage(),
                $node,
                0,
                $e
            );
        }
    }

    private function apply(string $operator, mixed $left, mixed $right): mixed
    {
        return match ($operator) {
            'minus'              => -$left,
            'plus'               => +$left,
            'not'                => !$left,
            'bitwise_not'        => ~$left,
            'add'                => $left + $right,
            'sub'                => $left - $right,
            'mul'                => $left * $right,
            'div'                => $left / $right,
            'mod'                => $left % $right,
            'concat'             => $left . $right,
            'bitwise_and'        => $left & $right,
            'bitwise_or'         => $left | $right,
            'bitwise_xor'        => $left ^ $right,
            'bitwise_shiftleft'  => $left << $right,
            'bitwise_shiftright' => $left >> $right,
            'equals'             => $left == $right,
            'not-equals'         => $left != $right,
            'identical'          => $left === $right,
            'not-identical'      => $left !== $right,
            'less'               => $left < $right,
            'greater'            => $left > $right,
            'less-equal'         => $left <= $right,
            'greater-equal'      => $left >= $right,
        };
    }

    /**
     * Decodes a leaf node to a PHP value.
     *
     * @throws CompilerException
     */
    private function leafValue(array $node, CompilationContext $compilationContext): mixed
    {
        switch ($node['type']) {
            case 'int':
            case 'uint':
            case 'long':
            case 'ulong':
                // Hexadecimal literals are kept verbatim by the parser. See #2014.
                return AbstractOperator::literalToNumber((string) $node['value']);

            case 'double':
            case 'float':
                return (float) $node['value'];

            case 'string':
            case 'istring':
                return (string) $node['value'];

            case 'char':
                return $this->charValue($node);

            case 'bool':
                return 'true' === $node['value'];

            case 'null':
                return null;

            case 'array':
            case 'empty-array':
                throw new CompilerException(
                    'An array cannot be used as an operand in a constant expression',
                    $node
                );

            case 'constant':
                return $this->compiledValue(
                    (new Constants())->compile($node, $compilationContext),
                    $node
                );

            case 'static-constant-access':
                return $this->compiledValue(
                    (new StaticConstantAccess())->compile($node, $compilationContext),
                    $node
                );
        }

        throw new CompilerException(
            sprintf('Expression of type "%s" is not a compile-time constant expression', $node['type']),
            $node
        );
    }

    /**
     * A char literal is its byte value, matching how a declared `char` behaves
     * everywhere else in Zephir (see #1629).
     *
     * @throws CompilerException
     */
    private function charValue(array $node): int
    {
        $value = (string) $node['value'];

        if (1 !== strlen($value)) {
            throw new CompilerException(
                sprintf('An escaped char literal (\'%s\') cannot be used in a constant expression', $value),
                $node
            );
        }

        return ord($value);
    }

    /**
     * @throws CompilerException
     */
    private function compiledValue(CompiledExpression $expression, array $node): mixed
    {
        $code = $expression->getCode();

        return match ($expression->getType()) {
            'int', 'uint', 'long', 'ulong' => AbstractOperator::literalToNumber((string) $code),
            'double', 'float'              => (float) $code,
            'string', 'istring', 'char'    => (string) $code,
            'bool', 'boolean'              => 'false' !== $code && false !== $code && '0' !== $code,
            'null'                         => null,
            default                        => throw new CompilerException(
                sprintf(
                    'Constant of type "%s" is not usable in a compile-time constant expression',
                    $expression->getType()
                ),
                $node
            ),
        };
    }

    /**
     * @throws CompilerException
     */
    private function rejectChar(array $node): void
    {
        if ('char' === $node['type']) {
            throw new CompilerException(
                'A char literal cannot be concatenated in a constant expression, '
                . 'because a char is a byte value; use a double-quoted string instead',
                $node
            );
        }
    }

    /**
     * Encodes a PHP value back as a literal node.
     *
     * @throws CompilerException
     */
    private function toNode(mixed $value, array $node): array
    {
        if (is_bool($value)) {
            return ['type' => 'bool', 'value' => $value ? 'true' : 'false'];
        }

        if (is_int($value)) {
            return ['type' => 'int', 'value' => (string) $value];
        }

        if (is_float($value)) {
            if (!is_finite($value)) {
                throw new CompilerException(
                    'Constant expression does not evaluate to a finite number',
                    $node
                );
            }

            // var_export() emits the shortest representation that round-trips,
            // and always keeps a decimal point so C reads it as a double.
            return ['type' => 'double', 'value' => var_export($value, true)];
        }

        if (is_string($value)) {
            return ['type' => 'string', 'value' => $value];
        }

        if (null === $value) {
            return ['type' => 'null'];
        }

        throw new CompilerException(
            'Constant expression does not evaluate to a scalar value',
            $node
        );
    }
}
