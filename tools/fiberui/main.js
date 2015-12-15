/*jslint devel:true */

(function () {
	'use strict';
	var app = require('app'),
		BrowserWindow = require('browser-window'),
		mainWindow;

	//require('crash-reporter').start();
	var server = require('./server');

	app.on('window-all-closed', function() {
		app.quit();
	});

	app.on('ready', function() {

	// ブラウザ(Chromium)の起動, 初期画面のロード
	mainWindow = new BrowserWindow({width: 800, height: 600});
	mainWindow.loadURL('file://' + __dirname + '/index.html');

	mainWindow.on('closed',
		function() {
			if (server.stopHRenderServer) {
				server.stopHRenderServer();
			}
			mainWindow = null;
		});
	});
}());
