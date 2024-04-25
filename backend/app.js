var express = require('express');
var path = require('path');
var cookieParser = require('cookie-parser');
var logger = require('morgan');

var indexRouter = require('./routes/index');
const usersRouter = require('./routes/users.js');
const accountsRouter = require('./routes/accounts.js');
const cardroutesRouter = require('./routes/cardroutes.js');
const accountinformationRouter = require ('./routes/accountinformation.js');
const jwt = require('jsonwebtoken');
var loginRouter = require('./routes/login.js');


var app = express();

app.use(logger('dev'));
app.use(express.json());
app.use(express.urlencoded({ extended: false }));
app.use(cookieParser());
app.use(express.static(path.join(__dirname, 'public')));

app.use('/', indexRouter);
app.use('/login',loginRouter);
//app.use(authenticateToken);
app.use('/users', usersRouter);
app.use('/accounts',accountsRouter);
app.use('/addusertoaccount',accountsRouter);
app.use('/cardroutes',cardroutesRouter);
app.use('/accountinformation',accountinformationRouter);


function authenticateToken(req, res, next) {
    const authHeader = req.headers['authorization']
    const token = authHeader && authHeader.split(' ')[1]
  
    console.log("token = "+token);
    if (token == null) return res.sendStatus(401)
  
    jwt.verify(token, process.env.MY_TOKEN, function(err, user) {
  
      if (err) return res.sendStatus(403)

      req.user = user
  
      next()
    })
  }


module.exports = app;