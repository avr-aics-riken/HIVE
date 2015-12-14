/*jslint devel:true */

(function () {
	'use strict';
	var app = require('app'),
		BrowserWindow = require('browser-window'),
		mainWindow;

	//require('crash-reporter').start();

	app.on('window-all-closed', function() {
		if (process.platform != 'darwin') {
			app.quit();
		}
	});

	app.on('ready', function() {
		mainWindow = new BrowserWindow({width: 800, height: 600});
		mainWindow.loadURL('file://' + __dirname + '/index.html');
		mainWindow.on('closed', function() {
			mainWindow = null;
		});
	});
}());
