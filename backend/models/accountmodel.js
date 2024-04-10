const db = require("../config/db.js");
const bcrypt = require("bcrypt");



const account = {
    get: function(callback){
        return db.query("SELECT * FROM account", callback);
    },
    getById: function(id, callback){
        return db.query("SELECT * FROM account WHERE idaccount = ?", [id], callback);
    },
    getByiduser: function(req, callback){
        return db.query("SELECT account.* FROM account JOIN user_account ON user_account.idaccount = account.idaccount JOIN user ON user.iduser = user_account.iduser WHERE user.iduser = ?",
        [req.iduser], callback);

    },
    getByCardNumber: function(card_number, callback){
        return db.query("SELECT account.*, card.card_type FROM account JOIN card ON card.idaccount = account.idaccount WHERE card_number = ?",
        [card_number], callback);
    },
    getExact: function(iduser, idaccount, callback){
        return db.query("SELECT * FROM user_account WHERE iduser = ? AND idaccount = ?",
        [iduser, idaccount], callback);
    },
    getConnectedUsers: function(idaccount, iduser, callback){
        return db.query("SELECT account.idaccount, user.email, user_account.iduser FROM account JOIN user_account ON user_account.idaccount = account.idaccount JOIN user ON user.iduser = user_account.iduser WHERE account.idaccount = ? AND user.iduser != ?",
        [idaccount, iduser], callback);
    },
    getCardsById: function(idaccount, callback){
        return db.query("SELECT * FROM account JOIN card on card.idaccount = account.idaccount WHERE account.idaccount = ?",
        [idaccount], callback);
    },
    getOwnerById: function(iduser, idaccount, callback){
        return db.query("SELECT account.idaccount, account.account_balance, account.owner FROM account JOIN user_account ON user_account.idaccount = account.idaccount JOIN user ON user.iduser = user_account.iduser WHERE user.iduser = ? AND account.idaccount = ?",
        [iduser, idaccount], callback);
    },
    add: function(newaccount, callback){
        console.log("model");
        return db.query("INSERT INTO account(name, account_strated, owner, credit_limit, account_balance) VALUES(?, ?, ?, ?, ?)",
        [newaccount.name, newaccount.account_strated,newaccount.owner,newaccount.credit_limit,newaccount.account_balance], callback);
    },
    addUserToAccount: function(iduser, idaccount, callback){
        return db.query("INSERT INTO user_account(user_iduser, account_idaccount) VALUES(?, ?)", [iduser, idaccount], callback);
    },
    updateBalance: function(amount, card_number, callback){
        return db.query("UPDATE account JOIN card ON card.account_ID = account.account_ID SET balance = balance + ? WHERE card.card_number = ?",
        [amount, card_number], callback);
    },
    deleteUserFromAccount: function(idaccount, iduser, callback){
        return db.query("DELETE FROM user_account WHERE idaccount = ? AND iduser = ?", [idaccount, iduser], callback);
    },
    delete: function(idaccount, callback){
        return db.query("DELETE FROM account WHERE idaccount = ?", [idaccount], callback);
    }
}

module.exports = account;