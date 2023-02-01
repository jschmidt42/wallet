/*
 * Copyright 2022-2023 Wiimag Inc. All rights reserved.
 * License: https://wiimag.com/LICENSE
 */
 
#pragma once

#include <foundation/string.h>

struct settings_t
{
    // UI settings
    int current_tab{ 0 };
    float font_scaling{ 1.0f };

    // Preferences
    bool show_symbols_TO{ false };
    bool show_symbols_US{ false };
    bool show_symbols_CVE{ false };
    bool show_symbols_NEO{ false };
    bool show_symbols_INDX{ false };
    bool show_bulk_eod{ false };

    char preferred_currency[32] { '\0' };
    char search_terms[256]{ '\0' };
    char search_filter[256]{ '\0' };

    double good_dividends_ratio{ 0.04 };

    // Dialog toggles
    bool show_create_report_ui{ false };
};

void settings_draw();

void settings_initialize();
void settings_shutdown();

extern settings_t SETTINGS;
