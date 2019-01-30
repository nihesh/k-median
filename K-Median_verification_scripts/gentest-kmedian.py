import random

MAX_SIZE = 13

CLIENTS = MAX_SIZE
FACILITIES = MAX_SIZE
OPEN = random.randint(2,MAX_SIZE)
REQMENT = random.randint(1,OPEN)

print(CLIENTS, FACILITIES, REQMENT, OPEN)

coords = []
for i in range(2*MAX_SIZE):
	coords.append(i)

clients = random.sample(coords, CLIENTS)
for val in clients:
	print(val, end=" ")
print()

facilities = random.sample(coords, FACILITIES)
for val in facilities:
	print(val, end=" ")
print()
