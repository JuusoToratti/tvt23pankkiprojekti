const db = require("../config/db.js");
const bcrypt = require("bcrypt");

const saltRounds = 10;
const user = {
    get: function(callback){
        return db.query("SELECT * FROM user", callback);
    },
    
    getById: function(id, callback){
        return db.query("SELECT * FROM user WHERE iduser = ?", [id], callback);
    },
    getByEmail: function(email, callback){
        return db.query("SELECT * FROM user WHERE email = ?", [email], callback);
    },
    getCredentials: function(email, password, callback){
        return db.query("SELECT email, pword FROM user WHERE email = ? AND pword = ?",
        [email, password], callback);
    },
    add: function(newuser, callback){
        console.log("model");
        bcrypt.hash(newuser.pword, saltRounds, function(err, hash){
            return db.query("INSERT INTO user(email, pword, fname, lname, adress, phonenumber, picture, username,iduser) VALUES(?,?,?,?,?,?,?,?,?)",
            [newuser.email, hash, newuser.fname, newuser.lname, newuser.adress, newuser.phonenumber,newuser.picture,newuser.username, newuser.iduser], callback);
        });
    },
    deleteuser: function(iduser, callback){
        return db.query("DELETE FROM user WHERE iduser = ?", [iduser], callback);
    },

updateAvatar: function(iduser, file, callback){
    return db.query("UPDATE user SET picture = ? WHERE iduser = ?",
    [file, iduser], callback);
}

}




module.exports = user;