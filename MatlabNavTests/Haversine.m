function [ angSep ] = Haversine( latA, lonA, latB, lonB )
%Haversine function that returns the angular separation between two
%coordinate sets. The input should be in degrees.
%   

latA = (latA*pi/180);
lonA = (lonA*pi/180);

latB = (latB*pi/180);
lonB = (lonB*pi/180);

deltaLat = latA - latB;
deltaLon = lonA - lonB;

angSep = (sin(deltaLat)^2) + (cos(latA) * cos(latB) * (sin(deltaLon/2)^2));

end

