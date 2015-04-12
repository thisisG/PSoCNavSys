close all
plot(curLon,curLat)
hold on
plot(wpLon,wpLat, 'LineStyle', 'none', 'Marker', 'x', 'MarkerSize', 12,...
'MarkerEdgeColo', 'red')
r = 0.001;
dHeading = heading+270;
dHeading = mod(dHeading,360);
dHeading = 360-dHeading;
rHeading = (pi*dHeading)/(180)
n = 5
arrowLat = curLat(2 : n : end);
arrowLon = curLon(2 : n : end);
arrowHeading = dHeading(2 : n : end);

u = r*cosd(arrowHeading)
v = r*sind(arrowHeading)


h = quiver(arrowLon, arrowLat,u, v)