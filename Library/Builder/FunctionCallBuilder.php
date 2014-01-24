<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2014 Zephir Team and contributors                     |
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

/**
 * FunctionCallBuilder
 *
 * Allows to manually build a function call AST node
 */
class FunctionCallBuilder
{

	protected $_name;

	protected $_parameters;

	protected $_type;

	/**
	 * FunctionCallBuilder construct
	 *
	 * @param string $name
	 * @param array $name
	 * @param int $name
	 */
	public function __construct($name, $parameters, $type=1)
	{
		$this->_name = $name;
		$this->_parameters = $parameters;
		$this->_type = $type;
	}

	public function get()
	{
		return array(
			'type'       => 'fcall',
			'name'       => $this->_name,
			'call-type'  => $this->_type,
			'parameters' => $this->_parameters
		);
	}

}