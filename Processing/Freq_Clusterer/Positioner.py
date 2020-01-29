import csv
import numpy as np

def positioner():
	with open('Position.csv') as csvfile:
		readCSV = np.asarray(list(csv.reader(csvfile, delimiter=',')))
		#print(sum(1 for line in readCSV))
		print("readCSV shape", readCSV.shape)

		Tx = (readCSV[:,0])[1:]
		X = (readCSV[:,1])[1:]
		Ty = (readCSV[:,2])[1:]
		Y = (readCSV[:,3])[1:]
		#print(T,X)
		Xgathered = np.zeros(int(X.size/12))
		Ygathered = np.zeros(int(Y.size/12))
		
		i = 0
		while(i<int(X.size/12)):
			Xgathered[i] = int("".join(str(x) for x in X[12*i:12*i+12]), 2) 
			i = i + 1
		i = 0
		while(i<int(Y.size/12)):
			Ygathered[i] = int("".join(str(y) for y in Y[12*i:12*i+12]), 2) 
			i = i + 1
		return (Xgathered, Tx[0::12], Ygathered, Ty[0::12])
def main():
	positionArray = positioner()
	print("Position and time data: ", positionArray)

if __name__ == '__main__':
	main()