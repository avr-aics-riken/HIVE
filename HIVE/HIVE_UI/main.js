'use strict';

var app = require('app');
var BrowserWindow = require('browser-window');

require('crash-reporter').start();

var mainWindow = null;
var server = require('./server');

app.on('window-all-closed', function() {
	app.quit();
});

app.on('ready', function() {

	// ブラウザ(Chromium)の起動, 初期画面のロード
	mainWindow = new BrowserWindow({width: 1280, height: 900});
	mainWindow.loadURL('file://' + __dirname + '/root/index.html');

	mainWindow.on('closed',
		function() {
			if (server.stopHRenderServer) {
				server.stopHRenderServer();
			}
			mainWindow = null;
	    });
    });
