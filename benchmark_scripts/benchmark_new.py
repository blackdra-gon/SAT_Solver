import os
import pathlib
import subprocess
import time
import datetime


def execute_program_p(filename):
    command = ['../cmake-build-debug/SAT_Solver', filename]
    start_time = time.time()
    print("start solving " + filename)
    print('at ' + str(datetime.datetime.now()))
    try:
        completed_process = subprocess.run(command, timeout=60 * 30, check=True)
        duration = time.time() - start_time
        print("completed solving " + filename)
        print("This took " + str(duration // 60) + 'min' + str(duration % 60) + 's')
    except subprocess.TimeoutExpired:
        print('Timeout expired')
    except subprocess.SubprocessError:
        print('Error in solving ' + filename)


def execute_program_for_files(folder_path):
    # Iterate over files in the given folder
    for filename in (os.listdir(folder_path)):
        # Exclude directories
        if os.path.isfile(os.path.join(folder_path, filename)) and \
                pathlib.Path(filename).suffix != '.sol' and \
                filename + '.sol' not in os.listdir(folder_path)\
                and filename not in timeout_30min_expired:
            file_path = os.path.join(folder_path, filename)
            execute_program_p(file_path)


# Provide the folder path here
folder_path = '/home/benjamin/Documents/Uni/SAT-Solver_Seminar/benchmark/'
timeout_30min_expired = ['7d2199baf2526263b5f753d290d2790b-md4__zeroOut_19__freeIn_23__seed_2.cnf']
print('benchmark started at ' + str(datetime.datetime.now()))
execute_program_for_files(folder_path)
