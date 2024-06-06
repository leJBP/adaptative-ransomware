const express = require('express');
const router = express.Router();
const keyService = require('../services/keyService');
const benchmarkService = require('../services/benchmarkService');


/**
 * @openapi
 * /api/keys/generate:
 *   post:
 *     summary: Générer une clé de chiffrement
 *     description: Génère une clé de chiffrement en fonction d'un benchmark
 *     tags:
 *       - Keys
 *     requestBody:
 *       required: true
 *       content:
 *         application/json:
 *           schema:
 *             type: object
 *             properties:
 *               identifier:
 *                 type: string
 *                 description: Identifiant unique pour la clé
 *               benchmark:
 *                 type: object
 *                 properties:
 *                   cpuMaxFreq:
 *                     type: number
 *                     description: Fréquence maximale du CPU en MHz
 *                   cpuMinFreq:
 *                     type: number
 *                     description: Fréquence minimale du CPU en MHz
 *                   cpuCores:
 *                     type: number
 *                     description: Nombre de coeurs du CPU
 *                   memorySize:
 *                     type: number
 *                     description: Taille de la mémoire en octets
 *                   dataSize:
 *                     type: number
 *                     description: Taille des données en octets
 *     responses:
 *       200:
 *         description: Clé de chiffrement générée
 *         content:
 *           application/json:
 *             schema:
 *               type: object
 *               properties:
 *                 encryptKey:
 *                   type: string
 *                   description: Clé de chiffrement
 *                 algorithm:
 *                   type: string
 *                   description: Algorithme de chiffrement
 *       400:
 *         description: Requête invalide ou autres erreurs
 */

router.post('/generate', benchmarkService.validateBenchmark, async (req, res) => {
    const identifier = req.body.identifier;
    const benchmark = req.body.benchmark;
    if (!identifier) return res.status(400).send('Identifier is required');
    if (!benchmark) return res.status(400).send('Benchmark is required');
    try {
        if (await keyService.findKey(identifier)) return res.status(400).send('Key already exists');
        const key = await benchmarkService.takeDecision(identifier, benchmark);
        res.send(key);
    } catch (err) {
        res.status(400).send(err.message);
    }
});

/**
 * @openapi
 * /api/keys/decrypt:
 *   post:
 *     summary: Récupérer la clé de déchiffrement associée à un identifiant
 *     tags:
 *       - Keys
 *     requestBody:
 *       required: true
 *       content:
 *         application/json:
 *           schema:
 *             type: object
 *             properties:
 *               identifier:
 *                 type: string
 *                 description: Identifiant unique pour la clé
 *     responses:
 *       200:
 *         description: Clé de déchiffrement récupérée
 *         content:
 *           application/json:
 *             schema:
 *               type: object
 *               properties:
 *                 decryptKey:
 *                   type: string
 *                   description: Clé de déchiffrement
 *                 algorithm:
 *                   type: string
 *                   description: Algorithme de chiffrement
 *       400:
 *         description: Clé non trouvée
 */
 router.post('/decrypt', async (req, res) => {
    const {identifier} = req.body;
    const {decryptKey, algorithm} = await keyService.getDecryptKey(identifier);
    res.send({decryptKey, algorithm});
});

/**
 * @openapi
 * /api/keys/:
 *   delete:
 *     summary: Supprime une clé basée sur l'identifiant fourni.
 *     tags:
 *       - Keys
 *     requestBody:
 *       required: true
 *       content:
 *         application/json:
 *           schema:
 *             type: object
 *             properties:
 *               identifier:
 *                 type: string
 *                 description: L'identifiant de la clé à supprimer.
 *     responses:
 *       200:
 *         description: Clé supprimée.
 *       400:
 *         description: Clé non trouvée.
 */
router.delete('/', async (req, res) => {
    const {identifier} = req.body;
    try {
        const key = await keyService.deleteKey(identifier);
        if (key == null) {
            res.status(400).send('Key not found');
            return;
        }
        res.send('Key deleted');
    } catch (err) {
        res.status(400).send(err.message);
    }
});

module.exports = router;