CC = g++
FLAGS = -g -Wall
OUT = run
OBJS = main.o file_functions.o curve.o distances.o
EIGEN_PATH = eigen

run: $(OBJS)
	$(CC) $(FLAGS) $^ -o $(OUT) -I $(EIGEN_PATH)

main.o: main.cpp
	$(CC) $(FLAGS) -c $< -o $@ -I $(EIGEN_PATH)

%.o: %.cpp %.h
	$(CC) $(FLAGS) -c $< -o $@ -I $(EIGEN_PATH)

clean:
	rm -f $(OBJS) $(OUT)
