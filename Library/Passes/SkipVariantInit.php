<?php

/*
 +----------------------------------------------------------------------+
 | Zephir Language                                                      |
 +----------------------------------------------------------------------+
 | Copyright (c) 2013 Zephir Team                                       |
 +----------------------------------------------------------------------+
 | This source file is subject to version 1.0 of the Zephir license,    |
 | that is bundled with this package in the file LICENSE, and is        |
 | available through the world-wide-web at the following url:           |
 | http://www.zephir-lang.com/license                                   |
 |                                                                      |
 | If you did not receive a copy of the Zephir license and are unable   |
 | to obtain it through the world-wide-web, please send a note to       |
 | license@zephir-lang.com so we can mail you a copy immediately.       |
 +----------------------------------------------------------------------+
*/

/**
 * SkipVariantInit
 *
 * In 'if'/'else' statements somtimes dynamical variables are initialized in every branch
 * Same case in
 */
class SkipVariantInit
{

	protected $_branches = array();

	protected $_variablesToSkip = array();

	/**
	 * Do the compilation pass
	 *
	 * @param int $branchNumber
	 * @param StatementsBlock $block
	 */
	public function pass($branchNumber, StatementsBlock $block)
	{
		$this->passStatementBlock($branchNumber, $block->getStatements());
		$this->_branches[$branchNumber] = 0;
	}

	public function passLetStatement($branchNumber, $statement)
	{
		foreach ($statement['assignments'] as $assignment) {
			if ($assignment['assign-type'] == 'variable') {
				if ($assignment['expr']['type'] != 'variable' && $assignment['expr']['type'] != 'array-access') {
					//echo $assignment['variable'], ' ', $assignment['expr']['type'], PHP_EOL;
					$this->_variablesToSkip[$branchNumber][$assignment['variable']] = 1;
				}
			}
		}
	}

	public function passStatementBlock($branchNumber, array $statements)
	{
		foreach ($statements as $statement) {
			switch ($statement['type']) {
				case 'let':
					$this->passLetStatement($branchNumber, $statement);
					break;
			}
		}
	}

	/**
	 * Returns a list of variables that are initialized in every analized branch
	 *
	 * @return array
	 */
	public function getVariables()
	{

		$variableStats = array();
		foreach ($this->_variablesToSkip as $branchNumber => $variables) {
			foreach ($variables as $variable => $one) {
				if (!isset($variableStats[$variable])) {
					$variableStats[$variable] = 1;
 				} else {
 					$variableStats[$variable]++;
 				}
			}
		}

		$variables = array();
		$numberBranches = count($this->_branches);
		foreach ($variableStats as $variable => $number) {
			if ($number == $numberBranches) {
				$variables[] = $variable;
			}
		}
		return $variables;
	}

}