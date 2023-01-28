/*
 * Copyright 2022-2023 equals-forty-two.com All rights reserved.
 * License: https://equals-forty-two.com/LICENSE
 */
 
#pragma once

#include "wallet.h"

#include <framework/config.h>
#include <framework/string_table.h>
#include <framework/option.h>

#define HASH_REPORT static_hash_string("report", 6, 0xbaf4a5498a0604aaULL)

struct title_t;
struct table_t;

typedef uuid_t report_handle_t;

struct report_transaction_t
{
    time_t date{ 0 };
    bool buy{ false };
    double qty{ 0 };
    double price{ 0 };
    title_t* title;

    double acc{ 0 };
    double rated{ 0 };
    double adjusted{ 0 };

    int rx{ 0 };
    int ry{ 0 };
};

struct report_t
{
    string_table_symbol_t name;
    config_handle_t data;
    
    // Persistent state
    uuid_t id{ 0 };
    bool dirty{ false };
    bool save{ false };
    int save_index{ 0 };
    bool opened{ true };

    // Title information
    size_t active_titles{ 0 };
    title_t** titles{ nullptr };
    report_transaction_t* transactions{ nullptr };
    double transaction_max_acc{ 0 };
    double transaction_total_sells{ 0 };

    // Report summary values
    wallet_t* wallet{ nullptr };
    tick_t summary_last_update{ 0 };
    double total_gain { 0 };
    double total_gain_p { 0 };
    double total_value{ 0 };
    double total_investment { 0 };
    double total_day_gain { 0 };
    double total_daily_average_p{ 0 };
    tick_t fully_resolved{ 0 };

    // UI data
    table_t* table;
    bool show_summary{ false };
    bool show_sold_title{ false };
    bool show_add_title_ui{ false };
    bool show_rename_ui{ false };
    bool show_order_graph{ false };
};

/// <summary>
/// Allocate a new report. The newly allocated report will be released automatically when shutting down.
/// </summary>
report_handle_t report_allocate(const char* name, size_t name_length);

/// <summary>
/// Returns the amount of managed reports.
/// </summary>
size_t report_count();

/// <summary>
/// 
/// </summary>
report_t* report_get(report_handle_t report_handle);

/// <summary>
/// Return the report a given index
/// </summary>
/// <param name="index"></param>
report_t* report_get_at(unsigned int index);

/// <summary>
/// 
/// </summary>
/// <param name="path"></param>
report_handle_t report_load(string_const_t report_file_path);

/// <summary>
/// Loads a report from a json file stored in the user cache.
/// </summary>
report_handle_t report_load(const char* name, size_t name_length);

/// <summary>
/// Saves a report back in the user cache.
/// </summary>
void report_save(report_t* report);

/// <summary>
/// Renders the report as a table.
/// </summary>
void report_render(report_t* report);

/// <summary>
/// Show menu for a given report.
/// </summary>
void report_menu(report_t* report);

/// <summary>
/// Search for a given report by name.
/// </summary>
report_handle_t report_find(const char* name, size_t name_length);

/// <summary>
/// 
/// </summary>
report_handle_t report_find_no_case(const char* name, size_t name_length);

/// <summary>
/// 
/// </summary>
bool report_handle_is_valid(report_handle_t handle);

/// <summary>
/// 
/// </summary>
void report_render_create_dialog(bool* show_ui);

/// <summary>
/// Update the summary of a report.
/// </summary>
void report_summary_update(report_t* report);

/// <summary>
/// Refresh all titles in the report.
/// </summary>
bool report_refresh(report_t* report);

/// <summary>
/// Checks if the report is fully loaded (all titles have been refreshed)
/// </summary>
bool report_is_loading(report_t* report);

/// <summary>
/// Run a single pass on all titles to synchronize them.
/// </summary>
bool report_sync_titles(report_t* report);

/// <summary>
/// Sort reports based on their save index (i.e. how they are ordered with tabs)
/// </summary>
void report_sort_order();

/// <summary>
/// 
/// </summary>
bool report_render_dialog_begin(string_const_t name, bool* show_ui, unsigned int flags = /*ImGuiWindowFlags_NoSavedSettings*/1 << 8);

/// <summary>
/// 
/// </summary>
bool report_render_dialog_end(bool* show_ui = nullptr);

/// <summary>
/// 
/// </summary>
void report_graph_render(report_t* report);

/// <summary>
/// 
/// </summary>
void report_graph_show_transactions(report_t* report);
