%% Generate 9 columns
% type,lat,lon,speed,heading,distance,distance,,
clear all

csvFile = './../WaypointSystem/TestData/csvtestB.csv'
csvRows = 1500;
startLat = 0;
endLat = 45;
startLon = 0;
endLon = 0;

type = linspace(0,0,csvRows)'

lat = linspace(startLat, endLat, csvRows)'

lon = linspace(startLon, endLon, csvRows)'

speed = type
heading = type
distance = type

csvMatrix = [type, lat, lon, speed, heading, distance, distance, type, type]

csvwrite(csvFile, csvMatrix)