import os
import subprocess
import time

def execute_program_p(filename):
    # Replace 'p' with the command or path to your program
    command = ['../cmake-build-debug/SAT_Solver', filename]
    start_time = time.time()
    process = subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

    # Wait for the process to finish or timeout after 10 minutes
    while process.poll() is None and time.time() - start_time < 600:
        time.sleep(1)

    # Check if the process is still running
    if process.poll() is None:
        process.terminate()
        print(f"Execution of '{filename}' exceeded 10 minutes and was aborted.")
        return

    # Calculate the execution time
    execution_time = time.time() - start_time

    # Process the output or errors if needed
    output, errors = process.communicate()

    # Print the execution time and any output/errors
    print(f"Execution of '{filename}' took {execution_time:.2f} seconds.")
    if output:
        print("Output:")
        print(output.decode())
    if errors:
        print("Errors:")
        print(errors.decode())

def execute_program_for_files(folder_path):
    # Iterate over files in the given folder
    for filename in os.listdir(folder_path):
        # Exclude directories
        if os.path.isfile(os.path.join(folder_path, filename)):
            file_path = os.path.join(folder_path, filename)
            execute_program_p(file_path)

# Provide the folder path here
folder_path = '/home/benjamin/Documents/Uni/SAT-Solver_Seminar/uf50-218/'
execute_program_for_files(folder_path)
