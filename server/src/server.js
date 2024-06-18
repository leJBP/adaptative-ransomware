const express = require('express');
const sequelize = require('./config/db');
const keysRouter = require('./routes/keysRoutes');
const swaggerJSDoc = require('swagger-jsdoc');
const swaggerUi = require('swagger-ui-express');

const app = express();

// Configuration Swagger
const swaggerOptions = {
    definition: {
        openapi: '3.0.0',
        info: {
            title: 'Documentation projet Cryptographie',
            version: '1.0.0',
            description: 'Documentation du projet cryptographie du semestre d\'été'
        }
    },
    apis: ['./src/routes/*.js']
};

const swaggerSpec = swaggerJSDoc(swaggerOptions);

// Route pour servir la documentation Swagger
app.use('/api-docs', swaggerUi.serve, swaggerUi.setup(swaggerSpec));

app.use(express.json());
app.use('/api/keys', keysRouter);

const PORT = process.env.PORT || 5000;



// Fonction pour démarrer le serveur
function startServer() {
    app.listen(PORT, () => console.log(`Serveur lancé sur le port ${PORT}`));
}

// Fonction pour essayer de se connecter à la base de données
function connectWithRetry() {
    sequelize.sync().then(() => {
        console.log('Connexion à la base de données réussie!');
        startServer();
    }).catch((error) => {
        console.error('Erreur de connexion à la base de données:', error);
        console.log('Nouvelle tentative de connexion dans 5 secondes...');
        setTimeout(connectWithRetry, 5000); // Réessayer après 5 secondes
    });
}

// Premier appel de la fonction
connectWithRetry();
