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
        allowNull: true,
    },
    privateKey: {
        type: DataTypes.TEXT,
        allowNull: true,
    }
}, {
    tableName: 'asymetricKeys',
        timestamps: false,
});


module.exports = AsymetricKeys;