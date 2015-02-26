function [ ...
    wgsDist, errDist, errDistPercent, ...
    wgsDistPSoC, errDisPSoC, errDistPercetPSoC ] ...
    = errorDataFromCsv( fileLoc, fileName)
%ERRORDATAFROMCSV Summary of this function goes here
%   Detailed explanation goes here
stdPrefix = 'out';
csvFile = strcat(fileLoc, stdPrefix, fileName);
psocPrefix = 'psocnmea';
psocCsvFile = strcat(fileLoc, psocPrefix, fileName);

csvData = csvread(csvFile, 1, 1);
currPos = csvData([1:end], [1:2]);
endPos = csvData([1:end],[6:7]);
wgsDist = distance(currPos, endPos, wgs84Ellipsoid);

genDist = csvData([1:end],[5]);
errDist = wgsDist - genDist;
errDistPercent = 100*(errDist./wgsDist);

csvDataPSoC = csvread(psocCsvFile, 1, 1);
currPosPSoC = csvDataPSoC([1:end], [1:2]);
endPosPSoC = csvDataPSoC([1:end],[6:7]);
wgsDistPSoC = distance(currPosPSoC, endPosPSoC, wgs84Ellipsoid);

genDistPSoC = csvDataPSoC([1:end],[5]);
errDisPSoC = wgsDistPSoC - genDistPSoC;
errDistPercetPSoC = 100*(errDisPSoC./wgsDistPSoC);
end