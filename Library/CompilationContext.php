<?php

/**
 * CompilationContext
 *
 * This class encapsulates important entities requiring during compilation
 */
class CompilationContext
{

	/**
	 * Current code printer
	 */
	public $codePrinter;

	/**
	 * Code printer for the header
	 */
	public $headerPrinter;

	/**
	 * Current symbol table
	 */
	public $symbolTable;

	/**
	 * Represents the class currently being compiled
	 */
	public $classDefinition;

	/**
	 * Represents the c-headers added to the file
	 */
	public $headersManager;

}