function Unscented_Kalman_Filter()
  
  % Initial parameters
  L = 1;
  a = 0.5;
  b = 2;
  k = 1;
  Q = 0.0001;
  R = 2;
  P = 1;
  xhat = 15;
  zk = 13;
  
  % Read the data
  fid = csvread('log.txt')';
  l = 500;
  
  % Filtered data
  X = zeros(1, l);
  
  for i = 1:l
    
    % Get measurement 
    zk = fid(i);
    
    % Save UKF value
    X(i) = xhat;
      
    % UKF filter
    [xhat, P] = ukf(xhat, zk, P, Q, R, a, k, b, L);
  end
  
  close all
  plot(1:l, X, 'r', 1:l, fid(1:l), 'b');
  legend('UKF', 'Real')
  grid on
end

function [xhat, P] = ukf(xhat, zk, P, Q, R, a, k, b, L)  
  % Init - Create the weights
  [Wa, Wc] = ukf_create_weights(a, b, k, L);
    
  % PREDICT: Step 0 - Predict the state and state estimation error covariance at the next time step
  s = ukf_compute_sigma_points(xhat, P, a, k, L);

  % PREDICT: Step 1 - Run our transition function
  x = ukf_transition(s, L);
 
  % PREDICT: Step 2 - Combine the predicted states to obtain the predicted states
  xhat = ukf_multiply_weights(x, Wa, L);

  % PREDICT: Step 3 - Compute the covariance of the predicted state
  P = ukf_estimate_covariance(x, xhat, Wc, Q, L);
  
  % UPDATE: Step 1 - Use the nonlinear measurement function to compute the predicted measurements for each of the sigma points.
  z = s; % Here we assume that the observation function z = h(s, u) = s
  
  % UPDATE: Step 2 - Combine the predicted measurements to obtain the predicted measurement
  zhat = ukf_multiply_weights(z, Wa, L);
  
  % UPDATE: Step 3 - Estimate the covariance of the predicted measurement
  Shat = ukf_estimate_covariance(z, zhat, Wc, R, L);
  
  % UPDATE: Step 4 - Estimate the cross-covariance between xhat and zhat. 
  Csz = ukf_estimate_cross_covariance(s, xhat, z, zhat, Wc, L);
  
  % UPDATE: Step 5 - Find kalman K matrix
  K = ukf_create_kalman_K(Shat, Csz, L);
  
  % UPDATE: Step 6 - Obtain the estimated state and state estimation error covariance
  [xhat, P] = ukf_state_update(K, Shat, P, xhat, zk, zhat, L);
  
end

function [Wa, Wc] = ukf_create_weights(a, b, k, L)
  N = 2 * L + 1;
  Wa = zeros(1, N);
  Wc = zeros(1, N);
  for i = 1:N
    if(i == 1)
      Wa(i) = (a*a*k-L)/(a*a*k);
      Wc(i) = Wa(i) + 1 - a*a + b;
    else
      Wa(i) = 1/(2*a*a*k);
      Wc(i) = Wa(i);
    end
  end
end

function [s] = ukf_compute_sigma_points(x, P, a, k, L)
  N = 2 * L + 1;
  compensate = L + 1;
  s = zeros(L, N);
  A = a*sqrt(k)*chol(P);
  for j = 1:N
    if(j == 1)
      s(:, j) = x;
    elseif(and(j >= 2, j <= L + 1))
      s(:, j) = x + A(:, j - 1);
    else 
      s(:, j) = x - A(:, j - compensate);
    end
  end
end

function x = ukf_multiply_weights(xi, W, L)
  N = 2 * L + 1;
  x = zeros(L, 1);
  for i = 1:N
    x = x + W(i)*xi(:, i);
  end
end

function P = ukf_estimate_covariance(xi, x, W, O, L)
  N = 2 * L + 1;
  P = zeros(L, L);
  for i = 1:N
    P = P + W(i)*(xi(:, i) - x)*(xi(:, i) - x)';
  end
  P = P + O;
end

function Csz = ukf_estimate_cross_covariance(s, xhat, z, zhat, Wc, L)
  N = 2 * L + 1;
  Csz = zeros(L, L);
  for i = 1:N
    Csz = Csz + Wc(i)*(s(:, i) - xhat)*(z(:, i) - zhat)';
  end
end

function K = ukf_create_kalman_K(Shat, Csz, L)
  K = zeros(L, L);
  for i = 1:L
    % Solve Ax = b with Cholesky
    A = chol(Shat, 'lower');
    y = linsolve(A, Csz(:, i));
    K(:, i) = linsolve(A', y);
  end
end

function [xhat, P] = ukf_state_update(K, Shat, P, xhat, zk, zhat, L)
  xhat = xhat + K*(zk - zhat);
  P = P - K*Shat*K';
end

function x = ukf_transition(s, L)
  N = 2 * L + 1;
  x = zeros(L, 1);
  for i = 1:N
    x(i) = std(s(:, i))*randn + mean(s(:, i)); % std*random_variable + average = Gaussian distribution
  end
end
