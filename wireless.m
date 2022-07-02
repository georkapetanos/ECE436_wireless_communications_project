format long
read_time_diff_6083 = readmatrix('nodes6083.txt'); %first test

time_diff_6083 = read_time_diff_6083 ./ 2;
mean_calc_time_6083 = mean(time_diff_6083);
std_calc_time_6083 = std(time_diff_6083);

read_time_diff_6067 = readmatrix('nodes6067.txt'); %first test

time_diff_6067 = read_time_diff_6067 ./ 2;
mean_calc_time_6067 = mean(time_diff_6067);
std_calc_time_6067 = std(time_diff_6067);

read_time_diff_6879 = readmatrix('nodes6879.txt'); %medium distance

time_diff_6879 = read_time_diff_6879 ./2;
mean_calc_time_6879 = mean(time_diff_6879);
std_calc_time_6879 = std(time_diff_6879);

read_time_diff_5489 = readmatrix('nodes5489.txt'); %long distance

time_diff_5489 = read_time_diff_5489 ./2;
mean_calc_time_5489 = mean(time_diff_5489);
std_calc_time_5489 = std(time_diff_5489);

read_time_diff_8889 = readmatrix('nodes8889.txt'); %short distance

time_diff_8889 = read_time_diff_8889 ./2;
mean_calc_time_8889 = mean(time_diff_8889);
std_calc_time_8889 = std(time_diff_8889);

read_time_diff_9172 = readmatrix('9172.txt'); 

time_diff_9172 = read_time_diff_9172 ./2;
mean_calc_time_9172 = mean(time_diff_9172);
std_calc_time_9172 = std(time_diff_9172);

read_time_diff_9179 = readmatrix('9179.txt'); 

time_diff_9179 = read_time_diff_9179 ./2;
mean_calc_time_9179 = mean(time_diff_9179);
std_calc_time_9179 = std(time_diff_9179);

read_time_diff_9188 = readmatrix('9188.txt'); 

time_diff_9188 = read_time_diff_9188 ./2;
mean_calc_time_9188 = mean(time_diff_9188);
std_calc_time_9188 = std(time_diff_9188);

read_time_diff_5991 = readmatrix('5991.txt'); 

time_diff_5991 = read_time_diff_5991 ./2;
mean_calc_time_5991 = mean(time_diff_5991);
std_calc_time_5991 = std(time_diff_5991);

read_time_diff_6391 = readmatrix('6391.txt'); 

time_diff_6391 = read_time_diff_6391 ./2;
mean_calc_time_6391 = mean(time_diff_6391);
std_calc_time_6391 = std(time_diff_6391);

read_time_diff_6391_2 = readmatrix('6391_2.txt'); 

time_diff_6391_2 = read_time_diff_6391_2 ./2;
mean_calc_time_6391_2 = mean(time_diff_6391_2);
std_calc_time_6391_2 = std(time_diff_6391_2);

pd_6083 = makedist('Normal', 'mu',mean_calc_time_6083,'sigma',std_calc_time_6083);
pd_6067 = makedist('Normal', 'mu',mean_calc_time_6067, 'sigma',std_calc_time_6067);
pd_6879 = makedist('Normal', 'mu',mean_calc_time_6879, 'sigma',std_calc_time_6879);
pd_5489 = makedist('Normal', 'mu',mean_calc_time_5489, 'sigma',std_calc_time_5489);
pd_8889 = makedist('Normal', 'mu',mean_calc_time_8889, 'sigma',std_calc_time_8889);
pd_9172 = makedist('Normal', 'mu',mean_calc_time_9172, 'sigma',std_calc_time_9172);
pd_9179 = makedist('Normal', 'mu',mean_calc_time_9179, 'sigma',std_calc_time_9179);
pd_9188 = makedist('Normal', 'mu',mean_calc_time_9188, 'sigma',std_calc_time_9188);
pd_5991 = makedist('Normal', 'mu',mean_calc_time_5991, 'sigma',std_calc_time_5991);
pd_6391 = makedist('Normal', 'mu',mean_calc_time_6391, 'sigma',std_calc_time_6391);
pd_6391_2 = makedist('Normal', 'mu',mean_calc_time_6391_2, 'sigma',std_calc_time_6391_2);

pdf_6083 = pdf(pd_6083, time_diff_6083);
pdf_6067 = pdf(pd_6067, time_diff_6067);
pdf_6879 = pdf(pd_6879, time_diff_6879);
pdf_5489 = pdf(pd_5489, time_diff_5489);
pdf_8889 = pdf(pd_8889, time_diff_8889);
pdf_9172 = pdf(pd_9172, time_diff_9172);
pdf_9179 = pdf(pd_9179, time_diff_9179);
pdf_9188 = pdf(pd_9188, time_diff_9188);
pdf_5991 = pdf(pd_5991, time_diff_5991);
pdf_6391 = pdf(pd_6391, time_diff_6391);
pdf_6391_2 = pdf(pd_6391_2, time_diff_6391_2);

figure(1)
plot(time_diff_6067, pdf_6067, 'b.', time_diff_6083, pdf_6083, 'r.');
legend('nodes 60-67', 'nodes 60-83')

figure(2)
plot(time_diff_9172, pdf_9172, 'r.',time_diff_9179, pdf_9179, 'b.', time_diff_9188, pdf_9188, 'g.', time_diff_5991, pdf_5991, 'm.')
legend('9172', '9179', '9188', '5991', '6391', '6391_2')

figure(3)
plot(time_diff_6879, pdf_6879, 'g.', time_diff_5489, pdf_5489, 'b.', time_diff_8889, pdf_8889, 'r.');
axis([0 0.004 0 8000]);
legend('6879', '5489', '8889')

figure(4)
plot(time_diff_6391, pdf_6391, 'r.', time_diff_6391_2, pdf_6391_2, 'b.' )
legend('6391 1st test','6391 2nd test')


