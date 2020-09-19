# Project I in Data-Structures
I implement a set of structures (hash tables, linked lists, binary trees) that allow the entry and queries in a large volume of records in patientRecord type.

One example of patient record is: 
889 Mary Smith COVID-2019 China 25-1-2019 27-1-2019

1. **recordID**: a string (it can have a single digit) that 
determines each record in a unique way.
2. **patientFirstName**: a string of letters without spaces.
3. **patientLastName**: a string of letters without spaces.
4. **diseaseID**: a string of letters, numbers, and possibly a
character “-” but without spaces.
5. **country**: a string of letters without spaces.
6. **entryDate**: date of admission to the hospital. It must have the form DD-MM-YYYY where DD expresses the day, MM the month, and YYYY the year when patient entered hospital.
7. **exitDate**: date the patient was discharged from the hospital. It must have the form DD-MM-YYYY where DD expresses the day, MM the month, and YYYY the time of its exit. It could be a null character (-) which means that the patient has not yet been discharged.

To get started, my application open the patientRecordsFile file to read
one by one the lines and initialize and store in memory the data structures that will
uses when executing queries.

When the application finishes editing the patientRecordsFile file, it will wait
user input from the keyboard. The user will be able to give the following commands
(arguments in [] are optional):

● **/globalDiseaseStats [date1 date2]**
The application will print for each virus, the number of cases recorded in the system. If
date1 date2 are given, the application will print for each virus, the number of cases that have
recorded in the system within the time period [date1 ... date2].
If there is a definition for [date1] there should be a definition for [date2], otherwise it will
an error message is printed to the user.

● **/diseaseFrequency virusName [country] date1 date2**
If country argument is not given, the application will print (for the virusName) the number of cases are recorded in the system during [date1 ... date2]. If 
country argument is given, the application will print (for virusName), the number of cases in
country that are recorded in space [date1 ... date2].

● **/topk-Diseases k country [date1 date2]**
The application will print, for the country, the diseases that are the top k of the cases
in space [date1 ... date2] (if given). If there is a definition for [date1] you should
there is also a definition for [date2], otherwise an error message will be printed to the user.

● **/topk-Countries k disease [date1 date2]**
The application will print, for the disease virus, the countries that have shown the top k of the cases
of this virus.

● **/insertPatientRecord recordID patientFirstName patientLastName diseaseID entryDate [exitDate]**
The application will introduce a new registration with its data in the system. The exitDate element is
optional.

● **/recordPatientExit recordID exitDate**
The application will add exitDate to the document with ID recordID.

● **/numCurrentPatients [disease]**
If the disease argument is given, the application will print the number of patients that are still being treated with
the disease. If no argument is given, the application will print the documents for each virus
patients still being treated.

● **/exit**
Exit the application. Make sure you release all the free memory properly.

The data structures that I used has the following architecture:

![Screenshot from 2020-09-19 11-06-39](https://user-images.githubusercontent.com/60033683/93662409-8568c480-fa68-11ea-9b1a-d5293451f932.png)

## Execution:
 1) **make**
 2) **./diseaseMonitor -p patientRecordsFile (i.e small.txt) -h1 diseaseHashtableNumOfEntries (i.e 10) -h2 countryHashtableNumOfEntries (i.e 10) -b bucketSize (i.e 20)**
 3) **make clean**
