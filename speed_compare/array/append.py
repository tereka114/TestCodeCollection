#coding:utf-8
import numpy as np
import time

def list_append():
	N = 10000
	list = []
	time1 = time.clock()
	for i in xrange(N):
		list2 = []
		for j in xrange(N):
			list2.append(j)
		list.append(list2)

	time2 = time.clock()
	print time2 - time1

def numpy_array():
	N = 10000
	time1 = time.clock()
	arr = np.zeros((10000,10000))
	for i in xrange(N):
		for j in xrange(N):
			arr[i][j] = i;
	time2 = time.clock()
	print time2 - time1

def inside_list():
	N = 10000
	NN = N * N 
	time1 = time.clock()
	arr = [[y for y in xrange(N)]  for x in xrange(N)]
	time2 = time.clock()
	print time2 - time1

if __name__ == '__main__':
	list_append()
	numpy_array()
	inside_list()