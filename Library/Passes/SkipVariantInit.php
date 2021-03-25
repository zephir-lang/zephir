<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Passes;

use Zephir\StatementsBlock;

/**
 * SkipVariantInit.
 *
 * In 'if'/'else' statements sometimes dynamical variables are initialized in every branch
 * Same case in 'switch' statements
 */
class SkipVariantInit
{
    protected $branches = [];

    protected $variablesToSkip = [];

    protected $ignoredVariables;

    /**
     * Do the compilation pass.
     *
     * @param int             $branchNumber
     * @param StatementsBlock $block
     */
    public function pass($branchNumber, StatementsBlock $block)
    {
        $this->passStatementBlock($branchNumber, $block->getStatements());
        $this->branches[$branchNumber] = 0;
    }

    /**
     * Check assignment types for possible skip.
     *
     * @param int   $branchNumber
     * @param array $statement
     */
    public function passLetStatement($branchNumber, $statement)
    {
        foreach ($statement['assignments'] as $assignment) {
            if ('variable' == $assignment['assign-type']) {
                if ('assign' == $assignment['operator']) {
                    switch ($assignment['expr']['type']) {
                        case 'variable':
                        case 'array-access':
                        case 'property-access':
                        case 'static-property-access':
                        case 'fcall':
                        case 'mcall':
                        case 'scall':
                            break;
                        default:
                            $this->variablesToSkip[$branchNumber][$assignment['variable']] = 1;
                            break;
                    }
                }
            }
        }
    }

    public function passStatementBlock($branchNumber, array $statements)
    {
        foreach ($statements as $statement) {
            if (!isset($statement['type'])) {
                continue;
            }

            switch ($statement['type']) {
                case 'let':
                    $this->passLetStatement($branchNumber, $statement);
                    break;
            }
        }
    }

    /**
     * Returns a list of variables that are initialized in every analyzed branch.
     *
     * @return array
     */
    public function getVariables()
    {
        $variableStats = [];

        foreach ($this->variablesToSkip as $branchNumber => $variables) {
            foreach ($variables as $variable => $one) {
                if (!isset($variableStats[$variable])) {
                    $variableStats[$variable] = 1;
                } else {
                    ++$variableStats[$variable];
                }
            }
        }

        $variables = [];
        $numberBranches = \count($this->branches);
        foreach ($variableStats as $variable => $number) {
            if ($number == $numberBranches) {
                $variables[] = $variable;
            }
        }

        return $variables;
    }

    /**
     * @param int   $branchNumber
     * @param array $variablesToSkip
     */
    public function setVariablesToSkip($branchNumber, $variablesToSkip)
    {
        $this->variablesToSkip[$branchNumber] = $variablesToSkip;
    }
}
