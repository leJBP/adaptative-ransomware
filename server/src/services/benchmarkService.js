const keyService = require('./keyService');

const analyzeBenchmark = async (benchmark) => {
    //Use the benchmark results to calculate a score from 0 to 100
    // Calculate the score
    const score = 45 //benchmark.ram + benchmark.cpu + benchmark.size;
    console.log(score)

    // Return the score
    return score;
}

const takeDecision = async (identifier, benchmark) => {
    // Get the decision from the score
    let encryptKey;

    const score = await analyzeBenchmark(benchmark);
    // Calculate the decision
        if(score >= 80)
        {
            encryptKey = keyService.generateRSA4096KeyPair(identifier);
        }
        else if(score >= 60)
        {
            encryptKey = keyService.generateRSA2048KeyPair(identifier);
        }
        else if(score >= 40)
        {
            encryptKey = keyService.generateCHACHA20Key(identifier);
        }
        else
        {
            encryptKey = keyService.generateAESKey(identifier);
        }
    // Return the decision
    return encryptKey;
}

module.exports = { analyzeBenchmark, takeDecision };