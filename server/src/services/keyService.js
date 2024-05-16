const crypto = require('crypto');
const Key = require('../models/keys');

const generateKeyPair = async (identifier) => {
    const { publicKey, privateKey } = crypto.generateKeyPairSync('rsa', {
        modulusLength: 4096,
        publicKeyEncoding: { type: 'spki', format: 'pem' },
        privateKeyEncoding: { type: 'pkcs8', format: 'pem' },
    });

    const keys = await Key.findOne({ where: { identifier } });

    if(keys)
    {
        return { publicKey: keys.publicKey, privateKey: keys.privateKey };
    }
    await Key.create({ identifier, publicKey, privateKey });

    return { publicKey, privateKey };
};

const getPrivateKey = async (identifier) => {
    const key = await Key.findOne({ where: { identifier } });
    return key ? key.privateKey : null;
};

module.exports = { generateKeyPair, getPrivateKey };