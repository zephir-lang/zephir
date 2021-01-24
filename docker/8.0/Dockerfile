FROM composer:latest as composer
FROM php:8.0-fpm

RUN apt update -y && apt install -y \
    wget \
    zip \
    git \
    apt-utils \
    sudo \
    libgmp-dev \
    libzip-dev && \
    pecl install psr

RUN docker-php-ext-install zip gmp && \
    docker-php-ext-enable psr

COPY --from=composer /usr/bin/composer /usr/local/bin/composer

CMD ["php-fpm"]
