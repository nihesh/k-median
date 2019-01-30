import random

CLIENTS = 15
FACILITIES = 15
REQMENT = 4

print(CLIENTS, FACILITIES, REQMENT)

coords = []
for i in range(20):
	coords.append(i)

clients = random.sample(coords, CLIENTS)
for val in clients:
	print(val, end=" ")
print()

facilities = random.sample(coords, FACILITIES)
for val in facilities:
	print(val, end=" ")
print()

opening = random.sample(coords, FACILITIES)
for val in opening:
	print(val, end=" ")
print()
