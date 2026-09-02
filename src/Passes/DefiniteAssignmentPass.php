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
 * Definite-assignment analysis over the body of a single method.
 *
 * It answers one question, for one kind of symbol: was a local that the user
 * declared without a value (`var x;` rather than `var x = 1;`) ever read at a
 * point where nothing had written to it yet? PHP evaluates such a read as null;
 * Zephir would otherwise hand userland the IS_UNDEF zval the declaration left
 * behind, which var_dump() prints as UNKNOWN:0.
 *
 * The result is consumed by DeclareStatement, which registers the slot with the
 * memory frame at the declaration so every later write downgrades to its
 * conditional-observe form. Doing that needs the answer *before* the body is
 * compiled, which is why this is a pass and not a check inside the emitter.
 *
 * Unlike LocalContextPass this always runs: it decides what the extension
 * returns, not how fast it gets there, so it cannot sit behind an optimization
 * flag.
 *
 * Both directions of imprecision are safe. Reporting a variable that did not
 * need it costs one ZEPHIR_INIT_VAR and a memory frame; missing one leaves
 * today's behaviour. Neither can leak, because the emitter only ever adds a
 * first registration, never removes one.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2679
 */
final class DefiniteAssignmentPass
{
    /**
     * Locals the user declared without a value, by name.
     *
     * @var array<string, true>
     */
    private array $candidates = [];

    /**
     * Candidates read where nothing had assigned them yet, by name.
     *
     * @var array<string, true>
     */
    private array $required = [];

    /**
     * Candidates something writes to somewhere, by name.
     *
     * @var array<string, true>
     */
    private array $written = [];

    public function pass(StatementsBlock $block): void
    {
        $this->walk($block->getStatements(), []);
    }

    /**
     * Whether this local has to be initialised to null at its declaration.
     *
     * A local nothing writes to at all is excluded: #2654 already starts those
     * at IS_NULL in the declaration itself, which is free, while this costs a
     * ZEPHIR_INIT_VAR and a memory frame. IS_NULL is safe there precisely
     * because there is no later write whose registration could be lost.
     *
     * @see Variable::isNeverAssigned()
     */
    public function requiresNullInitialization(string $name): bool
    {
        return isset($this->required[$name], $this->written[$name]);
    }

    /**
     * Records a write of `$name`.
     *
     * @param array<string, true> $assigned
     */
    private function assign(string $name, array &$assigned): void
    {
        $assigned[$name]      = true;
        $this->written[$name] = true;
    }

    /**
     * Records a read of `$name`, and flags it when nothing has assigned it yet.
     *
     * @param array<string, true> $assigned
     */
    private function read(string $name, array $assigned): void
    {
        if (isset($this->candidates[$name]) && !isset($assigned[$name])) {
            $this->required[$name] = true;
        }
    }

    /**
     * Walks an expression for reads.
     *
     * The recursion is deliberately generic - anything shaped like a node is
     * descended into - so a grammar addition cannot silently become an
     * unhandled read. Two node types are not generic:
     *
     *  - `fetch` writes its target on both paths, the value on a hit and
     *    ZVAL_NULL on a miss (kernel/array.c zephir_array_isset_*_fetch,
     *    kernel/object.c zephir_fetch_property*), so the target is an
     *    unconditional write and not a read at all. A read-only fetch also
     *    borrows its value without taking a reference, so registering that slot
     *    with the memory frame would free a reference the method never owned.
     *  - a closure body is a scope of its own, matching LocalContextPass.
     *
     * @param array<string, true> $assigned
     */
    private function readExpression(array $node, array &$assigned): void
    {
        $type = $node['type'] ?? null;

        if ('variable' === $type && isset($node['value']) && is_string($node['value'])) {
            $this->read($node['value'], $assigned);

            return;
        }

        /**
         * A closure body is a scope of its own, so its statements say nothing
         * about this method's locals. Its `use (...)` clause does: the values
         * are captured here, when the closure is created. A by-reference
         * capture shares one slot with the closure, so it is a write as well.
         */
        if ('closure' === $type || 'closure-arrow' === $type) {
            foreach ($node['use'] ?? [] as $capture) {
                if (!isset($capture['name']) || !is_string($capture['name'])) {
                    continue;
                }

                $this->read($capture['name'], $assigned);

                if (!empty($capture['reference'])) {
                    $this->assign($capture['name'], $assigned);
                }
            }

            return;
        }

        /**
         * The member name of a property or constant access is itself a
         * `variable` node - `this->image` carries `right: variable "image"` -
         * so descending into it would report a read of a local that only
         * shares the property's name. Only the object or class on the left is
         * a value. `property-dynamic-access` is not here on purpose: its right
         * really is a variable holding the name.
         */
        if (in_array($type, ['property-access', 'static-property-access', 'static-constant-access'], true)) {
            if (isset($node['left']) && is_array($node['left'])) {
                $this->readExpression($node['left'], $assigned);
            }

            return;
        }

        if ('fetch' === $type) {
            if (isset($node['right']) && is_array($node['right'])) {
                $this->readExpression($node['right'], $assigned);
            }

            if (isset($node['left']['value']) && is_string($node['left']['value'])) {
                $this->assign($node['left']['value'], $assigned);
            }

            return;
        }

        foreach ($node as $key => $value) {
            if ('type' !== $key && is_array($value)) {
                $this->readExpression($value, $assigned);
            }
        }
    }

    /**
     * Walks a block and returns the set of names definitely assigned after it.
     *
     * @param array<string, true> $assigned
     *
     * @return array<string, true>
     */
    private function walk(array $statements, array $assigned): array
    {
        foreach ($statements as $statement) {
            $assigned = match ($statement['type'] ?? '') {
                'declare'    => $this->walkDeclare($statement, $assigned),
                'let'        => $this->walkLet($statement, $assigned),
                'if'         => $this->walkIf($statement, $assigned),
                'switch'     => $this->walkSwitch($statement, $assigned),
                'while',
                'for',
                'loop'       => $this->walkMayNotRun($statement, $assigned),
                'do-while'   => $this->walkDoWhile($statement, $assigned),
                'try-catch'  => $this->walkTryCatch($statement, $assigned),
                default      => $this->walkLeaf($statement, $assigned),
            };
        }

        return $assigned;
    }

    /**
     * @param array<string, true> $assigned
     *
     * @return array<string, true>
     */
    private function walkDeclare(array $statement, array $assigned): array
    {
        foreach ($statement['variables'] ?? [] as $variable) {
            $name = $variable['variable'] ?? null;
            if (!is_string($name)) {
                continue;
            }

            if (isset($variable['expr'])) {
                $this->readExpression($variable['expr'], $assigned);
                $this->assign($name, $assigned);
                continue;
            }

            $this->candidates[$name] = true;
            unset($assigned[$name]);
        }

        return $assigned;
    }

    /**
     * A `do-while` body always runs, so what it assigns survives.
     *
     * @param array<string, true> $assigned
     *
     * @return array<string, true>
     */
    private function walkDoWhile(array $statement, array $assigned): array
    {
        $assigned = $this->walk($statement['statements'] ?? [], $assigned);
        if (isset($statement['expr'])) {
            $this->readExpression($statement['expr'], $assigned);
        }

        return $assigned;
    }

    /**
     * Only what both arms assign survives, and an `if` with no `else` assigns
     * nothing for certain. A `fetch` in the condition is the exception: it
     * writes its target whichever way the condition goes.
     *
     * @param array<string, true> $assigned
     *
     * @return array<string, true>
     */
    private function walkIf(array $statement, array $assigned): array
    {
        if (isset($statement['expr'])) {
            $this->readExpression($statement['expr'], $assigned);
        }

        $then = $this->walk($statement['statements'] ?? [], $assigned);

        if (!isset($statement['else_statements'])) {
            return $assigned;
        }

        return array_intersect_key($then, $this->walk($statement['else_statements'], $assigned));
    }

    /**
     * Any statement that only reads: return, echo, throw, yield, unset, calls,
     * a standalone fetch, and anything the grammar grows later.
     *
     * @param array<string, true> $assigned
     *
     * @return array<string, true>
     */
    private function walkLeaf(array $statement, array $assigned): array
    {
        foreach ($statement as $key => $value) {
            if ('type' !== $key && is_array($value)) {
                $this->readExpression($value, $assigned);
            }
        }

        return $assigned;
    }

    /**
     * @param array<string, true> $assigned
     *
     * @return array<string, true>
     */
    private function walkLet(array $statement, array $assigned): array
    {
        foreach ($statement['assignments'] ?? [] as $assignment) {
            /**
             * Everything an assignment carries is a value being read, except
             * the destructuring slots, which are targets: the assigned
             * expression, an array index (`let x[k] = v;`) and the nested
             * target of `let a->b->c = v;`.
             */
            foreach ($assignment as $key => $value) {
                if ('variables' !== $key && is_array($value)) {
                    $this->readExpression($value, $assigned);
                }
            }

            $assignType = $assignment['assign-type'] ?? '';

            if ('destructure' === $assignType) {
                foreach ($assignment['variables'] ?? [] as $slot) {
                    if (isset($slot['value']) && is_string($slot['value'])) {
                        $this->assign($slot['value'], $assigned);
                    }
                }

                continue;
            }

            $name = $assignment['variable'] ?? null;
            if (!is_string($name)) {
                continue;
            }

            /**
             * `let x = expr;` replaces the whole zval. Everything else either
             * updates through the variable (`let x[] = 1;`, `let x += 1;`,
             * `let x++;`) or targets a property reached through it
             * (`let x->p = 1;`), and both read it first.
             */
            if ('variable' === $assignType && 'assign' === ($assignment['operator'] ?? 'assign')) {
                $this->assign($name, $assigned);
                continue;
            }

            $this->read($name, $assigned);
            $this->assign($name, $assigned);
        }

        return $assigned;
    }

    /**
     * A loop body may run zero times, so nothing it assigns is certain
     * afterwards. Its reads still count, and the loop variables of a `for` are
     * assigned inside it.
     *
     * @param array<string, true> $assigned
     *
     * @return array<string, true>
     */
    private function walkMayNotRun(array $statement, array $assigned): array
    {
        if (isset($statement['expr'])) {
            $this->readExpression($statement['expr'], $assigned);
        }

        $body = $assigned;
        foreach (['key', 'value'] as $slot) {
            if (isset($statement[$slot]) && is_string($statement[$slot])) {
                $this->assign($statement[$slot], $body);
            }
        }

        $this->walk($statement['statements'] ?? [], $body);

        return $assigned;
    }

    /**
     * Only a switch with a `default` clause covers every value, and only then
     * can what its clauses assign be counted on.
     *
     * A clause that does not end in `break`, `return`, `throw` or `continue`
     * falls through into the next one exactly as it does in PHP, so state is
     * threaded from clause to clause and collected only where control actually
     * leaves the switch. Treating each clause as its own exit would call
     * `case 1: case 2: let x = 1; break;` a gap when it is not one.
     *
     * @param array<string, true> $assigned
     *
     * @return array<string, true>
     */
    private function walkSwitch(array $statement, array $assigned): array
    {
        if (isset($statement['expr'])) {
            $this->readExpression($statement['expr'], $assigned);
        }

        $hasDefault = false;
        $inClause   = false;
        $state      = $assigned;
        $exits      = [];

        foreach ($statement['clauses'] ?? [] as $clause) {
            if (isset($clause['expr'])) {
                $this->readExpression($clause['expr'], $assigned);
            }

            if ('default' === ($clause['type'] ?? '')) {
                $hasDefault = true;
            }

            $statements = $clause['statements'] ?? [];
            $state      = $this->walk($statements, $state);
            $inClause   = true;

            if ($this->leavesTheBlock($statements)) {
                $exits[]  = $state;
                $state    = $assigned;
                $inClause = false;
            }
        }

        /* Running off the end of the last clause is an exit of its own. */
        if ($inClause) {
            $exits[] = $state;
        }

        if (!$hasDefault || [] === $exits) {
            return $assigned;
        }

        $common = array_shift($exits);
        foreach ($exits as $exit) {
            $common = array_intersect_key($common, $exit);
        }

        return $common;
    }

    /**
     * Whether a clause body ends by leaving the switch rather than falling
     * through into the next clause.
     */
    private function leavesTheBlock(array $statements): bool
    {
        $last = end($statements);

        return is_array($last)
            && in_array($last['type'] ?? '', ['break', 'return', 'throw', 'continue'], true);
    }

    /**
     * Control reaches the statement after a try/catch two ways: the try block
     * ran to the end, or a catch ran to the end. So this is the same
     * intersection an if/else takes, over the try state and every catch that
     * completes normally. A catch that always throws or returns is not one of
     * those ways out and contributes nothing.
     *
     * A catch body itself starts from the state before the try, because the
     * exception may have been raised before any of the try's assignments.
     *
     * @param array<string, true> $assigned
     *
     * @return array<string, true>
     */
    private function walkTryCatch(array $statement, array $assigned): array
    {
        $exits = [$this->walk($statement['statements'] ?? [], $assigned)];

        foreach ($statement['catches'] ?? [] as $catch) {
            $body = $assigned;
            if (isset($catch['variable']['value']) && is_string($catch['variable']['value'])) {
                $this->assign($catch['variable']['value'], $body);
            }

            $statements = $catch['statements'] ?? [];
            $after      = $this->walk($statements, $body);

            if (!$this->leavesTheBlock($statements)) {
                $exits[] = $after;
            }
        }

        $common = array_shift($exits);
        foreach ($exits as $exit) {
            $common = array_intersect_key($common, $exit);
        }

        return $common;
    }
}
