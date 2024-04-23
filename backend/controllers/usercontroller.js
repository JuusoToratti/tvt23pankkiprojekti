const user = require("../models/usermodel");
const bcrypt = require("bcrypt");
const emailvalidator = require("email-validator");
//const jwt = require("../config/jwtAuth");// pois väliaikaisesti
const fs = require("fs");
const crypto = require('crypto');
const multer = require('multer');
require("dotenv").config();


const getAll = (req, res) => {
    user.get(function(err,dbResult){
        if(err){
            res.json(err);
        }else{
            res.json(dbResult);
        }
    });
}

const getById = (req, res) => {
    if(req.params.id){
        user.getById(req.params.id, function(err,dbResult){
            if(err){
                res.json(err);
            }else{
                res.json(dbResult);
            }
        });
    }
}

const userLogin = (req, res) => {
    if(emailvalidator.validate(req.body.email) && req.body.pword){
        user.getByEmail(req.body.email, function(err, dbResult){
            if(err){
                console.log(err);
                res.json(err);
            }else{
                if(dbResult.length > 0){
                    bcrypt.compare(req.body.pword, dbResult[0].pword, (err, match)=>{
                        if(err){
                            res.json(err);
                        }
                        if(match){
                            const token = jwt.generateToken(dbResult[0].iduser);
                            console.log("Created token:",token);
                            res.status(200).json({status:"success",message:"Successfully logged in.",token:token});
                        }else{
                            console.log("Invalid email or pword!");
                            res.json({status:"error",message:"Invalid email or pword."});
                        }
                    });
                }else{ 
                    console.log("No user found with this email");
                    res.json({status:"error",message:"No user found with this email."});
                }
            }
        });
    }else{
        res.json({status:"error",message:"Please enter email and pword."});
    }
}

const userRegister = (req, res) => {
    //console.log(req.body);
    //if(emailvalidator.validate(req.body.email) && req.body.pword && req.body.adress && req.body.fname && req.body.lname && req.body.phone && req.body.picture && req.body.username)
    if(1==1)
    {
        user.add(req.body, function(err, dbResult){
            if(err){
                //if(err.errno === 1062){
                  //  res.json({status:"error",message:"Email already exists."});
               // }
               res.send(err);
            }else{
                console.log("Successfully registered.");
                res.json({status:"success",message:"Successfully registered."});
            }
        });
    }else{
        res.json({message:"Please fill all fields."});
    }
}

const userInfo = (req, res) => {
    user.get(function(err, dbResult){
        if(err){
            console.log(err);
            return res.json(err);
        }
        for(let i = 0;i<dbResult.length;i++){
            if(dbResult[i].iduser === req.iduser){
                delete dbResult[i]["pword"];
                return res.json(dbResult[i]);
            }
        }
        return res.json({status:"error",message:"No data available"});
    });
}

const storage = multer.diskStorage({

    // file destination path defaults to 'uploads/' if not defined
    destination: process.env.DOWNLOAD_PATH || 'uploads/',
    filename: function (req, file, cb) {

        // valid mimes that match valid extensions
        let mimes = {'image/png':'png', 'image/jpeg':'jpg'};

        // get the file extension
        let fileExt = mimes[file.mimetype];

        // create filename from randombytes and add file extension to it
        let finalName = crypto.randomBytes(16).toString('hex') +'.' + fileExt;

        cb(null, finalName)
    }
})

const upload = multer(
    {
        storage: storage,
        limits: {
            // 2MB file size limit
            fileSize: 1024 * 1024 * 2
        },
        fileFilter(req, file, cb) {
            
            // valid mimes that match valid extensions
            let mimes = {'image/png':'png', 'image/jpeg':'jpg'};

            // get the file extension
            let fileExt = mimes[file.mimetype];

            if(!fileExt){
                // return error if mimetype is not in ext list
                return cb("Filetype not allowed: " + file.mimetype, false);
            }

            // else return undefined error
            cb(undefined, true);
        }
    }
).single('avatar');

const updateAvatar = (req, res) => {

    upload(req,res, (err) => {

        
        if(!req.file){
            return res.json({status:"error", message:"File not found"});
        }
        
        
        if(err){
            return res.json({status:"error", message:err});
        }

        
        user.getById(req.iduser, function(err, dbResult){

            if(err) {
                console.log(err);
                return res.json({status:"error", message:err});
            }

            if(dbResult.length > 0){
                let oldPicture = dbResult[0].picture;

                if(oldPicture != "default.png"){
                    console.log(`Deleting ${oldPicture}`);
                    let path = "./uploads/"+oldPicture;
                    fs.unlink(path, (err) => {
                        if(err != null){
                            console.log("Error on deleting old picture");
                            console.log(err);
                        }
                    });
                }
            }
        })

        let filename = req.file.filename;

        user.updateAvatar(req.iduser, filename, function(err, dbResult) {

            if(err) {
                console.log(err);
                return res.json({status:"error", message:err});
            }

            if(dbResult.affectedRows > 0){
                res.json({status:"success", message:"Profile picture updated."});
            }else{
                res.json({status:"error", message:"Profile picture was not updated."});
            }
        })
    })
}

module.exports = {
    getAll,
    userLogin,
    userRegister,
    getById,
    userInfo,
    updateAvatar,
}
