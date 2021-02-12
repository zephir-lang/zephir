FROM composer:2.0.8 as composer
FROM php:8.0-fpm

RUN CPU_CORES="$(getconf _NPROCESSORS_ONLN)";
ENV MAKEFLAGS="-j${CPU_CORES}"

RUN apt update -y && apt install -y \
    wget \
    zip \
    git \
    apt-utils \
    sudo \
    libicu-dev \
    libgmp-dev \
    libzip-dev && \
    pecl install psr

RUN docker-php-ext-install zip gmp intl && \
    docker-php-ext-enable psr

COPY --from=composer /usr/bin/composer /usr/local/bin/composer

CMD ["php-fpm"]
