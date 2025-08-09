% scripts/compute_metrics.m
% Compute RMSE and IAE between measured and simulated references.
% Expects two time series: (t_real, y_real) and (t_sim, y_sim).
% Options: idx_start (crop), lag_sec (manual), auto_lag (xcorr), scale_sim.

function S = compute_metrics(t_real, y_real, t_sim, y_sim, opts)
if nargin < 5, opts = struct; end
opts = defaults(opts, struct( ...
    'idx_start', 1, ...      % crop start index on real signal
    'lag_sec',   0.0, ...    % manual time shift applied to real signal
    'auto_lag',  true, ...   % estimate extra lag via xcorr
    'scale_sim', 1.0, ...    % scale for simulated signal
    'label',     'run' ...   % plot/print label
));

% Crop and shift real signal
t_real = t_real(:); y_real = y_real(:);
t_sim  = t_sim(:);  y_sim  = y_sim(:) * opts.scale_sim;

t_real = t_real(opts.idx_start:end) + opts.lag_sec;
y_real = y_real(opts.idx_start:end);

% Optional: auto lag via cross-correlation on detrended signals
if opts.auto_lag
    % resample sim to real time grid (coarse) for lag estimate
    y_sim_rs = interp1(t_sim, y_sim, t_real, 'linear', 'extrap');
    yr = y_real - mean(y_real); ys = y_sim_rs - mean(y_sim_rs);
    [xc, lags] = xcorr(yr, ys, 'biased');
    [~,i] = max(xc);
    dt_r = median(diff(t_real));
    lag_est = -lags(i) * dt_r; % negative because we shift real time axis
    t_real = t_real + lag_est;
end

% Interpolate sim onto (possibly shifted) real time base
y_sim_i = interp1(t_sim, y_sim, t_real, 'linear', 'extrap');

% Metrics
e   = y_real - y_sim_i;
rmse = sqrt(mean(e.^2));
dt   = median(diff(t_real));
iae  = sum(abs(e)) * dt;   % Integral of Absolute Error

% Plot
figure('Name', ['Metrics - ' opts.label]); clf
plot(t_real, y_real, 'b', 'LineWidth',1.5); hold on
plot(t_real, y_sim_i, 'r--', 'LineWidth',1.5)
grid on; xlabel('Time (s)'); ylabel('Ball position (m)')
title(sprintf('%s | RMSE=%.5f m, IAE=%.5f m·s', opts.label, rmse, iae))
legend('Measured','Simulation','Location','best')

% Return struct
S = struct('rmse', rmse, 'iae', iae);
disp(S)

end

function o = defaults(o, d)
fn = fieldnames(d);
for k=1:numel(fn), if ~isfield(o,fn{k}), o.(fn{k}) = d.(fn{k}); end, end
end
