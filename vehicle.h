/**
 * Header file for vehicle.c
 * @file vehicle.h
 * @author inesiscosta
*/

#ifndef VEHICLE
#define VEHICLE

/*Includes*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "parking_lot.h"
#include "utils.h"

/*Constants*/
#define INITIAL_HASH_TABLE_SIZE 128
#define LICENSE_PLATE_LENGTH 9
#define DATE_LENGTH 11
#define TIME_LENGTH 6

/*Vehicle Structure*/
typedef struct Vehicle {
    char license_plate[LICENSE_PLATE_LENGTH];
    char *name;
    char entry_date[DATE_LENGTH];
    char entry_time[TIME_LENGTH];
    char exit_date[DATE_LENGTH];
    char exit_time[TIME_LENGTH];
    float parking_fee;
    struct Vehicle *next;
} Vehicle;

/*Hash Table Structure*/
typedef struct HashTable {
    Vehicle **table;
    Vehicle **head;
    Vehicle **tail;
} HashTable;

/*Function Prototypes*/
void initialize_hash_table();
void free_hash_table();
int hash_function(char *license_plate);
int is_vehicle_parked(char *license_plate);
int is_vehicle_parked_here(char *name, char *license_plate);
void add_entry_to_hash_table(char *plate, char *name, char *entry_date, 
char *entry_time, char *latest_date, char *latest_time);
void register_vehicle_entry(char *command, char *latest_date, 
char *latest_time, char *first_date, char *first_date_set);
void update_exit_params(char *name, char *plate, char *exit_date, 
char *exit_time, char *entry_date, char *entry_time, float *parking_fee);
void register_vehicle_exit(char *command, char *latest_date, 
char *latest_time);
int compare_vehicle_entries(const void *vehicle_entry1, 
const void *vehicle_entry2);
void sort_entries(Vehicle *entries[], int num_entries);
void display_entries(Vehicle *entries[], int num_entries);
int count_entries_for_plate(char *plate);
void list_vehicle_entries_and_exits(char *command);
void find_and_store_billings(char *name, char *date, Vehicle billings[], 
int *num_billings);
void list_billings(char *command, char *latest_date, 
char *latest_time, char *first_date);
void sort_billings(Vehicle billings[], int num_billings);
void display_billings(Vehicle billings[], int num_billings);
void show_park_billings(char *name, char *date);
void show_daily_revenue_summary(char *name, char *latest_date, 
char *latest_time, char *first_date);
void remove_entries_for_parking_lot(char *name);
#endif