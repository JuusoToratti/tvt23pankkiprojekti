var express = require('express');
var path = require('path');
var cookieParser = require('cookie-parser');
var logger = require('morgan');

var indexRouter = require('./routes/index');
const usersRouter = require('./routes/users.js');
const accountsRouter = require('./routes/accounts.js');
const cardroutesRouter = require('./routes/cardroutes.js');


var app = express();

app.use(logger('dev'));
app.use(express.json());
app.use(express.urlencoded({ extended: false }));
app.use(cookieParser());
app.use(express.static(path.join(__dirname, 'public')));

app.use('/', indexRouter);
app.use('/users', usersRouter);
app.use('/accounts',accountsRouter);
app.use('/addusertoaccount',accountsRouter);
app.use('/cardroutes',cardroutesRouter);

module.exports = app;