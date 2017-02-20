#!/usr/bin/python
#####################################################
#
# runme.py
#     - Run all the available instances for some
#       project.
#     - Includes support for multi-threading.
#     - Assumes project code has already been
#       compiled (BOZO_dhullih: Add support to build)
#
#####################################################

import subprocess
import argparse
import os
import glob

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
                        required=True,
                        help=('Executable to run.'))
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
                              
    
