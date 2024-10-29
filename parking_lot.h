/**
 * Header file for parking_lot.c
 * @file parking_lot.h
 * @author @inesiscosta
*/

#ifndef PARKING
#define PARKING

/*Includes*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "utils.h"
#include "vehicle.h"

#define MAX_PARKS 20

/*parking lot structure*/
typedef struct ParkingLot {
    char *name;
    int capacity;
    float quarter_hourly_rate;
    float quarter_hourly_rate_after_first_hour;
    float max_daily_cost;
    int available_spaces;
} ParkingLot;


/*Function Prototypes*/
void cleanup(char *command);
ParkingLot *find_park_by_name(char *name);
void list_parking_lots();
int park_is_full(char *name);
float calculate_parking_fee(ParkingLot *parking_lot, char *entry_date,
char *entry_time, char *exit_date, char *exit_time);
int is_valid_parking_lot(char *name, int capacity,
float quarter_hourly_rate, float quarter_hourly_rate_after_first_hour, 
float max_daily_cost);
void create_parking_lot(char *command);
void update_available_spaces_after_entry(char *name, int *avalable_spaces);
void manage_parking_lot_after_exit(char *name, float *parking_fee,
char *entry_date, char *entry_time, char *exit_date, char *exit_time);
int find_parking_lot_index(char *name);
void sort_parks(char remaining_parks[][BUFSIZ], int num_parks);
void print_remaining_parks();
void remove_parking_lot_entry(int index);
#endif
