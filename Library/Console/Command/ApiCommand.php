<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Console\Command;

use Symfony\Component\Console\Exception\InvalidArgumentException;
use Symfony\Component\Console\Input\InputDefinition;
use Symfony\Component\Console\Input\InputInterface;
use Symfony\Component\Console\Input\InputOption;
use Symfony\Component\Console\Output\OutputInterface;
use Zephir\Compiler;
use Zephir\Config;

/**
 * Zephir\Console\Command\ApiCommand.
 *
 * Generates a HTML API based on the classes exposed in the extension.
 */
final class ApiCommand extends Command
{
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
            ->setName('api')
            ->setDescription('Generates a HTML API based on the classes exposed in the extension')
            ->setDefinition($this->createDefinition())
            ->setHelp(sprintf('%s.', $this->getDescription()).PHP_EOL.PHP_EOL.$this->getZflagsHelp());
    }

    protected function execute(InputInterface $input, OutputInterface $output)
    {
        $options = array_merge(
            $this->getDefaultOptions(),
            $this->sanitizeOptionsFromInput($input)
        );

        // TODO: Move all the stuff from the compiler
        $this->compiler->api($options);

        return 0;
    }

    protected function getDefaultOptions()
    {
        return [
            'path' => null,
            'output' => $this->config->get('path', 'api'),
            'options' => null,
            'url' => $this->config->get('base-url', 'api'),
        ];
    }

    protected function sanitizeOptionsFromInput(InputInterface $input)
    {
        $defaults = $this->getDefaultOptions();

        $options = array_filter($input->getOptions(), function ($v, $k) use ($defaults) {
            $allowedOpts = array_keys($defaults);

            return \in_array($k, $allowedOpts, true) && null !== $v;
        }, ARRAY_FILTER_USE_BOTH);

        foreach ($options as $option => $value) {
            // Prevent "" values
            if (empty($value) || !preg_match('/.+/', $value)) {
                throw new InvalidArgumentException(
                    sprintf('The "--%s" option requires a value.', $option)
                );
            }
        }

        return $options;
    }

    protected function createDefinition()
    {
        return new InputDefinition(
            [
                new InputOption(
                    'backend',
                    null,
                    InputOption::VALUE_REQUIRED,
                    'Used backend to generate HTML API for the extension',
                    'ZendEngine3'
                ),
                new InputOption(
                    'path',
                    'p',
                    InputOption::VALUE_REQUIRED,
                    'The API theme to be used'
                ),
                new InputOption(
                    'output',
                    'o',
                    InputOption::VALUE_REQUIRED,
                    'Output directory to generate theme'
                ),
                new InputOption(
                    'options',
                    null,
                    InputOption::VALUE_REQUIRED,
                    'Theme options'
                ),
                new InputOption(
                    'url',
                    null,
                    InputOption::VALUE_REQUIRED,
                    'The base URL to be used when generating links'
                ),
            ]
        );
    }
}
