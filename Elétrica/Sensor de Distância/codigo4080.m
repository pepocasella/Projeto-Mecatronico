distancia_mm4080 = [40,45,50,55,60,65,70,75,80];
bits_st_per4080 = [0.914863879,0.928245055,0.936298681,0.941526564,0.945777645,0.948643255,0.95094571,0.952037135,0.952346124];


plot(bits_st_per4080,distancia_mm4080,'o');
title('Distância x Média Bits')
xlabel('Média Bits ST') % x-axis label
ylabel('Distância') % y-axis label
grid on
figure


%----------------------- Regressão Polinomial -----------------------------

poly_number = 1;
[p4080,S] = polyfit(bits_st_per4080,distancia_mm4080,poly_number);
title('Distância x Média Bits -(Regressão Linear 40-80mm)')
xlabel('Média Bits ST') % x-axis label
ylabel('Distância') % y-axis label
poly1 = poly2sym(p4080);
plot(bits_st_per4080,distancia_mm4080,'o');
title('Distância x Média Bits -(Regressão Linear 40-80mm)')
xlabel('Média Bits ST') % x-axis label
ylabel('Distância') % y-axis label
grid on
hold on;
ezplot(poly1,[0.914863879,0.952346124]);
title('Distância x Média Bits -(Regressão Linear 40-80mm)')
xlabel('Média Bits ST') % x-axis label
ylabel('Distância') % y-axis label
hold off;
figure