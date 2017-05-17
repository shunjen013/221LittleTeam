filename = "outputlog2_2.txt"
file = open(filename, 'r')
for line in file.readlines():
	if "Ans" in line:
		print line.split()[1]