<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Zephir\Command;

use Symfony\Component\Console\Exception\InvalidArgumentException;
use Symfony\Component\Console\Input\InputInterface;
use Symfony\Component\Console\Input\InputOption;
use Symfony\Component\Console\Output\OutputInterface;

/**
 * Zephir\Command\ApiCommand
 *
 * Generates a HTML API based on the classes exposed in the extension.
 *
 * @package Zephir\Command
 */
class ApiCommand extends ContainerAwareCommand implements ZflagsAwareInterface
{
    use ZflagsAwareTrait;

    protected function configure()
    {
        $this
            ->setName('api')
            ->setDescription('Generates a HTML API based on the classes exposed in the extension')
            ->addOption(
                'backend',
                null,
                InputOption::VALUE_REQUIRED,
                'Used backend to generate HTML API for the extension',
                'ZendEngine3'
            )
            ->addOption(
                'path',
                'p',
                InputOption::VALUE_REQUIRED,
                'The API theme to be used'
            )
            ->addOption(
                'output',
                'o',
                InputOption::VALUE_REQUIRED,
                'Output directory to generate theme'
            )
            ->addOption(
                'options',
                null,
                InputOption::VALUE_REQUIRED,
                'Theme options'
            )
            ->addOption(
                'url',
                null,
                InputOption::VALUE_REQUIRED,
                'The base URL to be used when generating links'
            )
            ->setHelp($this->getZflagsHelp());
    }

    protected function execute(InputInterface $input, OutputInterface $output)
    {
        $defaults = [
            'path' => null,
            'output' => $this->config->get('path', 'api'),
            'options' => null,
            'url' => $this->config->get('base-url', 'api'),
        ];

        $options = array_filter($input->getOptions(), function ($v, $k) use ($defaults) {
            $allowedOpts = array_keys($defaults);

            return in_array($k, $allowedOpts, true) && $v !== null;
        }, ARRAY_FILTER_USE_BOTH);

        foreach ($options as $option => $value) {
            // Prevent "" values
            if (empty($value) || !preg_match('/.+/', $value)) {
                throw new InvalidArgumentException(
                    sprintf('The "--%s" option requires a value.', $option)
                );
            }
        }

        $options = array_merge($defaults, $options);

        // TODO: Move all the stuff from the compiler
        $this->compiler->api($options);

        return 0;
    }
}
