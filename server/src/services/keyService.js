const crypto = require('crypto');
const AsymetricKey = require('../models/asymetricKeys');
const SymetricKey = require('../models/symetricKeys');

/** Clés Symétriques **/
// Génère une clé symétrique AES-256 aléatoire
const generateAESKey = async (identifier) => {
    const keys = await SymetricKey.findOne({where: {identifier}});

    if (keys) {
        return {key: keys.key};
    }

    const key = crypto.randomBytes(32).toString('base64'); // Génère une clé AES256 aléatoire
    const iv = crypto.randomBytes(16).toString('base64'); // Génère un nonce AES256 aléatoire

    return await SymetricKey.create({identifier, key, infos: iv, algorithm: "AES-256"});
}

// Récupère la clé symétrique AES-256 associée à un identifiant
const getAESKey = async (identifier) => {
    const key = await SymetricKey.findOne({where: {identifier}});
    return key ? key.key : null;
}

// Génère une clé symétrique CHACHA20
const generateCHACHA20Key = async (identifier) => {
    const keys = await SymetricKey.findOne({where: {identifier}});

    if (keys) {
        return {key: keys.key};
    }

    const key = crypto.randomBytes(32).toString('base64'); // Génère une clé CHACHA20 aléatoire
    const nonce = crypto.randomBytes(12).toString('base64'); // Génère un nonce CHACHA20 aléatoire

    return await SymetricKey.create({identifier, key, infos: nonce, algorithm: "CHACHA20"});
}

// Récupère la clé symétrique CHACHA20 associée à un identifiant
const getCHACHA20Key = async (identifier) => {
    const key = await SymetricKey.findOne({where: {identifier}});
    return key ? key.key : null;
}

/** Clés Asymétriques **/
// Génère une paire de clés RSA-4096
const generateRSA4096KeyPair = async (identifier) => {
    const keys = await AsymetricKey.findOne({where: {identifier}});

    if (keys) {
        return {publicKey: keys.publicKey};
    }

    const {publicKey, privateKey} = crypto.generateKeyPairSync('rsa', {
        modulusLength: 4096,
        publicKeyEncoding: {type: 'spki', format: 'pem'},
        privateKeyEncoding: {type: 'pkcs8', format: 'pem'},
    });


    await AsymetricKey.create({identifier, publicKey, privateKey, algorithm: "RSA-4096"});

    return {publicKey};
};

// Récupère la clé publique RSA-4096 associée à un identifiant
const getRSA4096PublicKey = async (identifier) => {
    const key = await AsymetricKey.findOne({where: {identifier}});
    return key ? key.publicKey : null;
};

// Récupère la clé privée RSA-4096 associée à un identifiant
const getRSA4096PrivateKey = async (identifier) => {
    const key = await AsymetricKey.findOne({where: {identifier}});
    return key ? key.privateKey : null;
};

// Génère une paire de clés RSA-2048
const generateRSA2048KeyPair = async (identifier) => {
    const keys = await AsymetricKey.findOne({where: {identifier}});

    if (keys) {
        return {publicKey: keys.publicKey};
    }

    const {publicKey, privateKey} = crypto.generateKeyPairSync('rsa', {
        modulusLength: 2048,
        publicKeyEncoding: {type: 'spki', format: 'pem'},
        privateKeyEncoding: {type: 'pkcs8', format: 'pem'},
    });

    await AsymetricKey.create({identifier, publicKey, privateKey, algorithm: "RSA-2048"});

    return {publicKey};
};

// Récupère la clé publique RSA-2048 associée à un identifiant
const getRSA2048PublicKey = async (identifier) => {
    const key = await AsymetricKey.findOne({where: {identifier}});
    return key ? key.publicKey : null;
};

// Récupère la clé privée RSA-2048 associée à un identifiant
const getRSA2048PrivateKey = async (identifier) => {
    const key = await AsymetricKey.findOne({where: {identifier}});
    return key ? key.privateKey : null;
};


/** Other functions **/
// Trouve si l'identifiant est dans la table symetricKeys ou asymetricKeys
const findKey = async (identifier) => {
    const symetricKeys = await SymetricKey.findOne({where: {identifier}});
    const asymetricKeys = await AsymetricKey.findOne({where: {identifier}});

    return symetricKeys || asymetricKeys || null;
}

// Supprime une clé de la table symetricKeys ou asymetricKeys
const deleteKey = async (identifier) => {
    const key = await findKey(identifier);
    if (key && key.destroy) {
        return key.destroy();
    }
    return null;
}

// Renvoie la clé de déchiffrement associée à un identifiant
const getDecryptKey = async (identifier) => {
    const key = await findKey(identifier);
    // Si la clé est symétrique, on la renvoie
    if (key && key.key) {
        if(key.algorithm === "AES-256") {
            return {decryptKey: key.key, iv: key.infos, algorithm: key.algorithm};
        }
        if(key.algorithm === "CHACHA20") {
            return {decryptKey: key.key, nonce: key.infos, algorithm: key.algorithm};
        }
        return {decryptKey: key.key, infos: key.infos, algorithm: key.algorithm};
    }
    // Si la clé est asymétrique, on renvoie la clé privée
    if (key && key.privateKey) {
        return {decryptKey: key.privateKey, algorithm: key.algorithm};
    }
    return null;
}

//Export all module to generate
module.exports = {
    generateAESKey,
    generateCHACHA20Key,
    generateRSA4096KeyPair,
    generateRSA2048KeyPair,
    getDecryptKey,
    findKey,
    deleteKey
};