var express = require('express');
var router = express.Router();
var db = require("../config/db");
const usercontroller = require("../controllers/usercontroller");
//const jwtAuth = require("../config/jwtAuth.js");//

router.get("/info",usercontroller.userInfo);

router.get('/user',usercontroller.getAll)

router.get('/:id',usercontroller.getByiduser);

router.post("/login",usercontroller.userLogin);

router.post("/register",usercontroller.userRegister);

router.put("/avatar", usercontroller.updateAvatar);

router.delete("/deleteuser",usercontroller.deleteuser)

module.exports = router;