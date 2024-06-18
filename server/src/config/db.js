/**
 * This module exports a Sequelize instance configured to connect to a PostgreSQL database.
 * The database credentials are read from environment variables.
 *
 * @module db
 * @requires sequelize
 * @requires dotenv
 */

const { Sequelize } = require('sequelize'); // Importing Sequelize module
const dotenv = require('dotenv'); // Importing dotenv module
const result = dotenv.config(); // Loading environment variables from a .env file into process.env

// Retrieving the PostgreSQL password from environment variables
const postgresPassword = process.env.POSTGRES_PASSWORD;

// Setting the database's name to environment variable
const databaseName = process.env.POSTGRES_DB_NAME;

// Checking if the POSTGRES_PASSWORD environment variable is defined
if (postgresPassword === undefined) {
    throw new Error('POSTGRES_PASSWORD is not defined');
}

// Checking if the POSTGRES_PASSWORD environment variable is a string
if (typeof postgresPassword !== 'string') {
    throw new Error('POSTGRES_PASSWORD must be a string');
}

let host;
// Checking if the application is running in a production environment
if (process.env.NODE_ENV === 'production') {
    host = 'postgres'; // In production, the PostgreSQL server is assumed to be running on a host named 'postgres'
} else {
    host = 'localhost'; // In non-production environments, the PostgreSQL server is assumed to be running on localhost
}

/**
 * Sequelize instance configured to connect to a PostgreSQL database.
 * The database name is 'Seo_bdd', the username is 'postgres', and the password is read from the POSTGRES_PASSWORD environment variable.
 * The host is determined based on the NODE_ENV environment variable.
 *
 * @type {Sequelize}
 */
const sequelize = new Sequelize(databaseName, 'postgres', postgresPassword, {
    host: host,
    dialect: 'postgres',
    logging: false,
});

module.exports = sequelize; // Exporting the Sequelize instance