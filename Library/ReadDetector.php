<?php

/**
 * ReadDetector
 *
 * Detects if a variable is used in a given expression
 */
class ReadDetector
{
	public function detect($variable, $expression)
	{
		if ($expression['type'] == 'variable') {
			if ($variable == $expression['value']) {
				return true;
			}
		}

		if (isset($expression['left'])) {
			if ($this->detect($variable, $expression['left']) === true) {
				return true;
			}
		}

		if (isset($expression['right'])) {
			if ($this->detect($variable, $expression['right']) === true) {
				return true;
			}
		}

		return false;
	}
}