/**
 * A program implementing a parking lot management system.
 * This file contains the main function of that program.
 * @file main.c
 * @author inesiscosta
 */

/** Includes */
#include "parking_lot.h"
#include "vehicle.h"
#include "utils.h"

/**
 * Main function, reads the commands (p, e, s, v, f, r, q) and
 * calls the appropriate functions.
 */
int main() {
    char first_date[DATE_LENGTH] = "01-01-0000";
    int first_date_set = 0;
    char latest_date[DATE_LENGTH] = "01-01-0000";
    char latest_time[TIME_LENGTH] = "00:00";
    initialize_hash_table();
    while (1) {
        char *command = malloc(BUFSIZ * sizeof(char));
        fgets(command, BUFSIZ, stdin);
        command = realloc(command, strlen(command) + 1);
        switch(command[0]) {
            case 'p':
                create_parking_lot(command);
                break;
            case 'e':
                register_vehicle_entry(command, latest_date, latest_time,
                first_date, (char *)&first_date_set);
                break;
            case 's':
                register_vehicle_exit(command, latest_date, latest_time);
                break;
            case 'v':
                list_vehicle_entries_and_exits(command);
                break;
            case 'f':
                list_billings(command, latest_date, latest_time, first_date);
                break;
            case 'r':
                remove_parking_lot(command);
                break;
            case 'q':
                cleanup(command);
                return 0;
        }
        free(command);
    }
}
