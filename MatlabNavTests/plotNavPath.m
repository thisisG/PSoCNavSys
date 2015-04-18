close all
plot(curLon,curLat)
hold on
plot(wpLon,wpLat, 'LineStyle', 'none', 'Marker', 'x', 'MarkerSize', 12,...
'MarkerEdgeColo', 'red')
%plot(Ex1Lon,Ex1Lat, 'LineStyle', 'none', 'Marker', 'o', 'MarkerSize', 12,...
%'MarkerEdgeColo', 'blue')
%plot(Ex2Lon,Ex2Lat, 'LineStyle', 'none', 'Marker', 'o', 'MarkerSize', 12,...
%'MarkerEdgeColo', 'black')
r = 0.025;
dHeading = heading+270;
dHeading = mod(dHeading,360);
dHeading = 360-dHeading;
rHeading = (pi*dHeading)/(180)
n = 10
arrowLat = curLat(2 : n : end);
arrowLon = curLon(2 : n : end);
arrowHeading = dHeading(2 : n : end);
%%

u = r*cosd(arrowHeading)
v = r*sind(arrowHeading)

%%
h = quiver(arrowLon, arrowLat,u,v,1)
%%
arrows = horzcat(arrowLon, arrowLat, u, v)

csvwrite('NavTestTwoAQuiver', arrows)