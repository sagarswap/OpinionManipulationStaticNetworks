import subprocess
import os

def run_program_in_parallel(n):
    """
    Runs the compiled C++ program in parallel on 'n' different terminals.

    Parameters:
        n (int): The number of parallel terminals to launch the program in.
    """
    # Compile the program first
    compile_command = ["g++", "NoBot.cpp", "Node.cpp", "Edge.cpp", "Graph.cpp", "-o", "output"]
    print("Compiling the program...")
    compile_process = subprocess.run(compile_command, capture_output=True, text=True)

    if compile_process.returncode != 0:
        print(f"Compilation failed with error:\n{compile_process.stderr}")
        return
    
    print("Compilation successful.")
    
    # Run the program in 'n' parallel terminals
    processes = []
    for i in range(n):
        terminal_command = f"x-terminal-emulator -e ./output"  # For Linux; change for other OS
        print(f"Starting instance {i + 1}...")
        process = subprocess.Popen(terminal_command, shell=True)
        processes.append(process)
    
    # Wait for all processes to complete
    for i, process in enumerate(processes):
        process.wait()
        print(f"Instance {i + 1} completed.")

if __name__ == "__main__":
    n = int(input("Enter the number of parallel instances: "))
    run_program_in_parallel(n)
