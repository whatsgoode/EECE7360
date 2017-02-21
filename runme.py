#!/usr/bin/python
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

import subprocess
import argparse
import os
import glob

BIT_WIDTH = 2
NUM_ELEMS = 3
MARGIN = 3


def get_margin(fname):
    """Extract the margin of error for the instance."""
    fhandle = open(fname, 'r')
    margin = None
    for line in fhandle.readlines():
        if 'Solved' in line:
            margin = float(line.split(',')[MARGIN])
            pass
        pass
    assert margin is not None, "Could not find margin in file '%s'" % fname
    return margin

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

def generate_report(dir_name):
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
                final_row.append(get_margin(matches[0]))
                # For debug -- report file name itself
#                final_row.append(matches[0])
                pass
            pass
            
        fhandle.write(str(elem_count) + ',' + ','.join(str(n) for n in final_row) + '\n')

    fhandle.close()

    ####################################################################    
    # TODO_dhullih: Generate a csv reporting the time each instance took
    
    pass

def run_thread(e, instance_list):
    """Run the executable with provided list of instances. Return the process."""

    # Create the full list of commands to pack together.
    # cd to the directory of the executable and run there
    dir_name = os.path.dirname(e)
    cmd = ['cd ' + dir_name + '; ']
    for instance in instance_list:
        cmd.append('./{} {}; '.format(os.path.basename(e), instance))
        # Comment below out. It's if we want to create log files ourself, but the program
        # already handles that
#        cmd.append('{} {} > {}; '.format(os.path.basename(e), instance, os.path.basename(instance).replace('.dat', '.out')))
        pass
    print ''.join(cmd)
    p =subprocess.Popen(''.join(cmd), shell = True)
    return p

def build():
    """Build the project. BOZO_dhullih: Implement at some point for convenience."""
    pass

if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description=('Run a series of instances for some project. This assumes '
                     'the project code has already been compiled.'))
    
    parser.add_argument('-e',
                        type=str,
                        help=('Executable to run.'))
    parser.add_argument('-r',
                        type=str,
                        help=('Process out files in specified directory, generating a report.'))
    parser.add_argument('-t',
                        type=int,
                        default=8,
                        help=('Number of threads to spawn. Default = %(default)d.'))
    parser.add_argument('--i',
                        type=str,
                        default='instances',
                        help=('Instances directory containing the instances to run '
                              "through the executable. Default = '%(default)s'"))

    args = parser.parse_args()

    if args.e:
        assert os.path.isfile(args.e), "Executable '%s' does not exist." % args.e

        # BOZO_dhullih: If file does not exist, call make on that dir, check return
        # status, and continue if all went well.
    
        ####################################################################
        # Get the whole list of instances files and divide them up
        # across the threads we're creating
        file_list = glob.glob(os.getcwd() + '/' + (args.i) + '/*.dat')
        print 'Found', len(file_list), 'instances to work on.'
    
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
        processes = [run_thread(args.e, partitioned_lists[thread]) for thread in xrange(args.t)]
        print 'Threads created. Begin waiting for all threads to complete.'
        for p in processes:
            p.wait()
            pass
        print 'All threads returned. Done!'
        pass

    if args.r:
        generate_report(args.r)
        pass
    pass
                              
    
