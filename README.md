# Balanced trees - AVL and RB with cpp
Implementations of two kinds of balanced trees which are AVL and RB respectively.
It's implemented by c++

# Environment
> WSL2 Linux

> g++ (Ubuntu 11.3.0-1ubuntu1~22.04.1) 11.3.0

> g++ -o main main.cpp

I confirmed that no memory leak occurred using the following command.
valgrind --leak-check=yes ./main
valgrind --leak-check=yes --leak-check=full --show-leak-kinds=all ./main
Thank you.