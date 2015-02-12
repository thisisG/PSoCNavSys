clear all
close all

% csv B
csvFileB = './../WaypointSystem/TestData/outcsvtestB.csv';
csvDataB = csvread(csvFileB,1,1);
currPosB = csvDataB([1:end],[1:2]);
endPosB = csvDataB([1:end],[6:7]);
wgsDistanceB = distance(currPosB, endPosB, wgs84Ellipsoid)

% csv C
csvFileC = './../WaypointSystem/TestData/outcsvtestC.csv';
csvDataC = csvread(csvFileC,1,1);
currPosC = csvDataC([1:end],[1:2]);
endPosC = csvDataC([1:end],[6:7]);
wgsDistanceC = distance(currPosC, endPosC, wgs84Ellipsoid)

% csv D
csvFileD = './../WaypointSystem/TestData/outcsvtestD.csv';
csvDataD = csvread(csvFileD,1,1);
currPosD = csvDataD([1:end],[1:2]);
endPosD = csvDataD([1:end],[6:7]);
wgsDistanceD = distance(currPosD, endPosD, wgs84Ellipsoid)

%% Extract calculated data from csv data and plot the distance calculations
generatedDistanceB = csvDataB([1:end],[5]);
generatedDistanceC = csvDataC([1:end],[5]);
generatedDistanceD = csvDataD([1:end],[5]);
% Generate error magnitude vector
errorDistanceB = wgsDistanceB - generatedDistanceB;
errorDistanceBPercentage = 100*(errorDistanceB./wgsDistanceB);
errorDistanceC = wgsDistanceC - generatedDistanceC;
errorDistanceCPercentage = 100*(errorDistanceC./wgsDistanceC);
errorDistanceD = wgsDistanceD - generatedDistanceD;
errorDistanceDPercentage = 100*(errorDistanceD./wgsDistanceD);
% Plot error vs wgsDistance
%% For csv B - latitude 0->45
figure
subplot(2,1,1)
plot(wgsDistanceB, errorDistanceB, 'r', 'LineWidth', 2)
xlabel('wsg84Distance [m]')
ylabel('error [m]')
title('Error in distance calculations by varying latitude from 0 to 45 degrees');
subplot(2,1,2)
plot(wgsDistanceB, errorDistanceBPercentage, 'r', 'LineWidth', 2)
xlabel('wsg84Distance [m]')
ylabel('error [%]')
title('Percentage error in distance calculations by varying latitude from 0 to 45 degrees');

figure(3)
plot(errorDistanceB, 'r', 'LineWidth', 2)
%% For csv C - longitude 0->45
figure
subplot(2,1,1)
plot(wgsDistanceC, errorDistanceC, 'r', 'LineWidth', 2)
xlabel('wsg84Distance [m]')
ylabel('error [m]')
title('Error in distance calculations by varying longitude from 0 to 45 degrees');
subplot(2,1,2)
plot(wgsDistanceC, errorDistanceCPercentage, 'r', 'LineWidth', 2)
xlabel('wsg84Distance [m]')
ylabel('error [%]')
title('Percentage error in distance calculations by varying longitude from 0 to 45 degrees');


%% For csv D - latitude 0->45, lonitude 0->45
figure
subplot(2,1,1)
plot(wgsDistanceD, errorDistanceD, 'r', 'LineWidth', 2)
xlabel('wsg84Distance [m]')
ylabel('error [m]')
title('Error in distance calculations by varying latitude and longitude from 0 to 45 degrees');
subplot(2,1,2)
plot(wgsDistanceD, errorDistanceDPercentage, 'r', 'LineWidth', 2)
xlabel('wsg84Distance [m]')
ylabel('error [%]')
title('Percentage error in distance calculations by varying latitude and longitude from 0 to 45 degrees');

figure(7)
plot(errorDistanceD, 'r', 'LineWidth', 2)