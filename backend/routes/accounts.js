var express = require('express');
var router = express.Router();
const db = require("../config/db");
const accountController = require("../controllers/accountController");
//const jwtAuth = require("../config/jwtAuth");//

router.get('/accountall',accountController.getAll);

router.get("/getaccountbalance/:id",accountController.getaccountbalance);

router.get("/getcreditlimit/:id",accountController.getcreditlimit)

router.put("/update", accountController.updateBalance);

router.put("/updatecredit", accountController.updatecredit);

router.get("/ownedaccounts",accountController.getOwnedAccounts);



router.get("/:id/users", accountController.getConnectedUsers);

router.post("/addusertoaccount", accountController.addUserToAccount);

router.post("/addaccount",accountController.addAccount);


router.delete("/user",  accountController.disconnectUser);

router.delete("/deleteacc",  accountController.deleteAccount);

module.exports = router;