FROM composer:latest AS composer
FROM php:8.0-fpm

RUN CPU_CORES="$(getconf _NPROCESSORS_ONLN)";
ENV MAKEFLAGS="-j${CPU_CORES}"

# php:8.0-fpm is Debian bullseye, which is now only served by archive.debian.org:
# deb.debian.org keeps its index but has pruned the packages it lists (404).
RUN sed -i 's|http://deb.debian.org|http://archive.debian.org|g' /etc/apt/sources.list

RUN apt update -y && apt install -y \
    wget \
    zip \
    git \
    apt-utils \
    sudo \
    libicu-dev \
    libgmp-dev \
    libzip-dev && \
    pecl install psr xdebug zephir_parser

RUN docker-php-ext-install zip gmp intl mysqli && \
    docker-php-ext-enable psr zephir_parser xdebug

COPY --from=composer /usr/bin/composer /usr/local/bin/composer
# Bash script with helper aliases
COPY ./.bashrc /root/.bashrc

CMD ["php-fpm"]
