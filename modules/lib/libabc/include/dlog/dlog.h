/*
 * Copyright 2019 saehie.park@gmail.com
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __LIBABC_DLOG_H__
#define __LIBABC_DLOG_H__

#include <stdio.h>

#ifdef DEBUG
#define DDEBUG 1
#else
#define DDEBUG 0
#endif

#define DPRINT(fmt, ...) do { if (DDEBUG) fprintf(stderr, fmt, __VA_ARGS__); } while (0)
#define DLOG(msg) DPRINT("%s\r\n", msg)

#endif // __LIBABC_DLOG_H__
