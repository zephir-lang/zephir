<?php

/**
 * This file is part of the Zephir package.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

return [
    'class' => [
        'namespace' => 'Zephir\Stubs',
        'className' => 'Stubs',
        'classShortName' => 'get'
    ],
    'method' => [
        'visibility' => ['public', 'final'],
        'name'       => 'scalarInputAgrs',
        'parameters' => [
            ['data-type' => 'int', 'name' => 'val1'],
            ['data-type' => 'bool', 'name' => 'val2'],
            ['data-type' => 'string', 'name' => 'val3'],
            ['data-type' => 'double', 'name' => 'val4'],
        ],
        'statements' => [],
        'docblock'   => "Test function scalarInputArgs(int val1, bool val2, string val3, double val4)\n with return Integer\n @return int",
        'returnType' => null,
        'expression' => null
    ]
];
