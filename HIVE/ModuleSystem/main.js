/*jslint devel:true, node:true, nomen:true */
/*global process, require */
var app = require('app');
var BrowserWindow = require('browser-window');
var path = require('path');
var spawn = require('child_process').spawn;
var exec = require('child_process').exec;
var fs = require('fs');
var os = require('os');

// ウィンドウオブジェクトはGCに食われないようにグローバルに持っておく必要がある
var mainWindow = null;

// ウィンドウがすべて閉じたらアプリ終了.
app.on('window-all-closed', function () {
	"use strict";
	app.quit();
});

var server = require('./server');

// This method will be called when Electron has done everything
// initialization and ready for creating browser windows.
app.on('ready', function () {
	"use strict";
	var argv = process.argv,
		egargs = [];
		//connectURL = "ipc:///tmp/HIVE_IPC" + Math.random().toString().slice(0, 5),
		//encodedURL = encodeURIComponent(connectURL);

	// BrowserWindowの作成
	mainWindow = new BrowserWindow({
		"frame": true,
        "autoHideMenuBar": true,
        "width": 1280,
        "height": 800,
        "title": "HIVE Module System"/*,
		"titleBarStyle": "hidden",*/
	});

    // ズーム禁止
    //var webFrame = require('electron').webFrame;
    //webFrame.setZoomLevelLimits(1,1);
    
    
    //mainWindow.loadURL(path.join('file://' + __dirname, './client/index.html') + '?url=' + encodedURL);
    mainWindow.loadURL(path.join('file://' + __dirname, './client/index.html') + '?port=' + String(server.getPort()));

	mainWindow.on('close', function () {
		if (server.stopHRenderServer) {
			server.stopHRenderServer();
		}
	});

	// ウィンドウが閉じられた
	mainWindow.on('closed', (function () {
		return function () {
			// Dereference the window object, usually you would store windows
			// in an array if your app supports multi windows, this is the time
			// when you should delete the corresponding element.
			mainWindow = null;
		}
	}()));
});
