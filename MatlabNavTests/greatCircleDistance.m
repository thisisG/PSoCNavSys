function [ dist ] = greatCircleDistance( circlePath )
%UNTITLED5 Summary of this function goes here
%   

R = 6371*1000; % Earth radius in metres

dist = R * circlePath;

end

