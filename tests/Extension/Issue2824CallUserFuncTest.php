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

use Closure;
use PHPUnit\Framework\TestCase;
use Stub\Issue2824Callbacks;
use Throwable;

use function call_user_func;
use function call_user_func_array;
use function iterator_to_array;

/**
 * `call_user_func()` and `call_user_func_array()` resolve a callback in the
 * class whose code makes the call, as in a PHP method, and throw PHP's
 * TypeError for a callback or an argument list they cannot use.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2824
 */
final class Issue2824CallUserFuncTest extends TestCase
{
    private Issue2824Callbacks $test;

    protected function setUp(): void
    {
        $this->test = new Issue2824Callbacks();
    }

    public function testPrivateCallbackFromMethod(): void
    {
        $this->assertSame('secret:z!', $this->test->fromMethod());
        $this->assertSame('secret:w?', $this->test->fromMethodArray());
        $this->assertSame('secret:12', $this->test->fromMethodNamed());
    }

    public function testPrivateCallbackFromInternalMethod(): void
    {
        $this->assertSame('secret:x|secret:y', $this->test->fromInternal());
    }

    public function testPrivateCallbackFromGenerator(): void
    {
        $this->assertSame(['secret:g'], iterator_to_array($this->test->fromGenerator()));
    }

    public function testNotCallableThrowsLikePhp(): void
    {
        $this->assertSame(
            $this->outcome(fn () => call_user_func('issue2824_nope')),
            $this->outcome(fn () => $this->test->notCallable('issue2824_nope'))
        );
        $this->assertSame(
            $this->outcome(fn () => call_user_func('issue2824_nope', 1)),
            $this->outcome(fn () => $this->test->notCallableWithArgs('issue2824_nope'))
        );
        $this->assertSame(
            $this->outcome(fn () => call_user_func_array('issue2824_nope', [])),
            $this->outcome(fn () => $this->test->notCallableArray('issue2824_nope'))
        );
    }

    public function testNotArrayThrowsLikePhp(): void
    {
        $this->assertSame(
            $this->outcome(fn () => call_user_func_array('strlen', 'x')),
            $this->outcome(fn () => $this->test->notArray('strlen', 'x'))
        );
    }

    private function outcome(Closure $call): mixed
    {
        try {
            return $call();
        } catch (Throwable $e) {
            return [$e::class, $e->getMessage()];
        }
    }
}
