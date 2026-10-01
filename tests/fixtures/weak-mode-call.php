<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

/*
 * No strict_types declaration on purpose: strictness belongs to the file
 * that makes the call, so a call made from here coerces arguments weakly.
 */
return static fn (callable $callee, mixed ...$arguments) => $callee(...$arguments);
