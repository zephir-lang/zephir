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

use Symfony\Component\Console\Input\InputDefinition;
use Symfony\Component\Console\Input\InputInterface;
use Symfony\Component\Console\Input\InputOption;
use Symfony\Component\Console\Output\OutputInterface;
use Symfony\Component\Console\Style\SymfonyStyle;
use Zephir\Compiler;
use Zephir\Exception;
use Zephir\Exception\ExceptionInterface;
use Zephir\Exception\InvalidArgumentException;

/**
 * Zephir\Console\Command\GenerateCommand.
 *
 * Generates C code from the Zephir code without compiling it.
 */
final class GenerateCommand extends Command
{
    use ZflagsAwareTrait;

    private $compiler;

    public function __construct(Compiler $compiler)
    {
        $this->compiler = $compiler;

        parent::__construct();
    }

    protected function configure()
    {
        $this
            ->setName('generate')
            ->setDescription('Generates C code from the Zephir code without compiling it')
            ->setDefinition($this->createDefinition())
            ->setHelp(sprintf('%s.', $this->getDescription()).PHP_EOL.PHP_EOL.$this->getZflagsHelp());
    }

    protected function execute(InputInterface $input, OutputInterface $output)
    {
        $io = new SymfonyStyle($input, $output);

        if (\extension_loaded('timecop') && 1 == ini_get('timecop.func_override')) {
            $io->getErrorStyle()->warning(
                <<<MSG
The timecop extension was detected. It is recommended to disable
it from the launcher during the code generation to avoid potential
issues.

For more see:

- https://github.com/phalcon/zephir/issues/1950
- https://github.com/phalcon/cphalcon/issues/13117
MSG
            );
        }

        try {
            // TODO: Move all the stuff from the compiler
            $this->compiler->generate(true);
        } catch (InvalidArgumentException $e) {
            $io->getErrorStyle()->error(
                sprintf(
                    'Internal error: %s at %s:%d',
                    $e->getMessage(),
                    $e->getFile(),
                    $e->getLine()
                )
            );

            return 1;
        } catch (ExceptionInterface $e) {
            $io->getErrorStyle()->error($e->getMessage());

            return 1;
        } catch (Exception $e) {
            $io->getErrorStyle()->error($e->getMessage());

            return 1;
        }

        return 0;
    }

    protected function createDefinition()
    {
        return new InputDefinition(
            [
                new InputOption(
                    'backend',
                    null,
                    InputOption::VALUE_REQUIRED,
                    'Used backend to generate extension',
                    'ZendEngine3'
                ),
            ]
        );
    }
}
