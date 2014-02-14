<?php

namespace Zephir;

/**
 * Loader
 *
 * Loads classes when the composer autoloader is not installed
 */
class Loader
{
	/**
	 * @param string $className
	 */
	public function autoload($className)
	{
		require __DIR__ . str_replace('Zephir' . DIRECTORY_SEPARATOR, DIRECTORY_SEPARATOR, str_replace('\\', DIRECTORY_SEPARATOR, $className)) . '.php';
	}
}