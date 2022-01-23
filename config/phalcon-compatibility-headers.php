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
 * Custom ArgInfo definitions for cross PHP versions support.
 * These code blocks will be used inside `.zep.h` files.
 */
return [
    'Serializable' => [
        'unserialize' => [
            '#if PHP_VERSION_ID >= 80000' => [
                'ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(__ce__, 0, 1, IS_VOID, 0)',
                '    ZEND_ARG_TYPE_INFO(0, serialized, IS_STRING, 0)',
                'ZEND_END_ARG_INFO()',
            ],
            '#else' => [
                'ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(__ce__, 0, 1, IS_VOID, 0)',
                '    ZEND_ARG_INFO(0, serialized)',
                'ZEND_END_ARG_INFO()',
            ],
        ],
    ],

    'AbstractSerializer' => [
        'unserialize' => [
            '#if PHP_VERSION_ID >= 80000' => [
                'ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(__ce__, 0, 1, IS_VOID, 0)',
                '    ZEND_ARG_TYPE_INFO(0, serialized, IS_STRING, 0)',
                'ZEND_END_ARG_INFO()',
            ],
            '#else' => [
                'ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(__ce__, 0, 1, IS_VOID, 0)',
                '    ZEND_ARG_INFO(0, serialized)',
                'ZEND_END_ARG_INFO()',
            ],
        ],
    ],

    'SessionHandlerInterface' => [
        'gc' => [
            '#if PHP_VERSION_ID >= 80000' => [
                'ZEND_BEGIN_ARG_INFO_EX(__ce__, 0, 0, 1)',
                '    ZEND_ARG_TYPE_INFO(0, maxlifetime, IS_LONG, 0)',
                'ZEND_END_ARG_INFO()',
            ],
            '#else' => [
                'ZEND_BEGIN_ARG_INFO_EX(__ce__, 0, 0, 1)',
                '    ZEND_ARG_INFO(0, maxlifetime)',
                'ZEND_END_ARG_INFO()',
            ],
        ],
    ],

    'Phalcon\Session\Adapter\Noop' => [
        'gc' => [
            '#if PHP_VERSION_ID >= 80000' => [
                'ZEND_BEGIN_ARG_INFO_EX(__ce__, 0, 0, 1)',
                '    ZEND_ARG_TYPE_INFO(0, maxlifetime, IS_LONG, 0)',
                'ZEND_END_ARG_INFO()',
            ],
            '#else' => [
                'ZEND_BEGIN_ARG_INFO_EX(__ce__, 0, 0, 1)',
                '    ZEND_ARG_INFO(0, maxlifetime)',
                'ZEND_END_ARG_INFO()',
            ],
        ],
    ],
];
