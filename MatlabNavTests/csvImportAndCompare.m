clear all
close all

% csv B
csvFileB = './../WaypointSystem/TestData/outcsvtestB.csv';
csvDataB = csvread(csvFileB,1,1);
currPosB = csvDataB([1:end],[1:2]);
endPosB = csvDataB([1:end],[6:7]);
wgsDistanceB = distance(currPosB, endPosB, wgs84Ellipsoid)

% csv D
csvFileD = './../WaypointSystem/TestData/outcsvtestD.csv';
csvDataD = csvread(csvFileD,1,1);
currPosD = csvDataD([1:end],[1:2]);
endPosD = csvDataD([1:end],[6:7]);

wgsDistanceD = distance(currPosD, endPosD, wgs84Ellipsoid)

%% Extract calculated data from csv data and plot the distance calculations
generatedDistanceB = csvDataB([1:end],[5]);
generatedDistanceD = csvDataD([1:end],[5]);
% Generate error magnitude vector
errorDistanceB = wgsDistanceB - generatedDistanceB;
errorDistanceD = wgsDistanceD - generatedDistanceD;
% Plot error vs wgsDistance
%% For csv B
figure(1)
plot(wgsDistanceB, errorDistanceB, 'r', 'LineWidth', 2)
xlabel('wsg84Distance [m]')
ylabel('error [m]')

figure(2)
plot(errorDistanceB, 'r', 'LineWidth', 2)
%% For csv C

%% For csv D
figure(5)
plot(wgsDistanceD, errorDistanceD, 'r', 'LineWidth', 2)
xlabel('wsg84Distance [m]')
ylabel('error [m]')

figure(6)
plot(errorDistanceD, 'r', 'LineWidth', 2)