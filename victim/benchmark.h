#ifndef BENCHMARK_H
#define BENCHMARK_H

#define MAC_ADDR_LEN 6

struct benchmarkData
{
    long dataSize;
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

/**
 * @brief Get the identifier of the current computer, the MAC address will be the unique identifier.
 */
unsigned char* get_identifier();

#endif