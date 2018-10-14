<?php

/*
 * This file is part of the Zephir package.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Zephir\Support;

use PHPUnit\Framework\TestCase as BaseTestCase;

abstract class TestCase extends BaseTestCase
{
    /**
     * {@inheritdoc}
     *
     * @return string
     */
    public function toString()
    {
        $className = get_class($this);
        $className = explode('\\', $className);
        $className = array_pop($className);

        $method = $this->getPrintableMethodName();

        $buffer = sprintf('%s: %s', $className, $method);

        return $buffer . $this->getDataSetAsString();
    }

    protected function getPrintableMethodName()
    {
        $method = $this->getName();
        $method = preg_replace('#^(test|should)#', '', $method);
        $parts = preg_split('/(?=[A-Z])/', $method);
        $method = implode(' ', array_map(function ($part) {
            return mb_strtolower($part);
        }, $parts));

        $method = ucfirst(trim($method));

        return $method ?: 'Test';
    }
}
