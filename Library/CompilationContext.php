<?php

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
	 * @var Compiler
	 */
	public $compiler;

	/**
	 * Current code printer
	 *
	 * @var CodePrinter
	 */
	public $codePrinter;

	/**
	 * Code printer for the header
	 *
	 * @var CodePrinter
	 */
	public $headerPrinter;

	/**
	 * Current symbol table
	 *
	 * @var SymbolTable
	 */
	public $symbolTable;

	/**
	 * Represents the class currently being compiled
	 *
	 * @var ClassDefinition
	 */
	public $classDefinition;

	/**
	 * Represents the c-headers added to the file
	 *
	 * @var HeadersManager
	 */
	public $headersManager;

	/**
	 * Tells if the the compilation is being made inside a cycle
	 *
	 * @var int
	 */
	public $insideCycle = 0;

}
