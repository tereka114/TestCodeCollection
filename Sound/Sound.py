#coding:utf-8
import os
import sys
import wave
import pyublas
import scipy
import numpy as np
import re
import spectrum

class Sound(object):
	"""docstring for ClassName"""
	def __init__(self, filename):
		#音のパラメータ
		self.filename = filename
		self.period = 30 #音を取り出す秒数
		self.filedata,self.fs = self.readFile(filename) #波形データとサンプリング周波数
		self.length = len(self.filedata) #音の長さ
		self.start = 0
		self.end = len(self.filedata)-1

		#Fooriar Transform のパラメータ
		self.framenum = int(512) #音のフレーム
		self.sframenum = int(self.fs * 0.01) #音のシフト数
		self.fscale = np.fft.fftfreq(self.framenum,d=1.0/ self.fs) #フーリエ変換のスケールを保存した配列
		self.hamming_window = np.hamming(self.framenum)

		#FeatureExtract
		self.fft_parameter = [] #フーリエ変換
		self.powor_spectrum = [] #パワースペクトラム
		self.mfcc = [] #MFCC,Δ,ΔΔ
		self.emd_signal = []
		self.imf = []

	def readFile(self,filename):
		if filename.endswith(".mp3"):
			self.resampleMp3(filename)
			filename = filename.replace(".mp3",".wav")

		os.system("sox -t raw -c 1 -r 16000 -b 16 -e signed-integer '%s' -t wav '%s'" %(filename,filename.replace(".wav","_temp.wav")))
		filename = filename.replace(".wav","_temp.wav")

		wf = wave.open(filename,"rb")
		fs = wf.getframerate()
		channnel= wf.getnchannels()
		x = wf.readframes(wf.getnframes())
		x = scipy.fromstring(x,dtype="int16")
		wf.close()

		start = len(x) / 2 - fs * self.period
		end = len(x) / 2 + fs * self.period

		if start < 0: start = 0
		if end > len(x)-1: end = len(x)-1

		write_wave = wave.Wave_write(filename.replace("_temp.wav","_write.wav"))
		write_wave.setparams(wf.getparams())
		write_wave.writeframes(x[start:end])
		self.filename = filename.replace("_temp.wav","_write.wav")

		self.start = start
		self.end = end

		return x[start:end],float(fs)

	def resampleMp3(self,file):
		os.system("lame --decode '%s' '%s'" % (file,file.replace(".mp3",".wav")))

	def featureExtract(self):
		start = 0;

		while True:
			if start + self.framenum > self.end:
				end = self.end
				window_data = np.hamming(end - start) * self.filedata[start:end]
			else:
				end = start + self.framenum
				window_data = self.hamming_window * self.filedata[start:end]

			fftwave = np.fft.fft(window_data)
			Pwave = np.abs(fftwave) ** 2
			self.fft_parameter.append(fftwave)
			self.powor_spectrum.append(Pwave)

			start += self.sframenum
			if start > self.end: break

	def htkParameter(self):
		os.remove('config.hcopy')

		file = open('config.hcopy','w')
		param = 'SOURCEFORMAT = WAVE\n'
		param +='SOURCEKIND = WAVEFORM\n'
		param +='TARGETKIND = MFCC_D_A\n'
		param +='TARGETRATE = 100000.0\n'
		param +='SAVECOMPRESSED = T\n'
		param +='WINDOWSIZE = 320000.0\n'
		param +='USEHAMMING = T\n'
		param +='PREEMCOEF = 0.97\n'
		param +='NUMCHANS = 20\n'
		param +='NUMCEPS = 12\n'
		param +='CEPLIFTER = 22\n'
		param +='ENORMALISE = F\n'
		param +='DELTAWINDOW = 2\n'
		param +='ACCWONDOW = 2\n'
		file.writelines(param)

	def formatExchange(self,tempfile,textfile):
		box = []
		s = []
		flag = 0
		f = open(textfile, 'w')
		for line in open(tempfile, 'r'):
		    if "0:" in line and flag == 0:
		        flag = 1
		    
		    if flag == 1:
		       match = re.split(r' +', line)
		       for m in match:
		         if ":" in m or "END" in match:
		           for i in xrange(len(box)):
		              if i == (len(box) - 1):
		                 s += box[i] + '\n'
		              elif "-10000000000.000" in box[i]:
		                 a = box[i].replace("-10000000000.000", "")
		                 s += a + ' '
		                 s += "-10000000000.000" + ' '
		              else:
		                 s += box[i] + ' '

		           f.writelines(s)
		           s = []
		           box = []
		         elif m == "":
		           a = 0
		         else:
		           m = m.replace('\n','') 
		           box.append(m)
		             
		f.writelines(s)

	def htkFileLoad(self,mfccfile):
		fp = open(mfccfile, "r")
		ans = []
		count = 1
		for line in fp:
			for x in line.rstrip().split():
				ans.append(float(x))
		fp.close()

		return np.array(ans).reshape(len(ans)/36,36)

	def extractMFCC(self):
		self.htkParameter()
		mfccfile = self.filename.replace('_write.wav','.mfc')
		tempfile = self.filename.replace('_write.wav','_temp.txt')
		textfile = self.filename.replace('_write.wav','.txt')
		f = open('codstr.scp','w')
		s = self.filename + ' ' + mfccfile + '\n'
		f.writelines(s)
		f.close()

		os.system("HCopy -T 1 -C config.hcopy -S codstr.scp")
		os.system("HList -o -h '%s' > '%s'" % (mfccfile,tempfile))
		self.formatExchange(tempfile,textfile)
		self.mfcc = self.htkFileLoad(textfile)

	# def emd(self):
	# 	spectrum.emd(self.filedata)

if __name__ == '__main__':
	path = sys.argv[1]
	sound = Sound(path)
	sound.featureExtract()
	sound.extractMFCC()