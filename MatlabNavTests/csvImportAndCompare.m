csvFileB = './../WaypointSystem/TestData/outcsvtestB.csv';
csvDataB = csvread(csvFileB,1,1);

currPosB = csvDataB([1:end],[1:2]);
endPosB = csvDataB([1:end],[6:7]);

wgsDistanceB = distance(currPosB, endPosB, wgs84Ellipsoid)

%% Extract calculated data from csv data and plot the distance calculations

generatedDistanceB = csvDataB([1:end],[5]);

% Generate error magnitude vector
errorDistanceB = wgsDistanceB - generatedDistanceB;

% Plot error vs wgsDistance
%% For csv B
figure(1)
plot(wgsDistanceB, errorDistanceB, 'r', 'LineWidth', 2)
xlabel('wsg84Distance [m]')
ylabel('error [m]')

figure(2)
plot(errorDistanceB, 'r', 'LineWidth', 2)
%% For csv C