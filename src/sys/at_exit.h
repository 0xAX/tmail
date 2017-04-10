/*
 * at_exit.h - at exit callback registration.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#pragma once

int register_exit_cb(void (*cb)(void));
