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

namespace Extension;

use Throwable;

/**
 * PHP is the oracle: run the same operation twice, once through the compiled
 * extension and once in plain PHP, and compare the whole transcript -- the
 * diagnostics raised and the resulting value.
 *
 * Hard-coding the expected text instead would pin the assertion to one PHP
 * version, and PHP moves: string-offset diagnostics changed in 8.1 and again
 * in 8.5, implicit float-to-int conversion started deprecating in 8.1.
 * Comparing two live evaluations survives all of it.
 */
trait AssertsPhpParity
{
    /**
     * Runs both sides and asserts the diagnostics and the value agree.
     */
    private function assertMatchesPhp(callable $extension, callable $php): void
    {
        $this->assertSame(
            $this->transcript($php),
            $this->transcript($extension),
            'The extension must behave exactly as PHP does'
        );
    }

    private function transcript(callable $subject): string
    {
        $diagnostics = [];

        set_error_handler(static function (int $code, string $message) use (&$diagnostics): bool {
            $diagnostics[] = $message;

            return true;
        });

        try {
            $value  = $subject();
            $result = is_string($value)
                ? 'string(' . strlen($value) . ') ' . bin2hex($value)
                : var_export($value, true);
        } catch (Throwable $e) {
            $result = get_class($e) . ': ' . $e->getMessage();
        } finally {
            restore_error_handler();
        }

        return implode(' ; ', $diagnostics) . ' => ' . $result;
    }
}
