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

/**
 * The plain PHP counterpart of Stub\Issue2691::pushIntoReadonly().
 *
 * Readonly properties arrived in 8.1, so this file is only ever included from
 * a test that has checked the version first; on 8.0 Zephir's ZEND_ACC_READONLY
 * is a no-op shim and there is nothing to compare against.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2691
 */
final class Issue2691ReadonlyHolder
{
    public readonly array $readonlyRows;

    public function __construct()
    {
        $this->readonlyRows = ['a' => ['seed']];
    }

    public function pushIntoReadonly(string $key, string $value): bool
    {
        array_push($this->readonlyRows[$key], $value);

        return true;
    }

    public function getReadonlyRows(): array
    {
        return $this->readonlyRows;
    }
}
