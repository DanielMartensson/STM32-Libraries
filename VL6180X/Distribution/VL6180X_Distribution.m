% Read the log
close all
fid = csvread('log.txt')';

% Initial lists 
X = zeros(1, max(fid) + 5); % We add 5 just to have some extra columns of zero
sampels_for_every_mean = 50;
count = 1;
meanvalues = [];
stdvalues = [];
collection = zeros(1, sampels_for_every_mean);
for i = 1:length(fid)
  
  % Get the value from the log and place it into the list X, depending on what kind of value.
  % Here we only measureing the frequency how often a specific value appears
  value = fid(i);
  X(value) = X(value) + 1;
  
  % Collect the values
  collection(count) =  value;
  
  % For every sampels_for_every_mean:th intervall, do mean and std
  if(count >= sampels_for_every_mean)
    meanvalues = [meanvalues mean(collection)];
    stdvalues = [stdvalues std(collection)];
    
    % Reset
    count = 1;
  else
    count = count + 1; % Count
  end
end

% Distribution
bar(X/sum(X))
xlim([(min(fid)-5) (max(fid)+5)]) 
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
