import subprocess
print("Flashing clock generator resisters\n")

print subprocess.check_output(['/usr/local/bin/dfu-util', '-d', '483:df11',  '-a', '0', '-c', '1', '-s', '0x08060000', '-D', '5P49V5901.txt'])