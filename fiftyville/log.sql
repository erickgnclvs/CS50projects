-- Keep a log of any SQL queries you execute as you solve the mystery.
--who was the thief
--where did he go
--who helped


--looking for description
SELECT * FROM crime_scene_reports;
SELECT description FROM crime_scene_reports
WHERE year = 2021 AND month = 7 AND day = 28 AND street = "Humphrey Street";
--crime took place at 10:15am - 3 interviews that mention the bakery


--loking for interviews
SELECT * FROM interviews
WHERE year = 2021 AND month = 7 AND day = 28;

--intervies id 161, 162 and 163 (ruth, eugente, raymond)
--within 10 minutes after, enter car in parking lot and drive away - check footage at TIME - TIME + 10
--in the morning withdraws money at Leggett Street ATM
--leaving the bakery talks on the phone - travel on earliest flight of 7/29 - thief asks person to purchase ticket

--looking for security logs within to minutes of 10:15
SELECT name FROM people
JOIN bakery_security_logs ON bakery_security_logs.license_plate = people.license_plate
WHERE year = 2021 AND month = 7 AND day = 28 AND hour = 10 AND minute >= 15 AND minute <= 25;

+---------+
| Vanessa |
| Bruce   |--
| Barry   |
| Luca    |--
| Sofia   |
| Iman    |--
| Diana   |--
| Kelsey  |
+---------+

--looking for ATM transactions
SELECT name FROM people
JOIN bank_accounts ON bank_accounts.person_id = people.id
JOIN atm_transactions ON atm_transactions.account_number = bank_accounts.account_number
WHERE year = 2021 AND month = 7 AND day = 28 AND atm_location = "Leggett Street" AND transaction_type = "withdraw";

+---------+
|  name   |
+---------+
| Bruce   |--
| Diana   |--
| Brooke  |
| Kenny   |
| Iman    |--
| Luca    |--
| Taylor  |
| Benista |
+---------+


--looking for earliest flight off fiftyvile
SELECT * FROM flights
WHERE year = 2021 AND month = 7 AND day = 29;

+----+-------------------+------------------------+------+-------+-----+------+--------+
| id | origin_airport_id | destination_airport_id | year | month | day | hour | minute |
+----+-------------------+------------------------+------+-------+-----+------+--------+
| 18 | 8                 | 6                      | 2021 | 7     | 29  | 16   | 0      |
| 23 | 8                 | 11                     | 2021 | 7     | 29  | 12   | 15     |
| 36 | 8                 | 4                      | 2021 | 7     | 29  | 8    | 20     | -- first flight in the morning
| 43 | 8                 | 1                      | 2021 | 7     | 29  | 9    | 30     |
| 53 | 8                 | 9                      | 2021 | 7     | 29  | 15   | 20     |
+----+-------------------+------------------------+------+-------+-----+------+--------+
--first flight id 36 from 8 to 4

--look names of airports
SELECT * FROM airports;

+----+--------------+-----------------------------------------+---------------+
| id | abbreviation |                full_name                |     city      |
+----+--------------+-----------------------------------------+---------------+
| 1  | ORD          | OHare International Airport            | Chicago       |
| 2  | PEK          | Beijing Capital International Airport   | Beijing       |
| 3  | LAX          | Los Angeles International Airport       | Los Angeles   |
| 4  | LGA          | LaGuardia Airport                       | New York City |--destination
| 5  | DFS          | Dallas/Fort Worth International Airport | Dallas        |
| 6  | BOS          | Logan International Airport             | Boston        |
| 7  | DXB          | Dubai International Airport             | Dubai         |
| 8  | CSF          | Fiftyville Regional Airport             | Fiftyville    |--origin
| 9  | HND          | Tokyo International Airport             | Tokyo         |
| 10 | CDG          | Charles de Gaulle Airport               | Paris         |
| 11 | SFO          | San Francisco International Airport     | San Francisco |
| 12 | DEL          | Indira Gandhi International Airport     | Delhi         |
+----+--------------+-----------------------------------------+---------------+

-- look for passengers of flight id 36
SELECT * FROM passengers WHERE flight_id = 36;

+-----------+-----------------+------+
| flight_id | passport_number | seat |
+-----------+-----------------+------+
| 36        | 7214083635      | 2A   |
| 36        | 1695452385      | 3B   |
| 36        | 5773159633      | 4A   |
| 36        | 1540955065      | 5C   |
| 36        | 8294398571      | 6C   |
| 36        | 1988161715      | 6D   |
| 36        | 9878712108      | 7A   |
| 36        | 8496433585      | 7B   |
+-----------+-----------------+------+

-- doing all of it in a code way - thanks for the help here giovana
SELECT name FROM people
JOIN passengers ON passengers.passport_number = people.passport_number
WHERE passengers.flight_id = (
    SELECT id FROM flights
    WHERE year = 2021 AND month = 7 AND day = 29 AND origin_airport_id = (
        SELECT id FROM airports WHERE city = "Fiftyville")
    ORDER BY hour,minute LIMIT 1);

+--------+
|  name  |
+--------+
| Doris  |
| Sofia  |
| Bruce  |--
| Edward |
| Kelsey |
| Taylor |
| Kenny  |
| Luca   |--
+--------+


--looking for phone calls at time 10:15 - 10:25
SELECT * FROM phone_calls
WHERE year = 2021 AND month = 7 AND day = 28;

--looking for names
SELECT Name FROM people
JOIN phone_calls ON phone_calls.caller = people.phone_number
WHERE year = 2021 AND month = 7 AND day = 28 AND duration < 60;

+---------+
|  name   |
+---------+
| Sofia   |
| Kelsey  |
| Bruce   |--thiefffff
| Kelsey  |
| Taylor  |
| Diana   |
| Carina  |
| Kenny   |
| Benista |
+---------+

--escaped to new york city, saw it intuitively, but now lets do it smartly (again thx giovana)]
SELECT city FROM airports
WHERE id = (
    SELECT destination_airport_id FROM flights
    WHERE year = 2021 AND month = 7 AND day = 29 AND origin_airport_id = (
        SELECT id FROM airports WHERE city = "Fiftyville")
    ORDER BY hour, minute LIMIT 1
);

+---------------+
|     city      |
+---------------+
| New York City |
+---------------+

--look for whom bruce called
--check bruces phone number
SELECT phone_number FROM people WHERE name = "Bruce";

+----------------+
|  phone_number  |
+----------------+
| (367) 555-5533 |
+----------------+

--look for phonoecalls
SELECT name FROM people WHERE phone_number = (
    SELECT receiver FROM phone_calls
    WHERE year = 2021 AND month = 7  AND day = 28 AND duration < 60 AND caller = (
        SELECT phone_number FROM people WHERE name = "Bruce"
    )
);

+-------+
| name  |
+-------+
| Robin | -- helped bruce
+-------+