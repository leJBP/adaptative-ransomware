const { DataTypes } = require('sequelize');
const sequelize = require('../config/db');

const Keys = sequelize.define('Keys', {
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
    }
}, {
    tableName: 'keys',
        timestamps: false,
});


module.exports = Keys;