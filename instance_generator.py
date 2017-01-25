#!/usr/bin/python
#########################################################
#
# instance_generate.py
#     - Generate a series of subset sum instances
#
#########################################################

from __future__ import division
import argparse
import collections
import random
import math

ss_inst = collections.namedtuple('ss_inst', 'target_sum number_list selected_elems b n density')

def create_ss_inst(n, b):
    """Create an instance of the subset problem.

    Args:
        n (int): Number of elements to be in the list.
        b (int): Bit-width of the largest element in the list.

    Return:
        ss_inst (namedtuple): A subset sum instance packed in a namedtuple. 

    Raises:
        ValueError: n needs to be an even number.
    """

    if n % 2 != 0:
        raise ValueError('n (%d) needs to be an even number.' % n)
    
    #########################################################################################
    # Generate a random list of numbers.
    # Given the required max bit width, generate a list of b random 0's or 1's.
    # Put those in a list to then be concatenated together, and convert to an actual int.
    for attempt in xrange(50):
        number_list = [] 
        for x in xrange(n):
            bits = []
            for i in xrange(b):
                bits.append(str(random.randint(0,1)))
                pass

            number_list.append(int(''.join(bits), 2))
            pass

        if max(number_list).bit_length() == b:
            break
        pass
    else:
        assert 0, \
            '''Max value (%d) did not have the required bit-width of %d. 
            It had a bit-width of %d. Asserting after multiple attempts.''' % (max(number_list), b, max(number_list).bit_length())
        pass

    #########################################################################################
    # Now come up with the target sum.
    # Randomly choose exactly half of the elements in the list and add them up.
    selected_elems = [number_list[i]
                      for i in random.sample(xrange(len(number_list)), len(number_list) // 2)]

    target_sum = sum(selected_elems)
    density = n / b
    
    return ss_inst(target_sum, number_list, selected_elems, b, n, density)

def gen_sweep_of_ss_inst(b_values, n_values):
    """Sweep across the start/end values of b and n to create num 
    different subset sum instances.

    Args:
        num (int): Number of instances
        b_list (list): List of b (bit-width) values to cross
        n_list (list): List of n (num elements in set) values to cross

    Returns:
        instances (list): List of subset sum instances.
    """

    instances = []
    for n in n_values:
        for b in b_values:
            instances.append(create_ss_inst(n, b))
            pass
        pass

    return instances

def write_ss_inst_to_file(inst, uniquifier = '', prefix = 'ss_inst'):
    """Write out a subset sum instance to a file."""

    # Format of the file is:
    # <length of set> <target sum>
    # <number 1>
    # <number 2>
    # ...
    # <number n>

    if uniquifier != '':
        uniquifier = '_' + uniquifier
        
    file_name = '%s_%db_%dn%s.dat' % (prefix, inst.b, inst.n, uniquifier)
    print 'Creating file', file_name
    
    fhandle = open(file_name, 'w')
    fhandle.write('%s %s\n' % (inst.n, inst.target_sum))
    for num in inst.number_list:
        fhandle.write(str(num) + '\n')
        pass
    fhandle.close()
    pass
        
if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description='''Generate a collection of subset sum instances.
        The instances are a sweep of n (number of elements in the set) and b
        (log2(max integer of set)).''')
    
    parser.add_argument('-s',
                        type=int,
                        default=-1,
                        help='Seed to use for random number generate. By default no seed is provided.')
    parser.add_argument('-d',
                        type=str,
                        default='.',
                        help='Directory to store the generated instances. Default = %(default)s')
    parser.add_argument('--uniq',
                        type=str,
                        default='',
                        help='File-name uniquifier. Default = <None>')    
    parser.add_argument('--start_n',
                        type=int,
                        default=2,
                        help='Start set size. Default = %(default)d')
    parser.add_argument('--end_n',
                        type=int,
                        default=2,
                        help='End set size. Default = %(default)d')
    parser.add_argument('--start_b',
                        type=int,
                        default=1,
                        help='Start bit size. Default = %(default)d')    
    parser.add_argument('--end_b',
                        type=int,
                        default=2,
                        help='End bit size. Default = %(default)d')
    parser.add_argument('-p',
                        action='store_true',
                        help='Only print out the list of generated cases.')
    
    args = parser.parse_args()

    assert args.start_n % 2 == 0, 'start_n must be even.'
    assert args.end_n % 2 == 0, 'end_n must be even.'

    if args.s != -1:
        random.seed(args.s)
        pass

    # unroll the range of b and n values
    b_values = range(args.start_b, args.end_b + 1)
    n_values = range(args.start_n, args.end_n + 1, 2)
        
    instances = gen_sweep_of_ss_inst(b_values, n_values)

    for inst in instances:
        if args.p:
            print inst
        else:
            write_ss_inst_to_file(inst, args.uniq)
            pass
        pass

    if args.p:
        print '------------------------'
        print 'Printing the density of each instance...'
        densities = []
        for inst in instances:
            densities.append(inst.density)
            pass
        print densities
        print 'Max density:', max(densities)
        print 'Min density:', min(densities )
        print 'Avg density:', sum(densities) / len(densities)
    
    print '------------------'
    print len(instances), 'instances created.'        
    
