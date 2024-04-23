const card = require("../models/cardmodel");
const account = require("../models/accountmodel");
const bcrypt = require("bcrypt");
//const { json } = require("express/lib/response");
//const jwt = require("../config/jwtAuth");//pois päältä väliaikaisesti

const getAll = (req, res) => {
    card.get(function(err,dbResult){
        if(err){
            return res.json({status:"error",message:err});
        }else{
            res.json({status:"success",message:dbResult});
        }
    });
}

const getByCardNumber = (req, res) => {

    if(req.params.card_number){
        card.getByiduser(req.iduser, (err, dbResult) =>{

            if(err){
                return res.json({status:"error",message:err});
            }

            let hasAccessToCard = false;
            for(let i = 0; i < dbResult.length; i++){
                if(dbResult[i].card_number.toString() === req.params.card_number){
                    hasAccessToCard = true;
                }
            }

            if(!hasAccessToCard){
                return res.json({status:"error",message:"User doesn't have this card"});
            }

            card.getByNumber(req.params.card_number, function(err,dbResult){
                if(err){
                    return res.json({status:"error",message:err});
                }
    
                if(dbResult.length > 0){
                    return res.json(dbResult);
                }else{
                    return res.json({status:"error",message:"Card not found"})
                }
            });
        });
    }else{
        return res.json({status:"error",message:"Please fill all fields"});
    }
}

const getByiduser = (req, res) => {
    card.getByiduser(req.iduser, function(err,dbResult){

        if(err){
            return res.json({status:"error",message:err})
        }

        if(dbResult.length > 0){
            res.json(dbResult);
        }else{
            res.json({status:"error",message:"No cards found for this user"})
        }

    });
}

const getCardAccountInfo = (req, res) => {
    card.getCardAccountInfo(req.iduser, function(err, dbResult){
        if(err){
            return res.json({status:"error",message:err});
        }
        if(dbResult.length > 0){
            return res.json(dbResult);
        }else{
            return res.json({status:"error",message:"No cards found for this user"});
        }
    });
}

const getCardAccountInfoByNumber = (req, res) => {
    if(req.params.card_number){
        card.getByiduser(req.iduser, (err, dbResult) =>{

            if(err){
                return res.json({status:"error",message:err});
            }

            let hasAccessToCard = false;
        
            for(let i = 0; i < dbResult.length; i++){
                if(dbResult[i].card_number === req.params.card_number){
                    hasAccessToCard = true;
                }
            }

            if(!hasAccessToCard){
                return res.json({status:"error",message:"User doesn't have this card"});
            }

            card.getCardAccountInfoByNumber(req.params.card_number, function(err, dbResult){
                if(err){
                    return res.json({status:"error",message:err});
                }
                if(dbResult.length > 0){
                    return res.json(dbResult[0]);
                }else{
                    return res.json({status:"error",message:"No cards found for this user"});
                }
            });
        });
    }
}

const unlock = (req, res) => {
    if(req.body.card_number){

        card.getByiduser(req.iduser, (err, dbResult) =>{

            if(err){
                return res.json({status:"error",message:err});
            }

            let hasAccessToCard = false;
        
            for(let i = 0; i < dbResult.length; i++){
                if(dbResult[i].card_number === req.body.card_number){
                    hasAccessToCard = true;
                }
            }

            if(!hasAccessToCard){
                return res.json({status:"error",message:"User doesn't have this card"});
            }

    


            card.updateTries(0, req.body.card_number, (err, dbResult) =>{

                if(err){
                    return res.json({status:"error",message:"Error on resetting card's tries"})
                }

                if(dbResult.affectedRows > 0){
                    console.log("Unlocking card, resetting tries");
                }else{
                    console.log("Error on resetting tries");
                }
            });

            card.updateActiveStatus(1, req.body.card_number, function(err, dbResult){

                if(err){
                    return res.json({status:"error",message:err})
                }
                
                if(dbResult.affectedRows > 0){
                    res.json({status:"success",message:"Successfully modified card's status"});
                }else{
                    res.json({status:"error",message:"Card not found"})
                }
                
            });
        })
        
    }else{
        res.json({status:"error",message:"Please fill all fields"});
    }
}



const addCard = (req, res) => {
    if(1==1)
    card.add(function(err, dbResult){
        if(err){
            return res.json({status:"error",message:err});
        }else{
            res.json({status:"success",message:"Successfully added new card to database."});
        }
    });
}


const connectCard = (req, res) => {
    if(req.iduser && req.body.accountId && req.body.card_type && req.body.pin){
        const pinRegex = /\D/;
        const pinInput = req.body.pin.toString();
        let validatePin = false;
        if(!pinInput.match(pinRegex) && pinInput.length === 4){
            validatePin = true;
        }
        if(!validatePin){
            return res.json({status:"error",message:"Pin must be exactly 4 digits long."});
        }
        let ctype = null;
        if(req.body.card_type === "Debit"){
            ctype = 0;
        }else if(req.body.card_type === "Credit"){
            ctype = 1;
        }else{
            return res.json({status:"error",message:"Invalid card type."});
        }
        account.getByiduser(req, function(err, dbResult){
            if(err){
                return res.json({status:"error",message:err});
            }else{
                let hasAccessToAccount = false;
                for(let i=0;i<dbResult.length;i++){
                    if(dbResult[i].account_ID === req.body.accountId){
                        hasAccessToAccount = true;
                    }
                }
                if(!hasAccessToAccount){
                    return res.json({status:"error",message:"User does not have access to this account."});
                }
                card.connectCard(req.body.accountId, req.iduser, ctype, req.body.pin, function(err, dbResult){
                    if(err){
                        return res.json({status:"error",message:err});
                    }else if(dbResult.affectedRows === 0){
                        return res.json({status:"error",message:"No cards available."});
                    }
                    else{
                        return res.json({status:"success",message:"Successfully added card to selected account."});
                    }
                });
            }
        });
    }else{
        return res.json({status:"error",message:"Please fill all fields."});
    }
}

const disconnectCard = (req, res) => {
    if(req.body.card_number){
        card.getByiduser(req.iduser, function(err, dbResult){
            if(err){
                return res.json({status:"error",message:err});
            }
            let hasAccessToCard = false;
            for(let i=0;i<dbResult.length;i++){
                if(dbResult[i].card_number === req.body.card_number){
                    hasAccessToCard = true;
                }
            }
            if(!hasAccessToCard){
                return res.json({status:"error",message:"User does not have access to this card."});
            }
            card.disconnectCard(req.body.card_number, function(err, dbResult){
                if(err){
                    return res.json({status:"error",message:err});
                }
                if(dbResult.affectedRows > 0){
                    return res.json({status:"success",message:"Successfully deleted card."});
                }else{
                    return res.json({status:"error",message:"Unknown error has occurred."});
                }
            });
        });
    }else{
        return res.json({status:"error",message:"Please fill all fields."});
    }
}

const getTries = (req, res) => {
    if(req.params.card_number){
        card.getTries(req.params.card_number, function(err, dbResult){
            if(dbResult.length > 0){
                return res.json({tries:dbResult[0].tries});
            }else{
                return res.json({status:"error",message:"Card was not found."});
            }
        });
    }else{
        return res.json({status:"error",message:"Please fill all fields."});
    }
}

const authenticate = (req, res) => {

    let iduser = null;

    if(req.body.card_number && req.body.pin){
        card.getByNumber(req.body.card_number, function(err, dbResult){
            if(err){
                return res.json({status:"error",message:err})
            }

    
            if(dbResult.length > 0){

                iduser = dbResult[0].iduser;
                if(dbResult[0].active === 0){
                    return res.json({status:"error",message:"Card is locked!"});
                }
                
                bcrypt.compare(req.body.pin, dbResult[0].pin.toString(), (err, match)=>{
                    if(err){
                        return res.json({status:"error",message:err})
                    }

                    if(match){
                       // const token = jwt.generateToken(iduser);//pois päältä väliaikaisesti
                        console.log("Created token:",token);
                        card.updateTries(0, req.body.card_number, (err, dbResult) =>{
                            if(err){
                                console.log("Error on resetting card tries.");
                            }
        
                            if(dbResult.affectedRows > 0){
                                console.log("Unlocking card, resetting tries");
                            }else{
                                console.log("Error on resetting tries");
                            }
                        });
                        //return res.json({status:"success",message:"Successfully logged in.",token:token});//
                    }else{
                        console.log("Invalid pin code or card number!");

                        let tries = dbResult[0].tries + 1;
                        
                        card.updateTries(tries, req.body.card_number, (err, dbResult) =>{
                            if(err){
                                console.log(err);
                            }

                            if(dbResult.affectedRows > 0){
                                console.log("Incremented tries");
                            }else{
                                console.log("Error on incrimenting tries");
                            }
                        })

                      
                        if(tries === 3 && dbResult[0].active === 1){
                            card.updateActiveStatus(0, req.body.card_number, (err, dbResult) =>{
                                if(err){
                                    console.log(err);
                                }

                                if(dbResult.affectedRows > 0){
                                    console.log("Card is deactivated");
                                }else{
                                    console.log("Error on deactivating card");
                                }

                                return res.json({status:"error",message:"Card deactivated!"});
                            })
                        }else{
                            return res.json({status:"error",message:"Invalid pin code!", tries:tries});
                        }
                    }
                });

            }else{
                res.json({status:"error",message:"Card was not found"});
            }
            
        });
    }else{
        res.json({status:"error",message:"Please fill all fields"})
    }
    
}

const deleteCard = (req, res) => {
    if(req.body.card_number){

        
        card.getByiduser(req.iduser, (err, dbResult) =>{

            if(err){
                return res.json({status:"error",message:err});
            }

            let hasAccessToCard = false;
        
            for(let i = 0; i < dbResult.length; i++){
                if(dbResult[i].card_number === req.body.card_number){
                    hasAccessToCard = true;
                }
            }

            if(!hasAccessToCard){
                return res.json({status:"error",message:"User doesn't have this card"});
            }

            card.delete(req, function(err,result) {

                if(err){
                    return res.json({status:"error",message:err})
                }
    
                if(result.affectedRows > 0){
                    res.json({status:"success",message:"Card deleted"});
                }else{
                    res.json({status:"error",message:"Card not found"})
                }
            })
        })
        
    }else{
        res.json({status:"error",message:"Please fill all fields"})
    }
    
}

module.exports = {
    getAll,
    getByCardNumber,
    getByiduser,
    getCardAccountInfo,
    getCardAccountInfoByNumber,
    getTries,
    unlock,
    addCard,
    authenticate,
    deleteCard,
    connectCard,
    disconnectCard
}