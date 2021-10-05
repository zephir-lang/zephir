<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir;

use ReflectionException;
use Zephir\Passes\MutateGathererPass;
use Zephir\Statements\BreakStatement;
use Zephir\Statements\ContinueStatement;
use Zephir\Statements\DeclareStatement;
use Zephir\Statements\DoWhileStatement;
use Zephir\Statements\EchoStatement;
use Zephir\Statements\ForStatement;
use Zephir\Statements\IfStatement;
use Zephir\Statements\LetStatement;
use Zephir\Statements\LoopStatement;
use Zephir\Statements\RequireOnceStatement;
use Zephir\Statements\RequireStatement;
use Zephir\Statements\ReturnStatement;
use Zephir\Statements\SwitchStatement;
use Zephir\Statements\ThrowStatement;
use Zephir\Statements\TryCatchStatement;
use Zephir\Statements\UnsetStatement;
use Zephir\Statements\WhileStatement;
use function count;

/**
 * This represents a single basic block in Zephir.
 * A statements block is simply a container of instructions that execute sequentially.
 */
class StatementsBlock
{
    protected $statements;

    protected bool $unreachable = false;

    protected $debug = false;

    protected $loop = false;

    protected $mutateGatherer;

    /**
     * @var array|null
     */
    protected $lastStatement;

    /**
     * StatementsBlock constructor.
     *
     * @param array $statements
     */
    public function __construct(array $statements)
    {
        $this->statements = $statements;

        $debug = false;
        if (getenv('ZEPHIR_DEBUG')) {
            // Do not use this feature for typical use case.
            // Enabling debug mode using env var provided only for
            // testing purposes and may be removed in the future.
            // You SHOULD NOT rely on this possibility.
            $debug = getenv('ZEPHIR_DEBUG');
        }

        // TODO: Add container support
        // elseif ($this->container->has('ZEPHIR_DEBUG')) {
        //     $debug = $this->container->get('ZEPHIR_DEBUG');
        // }

        $this->debug = filter_var($debug, FILTER_VALIDATE_BOOLEAN);
    }

    /**
     * Sets whether the statements blocks belongs to a loop.
     *
     * @param bool $loop
     *
     * @return StatementsBlock
     */
    public function isLoop($loop)
    {
        $this->loop = $loop;

        return $this;
    }

    /**
     * @param CompilationContext $compilationContext
     * @param bool               $unreachable
     * @param int                $branchType
     *
     * @return Branch
     *
     * @throws Exception
     * @throws ReflectionException
     */
    public function compile(CompilationContext $compilationContext, bool $unreachable = false, int $branchType = Branch::TYPE_UNKNOWN): Branch
    {
        $compilationContext->codePrinter->increaseLevel();
        ++$compilationContext->currentBranch;

        /**
         * Create a new branch.
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
            $compilationContext->cycleBlocks[] = $this;
        }

        $where = '';
        if ($compilationContext->classDefinition) {
            $where = sprintf(
                'in %s',
                $compilationContext->classDefinition->getCompleteName()
            );

            if ($compilationContext->currentMethod) {
                $where .= sprintf('::%s', $compilationContext->currentMethod->getName());
            }
        }

        foreach ($statements as $statement) {
            /*
             * TODO: Generate GDB hints
             */
            if ($this->debug) {
                if (isset($statement['file'])) {
                    if ('declare' != $statement['type'] && 'comment' != $statement['type']) {
                        $compilationContext->codePrinter->outputNoIndent(
                            '#line '.$statement['line'].' "'.$statement['file'].'"'
                        );
                    }
                }
            }

            /**
             * Show warnings if code is generated when the 'unreachable state' is 'on'
             */
            if (true === $this->unreachable) {
                switch ($statement['type']) {
                    case 'echo':
                        $compilationContext->logger->warning(
                            sprintf('Unreachable code %s', $where),
                            ['unreachable-code', $statement['expressions'][0]]
                        );
                        break;

                    case 'let':
                        $compilationContext->logger->warning(
                            sprintf('Unreachable code %s', $where),
                            ['unreachable-code', $statement['assignments'][0]]
                        );
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
                            $compilationContext->logger->warning(
                                sprintf('Unreachable code %s', $where),
                                ['unreachable-code', $statement['expr']]
                            );
                        } else {
                            $compilationContext->logger->warning(
                                sprintf('Unreachable code %s', $where),
                                ['unreachable-code', $statement]
                            );
                        }
                        break;

                    default:
                        $compilationContext->logger->warning(
                            sprintf('Unreachable code %s', $where),
                            ['unreachable-code', $statement]
                        );
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

                case 'require_once':
                    $requireOnceStatement = new RequireOnceStatement($statement);
                    $requireOnceStatement->compile($compilationContext);
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
                    $compilationContext->codePrinter->output($compiledExpression->getCode().';');
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
                            $compilationContext->codePrinter->output($compiledExpression->getCode().';');
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
                    throw new Exception('Unsupported statement: '.$statement['type']);
            }

            if ('comment' != $statement['type']) {
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

        --$compilationContext->currentBranch;
        $compilationContext->codePrinter->decreaseLevel();

        return $currentBranch;
    }

    /**
     * Returns the statements in the block.
     *
     * @return array
     */
    public function getStatements(): array
    {
        return $this->statements;
    }

    /**
     * Setter for statements.
     *
     * @param array $statements
     */
    public function setStatements(array $statements): void
    {
        $this->statements = $statements;
    }

    /**
     * Returns the type of the last statement executed.
     *
     * @return string
     */
    public function getLastStatementType()
    {
        return $this->lastStatement['type'];
    }

    /**
     * Returns the last statement executed.
     *
     * @return array|null
     */
    public function getLastStatement()
    {
        return $this->lastStatement;
    }

    /**
     * Checks whether the block is empty or not.
     *
     * @return bool
     */
    public function isEmpty(): bool
    {
        return 0 === count($this->statements);
    }

    /**
     * Create/Returns a mutate gatherer pass for this block.
     *
     * @param bool $pass
     *
     * @return MutateGathererPass
     */
    public function getMutateGatherer(bool $pass = false): MutateGathererPass
    {
        if (!$this->mutateGatherer) {
            $this->mutateGatherer = new MutateGathererPass();
        }

        if ($pass) {
            $this->mutateGatherer->pass($this);
        }

        return $this->mutateGatherer;
    }
}
