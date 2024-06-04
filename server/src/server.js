const express = require('express');
const sequelize = require('./config/db');
const keysRouter = require('./routes/keysRoutes');

const app = express();
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
