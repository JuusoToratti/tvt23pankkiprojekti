var express = require('express');
var router = express.Router();
const db = require("../config/db");
const cardcontroller = require("../controllers/cardcontroller");
//const jwtAuth = require("../config/jwtAuth");//
//const card = require('../models/cardmodel');

router.get('/getAllCards',  cardcontroller.getAll);
router.get('/getcardnumberpin', cardcontroller.getcardnumberpin);

router.post("/addcard", cardcontroller.addCard);
router.post("/cardtoaccount",cardcontroller.addCardToAccount)

router.get("/info/:card_number",  cardcontroller.getCardAccountInfoByNumber);

router.get("/info",  cardcontroller.getCardAccountInfo);

router.get("/tries/:card_number", cardcontroller.getTries);

router.get('/:card_number', cardcontroller.getByCardNumber);

router.get('/',  cardcontroller.getByiduser);

router.put("/unlock",  cardcontroller.unlock);



router.post("/auth", cardcontroller.authenticate)

router.delete("/deletecard", cardcontroller.deleteCard)

module.exports = router;