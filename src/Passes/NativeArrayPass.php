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

namespace Zephir\Passes;

use Zephir\StatementsBlock;

use function in_array;
use function is_array;
use function is_string;

/**
 * Which locals of a single method can only ever hold a native array.
 *
 * A read-only subscript read hands the target a pointer the container owns:
 * the kernel skips the addref (ZVAL_COPY_VALUE) and the emitter neither
 * observes the target nor registers it with the memory frame. That bargain
 * only works if something else keeps the value alive, which a hash table does
 * and an ArrayAccess object does not: offsetGet() synthesises the value and
 * owns nothing once it has returned. Borrowing from one used to leak the
 * allocation, or free it under a target still pointing at it.
 *
 * The declared type cannot answer this. `array` is a compile-time label, and
 * `array a; let a = someVar;` copies the source zval unchanged
 * (Zephir\Statements\Let\Variable::doArrayAssignmentProcess), so an `array`
 * local can hold an object. It also answers too little: the containers worth
 * optimizing are `var`s assigned from array literals.
 *
 * So the proof is about writes, not declarations. A name is proven when
 * something can only have put an array in it and nothing untracked can have
 * written it since. The analysis is deliberately flow insensitive: it looks at
 * every write in the method whatever the order, so a loop that reassigns the
 * container after the read cannot slip past.
 *
 * Unlike LocalContextPass this always runs, generator steps included: it
 * decides whether the extension frees a value it owns, not how fast it gets
 * there, so it cannot sit behind an optimization flag.
 *
 * Imprecision is safe in one direction only, and the code is arranged so that
 * is the direction it falls: **anything not recognised taints**. Missing a
 * proof costs one refcount pair and a memory frame slot. Granting one wrongly
 * corrupts memory. When extending this, add to the taint side first.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2682
 */
final class NativeArrayPass
{
    /**
     * Assign types that write the variable itself with an arbitrary value.
     * Everything not listed here and not in UPDATES_THROUGH taints anyway.
     */
    private const REPLACES = ['variable', 'incr', 'decr'];

    /**
     * Assign types that reach through the variable instead of replacing it.
     *
     * An array update (`let x[] = 1;`) neither proves nor taints: on a `var`
     * holding null the kernel creates an array, on an object it calls
     * offsetSet(). A property or static-property assignment does not write
     * this local at all.
     *
     * @see Zephir\Statements\LetStatement::compile()
     */
    private const UPDATES_THROUGH = [
        'array-index',
        'array-index-append',
        'dynamic-variable-string',
        'object-property',
        'static-property',
        'static-property-append',
        'static-property-array-index',
        'static-property-array-index-append',
        'string-dynamic-object-property',
        'variable-append',
        'variable-dynamic-object-property',
    ];

    /**
     * Names something wrote an array to, by name.
     *
     * @var array<string, true>
     */
    private array $proven = [];

    /**
     * Names something may have written a non-array to, by name.
     *
     * @var array<string, true>
     */
    private array $tainted = [];

    /**
     * Whether every value this local can hold is a native array.
     *
     * A defaulted `array p = null` parameter is reported as proven and can be
     * null. That is deliberate and harmless: the borrow only goes wrong for an
     * object container, because that is the only one that builds a value and
     * then owns nothing. A null or scalar container yields ZVAL_NULL and a
     * string container yields an interned one-character string, neither of
     * which is refcounted.
     */
    public function isProvenNativeArray(string $name): bool
    {
        return isset($this->proven[$name]) && !isset($this->tainted[$name]);
    }

    public function pass(StatementsBlock $block): void
    {
        $this->walk($block->getStatements());
    }

    /**
     * PHP enforces an `array` parameter through its arg-info, and the prologue
     * runs zephir_get_arrval() on it, so it cannot arrive as anything else. A
     * by-reference parameter is excluded: the slot is shared with the caller.
     *
     * Every other parameter is a write of an unknown value, so it taints.
     *
     * @param array<int, array> $parameters
     */
    public function passParameters(array $parameters): void
    {
        foreach ($parameters as $parameter) {
            $name = $parameter['name'] ?? null;
            if (!is_string($name)) {
                continue;
            }

            if ('array' === ($parameter['data-type'] ?? null) && empty($parameter['reference'])) {
                $this->proven[$name] = true;
                continue;
            }

            $this->tainted[$name] = true;
        }
    }

    /**
     * Whether this expression can only evaluate to a native array.
     */
    private function isArrayExpression(array $expr): bool
    {
        return match ($expr['type'] ?? null) {
            'array', 'empty-array' => true,
            'cast'                 => 'array' === ($expr['left'] ?? null),
            // A parenthesised expression: `let a = ([1, 2]);`
            'list'                 => is_array($expr['left'] ?? null) && $this->isArrayExpression($expr['left']),
            default                => false,
        };
    }

    private function proven(string $name): void
    {
        $this->proven[$name] = true;
    }

    private function taint(?string $name): void
    {
        if (is_string($name)) {
            $this->tainted[$name] = true;
        }
    }

    /**
     * Records the effect of writing `$expr` over the whole of `$name`.
     */
    private function write(string $name, ?array $expr): void
    {
        if (null !== $expr && $this->isArrayExpression($expr)) {
            $this->proven($name);

            return;
        }

        $this->taint($name);
    }

    /**
     * @param array<int, array> $statements
     */
    private function walk(array $statements): void
    {
        foreach ($statements as $statement) {
            if (is_array($statement)) {
                $this->walkNode($statement);
            }
        }
    }

    /**
     * `try { } catch \Exception, e { }` writes `e` with the thrown object.
     */
    private function walkCatches(array $node): void
    {
        foreach ($node['catches'] ?? [] as $catch) {
            $this->taint($catch['variable']['value'] ?? null);
        }
    }

    /**
     * A closure body is a scope of its own and gets its own pass, so only the
     * capture list says anything about this method's locals. A by-reference
     * capture shares one slot with the closure, which can write anything into
     * it; a by-value capture cannot, but an arrow closure's list is implicit,
     * so both are treated the same way. The body is not descended into.
     */
    private function walkClosure(array $node): void
    {
        foreach ($node['use'] ?? [] as $capture) {
            $this->taint($capture['name'] ?? null);
        }
    }

    private function walkDeclare(array $node): void
    {
        $isArray = 'array' === ($node['data-type'] ?? null);

        foreach ($node['variables'] ?? [] as $variable) {
            $name = $variable['variable'] ?? null;
            if (!is_string($name)) {
                continue;
            }

            /**
             * A declared `array` is proof on its own, but only because every
             * way of putting something else in it taints: an assignment from
             * anything but an array literal, a fetch, a loop slot, a catch, a
             * by-reference capture or a call that can write back.
             */
            if ($isArray) {
                $this->proven($name);
            }

            if (isset($variable['expr']) && is_array($variable['expr'])) {
                $this->write($name, $variable['expr']);
                $this->walkNode($variable['expr']);
            }
        }
    }

    /**
     * `fetch v, c["k"]` writes `v` on both paths, the value on a hit and
     * ZVAL_NULL on a miss.
     */
    private function walkFetch(array $node): void
    {
        $this->taint($node['left']['value'] ?? null);
    }

    /**
     * The key and value slots of `for k, v in ...` are plain strings, and each
     * iteration writes whatever the source holds into them.
     */
    private function walkFor(array $node): void
    {
        foreach (['key', 'value'] as $slot) {
            if (isset($node[$slot]) && is_string($node[$slot])) {
                $this->taint($node[$slot]);
            }
        }
    }

    private function walkLet(array $node): void
    {
        foreach ($node['assignments'] ?? [] as $assignment) {
            if (isset($assignment['expr']) && is_array($assignment['expr'])) {
                $this->walkNode($assignment['expr']);
            }

            foreach ($assignment['index-expr'] ?? [] as $index) {
                if (is_array($index)) {
                    $this->walkNode($index);
                }
            }

            $assignType = $assignment['assign-type'] ?? '';

            if ('destructure' === $assignType) {
                foreach ($assignment['variables'] ?? [] as $slot) {
                    $this->taint($slot['value'] ?? null);
                }

                continue;
            }

            if (in_array($assignType, self::UPDATES_THROUGH, true)) {
                continue;
            }

            $name = $assignment['variable'] ?? null;
            if (!is_string($name)) {
                continue;
            }

            /**
             * `let x = expr;` is the only write that can prove anything.
             * A compound operator (`let x += 1;`), an increment and any assign
             * type the grammar grows later all land on the taint side.
             */
            if (
                'variable' === $assignType
                && 'assign' === ($assignment['operator'] ?? 'assign')
                && isset($assignment['expr'])
                && is_array($assignment['expr'])
            ) {
                $this->write($name, $assignment['expr']);

                continue;
            }

            $this->taint($name);
        }
    }

    /**
     * Statements and expressions share the `type` key, so one walker covers
     * both, and a list of either is an array of nodes that falls through to
     * the generic descent below.
     */
    private function walkNode(array $node): void
    {
        switch ($node['type'] ?? null) {
            case 'declare':
                $this->walkDeclare($node);

                return;

            case 'let':
                $this->walkLet($node);

                return;

            case 'closure':
            case 'closure-arrow':
                $this->walkClosure($node);

                return;

            case 'fetch':
                $this->walkFetch($node);
                break;

            case 'for':
                $this->walkFor($node);
                break;

            case 'try-catch':
                $this->walkCatches($node);
                break;

            case 'fcall':
            case 'mcall':
            case 'scall':
                $this->walkParameters($node);
                break;

            case 'unset':
                $this->walkUnset($node);
                break;
        }

        foreach ($node as $key => $value) {
            if ('type' !== $key && is_array($value)) {
                $this->walkNode($value);
            }
        }
    }

    /**
     * A call argument reaches the callee as the caller's own zval, so a
     * by-reference parameter writes straight into this local. Which parameters
     * are by reference is a property of the callee, unknown here and unknown
     * for a dynamic call at all, so every variable handed to a call taints.
     */
    private function walkParameters(array $node): void
    {
        foreach ($node['parameters'] ?? [] as $parameter) {
            if ('variable' === ($parameter['parameter']['type'] ?? null)) {
                $this->taint($parameter['parameter']['value'] ?? null);
            }
        }
    }

    /**
     * `unset(x)` leaves the slot empty; `unset(x["k"])` only removes a key and
     * says nothing about `x`.
     */
    private function walkUnset(array $node): void
    {
        $target = $node['expr'] ?? null;
        while (is_array($target) && 'list' === ($target['type'] ?? null)) {
            $target = $target['left'] ?? null;
        }

        if (is_array($target) && 'variable' === ($target['type'] ?? null)) {
            $this->taint($target['value'] ?? null);
        }
    }
}
