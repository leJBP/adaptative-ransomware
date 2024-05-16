const express = require('express');
const sequelize = require('./config/db');
const keysRouter = require('./routes/keysRoutes');

const app = express();
app.use(express.json());

app.use('/api/keys', keysRouter);

sequelize.sync()
    .then(() => {
        const PORT = process.env.PORT || 5000;
        app.listen(PORT, () => console.log(`Serveur lancé sur le port ${PORT}`));
    })
    .catch((error) => {
        console.error('Erreur de synchronisation avec la base de données', error);
        process.exit(1);
    });