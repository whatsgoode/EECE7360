param set_len;
set Y := 1..set_len;
param values{Y};
var x{Y} binary;
param target;

maximize z:
    sum{i in Y} x[i];

s.t. c1:
    sum{i in Y} values[i]*x[i] = target;

