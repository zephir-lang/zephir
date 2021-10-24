<?php

declare(strict_types=1);

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

if (!file_exists(__DIR__.'/Library')) {
    exit(0);
}

$header = <<<'EOF'
This file is part of the Zephir.

(c) Phalcon Team <team@zephir-lang.com>

For the full copyright and license information, please view
the LICENSE file that was distributed with this source code.
EOF;

$finder = PhpCsFixer\Finder::create()
    ->in(__DIR__)
    ->append([__FILE__])
    ->exclude([
        'ext',
        'ide',
        'config/class-entries',
        'templates/ZendEngine3',
        'templates/Api',
        'tests/ext-bootstrap',
        'tests/fixtures',
        'tests/sharness',
    ])
    ->notPath('#config/class-entries.php#')
    ->notPath('#tests/fixtures/stubs/.*#')
    ->notPath('#tests/ext-bootstrap.php#');

$config = new PhpCsFixer\Config();
$config
    ->setRiskyAllowed(true)
    ->setFinder($finder)
    ->setRules([
        '@Symfony' => true,
        '@Symfony:risky' => true,
        '@PHPUnit48Migration:risky' => true,
        '@PSR2' => true,
        'php_unit_no_expectation_annotation' => false,
        'array_syntax' => ['syntax' => 'short'],
        'fopen_flags' => false,
        'ordered_imports' => [
            'sort_algorithm' => 'alpha',
            'imports_order' => [
                'class',
                'function',
                'const',
            ],
        ],
        'protected_to_private' => false,
        'phpdoc_summary' => false,
        'phpdoc_to_comment' => false,
        'phpdoc_var_annotation_correct_order' => true,
        'no_superfluous_phpdoc_tags' => false,
        'native_constant_invocation' => false,
        'native_function_invocation' => false,
        'no_extra_blank_lines' => [
            'tokens' => [
                'continue',
                'curly_brace_block',
                'default',
                'extra',
                'parenthesis_brace_block',
                'square_brace_block',
                'switch',
                'throw',
                'use_trait',
            ],
        ],
        'single_line_throw' => false,
        'psr_autoloading' => false,
        'types_spaces' => ['space' => 'single'],
        'yoda_style' => false,
    ]);

return $config;
