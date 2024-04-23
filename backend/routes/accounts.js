var express = require('express');
var router = express.Router();
const db = require("../config/db");
const accountController = require("../controllers/accountController");
//const jwtAuth = require("../config/jwtAuth");//
//const account = require('../models/accountmodel');

router.get("/ownedaccounts",accountController.getOwnedAccounts);

router.get("/:id",accountController.getById);

router.get("/:id/users", accountController.getConnectedUsers);

router.post("/addusertoaccount", accountController.addUserToAccount);

router.post("/addaccount",accountController.addAccount);

router.put("/update", accountController.updateBalance);

router.delete("/user",  accountController.disconnectUser);

router.delete("/deleteacc",  accountController.deleteAccount);

module.exports = router;