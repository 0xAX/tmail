#!/bin/bash

PREFIX="${HOME}/opt"

wget -P download-cache/ "https://www.openssl.org/source/openssl-1.1.0g.tar.gz"
tar zxf "download-cache/openssl-1.1.0g.tar.gz"
cd openssl-1.1.0g
./config shared --prefix="${PREFIX}" --openssldir="${PREFIX}" -DPURIFY && make all install_sw

