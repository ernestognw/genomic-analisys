
sequences = open("./src/tests/1/seq.txt", "r")

f = open("./src/tests/1/ref.seq", "r")

original = f.read()

for index, line in enumerate(sequences.readlines()):
    position = original.find(line.strip());
    if position != -1:
        print(f'Line {index} found at {position}')
