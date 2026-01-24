<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

declare(strict_types=1);

namespace Zephir\Statements;

use Zephir\Exception;

/**
 * Factory for creating statement instances based on statement type.
 * Eliminates large switch/case blocks for statement dispatching.
 */
final class StatementFactory
{
    /**
     * Map of statement types to their handler classes
     */
    private const STATEMENT_MAP = [
        'let' => LetStatement::class,
        'echo' => EchoStatement::class,
        'declare' => DeclareStatement::class,
        'if' => IfStatement::class,
        'while' => WhileStatement::class,
        'do-while' => DoWhileStatement::class,
        'switch' => SwitchStatement::class,
        'for' => ForStatement::class,
        'return' => ReturnStatement::class,
        'require' => RequireStatement::class,
        'require_once' => RequireOnceStatement::class,
        'loop' => LoopStatement::class,
        'break' => BreakStatement::class,
        'continue' => ContinueStatement::class,
        'unset' => UnsetStatement::class,
        'throw' => ThrowStatement::class,
        'try-catch' => TryCatchStatement::class,
    ];

    /**
     * Create a statement instance based on the statement type
     *
     * @param array $statement The statement data
     * @return StatementAbstract
     * @throws Exception
     */
    public static function create(array $statement): StatementAbstract
    {
        $type = $statement['type'] ?? null;

        if (!isset(self::STATEMENT_MAP[$type])) {
            throw new Exception('Unsupported statement: ' . ($type ?? 'unknown'));
        }

        $className = self::STATEMENT_MAP[$type];
        return new $className($statement);
    }

    /**
     * Check if a statement type is supported
     */
    public static function isSupported(string $type): bool
    {
        return isset(self::STATEMENT_MAP[$type]);
    }
}
