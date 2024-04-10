const emailvalidator = require("email-validator");
const sanitizer = require("sanitizer");
const { json } = require("express/lib/response");
const account = require("../models/accountmodel");
const user = require("../models/usermodel");
const card = require("../models/cardmodel");

const getAll = (req, res) => {
    account.get(function(err, dbResult){
        if(err){
            return res.json(err);
        }else{
            return res.json(dbResult);
        }
    });
}

const getById = (req, res) => {
    account.getById(req.params.id,function(err, dbResult){
        if(err){
            return res.json(err);
        }else{
            return res.json(dbResult);
        }
    });
}

const getOwnedAccounts = (req, res) => {
    account.getByiduser(req,function(err, dbResult){
        if(err){
            return res.json(err);
        }else{
            return res.json(dbResult);
        }
    });
}

const addAccount = (req, res) => {
    // if(req.iduser && req.body.name)
    if(1==1)
    {
        account.add(req.body,function(err, dbResult){
            if(err){
                    //return res.json(err);
                    res.send(err);        
            }else{
                return res.json({status:"success",message:"New account added succesfully!"});
            }
        });
    }else{
        return res.json({status:"error",message:"Please fill all fields."});
    }
}

const addUserToAccount = (req, res) => {
    if((req.body.iduser) && req.body.idaccount)
   // if (1==1)
      {  
        account.addUserToAccount(req.body.iduser, req.body.idaccount, function(err, dbResult){
if(err){
    res.send(err);
       
        }else{
            return res.json({status:"success",message:"New account and user added succesfully!"});
        }  
    });
}else{
    return res.json({status:"error",message:"Please fill all fields."});
    }
}
const getConnectedUsers = (req, res) => {
    if(req.params.id){
        account.getOwnerById(req.iduser, req.params.id, function(err, dbResult){
            if(err){
                return res.json({status:"error",message:err});
            }
            let hasAccessToAccount = false;
            let accountId = null;
            for(let i=0;i<dbResult.length;i++){
                if(dbResult[i].account_ID === Number(req.params.id) && dbResult[i].owner === req.iduser){
                    hasAccessToAccount = true;
                    accountId = dbResult[i].account_ID;
                }
            }
            if(!hasAccessToAccount){
                return res.json({status:"error",message:"You are not the owner of this account."});
            }
            account.getConnectedUsers(accountId, req.iduser, function(err, dbResult){
                if(err){
                    return res.json({status:"error",message:err});
                }
                if(dbResult.length > 0){
                    return res.json({status:"success",message:dbResult});
                }else{
                    return res.json({status:"error",message:"No users found."});
                }
            });
        });
    }else{
        return res.json({status:"error",message:"Please fill all fields."});
    }
}

const updateBalance = (req, res) => {
    if(req.body.card_number && req.body.amount && req.body.action && req.body.type){
        card.getByUserID(req.iduser, (err, dbResult) =>{

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

            account.getByCardNumber(req.body.card_number, function(err, dbResult){
                if(err){
                    return res.json({status:"error",message:err});
                }
                if(dbResult.length > 0){
                    let curbalance = dbResult[0].balance;
                    if(req.body.action === "0" && dbResult[0].card_type === 0 || req.body.action === "0" && dbResult[0].card_type === 1 && req.body.type === "0"){
                        if(dbResult[0].balance >= req.body.amount){
                            account.updateBalance(-Math.abs(req.body.amount), req.body.card_number, function(err, dbResult){
                                curbalance += -Math.abs(req.body.amount);
                                return res.json({status:"success",balance:curbalance});
                            });
                        }else{
                            return res.json({status:"error",message:"Not enough balance."});
                        }
                    }else if(req.body.action === "0" && dbResult[0].card_type === 1 || req.body.action === "0" && dbResult[0].card_type === 1 && req.body.type === "1"){
                        account.updateBalance(-Math.abs(req.body.amount), req.body.card_number, function(err, dbResult){
                            if(err){
                                return res.json({status:"error",message:err});
                            }
                            if(dbResult.affectedRows > 0){
                                curbalance += -Math.abs(req.body.amount);
                                return res.json({status:"success",balance:curbalance});
                            }
                        });
                    }else if(req.body.action === "1"){
                        account.updateBalance(Math.abs(req.body.amount), req.body.card_number, function(err, dbResult){
                            if(err){
                                return res.json({status:"error",message:err});
                            }
                            if(dbResult.affectedRows > 0){
                                curbalance += Math.abs(req.body.amount);
                                return res.json({status:"success",balance:curbalance});
                            }
                        });
                    }else{
                        return res.json({status:"error",message:"Invalid value for 'action'."});
                    }
                }
            });
        });
    }else{
        return res.json({status:"error",message:"Please fill all fields."});
    }
}

const disconnectUser = (req, res) => {
    if(req.body.account && req.body.user){
        account.getOwnerById(req.iduser, req.body.account, function(err, dbResult){
            if(err){
                return res.json({status:"error",message:err});
            }
            let hasAccessToAccount = false;
            for(let i=0;i<dbResult.length;i++){
                if(dbResult[i].account_ID.toString() === req.body.account.toString() && dbResult[i].owner.toString() === req.iduser){
                    hasAccessToAccount = true;
                }
            }
            if(!hasAccessToAccount){
                return res.json({status:"error",message:"You are not the owner of this account."});
            }
            account.deleteUserFromAccount(req.body.account, req.body.user, function(err, dbResult){
                if(err){
                    return res.json({status:"error",message:err});
                }
                if(dbResult.affectedRows > 0){
                    return res.json({status:"success",message:"Successfully removed user from account."});
                }else{
                    return res.json({status:"error",message:"An unknown error has occurred."});
                }
            });
        });
    }else{
        return res.json({status:"error",message:"Please fill all fields."});
    }
}

const deleteAccount = (req, res) => {
    if(req.body.id){
        account.getOwnerById(req.iduser, req.body.id, function(err, dbResult){
            if(err){
                return res.json({status:"error",message:err});
            }
            let hasAccessToAccount = false;
            let accountId = null;
            for(let i=0;i<dbResult.length;i++){
                if(dbResult[i].account_ID === req.body.id && dbResult[i].owner === req.iduser){
                    hasAccessToAccount = true;
                    accountId = dbResult[i].account_ID;
                    if(dbResult[i].balance !== 0){
                        return res.json({status:"error",message:"Account's balance must be 0 before deleting."});
                    }
                }
            }
            if(!hasAccessToAccount){
                return res.json({status:"error",message:"You are not the owner of this account."});
            }
            card.disconnectByAccountId(accountId, function(err, dbResult){
                if(err){
                    return res.json({status:"error",message:err});
                }
            });
            account.delete(accountId, function(err, dbResult){
                if(err){
                    return res.json({status:"error",message:err});
                }
                if(dbResult.affectedRows > 0){
                    return res.json({status:"success",message:"Successfully deleted account "+req.body.id});
                }
            });
        });
    }else{
        return res.json({status:"error",message:"Please fill all fields."});
    }
}

module.exports = {
    getAll,
    getById,
    getOwnedAccounts,
    getConnectedUsers,
    addAccount,
    addUserToAccount,
    updateBalance,
    disconnectUser,
    deleteAccount,
}