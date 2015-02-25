/*jslint devel:true */
/*global $, $ready, $animate, io, fs, URL, FileReader */

$ready(function () {
	"use strict";
	var logShowState = 0,
		logMin = "0px",
		logMax = "400px",
		logMaxW = "800px",
		logCloseBottomMin = "10px",
		logCloseBottomMax = "390px",
		menuTabState = 0,
		menuMin = "30px",
		menuMax = "280px";
	$('closeConsoleOutput').addEventListener('click', function () {
		$('consoleOutput').style.overflow = "hidden";
		if (logShowState === 0) {
			logShowState = 1;
			$animate($('consoleTextBlock'), {
				'height' : {
					from: logMax,
					to: logMin
				},
				'width' : {
					from : logMax,
					to : logMin
				}
			}, 1000, function () {
				logShowState = 2;
			});
			$animate($('consoleOutput'), {
				'height' : {
					from: logMax,
					to: logMin
				},
				'width' : {
					from: logMax,
					to: logMin
				}
			}, 1000, function () {
				logShowState = 2;
				$('closeConsoleOutput').value = "^";
			});
			$animate($('closeConsoleOutput'), {
				'bottom': {
					from: logCloseBottomMax,
					to : logCloseBottomMin
				}
			}, 1000, function () {
				logShowState = 2;
			});
		} else if (logShowState === 2) {
			logShowState = 3;
			$animate($('consoleTextBlock'), {
				'height' : {
					from: logMin,
					to: logMax
				},
				'width' : {
					from: logMin,
					to: logMaxW
				}
			}, 1000, function () {
				logShowState = 0;
			});
			$animate($('consoleOutput'), {
				'height' : {
					from: logMin,
					to: logMax
				},
				'width' : {
					from: logMin,
					to: logMaxW
				}
			}, 1000, function () {
				$('closeConsoleOutput').value = "v";
				$('consoleOutput').style.overflow = "auto";
				logShowState = 0;
			});
			$animate($('closeConsoleOutput'), {
				'bottom': {
					from: logCloseBottomMin,
					to : logCloseBottomMax
				}
			}, 1000, function () {
				logShowState = 0;
			});
		}
	});
	$('closeMenuTab').addEventListener('click', function () {
		$('menuTab').style.overflow = "hidden";
		if (menuTabState === 0) {
			menuTabState = 1;
			$animate($('menuTab'), {
				'width' : {
					from : menuMax,
					to : menuMin
				}
			}, 1000, function () {
				$('closeMenuTab').value = "<<";
				menuTabState = 2;
			});
		} else if (menuTabState === 2) {
			menuTabState = 3;
			$animate($('menuTab'), {
				'width' : {
					from : menuMin,
					to : menuMax
				}
			}, 1000, function () {
				$('closeMenuTab').value = ">>";
				$('menuTab').style.overflow = "auto";
				menuTabState = 0;
			});
		}
	});
});


