const express = require('express');
const router = express.Router();
const keyService = require('../services/keyService');

router.post('/generate', async (req, res) => {
    const identifier  = req.body.identifier;
    const { publicKey } = await keyService.generateKeyPair(identifier);
    res.send( publicKey );
});

router.post('/decrypt', async (req, res) => {
    const { identifier } = req.body;
    const privateKey = await keyService.getPrivateKey(identifier);
    res.send( privateKey );
});

module.exports = router;