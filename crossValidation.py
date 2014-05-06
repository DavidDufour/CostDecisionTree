import sys
import os	
from random import shuffle

def k_fold_cross_validation(items, k, randomize=False):

	if randomize:
		items = list(items)
		shuffle(items)

	slices = [items[i::k] for i in xrange(k)]

	for i in xrange(k):
		validation = slices[i]
		training = [item
					for s in slices if s is not validation
					for item in s]
		yield training, validation

if __name__ == '__main__':
	header = ""
	examples = []
	
	fileName = os.path.splitext(sys.argv[1])[0]

	input = open(str(sys.argv[1]), 'r')
	for line in input:
		if line[0] != '@' and line[0] != '\n':
		  examples.append(line)
		else:
			header += line
	input.close()

	i = 0;
	for training, validation in k_fold_cross_validation(examples, 10, True):
		
		train = open(fileName + 'Train' + str(i) + '.arff', 'w')		
		test = open(fileName + 'Test' + str(i) + '.arff', 'w')	
		
		train.write(header)	

		for t in training:
			train.write(t)
		for v in validation:
			test.write(v)

		train.close()
		test.close()

		data = open(fileName + '.data', 'w')		
		Ctest = open(fileName + '.test', 'w')

		for t in training:
			data.write(t)
		for v in validation:
			Ctest.write(v)
		
		data.close()
		Ctest.close()

		#put all programs to test here and you're done :D

		#os.system("./ID3 ./" + fileName + "Train" + str(i) + ".arff ./" + fileName + "Tree" + str(i) + ".cpp >> size")
		#os.system("g++ ./" + fileName + "Tree" + str(i) + ".cpp ; ./a.out ./" + fileName + "Test" + str(i) + ".arff >> accuracy")

		os.system("./D4 ./" + fileName + "Train" + str(i) + ".arff " + fileName + ".cost ./Tree" + str(i) + ".cpp >> D4size")
		os.system("g++ ./Tree" + str(i) + ".cpp ; ./a.out ./" + fileName + "Test" + str(i) + ".arff >> D4accuracy")

		os.system("./c4.5 -m 1 -u -g -f ./" + fileName + " >> C4.5size")

		i += 1;

	os.system("rm a.out")
	os.system("rm Tree*.cpp")
	os.system("rm " + fileName + ".data")
	os.system("rm " + fileName + ".test")
	os.system("rm " + fileName + ".tree")
	os.system("rm " + fileName + ".unpruned")
	os.system("rm ./Data/*Test*.arff")
	os.system("rm ./Data/*Train*.arff")
