function [ circleDist ] = inverseHaversine( angSep )
%INVERSEHAVERSINE Returns the great circle distance of the angular
%separation normalised to a unit circle.
%   

circleDist = 2 * atan2(sqrt(angSep),sqrt(1-angSep));

end

