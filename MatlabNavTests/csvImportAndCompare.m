clear all
close all

%%

fileA = 'csvtestA.csv';
locA = './../WaypointSystem/TestData/';

[wsgDistA, errDistA, errDistPercentA,...
 wsgDistPSoCA, errDistPSoCA, errDistPercentPSoCA]...
 = errorDataFromCsv(locA, fileA);

%%

fileB = 'csvtestB.csv';
locB = './../WaypointSystem/TestData/';

[wsgDistB, errDistB, errDistPercentB,...
 wsgDistPSoCB, errDistPSoCB, errDistPercentPSoCB]...
 = errorDataFromCsv(locB, fileB);

%%

fileC = 'csvtestC.csv';
locC = './../WaypointSystem/TestData/';

[wsgDistC, errDistC, errDistPercentC,...
 wsgDistPSoCC, errDistPSoCC, errDistPercentPSoCC]...
 = errorDataFromCsv(locC, fileC);

%%

fileD = 'csvtestD.csv';
locD = './../WaypointSystem/TestData/';

[wsgDistD, errDistD, errDistPercentD,...
 wsgDistPSoCD, errDistPSoCD, errDistPercentPSoCD]...
 = errorDataFromCsv(locD, fileD);

%% For csv B - latitude 0->45
figure
subplot(3,1,1)
plot(wsgDistB, errDistB, 'r', 'LineWidth', 2);
hold on
plot(wsgDistPSoCB, errDistPSoCB, '--b', 'LineWidth', 1);
xlabel('wsg84Distance [m]')
ylabel('error [m]')
title('Error in distance calculations by varying latitude from 0 to 45 degrees');
legend('Computer', 'PSoC')

subplot(3,1,2)
plot(wsgDistB, errDistPercentB, 'r', 'LineWidth', 2)
hold on
plot(wsgDistPSoCB, errDistPercentPSoCB, '--b', 'LineWidth', 1);
xlabel('wsg84Distance [m]')
ylabel('error [%]')
legend('Computer', 'PSoC')
title('Percentage error in distance calculations by varying latitude from 0 to 45 degrees');

subplot(3,1,3)
plot(wsgDistB, 100*(errDistB-errDistPSoCB)/errDistB)
xlabel('wsg84Distance [m]')
ylabel('relative error [%]')
title('Percentage difference between distance error on computer and distance error on PSoC')

%% For csv C - longitude 0->45
figure
subplot(2,1,1)
plot(wsgDistC, errDistC, 'r', 'LineWidth', 2)
xlabel('wsg84Distance [m]') 
ylabel('error [m]')
title('Error in distance calculations by varying longitude from 0 to 45 degrees');

subplot(2,1,2)
plot(wsgDistC, errDistPercentC, 'r', 'LineWidth', 2)
xlabel('wsg84Distance [m]')
ylabel('error [%]')
title('Percentage error in distance calculations by varying longitude from 0 to 45 degrees');

%% For csv D - latitude 0->45, lonitude 0->45
figure
subplot(2,1,1)
plot(wsgDistD, errDistD, 'r', 'LineWidth', 2)
xlabel('wsg84Distance [m]')
ylabel('error [m]')
title('Error in distance calculations by varying latitude and longitude from 0 to 45 degrees');

subplot(2,1,2)
plot(wsgDistD, errDistPercentD, 'r', 'LineWidth', 2)
xlabel('wsg84Distance [m]')
ylabel('error [%]')
title('Percentage error in distance calculations by varying latitude and longitude from 0 to 45 degrees');
