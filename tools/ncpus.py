import multiprocessing
import argparse

def print_ncpus():
	parser = argparse.ArgumentParser()
	parser.add_argument("-m", help="max cpus allowed", type = int)
	args = parser.parse_args()

	ncpus = multiprocessing.cpu_count()
	if (args.m and args.m < ncpus):
		ncpus = args.m

	print(ncpus)

if __name__ == '__main__':
	print_ncpus()
