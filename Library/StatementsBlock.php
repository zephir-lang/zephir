<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2015 Zephir Team and contributors                     |
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

namespace Zephir;

use Zephir\Exception;
use Zephir\Statements\DeclareStatement;
use Zephir\Statements\IfStatement;
use Zephir\Statements\ForStatement;
use Zephir\Statements\WhileStatement;
use Zephir\Statements\EchoStatement;
use Zephir\Statements\LetStatement;
use Zephir\Statements\RequireStatement;
use Zephir\Statements\ReturnStatement;
use Zephir\Statements\FetchStatement;
use Zephir\Statements\ThrowStatement;
use Zephir\Statements\LoopStatement;
use Zephir\Statements\BreakStatement;
use Zephir\Statements\ContinueStatement;
use Zephir\Statements\DoWhileStatement;
use Zephir\Statements\SwitchStatement;
use Zephir\Statements\TryCatchStatement;
use Zephir\Statements\UnsetStatement;
use Zephir\Passes\MutateGathererPass;

/**
 * StatementsBlock
 *
 * This represents a single basic block in Zephir. A statements block is simply a container of instructions that execute sequentially.
 */
class StatementsBlock
{
    protected $statements;

    protected $unreachable;

    protected $debug = false;

    protected $loop = false;

    protected $mutateGatherer;

    protected $lastStatement;

    /**
     * StatementsBlock constructor
     *
     * @param array $statements
     */
    public function __construct(array $statements)
    {
        $this->statements = $statements;
    }

    /**
     * Sets whether the statements blocks belongs to a loop
     *
     * @param boolean $loop
     * @return StatementsBlock
     */
    public function isLoop($loop)
    {
        $this->loop = $loop;
        return $this;
    }

    /**
     * @param CompilationContext $compilationContext
     * @param boolean $unreachable
     * @param int $branchType
     * @return Branch
     */
    public function compile(CompilationContext $compilationContext, $unreachable = false, $branchType = Branch::TYPE_UNKNOWN)
    {
        $compilationContext->codePrinter->increaseLevel();
        $compilationContext->currentBranch++;

        /**
         * Create a new branch
         */
        $currentBranch = new Branch();
        $currentBranch->setType($branchType);
        $currentBranch->setUnreachable($unreachable);

        /**
         * Activate branch in the branch manager
         */
        $compilationContext->branchManager->addBranch($currentBranch);

        $this->unreachable = $unreachable;

        $statements = $this->statements;

        /**
         * Reference the block if it belongs to a loop
         */
        if ($this->loop) {
            array_push($compilationContext->cycleBlocks, $this);
        }

        foreach ($statements as $statement) {
            /**
             * Generate GDB hints
             */
            if ($this->debug) {
                if (isset($statement['file'])) {
                    if ($statement['type'] != 'declare' && $statement['type'] != 'comment') {
                        $compilationContext->codePrinter->outputNoIndent('#line ' . $statement['line'] . ' "' . $statement['file'] . '"');
                    }
                }
            }

            /**
             * Show warnings if code is generated when the 'unreachable state' is 'on'
             */
            if ($this->unreachable === true) {
                switch ($statement['type']) {

                    case 'echo':
                        $compilationContext->logger->warning('Unreachable code', "unreachable-code", $statement['expressions'][0]);
                        break;

                    case 'let':
                        $compilationContext->logger->warning('Unreachable code', "unreachable-code", $statement['assignments'][0]);
                        break;

                    case 'fetch':
                    case 'fcall':
                    case 'mcall':
                    case 'scall':
                    case 'if':
                    case 'while':
                    case 'do-while':
                    case 'switch':
                    case 'for':
                    case 'return':
                    case 'c-block':
                        if (isset($statement['expr'])) {
                            $compilationContext->logger->warning('Unreachable code', "unreachable-code", $statement['expr']);
                        } else {
                            $compilationContext->logger->warning('Unreachable code', "unreachable-code", $statement);
                        }
                        break;

                    default:
                        $compilationContext->logger->warning('Unreachable code', "unreachable-code", $statement);
                }
            }

            switch ($statement['type']) {

                case 'let':
                    $letStatement = new LetStatement($statement);
                    $letStatement->compile($compilationContext);
                    break;

                case 'echo':
                    $echoStatement = new EchoStatement($statement);
                    $echoStatement->compile($compilationContext);
                    break;

                case 'declare':
                    $declareStatement = new DeclareStatement($statement);
                    $declareStatement->compile($compilationContext);
                    break;

                case 'if':
                    $ifStatement = new IfStatement($statement);
                    $ifStatement->compile($compilationContext);
                    break;

                case 'while':
                    $whileStatement = new WhileStatement($statement);
                    $whileStatement->compile($compilationContext);
                    break;

                case 'do-while':
                    $doWhileStatement = new DoWhileStatement($statement);
                    $doWhileStatement->compile($compilationContext);
                    break;

                case 'switch':
                    $switchStatement = new SwitchStatement($statement);
                    $switchStatement->compile($compilationContext);
                    break;

                case 'for':
                    $forStatement = new ForStatement($statement);
                    $forStatement->compile($compilationContext);
                    break;

                case 'return':
                    $returnStatement = new ReturnStatement($statement);
                    $returnStatement->compile($compilationContext);
                    $this->unreachable = true;
                    break;

                case 'require':
                    $requireStatement = new RequireStatement($statement);
                    $requireStatement->compile($compilationContext);
                    break;

                case 'loop':
                    $loopStatement = new LoopStatement($statement);
                    $loopStatement->compile($compilationContext);
                    break;

                case 'break':
                    $breakStatement = new BreakStatement($statement);
                    $breakStatement->compile($compilationContext);
                    $this->unreachable = true;
                    break;

                case 'continue':
                    $continueStatement = new ContinueStatement($statement);
                    $continueStatement->compile($compilationContext);
                    $this->unreachable = true;
                    break;

                case 'unset':
                    $unsetStatement = new UnsetStatement($statement);
                    $unsetStatement->compile($compilationContext);
                    break;

                case 'throw':
                    $throwStatement = new ThrowStatement($statement);
                    $throwStatement->compile($compilationContext);
                    $this->unreachable = true;
                    break;

                case 'try-catch':
                    $throwStatement = new TryCatchStatement($statement);
                    $throwStatement->compile($compilationContext);
                    $this->unreachable = false;
                    break;

                case 'fetch':
                    $expr = new Expression($statement['expr']);
                    $expr->setExpectReturn(false);
                    $compiledExpression = $expr->compile($compilationContext);
                    $compilationContext->codePrinter->output($compiledExpression->getCode() . ';');
                    break;

                case 'mcall':
                    $methodCall = new MethodCall();
                    $expr = new Expression($statement['expr']);
                    $expr->setExpectReturn(false);
                    $methodCall->compile($expr, $compilationContext);
                    break;

                case 'fcall':
                    $functionCall = new FunctionCall();
                    $expr = new Expression($statement['expr']);
                    $expr->setExpectReturn(false);
                    $compiledExpression = $functionCall->compile($expr, $compilationContext);
                    switch ($compiledExpression->getType()) {
                        case 'int':
                        case 'double':
                        case 'uint':
                        case 'long':
                        case 'ulong':
                        case 'char':
                        case 'uchar':
                        case 'bool':
                            $compilationContext->codePrinter->output($compiledExpression->getCode() . ';');
                            break;
                    }
                    break;

                case 'scall':
                    $methodCall = new StaticCall();
                    $expr = new Expression($statement['expr']);
                    $expr->setExpectReturn(false);
                    $methodCall->compile($expr, $compilationContext);
                    break;

                case 'cblock':
                    $compilationContext->codePrinter->output($statement['value']);
                    break;

                case 'comment':
                case 'empty':
                    break;

                default:
                    throw new Exception('Unsupported statement: ' . $statement['type']);
            }

            if ($statement['type'] != 'comment') {
                $this->lastStatement = $statement;
            }
        }

        /**
         * Reference the block if it belongs to a loop
         */
        if ($this->loop) {
            array_pop($compilationContext->cycleBlocks);
        }

        /**
         * Traverses temporal variables created in a specific branch
         * marking them as idle
         */
        $compilationContext->symbolTable->markTemporalVariablesIdle($compilationContext);

        $compilationContext->branchManager->removeBranch($currentBranch);

        $compilationContext->currentBranch--;
        $compilationContext->codePrinter->decreaseLevel();

        return $currentBranch;
    }

    /**
     * Returns the statements in the block
     *
     * @return array
     */
    public function getStatements()
    {
        return $this->statements;
    }

    /**
     * Setter for statements
     *
     * @param array $statements
     */
    public function setStatements(array $statements)
    {
        $this->statements = $statements;
    }

    /**
     * Returns the type of the last statement executed
     *
     * @return string
     */
    public function getLastStatementType()
    {
        return $this->lastStatement['type'];
    }

    /**
     * Returns the last statement executed
     *
     * @return array
     */
    public function getLastStatement()
    {
        return $this->lastStatement;
    }

    /**
     * Returns the last line in the last statement
     */
    public function getLastLine()
    {
        if (!$this->lastStatement) {
            $this->lastStatement = $this->statements[count($this->statements) - 1];
        }
    }

    /**
     * Create/Returns a mutate gatherer pass for this block
     *
     * @param boolean $pass
     * @return MutateGathererPass
     */
    public function getMutateGatherer($pass = false)
    {
        if (!$this->mutateGatherer) {
            $this->mutateGatherer = new MutateGathererPass;
        }
        if ($pass) {
            $this->mutateGatherer->pass($this);
        }
        return $this->mutateGatherer;
    }
}
