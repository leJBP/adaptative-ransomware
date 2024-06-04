const keyService = require('./keyService');
const { body } = require('express-validator');

validateBenchmark = [
    body('cpuMaxFreq').notEmpty().isNumeric().trim().escape(),
    body('cpuMinFreq').notEmpty().isNumeric().trim().escape(),
    body('cpuCores').notEmpty().isNumeric().trim().escape(),
    body('memorySize').notEmpty().isNumeric().trim().escape(),
    body('dataSize').notEmpty().isNumeric().trim().escape()
]

// Valeurs de références pour le benchmark en octets
const REFERENCE_VALUES = {
    cpuMaxFreq: 3000000000,
    cpuMinFreq: 1000000000,
    cpuCores: 4,
    memorySize: 8589934592,
    dataSize: 1000000
}

// Pondération des valeurs de références
const REFERENCE_WEIGHTS = {
    cpuMaxFreq: 0.3,
    cpuMinFreq: 0.1,
    cpuCores: 0.3,
    memorySize: 0.1,
    dataSize: 0.2
}

const takeDecision = async (identifier, benchmark) => {
    // Comparer le benchmark avec les valeurs de référence
    const score = compareBenchmark(benchmark);
    console.log('score', score);
    if(score >= 75) {
        console.log('generateRSA4096Key');
        return await keyService.generateRSA4096KeyPair(identifier);
    }
    else if (score >= 50) {
        console.log('generateRSA2048Key');
        return await keyService.generateRSA2048KeyPair(identifier);
    }
    else if(score >= 25) {
        console.log('generateCHACHA20Key');
        return await keyService.generateAESKey(identifier);
    }
    else if(score < 25) {
        console.log('generateAESKey');
        return await keyService.generateAESKey(identifier);
    }
}

const compareBenchmark = (benchmark) => {
    // Extraire les valeurs du benchmark
    const { cpuMaxFreq, cpuMinFreq, cpuCores, memorySize, dataSize } = benchmark;

    // Calculer le score pour chaque paramètre, en fonction de la valeur de référence
    const scores = {
        cpuMaxFreq: (cpuMaxFreq / REFERENCE_VALUES.cpuMaxFreq) * REFERENCE_WEIGHTS.cpuMaxFreq,
        cpuMinFreq: (cpuMinFreq / REFERENCE_VALUES.cpuMinFreq) * REFERENCE_WEIGHTS.cpuMinFreq,
        cpuCores: (cpuCores / REFERENCE_VALUES.cpuCores) * REFERENCE_WEIGHTS.cpuCores,
        memorySize: (memorySize / REFERENCE_VALUES.memorySize) * REFERENCE_WEIGHTS.memorySize,
        dataSize: ((REFERENCE_VALUES.dataSize / dataSize) - 1) * REFERENCE_WEIGHTS.dataSize  // Inversé car moins c'est mieux
    };

    // Calculer le score total en sommant les scores de chaque paramètre
    let totalScore = 0;
    for (const score of Object.values(scores)) {
        totalScore += score;
    }
    console.log('totalScore', totalScore);

    // Normaliser le score total pour qu'il soit entre 0 et 100
    totalScore = totalScore * 100; // Ajustez cette multiplication pour obtenir une échelle de 0 à 100 selon les besoins
    totalScore = Math.max(0, Math.min(100, totalScore)); // Clamp le score entre 0 et 100

    return totalScore;
}


module.exports = {
    validateBenchmark,
    takeDecision
}