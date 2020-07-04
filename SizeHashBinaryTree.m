%% Gráfico de taxa de ocupação da hash table com Binary Tree

fileID = fopen('sizeBinary.txt');
mydata = textscan(fileID, '%f');
md = mydata{1,1};

bar(md);
xlabel('Iteration','FontSize',12,'FontWeight','bold');
ylabel('Occupancy Rate','FontSize',12,'FontWeight','bold');
ylim([0 1]);





