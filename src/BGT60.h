/*
 * BGT60.h
 *
 * Simple 'driver'/access class for the Infineon BGT60LTR11 Radar Shield2Go
 *    
 * Copyright (c) 2023 dxcfl
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <zephyr/kernel.h>

class BGT60
{

public:
    static BGT60 &Instance()
    {
        static BGT60 instance;
        return instance;
    };

    static void Init();
    static bool IsReady();
    static uint64_t GetLastTdTime();
    static bool GetLastPd();
    static bool GetTd();
    static bool GetPd();
};