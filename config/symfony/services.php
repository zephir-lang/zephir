<?php

declare(strict_types=1);

namespace Symfony\Component\DependencyInjection\Loader\Configurator;

use Zephir\Backends\BackendFactory;
use Zephir\BaseBackend;
use Zephir\Compiler;
use Zephir\Config;
use Zephir\Console\Application;
use Zephir\FileSystem\FileSystemInterface;
use Zephir\FileSystem\HardDisk;
use Zephir\Logger\Formatter\CompilerFormatter;

return function (ContainerConfigurator $configurator) {
    $services = $configurator
        ->services()
        ->defaults()
        ->autowire()
        ->autoconfigure()
        ->private()
        ->tag('monolog.logger', ['channel' => 'zephir']);

    $services
        ->load('Zephir\\', '../../Library')
        ->exclude('../../Library/{autoload.php,bootstrap.php,functions.php}');

    $services
        ->set(Application::class)
        ->public();

    $services
        ->set(FileSystemInterface::class, HardDisk::class)
        ->public()
        ->lazy()
        ->arg('$basePath', '%local_cache_path%');

    $services
        ->set(Config::class)
        ->factory('Zephir\Config::fromServer')
        ->public();

    $services
        ->set(BackendFactory::class)
        ->public()
        ->arg('$kernelsPath', param('kernels_path'))
        ->arg('$templatesPath', param('templates_path'));

    $services
        ->set(BaseBackend::class)
        ->factory([service(BackendFactory::class), 'createBackend'])
        ->public();

    $services
        ->set('compiler_log_formatter', CompilerFormatter::class);

    $services
        ->set(Compiler::class)
        ->public()
        ->call('setPrototypesPath', ['%prototypes_path%'])
        ->call('setOptimizersPath', ['%optimizers_path%'])
        ->call('setTemplatesPath', ['%templates_path%'])
        ->call('setLogger', [tagged_locator('monolog.logger')]);

    $configurator
        ->parameters()
        ->set('prototypes_path', '%kernel.project_dir%/prototypes')
        ->set('optimizers_path', '%kernel.project_dir%/Library/Optimizers')
        ->set('kernels_path', '%kernel.project_dir%/kernels')
        ->set('templates_path', '%kernel.project_dir%/templates')
        ->set('local_cache_path', '%kernel.local_cache_dir%');

    $configurator->extension('monolog', [
        'handlers' => [
            'console_stderr' => [
                'type' => 'stream',
                'path' => 'php://stderr',
                'process_psr_3_messages' => true,
                'bubble' => false,
                'level' => 'warning',
                'formatter' => 'compiler_log_formatter',
            ],
            'console_stdout' => [
                'type' => 'stream',
                'path' => 'php://stdout',
                'process_psr_3_messages' => true,
                'bubble' => false,
                'level' => 'info',
                'formatter' => 'compiler_log_formatter',
            ],
        ]
    ]);
};
