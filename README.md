PSoCNavSys
==============
PSoCNavSys is a proof of concept navigation system for use with a PSoC 5LP.

## Installation
Fork, download or otherwise obtain the repository.

## Dependencies

### emFile by SEGGER Microcontroller GmbH & Co. KG

http://www.cypress.com/go/comp_emFile

Install in ./PSoCNavSystem/emFile/

Or change build settings to point them to where your include files and libraries for linking is located.

### NMEA Library by Tim

http://nmea.sourceforge.net/

Install in ./source/nmea/

### fast-cpp-csv-parser by Ben Strasser

https://github.com/ben-strasser/fast-cpp-csv-parser

Install in ./source/csv

## Examples

Example project for testing the system can be found in ./PSoCNavSystem/NavSystem.cydsn/

Requires PSoC Creator 3.1 (or later)

Code to generate waypoint sets and simulate the system can be found in ./WaypointSystem/

Requires Visual Studio 2013 (or later)

*Happy Coding!*

MIT license laid out inlocated in LICENSE.txt
