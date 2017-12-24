<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir                                                                   |
 | Copyright (c) 2013-present Zephir Team (https://zephir-lang.com/)        |
 |                                                                          |
 | This source file is subject the MIT license, that is bundled with this   |
 | package in the file LICENSE, and is available through the world-wide-web |
 | at the following url: http://zephir-lang.com/license.html                |
 +--------------------------------------------------------------------------+
 */

namespace Zephir\Exception;

/**
 * Zephir\Exception\ExceptionInterface
 *
 * @package Zephir\Exception
 */
trait ExceptionExtraAwareTrait
{
    /**
     * Extra info.
     *
     * @var array
     */
    protected $extra = [];

    /**
     * {@inheritdoc}
     *
     * @return array
     */
    public function getExtra()
    {
        return $this->extra;
    }

    /**
     * {@inheritdoc}
     *
     * @return string
     */
    public function getErrorRegion()
    {
        $region = '';
        $extra = $this->getExtra();

        if (isset($extra['file']) && file_exists($extra['file'])) {
            $lines = file($extra['file']);

            if (isset($lines[$extra['line'] - 1])) {
                $line = $lines[$extra['line'] - 1];
                $region .= sprintf("\t%s", str_replace("\t", " ", $line));

                if (($extra['char'] - 1) > 0) {
                    $region .= sprintf("\t%s^\n", str_repeat('-', $extra['char'] - 1));
                }
            }
        }

        return $region;
    }
}
