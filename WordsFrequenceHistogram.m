%% working with the words file
fileID = fopen('words.txt'); %open file

mydata = textscan(fileID, '%s');
md = mydata{1,1}; %put words in a matrix

fclose(fileID);

%% working with the words frequence file
fileID2 = fopen('wordsFrequence.txt');

mydata2 = textscan(fileID2, '%f');
md2 = mydata2{1,1}; %put words frequence in a matrix

fclose(fileID2);

%% histogram
newCell = {md md2}; %join both arrays in a cell array

bar([newCell{:,2}]); %histogram
xlabel('Words (> 500 times)','FontSize',12,'FontWeight','bold');
ylabel('Frequence','FontSize',12,'FontWeight','bold');

xtickangle(90); %making words appear in vertical position

%associate each bar to a word
xticklabels(md');
xticks(1: 1: length(md)); 

