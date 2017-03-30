#!/usr/bin/python2.7
#####################################################
#
# runme.py
#     - Run all the available instances for some
#       project.
#     - Process output data from a given run.
#     - Includes support for multi-threading.
#     - Assumes project code has already been
#       compiled (BOZO_dhullih: Add support to build)
#
#####################################################

from __future__ import division
import subprocess
from subprocess import PIPE, STDOUT
import argparse
import os
import glob

BIT_WIDTH = 2
NUM_ELEMS = 3
MARGIN = 3
RUNTIME = 1 

def get_margin(fname, ampl):
    """Extract the margin of error for the instance."""
    fhandle = open(fname, 'r')
    margin = None
    if ampl:
        for line in fhandle.readlines():
            if 'target_sum' in line:
                target_sum = float(line.split()[-1])
            elif 'calculated_sum' in line:
                calculated_sum = float(line.split()[-1])
                pass
            pass
        margin = abs(1 - target_sum / calculated_sum)
        pass
    else:
        for line in fhandle.readlines():
            if 'Solved' in line:
                try:
                    margin = float(line.split(',')[MARGIN])
                except IndexError:
                    # This is to account for Project1's slightly different format
                    # (if it didn't solve it in 10 minutes, it simply gave up)                    
                    if 'YES' in line:
                        margin = 0
                    else:
                        margin = 'inf'
                        pass
                    pass
                pass
            pass
        assert margin is not None, "Could not find margin in file '%s'" % fname
        
    return margin

def get_runtime(fname, ampl):
    """Extract the margin of error for the instance."""
    fhandle = open(fname, 'r')
    margin = None
    if ampl:
        for line in fhandle.readlines():
            if '_solve_elapsed_time' in line:
                runtime = line.split()[-1].strip()
            pass
        pass
    else:
        for line in fhandle.readlines():
            if 'Solved' in line:
                # Get the reported runtime, strip it, and then get the raw value
                # (the first element.. second is just the value 'second')
                try:
                    runtime = float(line.split(',')[RUNTIME].strip().split()[0])
                except IndexError:
                    # Default value of 10 minutes
                    # This is to account for Project1's slightly different format
                    # (if it didn't solve it in 10 minutes, it simply gave up)                    
                    runtime = 60 * 10 
                pass
            pass
        pass
    
    return runtime

def get_bit_width(string):
    """Extract bit width of instance from file name."""
    # Sample name: ss_inst_27b_18n.out
    # Just split by the '_' char. 
    # Remove the 'b' char at the end of term.
    return int(os.path.basename(string).split('_')[BIT_WIDTH][:-1])

def get_num_elems(string):
    """Extract the number of elements in the set from the file name."""
    # Sample name: ss_inst_27b_18n.out
    # Just split by the '_' char. 
    # Remove the 'n.out' at the end of the term.
    return int(os.path.basename(string).split('_')[NUM_ELEMS][:-5])

def generate_report(dir_name, ampl):
    """Summarize results in a specified directory."""
    
    file_list = glob.glob(os.getcwd() + '/' + (args.r) + '/*.out')
    print 'Found', len(file_list), 'files to process.'

    ####################################################################
    # Generate a csv reporting the % margin error of each instance

    # Come up with a list of the unique bit-widths that were used
    # across all the instances
    bit_width_list = set()
    num_elems_list = set()    
    for fname in file_list:
        bit_width_list.add(get_bit_width(fname))
        num_elems_list.add(get_num_elems(fname))        
        pass
        
    bit_width_list = sorted(bit_width_list)
    num_elems_list = sorted(num_elems_list)    

    fhandle = open('margin_error.csv', 'w')
    fhandle.write(',' + ','.join(str(n) for n in bit_width_list) + '\n')

    for elem_count in num_elems_list:
        # Find all files that match this elem_count
        matching_num_elems = [fname for fname in file_list if get_num_elems(fname) == elem_count]

        # Sort from smallest bit-width to largest
        # BOZO_dhullih: currently not needed
        matching_num_elems = sorted(matching_num_elems, key=lambda k: get_bit_width(k))

        final_row = []
        # BOZO_dhullih: This is just generally inefficient, but given the relatively
        # small number of instances we're processing, you won't notice
        for index, bit_width in enumerate(bit_width_list):
            # See if there is a corresponding file that matches this bit width
            # If there is, report the data, else report nothing for that entry
            matches = [fname for fname in matching_num_elems if get_bit_width(fname) == bit_width]
            assert len(matches) < 2, "Found multiple matches (%s), which is not expected." % matches

            if len(matches) == 0:
                final_row.append(' ')
            else:
                # Add the meaningful data for this cell
                # Extract the % margin for this instance
                final_row.append(get_margin(matches[0], ampl))
                # For debug -- report file name itself
#                final_row.append(matches[0])
                pass
            pass
            
        fhandle.write(str(elem_count) + ',' + ','.join(str(n) for n in final_row) + '\n')

    fhandle.close()

    ####################################################################    
    # ... same data, but in a format to lend itself to a plot
    fhandle = open('margin_error_plot.csv', 'w')
    instance_names = []
    final_row = []    
    for elem_count in num_elems_list:
        # Find all files that match this elem_count
        matching_num_elems = [fname for fname in file_list if get_num_elems(fname) == elem_count]

        # Sort from smallest bit-width to largest
        # BOZO_dhullih: currently not needed
        matching_num_elems = sorted(matching_num_elems, key=lambda k: get_bit_width(k))

        # BOZO_dhullih: This is just generally inefficient, but given the relatively
        # small number of instances we're processing, you won't notice
        for index, bit_width in enumerate(bit_width_list):
            # See if there is a corresponding file that matches this bit width
            # If there is, report the data, else report nothing for that entry
            matches = [fname for fname in matching_num_elems if get_bit_width(fname) == bit_width]
            assert len(matches) < 2, "Found multiple matches (%s), which is not expected." % matches

            if len(matches) == 0:
                pass
            else:
                # Add the meaningful data for this cell
                # Extract the % margin for this instance
                final_row.append(get_margin(matches[0], ampl))
                instance_names.append('%sn%sb' % (str(elem_count), str(bit_width)))
                # For debug -- report file name itself
#                final_row.append(matches[0])
                pass
            pass
        pass
    fhandle.write(','.join(str(n) for n in final_row) + '\n')
    fhandle.write(','.join(str(n) for n in instance_names) + '\n')

    fhandle.close()
    
    ####################################################################    
    # Generate a csv reporting the time each instance took
    fhandle = open('runtime.csv', 'w')
    fhandle.write(',' + ','.join(str(n) for n in bit_width_list) + '\n')

    for elem_count in num_elems_list:
        # Find all files that match this elem_count
        matching_num_elems = [fname for fname in file_list if get_num_elems(fname) == elem_count]

        # Sort from smallest bit-width to largest
        # BOZO_dhullih: currently not needed
        matching_num_elems = sorted(matching_num_elems, key=lambda k: get_bit_width(k))

        final_row = []
        # BOZO_dhullih: This is just generally inefficient, but given the relatively
        # small number of instances we're processing, you won't notice
        for index, bit_width in enumerate(bit_width_list):
            # See if there is a corresponding file that matches this bit width
            # If there is, report the data, else report nothing for that entry
            matches = [fname for fname in matching_num_elems if get_bit_width(fname) == bit_width]
            assert len(matches) < 2, "Found multiple matches (%s), which is not expected." % matches

            if len(matches) == 0:
                final_row.append(' ')
            else:
                # Add the meaningful data for this cell
                # Extract the % margin for this instance
                final_row.append(get_runtime(matches[0], ampl))
                # For debug -- report file name itself
#                final_row.append(matches[0])
                pass
            pass
            
        fhandle.write(str(elem_count) + ',' + ','.join(str(n) for n in final_row) + '\n')

    fhandle.close()

    ####################################################################    
    # ... same data, but in a format to lend itself to a plot
    fhandle = open('runtime_plot.csv', 'w')
    instance_names = []
    final_row = []    
    for elem_count in num_elems_list:
        # Find all files that match this elem_count
        matching_num_elems = [fname for fname in file_list if get_num_elems(fname) == elem_count]

        # Sort from smallest bit-width to largest
        # BOZO_dhullih: currently not needed
        matching_num_elems = sorted(matching_num_elems, key=lambda k: get_bit_width(k))

        # BOZO_dhullih: This is just generally inefficient, but given the relatively
        # small number of instances we're processing, you won't notice
        for index, bit_width in enumerate(bit_width_list):
            # See if there is a corresponding file that matches this bit width
            # If there is, report the data, else report nothing for that entry
            matches = [fname for fname in matching_num_elems if get_bit_width(fname) == bit_width]
            assert len(matches) < 2, "Found multiple matches (%s), which is not expected." % matches

            if len(matches) == 0:
                pass
            else:
                # Add the meaningful data for this cell
                # Extract the % margin for this instance
                final_row.append(get_runtime(matches[0], ampl))
                instance_names.append('%sn%sb' % (str(elem_count), str(bit_width)))
                # For debug -- report file name itself
#                final_row.append(matches[0])
                pass
            pass
        pass
    fhandle.write(','.join(str(n) for n in final_row) + '\n')
    fhandle.write(','.join(str(n) for n in instance_names) + '\n')

    fhandle.close()
    
    pass

def run_thread(e, dir_name, instance_list, gen_log, ftype):
    """Run the executable with provided list of instances. Return the process."""

    # Create the full list of commands to pack together.
    # cd to the directory of the executable and run there
    cmd = ['cd ' + dir_name + '; ']
    for instance in instance_list:
        cmd.append('{} {}; '.format(e, instance))
        pass
    print ''.join(cmd)
    if gen_log:
        log_name = os.path.basename(instance).replace(ftype, '.out')
        fhandle = open(log_name, 'wb')
        p = subprocess.Popen(''.join(cmd), stdout = fhandle, stderr = fhandle, shell = True)
        pass
    else:
        p = subprocess.Popen(''.join(cmd), shell = True, stdout=PIPE, stderr=STDOUT)

    return p

def build():
    """Build the project. BOZO_dhullih: Implement at some point for convenience."""
    pass

if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description=('Run a series of instances for some project. This assumes '
                     'the project code has already been compiled/ready to run.'))
    
    parser.add_argument('-e',
                        type=str,
                        help=('Executable/command to run.'))
    parser.add_argument('-f',
                        type=str,
                        default='.run',                        
                        help=("File type of the instances to look for for the command to execute. Default = '%(default)s'"))
    parser.add_argument('-d',
                        type=str,
                        default='.',                        
                        help=("Directory to run executable/command in. Default = '%(default)s'"))
    parser.add_argument('-l',
                        action='store_true',                        
                        help=('Save stdout of the executable/command to a log file.'))
    parser.add_argument('-r',
                        type=str,
                        help=('Process out files in specified directory, generating a report.'))
    parser.add_argument('-t',
                        type=int,
                        default=8,
                        help=('Number of threads to spawn. Default = %(default)d.'))
    parser.add_argument('--ampl',
                        action='store_true',
                        help='Indicate data is generated by AMPL. For -r switch only.')
    parser.add_argument('-i',
                        type=str,
                        default='instances',
                        help=('Instances directory containing the instances to run '
                              "through the executable. Default = '%(default)s'"))

    args = parser.parse_args()

    if args.e:
        ####################################################################
        # Get the whole list of instances files and divide them up
        # across the threads we're creating
        file_list = glob.glob(os.getcwd() + '/' + (args.i) + '/*' + args.f)
        print 'Found', len(file_list), 'instances to work on.'

        if len(file_list):
            partitioned_lists = [file_list[i::args.t] for i in xrange(args.t)]

            total_instance_count = 0
            for thread in xrange(len(partitioned_lists)):
                print 'Thread', thread, 'gets', len(partitioned_lists[thread]), 'instances'
                total_instance_count = total_instance_count + len(partitioned_lists[thread])
                pass

            assert total_instance_count == len(file_list), ("Divided up instances (%d) does not "
                                                            "add up to the original number of instances (%d).") \
                                                            % (total_instance_counter, len(file_list))

        ####################################################################
        # Kick off the threads
        if len(file_list):
            processes = [run_thread(args.e, args.d, partitioned_lists[thread], args.l, args.f) for thread in xrange(args.t)]
            print 'Threads created. Begin waiting for all threads to complete.'
            for p in processes:
                p.wait()
                out, error = p.communicate()
                print out
                pass
            print 'All threads returned. Done!'
            pass
        pass

    if args.r:
        generate_report(args.r, args.ampl)
        pass
    pass
                              
    
