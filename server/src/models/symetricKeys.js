const { DataTypes } = require('sequelize');
const sequelize = require('../config/db');

const SymetricKeys = sequelize.define('Keys', {
    identifier: {
        type: DataTypes.STRING,
        allowNull: false,
        unique: true,
        primaryKey: true
    },
    key: {
        type: DataTypes.TEXT,
        allowNull: true,
    },
    algorithm: {
        type: DataTypes.STRING,
        allowNull: false,
    }
}, {
    tableName: 'symetricKeys',
    timestamps: false,
});


module.exports = SymetricKeys;