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
 * CompilationContext
 *
 * This class encapsulates important entities requiring during compilation
 */
class CompilationContext
{

	/**
	 * Compiler
	 *
	 * @var \Compiler
	 */
	public $compiler;

	/**
	 * Current code printer
	 *
	 * @var \CodePrinter
	 */
	public $codePrinter;

	/**
	 * Whether the current method is static or not
	 *
	 * @var boolean
	 */
	public $staticContext;

	/**
	 * Code printer for the header
	 *
	 * @var \CodePrinter
	 */
	public $headerPrinter;

	/**
	 * Current symbol table
	 *
	 * @var \SymbolTable
	 */
	public $symbolTable;

	/**
	 * Type inference data
	 *
	 * @param \StaticTypeInference
	 */
	public $typeInference;

	/**
	 * Represents the class currently being compiled
	 *
	 * @var \ClassDefinition
	 */
	public $classDefinition;

	/**
	 * Current method being compiled
	 *
	 * @var \ClassMethod
	 */
	public $currentMethod;

	/**
	 * Represents the c-headers added to the file
	 *
	 * @var \HeadersManager
	 */
	public $headersManager;

	/**
	 * Represents the c-headers added to the file
	 *
	 * @var \HeadersManager
	 */
	public $stringsManager;

	/**
	 * Tells if the the compilation is being made inside a cycle
	 *
	 * @var int
	 */
	public $insideCycle = 0;

	/**
	 * Tells if the the compilation is being made inside a switch
	 *
	 * @var int
	 */
	public $insideSwitch = 0;

	/**
	 * The current branch, variables declared in conditional branches
	 * must be market if they're used out of those branches
	 */
	public $currentBranch = 0;

	/**
	 * Global config
	 *
	 * @var \Config
	 */
	public $config;

	/**
	 * Global logger
	 *
	 * @var \Logger
	 */
	public $logger;

}
