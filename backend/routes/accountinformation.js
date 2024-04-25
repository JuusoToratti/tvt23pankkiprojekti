var express = require('express');
var router = express.Router();
const db = require("../config/db");
const accountinformationcontroller = require("../controllers/accountinformationcontroller.js");
//const jwtAuth = require("../config/jwtAuth");//pois väliaikaisesti

router.get("/info",accountinformationcontroller.getAll);

router.get("/infocredit",accountinformationcontroller.getcredit);

router.get("/getByCardNumber/:card_number/:page",accountinformationcontroller.getByCardNumber)

router.get("/getByCardNumber/:card_number",accountinformationcontroller.getByCardNumberFixed)

router.post("/create",accountinformationcontroller.add);

router.post("/createcredit",accountinformationcontroller.add)

router.delete("/delete",accountinformationcontroller.deleteaccoutinformation);

module.exports = router;