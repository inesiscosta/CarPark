/**
 * Header file with function prototypes for utils.c
 * @file utils.h
 * @author @inesiscosta
*/

#ifndef UTILS
#define UTILS

/*Includes*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "vehicle.h"
#include "parking_lot.h"

/*Function Prototypes*/
void breakdown_datetime(char *date, char *time, int *day, int *month, 
int *year, int *hours, int *minutes);
int compare_datetime(char *date1, char *time1, char *date2, char *time2);
void update_first_date(char *entry_date, char *first_date, 
char *first_date_set);
int park_exists(char *name);
int is_valid_license_plate(char *plate);
int is_valid_datetime(char *date, char *time, char *latest_date, 
char *latest_time);
int is_valid_entry(char *name, char *plate, char *entry_date, 
char *entry_time, char *latest_date, char *latest_time);
int is_valid_exit(char *name, char *plate, char *exit_date, char *exit_time, 
char *latest_date, char *latest_time);
void format_time(char *time);
void validate_and_call_show_park_billings(char *name, char *date, 
char *latest_date, char *latest_time);
void validate_and_call_show_daily_revenue_summary(char *name,
char *latest_date, char *latest_time, char *first_date);
int month_days_before(int month);
int calculate_total_minutes(char *date, char *time);
void increment_date(char *date);
void remove_parking_lot(char *command);
#endif