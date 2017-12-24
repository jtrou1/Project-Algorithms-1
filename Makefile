CC = g++
FLAGS = -g -Wall
OUT = run
OBJS = main.o file_functions.o curve.o help_functions.o cluster.o distances.o binary_mean_tree.o hash_functions.o hashtable.o list.o initialization.o assignment.o update.o
EIGEN_PATH = eigen

run: $(OBJS)
	$(CC) $(FLAGS) $^ -o $(OUT) -I $(EIGEN_PATH)

main.o: main.cpp
	$(CC) $(FLAGS) -c $< -o $@ -I $(EIGEN_PATH)

%.o: %.cpp %.h
	$(CC) $(FLAGS) -c $< -o $@ -I $(EIGEN_PATH)

clean:
	rm -f $(OBJS) $(OUT)
