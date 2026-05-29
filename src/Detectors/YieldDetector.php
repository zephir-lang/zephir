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

namespace Zephir\Detectors;

/**
 * Walks a statements tree to determine whether a method body contains any
 * `yield` statement. A method that contains a yield must compile to a PHP
 * generator at the engine level; Zephir tracks the answer here so callers
 * (`Method::isGenerator()`, future codegen) can dispatch on it before the
 * statement-level handler ever runs.
 *
 * See https://github.com/zephir-lang/zephir/issues/1849 for the broader
 * yield-support roadmap. The detector is intentionally narrow: it answers
 * a yes/no question against the AST, nothing more.
 */
final class YieldDetector
{
    private bool $found = false;

    /**
     * Returns true if any yield statement appears anywhere inside the given
     * statements tree (including nested blocks: if/else, loops, switch, try).
     * Yield inside a nested closure does NOT count: a closure body is a
     * separate function whose generator-ness is its own concern.
     *
     * @param array<int, array<string, mixed>> $statements
     */
    public function detect(array $statements): bool
    {
        $this->found = false;
        $this->walk($statements);

        return $this->found;
    }

    /**
     * @param array<int, array<string, mixed>> $statements
     */
    private function walk(array $statements): void
    {
        foreach ($statements as $statement) {
            if ($this->found) {
                return;
            }

            $type = $statement['type'] ?? null;

            if ($type === 'yield') {
                $this->found = true;

                return;
            }

            if (isset($statement['statements']) && is_array($statement['statements'])) {
                $this->walk($statement['statements']);
            }

            if (isset($statement['else_statements']) && is_array($statement['else_statements'])) {
                $this->walk($statement['else_statements']);
            }

            if (isset($statement['elseif_statements']) && is_array($statement['elseif_statements'])) {
                foreach ($statement['elseif_statements'] as $clause) {
                    if (isset($clause['statements']) && is_array($clause['statements'])) {
                        $this->walk($clause['statements']);
                    }
                }
            }

            if (isset($statement['clauses']) && is_array($statement['clauses'])) {
                foreach ($statement['clauses'] as $clause) {
                    if (isset($clause['statements']) && is_array($clause['statements'])) {
                        $this->walk($clause['statements']);
                    }
                }
            }

            if (isset($statement['catches']) && is_array($statement['catches'])) {
                foreach ($statement['catches'] as $catch) {
                    if (isset($catch['statements']) && is_array($catch['statements'])) {
                        $this->walk($catch['statements']);
                    }
                }
            }
        }
    }
}
