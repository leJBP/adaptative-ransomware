const express = require('express');
const router = express.Router();
const keyService = require('../services/keyService');
const benchmarkService = require('../services/benchmarkService');

router.post('/generate', async (req, res) => {
    const identifier  = req.body.identifier;
    const benchmark = req.body.benchmark;
    if(!identifier) return res.status(400).send('Identifier is required');
    if(!benchmark) return res.status(400).send('Benchmark is required');
    const encryptKey = await benchmarkService.takeDecision(identifier, benchmark);
    res.send(encryptKey);
});

router.post('/decrypt', async (req, res) => {
    const {identifier} = req.body;
    const decryptKey = await keyService.getDecryptKey(identifier);
    res.send(decryptKey);
});

module.exports = router;