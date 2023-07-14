import os
import pathlib
import subprocess
import time


def execute_program_p(filename):
    command = ['../cmake-build-debug/SAT_Solver', filename]
    start_time = time.time()
    print("start solving " + filename)
    try:
        completed_process = subprocess.run(command, timeout=60 * 30)
        duration = time.time() - start_time
        print("completed solving " + filename + ". This took " + str(duration // 60) + 'min' + str(duration % 60) + 's')
    except subprocess.TimeoutExpired:
        print('Timeout expired')


def execute_program_for_files(folder_path):
    # Iterate over files in the given folder
    for filename in (os.listdir(folder_path)):
        # Exclude directories
        if os.path.isfile(os.path.join(folder_path, filename)) and \
                pathlib.Path(filename).suffix != '.sol' and \
                filename + '.sol' not in os.listdir(folder_path):
            file_path = os.path.join(folder_path, filename)
            execute_program_p(file_path)


# Provide the folder path here
folder_path = '/home/benjamin/Documents/Uni/SAT-Solver_Seminar/benchmark/'
execute_program_for_files(folder_path)
