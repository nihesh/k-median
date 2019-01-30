import random
print("5 5 4 3")

coords = []
for i in range(11):
	coords.append(i)

clients = random.sample(coords, 5)
for val in clients:
	print(val, end=" ")
print()

facilities = random.sample(coords, 5)
for val in facilities:
	print(val, end=" ")
print()
