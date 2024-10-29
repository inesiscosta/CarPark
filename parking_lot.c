/**
 * This file contains the implementation of the functions that use the 
 * ParkingLot structure for the parking lot management system.
 * @file parking_lot.c
 * @author @inesiscosta
 */

/** Include */
#include "parking_lot.h"


ParkingLot parking_lots[MAX_PARKS]; // Array of ParkingLots
int num_parks = 0; // Current number of parking lots in the system

/**
 * Frees the memory allocated for command, hash_table and parking lot names.
 * @param command the command to be freed
 */
void cleanup(char *command) {
    free(command);
    for (int i = 0; i < num_parks; i++)
        free(parking_lots[i].name);
    free_hash_table();
}

/**
 * Searches within the array parking_lots for a ParkingLot
 * with the name provided.
 * @param name the name of the parking_lot
 * @return ParkingLot* if park with the given name is found, otherwise NULL
 */
ParkingLot *find_park_by_name(char *name) {
    for (int i = 0; i < num_parks; i++)
        if (strcmp(parking_lots[i].name, name) == 0)
            return &parking_lots[i];
    return NULL;
}

/**
 * Lists the parking lots.
 */
void list_parking_lots() {
    for (int i = 0; i < num_parks; i++) {
        printf("%s %d %d\n", parking_lots[i].name, 
        parking_lots[i].capacity, parking_lots[i].available_spaces);
    }
}

/**
 * Checks whether the given park is full (doesn't have empty spaces).
 * @param name the name of the parking lot
 * @return 1 if park exists and is full, 0 otherwise 
 */
int park_is_full(char *name) {
    ParkingLot *parking_lot = find_park_by_name(name);
    return (parking_lot != NULL && parking_lot->available_spaces == 0);
}

/**
 * Calculates the amount of money made due to a vehicle's stay in the park
 * @param parking_lot the name of the parking lot the vehicle parked in
 * @param entry_date the date the vehicle entered the parking lot
 * @param entry_time the time the vehicle entered the parking lot
 * @param exit_date the date the vehicle exited the parking lot
 * @param exit_time the time the vehicle exited the parking lot
 * @return return the amount of money made by the parking lot with this
 * entry/exit pair
 */
float calculate_parking_fee(ParkingLot *parking_lot, char *entry_date,
char *entry_time, char *exit_date, char *exit_time) {
    int entry_total_minutes = calculate_total_minutes(entry_date, entry_time);
    int exit_total_minutes = calculate_total_minutes(exit_date, exit_time);
    int total_minutes_difference = exit_total_minutes - entry_total_minutes;
    float fee = 0.0;
    int num_24_hour_periods = total_minutes_difference / (24 * 60);
    int remaining_minutes = total_minutes_difference % (24 * 60);
    fee += num_24_hour_periods * parking_lot->max_daily_cost;
    /* 16 mins counts as 2 * 15 min slots and division always rounds down,
    so we add 14 minutes to make sure we round up*/
    int num_quarter_hour_slots = (remaining_minutes + 14) / 15;
    if (num_quarter_hour_slots <= 4) {
        fee += num_quarter_hour_slots * parking_lot->quarter_hourly_rate;
    } else {
        // Applying the first hour "discount"
        fee += 4 * parking_lot->quarter_hourly_rate;
        remaining_minutes -= 60;
        // Same as before with the 14 min rounding up
        fee += ((remaining_minutes + 14) / 15) * 
        parking_lot->quarter_hourly_rate_after_first_hour;
    } 
    if (fee > parking_lot->max_daily_cost * (num_24_hour_periods + 1))
        fee = parking_lot->max_daily_cost * (num_24_hour_periods + 1);
    return fee;
}

/**
 * Checks whether a parking lot is valid.
 * @param name the name of the parking lot
 * @param capacity tha parking lots maximum capacity
 * @param quarter_hourly_rate the quarter hourly rate for the first hour of
 * parking each day
 * @param quarter_hourly_rate_after_first_hour the quarter hourly rate for
 * the subsequent hours of the day
 * @param max_daily_cost the max daily cost of the park
 * @return 0 if park is invalid, 1 otherwise 
 */
int is_valid_parking_lot(char *name, int capacity,
float quarter_hourly_rate, float quarter_hourly_rate_after_first_hour, 
float max_daily_cost) {
    for (int i = 0; i < num_parks; i++)
        if (strcmp(parking_lots[i].name, name) == 0) {
            printf("%s: parking already exists.\n", name);
            return 0;
        }
    if (num_parks >= MAX_PARKS) {
        printf("too many parks.\n");
        return 0;
    }
    if (capacity <= 0) {
        printf("%d: invalid capacity.\n", capacity);
        return 0;
    }
    if (quarter_hourly_rate <= 0 || 
        quarter_hourly_rate_after_first_hour <= quarter_hourly_rate || 
        max_daily_cost <= quarter_hourly_rate_after_first_hour) {
        printf("invalid cost.\n");
        return 0;
    }
    return 1;
}

/**
 * Creates a parking lot object and adds it to the array of parking lots.
 * Or lists parking lots if enough arguments can't be extracted from command.
 * @param command command p in form "p <name> <capacity> <quarter_hourly_rate>
 * <quarter_hourly_rate_after_first_hour> <max_daily_cost>" or "p".
 */
void create_parking_lot(char *command) {
    char *name = (char *)malloc(BUFSIZ * sizeof(char));
    int capacity;
    float quarter_hourly_rate, quarter_hourly_rate_after_first_hour, 
    max_daily_cost;
    if (sscanf(command, "p \"%[^\"]\" %d %f %f %f", name, &capacity, 
    &quarter_hourly_rate, &quarter_hourly_rate_after_first_hour, 
    &max_daily_cost) != 5 
    && sscanf(command, "p %s %d %f %f %f", name, &capacity, 
    &quarter_hourly_rate, &quarter_hourly_rate_after_first_hour, 
    &max_daily_cost) != 5) {
        // If not enough arguments are provided, list parking lots
        list_parking_lots();
        free(name);
        return;
    }
    name = realloc(name, strlen(name) +1);
    if (is_valid_parking_lot(name, capacity, quarter_hourly_rate, 
    quarter_hourly_rate_after_first_hour, max_daily_cost)) {
        parking_lots[num_parks].name = strdup(name); 
        parking_lots[num_parks].capacity = capacity;
        parking_lots[num_parks].quarter_hourly_rate = quarter_hourly_rate;
        parking_lots[num_parks].quarter_hourly_rate_after_first_hour = 
        quarter_hourly_rate_after_first_hour;
        parking_lots[num_parks].max_daily_cost = max_daily_cost;
        parking_lots[num_parks].available_spaces = capacity;
        num_parks++;
    }
    free(name);
}

/**
 * Updates the number of available spaces in a ParkingLot after a vehicle's
 * entry.
 * @param name the parking lot's name
 * @param available_spaces the number of available spaces in the parking lot
 */
void update_available_spaces_after_entry(char *name, int *avalable_spaces) {
    ParkingLot *parking_lot = find_park_by_name(name);
    parking_lot->available_spaces--;
    *avalable_spaces = parking_lot->available_spaces;
}

/**
 * Updates the number of available spaces in a parking lot after a vehicle's
 * exit and calulates the parking fee.
 * @param name the parking lot's name
 * @param available_spaces the number of available spaces in the parking lot
 */
void manage_parking_lot_after_exit(char *name, float *parking_fee,
char *entry_date, char *entry_time, char *exit_date, char *exit_time) {
    ParkingLot *parking_lot = find_park_by_name(name);
    parking_lot->available_spaces++;
    *parking_fee = calculate_parking_fee(parking_lot,
    entry_date, entry_time, exit_date, exit_time);
}

/**
 * Finds the index of a parking lot by its name.
 * @param name the parking lot's name
 * @return the index of the parking lot in the array of parking lots or
 * -1 if park is not found
 */
int find_parking_lot_index(char *name) {
    for (int i = 0; i < num_parks; i++)
        if (strcmp(parking_lots[i].name, name) == 0)
            return i;
    return -1;
}

/**
 * Sorts the parking lots by name using Insertion Sort.
 * @param remaining_parks the array of parking lot names
 * @param num_parks the number of parking lots in the array
 */
void sort_parks(char remaining_parks[][BUFSIZ], int num_parks) {
    // Iterate through the array starting at second element
    for (int i = 1; i < num_parks; i++) {
        char key[BUFSIZ];
        strcpy(key, remaining_parks[i]);
        int j = i - 1;
        // Move elements of remaining_parks that are greater than key
        while (j >= 0 && strcmp(remaining_parks[j], key) > 0) {
            strcpy(remaining_parks[j + 1], remaining_parks[j]);
            j = j - 1;
        }
        // Place key at after the element that is smaller than it
        strcpy(remaining_parks[j + 1], key);
    }
}

/**
 * Prints the remaining parking lots after a removal in a sorted order.
 */
void print_remaining_parks() {
    char remaining_parks[MAX_PARKS][BUFSIZ];
    for (int i = 0; i < num_parks; i++)
        strcpy(remaining_parks[i], parking_lots[i].name);
    sort_parks(remaining_parks, num_parks);
    for (int i = 0; i < num_parks; i++)
        printf("%s\n", remaining_parks[i]);
}

/**
 * Removes a parking lot entry from the array of parking lots.
 * @param index the index of the parking lot to be removed
 */
void remove_parking_lot_entry(int index) {
    remove_entries_for_parking_lot(parking_lots[index].name);
    free(parking_lots[index].name);
    for (int i = index; i < num_parks - 1; i++)
        parking_lots[i] = parking_lots[i+1];
    num_parks--;
}
