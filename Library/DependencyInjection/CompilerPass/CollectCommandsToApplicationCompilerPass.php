<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\DependencyInjection\CompilerPass;

use Symfony\Component\Console\Command\Command;
use Symfony\Component\DependencyInjection\Compiler\CompilerPassInterface;
use Symfony\Component\DependencyInjection\ContainerBuilder;
use Symfony\Component\DependencyInjection\Reference;
use Zephir\Console\Application;

final class CollectCommandsToApplicationCompilerPass implements CompilerPassInterface
{
    public function process(ContainerBuilder $containerBuilder)
    {
        $applicationDefinition = $containerBuilder->getDefinition(Application::class);

        foreach ($containerBuilder->getDefinitions() as $name => $definition) {
            if (null === $definition->getClass()) {
                continue;
            }

            /*
             * TODO: Deal with SelfUpdateCommand.
             *
             * if (\substr(__FILE__, 0, 5) === 'phar:') {
             *     // ...
             * }
             */
            if (is_a($definition->getClass(), Command::class, true)) {
                $applicationDefinition->addMethodCall('add', [new Reference($name)]);
            }
        }
    }
}
