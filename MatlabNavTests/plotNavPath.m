close all
plot(curLon,curLat)
hold on
plot(wpLon,wpLat, 'LineStyle', 'none', 'Marker', 'x', 'MarkerSize', 12,...
'MarkerEdgeColo', 'red')
%plot(Ex1Lon,Ex1Lat, 'LineStyle', 'none', 'Marker', 'o', 'MarkerSize', 12,...
%'MarkerEdgeColo', 'blue')
%plot(Ex2Lon,Ex2Lat, 'LineStyle', 'none', 'Marker', 'o', 'MarkerSize', 12,...
%'MarkerEdgeColo', 'black')
r = 0.0015;
dHeading = heading+270;
dHeading = mod(dHeading,360);
dHeading = 360-dHeading;
rHeading = (pi*dHeading)/(180)
n = 39
arrowLat = curLat(1 : n : end);
arrowLon = curLon(1 : n : end);
arrowHeading = dHeading(2 : n : end);


u = r*cosd(arrowHeading)
v = r*sind(arrowHeading)


h = quiver(arrowLon, arrowLat,u,v,1)
%%
arrows = horzcat(arrowLon, arrowLat, u, v)

dlmwrite('ProperTestQuiver.csv', arrows, 'precision', 10)

%csvwrite('ProperTestQuiver.csv', arrows)