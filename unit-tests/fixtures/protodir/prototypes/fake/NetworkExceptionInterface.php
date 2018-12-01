<?php

namespace Fake\Http\Client;

interface NetworkExceptionInterface extends ClientExceptionInterface
{
    public function getRequest();
}
