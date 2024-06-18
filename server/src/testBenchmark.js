const keyService = require('./services/keyService');  // Assurez-vous que ce module est correctement importé
const { validateBenchmark, takeDecision } = require('./services/benchmarkService');

const benchmarks = [
    { cpuMaxFreq: 4000, cpuMinFreq: 1500, cpuCores: 4, memorySize: 8 * 1024 * 1024 * 1024, dataSize: 512 * 1024 * 1024 },
    { cpuMaxFreq: 2500, cpuMinFreq: 1200, cpuCores: 2, memorySize: 4 * 1024 * 1024 * 1024, dataSize: 2 * 1024 * 1024 * 1024 },
    { cpuMaxFreq: 5000, cpuMinFreq: 3000, cpuCores: 8, memorySize: 32 * 1024 * 1024 * 1024, dataSize: 1 * 1024 * 1024 * 1024 },
    { cpuMaxFreq: 1000, cpuMinFreq: 500, cpuCores: 1, memorySize: 2 * 1024 * 1024 * 1024, dataSize: 3 * 1024 * 1024 * 1024 },
    { cpuMaxFreq: 3000, cpuMinFreq: 2000, cpuCores: 4, memorySize: 16 * 1024 * 1024 * 1024, dataSize: 512 * 1024 * 1024},
    { cpuMaxFreq: 5000, cpuMinFreq: 1000, cpuCores: 4, memorySize: 16 * 1024 * 1024 * 1024, dataSize: 1024 * 1024 * 1024},
    { cpuMaxFreq: 5000, cpuMinFreq: 1000, cpuCores: 4, memorySize: 16 * 1024 * 1024 * 1024, dataSize: 1024 * 1024 * 1024 * 1024},
    // Ajoutez d'autres benchmarks ici
];

const displayResults = async () => {
    console.log('Benchmark Results:');
    console.log('---------------------------------------------------------');
    console.log('| CPU Max Freq | CPU Min Freq | CPU Cores | Memory Size | Data Size | Algorithm | Time');
    console.log('---------------------------------------------------------');

    for (const benchmark of benchmarks) {
        //random id
        const identifier = Math.random().toString(36).substring(7);
        const startTime = Date.now();
        const decision = await takeDecision(identifier, benchmark);
        const endTime = Date.now();
        const time = (endTime - startTime) / 1000;
        //delete key
        await keyService.deleteKey(identifier);
        console.log(`| ${benchmark.cpuMaxFreq} MHz | ${benchmark.cpuMinFreq} MHz | ${benchmark.cpuCores} | ${(benchmark.memorySize / (1024 * 1024 * 1024)).toFixed(2)} Go | ${(benchmark.dataSize / (1024 * 1024 * 1024)).toFixed(2)} Go | ${decision.algorithm} | ${time} s`);
    }

    console.log('---------------------------------------------------------');
};

displayResults();
