/**
 * File containing auxiliar functions for the park management system.
 * @file utils.c
 * @author @inesiscosta
 */


#include "utils.h"

/**
 * Breaks down the date and time into their respective components.
 * @param date the date string to be broken down into int day, month, year
 * @param time the time to be broken down into int hour, minute
 * @param day the day of the date as an integer
 * @param month the month of the date as an integer
 * @param year the year of the date as an integer
 * @param hours the hour of the time as an integer
 * @param minutes the minute of the time as an integer
 */
void breakdown_datetime(char *date, char *time, int *day, int *month, 
int *year, int *hours, int *minutes) {
    sscanf(date, "%d-%d-%d", day, month, year);
    sscanf(time, "%d:%d", hours, minutes); 
}

/**
 * Compares two date and time strings.
 * @param date1 the first date string
 * @param time1 the first time string
 * @param date2 the second date string
 * @param time2 the second time string
 * @return -1 if date1/time1 is earlier than date2/time2, 0 if they are equal
 * and 1 if date1/time1 is later than date2/time2
 */
int compare_datetime(char *date1, char *time1, char *date2, char *time2) {
    int day1, month1, year1, hour1, minute1;
    int day2, month2, year2, hour2, minute2;
    breakdown_datetime(date1, time1, &day1, &month1, &year1, &hour1, &minute1);
    breakdown_datetime(date2, time2, &day2, &month2, &year2, &hour2, &minute2);
    if (year1 != year2) {
        return (year1 < year2) ? -1 : 1;
    } 
    if (month1 != month2) {
        return (month1 < month2) ? -1 : 1;
    } 
    if (day1 != day2) {
        return (day1 < day2) ? -1 : 1;
    } 
    if (hour1 != hour2) {
        return (hour1 < hour2) ? -1 : 1;
    } 
    if (minute1 != minute2) {
        return (minute1 < minute2) ? -1 : 1;
    } 
    return 0;
}

/** 
 * Saves the date of the first entry registered into the system.
 * @param entry_date date of the current entry
 * @param first_date the first entry date 
 * @param first_date_set a flag to signal if the first date has been set or not
 */
void update_first_date(char *entry_date, char *first_date, 
char *first_date_set) {
    if (!(*first_date_set)
    || compare_datetime(entry_date, "00:00", first_date, "00:00") < 0) {
        strcpy(first_date, entry_date);
        *first_date_set = 1;
    }
}

/**
 * Checks whether a park exists within the system.
 * @param name the name of the parking lot
 * @return 1 if park exists, 0 otherwise 
 */
int park_exists(char *name) {
    return find_park_by_name(name) != NULL;
}

/**
 * Checks if a license plate is valid.
 * @param plate the license plate to be checked
 * @return 1 if the license plate is valid, 0 otherwise
 */
int is_valid_license_plate(char *plate) {
    if (strlen(plate) != 8 || plate[2] != '-' || plate[5] != '-') {
        return 0;
    }
    int letters = 0, numbers = 0;
    char previous_char = '\0';
    for (int i = 0; plate[i] != '\0'; i++) {
        char current_char = plate[i];
        if (isalpha(current_char)) {
            letters++;
            // Invald if letter is not uppercase or is followed by a digit
            if (!isupper(current_char) || isdigit(previous_char)) {
                return 0;
            }
        // Invalid if digit is not followed by a letter (must be a pair)
        } else if (isdigit(current_char)) {
            numbers++;
            if (isalpha(previous_char)) {
                return 0;
            }
        } else if (current_char != '-') {
            return 0;
        }
        previous_char = current_char;
    }
    if (letters < 2 || numbers < 2) {
        return 0;
    }
    return 1;
}

/**
 * Checks if a date and time string is valid. By valid it's meant that the date
 * and time are in the correct format, are actual days in the calendar and that
 * they are later than the latest date and time registered in the program.
 * @param date the date string
 * @param time the time string
 * @param latest_date the latest entry/exit date registered in the program
 * @param latest_time the latest entry/exit time registered in the program
 * @return 1 if the date and time are valid, 0 otherwise
 */
int is_valid_datetime(char *date, char *time, char *latest_date, 
char *latest_time) {
    int day, month, year, hour, minute;
    breakdown_datetime(date, time, &day, &month, &year, &hour, &minute);
    if (compare_datetime(date, time, latest_date, latest_time) < 0) {
        return 0; 
    }
    if (month < 1 || month > 12) {
        return 0;
    }
    // Check if day is valid for the month
    int days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (day < 1 || day > days_in_month[month - 1]) {
        return 0;
    }
    if (hour < 0 || hour > 23) {
        return 0;
    }
    if (minute < 0 || minute > 59) {
        return 0;
    }
    return 1;
}

/**
 * Checks whether a vehicle entry is valid.
 * @param name the parking lot's name
 * @param plate the vehicle's plate
 * @param entry_date the vehicle's entry date
 * @param entry_time the vehicle's entry time
 * @param latest_date the latest date entered into the system
 * @param latest_time the latest time entered into the system
 * @return 0 if entry is invalid, 1 otherwise 
 */
int is_valid_entry(char *name, char *plate, char *entry_date, 
char *entry_time, char *latest_date, char *latest_time) {
    if (!park_exists(name)) {
        printf("%s: no such parking.\n", name);
        return 0;
    } else if (park_is_full(name)) {
        printf("%s: parking is full.\n", name);
        return 0;
    } else if (!is_valid_license_plate(plate)) {
        printf("%s: invalid licence plate.\n", plate);
        return 0;
    } else if (is_vehicle_parked(plate)) {
        printf("%s: invalid vehicle entry.\n", plate);
        return 0;
    } else if (!is_valid_datetime(entry_date, entry_time, latest_date, 
    latest_time)) {
        printf("invalid date.\n");
        return 0;
    }
    return 1;
}

/**
 * Checks whether an exit is valid.
 * @param name the parking lot's name
 * @param plate the vehicle's plate
 * @param exit_date the vehicle's exit date
 * @param exit_time the vehicle's exit time
 * @param latest_date the latest date registered in the system
 * @param latest_time the latest time registered in the system
 * @return 0 if exit is invalid, 1 otherwise 
 */
int is_valid_exit(char *name, char *plate, char *exit_date, char *exit_time, 
char *latest_date, char *latest_time) {
    if (!park_exists(name)) {
        printf("%s: no such parking.\n", name);
        return 0;
    } else if (!is_valid_license_plate(plate)){
        printf("%s: invalid licence plate.\n", plate);
        return 0;
    } else if (!is_vehicle_parked_here(name, plate)) {
        printf("%s: invalid vehicle exit.\n", plate);
        return 0;
    } else if (!is_valid_datetime(exit_date, exit_time, latest_date, 
    latest_time)) {
        printf("invalid date.\n");
        return 0;
    }
    return 1;
}

/**
 * Formats the time string to HH:MM.
 * @param time the unformatted time string
 */
void format_time(char *time) {
    int hours, minutes;
    sscanf(time, "%d:%d", &hours, &minutes);
    sprintf(time, "%02d:%02d", hours, minutes);
}

/**
 * Checks if parking lot exists if the given date is valid and calls
 * show_park_billings.
 * @param name the parking lot's name
 * @param date the date for which the billings are to be found
 * @param latest_date the latest date registered in the system
 * @param latest_time the latest time registered in the system
 */
void validate_and_call_show_park_billings(char *name, char *date, 
char *latest_date, char *latest_time) {
    name = realloc(name, strlen(name) + 1);
    if (!park_exists(name)) {
        printf("%s: no such parking.\n", name);
        free(name);
        return;
    } else if (!is_valid_datetime(latest_date, latest_time, date, "00:00")){
        printf("invalid date.\n");
        free(name);
        return;
    }
    show_park_billings(name, date);
}

/**
 * Checks if parking lot exists and calls show_daily_revenue_summary.
 * @param name the parking lot's name
 * @param latest_date the latest date registered in the system
 * @param latest_time the latest time registered in the system
 * @param first_date the earliest date registered in the system
 */
void validate_and_call_show_daily_revenue_summary(char *name,
char *latest_date, char *latest_time, char *first_date) {
    name = realloc(name, strlen(name) + 1);
    if (!park_exists(name)) {
        printf("%s: no such parking.\n", name);
        free(name);
        return;
    }
    show_daily_revenue_summary(name, latest_date, latest_time, first_date);
}

/**
 * Calculates the total number of days from the start of the year to a given month.
 * @param month the month to calculate the days before
 * @return the total number of days before the given month
 */
int month_days_before(int month) {
    int days = 0;
    for (int i = 1; i <= month; i++) {
        switch (i) {
            case 1: case 3: case 5: case 7: case 8: case 10: case 12:
                days += 31;
                break;
            case 4: case 6: case 9: case 11:
                days += 30;
                break;
            case 2:
                days += 28;
                break;
        }
    }
    return days;
}

/**
 * Calculates the total number of minutes elapsed since A.C. as if time started
 * counting on 01-01-0000 at 00:00.
 * @param date the date string
 * @param time the time string
 * @return the total number of minutes elapsed since 01-01-0000 at 00:00
 */
int calculate_total_minutes(char *date, char *time) {
    int day, month, year, hour, minute;
    sscanf(date, "%d-%d-%d", &day, &month, &year);
    sscanf(time, "%d:%d", &hour, &minute);
    int total_minutes = year * 365 * 24 * 60 + (year / 4) * 24 * 60 + 
    (month_days_before(month - 1) + day - 1) * 24 * 60 + hour * 60 + minute;
    return total_minutes;
}

/**
 * Increments the date by one day.
 * @param date the date string to be incremented
 */
void increment_date(char *date) {
    int year, month, day;
    sscanf(date, "%d-%d-%d", &day, &month, &year);
    int days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    day++;
    if (day > days_in_month[month - 1]) {
        day = 1;
        month++;
        if (month > 12) {
            month = 1;
            year++;
        }
    }
    sprintf(date, "%02d-%02d-%04d", day, month, year);
}

/**
 * Extracts the parking lot name from the command and removes the parking lot.
 * @param command command in format "r <name>"
 */
void remove_parking_lot(char *command) {
    char *name = (char *)malloc(BUFSIZ * sizeof(char));
    if (sscanf(command, "r \"%[^\"]\"", name) == 1  
    || sscanf(command, "r %s", name) == 1) {
        name = realloc(name, strlen(name) +1);
        int found_index = find_parking_lot_index(name);
        if (found_index != -1) {
            remove_parking_lot_entry(found_index);
            print_remaining_parks();
        } else {
            printf("%s: no such parking.\n", name);
        }
    }
    free(name);
}