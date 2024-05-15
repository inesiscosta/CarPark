/**
 * File containing the functions which deal with the vehicle struct for the
 * parking lot management system.
 * @file vehicle.c
 * @author inesiscosta
 */

#include "vehicle.h"

HashTable vehicle_hash_table;

/**
 * Initializes the hash table.
 */
void initialize_hash_table() {
    vehicle_hash_table.table = (Vehicle **)
    malloc(INITIAL_HASH_TABLE_SIZE * sizeof(Vehicle *));
    vehicle_hash_table.head = (Vehicle **)
    malloc(INITIAL_HASH_TABLE_SIZE * sizeof(Vehicle *));
    vehicle_hash_table.tail = (Vehicle **)
    malloc(INITIAL_HASH_TABLE_SIZE * sizeof(Vehicle *));
    for (int i = 0; i < INITIAL_HASH_TABLE_SIZE; i++) {
        vehicle_hash_table.table[i] = NULL;
        vehicle_hash_table.head[i] = NULL;
        vehicle_hash_table.tail[i] = NULL;
    }
}

/**
 * Frees the hash table.
 */
void free_hash_table() {
    for (int i = 0; i < INITIAL_HASH_TABLE_SIZE; i++) {
        Vehicle *current = vehicle_hash_table.table[i];
        while (current != NULL) {
            Vehicle *temp = current;
            current = current->next;
            free(temp->name);
            free(temp);
        }
    }
    free(vehicle_hash_table.table);
    free(vehicle_hash_table.tail);
    free(vehicle_hash_table.head);
}

/** 
 * A function which given a vehicle's license plate determines where the 
 * vehicle will be/is stored in the hash table.
 * @param license_plate the vehicle license plate
 * @return hash_index the hash table index to which the vehicle with the given
 * license plate will be / is stored
 */
int hash_function(char *license_plate) {
    int hash_index, a = 31415, b = 27183;
    for (hash_index = 0; *license_plate != '\0'; license_plate++, a = a * b % 
    (INITIAL_HASH_TABLE_SIZE - 1))
        hash_index = (a * hash_index + *license_plate) 
        % INITIAL_HASH_TABLE_SIZE;
    return hash_index;
}

/**
 * Checks whether a vehicle is parked in any park.
 * @param license_plate the vehicle's license plate
 * @return 1 if the vehicles is parked, 0 otherwise 
 */
int is_vehicle_parked(char *license_plate) {
    int hash_index = hash_function(license_plate);
    Vehicle *current = vehicle_hash_table.table[hash_index];
    while (current != NULL) {
        if (strcmp(current->license_plate, license_plate) == 0 && 
        current->exit_date[0] == '\0' && current->exit_time[0] == '\0') {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

/**
 * Checks wether a vehicle is parked in a given park.
 * @param name the parking lot's name
 * @param license_plate the vehicle's license plate
 * @return 1 if the vehicle is parked in that parking lot, 0 otherwise 
 */
int is_vehicle_parked_here(char *name, char *license_plate) {
    int hash_index = hash_function(license_plate);
    Vehicle *current = vehicle_hash_table.table[hash_index];
    while (current != NULL) {
        if (strcmp(current->license_plate, license_plate) == 0 && 
        strcmp(current->name, name) == 0 
        && current->exit_date[0] == '\0' && current->exit_time[0] == '\0') {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

/**
 * Adds a vehicle's entry parameters to the hash_table
 * @param plate the vehicle's plate
 * @param name the parking lot's name
 * @param entry_date the vehicle's entry date into said parking lot
 * @param entry_time the vehicle's time of entry into said parking lot
 * @param latest_date the latest date entered into the system 
 * @param latest_time the latest time entered into the system
 */
void add_entry_to_hash_table(char *plate, char *name, char *entry_date, 
char *entry_time, char *latest_date, char *latest_time) {
    int hash_index = hash_function(plate); 
    Vehicle *new_vehicle = (Vehicle*) malloc(sizeof(Vehicle));
    strcpy(new_vehicle->license_plate, plate);
    new_vehicle->name = strdup(name);
    strcpy(new_vehicle->entry_date, entry_date);
    format_time(entry_time);
    strcpy(new_vehicle->entry_time, entry_time);
    strcpy(latest_date, entry_date);
    strcpy(latest_time, entry_time);
    //Empty strings to indicate that the vehicle has not exited the parking lot
    strcpy(new_vehicle->exit_date, "");
    strcpy(new_vehicle->exit_time, ""); 
    new_vehicle->next = NULL;
    if (vehicle_hash_table.table[hash_index] == NULL) {
        vehicle_hash_table.table[hash_index] = new_vehicle;
        vehicle_hash_table.head[hash_index] = new_vehicle;
        vehicle_hash_table.tail[hash_index] = new_vehicle;
    } else {
        vehicle_hash_table.tail[hash_index]->next = new_vehicle;
        vehicle_hash_table.tail[hash_index] = new_vehicle;
    }
}

/**
 * Registers a vehicle's entry and print's out the parking lot's name and
 * remaning available spaces
 * @param command command in format"e <name> <plate> <entry_date> <entry_time>"
 * @param latest_date the latest date registered in the system
 * @param latest_time the latest time registered in the system
 * @param first_date the earliest date in the system
 * @param first_date_set a flag indicating if the first date has been set
 */
void register_vehicle_entry(char *command, char *latest_date, 
char *latest_time, char *first_date, char *first_date_set) {
    char *name = (char *)malloc(BUFSIZ * sizeof(char));
    char plate[LICENSE_PLATE_LENGTH], entry_date[DATE_LENGTH], 
    entry_time[TIME_LENGTH];
    if (sscanf(command, "e \"%[^\"]\" %10s %s %s", name, plate, entry_date,
    entry_time) == 4 || sscanf(command, "e %s %10s %s %s", name, plate, 
    entry_date, entry_time) == 4) {
        name = realloc(name, strlen(name) + 1);
        if (is_valid_entry(name, plate, entry_date, entry_time, latest_date, 
        latest_time)) {
            int available_spaces = 0;
            update_available_spaces_after_entry(name, &available_spaces);
            add_entry_to_hash_table(plate, name, entry_date, entry_time, 
            latest_date, latest_time);
            update_first_date(entry_date, first_date, first_date_set);
            printf("%s %d\n", name, available_spaces);
        }
    }
    free(name);
}

/**
 * Updates the exit date and time in the hash table and calculates and
 * also updates the parking fee.
 * @param name the parking lot's name
 * @param plate the vehicle's plate
 * @param exit_date the vehicle's exit date
 * @param exit_time the vehicle's exit time
 * @param entry_date the vehicle's entry date
 * @param entry_time the vehicle's entry time
 * @param parking_fee the parking fee of the vehicle
 */
void update_exit_params(char *name, char *plate, char *exit_date, 
char *exit_time, char *entry_date, char *entry_time, float *parking_fee) {
int hash_index = hash_function(plate);
    Vehicle *current = vehicle_hash_table.table[hash_index];
    while (current != NULL) {
        if (strcmp(current->license_plate, plate) == 0 && 
            strcmp(current->name, name) == 0 && 
            current->exit_date[0] == '\0' && 
            current->exit_time[0] == '\0') {
            format_time(exit_time); //HH:MM
            strcpy(current->exit_date, exit_date);
            strcpy(current->exit_time, exit_time);
            strcpy(entry_date, current->entry_date);
            strcpy(entry_time, current->entry_time);
            //Update available space + calculate parking fee
            manage_parking_lot_after_exit(name, parking_fee, entry_date,
                                          entry_time, exit_date, exit_time);
            current->parking_fee = *parking_fee;
            break;
        }
        current = current->next;
    }
}

/**
 * Registers a vehicle's exit and prints out the vehicle's plate, entry date,
 * entry time, exit date, exit time and parking fee.
 * @param command command in format "s <name> <plate> <exit_date> <exit_time>"
 * @param latest_date the latest date registered in the system
 * @param latest_time the latest time registered in the system
 */
void register_vehicle_exit(char *command, char *latest_date, 
char *latest_time) {
    char *name = (char *)malloc(BUFSIZ * sizeof(char));
    char plate[LICENSE_PLATE_LENGTH], exit_date[DATE_LENGTH], 
    exit_time[TIME_LENGTH];
    float parking_fee = 0.0;
    if (sscanf(command, "s \"%[^\"]\" %10s %s %s", name, plate, exit_date, 
    exit_time) == 4 || sscanf(command, "s %s %10s %s %s", name, plate, 
    exit_date, exit_time) == 4) {
        name = realloc(name, strlen(name) + 1);
        if (is_valid_exit(name, plate, exit_date, exit_time, latest_date, 
        latest_time)) {
            char entry_date[DATE_LENGTH], entry_time[TIME_LENGTH];
            update_exit_params(name, plate, exit_date, exit_time, 
            entry_date, entry_time, &parking_fee);
            printf("%s %s %s %s %s %.2f\n", plate, entry_date, 
                entry_time, exit_date, exit_time, parking_fee);
            strcpy(latest_date, exit_date);
            strcpy(latest_time, exit_time); 
        }
    }
    free(name);
}

/**
 * Compares two vehicle entries by their name, entry date and entry time.
 * @param vehicle_entry1 the first vehicle entry
 * @param vehicle_entry2 the second vehicle entry
 * @return 0 if the entries are equal, 
 * a positive integer if vehicle_entry1 > vehicle_entry2,
 * a negative integer if vehicle_entry1 < vehicle_entry2
 */
int compare_vehicle_entries(const void *vehicle_entry1, 
const void *vehicle_entry2) {
    Vehicle *entry1 = *(Vehicle **)vehicle_entry1;
    Vehicle *entry2 = *(Vehicle **)vehicle_entry2;
    int comparison_result = strcmp(entry1->name, entry2->name);
    if (comparison_result != 0)
        return comparison_result;
    comparison_result = compare_datetime(entry1->entry_date,
    entry1->entry_time, entry2->entry_date, entry2->entry_time);
    return comparison_result;
}

/**
 * Sorts the billings array by license plate using Insertion Sort.
 * @param billings the array of billings
 * @param num_billings the number of billings in the array
 */
void sort_entries(Vehicle *entries[], int num_entries) {
    for (int i = 1; i < num_entries; i++) {
        Vehicle *key = entries[i];
        int j = i - 1;
        while (j >= 0 && compare_vehicle_entries(&entries[j], &key) > 0) {
            entries[j + 1] = entries[j];
            j = j - 1;
        }
        entries[j + 1] = key;
    }
}

/**
 * Displays the vehicle entries in format:
 * <name> <entry_date> <entry_time> <exit_date> <exit_time>.
 * @param entries the array of vehicle entries
 * @param num_entries the number of vehicle entries in the array
 */
void display_entries(Vehicle *entries[], int num_entries) {
    for (int i = 0; i < num_entries; i++) {
        if (strlen(entries[i]->exit_date) == 0 && 
        strlen(entries[i]->exit_time) == 0) {
            printf("%s %s %s\n", entries[i]->name, entries[i]->entry_date, 
            entries[i]->entry_time);
        } else {
            printf("%s %s %.5s %s %.5s\n", entries[i]->name, 
            entries[i]->entry_date, entries[i]->entry_time, 
            entries[i]->exit_date, entries[i]->exit_time);
        }
    }
}

/**
 * Counts the number of entries for a given vehicle by their license plate.
 * @param plate the license plate
 * @return the number of entries for the given license plate
 */
int count_entries_for_plate(char *plate) {
    int hash_index = hash_function(plate);
    Vehicle *current = vehicle_hash_table.table[hash_index];
    int num_entries = 0;
    while (current != NULL) {
        if (strcmp(current->license_plate, plate) == 0) {
            num_entries++;
        }
        current = current->next;
    }
    return num_entries;
}

/**
 * Lists the vehicle entries and exits for a given license plate.
 * Or prints a descriptive error message if the license plate is invalid or no
 * entries are found.
 * @param command command in format "v <plate>"
 */
void list_vehicle_entries_and_exits(char *command) {
    char plate[LICENSE_PLATE_LENGTH];
    sscanf(command, "v %s", plate);
    if (!is_valid_license_plate(plate)) {
        printf("%s: invalid licence plate.\n", plate);
        return;
    }
    int num_entries = count_entries_for_plate(plate);
    if (num_entries == 0) {
        printf("%s: no entries found in any parking.\n", plate);
        return;
    }
    Vehicle *entries[num_entries];
    int hash_index = hash_function(plate);
    Vehicle *current = vehicle_hash_table.table[hash_index];
    int index = 0;
    while (current != NULL) {
        if (strcmp(current->license_plate, plate) == 0) {
            entries[index++] = current;
        }
        current = current->next;
    }
    sort_entries(entries, num_entries);
    display_entries(entries, num_entries);
}

/**
 * Finds and stores the billings for a given parking lot's name and date.
 * @param name the parking lot's name
 * @param date the date for which the billings are to be found
 * @param billings array of Vehicles to store the billings
 * @param num_billings number of billing entries found
 * @param parking_lot 
 */
void find_and_store_billings(char *name, char *date, Vehicle billings[], 
int *num_billings) {
    for (int i = 0; i < INITIAL_HASH_TABLE_SIZE; i++) {
        Vehicle *current = vehicle_hash_table.table[i];
        while (current != NULL) {
            if (strcmp(current->name, name) == 0 && 
            strcmp(current->exit_date, date) == 0) {
                strcpy(billings[*num_billings].license_plate, 
                current->license_plate);
                strcpy(billings[*num_billings].exit_time, current->exit_time);
                billings[*num_billings].parking_fee = current->parking_fee;
                (*num_billings)++;
            }
            current = current->next;
        }
    }
}

/**
 * Extracts arguments from command f and determines which function to call to 
 * validate and list billings depending on the number of arguments extracted.
 * @param command command in format "f <name> <date>" or "f <name>"
 * @param latest_date the latest date registered in the system
 * @param latest_time the latest time registered in the system
 * @param first_date the earliest date registered in the system
 */
void list_billings(char *command, char *latest_date, 
char *latest_time, char *first_date) {
    char date[DATE_LENGTH];
    char *name = (char *)malloc(BUFSIZ * sizeof(char));
    if (sscanf(command, "f \"%[^\"]\" %s", name, date) == 2) {
        validate_and_call_show_park_billings(name, date, latest_date,
        latest_time);
    } else if (sscanf(command, "f \"%[^\"]\"", name) == 1) {
        validate_and_call_show_daily_revenue_summary(name, latest_date,
        latest_time, first_date);
    } else if (sscanf(command, "f %s %s", name, date) == 2) {
        validate_and_call_show_park_billings(name, date, latest_date,
        latest_time);
    } else if (sscanf(command, "f %s", name) == 1) {
        validate_and_call_show_daily_revenue_summary(name, latest_date,
        latest_time, first_date);
    }
}

/**
 * An insertion sort algorithm to sort the billings array by exit time.
 */
void sort_billings(Vehicle billings[], int num_billings) {
    for (int i = 1; i < num_billings; i++) {
        Vehicle key = billings[i];
        int j = i - 1;
        while (j >= 0 && strcmp(billings[j].exit_time, key.exit_time) > 0) {
            billings[j + 1] = billings[j];
            j = j - 1;
        }
        billings[j + 1] = key;
    }
}

/**
 * Displays the billings array.
 */
void display_billings(Vehicle billings[], int num_billings) {
    for (int i = 0; i < num_billings; i++) {
        printf("%s %s %.2f\n", billings[i].license_plate, 
        billings[i].exit_time, billings[i].parking_fee);
    }
}

/**
 * Sorts the array billings by license plate.
 * @param billings the array of billings
 * @param num_billings the number of billings in the array
 */
void show_park_billings(char *name, char *date) {
    Vehicle billings[INITIAL_HASH_TABLE_SIZE];
    int num_billings = 0;
    find_and_store_billings(name, date, billings, &num_billings);
    sort_billings(billings, num_billings);
    display_billings(billings, num_billings);
    free(name);
}

/**
 * Shows the revenue made by a parking lot day by day.
 * @param name the name of the parking lot
 * @param latest_date the latest date registered in the system
 * @param latest_time the latest time registered in the system
 * @param first_date the string of the first date stored in the system
 */
void show_daily_revenue_summary(char *name, char *latest_date, 
char *latest_time, char *first_date) {
    char date[DATE_LENGTH] = "01-01-0000";
    strcpy(date, first_date);
    while (compare_datetime(date, "00:00", latest_date, latest_time) <= 0) {
        float total_daily_revenue = 0.0;
        int parking_lot_index = find_parking_lot_index(name);
        if (parking_lot_index != -1) {
            for (int i = 0; i < INITIAL_HASH_TABLE_SIZE; i++) {
                Vehicle *current = vehicle_hash_table.table[i];
                while (current != NULL) {
                    if (strcmp(current->name, name) == 0 && 
                    strcmp(current->exit_date, date) == 0) {
                        total_daily_revenue += current->parking_fee;
                    }
                    current = current->next;
                }
            }
            if (total_daily_revenue != 0) {
                printf("%s %.2f\n", date, total_daily_revenue);
            }
        }
        increment_date(date);
    }
    free(name);
}

/**
 * Removes all entries for a given parking lot.
 * @param name the parking lot's name
 */
void remove_entries_for_parking_lot(char *name) {
    for (int i = 0; i < INITIAL_HASH_TABLE_SIZE; i++) {
        Vehicle *current = vehicle_hash_table.table[i];
        Vehicle *prev = NULL;
        while (current != NULL) {
            if (strcmp(current->name, name) == 0) {
                if (prev == NULL) {
                    vehicle_hash_table.table[i] = current->next;
                    if (current == vehicle_hash_table.tail[i]) {
                        vehicle_hash_table.tail[i] = NULL;
                    }
                } else {
                    prev->next = current->next;
                    if (current == vehicle_hash_table.tail[i]) {
                        vehicle_hash_table.tail[i] = prev;
                    }
                }
                free(current->name);
                free(current);
                current = (prev == NULL) ? vehicle_hash_table.table[i] : prev->next;
            } else {
                prev = current;
                current = current->next;
            }
        }
    }
}


