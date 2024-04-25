const db = require("../config/db.js");

const login={
  checkpin: function(card_number, callback) {
      return db.query('SELECT pin FROM card WHERE card_number = ?',[card_number], callback); 
    }
};
          
module.exports = login;