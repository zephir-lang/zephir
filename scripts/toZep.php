<?php
/*
  +------------------------------------------------------------------------+
  | Phalcon Framework                                                      |
  +------------------------------------------------------------------------+
  | Copyright (c) 2011-2013 Phalcon Team (http://www.phalconphp.com)       |
  +------------------------------------------------------------------------+
  | This source file is subject to the New BSD License that is bundled     |
  | with this package in the file docs/LICENSE.txt.                        |
  |                                                                        |
  | If you did not receive a copy of the license and are unable to         |
  | obtain it through the world-wide-web, please send an email             |
  | to license@phalconphp.com so we can send you a copy immediately.       |
  +------------------------------------------------------------------------+
  | Authors: Andres Gutierrez <andres@phalconphp.com>                      |
  |          Eduar Carvajal <eduar@phalconphp.com>                         |
  +------------------------------------------------------------------------+
*/

class ZephirTraslator
{
	/**
	 * Folder with source code 
	 * 
	 * @var string 
	 */
	private $_folderInit = 'ext/';
	
	
	/**
	 * Folder target of new source code 
	 * 
	 * @var string 
	 */
	private $_folderTarget = 'phalcon/';
	 
	/**
	 * Flag of Comment block
	 * 
	 * @var bool
	 */
	private $_comment = false;
	

	public function __construct()
	{
		//Scan Files to copy to zephir format
		$this->_scanFiles($this->_folderInit);
		$this->_readFileC('ext/version.c');
		
	}
	
	/**
	 * Check if existe folder in phalcon from ext
	 */
	private function _checkFolder($path='')
	{
		if (!is_dir($this->_folderTarget.$path)){
			mkdir($this->_folderTarget.$path);
		}
	}
	
	/**
	 * Make a ZEP file from C file
	 * 
	 */
	private function _readFileC($path)
	{
		//Check if file have *.c extension
		if (is_file($path) && preg_match('/.c$/',$path)) {
			
			echo PHP_EOL."--ZEP--".PHP_EOL.$path;
			
			$newPath = str_replace($this->_folderInit,$this->_folderTarget,$path);
			$newPath = substr($newPath,0,count($newPath)-3).".zep";
			
			echo PHP_EOL."New File:".$newPath;
			
			$fileZep = fopen($newPath,"a");
			
			if ($fileZep) {
				
				//get Content of File
				$file = file($path);
				
				$flagWrite = false;
				foreach ($file as $line)
				{
					$line = trim($line);
					
					//check if is active commnet
					$flagWrite = false;
						
					if ($line == '/*') {
						$this->_comment = true;
						$flagWrite = true;
					}
					
					echo PHP_EOL."IF".PHP_EOL."if ({$this->_comment} || $flagWrite) {";
					if ($this->_comment==true || $flagWrite==true) {
						fwrite($fileZep, $line.PHP_EOL);
					}
					
					if ($this->_comment == true && $line == '*/') {
						$this->_comment = false;
						$flagWrite = false;
					}
					
				}
				
				fclose($fileZep);
			} else {
				echo "No se pudo crear el archivo";
			}
		}
	}
	
	/**
	 * Scan Files and Folders
	 * 
	 */
	private function _scanFiles($path='')
	{
		
		$results = scandir($path);

		//Check if Phalcon folder exists
		$this->_checkFolder();
		
		foreach ($results as $result) 
		{
			if ($result === '.' or $result === '..') continue;

			//Copy structure of folders
			if (is_dir($path . '/' . $result)) {
				
				//New path en phalcon folder
				$newPath = $path . $result;
				$newPath = str_replace("ext/","",$newPath);
				
				//Check if new folder in Phalcon's folder exists
				$this->_checkFolder($newPath);
				
				//Recursive
				$this->_scanFiles($path . $result);
			} else {
				
				//make content to zep files from *.c files into ext/ folder
				//$this->_readFileC($path . $result);
				
			}
		}
		return true;
	}
	
}

$zephirTraslator = new ZephirTraslator();

?>
