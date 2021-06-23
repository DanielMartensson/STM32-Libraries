% Read the log
close all
fid = csvread('log.txt')';

% Initial lists 
X = zeros(1, 10);
sampels_for_every_mean = 50;
count = 1;
meanvalues = [];
stdvalues = [];
sum_value = 0;
std_value = zeros(1, sampels_for_every_mean);

for i = 1:length(fid)
  
  % Get the value from the log and place it into the list X, depending on what kind of value.
  % Here we only measureing the frequency how often a specific value appears
  value = fid(i);
  if(value == 10)
    X(1) = X(1) + 1;
  elseif(value == 11)
    X(2) = X(2) + 1;
  elseif(value == 12)
    X(3) = X(3) + 1;
  elseif(value == 13)
    X(4) = X(4) + 1;
  elseif(value == 14)
    X(5) = X(5) + 1;
  elseif(value == 15)
    X(6) = X(6) + 1;
  elseif(value == 16)
    X(7) = X(7) + 1;
  elseif(value == 17)
    X(8) = X(8) + 1;
  elseif(value == 18)
    X(9) = X(9) + 1;
  elseif(value == 19)
    X(10) = X(10) + 1;
  end
  
  % Count the sum of the value
  sum_value = sum_value + value;
  std_value(count) = value;
  
  % For every sampels_for_every_mean:th intervall, do mean and std
  if(count >= sampels_for_every_mean)
    the_mean = sum_value/count;
    the_std = sqrt(sum((std_value - the_mean).^2)/count); % Formula std = sqrt(sum((x_i - u)^2)/N)
    meanvalues = [meanvalues the_mean];
    stdvalues = [stdvalues the_std];
    
    % Reset
    count = 1;
    sum_value = 0;
  else
    count = count + 1; % Count
  end
end

% Distribution
P = [zeros(1,10) X]; % We want to display the distribution from min(fid) to max(fid)
bar(P/sum(P))
xlim([10 20]) 
grid on
title(sprintf('Distribution where total samples are %i', length(fid)))
ylabel('Propability')
xlabel('value')

% Mean
figure
plot(meanvalues)
grid on
title(sprintf('Mean over time where mean count is %i', sampels_for_every_mean))
ylabel('Mean')
xlabel('Time')

% Std
figure
plot(stdvalues)
grid on
title(sprintf('Std over time where std count is %i', sampels_for_every_mean))
ylabel('Std')
xlabel('Time')
