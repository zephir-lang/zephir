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

namespace Code\Builder;

/**
 * Class Struct
 * @package Code\Builder
 */
class Struct
{
	/**
	 * @var string
	 */
	protected $name;

	/**
	 * @var array
	 */
	protected $properties = array();

	/**
	 * @param $name
	 * @throws \InvalidArgumentException
	 */
	public function __construct($name)
	{
		if (!is_string($name)) {
			throw new \InvalidArgumentException('Struct name must be string!');
		}

		if (empty($name)) {
			throw new \InvalidArgumentException('Struct name must not be empty');
		}

		$this->name = $name;
	}

	/**
	 * @param $type
	 * @param $name
	 * @throws \InvalidArgumentException
	 */
	public function addProperty($type, $name)
	{
		if (!is_string($type)) {
			throw new \InvalidArgumentException('Property type must be string!');
		}

		if (!is_string($name)) {
			throw new \InvalidArgumentException('Property name must be string!');
		}

		$this->properties[$name] = $this->convertToCType($type);
	}

	/**
	 * @return string
	 */
	public function __toString()
	{
		$code = 'typedef struct '. $this->name .' { '.PHP_EOL;

		foreach($this->properties as $name => $type) {
			$code .= T . $type . ' ' . $name . ';' . PHP_EOL;
		}

		return $code . '} ' . substr($this->name, 1) . ';' . PHP_EOL;
	}

	/**
	 * @param $type
	 * @return string
	 * @throws \Exception
	 */
	protected function convertToCType($type)
	{
		switch($type) {
			case 'boolean':
			case 'bool':
				return 'zend_bool';
			case 'int':
			case 'float':
				return $type;
			default:
				throw new \Exception('Unknown global type: ' . $type);
				break;
		}
	}
} 