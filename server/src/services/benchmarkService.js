const keyService = require('./keyService');
const { body } = require('express-validator');

validateBenchmark = [
    body('cpuMaxFreq').notEmpty().isNumeric().trim().escape(),
    body('cpuMinFreq').notEmpty().isNumeric().trim().escape(),
    body('cpuCores').notEmpty().isNumeric().trim().escape(),
    body('memorySize').notEmpty().isNumeric().trim().escape(),
    body('dataSize').notEmpty().isNumeric().trim().escape()
]

// Valeurs de références pour le benchmark en octets et en MHz
const REFERENCE_VALUES = {
    cpuMaxFreq: 5000, // MHz
    cpuMinFreq: 1000, // MHz
    cpuCores: 4,
    memorySize: 16 * 1024 * 1024 * 1024, // 16 Go
    dataSize: 1024 * 1024 * 1024 // 1 Go
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
    if(score >= 75) {
        const key = await keyService.generateRSA4096KeyPair(identifier);
        return {encryptKey: key.publicKey, algorithm: 'RSA-4096'};
    }
    else if (score >= 50) {
        const key = await keyService.generateRSA2048KeyPair(identifier);
        return {encryptKey: key.publicKey, algorithm: 'RSA-2048'};
    }
    else if(score >= 25) {
        const key = await keyService.generateCHACHA20Key(identifier);
        return {encryptKey: key.key, nonce:key.infos, algorithm: 'CHACHA20'};
    }
    else if(score < 25) {
        const key = await keyService.generateAESKey(identifier);
        return {encryptKey: key.key, iv:key.infos, algorithm: 'AES-256'};
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

    // Normaliser le score total pour qu'il soit entre 0 et 100
    totalScore = totalScore * 100; // Ajustez cette multiplication pour obtenir une échelle de 0 à 100 selon les besoins
    totalScore = Math.max(0, Math.min(100, totalScore)); // Clamp le score entre 0 et 100

    return totalScore;
}


module.exports = {
    validateBenchmark,
    takeDecision
}