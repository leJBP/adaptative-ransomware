const crypto = require('crypto');
const AsymetricKey = require('../models/asymetricKeys');
const SymetricKey = require('../models/symetricKeys');

/** Clés Symétriques **/
// Génère une clé symétrique AES-256 aléatoire
const generateAESKey = async (identifier) => {
    const key = crypto.randomBytes(32).toString('hex'); // Génère une clé AES-256 aléatoire

    const keys = await SymetricKey.findOne({where: {identifier}});

    if (keys) {
        return {key: key};
    }
    await SymetricKey.create({identifier, key});

    return {key};
}

// Récupère la clé symétrique AES-256 associée à un identifiant
const getAESKey = async (identifier) => {
    const key = await SymetricKey.findOne({ where: { identifier } });
    return key ? key.key : null;
}

// Génère une clé symétrique CHACHA20
const generateCHACHA20Key = async (identifier) => {

    const key = crypto.randomBytes(32).toString('hex'); // Génère une clé CHACHA20 aléatoire

    const keys = await SymetricKey.findOne({where: {identifier}});

    if (keys) {
        return {key: key};
    }
    await SymetricKey.create({identifier, key});

    return {key};
}

// Récupère la clé symétrique CHACHA20 associée à un identifiant
const getCHACHA20Key = async (identifier) => {
    const key = await SymetricKey.findOne({ where: { identifier } });
    return key ? key.key : null;
}

/** Clés Asymétriques **/
// Génère une paire de clés RSA-4096
const generateRSA4096KeyPair = async (identifier) => {

    const { publicKey, privateKey } = crypto.generateKeyPairSync('rsa', {
        modulusLength: 4096,
        publicKeyEncoding: { type: 'spki', format: 'pem' },
        privateKeyEncoding: { type: 'pkcs8', format: 'pem' },
    });

    const keys = await AsymetricKey.findOne({ where: { identifier } });

    if(keys)
    {
        return { publicKey: keys.publicKey, privateKey: keys.privateKey };
    }
    await AsymetricKey.create({ identifier, publicKey, privateKey });

    return { publicKey };
};

// Récupère la clé privée RSA-4096 associée à un identifiant
const getRSA4096PrivateKey = async (identifier) => {
    const key = await AsymetricKey.findOne({ where: { identifier } });
    return key ? key.privateKey : null;
};

// Génère une paire de clés RSA-2048
const generateRSA2048KeyPair = async (identifier) => {
    const { publicKey, privateKey } = crypto.generateKeyPairSync('rsa', {
        modulusLength: 2048,
        publicKeyEncoding: { type: 'spki', format: 'pem' },
        privateKeyEncoding: { type: 'pkcs8', format: 'pem' },
    });

    const keys = await AsymetricKey.findOne({ where: { identifier } });

    if(keys)
    {
        return { publicKey: keys.publicKey, privateKey: keys.privateKey };
    }
    await AsymetricKey.create({ identifier, publicKey, privateKey });

    return { publicKey };
};

// Récupère la clé privée RSA-2048 associée à un identifiant
const getRSA2048PrivateKey = async (identifier) => {
    const key = await AsymetricKey.findOne({ where: { identifier } });
    return key ? key.privateKey : null;
};

/** Other functions **/
// Trouve si l'identifiant est dans la table symetricKeys ou asymetricKeys
const findKey = async (identifier) => {
    const symetricKeys = await SymetricKey.findOne({ where: { identifier } });
    const asymetricKeys = await AsymetricKey.findOne({ where: { identifier } });

    return symetricKeys || asymetricKeys;
}

// Renvoie la clé de déchiffrement associée à un identifiant
const getDecryptKey = async (identifier) => {
    const key = await findKey(identifier);
    // Si la clé est symétrique, on la renvoie
    if (key && key.key) {
        return key.key;
    }
    // Si la clé est asymétrique, on renvoie la clé privée
    if (key && key.privateKey) {
        return key.privateKey;
    }
    return null;
}

//Export all module to generate
module.exports = {
    generateAESKey,
    generateCHACHA20Key,
    generateRSA4096KeyPair,
    generateRSA2048KeyPair,
    getDecryptKey
};