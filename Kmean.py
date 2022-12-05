from sklearn.cluster import KMeans
import matplotlib.pyplot as plt

x = [8,3,16,8,16,1,4,0,10,9,10,4,8,2,0]
y = [2,7,2,0,5,9,14,11,3,0,0,5,4,5,1]

data = list(zip(x,y))
print(data)

for i in range(1,16):

	kmeans = KMeans(n_clusters = 2)
	kmeans.fit(data)
	
	
temp = kmeans.predict(data)

print(temp)

e = kmeans.cluster_centers_

print(e)

plt.scatter(x,y,c = kmeans.labels_)

plt.show()
