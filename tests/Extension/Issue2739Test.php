<?php

declare(strict_types=1);

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Extension;

use PHPUnit\Framework\TestCase;
use Stub\Issue2739;

/**
 * Zephir `time()` against PHP `time()` right after a second boundary.
 *
 * PHP `time()` is `php_time()`, which reads `gettimeofday()`. Zephir used C
 * `time(NULL)`, which on glibc reads a coarse clock that still gives the
 * previous second for a few milliseconds after the boundary. A Zephir read that
 * follows a PHP read could therefore be one second smaller.
 *
 * The window is only open just after PHP `time()` changes, so each test waits
 * for that change and then samples Zephir `time()` inside the window.
 *
 * @issue https://github.com/zephir-lang/zephir/issues/2739
 */
final class Issue2739Test extends TestCase
{
    private const BOUNDARIES = 2;
    private const WINDOW_SECONDS = 0.005;

    private Issue2739 $subject;

    protected function setUp(): void
    {
        $this->subject = new Issue2739();
    }

    public function testNowIsTheCurrentTimestamp(): void
    {
        $before = time();
        $now = $this->subject->now();

        $this->assertGreaterThanOrEqual($before, $now);
        $this->assertLessThanOrEqual(time(), $now);
    }

    public function testNowIsNeverBehindPhpTimeAfterASecondBoundary(): void
    {
        $lagging = 0;

        for ($i = 0; $i < self::BOUNDARIES; $i++) {
            $php = $this->waitForNextSecond();
            $end = microtime(true) + self::WINDOW_SECONDS;
            while (microtime(true) < $end) {
                if ($this->subject->now() < $php) {
                    $lagging++;
                }
            }
        }

        $this->assertSame(0, $lagging, 'Zephir time() gave a second before PHP time()');
    }

    public function testNotBeforeAcceptsPhpTimeAfterASecondBoundary(): void
    {
        $rejected = 0;

        for ($i = 0; $i < self::BOUNDARIES; $i++) {
            $this->waitForNextSecond();
            $end = microtime(true) + self::WINDOW_SECONDS;
            while (microtime(true) < $end) {
                if (!$this->subject->notBefore(time())) {
                    $rejected++;
                }
            }
        }

        $this->assertSame(0, $rejected, 'notBefore(time()) rejected the current PHP time()');
    }

    private function waitForNextSecond(): int
    {
        $start = time();
        do {
            $now = time();
        } while ($now === $start);

        return $now;
    }
}
