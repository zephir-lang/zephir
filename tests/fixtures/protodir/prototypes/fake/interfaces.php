<?php

namespace Fake\Http\Client;

interface ClientExceptionInterface extends \Throwable
{
}

interface NetworkExceptionInterface extends ClientExceptionInterface
{
    public function getRequest();
}
