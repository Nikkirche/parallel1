# Build
install all packages from vckpg.json by vckpg and run cmake.
# Run 
Data was captured on AMD Ryzen 5 7535U (12) @ 4.63 GHz with
taskset -c 0-3 and is located in data.txt file.
There are two targets in the project - perf checks algorithm for correctness, 
perf checks for speed.
I have written initially parallel algorithm(parallel in code) based on algorithm from lecture, but it is not fast enough, it gives only speedup around of 2x.
Sillier algorithm(parallel_without_copy in code) with only fork join makes a speedup around of 2.9x. 