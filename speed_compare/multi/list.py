#coding:utf-8
import numpy as np
import time

N = 100000000;

def inside_list():
	global N

	list1 = [i for i in xrange(N)]
	list2 = [i for i in xrange(N)]

	time1 = time.clock()

	for i in xrange(N):
		list1[i] * list2[i]

	time2 = time.clock()

	print time2 - time1

def numpy_array():
	global N
	arr1 = np.arange(0,N)
	arr2 = np.arange(0,N)

	time1 = time.clock()

	for i in xrange(N):
		arr1[i] * arr2[i]

	time2 = time.clock()
	print time2 - time1

def numpy_sum_array():
	global N
	arr1 = np.arange(0,N)
	arr2 = np.arange(0,N)

	time1 = time.clock()

	arr1 * arr2

	time2 = time.clock()
	print time2 - time1

if __name__ == '__main__':
	inside_list()
	numpy_array()
	numpy_sum_array()