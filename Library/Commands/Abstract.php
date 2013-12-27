<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013 Zephir Team and contributors                          |
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
 * CommandAbstract
 *
 * Provides a superclass for commands
 */
abstract class CommandAbstract implements CommandInterface
{
	private $_parameters = null;

	/**
	 * Command provided by this command
	 *
	 * @return string
	 */
	abstract public function getCommand();

	/**
	 * Command usage
	 *
	 * @return string
	 */
	abstract public function getUsage();

	/**
	 * @return string
	 */
	abstract public function getDescription();

	/**
	 * Returns parameter named $name if specified
	 * on the commmand line else null
	 *
	 * @param string $name
	 * @param string $value
	 * @return void
	 */
	protected function setParameter($name, $value)
	{
		if (!isset($this->_parameters)) {
			$this->_parameters = array();
		}
		$this->_parameters[$name] = $value;
	}
	
	/**
	 * Returns parameter named $name if specified
	 * on the commmand line else null
	 * @param string $parameterName
	 * @return string
	 */
	public function getParameter($name)
	{
		return (isset($this->_parameters[$name])) ? $this->_parameters[$name] : null;
	}
	
	/**
	 * Executes the command
	 *
	 * Config $config
	 * Logger $logger
	 */
	public function execute(Config $config, Logger $logger)
	{
		$compiler = new Compiler($config, $logger);
		$command = $this->getCommand();
		$compiler->$command($this);
	}

}
