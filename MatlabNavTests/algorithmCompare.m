%%
deltaFracDeg = 1;

baseLat = -90:deltaFracDeg:90;

baseLong = -180:deltaFracDeg:180;

posCount = 1;
%%
for outerCount = 1:length(baseLat)

    for innerCount = 1:length(baseLong)
    
    coordPair(posCount,:) = [baseLat(outerCount), baseLong(innerCount)];    
    posCount = posCount + 1;
    end
    
end

%%