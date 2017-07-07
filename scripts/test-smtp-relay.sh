#!/bin/bash

docker=$(whereis docker | awk '{print $2}')

if [[ -z $docker ]];
then
    echo -e "\033[0;31mError: docker isn't available on your machine.\e[0m";
    exit 1
fi

GMAIL_LOGIN=$1
GMAIL_PASSWORD=$2

if [[ -z $GMAIL_LOGIN ]];
then
    echo -e "\033[0;33mYou should pas you GMAIL email as the first parameter.\e[0m"
    exit 1
fi

if [[ -z $GMAIL_PASSWORD ]];
then
    echo -e "\033[0;33mYou should pas you GMAIL email as the second parameter.\e[0m"
    exit 1
fi

#
# check if we have already launched container
#
docker inspect exim-gmail-relay 2> /dev/null 1> /dev/null

if [[ "$?" == 0 ]];
then
    docker stop exim-gmail-relay
    docker rm   exim-gmail-relay
fi

docker run --name exim-gmail-relay              \
    -e EXIM_GMAIL_LOGIN=$GMAIL_LOGIN            \
    -e EXIM_GMAIL_PASSWORD=$GMAIL_PASSWORD      \
    -d selim13/exim-gmail-relay:latest

exit 0
