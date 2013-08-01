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
 * Compiler
 *
 * Main compiler
 */
class Compiler
{

	protected $_files;

	protected $_definitions;

	protected $_compiledFiles;

	/**
	 * Pre-compiles classes creating a CompilerFile definition
	 *
	 * @param string $filePath
	 */
	protected function _preCompile($filePath)
	{
		if (preg_match('/\.zep$/', $filePath)) {
			$className = str_replace('/', '\\', $filePath);
			$className = preg_replace('/.zep$/', '', $className);
			$className = join('\\', array_map(function($i) { return ucfirst($i); }, explode('\\', $className)));
			$this->_files[$className] = new CompilerFile($className, $filePath);
			$this->_files[$className]->preCompile();
			$this->_definitions[$className] = $this->_files[$className]->getClassDefinition();
		}
	}

	protected function _recursivePreCompile($path)
	{
		/**
		 * Pre compile all files
		 */
		$iterator = new DirectoryIterator($path);
		foreach ($iterator as $item) {
			if ($item->isDir()) {
				if ($item->getFileName() != '.' && $item->getFileName() != '..') {
					$this->_recursivePreCompile($item->getPathname());
				}
			} else {
				$this->_preCompile($item->getPathname());
			}
		}
	}

	/**
	 * Allows to check if a class is part of the compiled extension
	 *
	 * @param string $className
	 * @return bolean
	 */
	public function isClass($className)
	{
		$classes = array_keys($this->_definitions);
		foreach ($classes as $value) {
			if (!strcasecmp($value, $className)) {
				return true;
			}
		}
		return false;
	}

	/**
	 * Returns class the class definition from a given class name
	 *
	 * @param string $className
	 * @return ClassDefinition
	 */
	public function getClassDefinition($className)
	{
		foreach ($this->_definitions as $key => $value) {
			if (!strcasecmp($key, $className)) {
				return $value;
			}
		}
		return false;
	}

	/**
	 *
	 */
	public function compile()
	{

		if (!is_dir('.temp')) {
			mkdir('.temp');
		}

		/**
		 * Round 1. pre-compile all files in memory
		 */
		$this->_recursivePreCompile('test');

		/**
		 * Round 2. compile all files to C sources
		 */
		$files = array();
		foreach ($this->_files as $compileFile) {
			$compileFile->compile($this);
			$files[] = $compileFile->getCompiledFile();
		}

		$this->_compiledFiles = $files;

		/**
		 * Round 3. create config.m4 and config.w32 files
		 */
		$this->createConfigFiles();

		/**
		 * Round 4. create project.c and project.h files
		 */
		$this->createProjectFiles();

	}

	/**
	 * Create config.m4 and config.w32 by compiled files to test extension
	 *
	 */
	public function createConfigFiles($project='test')
	{

		$content = file_get_contents('templates/config.m4');

		if (empty($content)) {
			throw new Exception("Template config.m4 doesn't exists");
		}

		$toReplace = array(
			'%PROJECT_LOWER%' 		=> strtolower($project),
			'%PROJECT_UPPER%' 		=> strtoupper($project),
			'%PROJECT_CAMELIZE%' 	=> ucfirst($project),
			'%FILES_COMPILED%' 		=> implode(' ', $this->_compiledFiles),
		);

		foreach ($toReplace as $mark => $replace) {
			$content = str_replace($mark,$replace,$content);
		}

		file_put_contents('ext/config.m4', $content);
	}

	/**
	 * Create project.c and project.h by compiled files to test extension
	 *
	 */
	public function createProjectFiles($project='test')
	{

		/**
		 * project.c
		 */
		$content = file_get_contents('templates/project.c');

		if (empty($content)) {
			throw new Exception("Template project.c doesn't exists");
		}

		$classEntries = array();
		$classInits = array();
		foreach ($this->_files as $file) {
			$classDefinition = $file->getClassDefinition();
			$classEntries[] = 'zend_class_entry *' . $classDefinition->getClassEntry() . ';';
			$classInits[] = 'ZEPHIR_INIT(' . $classDefinition->getCNamespace() . '_' . $classDefinition->getName() . ');';
		}

		$toReplace = array(
			'%PROJECT_LOWER%' 		=> strtolower($project),
			'%PROJECT_UPPER%' 		=> strtoupper($project),
			'%PROJECT_CAMELIZE%' 	=> ucfirst($project),
			'%CLASS_ENTRIES%' 		=> implode(PHP_EOL, $classEntries),
			'%CLASS_INITS%'			=> implode(PHP_EOL . "\t", $classInits),
		);

		foreach ($toReplace as $mark => $replace) {
			$content = str_replace($mark, $replace, $content);
		}

		file_put_contents('ext/' . strtolower($project) . '.c', $content);

		unset($content);

		/**
		 * project.h
		 */
		$content = file_get_contents('templates/project.h');

		if (empty($content)) {
			throw new Exception("Template project.h doesn't exists");
		}

		$includeHeaders = array();
		foreach ($this->_compiledFiles as $file) {
			$fileH = str_replace(".c", ".h", $file);
			$include = '#include "' . $fileH . '"';
			$includeHeaders[] = $include;
		}

		$toReplace = array(
			'%INCLUDE_HEADERS%' => implode(PHP_EOL, $includeHeaders)
		);

		foreach ($toReplace as $mark => $replace) {
			$content = str_replace($mark, $replace, $content);
		}

		file_put_contents('ext/' . strtolower($project) . '.h', $content);
	}

	/**
	 * Boots the compiler showing exceptions
	 */
	public static function boot()
	{
		try {
			$c = new Compiler();
			echo $c->compile();
		} catch (Exception $e) {
			echo 'Exception: ', $e->getMessage(), PHP_EOL;
			if (method_exists($e, 'getExtra')) {
				$extra = $e->getExtra();
				if (is_array($extra)) {
					if (isset($extra['file'])) {
						echo PHP_EOL;
						$lines = file($extra['file']);
						$line = $lines[$extra['line'] - 1];
						echo "\t", str_replace("\t", " ", $line);
						echo "\t", str_repeat("-", $extra['char'] - 1), "^", PHP_EOL;
					}
				}
			}
			echo PHP_EOL;
			$pwd = getcwd();
			echo $e->getFile(), ' ', $e->getLine(), PHP_EOL;
			echo str_replace($pwd, '', $e->getTraceAsString()), PHP_EOL;
			exit(1);
		}
	}

}
