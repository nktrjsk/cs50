-- Keep a log of any SQL queries you execute as you solve the mystery.

-- Get info about crime
SELECT description
FROM crime_scene_reports
WHERE month = 7 AND day = 28 AND year = 2021 AND street = 'Humphrey Street';

-- Get relevant interviews
SELECT transcript
FROM interviews
WHERE month = 7 AND day = 28 AND year = 2021 AND transcript LIKE '%bakery%';

-- Get license plates from bakery systems according to first interview
SELECT *
FROM bakery_security_logs
WHERE month = 7 AND day = 28 AND year = 2021 AND activity = "exit" AND hour = 10 AND minute <= 25;

-- Get ATM logs according to second interview and joining it to get information about person
SELECT *
FROM atm_transactions
JOIN bank_accounts
ON atm_transactions.account_number = bank_accounts.account_number
JOIN people
ON person_id = people.id
WHERE month = 7 AND day = 28 AND year = 2021 AND transaction_type = "withdraw" AND atm_location = "Leggett Street";

-- Get phone calls logs according to third interview to find out about caller and receiver
SELECT *
FROM phone_calls
JOIN people
ON caller = people.phone_number
WHERE month = 7 AND day = 28 AND year = 2021 AND duration < 60;
SELECT *
FROM phone_calls
JOIN people
ON receiver = people.phone_number
WHERE month = 7 AND day = 28 AND year = 2021 AND duration < 60;

-- Get the earliest flight
SELECT *
FROM flights
JOIN airports
ON destination_airport_id = airports.id
WHERE month = 7 AND day = 29 AND year = 2021;