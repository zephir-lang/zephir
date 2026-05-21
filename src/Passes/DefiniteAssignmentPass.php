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

use function array_intersect_key;
use function array_fill_keys;
use function in_array;
use function is_array;

/**
 * Walks the statements of a method body and emits a warning when a `var`-typed
 * local is read on a path that did not assign it. Models definite-assignment
 * analysis with branch merging: a variable is "assigned" after an `if/else`
 * only if it was assigned in BOTH the true and false statements.
 *
 * Conservative by design: false negatives over false positives. Switch is
 * treated as a no-op merge (no per-clause assignment is tracked into the
 * after-state), since fallthrough and a missing `default` make the merge
 * unsafe in general.
 *
 * Pairs with `Backend::generateInitCode` auto-nulling user `var` locals at
 * the prologue (issue #1875). The auto-null makes the read runtime-safe under
 * PHP semantics; this pass is the hygiene signal that tells the author the
 * read is relying on the implicit null.
 */
class DefiniteAssignmentPass
{
    /**
     * Variable names known to be definitely assigned at the current point of
     * the walk. Keys only; values always true.
     */
    private array $assigned = [];

    /**
     * Collected diagnostics. Each entry is ['name' => string, 'node' => array].
     */
    private array $warnings = [];

    /**
     * Variable names declared via `var`/`array`/etc. in this method. Used to
     * scope the warning: we only flag declared `var`-typed locals (the case
     * issue #1875 cares about), not parameters, this, return_value, or names
     * that aren't even in this function's symbol table.
     */
    private array $declared = [];

    /**
     * Tracks which declared variables had an explicit initializer at their
     * `var X = value;` declaration, so subsequent reads are safe.
     */
    private array $declaredInitialized = [];

    public function __construct(array $parameterNames = [])
    {
        $this->assigned = array_fill_keys($parameterNames, true);
    }

    public function pass(StatementsBlock $block): void
    {
        $this->passStatementBlock($block->getStatements());
    }

    /**
     * @return array<int, array{name: string, node: array}>
     */
    public function getWarnings(): array
    {
        return $this->warnings;
    }

    private function passStatementBlock(array $statements): void
    {
        foreach ($statements as $statement) {
            $this->passStatement($statement);
        }
    }

    private function passStatement(array $statement): void
    {
        switch ($statement['type']) {
            case 'declare':
                $this->declareVariables($statement);
                break;

            case 'let':
                if (isset($statement['assignments'])) {
                    foreach ($statement['assignments'] as $assignment) {
                        if (isset($assignment['expr'])) {
                            $this->passExpression($assignment['expr']);
                        }
                        if (isset($assignment['variable'])) {
                            // `variable` is the LHS root: `let x = ...` and
                            // `let x[k] = ...` both mark x assigned. Compound
                            // ops on undefined targets are also caught by the
                            // RHS expression walk above (x referenced as RHS).
                            $this->assigned[$assignment['variable']] = true;
                        }
                    }
                }
                break;

            case 'if':
                if (isset($statement['expr'])) {
                    $this->passExpression($statement['expr']);
                }

                $before = $this->assigned;

                if (isset($statement['statements'])) {
                    $this->passStatementBlock($statement['statements']);
                }
                $afterTrue = $this->assigned;

                $this->assigned = $before;
                if (isset($statement['else_statements'])) {
                    $this->passStatementBlock($statement['else_statements']);
                    $afterFalse = $this->assigned;
                    // Variable is "assigned after if/else" only if both
                    // branches assigned it.
                    $this->assigned = array_intersect_key($afterTrue, $afterFalse);
                } else {
                    // No else: false branch leaves the set unchanged.
                    // The merged after-state is the intersection of the
                    // true-branch after-state and the before-state.
                    $this->assigned = array_intersect_key($afterTrue, $before);
                }
                break;

            case 'while':
            case 'do-while':
                if (isset($statement['expr'])) {
                    $this->passExpression($statement['expr']);
                }
                $before = $this->assigned;
                if (isset($statement['statements'])) {
                    $this->passStatementBlock($statement['statements']);
                }
                // Loop body may run zero times; do-while is treated the same
                // for simplicity. Reset to pre-loop state.
                $this->assigned = $before;
                break;

            case 'for':
                if (isset($statement['expr'])) {
                    $this->passExpression($statement['expr']);
                }
                $before = $this->assigned;
                // Loop key/value are bound inside the body if it runs; mark
                // them assigned within but reset after.
                if (isset($statement['key'])) {
                    $this->assigned[$statement['key']] = true;
                }
                if (isset($statement['value'])) {
                    $this->assigned[$statement['value']] = true;
                }
                if (isset($statement['statements'])) {
                    $this->passStatementBlock($statement['statements']);
                }
                $this->assigned = $before;
                break;

            case 'loop':
                $before = $this->assigned;
                if (isset($statement['statements'])) {
                    $this->passStatementBlock($statement['statements']);
                }
                $this->assigned = $before;
                break;

            case 'switch':
                if (isset($statement['expr'])) {
                    $this->passExpression($statement['expr']);
                }
                $before = $this->assigned;
                if (isset($statement['clauses'])) {
                    foreach ($statement['clauses'] as $clause) {
                        $this->assigned = $before;
                        if (isset($clause['expr'])) {
                            $this->passExpression($clause['expr']);
                        }
                        if (isset($clause['statements'])) {
                            $this->passStatementBlock($clause['statements']);
                        }
                    }
                }
                // Switch fallthrough + missing default make a safe merge hard.
                // Reset to before-state: conservative.
                $this->assigned = $before;
                break;

            case 'try-catch':
                $before = $this->assigned;
                if (isset($statement['statements'])) {
                    $this->passStatementBlock($statement['statements']);
                }
                $afterTry = $this->assigned;

                if (isset($statement['catches'])) {
                    foreach ($statement['catches'] as $catch) {
                        $this->assigned = $before;
                        if (isset($catch['variable']['value'])) {
                            $this->assigned[$catch['variable']['value']] = true;
                        }
                        if (isset($catch['statements'])) {
                            $this->passStatementBlock($catch['statements']);
                        }
                    }
                }
                // Try may throw at any point; conservatively merge to before.
                $this->assigned = array_intersect_key($afterTry, $before);
                break;

            case 'return':
            case 'throw':
                if (isset($statement['expr'])) {
                    $this->passExpression($statement['expr']);
                }
                break;

            case 'yield':
                if (isset($statement['expr'])) {
                    $this->passExpression($statement['expr']);
                }
                if (isset($statement['key'])) {
                    $this->passExpression($statement['key']);
                }
                if (isset($statement['value'])) {
                    $this->passExpression($statement['value']);
                }
                break;

            case 'echo':
                if (isset($statement['expressions'])) {
                    foreach ($statement['expressions'] as $expr) {
                        $this->passExpression($expr);
                    }
                }
                break;

            case 'fetch':
                if (isset($statement['expr'])) {
                    $this->passExpression($statement['expr']);
                    // `fetch x, expr[k]` assigns x: extract the target.
                    if (isset($statement['expr']['left']['value'])) {
                        $this->assigned[$statement['expr']['left']['value']] = true;
                    }
                }
                break;

            case 'unset':
                // unset() doesn't read in a way that creates an undef-read
                // risk here; the assigned state of the variable doesn't
                // change in a way this pass needs to track.
                break;

            case 'mcall':
            case 'fcall':
            case 'scall':
            case 'require':
            case 'require_once':
                if (isset($statement['expr'])) {
                    $this->passExpression($statement['expr']);
                }
                break;

            case 'break':
            case 'continue':
            case 'comment':
            case 'cblock':
            case 'empty':
            default:
                break;
        }
    }

    private function declareVariables(array $statement): void
    {
        if (!isset($statement['variables']) || !is_array($statement['variables'])) {
            return;
        }
        $dataType = $statement['data-type'] ?? 'variable';
        foreach ($statement['variables'] as $declared) {
            $name = $declared['variable'] ?? null;
            if ($name === null) {
                continue;
            }
            // Only track `var`-typed locals for the warning. Typed locals
            // (int/double/bool/string/array) have their own runtime semantics
            // and either get auto-default values from the compiler or fall
            // back to a C-level default — out of scope for this pass.
            if ($dataType !== 'variable') {
                continue;
            }
            $this->declared[$name] = true;

            if (isset($declared['expr'])) {
                $this->passExpression($declared['expr']);
                $this->assigned[$name] = true;
                $this->declaredInitialized[$name] = true;
            }
        }
    }

    private function passExpression(array $expression): void
    {
        if (!isset($expression['type'])) {
            return;
        }

        switch ($expression['type']) {
            case 'variable':
                $name = $expression['value'] ?? null;
                if ($name === null) {
                    break;
                }
                // Only warn for variables declared as `var` in this method.
                // Skip this/return_value, parameters (already in $assigned),
                // and any name we don't have a declaration record for.
                if (in_array($name, ['this', 'return_value', 'return_value_ptr'], true)) {
                    break;
                }
                if (!isset($this->declared[$name])) {
                    break;
                }
                if (isset($this->assigned[$name])) {
                    break;
                }
                $this->warnings[] = [
                    'name' => $name,
                    'node' => $expression,
                ];
                break;

            case 'bool':
            case 'double':
            case 'int':
            case 'uint':
            case 'long':
            case 'ulong':
            case 'string':
            case 'istring':
            case 'null':
            case 'char':
            case 'uchar':
            case 'empty-array':
            case 'constant':
            case 'static-constant-access':
            case 'closure':
            case 'closure-arrow':
                break;

            case 'sub':
            case 'add':
            case 'div':
            case 'mul':
            case 'mod':
            case 'and':
            case 'or':
            case 'concat':
            case 'equals':
            case 'identical':
            case 'not-identical':
            case 'not-equals':
            case 'less':
            case 'greater':
            case 'greater-equal':
            case 'less-equal':
            case 'bitwise_and':
            case 'bitwise_or':
            case 'bitwise_xor':
            case 'bitwise_shiftleft':
            case 'bitwise_shiftright':
            case 'irange':
            case 'erange':
                if (isset($expression['left'])) {
                    $this->passExpression($expression['left']);
                }
                if (isset($expression['right'])) {
                    $this->passExpression($expression['right']);
                }
                break;

            case 'typeof':
            case 'bitwise_not':
            case 'short-ternary':
            case 'ternary':
            case 'unlikely':
            case 'likely':
            case 'clone':
            case 'require_once':
            case 'list':
            case 'minus':
            case 'require':
            case 'instanceof':
            case 'empty':
            case 'isset':
            case 'not':
                if (isset($expression['left'])) {
                    $this->passExpression($expression['left']);
                }
                if (isset($expression['right'])) {
                    $this->passExpression($expression['right']);
                }
                if (isset($expression['extra'])) {
                    $this->passExpression($expression['extra']);
                }
                break;

            case 'array-access':
                // arr[k]: both sides are real expressions.
                if (isset($expression['left'])) {
                    $this->passExpression($expression['left']);
                }
                if (isset($expression['right'])) {
                    $this->passExpression($expression['right']);
                }
                break;

            case 'property-access':
            case 'property-string-access':
            case 'property-dynamic-access':
                // obj->prop: `right` is the property NAME (encoded as a
                // {type: variable, value: <propName>} AST node), not a local
                // variable read. Walk only `left` (the receiver).
                if (isset($expression['left'])) {
                    $this->passExpression($expression['left']);
                }
                break;

            case 'mcall':
            case 'fcall':
            case 'scall':
                if (isset($expression['variable']) && is_array($expression['variable'])) {
                    $this->passExpression($expression['variable']);
                }
                if (isset($expression['parameters'])) {
                    foreach ($expression['parameters'] as $parameter) {
                        if (isset($parameter['parameter'])) {
                            $this->passExpression($parameter['parameter']);
                        }
                    }
                }
                break;

            case 'array':
                if (isset($expression['left'])) {
                    foreach ($expression['left'] as $item) {
                        if (isset($item['value'])) {
                            $this->passExpression($item['value']);
                        }
                        if (isset($item['key']) && is_array($item['key'])) {
                            $this->passExpression($item['key']);
                        }
                    }
                }
                break;

            case 'new':
            case 'new-type':
                if (isset($expression['parameters'])) {
                    foreach ($expression['parameters'] as $parameter) {
                        if (isset($parameter['parameter'])) {
                            $this->passExpression($parameter['parameter']);
                        }
                    }
                }
                break;

            case 'fetch':
                // fetch as expression (e.g. `if fetch x, arr[k]`): walk the
                // source on the right, and mark the target on the left as
                // assigned. Zephir's fetch binds the target either way (null
                // if the key is missing, the value otherwise); the bool
                // return is what gates the if-condition.
                if (isset($expression['right'])) {
                    $this->passExpression($expression['right']);
                }
                if (isset($expression['left']['value'])) {
                    $this->assigned[$expression['left']['value']] = true;
                }
                break;

            case 'cast':
            case 'type-hint':
                if (isset($expression['right'])) {
                    $this->passExpression($expression['right']);
                }
                break;

            case 'static-property-access':
            case 'reference':
            default:
                // Conservative no-op: missing branches just mean a few reads
                // aren't tracked, which is consistent with the "false
                // negatives over false positives" stance.
                break;
        }
    }
}
