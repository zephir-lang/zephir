<?php

class Compiler
{

	protected $_files;

	protected $_definitions;

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
		foreach ($this->_files as $compileFile) {
			$compileFile->compile();
		}

	}

}
