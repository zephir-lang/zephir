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

namespace Zephir\Console\Command;

use Symfony\Component\Console\Input\InputInterface;
use Symfony\Component\Console\Input\InputOption;
use Symfony\Component\Console\Output\OutputInterface;
use Symfony\Component\Console\Style\SymfonyStyle;
use Zephir\Compiler;
use Zephir\Exception;
use Zephir\Exception\ExceptionInterface;
use Zephir\Exception\InvalidArgumentException;

/**
 * Generate Command
 *
 * Generates C code from the Zephir code without compiling it.
 */
final class GenerateCommand extends AbstractCommand
{
    use ZflagsAwareTrait;

    private Compiler $compiler;

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
            ->addOption('trace', 't', InputOption::VALUE_NONE, 'Show trace message output (in case of exception error)')
            ->setHelp(sprintf('%s.', $this->getDescription()).PHP_EOL.PHP_EOL.$this->getZflagsHelp());
    }

    protected function execute(InputInterface $input, OutputInterface $output)
    {
        $io = new SymfonyStyle($input, $output);

        $trace = $input->getOption('trace');

        if (\extension_loaded('timecop') && 1 == ini_get('timecop.func_override')) {
            $io->getErrorStyle()->warning(
                <<<MSG
The timecop extension was detected. It is recommended to disable
it from the launcher during the code generation to avoid potential
issues.

For more see:

- https://github.com/zephir-lang/zephir/issues/1950
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
        } catch (ExceptionInterface | Exception $e) {
            if ($trace === true) {
                $io->getErrorStyle()->error($e->getMessage().sprintf(' (Zephir file: %s#%d)', $e->getFile(), $e->getLine()));
                $io->getErrorStyle()->error($e->getTraceAsString());
            } else {
                $io->getErrorStyle()->error($e->getMessage());
            }

            return 1;
        }

        return 0;
    }
}
