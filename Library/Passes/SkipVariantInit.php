<?php

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

namespace Zephir\Passes;

use Zephir\StatementsBlock;

/**
 * SkipVariantInit
 *
 * In 'if'/'else' statements sometimes dynamical variables are initialized in every branch
 * Same case in 'switch' statements
 */
class SkipVariantInit
{

    protected $branches = array();

    protected $variablesToSkip = array();

    protected $ignoredVariables;

    /**
     * Do the compilation pass
     *
     * @param int $branchNumber
     * @param StatementsBlock $block
     */
    public function pass($branchNumber, StatementsBlock $block)
    {
        $this->passStatementBlock($branchNumber, $block->getStatements());
        $this->branches[$branchNumber] = 0;
    }

    /**
     * Check assignment types for possible skip
     *
     * @param int $branchNumber
     * @param array $statement
     */
    public function passLetStatement($branchNumber, $statement)
    {
        foreach ($statement['assignments'] as $assignment) {
            if ($assignment['assign-type'] == 'variable') {
                if ($assignment['operator'] == 'assign') {
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
     * Returns a list of variables that are initialized in every analyzed branch
     *
     * @return array
     */
    public function getVariables()
    {
        $variableStats = array();

        foreach ($this->variablesToSkip as $branchNumber => $variables) {
            foreach ($variables as $variable => $one) {
                if (!isset($variableStats[$variable])) {
                    $variableStats[$variable] = 1;
                } else {
                    $variableStats[$variable]++;
                }
            }
        }

        $variables = array();
        $numberBranches = count($this->branches);
        foreach ($variableStats as $variable => $number) {
            if ($number == $numberBranches) {
                $variables[] = $variable;
            }
        }
        return $variables;
    }

    /**
     * @param int $branchNumber
     * @param array $variablesToSkip
     */
    public function setVariablesToSkip($branchNumber, $variablesToSkip)
    {
        $this->variablesToSkip[$branchNumber] = $variablesToSkip;
    }
}
