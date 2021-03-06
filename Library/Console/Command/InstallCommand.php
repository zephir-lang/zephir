<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Console\Command;

use Symfony\Component\Console\Input\InputInterface;
use Symfony\Component\Console\Input\InputOption;
use Symfony\Component\Console\Output\OutputInterface;
use Symfony\Component\Console\Style\SymfonyStyle;
use Zephir\Compiler;
use Zephir\Config;
use Zephir\Exception;
use Zephir\Exception\CompilerException;
use Zephir\Exception\NotImplementedException;

/**
 * Zephir\Console\Command\InstallCommand.
 *
 * Installs the extension in the extension directory.
 */
final class InstallCommand extends Command
{
    use DevelopmentModeAwareTrait;
    use ZflagsAwareTrait;

    private $compiler;
    private $config;

    public function __construct(Compiler $compiler, Config $config)
    {
        $this->compiler = $compiler;
        $this->config = $config;

        parent::__construct();
    }

    protected function configure()
    {
        $this
            ->setName('install')
            ->setDescription('Installs the extension in the extension directory (may require root password)')
            ->addOption('dev', null, InputOption::VALUE_NONE, 'Install the extension in development mode')
            ->addOption('no-dev', null, InputOption::VALUE_NONE, 'Install the extension in production mode')
            ->setHelp($this->getDevelopmentModeHelp().PHP_EOL.$this->getZflagsHelp());
    }

    protected function execute(InputInterface $input, OutputInterface $output)
    {
        $io = new SymfonyStyle($input, $output);

        try {
            $this->compiler->install(
                $this->isDevelopmentModeEnabled($input)
            );
        } catch (NotImplementedException $e) {
            $io->getErrorStyle()->note($e->getMessage());

            return 0;
        } catch (CompilerException $e) {
            $io->getErrorStyle()->error($e->getMessage());

            return 1;
        } catch (Exception $e) {
            $io->getErrorStyle()->error($e->getMessage());

            return 1;
        }

        $success = ['Extension installed.'];

        $namespace = $this->config->get('namespace');
        if (!\extension_loaded($namespace)) {
            $success[] = sprintf('Add "extension=%s.so" to your php.ini', $namespace);
        }

        $io->text($success);
        $io->note("Don't forget to restart your web server");

        return 0;
    }

    /**
     * {@inheritdoc}
     *
     * @return string
     */
    protected function getDevelopmentModeHelp()
    {
        return <<<EOT
Using <comment>--dev</comment> option will try installing the extension in development mode
(debug symbols and no optimizations). An extension compiled with debugging symbols means
you can run a program or library through a debugger and the debugger's output will be user
friendlier. These debugging symbols also enlarge the program or library significantly.

NOTE: Zephir development mode will be enabled silently if your PHP binary was compiled in
a debug configuration.

In some cases, we would like to get production ready extension even if the PHP binary was
compiled in a debug configuration. Use <comment>--no-dev</comment> option to achieve this behavior.

NOTE: This command may require root password on Linux/Unit systems.

EOT;
    }
}
