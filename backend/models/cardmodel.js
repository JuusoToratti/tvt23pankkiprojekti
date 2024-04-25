const db = require("../config/db.js");
const bcrypt = require("bcrypt");

const saltRounds = 10;
const card = {
    get: function(callback){
        return db.query("SELECT * FROM card", callback);
    },
    getcardnumberpin: function(callback){
        return db.query("select card_number, pin FROM card where idcard = 4;", callback);
    },

    addCard: function(newcard,callback){
        console.log("model");
         bcrypt.hash(newcard.pin, saltRounds, function(err, hash){
            return db.query("INSERT INTO card (pin, card_number, active,idcard,idaccount,user_iduser,cardtype ) VALUES(?,?,?,?,?,?,?)",
            [hash,newcard.card_number,newcard.active,newcard.idcard,newcard.idaccount,newcard.user_iduser,newcard.cardtype,], callback);
        });
    },
    addCardToAccount: function(user_idcard, idaccount, callback){
        return db.query("INSERT INTO (user_iduser, idaccount) VALUES(?, ?)", [iduser, idaccount], callback);
    },
    getByNumber: function(card_number, callback){
        return db.query("SELECT * FROM card WHERE card_number = ?", [card_number], callback);
    },
    getByiduser: function(iduser, callback){
        return db.query("SELECT * FROM card WHERE iduser = ?", [iduser], callback);
    },
    getCardAccountInfo: function(iduser, callback){
        return db.query("SELECT card.card_number, card.card_type, account.account_ID, account.name, account.balance FROM card JOIN account ON account.account_ID = card.account_ID WHERE card.iduser = ?",
        [iduser], callback);
    },
    getCardAccountInfoByNumber: function(card_number, callback){
        return db.query("SELECT card.card_number, card.card_type, account.account_ID, account.name, account.balance FROM card JOIN account on account.account_ID = card.account_ID WHERE card_number = ?",
        [card_number], callback);
    },
    getTries: function(card_number, callback){
        return db.query("SELECT tries FROM card WHERE card_number = ?", [card_number], callback);
    },
    updateActiveStatus: function(active, card_number, callback){
        return db.query("UPDATE card SET active = ? WHERE card_number = ?", [active, card_number], callback);
    },
    connectCard: function(accountId, userId, card_type, pin, callback){
        bcrypt.hash(pin, saltRounds, function(err, hash){
            return db.query("UPDATE card SET account_ID = ?, user_ID = ?, card_type = ?, pin = ?, active = 1 WHERE user_ID IS NULL AND account_ID IS NULL ORDER BY card_ID ASC LIMIT 1",
            [accountId, userId, card_type, hash], callback);
        });
    },
    disconnectCard: function(card_number, callback){
        return db.query("UPDATE card SET account_ID = NULL, iduser = NULL, card_type = 0, active = 0 WHERE card_number = ?",
        [card_number], callback);
    },
    disconnectByAccountId: function(accountId, callback){
        return db.query("UPDATE card SET account_ID = NULL, iduser = NULL, card_type = 0, active = 0 WHERE account_ID = ?",
        [accountId], callback);
    },

    delete: function(idcard, callback){
        return db.query("DELETE FROM card WHERE idcard = ?", [idcard], callback);
    },
    updateTries: function(tries, card_number, callback){
        return db.query("UPDATE card SET tries = ? WHERE card_number = ?", [tries, card_number], callback);
    }

}

module.exports = card;