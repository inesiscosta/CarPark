# Parking Lot Management System

This was a project for my Introduction to Algorithms and Data Structures class. It is a program written in C to manage car parks allowing a user to register different parks and log vehicle entrys and exits as well as the fare they need to pay for the time they stayed in the park. enunciado.md contains the instructions provided by the teacher.

## Installation
To set up the build environment run the following commands in your repository's root directory.

1. **Install build dependencies:**
    ```sh
    sudo apt-get update && sudo apt-get install -y build-essential
    ```

2. **Build the project with GCC:**
    ```sh
    gcc -O3 -Wall -Wextra -Werror -Wno-unused-result -o parkingsystem *.c
    ```

### Usage
To use the Parking Lot Management System, run the compiled program:
```bash
./parkingsystem
```
Below are the commands you can use with the Parking Management System:

1. **Terminate the Program / Quit (`q`):**
   - **Input:** `q`
   - **Output:** None

2. **Create or List Parking Lots (`p`):**
   - **Input:** `p <park-name> <capacity> <fare-for-each-15-minute-block> <fare-for-each-15-minute-block-after-1-hour> <max-daily-fare>`
   - **Output without arguments:** `<park-name> <capacity> <available-spaces>`
   - **Output with arguments:** None
   - **Errors:**
     - `parking already exists.` if the parking lot name already exists.
     - `invalid capacity.` if the capacity is ≤ 0.
     - `invalid cost.` if any cost is ≤ 0 or costs are not increasing.
     - `too many parks.` if the maximum number of parks is reached.

3. **Register Vehicle Entry (`e`):**
   - **Input:** `e <park-name> <license-plate> <date> <time>`
   - **Output:** `<park-name> <available-spaces>`
   - **Errors:**
     - `no such parking.` if the parking lot does not exist.
     - `parking is full.` if the parking lot is full.
     - `invalid license plate.` if the license plate is invalid.
     - `invalid vehicle entry.` if the vehicle is already inside a parking lot.
     - `invalid date.` if the date/time is invalid or earlier than the last recorded entry/exit.

4. **Register Vehicle Exit (`s`):**
   - **Input:** `s <park-name> <license-plate> <date> <time>`
   - **Output:** `<license-plate> <entry-date> <entry-time> <exit-date> <exit-time> <fare-paid>`
   - **Errors:**
     - `no such parking.` if the parking lot does not exist.
     - `invalid licence plate.` if the license plate is invalid.
     - `invalid vehicle exit.` if the vehicle is not in the specified parking lot.
     - `invalid date.` if the date/time is invalid or earlier than the last recorded entry/exit.

5. **List Vehicle Entries and Exits (`v`):**
   - **Input:** `v <license-plate>`
   - **Output:** `<park-name> <entry-date> <entry-time> <exit-date> <exit-time>`
   - **Errors:**
     - `invalid licence plate.` if the license plate is invalid.
     - `no entries found in any parking.` if there are no recorded entries for the license plate.

6. **Show Parking Lot Billing (`f`):**
   - **Input:** `f <park-name> [<date>]`
   - **Output with one argument:** `<data> <invoiced-amount>`
   - **Output with two arguments:** `<license-plate> <exit-time> <fare-paid>`
   - **Errors:**
     - `no such parking.` if the parking lot does not exist.
     - `invalid date.` if the date is invalid or after the last recorded entry/exit.

7. **Remove Parking Lot (`r`):**
   - **Input:** `r <park-name>`
   - **Output:** `<park-name>`
   - **Errors:**
     - `no such parking.` if the parking lot does not exist.


## Credits:
The tests and respective makefiles were provided by the teacher.

All rights reserved to Pedro Reis @ IST
Copyright (C) 2021, Pedro Reis dos Santos
