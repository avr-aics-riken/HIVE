/*jslint devel:true */
/*global SVG, svgNodeUI, io, fs, URL, FileReader */

$ready(function () {
	var logShowState = 0,
		logMin = "40px",
		logMax = "400px",
		logMaxW = "800px",
		menuTabState = 0,
		menuMin = "30px",
		menuMax = "480px";
	$('closeConsoleOutput').addEventListener('click', function () {
		if (logShowState === 0) {
			logShowState = 1;
			$animate($('consoleTextBlock'), {
				'height' : {
					from: logMax,
					to: logMin
				}
				,'width' : {
					from: logMax,
					to: logMin
				}
			}, 1000, function () {
				logShowState = 2;
			});
			$animate($('consoleOutput'), {
				'height' : {
					from: logMax,
					to: logMin
				}
				,'width' : {
					from: logMax,
					to: logMin
				}
			}, 1000, function () {
				logShowState = 2;
				$('closeConsoleOutput').value = "^";
			});
		} else if (logShowState === 2) {
			logShowState = 3;
			$animate($('consoleTextBlock'), {
				'height' : {
					from: logMin,
					to: logMax
				}
				,'width' : {
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
				}
				,'width' : {
					from: logMin,
					to: logMaxW
				}
			}, 1000, function () {
				$('closeConsoleOutput').value = "v";
				logShowState = 0;
			});
		}
	});
	$('closeMenuTab').addEventListener('click', function () {
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
				menuTabState = 0;
			});
		}
	});
});


