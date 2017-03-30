param set_len;
param values{i in 0..set_len-1};
param target_sum;

var x{i in 0..set_len-1} binary;

maximize calculated_sum:
        sum{i in 0..set_len-1} x[i] * values[i];

s.t. c1:
        sum{i in 0..set_len-1} values[i] * x[i] = target_sum;

