const { DataTypes } = require('sequelize');
const sequelize = require('../config/db');

const AsymetricKeys = sequelize.define('Keys', {
    identifier: {
        type: DataTypes.STRING,
        allowNull: false,
        unique: true,
        primaryKey: true
    },
    publicKey: {
        type: DataTypes.TEXT,
        allowNull: false,
    },
    privateKey: {
        type: DataTypes.TEXT,
        allowNull: false,
    },
    algorithm: {
        type: DataTypes.STRING,
        allowNull: false,
    }
}, {
    tableName: 'asymetricKeys',
        timestamps: false,
});


module.exports = AsymetricKeys;