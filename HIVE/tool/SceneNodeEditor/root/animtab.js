/*jslint devel:true */
/*global $, $ready, $animate, io, fs, URL */
(function () {
	"use strict";

	var animtab = {},
		buttonNum = 0,
		initialOverflow = {},
		isMoving = {};
	
	function to_json(key, value) {
		var json = {};
		json[key] = value;
		return json;
	}
	
	function to_num(pixelStr) {
		return pixelStr.split('px').join('');
	}
	
	/// initialize dialog and set separator 
	function setupSeparator(direction, separator, button, targets, whstr) {
		var dragging = "no", // "no", "ready", "yes"
			buttonID = Object.keys(button)[0],
			targetID = Object.keys(targets)[0],
			buttonElem = document.getElementById(buttonID),
			diffButtonTargetMax = 0;
		
		diffButtonTargetMax = to_num(button[buttonID].max) - to_num(targets[targetID].max);
		//console.log("diffButtonTargetMax" + diffButtonTargetMax);
		
		separator.onmousedown = function (e) {
			var target,
				id;
			
			e.preventDefault();
			
			dragging = "ready";
			for (id in targets) {
				if (targets.hasOwnProperty(id)) {
					target = document.getElementById(id);
					target.style.overflow = "hidden";
				}
			}
		};
		separator.onmouseover = function (e) {
			//e.preventDefault();
			
			if (direction === 'left') {
				separator.style.cursor = "w-resize";
			} else if (direction === 'right') {
				separator.style.cursor = "e-resize";
			} else if (direction === 'top') {
				separator.style.cursor = "s-resize";
			} else if (direction === 'bottom') {
				separator.style.cursor = "n-resize";
			}
		};
		document.addEventListener('mouseup', function (e) {
			var target,
				id;
			
			e.preventDefault();
			
			dragging = "no";
			for (id in targets) {
				if (targets.hasOwnProperty(id)) {
					target = document.getElementById(id);
					target.style.overflow = initialOverflow[id];
				}
			}
		});
		document.addEventListener('mousemove', function (e) {
			e.preventDefault();
			
			var offset,
				pos,
				i,
				target,
				id;
			if (dragging === "yes") {
				isMoving[separator] = true;
				
				if (direction === 'left') {
					offset = window.pageXOffset || document.documentElement.scrollLeft;
					pos = offset + e.clientX;
				} else if (direction === 'right') {
					offset = window.pageXOffset || document.documentElement.scrollLeft;
					pos = document.documentElement.clientWidth - (offset + e.clientX);
				} else if (direction === 'top') {
					offset = window.pageYOffset || document.documentElement.scrollTop;
					pos = offset + e.clientY;
				} else if (direction === 'bottom') {
					offset = window.pageYOffset || document.documentElement.scrollTop;
					pos = document.documentElement.clientHeight - (offset + e.clientY);
				}
				//console.log(direction + ":" + pos);
				for (id in targets) {
					if (targets.hasOwnProperty(id)) {
						target = document.getElementById(id);
						target.style[whstr] = pos + 'px';
						targets[id].max = pos + 'px';
					}
				}
				separator.style[direction] = pos + 'px';
				buttonElem.style[direction] = (pos + diffButtonTargetMax) + 'px';
				button[buttonID].max = (pos + diffButtonTargetMax) + 'px';
			}
			if (dragging === "ready") {
				dragging = "yes";
			}
		});
	}
	
	function create(direction, button, targets, textlabel) {
		var buttonElem = document.createElement("input"),
			separatorElem = document.createElement("span"),
			buttonID = Object.keys(button)[0],
			buttonMin = button[buttonID].min,
			buttonMax = button[buttonID].max,
			whstr,
			json,
			time = 500,
			state = 0,
			targetElem,
			targetMin,
			targetMax,
			minimum = Number.MAX_VALUE,
			maximum = Number.MIN_VALUE,
			temp,
			id,
			isDragging = "no";
		
		/*
		buttonElem.type = "button";
		buttonElem.id = buttonID;
		buttonElem.className = buttonID;
		buttonElem.style.position = "absolute";
		buttonElem.value = beforeLabel;
		*/
		separatorElem.innerHTML = textlabel;
		
		if (direction === 'bottom' || direction === 'top') {
			whstr = 'height';
			//separatorElem.style.width = "100px";
			//separatorElem.style.height = "20px";
		} else if (direction === 'right' || direction === 'left') {
			whstr = 'width';
			//separatorElem.style.width = "20px";
			//separatorElem.style.height = "100px";
		}
		separatorElem.id = buttonID;
		separatorElem.className = buttonID;
		separatorElem.style.position = "absolute";
		
		// add elements
		document.body.appendChild(separatorElem);
		//document.body.appendChild(buttonElem);
		
		for (id in targets) {
			if (targets.hasOwnProperty(id)) {
				targetElem = $(id);
				initialOverflow[id] = targetElem.style.overflow;
				targetMin = targets[id].min;
				targetMax = targets[id].max;
				temp = to_num(targetMin);
				if (temp < minimum) {
					minimum = temp;
				}
				temp = to_num(targetMax);
				//console.log(temp);
				if (temp > maximum) {
					maximum = temp;
				}
			}
		}
		//console.log("maximum:" + maximum);
		separatorElem.style[direction] = maximum + 'px';
		if (buttonMin === 'auto') {
			buttonMin = minimum + 'px';
		}
		if (buttonMax === 'auto') {
			buttonMax = maximum + 'px';
		}
		//console.log("buttonMin:" + buttonMin);
		//console.log("buttonMax:" + buttonMax);
		button[buttonID].min = buttonMin;
		button[buttonID].max = buttonMax;
		
		function createAnimateButton(e) {
			var i = 0,
				id,
				targetElem,
				targetMin,
				targetMax;
			
			buttonMin = button[buttonID].min;
			buttonMax = button[buttonID].max;

			function beforeTarget() {
				state = 2;
			}
			
			function afterTarget() {
				targetElem.style.overflow = initialOverflow[id];
				state = 0;
			}
			
			function beforeButton() {
				//buttonElem.value = afterLabel;
			}
			
			function afterButton() {
				//buttonElem.value = beforeLabel;
			}
			
			function beforeSep() {}
			function afterSep() {}
			
			for (id in targets) {
				if (targets.hasOwnProperty(id)) {
					targetElem = $(id);
					targetElem.style.overflow = "hidden";
					targetMin = targets[id].min;
					targetMax = targets[id].max;
					if (state === 0) {
						state = 1;
						$animate(targetElem, to_json(whstr, { from: targetMax, to: targetMin }), time, beforeTarget);
						//$animate(buttonElem, to_json(direction, { from : buttonMax, to : buttonMin }), time, beforeButton);
						$animate(separatorElem, to_json(direction, { from : buttonMax, to : buttonMin }), time, beforeSep);
					} else if (state === 2) {
						state = 3;
						$animate(targetElem, to_json(whstr, { from: targetMin, to: targetMax }), time, afterTarget);
						//$animate(buttonElem, to_json(direction, { from : buttonMin, to : buttonMax }), time, afterButton);
						$animate(separatorElem, to_json(direction, { from : buttonMin, to : buttonMax }), time, afterSep);
					}
				}
			}
		}
		
		isMoving[separatorElem] = false;
		setupSeparator(direction, separatorElem, button, targets, whstr, createAnimateButton);
		
		function createButton(direction, targets) {
			separatorElem.addEventListener('click', function () {
				if (isMoving.hasOwnProperty(separatorElem)) {
					//console.log("isMoving:" + isMoving[separatorElem]);
					if (!isMoving[separatorElem]) {
						createAnimateButton();
					}
					isMoving[separatorElem] = false;
				}
			});
		}
		
		$ready((function (direction, button, targets, textlabel) { return function () {
			createButton(direction, button, targets, textlabel);
		}; }(direction, button, targets, textlabel)));
	}
	
	window.animtab = animtab;
	window.animtab.create = create;
}());
