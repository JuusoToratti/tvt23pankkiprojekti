var express = require('express');
var router = express.Router();
const db = require("../config/db");
const logsController = require("../controllers/accountinformationcontroller");
//const jwtAuth = require("../config/jwtAuth");//pois väliaikaisesti

router.get("/",jwtAuth.verifyToken,accountinformationcontroller.getAll);

router.get("/getByCardNumber/:card_number/:page",jwtAuth.verifyToken,accountinformationcontroller.getByCardNumber)

router.get("/getByCardNumber/:card_number",jwtAuth.verifyToken,accountinformationcontroller.getByCardNumberFixed)

router.post("/create",jwtAuth.verifyToken,accountinformationcontroller.add);

router.delete("/delete",jwtAuth.verifyToken,accountinformationcontroller.deleteaccountinformation);

module.exports = router;