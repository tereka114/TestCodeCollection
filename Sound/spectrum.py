#coding:utf-8
import numpy as np
from scipy import interpolate
from pylab import *
import time

# ZeroCrossRate
def zcross(x):
	znum = np.array([1 if x[i] > 0 else 0 for i in xrange(len(x))])
	a_znum = znum[1:len(x)]
	b_znum = znum[0:len(x)-1]
	zcr = 0.5 * (np.sum(np.abs(a_znum - b_znum)))
	return zcr

def make_spline_matrix(x,y):
	""" 連立方程式用の行列を作成 """
	N = len(x)
	matrix = []
	h = [x[i+1] - x[i] for i in xrange(N-1)]
	v = [6*((y[i+1]-y[i])/h[i] - (y[i]-y[i-1])/h[i-1]) for i in xrange(1,N-1)]
	v.insert(0,0.0)
	v.append(0.0)
	temp = []
	time1 = time.clock()
	for i in xrange(N):
		if i == 0:
			temp.append(1.0)
		else:
			temp.append(0.0)
	matrix.append(temp)

	time2 = time.clock()
	print "time1",time2 - time1

	for i in xrange(N-2):
		temp = []
		for j in xrange(N):
			if j == i:
				temp.append(h[i])
			elif j == i+1:
				temp.append(2*(h[i]+h[i+1]))
			elif j == (i + 2):
				temp.append(h[i+1])
			else:
				temp.append(0.0)
		matrix.append(temp)

	temp = []
	time3 = time.clock()
	print "time2",time3-time2

	for i in xrange(N):
		if i == N-1:
			temp.append(1.0)
		else:
			temp.append(0.0)
	matrix.append(temp)
	return np.dot(np.matrix(np.linalg.inv(matrix)),np.matrix(v).reshape(len(v),1))

def spline_coeffient(x,y):
	u = make_spline_matrix(x,y)
	u = u.A1.tolist()
	a,b,c,d = [],[],[],[]
	for j in xrange(0,len(x)-1):
		a.append((u[j+1] - u[j]) / (3*(x[j+1] - x[j])))
		b.append(u[j])
		c.append(((y[j+1] - y[j]) / (x[j+1] - x[j]))-((x[j+1]-x[j])*(u[j+1]+2*u[j])/3))
		d.append(y[j])
	return a,b,c,d

def spline(x,pre_x,pre_y):
	a,b,c,d = spline_coeffient(pre_x,pre_y)
	y = []
	for i in xrange(len(x)):
		for j in xrange(len(a)):
			if x[i] <= pre_x[0]:
				break
			if x[i] > pre_x[j] and x[i] <= pre_x[j+1]:
				break
		y.append(a[j]*(x[i]-pre_x[j])**3+b[j]*(x[i]-pre_x[j])**2+c[j]*(x[i]-pre_x[j])+d[j])
	return y

def emd(x):
	signal = np.array(x)
	imf = []
	while(1):
		maximum_value = np.array([float(signal[i]) for i in xrange(1,len(x)-1) if signal[i-1] < signal[i] and signal[i] > signal[i+1]])
		maximum_axis = np.array([float(i) for i in xrange(1,len(signal)-1) if signal[i-1] < signal[i] and signal[i] > signal[i+1]])
		minimum_value = np.array([float(signal[i]) for i in xrange(1,len(x)-1) if signal[i-1] > signal[i] and signal[i+1] > signal[i]])
		minimum_axis = np.array([float(i) for i in xrange(1,len(signal)-1) if signal[i-1] > signal[i] and signal[i+1] > signal[i]])

		#print spline(np.arange(0,len(x)),minimum_axis,minimum_value),len(x)
		ave = (np.array(spline(np.arange(0,len(x)),maximum_axis,maximum_value)) + np.array(spline(np.arange(0,len(x)),minimum_axis,minimum_value)))/2.0
		imf.append(ave)
		n_signal = signal - ave
		#print maximum_value,maximum_axis,minimum_value,minimum_axis

		y = np.arange(0,len(x))
		plot(y,signal)
		plot(y,np.array(spline(np.arange(0,len(x)),maximum_axis,maximum_value)))
		plot(y,np.array(spline(np.arange(0,len(x)),minimum_axis,minimum_value)))
		show()

		""" 終了判定 """
		sd = 0.0
		for i in xrange(len(ave)):
			sd += np.abs(n_signal[i] - signal[i])**2 / (n_signal[i] * n_signal[i])
		print sd

		signal = n_signal
		print x,y
		if sd < 0.01:
			break
	return signal,imf

if __name__ == '__main__':
	emd([1,2,34,5,6,7,4,2,100,32,1,4,10,30])