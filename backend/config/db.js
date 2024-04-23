var mysql = require("mysql2");
require("dotenv").config();

const conn = mysql.createPool({
  host: process.env.DB_HOST,
  user: process.env.DB_USER,
  password: process.env.DB_PASS,
  database: process.env.DB_DB,
  multipleStatements: "true"
})

module.exports = conn;