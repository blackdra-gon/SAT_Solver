import os
import signal
from subprocess import Popen, PIPE, TimeoutExpired

from time import monotonic as timer

TIMEOUT = 60*10  # seconds


def run_with_timeout(instance_path):
    solver_path = "../cmake-build-debug/SAT_Solver"
    command = solver_path + " " + instance_path


    # make sure to properly kill subprocesses after timeout
    # see: https://stackoverflow.com/questions/36952245/subprocess-timeout-failure
    start = timer()
    with Popen(command, stderr=PIPE, stdout=PIPE, preexec_fn=os.setsid, universal_newlines=True) as process:
        try:
            gist = process.communicate(timeout=TIMEOUT)[0]
        except TimeoutExpired:
            os.killpg(process.pid, signal.SIGINT) # send signal to the process group
            gist = "Timeout expired"
    return gist
