import os
import pathlib
import subprocess
import time
import datetime

import git

global result_file

# Provide the folder path here
#folder_path = '/home/benjamin/Documents/Uni/SAT-Solver_Seminar/benchmark/'
folder_path = '/home/benjamin/Documents/Uni/SAT-Solver_Seminar/uf250-1065/'
use_n_files = 100

def duration_min_sec(start_time):
    duration = time.time() - start_time
    minutes = int(duration // 60)
    seconds = int(duration % 60)
    return [minutes, seconds]

def execute_program_p(filename):
    command = ['../cmake-build-debug/SAT_Solver', filename]
    start_time = time.time()
    print("start solving " + filename)
    print('at ' + str(datetime.datetime.now()))
    timeout_minutes = 30
    try:
        completed_process = subprocess.run(command, timeout=60 * timeout_minutes, check=True, capture_output=True)
        minutes, seconds = duration_min_sec(start_time)
        solver_output = completed_process.stdout.decode()
        result = solver_output.split("\n")[-2]
        return [result, minutes, seconds]
    except subprocess.TimeoutExpired:
        return ["TIMEOUT", timeout_minutes, 0]
    except subprocess.SubprocessError:
        minutes, seconds = duration_min_sec(start_time)
        return ["ERROR", minutes, seconds]


def execute_program_for_files(folder_path):
    global result_file
    global_start_time = time.time()
    # Iterate over files in the given folder
    folder_content = os.listdir(folder_path)
    folder_content.sort()
    if use_n_files != 0:
        folder_content = folder_content[:use_n_files]
    for filename in folder_content:
        file_path = os.path.join(folder_path, filename)
        result = execute_program_p(file_path)
        result.insert(0, filename)
        result_str = ','.join(str(x) for x in result)
        result_file.write(result_str + "\n")
    minutes, seconds = duration_min_sec(global_start_time)
    result_file.write(f"Overall_Time,,{minutes},{seconds}\n")

def create_result_file():
    global result_file
    now = datetime.datetime.now()
    result_file_name = now.strftime("%Y-%m-%d_%H:%M") + "_benchmark.csv"
    result_file = open(f"../benchmark_results/{result_file_name}", "x")
    repo = git.Repo("/home/benjamin/CLionProjects/SAT_Solver")
    branch = repo.head.reference
    commit_message = branch.commit.message
    commit_hex = branch.commit.hexsha
    result_file_header = []
    result_file_header.append("Solver Version: " + commit_hex + "\n")
    result_file_header.append("Latest Commit message: " + commit_message)
    result_file_header.append("Benchmark Folder: " + folder_path + "\n")
    if use_n_files != 0:
        result_file_header.append(f"Uses the first {use_n_files} files of benchmark folder" + "\n")
    else:
        result_file_header.append("\n")
    result_file_header.append("filename,result,min,sec\n")
    result_file.writelines(result_file_header)


if __name__ == '__main__':
    print('benchmark started at ' + str(datetime.datetime.now()))
    create_result_file()
    execute_program_for_files(folder_path)

    #
    # # Exclude directories
    # if os.path.isfile(os.path.join(folder_path, filename)) and \
    #         pathlib.Path(filename).suffix != '.sol' and \
    #         filename + '.sol' not in os.listdir(folder_path) \
    #         and filename not in timeout_30min_expired:
