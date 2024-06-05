const express = require('express');
const router = express.Router();
const keyService = require('../services/keyService');
const benchmarkService = require('../services/benchmarkService');


router.post('/generate', benchmarkService.validateBenchmark, async (req, res) => {
    const identifier  = req.body.identifier;
    const benchmark = req.body.benchmark;
    if(!identifier) return res.status(400).send('Identifier is required');
    if(!benchmark) return res.status(400).send('Benchmark is required');
    try {
        if(await keyService.findKey(identifier)) return res.status(400).send('Key already exists');
        const {encryptKey, algorithm} = await benchmarkService.takeDecision(identifier, benchmark);
        res.send({encryptKey, algorithm});
    }
    catch (err) {
        res.status(400).send(err.message);
    }
});

router.post('/decrypt', async (req, res) => {
    const {identifier} = req.body;
    const {decryptKey, algorithm} = await keyService.getDecryptKey(identifier);
    res.send({decryptKey, algorithm});
});

module.exports = router;