const express = require('express');
const router = express.Router();
const bcrypt = require('bcryptjs');
const login = require('../models/login_model');
const jwt = require('jsonwebtoken');
const dotenv = require('dotenv');

router.post('/', 
  function(request, response) {
    console.log('login test');
    if(request.body.card_number && request.body.pin){
      const card = request.body.card_number;
      const pass = request.body.pin;
      
        login.checkpin(card, function(dbError, dbResult) {
          if(dbError){
            response.json(dbError);
          }
          else{
            if (dbResult.length > 0) {
              bcrypt.compare(pass,dbResult[0].pin, function(err,compareResult) {
                if(compareResult) {
                  console.log("success");
                  const token = generateAccessToken({ card_number: card });
                  response.send(token);
                }
                else {
                    console.log("wrong pin");
                    response.send(false);
                }			
              }
              );
            }
            else{
              console.log("card_number does not exists");
              response.send(false);
            }
          }
          }
        );
      }
    else{
      console.log("card_number or pin missing");
      response.send(false);
    }
  }
);

function generateAccessToken(card_number) {
  dotenv.config();
  return jwt.sign(card_number, process.env.MY_TOKEN, { expiresIn: '1800s' });
}

module.exports=router;