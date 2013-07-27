<?php

class Compiler
{

	protected $_files;

	protected $_definitions;
	
	protected $_compiledFiles;

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
		$files = array(); 
		foreach ($this->_files as $compileFile) 
		{
			$compileFile->compile();
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
		
		$toReplace = array (
			'%PROJECT_LOWER%' 		=> strtolower($project),
			'%PROJECT_UPPER%' 		=> strtoupper($project),
			'%PROJECT_CAMELIZE%' 	=> ucfirst($project),
			'%FILES_COMPILED%' 		=> implode(' ', $this->_compiledFiles),
		);
		
		foreach ($toReplace as $mark => $replace)
		{
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

		/////project.c////
		$content = file_get_contents('templates/project.c');
		
		if (empty($content)) {
			throw new Exception("Template project.c doesn't exists");
		}
		
		$classEntries = array();
		$classInits = array();
		foreach ($this->_files as $file)
		{
			$classEntries[] = 'zend_class_entry *'.$file->getClassDefinition()->getClassEntry().';';
			$classInits[] = 'ZEPHIR_INIT('.$file->getClassDefinition()->getCNamespace().'_'.$file->getClassDefinition()->getName().');';
		}
		
		$toReplace = array (
			'%PROJECT_LOWER%' 		=> strtolower($project),
			'%PROJECT_UPPER%' 		=> strtoupper($project),
			'%PROJECT_CAMELIZE%' 	=> ucfirst($project),
			'%CLASS_ENTRIES%' 		=> implode(PHP_EOL, $classEntries),
			'%CLASS_INITS%'			=> implode(PHP_EOL.'	', $classInits),
		);
		
		foreach ($toReplace as $mark => $replace)
		{
				$content = str_replace($mark,$replace,$content);
		}
		
		file_put_contents('ext/'.strtolower($project).'.c', $content);
		
		unset($content);
		
		/////project.h////
		$content = file_get_contents('templates/project.h');
		
		if (empty($content)) {
			throw new Exception("Template project.h doesn't exists");
		}
		
		$includeHeaders = array();
		foreach ($this->_compiledFiles as $file)
		{
			$fileH = str_replace(".c",".h",$file);
			$include = '#include "'.$fileH.'"';
			$includeHeaders[]= $include;
		}
		
		$toReplace = array (
			'%INCLUDE_HEADERS%' => implode(PHP_EOL,$includeHeaders)
		);
		
		foreach ($toReplace as $mark => $replace)
		{
				$content = str_replace($mark,$replace,$content);
		}
		
		file_put_contents('ext/'.strtolower($project).'.h', $content);
	}

}
