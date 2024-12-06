import subprocess

test_cases = [
    {
        "description": "Test echo command",
        "command": 'echo "Hello, World!"'
    },
    # Add more test cases
]

def run_command(shell_path, command):
    process = subprocess.Popen(
        [shell_path],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True
    )
    stdout, stderr = process.communicate(command + '\nexit\n')  # Ensure the shell exits after the command
    return stdout, stderr, process.returncode

def run_command_with_valgrind(command):
    process = subprocess.Popen(
        ['valgrind', '--leak-check=full', './minishell'],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True
    )
    stdout, stderr = process.communicate(command + '\nexit\n')
    return stdout, stderr, process.returncode

def compare_outputs(output1, output2):
    return output1.strip() == output2.strip()

def check_memory_leaks(valgrind_output):
    leak_summary_start = valgrind_output.find("LEAK SUMMARY:")
    if leak_summary_start == -1:
        return True  # Unable to find leak summary, treat as leak detected
    leak_summary = valgrind_output[leak_summary_start:]
    if "definitely lost: 0 bytes" in leak_summary:
        return False
    else:
        return True

for test in test_cases:
    print(f"Running Test: {test['description']}")
    my_shell_output, my_shell_error, _ = run_command('./minishell', test['command'])
    system_shell_output, system_shell_error, _ = run_command('/bin/bash', test['command'])

    if compare_outputs(my_shell_output, system_shell_output):
        print("Output Match: PASS")
    else:
        print("Output Match: FAIL")
        print(f"Expected Output:\n{system_shell_output}")
        print(f"My Shell Output:\n{my_shell_output}")

    _, valgrind_output, _ = run_command_with_valgrind(test['command'])
    if check_memory_leaks(valgrind_output):
        print("Memory Leak: DETECTED")
        print(valgrind_output)
    else:
        print("Memory Leak: NONE")
    print("-" * 50)
