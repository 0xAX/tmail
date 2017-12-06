#!/bin/bash

PREFIX="${HOME}/opt"

wget -P download-cache/ "https://www.openssl.org/source/openssl-1.1.0g.tar.gz"
tar zxf "download-cache/openssl-1.1.0g.tar.gz"
cd openssl-1.1.0g
./config shared --prefix="${PREFIX}" --openssldir="${PREFIX}" -DPURIFY && make -j4 all install_sw

