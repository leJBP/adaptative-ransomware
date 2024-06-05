#ifndef BENCHMARK_H
#define BENCHMARK_H

struct benchmarkData
{
    int dataSize;
    int cpuCore;
    int cpuMinFreq;
    int cpuMaxFreq;
    int memorySize;
};
typedef struct benchmarkData benchmarkData;

/**
 * @brief Get the benchmark data of the current computer.
 * @return benchmarkData* the benchmark data of the current computer
 * @warning to provide the size of the data to allocate use files_finder, the size is available in the struct
*/
benchmarkData* benchmark_pc(int size);

#endif