with open("input.txt") as f:
    lines = f.readlines()
output = []
for i in range(0, len(lines), 3):
    output.append(lines[i].strip())
with open("output.txt", "w") as f:
    for line in output:
        f.write(line + "\n")