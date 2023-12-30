/*
 * BGT60.cpp
 *
 * Simple 'driver'/access class for the Infineon BGT60LTR11 Radar Shield2Go
 *    
 * Copyright (c) 2023 dxcfl
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>

#include "BGT60.h"

#define TD_NODE DT_ALIAS(targetd)
#define PD_NODE DT_ALIAS(phased)
static const struct gpio_dt_spec td = GPIO_DT_SPEC_GET(TD_NODE, gpios);
static const struct gpio_dt_spec pd = GPIO_DT_SPEC_GET(PD_NODE, gpios);

static bool ready = false;
static uint64_t last_td_time = 0;
static bool last_pd = false;

static struct gpio_callback td_cb_data;

void td_activated(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    uint64_t now = k_uptime_get();
    bool pd_pin = gpio_pin_get_dt(&pd);
    last_td_time = now;
    last_pd = pd_pin;
}

void BGT60::Init()
{
    int ret;
    
    printk("BGT60: Init ...\n\r");

    if (!device_is_ready(td.port))
    {
        printk("BGT60: Target detection port (TD) not available ...\n\r");
        return;
    }

    if (!device_is_ready(pd.port))
    {
        printk("BGT60: Phase detection port (PD) not available ...\n\r");
        return;
    }

    ret = gpio_pin_configure_dt(&td, GPIO_INPUT);
    if (ret < 0)
    {
        printk("BGT60: Target detection port (TD) not available for input ...\n\r");
        return;
    }

    ret = gpio_pin_configure_dt(&pd, GPIO_INPUT);
    if (ret < 0)
    {
        printk("BGT60: Phase detection port (PD) not available for input ...\n\r");
        return;
    }

    ret = gpio_pin_interrupt_configure_dt(&td, GPIO_INT_EDGE_TO_ACTIVE);
    if (ret < 0)
    {
        printk("BGT60: Could not configure ISR for TD ...\n\r");
        return;
    }

    gpio_init_callback(&td_cb_data, td_activated, BIT(td.pin));
    gpio_add_callback(td.port, &td_cb_data);

    ready = true;
}

bool BGT60::IsReady()
{
    return ready;
}

uint64_t BGT60::GetLastTdTime()
{
    return last_td_time;
}

bool BGT60::GetLastPd()
{
    return last_pd;
}

bool BGT60::GetTd() {
    return gpio_pin_get_dt(&td);
}

bool BGT60::GetPd() {
    return gpio_pin_get_dt(&pd);
}