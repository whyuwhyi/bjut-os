import subprocess

for _ in range(1000):
    result = subprocess.run(['./task3'], capture_output=True, text=True)
    for line in result.stdout.split('\n'):
        if "The sum of the one thousand data is" in line:
            print(line)