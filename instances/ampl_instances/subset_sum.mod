set values;
var x{values} binary;
param target;

maximize z:
        #sum{i in values} i*x[i];
        sum{i in values} x[i];

s.t. c1:
        sum{i in values} i*x[i] = target;

