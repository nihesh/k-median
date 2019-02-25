import random
import numpy as np

MAX_SIZE = 13

def type1():		# Uniform
	
	global MAX_SIZE

	CLIENTS = MAX_SIZE
	FACILITIES = MAX_SIZE
	OPEN = random.randint(2,MAX_SIZE)
	REQMENT = random.randint(1,OPEN)

	print(CLIENTS, FACILITIES, OPEN)

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

	for i in range(CLIENTS):
		print(random.randint(1,REQMENT), end=" ")
	print()

def type2():		# Gaussian

	global MAX_SIZE

	CLIENTS = MAX_SIZE
	FACILITIES = MAX_SIZE
	OPEN = random.randint(2,MAX_SIZE)
	REQMENT = random.randint(1,OPEN)

	print(CLIENTS, FACILITIES, OPEN)

	MU_CLIENT = 0
	MU_FACILITY = 20
	SIG_CLIENT = 30
	SIG_FACILITY = 40

	for val in range(CLIENTS):
		print(int(np.random.normal(MU_CLIENT,SIG_CLIENT,1)), end=" ")
	print()

	for val in range(FACILITIES):
		print(int(np.random.normal(MU_FACILITY,SIG_FACILITY,1)), end=" ")
	print()

	for i in range(CLIENTS):
		print(random.randint(1,REQMENT), end=" ")
	print()

def type3():		# Gaussian

	global MAX_SIZE

	CLIENTS = MAX_SIZE
	FACILITIES = MAX_SIZE
	OPEN = random.randint(2,MAX_SIZE)
	REQMENT = random.randint(1,OPEN)

	MU_CLIENT = 0
	MU_FACILITY = 20
	SIG_CLIENT = 20
	SIG_FACILITY = 20

	print(CLIENTS, FACILITIES, OPEN)

	for val in range(CLIENTS):
		print(random.sample([int(np.random.normal(MU_CLIENT,SIG_CLIENT,1)), int(np.random.normal(2*MU_FACILITY - MU_CLIENT,SIG_CLIENT,1))], 1)[0], end=" ")
	print()

	for val in range(FACILITIES):
		print(int(np.random.normal(MU_FACILITY,SIG_FACILITY,1)), end=" ")
	print()

	for i in range(CLIENTS):
		print(random.randint(1,REQMENT), end=" ")
	print()

type3()


