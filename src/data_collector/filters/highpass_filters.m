clear all
close all
clc

order = 2;

sampling_rates = [64 128 256 512 1024 2048 4096]; % s

edge_frequencies = [0.1 1]; % Hz

normfreq = bsxfun(@rdivide, edge_frequencies,(sampling_rates/2)');

num = zeros(length(sampling_rates),length(edge_frequencies),order+1);
den = zeros(length(sampling_rates),length(edge_frequencies),order);
ing = zeros(length(sampling_rates),length(edge_frequencies),1);
oug = zeros(length(sampling_rates),length(edge_frequencies),1);

N = length(edge_frequencies) * length(sampling_rates);

for e_idx = 1:length(edge_frequencies)
    
    for s_idx = 1:length(sampling_rates)
        
        d = fdesign.highpass('N,F3db',order,edge_frequencies(e_idx)/sampling_rates(s_idx)*2);
        hd = design(d,'butter','SOSScaleNorm','linf');
        
        num(s_idx,e_idx,:) = hd.coefficients{1}(1:order+1);
        den(s_idx,e_idx,:) = hd.coefficients{1}(order+3:end);
        ing(s_idx,e_idx,:) = hd.coefficients{2}(1);
        oug(s_idx,e_idx,:) = hd.coefficients{2}(2);
    end
    
end
%%
den = reshape(den,[length(edge_frequencies)*length(sampling_rates) order]);
num = reshape(num,[length(edge_frequencies)*length(sampling_rates) order+1]);

%% output filter coefficients in c++ style
clc
fprintf('double normfreq_[%d] = { ',N);
fprintf([repmat('%.16f, ', 1, N-1) '%.16f };\n'], normfreq(:))

fprintf('double ing_[%d] = { ',N);
fprintf([repmat('%.16f, ', 1, N-1) '%.16f };\n'], ing(:))

fprintf('double oug_[%d] = { ',N);
fprintf([repmat('%.16f, ', 1, N-1) '%.16f };\n'], oug(:))

fprintf('double num_[%d][%d] = {\n',N,order+1);
fprintf(['{ ' repmat('%.16f ,', 1, order) '%.16f },\n'], num')
fprintf('};\n');

fprintf('double den_[%d][%d] = {\n',N,order);
fprintf(['{ ' repmat('%.16f ,', 1, order-1) '%.16f },\n'], den')
fprintf('};\n');

