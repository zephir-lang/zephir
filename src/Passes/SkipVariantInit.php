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

use function count;
use function in_array;

/**
 * In 'if'/'else' statements sometimes dynamical variables are initialized in every branch
 * Same case in 'switch' statements
 */
class SkipVariantInit
{
    protected array $branches = [];

    protected array $variablesToSkip = [];

    /**
     * Returns a list of variables that are initialized in every analyzed branch.
     */
    public function getVariables(): array
    {
        $variableStats = [];

        foreach ($this->variablesToSkip as $variables) {
            foreach ($variables as $variable => $one) {
                if (!isset($variableStats[$variable])) {
                    $variableStats[$variable] = 1;
                } else {
                    ++$variableStats[$variable];
                }
            }
        }

        $variables      = [];
        $numberBranches = count($this->branches);
        foreach ($variableStats as $variable => $number) {
            if ($number == $numberBranches) {
                $variables[] = $variable;
            }
        }

        return $variables;
    }

    /**
     * Do the compilation pass.
     */
    public function pass(int $branchNumber, StatementsBlock $block): void
    {
        $this->passStatementBlock($branchNumber, $block->getStatements());
        $this->branches[$branchNumber] = 0;
    }

    /**
     * Check assignment types for possible skip.
     */
    public function passLetStatement(int $branchNumber, array $statement): void
    {
        $skipTypes = [
            'variable',
            'array-access',
            'property-access',
            'static-property-access',
            'fcall',
            'mcall',
            'scall',
        ];

        foreach ($statement['assignments'] as $assignment) {
            if ($assignment['assign-type'] !== 'variable' || $assignment['operator'] !== 'assign') {
                continue;
            }

            if (in_array($assignment['expr']['type'], $skipTypes, true)) {
                continue;
            }

            $this->variablesToSkip[$branchNumber][$assignment['variable']] = 1;
        }
    }

    public function passStatementBlock($branchNumber, array $statements): void
    {
        foreach ($statements as $statement) {
            if (!isset($statement['type'])) {
                continue;
            }

            if ($statement['type'] === 'let') {
                $this->passLetStatement($branchNumber, $statement);
            }
        }
    }

    public function setVariablesToSkip(int $branchNumber, array $variablesToSkip): void
    {
        $this->variablesToSkip[$branchNumber] = $variablesToSkip;
    }
}
