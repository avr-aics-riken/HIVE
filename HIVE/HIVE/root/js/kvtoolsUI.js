var _kvtools_UIs = {};
function kUI(id_name){
	return _kvtools_UIs[id_name];
}
//------------------------------------------------------

var KListClass = function(wrapper){
	this.wrapper = wrapper;
	this.selectedValue = null;
}
	
// methods
KListClass.prototype = {
	AddItem:function(itemname,idname){
		var itm = document.getElementById('KList-Item');
		if (!itm){
			console.log('Not found ID:KList-Item');
			return;
		}
		itm =  itm.children[0];

		var newitem = itm.cloneNode(true);
		newitem.setAttribute('id',idname);
		newitem.setAttribute('alt',itemname);
		//console.log(newitem);
		
		var s = newitem.getElementsByClassName('KList-Item-Caption');
		if (s.length){
			s[0].innerHTML = itemname;
		}
		var d = newitem.getElementsByClassName('KDustButton');
		if (d.length>0){
			d[0].setAttribute('id',idname+'-dustbtn');
		}
		
		this.wrapper.appendChild(newitem);
	},
	
	Clear:function(){
		this.wrapper.innerHTML = '';
	},
	
	Select:function(val){
		var wp = this.wrapper.childNodes;
		for (var i = 0; i < wp.length; ++i){
			wp[i].style.backgroundColor = "transparent";

			if (wp[i].getAttribute('alt') === val){
				wp[i].style.backgroundColor = "rgb(214, 79, 40)";
				this.selectedValue = val;
			}
		}
	},
	
	GetSelectedValue : function() {
		return this.selectedValue;
	}
	
}

KListClass.Init = function(){
	var lst = document.getElementsByClassName('KList');
	for (var i=0; i<lst.length;++i) {
		//console.log(lst[i].id);
		_kvtools_UIs[lst[i].id] = new KListClass(lst[i]);
	}
}

KListClass.userInit = function(dom){
	var el = dom.getElementsByClassName('KList');
	for (var k = 0; k < el.length; ++k) {
		_kvtools_UIs[el[k].id] = new KListClass(el[k]);
	}
}


//------------------------------------------------------

var KButtonClass = function(wrapper){
	this.wrapper = wrapper;
	
	// register events
	wrapper.addEventListener('click',function(inst){
		return function(e){inst.onClick(e)}
	}(this));
}

// methods
KButtonClass.prototype = {
	onClick:function(e){
		//console.log('Button clicked.');
		if (this.clickFunc)
			this.clickFunc();
	},
	setValue:function(val){
		this.wrapper.innerHTML = val;
	},
	getValue:function(){
		return this.wrapper.innerHTML;
	}
}

KButtonClass.Init = function(){
	var lst = document.getElementsByClassName('KButton');
	for (var i=0; i<lst.length;++i) {
		//console.log(lst[i].id);
		_kvtools_UIs[lst[i].id] = new KButtonClass(lst[i]);
	}
}

KButtonClass.userInit = function(dom){
	var el = dom.getElementsByClassName('KButton');
	for (var k = 0; k < el.length; ++k) {
		_kvtools_UIs[el[k].id] = new KButtonClass(el[k]);
	}
}

//------------------------------------------------------

var KWindowClass = function(wrapper){
	this.wrapper = wrapper;
	
	// register events
	this.dragging = false;
	this.mstartx = 0;
	this.mstarty = 0;
	var sx = wrapper.style.left.split('px');
	var sy = wrapper.style.top.split('px');
	this.x = (sx.length > 0 ? parseInt(sx[0]) : 0)
	this.y = (sy.length > 0 ? parseInt(sy[0]) : 0)
	
	var res = wrapper.getElementsByClassName('KWindow-TitleBar');
	if (res.length == 0)
		return;
	var movewrapper = res[0];
	movewrapper.addEventListener('mousedown',function(inst){return function(e){
		inst.dragging = true;
		inst.mstartx = e.clientX;
		inst.mstarty = e.clientY;

		function mup(inst){return function(e){
			inst.dragging = false;
			document.removeEventListener('mouseup',inst.mup);
			document.removeEventListener('mousemove',inst.mmove);
		}}
		function mmove(inst){return function(e){
			if (inst.dragging){
				var dx = e.clientX - inst.mstartx;
				var dy = e.clientY - inst.mstarty;
				inst.x += dx;
				inst.y += dy;
				inst.wrapper.style.top  = inst.y +'px';
				inst.wrapper.style.left = inst.x +'px';
			}
			inst.mstartx = e.clientX;
			inst.mstarty = e.clientY;
		}}
		
		inst.mmove = mmove(inst);
		inst.mup   = mup(inst);
		document.addEventListener('mouseup',inst.mup);
		document.addEventListener('mousemove',inst.mmove);
	}}(this));
}

KWindowClass.prototype.SetShow = function(show){
	this.wrapper.style.display = (show ? "" : "none");
}

KWindowClass.prototype.GetShow = function(){
	return (this.wrapper.style.display === "none" ? false : true);
}

KWindowClass.Init = function(){
	var lst = document.getElementsByClassName('KWindow');
	for (var i=0; i<lst.length;++i) {
		//console.log(lst[i].id);
		_kvtools_UIs[lst[i].id] = new KWindowClass(lst[i]);
	}
	var lst = document.getElementsByClassName('KFrameWindow');
	for (var i=0; i<lst.length;++i) {
		//console.log(lst[i].id);
		_kvtools_UIs[lst[i].id] = new KWindowClass(lst[i]);
	}
}

KWindowClass.userInit = function(dom){
	var el = dom.getElementsByClassName('KWindow');
	for (var k = 0; k < el.length; ++k) {
		_kvtools_UIs[el[k].id] = new KWindowClass(el[k]);
	}
	var el = dom.getElementsByClassName('KFrmeWindow');
	for (var k = 0; k < el.length; ++k) {
		_kvtools_UIs[el[k].id] = new KWindowClass(el[k]);
	}
}

//------------------------------------------------------
KCaptionClass = function(wrapper){
	this.wrapper = wrapper;
}

KCaptionClass.prototype = {
	SetCaption:function(cap){
		this.wrapper.innerHTML = cap;
	}
}

KCaptionClass.Init = function(){
	var lst = document.getElementsByClassName('KCaption');
	for (var i=0; i<lst.length;++i) {
		//console.log(lst[i].id);
		_kvtools_UIs[lst[i].id] = new KCaptionClass(lst[i]);
	}
}

KCaptionClass.userInit = function(dom){
	var el = dom.getElementsByClassName('KCaption');
	for (var k = 0; k < el.length; ++k) {
		_kvtools_UIs[el[k].id] = new KCaptionClass(el[k]);
	}
}

//------------------------------------------------------
KShaderListClass = function(wrapper){
	this.wrapper = wrapper;
	this.prototype = KListClass.prototype;
	this.onChangedFunc = null;
	this.selectedVal = null;
	
	this.wrapper.addEventListener('change', function(thisptr){ return function(){
		var val = thisptr.wrapper.value;
		if (thisptr.onChangedFunc)
			thisptr.onChangedFunc(val);
	}}(this));

}

KShaderListClass.prototype.Clear = function () {
	this.wrapper.innerHTML = '';
}

KShaderListClass.prototype.AddItem = function(cap,img, idname){
	var newitem = document.createElement('div'),
		imgdiv = document.createElement('img'),
		txtdiv = document.createElement('p');
	
	var shadername = (cap.split('.'))[0];
	//console.log(shadername);
	imgdiv.classList.add('ppCell');
	imgdiv.setAttribute('src',img);
	imgdiv.setAttribute('style','width:64px;height:64px');
	txtdiv.classList.add('ppCell');
	txtdiv.innerHTML = cap.replace('.frag','');
	txtdiv.style.width = '300px';
	
	newitem.setAttribute('id', idname);
	newitem.setAttribute('alt', shadername+'.frag');
	newitem.classList.add('ppRow');
	newitem.appendChild(imgdiv);
	newitem.appendChild(txtdiv);
	
	this.wrapper.appendChild(newitem);
}

KShaderListClass.prototype.GetSelectedValue = function () {
	return this.selectedVal;
}

KShaderListClass.prototype.ChangeCallback = function (func) {
	this.onChangedFunc = func;
}

KShaderListClass.prototype.Select = function (val) {
	var tg = this.wrapper.childNodes;
	for (var i = 0; i < tg.length; ++i){
		tg[i].style.backgroundColor = "transparent";
		
		if (tg[i].getAttribute('alt') === val){
			this.selectedVal = val;
			tg[i].style.backgroundColor = "rgb(214, 79, 40)";
		}
	}
	if (this.onChangedFunc) {
		this.onChangedFunc(val);
	}
}

KShaderListClass.prototype.Show = function(show){
	this.wrapper.style.display = (show ? "" : "none");
}

KShaderListClass.Init = function(){
	var lst = document.getElementsByClassName('KShaderList');
	for (var i=0; i<lst.length;++i) {
		//console.log(lst[i].id);
		_kvtools_UIs[lst[i].id] = new KShaderListClass(lst[i]);
	}
}

KShaderListClass.userInit = function(dom){
	var el = dom.getElementsByClassName('KShaderList');
	for (var k = 0; k < el.length; ++k) {
		_kvtools_UIs[dom.id] = new KShaderListClass(dom);
	}
}

//------------------------------------------------------
KDropdownListClass = function(wrapper){
	this.wrapper = wrapper;
	this.prototype = KListClass.prototype;
	this.onChangedFunc = null;
	
	this.wrapper.addEventListener('change', function(thisptr){ return function(){
		var val = thisptr.wrapper.value;
		if (thisptr.onChangedFunc)
			thisptr.onChangedFunc(val);
	}}(this));
}

KDropdownListClass.prototype.Show = function(show){
	this.wrapper.style.display = (show ? "" : "none");
}

KDropdownListClass.Init = function(){
	var lst = document.getElementsByClassName('KDropdownList');
	for (var i=0; i<lst.length;++i) {
		//console.log(lst[i].id);
		_kvtools_UIs[lst[i].id] = new KDropdownListClass(lst[i]);
	}
}

KDropdownListClass.prototype.ChangeCallback = function (func) {
	this.onChangedFunc = func;
}

KDropdownListClass.userInit = function(dom){
	var el = dom.getElementsByClassName('KDropdownList');
	for (var k = 0; k < el.length; ++k) {
		_kvtools_UIs[el[k].id] = new KDropdownListClass(el[k]);
	}
}


//------------------------------------------------------
KColorPickerClass = function(wrapper){
	//console.log(wrapper);
	this.wrapper = wrapper;
	
	var c = document.createElement('canvas');
	this.wrapper.appendChild(c);
	

	this.wrapper.setAttribute('style','position:relative;');
	var inputR = document.createElement('input'),
		inputG = document.createElement('input'),
		inputB = document.createElement('input'),
		inputA = document.createElement('input');
	inputR.setAttribute('style','position:absolute;left:0px;bottom:0px;width:50px');
	inputG.setAttribute('style','position:absolute;left:50px;bottom:0px;width:50px');
	inputB.setAttribute('style','position:absolute;left:100px;bottom:0px;width:50px');
	inputA.setAttribute('style','position:absolute;left:150px;bottom:0px;width:50px');
	inputR.classList.add('KColorPickerInput');
	inputG.classList.add('KColorPickerInput');
	inputB.classList.add('KColorPickerInput');
	inputA.classList.add('KColorPickerInput');
	this.wrapper.appendChild(inputR);
	this.wrapper.appendChild(inputG);
	this.wrapper.appendChild(inputB);
	this.wrapper.appendChild(inputA);
	this.valueR = inputR;
	this.valueG = inputG;
	this.valueB = inputB;
	this.valueA = inputA;
	
	this.circleRGB = [255,255,255];
	this.circleXY = [cw*0.5, cw*0.5];
	this.blackWhiteX = 1.0;
	this.alphaX = 1.0;
	
	/*var rct = this.wrapper.getBoundingClientRect(),
		cw = rct.right - rct.left,
		ch = rct.bottom - rct.top;*/
	var cw = 200,
		ch = 267;
	
	this.changeColorCallback = null;
	// this.changeColorCallback = function (r,g,b,a) { console.log(r,g,b,a);} // debug
	
	this.imageObj = new Image();
	this.imageObj.src = 'image/colorpicker.png';
	this.imageObj.onload = function(thisptr) { return function() {
		var margin_x = 0,
			margin_y = 0;
		
		thisptr.wrapper.style.width  = cw + margin_x * 2;
		thisptr.wrapper.style.height = ch + margin_y * 2;
		
		c.style.width  = c.width  = cw;
		c.style.height = c.height = ch;
		var ctx = c.getContext ('2d');
		thisptr.ctx = ctx;

		thisptr.drawGradBar = function (sx, sy, w, h) {
			var bwGrad = ctx.createLinearGradient(sx, 0, w, 0);
			bwGrad.addColorStop(0.0,'#000');
			bwGrad.addColorStop(1.0,'#FFF');

			this.ctx.fillStyle = bwGrad;
			this.ctx.fillRect (sx, sy, w, h);
		}

		thisptr.drawColorCircle = function () {
			this.ctx.clearRect(0,0,cw,ch);
			this.drawGradBar(0, cw+5, cw, 15);
			this.drawGradBar(0, cw+25, cw, 15);
			this.ctx.drawImage(this.imageObj, margin_x, margin_y);
		}
		thisptr.drawPicker = function (dx, dy, rad, col) {
			this.ctx.beginPath();
			this.ctx.lineWidth = 2;
			this.ctx.strokeStyle = "#777";
			this.ctx.arc(dx,dy,rad, 0, Math.PI*2, true);
			this.ctx.fillStyle = col;
			this.ctx.fill();
			this.ctx.stroke();
		}
		
		thisptr.drawPickPoints = function () {
			var bw,alp,
				color = 'rgb(' + this.circleRGB[0] + ',' + this.circleRGB[1] + ',' + this.circleRGB[2] + ')';
			this.drawPicker(this.circleXY[0], this.circleXY[1], 8, color);
			bw = Math.floor(this.blackWhiteX * 255);
			this.drawPicker(this.blackWhiteX * cw, cw + 12, 8, 'rgb('+bw+','+bw+','+bw+')');
			alp = Math.floor(this.alphaX * 255);
			this.drawPicker(this.alphaX * cw, cw + 32, 8, 'rgb('+alp+','+alp+','+alp+')');
			
			// update values
			this.valueR.value = Math.floor(this.circleRGB[0] * this.blackWhiteX);
			this.valueG.value = Math.floor(this.circleRGB[1] * this.blackWhiteX);
			this.valueB.value = Math.floor(this.circleRGB[2] * this.blackWhiteX);
			this.valueA.value = Math.floor(this.alphaX * 255);
		}
		
		thisptr.pickColor = function (x,y) {
			this.drawColorCircle();
			var imageData = ctx.getImageData(0, 0, this.imageObj.width, this.imageObj.height),
				data = imageData.data,
				ctrx = cw * 0.5,
				ctry = cw * 0.5,
				vec = [x - ctrx, y - ctry],
				rd = Math.sqrt(vec[0]*vec[0] + vec[1]*vec[1]);
					
			vec[0] /= rd;
			vec[1] /= rd;
			rd = Math.min(rd-1, ctrx);
			x = Math.floor(vec[0] * rd + ctrx);
			y = Math.floor(vec[1] * rd + ctry);

			this.circleXY = [x, y];
			
			//console.log(x,y);
			var red = data[((this.imageObj.width * y) + x) * 4];
			var green = data[((this.imageObj.width * y) + x) * 4 + 1];
			var blue = data[((this.imageObj.width * y) + x) * 4 + 2];
			this.circleRGB = [red, green, blue];
		
			this.drawPickPoints();

			if (this.changeColorCallback){
				var col = this.getColor();
				this.changeColorCallback(col[0],col[1],col[2],col[3]);
			}
		}
		
		thisptr.pickColorGrad = function (x,y) {
			this.drawColorCircle();
			
			var bw = Math.max(0.0, Math.min(x, cw)) / cw;
			this.blackWhiteX = bw;
			
			this.drawPickPoints();

			if (this.changeColorCallback){
				var col = this.getColor();
				this.changeColorCallback(col[0],col[1],col[2],col[3]);
			}
		}
		thisptr.pickColorAlpha = function (x,y) {
			this.drawColorCircle();
			
			var alp = Math.max(0.0, Math.min(x, cw)) / cw;
			this.alphaX = alp;
			
			this.drawPickPoints();

			if (this.changeColorCallback){
				var col = this.getColor();
				this.changeColorCallback(col[0],col[1],col[2],col[3]);
			}
		}
		
		thisptr.mspress = false;
		thisptr.mspressGrad = false;
		thisptr.mspressAlpha = false;
		c.addEventListener('mousedown', function(thisptr){ return function(e){
			var x = e.clientX - thisptr.wrapper.getBoundingClientRect().left,
				y = e.clientY - thisptr.wrapper.getBoundingClientRect().top;
			if (y < cw) { // circle
				thisptr.mspress = true;
				thisptr.pickColor(x,y);
			} else if (y < cw + 20) { // grad
				thisptr.mspressGrad = true;
				thisptr.pickColorGrad(x,y);
			} else { // alpha
				thisptr.mspressAlpha = true;
				thisptr.pickColorAlpha(x,y);
			}
		}}(thisptr));
		document.body.addEventListener('mouseup', function(thisptr){ return function(e){
			thisptr.mspress = false;
			thisptr.mspressGrad = false;
			thisptr.mspressAlpha = false;
		}}(thisptr));
		document.body.addEventListener('mousemove', function(thisptr){ return function(e){
			var x = e.clientX - thisptr.wrapper.getBoundingClientRect().left,
				y = e.clientY - thisptr.wrapper.getBoundingClientRect().top;
			if (thisptr.mspress){
				thisptr.pickColor(x,y);
			}
			if (thisptr.mspressGrad) {
				thisptr.pickColorGrad(x,y);
			}
			if (thisptr.mspressAlpha) {
				thisptr.pickColorAlpha(x,y);
			}
		}}(thisptr));

		thisptr.drawColorCircle();
		thisptr.drawPickPoints();
	
	}}(this);
	
}

KColorPickerClass.prototype = {
	getColor : function(){
		return [this.circleRGB[0] / 255.0 * this.blackWhiteX,
				this.circleRGB[1] / 255.0 * this.blackWhiteX,
				this.circleRGB[2] / 255.0 * this.blackWhiteX,
				this.alphaX];
	},
	setColor : function(red, green, blue, alpha){
		
		// TODO: Inverse HS color
		// vec3 rgb = clamp( abs(mod(c.x*6.0+vec3(0.0,4.0,2.0),6.0)-3.0)-1.0, 0.0, 1.0 );
		// rgb = rgb*rgb*(3.0-2.0*rgb);
		// return c.z * mix( vec3(1.0), rgb, c.y) + (1.0-c.z);
		this.circleXY = [100, 100]; // TODO: calc HS
		
		if (red === 0 && green === 0 && blue === 0) {
			red = 1;
			green = 1;
			blue = 1;
			this.blackWhiteX = 0;
		} else {
			var n = Math.sqrt(red * red + green * green + blue * blue);
			this.blackWhiteX = n;
			red /= n;
			green /= n;
			blue /= n;
		}
		this.circleRGB = [Math.floor(red   * 255),
						  Math.floor(green * 255),
						  Math.floor(blue  * 255)];
		this.alphaX = alpha;
	
		if (this.drawColorCircle)
			this.drawColorCircle();
		if (this.drawPickPoints)
			this.drawPickPoints();
		
		// HACK set direct value
		this.valueR.value = this.circleRGB[0];
		this.valueG.value = this.circleRGB[1];
		this.valueB.value = this.circleRGB[2];
		this.valueA.value = Math.floor(alpha * 255);
	},
	ChangeColorCallback : function(func){
		this.changeColorCallback = func;
	}
}

KColorPickerClass.Init = function(){
	var lst = document.getElementsByClassName('KColorPicker');
	for (var i=0; i<lst.length;++i) {
		_kvtools_UIs[lst[i].id] = new KColorPickerClass(lst[i]);
	}
}

KColorPickerClass.userInit = function(dom){
	var el = dom.getElementsByClassName('KColorPicker');
	for (var k = 0; k < el.length; ++k) {
		_kvtools_UIs[el[k].id] = new KColorPickerClass(el[k]);
	}
}



//------------------------------------------------------
KTransferFunctionClass = function(wrapper){
	var i;
	//console.log(wrapper);
	this.wrapper = wrapper;
	
	this.cw = 300;
	this.colorbarHeight = 20;
	
	var c = document.createElement('canvas');
	this.wrapper.appendChild(c);

	var colorframe = document.createElement('canvas');
	colorframe.setAttribute("style", "width:"+this.cw+"px;height:"+this.colorbarHeight+"px;");
	colorframe.width = this.cw;
	colorframe.height = this.colorbarHeight;
	this.wrapper.appendChild(colorframe);
	var colorctx = colorframe.getContext ('2d');
	this.colorctx = colorctx;
	
	// TODO: beautiful me
	var disableColor = "#8b8b8b";
	var btnframe = document.createElement('div');
	btnframe.setAttribute("style", "height:20px; display: flex; display:-webkit-flex; flex-direction:row;");
	var redbtn = document.createElement('div');
	redbtn.setAttribute("style", "background-color:"+disableColor+";width:60px;height:20px;font-size:13px;color:#555;");
	redbtn.innerHTML = "RED";
	var greenbtn = document.createElement('div');
	greenbtn.setAttribute("style", "background-color:"+disableColor+";width:60px;height:20px;font-size:13px;color:#555;");
	greenbtn.innerHTML = "GREEN";
	var bluebtn = document.createElement('div');
	bluebtn.setAttribute("style", "background-color:"+disableColor+";width:60px;height:20px;font-size:13px;color:#555;");
	bluebtn.innerHTML = "BLUE";
	var alphabtn = document.createElement('div');
	alphabtn.setAttribute("style", "background-color:"+disableColor+";width:60px;height:20px;font-size:13px;color:#555;");
	alphabtn.innerHTML = "ALPHA";
	var allbtn = document.createElement('div');
	allbtn.setAttribute("style", "background-color:#FFF;width:60px;height:20px;font-size:13px;color:#555;");
	allbtn.innerHTML = "RGB";

	this.wrapper.appendChild(btnframe);
	btnframe.appendChild(redbtn);
	btnframe.appendChild(greenbtn);
	btnframe.appendChild(bluebtn);
	btnframe.appendChild(allbtn);
	btnframe.appendChild(alphabtn);
	this.redbtn = redbtn;
	this.greenbtn = greenbtn;
	this.bluebtn = bluebtn;
	this.alphabtn = alphabtn;
	this.allbtn = allbtn;
	
	var presetframe = document.createElement('div');
	presetframe.setAttribute("style", "height:30px; display: flex; display:-webkit-flex; flex-direction:row;");
	var preText = document.createElement('div');
	preText.class = 'KCaption';
	preText.innerHTML = 'Preset:';
	preText.style.width = '100px';
	preText.style.color = '#FFF';
	var preSelect = document.createElement('select');
	preSelect.className = 'KDropdownList';
	preSelect.style.width = '200px';
	
	var presetList = '';
	presetList += '<option value="0" selected>Custom</option>';
	presetList += '<option value="1">Blue and Red</option>';
	presetList += '<option value="2">Black and White</option>';
	presetList += '<option value="3">BGR Gradation</option>';
	preSelect.innerHTML = presetList;
	preSelect.addEventListener('change', function (thisptr) { return function () {
		var t = parseInt(preSelect.value);
		if (t === 1) {
			for (i = 0; i < thisptr.numVals; ++i){
				thisptr.valueRed[i]   = i / thisptr.numVals;
				thisptr.valueGreen[i] = 0.0;
				thisptr.valueBlue[i]  = 1.0 - i / thisptr.numVals;
				thisptr.valueAlpha[i] = 1.0;
			}
		} else if (t === 2) {
			for (i = 0; i < thisptr.numVals; ++i){
				thisptr.valueRed[i]   = i / thisptr.numVals;
				thisptr.valueGreen[i] = i / thisptr.numVals;
				thisptr.valueBlue[i]  = i / thisptr.numVals;
				thisptr.valueAlpha[i] = 1.0;
			}
		} else if (t === 3) {
			for (i = 0; i < thisptr.numVals; ++i){
				thisptr.valueRed[i]   = i / thisptr.numVals;
				thisptr.valueGreen[i] = Math.sin(i / thisptr.numVals * Math.PI);
				thisptr.valueBlue[i]  = 1.0 - i / thisptr.numVals;
				thisptr.valueAlpha[i] = 1.0;
			}
		}

		
		thisptr.drawGraph();
		if (thisptr.changeCallback){
			thisptr.changeCallback();
		}
	}}(this));
	
	
	this.wrapper.appendChild(presetframe);
	presetframe.appendChild(preText);
	presetframe.appendChild(preSelect);
	
	var minmaxframe = document.createElement('div');
	minmaxframe.setAttribute("style", "height:30px; display: flex; display:-webkit-flex; flex-direction:row;");
	
	var minText = document.createElement('div');
	minText.class = 'KCaption';
	minText.innerHTML = 'Min:';
	minText.style.width = '50px';
	minText.style.color = '#FFF';
	var maxText = document.createElement('div');
	maxText.class = 'KCaption';
	maxText.innerHTML = 'Max:';
	maxText.style.color = '#FFF';
	maxText.style.width = '50px';
	
	var minInput = document.createElement('input');
	minInput.setAttribute('type', 'number');
	minInput.setAttribute('step', '0.00001');
	minInput.value = '0';
	minInput.style.width = '100px';
	var maxInput = document.createElement('input');
	maxInput.setAttribute('type', 'number');
	maxInput.setAttribute('step', '0.00001');
	maxInput.value = '1';
	maxInput.style.width = '100px';

	this.minInput = minInput;
	this.maxInput = maxInput;
	minInput.addEventListener('keyup', function (thisptr) { return function(e) {
		if (e.keyCode === 8 || (46 <= e.keyCode && e.keyCode <= 57) || (187 <= e.keyCode && e.keyCode <= 190)) {
			console.log(parseFloat(minInput.value));
			if (parseFloat(minInput.value) !== NaN) {
				thisptr.valMin = parseFloat(minInput.value);
				thisptr.drawGraph();
			}
		}
	}}(this));
	maxInput.addEventListener('keyup', function (thisptr) { return function(e) {
		if (e.keyCode === 8 || (46 <= e.keyCode && e.keyCode <= 57) || (187 <= e.keyCode && e.keyCode <= 190)) {
			if (parseFloat(maxInput.value) !== NaN) {
				thisptr.valMax = parseFloat(maxInput.value);
				thisptr.drawGraph();
			}
		}
	}}(this));
	
	
	this.wrapper.appendChild(minmaxframe);
	minmaxframe.appendChild(minText);
	minmaxframe.appendChild(minInput);
	minmaxframe.appendChild(maxText);
	minmaxframe.appendChild(maxInput);
	
	this.allBtnOff = function () {
		this.redbtn.style.backgroundColor = disableColor;
		this.greenbtn.style.backgroundColor = disableColor;
		this.bluebtn.style.backgroundColor = disableColor;
		this.alphabtn.style.backgroundColor = disableColor;
		this.allbtn.style.backgroundColor = disableColor;
	}
	this.graphMode = (1|2|4);
	redbtn.addEventListener('click', function (self) { return function(){
		self.graphMode = 1;
		self.allBtnOff();
		this.style.backgroundColor = "#F00";
	}}(this));
	greenbtn.addEventListener('click', function (self) { return function(){
		self.graphMode = 2;
		self.allBtnOff();
		this.style.backgroundColor = "#0F0";
	}}(this));
	bluebtn.addEventListener('click', function (self) { return function(){
		self.graphMode = 4;
		self.allBtnOff();
		this.style.backgroundColor = "#11F";
	}}(this));
	alphabtn.addEventListener('click', function (self) { return function(){
		self.graphMode = 8;
		self.allBtnOff();
		this.style.backgroundColor = "#000";
	}}(this));
	allbtn.addEventListener('click', function (self) { return function(){
		self.graphMode = (1|2|4);
		self.allBtnOff();
		this.style.backgroundColor = "#FFF";
	}}(this));


	this.changeCallback = null;
	
	var numVals = 256;
	this.numVals     = numVals;
	this.valueRed   = [numVals];
	this.valueGreen = [numVals];
	this.valueBlue  = [numVals];
	this.valueAlpha = [numVals];
	/*this.histRed = [numVals];
	this.histGreen = [numVals];
	this.histBlue = [numVals];*/
	this.defaultValMin = 0;
	this.defaultValMax = 0;
	this.valMin = 0;
	this.valMax = 0;
	this.hist = [numVals];
	for (i = 0; i < numVals; ++i){
		this.valueRed[i] = i/numVals;
		this.valueGreen[i] = 0.8;
		this.valueBlue[i] = 1.0 - i / numVals;
		this.valueAlpha[i] = 1.0;
		this.hist[i] = 0;
	}
	
	var	margin_x = 0,
		margin_y = 0,
		rct = wrapper.getBoundingClientRect(),
		cw = rct.right - rct.left;
		ch = 300;//rct.bottom - rct.top;

	c.style.width  = c.width  = cw;
	c.style.height = c.height = ch;
	var ctx = c.getContext ('2d');
	this.ctx = ctx;

	this.transFunc = function (x) {
		return Math.sqrt(x);
	}
	this.invTransFunc = function (x) {
		return x*x;
	}
	
	
	this.drawGraph = function () {
		// clear
		this.ctx.beginPath();
		this.ctx.clearRect(0,0,cw,ch);
		
		// waku
		this.ctx.beginPath();
		this.ctx.fillStyle = "rgb(60, 60, 60)";
		this.ctx.fillRect(0,0,cw,ch);
		this.ctx.strokeStyle = "#676767";
		for (var x = 0; x <= cw; x += 50){
			this.ctx.moveTo(x,0);
			this.ctx.lineTo(x,ch);
		}
		for (var y = 0; y <= ch; y += 50){
			//console.log(Math.sqrt(y/ch)*ch);
			var yv = this.transFunc(y/ch);
			this.ctx.moveTo(0 ,(1.0 - yv)*ch);
			this.ctx.lineTo(cw,(1.0 - yv)*ch);
		}
		ctx.font = "16px";
		ctx.fillStyle = "#8e8e8e";
		ctx.fillText("1.0", 5, 20);
		ctx.fillText("0.0", 5, ch-30);
		ctx.fillText("min", 20, ch-5);
		ctx.fillText("max", cw - 40, ch-5);
		this.ctx.stroke();

		ctx.globalCompositeOperation = 'lighter';
		/*this.drawValueLine(this.values, '#AAA');
		this.drawValueLine(this.histRed, '#F00');
		this.drawValueLine(this.histGreen, '#0F0');
		this.drawValueLine(this.histBlue, '#00F');*/
		this.drawValueLine(this.valueRed, '#F00');
		this.drawValueLine(this.valueGreen, '#0F0');
		this.drawValueLine(this.valueBlue, '#11F');

		this.drawColorBar(this.valueRed, this.valueGreen, this.valueBlue, this.valueAlpha);
		ctx.globalCompositeOperation = 'source-over';
		this.drawValueLine(this.valueAlpha, '#000');
		this.drawValueLine(this.hist, 'rgb(154, 79, 40)', true);
		
		this.minInput.value = this.valMin;
		this.maxInput.value = this.valMax;

	}
	this.drawValueLine = function (vals, col, scaling) {
		// Graph
		this.ctx.lineWidth = 2;
		this.ctx.beginPath();
		this.ctx.strokeStyle = col;
		this.ctx.moveTo(0,ch);
		
		
		var defDiff = this.defaultValMax - this.defaultValMin,
			valDiff = this.valMax - this.valMin,
			gscale = 1.0,
			goffset = 0.0;
		
		if (scaling) {
			goffset = (this.defaultValMin - this.valMin) / defDiff;
			gscale = defDiff / valDiff;
		}
		for (var i = 0; i < numVals; ++i){
			//this.ctx.lineTo(i / numVals *  cw, (1.0 - vals[i]) * ch); // MODE NORMAL
			this.ctx.lineTo((i / numVals  * gscale + goffset) *  cw , ch - this.transFunc(vals[i])*ch); // MODE SQRT
		}
		this.ctx.stroke();
		this.ctx.lineWidth = 1;
	}

	this.drawColorBar = function (red, green, blue, alpha) {
		var colorData = this.colorctx.getImageData(0,0, this.cw, this.colorbarHeight),
			i,j, iw = this.cw * 4;

		for (j = 0; j < this.colorbarHeight; j++) {
			for (i = 0; i < this.cw; i++) {
			var d = i * 4,
				s = Math.floor(i * this.numVals / this.cw),
				backgrd = (1.0 - alpha[s]) * ((((i/10)|0)%2) ^ (((j/10)|0)%2)) * 155;
			colorData.data[j * iw + d + 0] = ((red[s]  * 255 * alpha[s] + backgrd)| 0);
			colorData.data[j * iw + d + 1] = ((green[s]* 255 * alpha[s] + backgrd)| 0);
			colorData.data[j * iw + d + 2] = ((blue[s] * 255 * alpha[s] + backgrd)| 0);
			colorData.data[j * iw + d + 3] = 255;
			}
		}
		this.colorctx.putImageData(colorData, 0, 0);
	};
	
	this.oldx = 0;
	this.oldy = 0;
	
	this.mouseMoveFunc = function(thisptr){ return function(e){
		var x = e.clientX - thisptr.wrapper.getBoundingClientRect().left,
			y = e.clientY - thisptr.wrapper.getBoundingClientRect().top;
		x = Math.min(x, thisptr.cw);
		x = Math.min(cw, Math.max(0.0, x));
		y = Math.min(ch, Math.max(0.0, y));
		if (thisptr.mspress){
			var minx = Math.min(x, thisptr.oldx),
				maxx = Math.max(x, thisptr.oldx),
				miny = thisptr.oldy,//Math.min(y, thisptr.oldy),
				maxy = y,//Math.max(y, thisptr.oldy),
				i;
			if (x < thisptr.oldx) {
				i = miny;
				miny = maxy;
				maxy = i;
			}
			
			for (i = minx+1; i <= maxx; ++i) {
				var dy = (maxy - miny) * (i - minx+1)/(maxx - minx+1) + miny;
				//thisptr.setMaskedGraphValue(Math.floor(i/cw*numVals), 1.0 - dy/ch); // MODE NORMAL
				thisptr.setMaskedGraphValue(Math.floor(i/cw*numVals), thisptr.invTransFunc((ch-dy)/ch)); // MODE SQRT
			}
			
			thisptr.drawGraph();
			if (thisptr.changeCallback){
				thisptr.changeCallback();
			}
		}
		thisptr.oldx = x;
		thisptr.oldy = y;
	}}(this);

	c.addEventListener('mousedown', function(thisptr){ return function(e){
		thisptr.oldx = e.clientX - thisptr.wrapper.getBoundingClientRect().left;
		thisptr.oldy = e.clientY - thisptr.wrapper.getBoundingClientRect().top;
		thisptr.mspress = true;
		document.addEventListener('mousemove', thisptr.mouseMoveFunc);
	}}(this));
	document.body.addEventListener('mouseup', function(thisptr){ return function(e){
		thisptr.mspress = false;
		document.removeEventListener('mousemove', thisptr.mouseMoveFunc);
	}}(this));
	
	this.drawGraph();
}

KTransferFunctionClass.prototype = {
	setGraphValue : function(n, val_r, val_g, val_b, val_a){
		if (n < 0 || this.numVals <= n)
			return;

		this.valueRed[n]   = val_r;
		this.valueGreen[n] = val_g;
		this.valueBlue[n]  = val_b;
		this.valueAlpha[n] = val_a;

	},
	
	setMaskedGraphValue : function(n, val){
		if (n < 0 || this.numVals <= n)
			return;
		val = Math.max(0.0, Math.min(1.0, val));
		if (this.graphMode & 1) {
			this.valueRed[n] = val;
		}
		if (this.graphMode & 2) {
			this.valueGreen[n] = val;
		}
		if (this.graphMode & 4) {
			this.valueBlue[n] = val;
		}
		if (this.graphMode & 8) {
			this.valueAlpha[n] = val;
		}
	},
	getGraphValueRed : function () {
		return this.valueRed;
	},
	getGraphValueGreen : function () {
		return this.valueGreen;
	},
	getGraphValueBlue : function () {
		return this.valueBlue;
	},
	getGraphValueAlpha : function () {
		return this.valueAlpha;
	},
	getMinValue : function () {
		return this.valMin;
	},
	getMaxValue : function () {
		return this.valMax;
	},
	setMinValue : function (val) {
		this.valMin = val;
	},
	setMaxValue : function (val) {
		this.valMax =val;
	},

	getNumValues : function () {
		return this.numVals;
	},
	setAnalyzeResult : function (result, component) {
		var i, hist, componentNum;
		if (result && result.histgram) {
			componentNum = (result.type === "vector" ? 3 : 1);
			hist = result.histgram;
			for (i = 0; i < this.numVals; ++i) {
				this.hist[i] = hist[i * componentNum + component];
			}
			
			this.defaultValMin = result.min[component];
			this.defaultValMax = result.max[component];
			this.valMin = this.defaultValMin
			this.valMax = this.defaultValMax;
		}
	}
}

KTransferFunctionClass.Init = function() {
	var lst = document.getElementsByClassName('KTransferFunction');
	for (var i=0; i<lst.length;++i) {
		_kvtools_UIs[lst[i].id] = new KTransferFunctionClass(lst[i]);
	}
}

KTransferFunctionClass.userInit = function(dom) {
	var el = dom.getElementsByClassName('KTransferFunction');
	for (var k = 0; k < el.length; ++k) {
		_kvtools_UIs[el[k].id] = new KTransferFunctionClass(el[k]);
	}
}


//------------------------------------------------------

KSliderClass = function(wrapper){
	this.wrapper = wrapper;
	var c = document.createElement('canvas');
	this.wrapper.appendChild(c);
	this.changeColorCallback = null;
	this.ctx = c.getContext('2d');
	
	this.cw = 200;
	this.ch = 24;
	c.style.width  = c.width  = this.cw;
	c.style.height = c.height = this.ch;
	
	this.value = 0.0;
	this.maxValue = 1.0;
	this.minValue = 0.0;
	
	var inputValue = document.createElement('input');
	this.wrapper.setAttribute('style','position:relative;');
	inputValue.setAttribute('style','position:absolute;right:0px;bottom:0px;width:100px');
	inputValue.classList.add('KColorPickerInput');
	this.wrapper.appendChild(inputValue);
	this.inputValue = inputValue;
	
	inputValue.value = this.value;
	
	this.roundRect = function (x, y, w, h, r) {
		this.ctx.beginPath();
		this.ctx.clearRect(x, y, w, h);
		this.ctx.lineWidth = 1;
		this.ctx.moveTo(x, y+r);
		this.ctx.arc(x+r,   y+w-r, r, Math.PI, Math.PI/2, 1);
		this.ctx.arc(x+h-r, y+w-r, r, Math.PI/2, 0, 1);
		this.ctx.arc(x+h-r, y+r,   r, 0, Math.PI*3/2, 1);
		this.ctx.arc(x+r,   y+r,   r, Math.PI*3/2, Math.PI, 1);
		this.ctx.closePath();
		this.ctx.strokeStyle = "#fafafa";
		this.ctx.fillStyle = '#262626';
		this.ctx.fill();
		this.ctx.stroke();
	}
	this.drawValue = function () {
		this.ctx.beginPath();
		this.ctx.strokeStyle = "#fafafa";
		this.ctx.fillStyle = 'rgb(206,79,30)';
		this.ctx.arc((this.value - this.minValue) / (this.maxValue - this.minValue) * (this.cw - 20) + 10, 12, 10, 0, Math.PI*2, true);
		this.ctx.fill();
		this.ctx.stroke();
	}
	this.drawGraph = function () {
		this.inputValue.value = this.value.toFixed(5);
		this.roundRect(0, 2, 20, 200, 10);
		this.drawValue();
	}
	
	this.mspress = false;
	this.mouseMoveFunc = function(self){ return function(e){
		if (self.mspress){
			var x = e.clientX - self.wrapper.getBoundingClientRect().left;
			var y = e.clientY - self.wrapper.getBoundingClientRect().top;
			
			self.value = ((x - 10) / (self.cw - 20)) * (self.maxValue - self.minValue) + self.minValue;
			self.value = Math.max(self.minValue, Math.min(self.value, self.maxValue));
			
			self.drawGraph();

			if (self.changeCallback){
				self.changeCallback(self.value);
			}
		}
	}}(this);

	c.addEventListener('mousedown', function(self){ return function(e){
		self.mspress = true;
		document.addEventListener('mousemove', self.mouseMoveFunc);
	}}(this));
	document.body.addEventListener('mouseup', function(self){ return function(e){
		self.mspress = false;
		document.removeEventListener('mousemove', self.mouseMoveFunc);
	}}(this));
	
	this.drawGraph();
}

KSliderClass.prototype = {
	setMaxValue : function (val) {
		this.maxValue = val;
		this.value = Math.max(this.minValue, Math.min(this.value, this.maxValue));
		this.drawGraph();
	},
	setMinValue : function (val) {
		this.minValue = val;
		this.value = Math.max(this.minValue, Math.min(this.value, this.maxValue));
		this.drawGraph();
	},
	setValue : function (val) {
		this.value = val;
		this.value = Math.max(this.minValue, Math.min(this.value, this.maxValue));
		this.drawGraph();
	},
	getValue : function () {
		return this.value;
	},
	ChangeColorCallback : function(func){
		this.changeColorCallback = func;
	}
}
KSliderClass.Init = function(){
	var lst = document.getElementsByClassName('KSlider');
	for (var i=0; i<lst.length;++i) {
		_kvtools_UIs[lst[i].id] = new KSliderClass(lst[i]);
	}
}

KSliderClass.userInit = function(dom){
	var el = dom.getElementsByClassName('KSlider');
	for (var k = 0; k < el.length; ++k) {
		_kvtools_UIs[el[k].id] = new KSliderClass(el[k]);
	}
}

//------------------------------------------------------

KTimelineClass = function(wrapper){
	this.wrapper = wrapper;
	var c = document.createElement('canvas');
	this.c = c;
	this.wrapper.appendChild(c);
	this.changeColorCallback = null;
	this.ctx = c.getContext('2d');
	
	this.timeVal = 0;
		
	this.value = 0.0;
	this.maxValue = 1.0;
	this.minValue = 0.0;
	
	this.changeTimeCallback = null;
	this.deleteCallback = null;
	this.selectCallback = null;
	
	this.graphXOffset = 0.0;
	this.timeScale = 240.0;
	var timeGrid = 60.0;
	var startYPos = 60;
	
	this.tlData = null;
	this.setTimelineData = function (data) {
		this.tlData = data;
	};
	this.drawTimelineData = function () {
		if (!this.tlData || this.tlData.length === 0)
			return;
		ctx = this.ctx;
		ctx.beginPath();
		ctx.lineWidth = 5;
		ctx.strokeStyle = "#81d60b";
		ctx.fillStyle = "#81d60b";
		var name, i, n, t, sy;
		sy = startYPos;
		for (name in this.tlData) {
			n = this.tlData[name].length;
			if (n === 0) {
				continue;
			}
			t = this.tlData[name][0].time * this.timeScale - this.graphXOffset;
			ctx.fillText(name, 5, sy-5);
			ctx.moveTo(t, sy);
			for (i = 1; i < n; i += 1) {
				t = this.tlData[name][i].time * this.timeScale - this.graphXOffset;
				ctx.lineTo(t, sy);
			}
			sy += 50;
		}
		
		sy = startYPos;
		for (name in this.tlData) {
			n = this.tlData[name].length;
			if (n === 0) {
				continue;
			}
			for (i = 0; i < n; i += 1) {
				t = this.tlData[name][i].time * this.timeScale - this.graphXOffset;
				ctx.moveTo(t, sy);
				ctx.arc(t, sy, 3, 0, 2.0 * Math.PI);
			}
			sy += 50;
		}
		
		ctx.stroke();
		ctx.lineWidth = 1;
	}
	
	this.drawBaseGraph = function () {
		var x,
			y,
			i,
			cw = this.cw,
			ch = this.ch,
			ctx = this.ctx;
		// clear
		ctx.beginPath();
		ctx.clearRect(0,0,cw,ch);
		
		// waku
		ctx.beginPath();
		ctx.fillStyle = "#262626";
		ctx.fillRect(0,0,cw,ch);
		ctx.strokeStyle = "#FFF";

		var sx = this.graphXOffset,
			ex = this.cw + this.graphXOffset,
			gx;

		for (x = sx; x <= ex; x += timeGrid){
			gx = Math.floor(x / timeGrid) * timeGrid;
			ctx.moveTo(gx - sx, 30);
			ctx.lineTo(gx - sx,ch);
		}
		for (y = 30; y <= ch; y += 50){
			ctx.moveTo(0 ,y);
			ctx.lineTo(cw,y);
		}
		ctx.font = "16px";
		ctx.fillStyle = "#fff";
		for (i = sx; i < ex; i += this.timeScale) {
			gx = Math.floor(i / this.timeScale) * this.timeScale;
			ctx.fillText((gx/this.timeScale).toFixed(1) + "[sec]", gx - sx, 28);
		}
		ctx.stroke();
	};
	this.drawBar = function () {
		ctx = this.ctx;
		ctx.beginPath();
		ctx.strokeStyle = "#eb8022";
		var tx = this.timeVal * this.timeScale - this.graphXOffset,
			sx = this.graphXOffset;
		ctx.moveTo(tx, 20);
		ctx.lineTo(tx - 10, 5);
		ctx.lineTo(tx + 10, 5);
		ctx.lineTo(tx, 20);
		ctx.lineTo(tx, this.ch);
		ctx.fillStyle = "#eb8022";
		ctx.fillText((tx/this.timeScale).toFixed(3) + "[sec]", tx - sx + 10, 15);
		ctx.stroke();
	};
	this.drawGraph = function () {
		this.drawBaseGraph();
		this.drawBar();
		this.drawTimelineData();
	};
	
	this.resize = function () {
		var rect = this.wrapper.getBoundingClientRect(),
			c = this.c;
		this.cw = rect.right - rect.left;
		this.ch = rect.bottom - rect.top;
		c.style.width  = c.width  = this.cw;
		c.style.height = c.height = this.ch;
	};
	this.resize();
	
	this.mspress = false;
	this.mouseMoveFunc = function(self){ return function(e){
		if (self.mspress){
			var x = e.clientX - self.wrapper.getBoundingClientRect().left,
				y = e.clientY - self.wrapper.getBoundingClientRect().top,
				rateX = x / (self.wrapper.getBoundingClientRect().right - self.wrapper.getBoundingClientRect().left);

			// scroll
			if (rateX > 0.9) {
				self.graphXOffset += 20 * (rateX - 0.9)/0.1;
			} else if (rateX < 0.1) {
				self.graphXOffset -= 20 * (1.0 - rateX/0.1);
			}
			self.graphXOffset = Math.max(0.0, self.graphXOffset);

			// Time val
			self.timeVal = (x + self.graphXOffset) / self.timeScale;
			self.timeVal = Math.max(0.0, self.timeVal);
			
			// Draw
			self.drawGraph();

			if (self.changeTimeCallback){
				self.changeTimeCallback(self.timeVal);
			}
		}
	}}(this);
	
	
	function resizeFunc(self) {
		return function () {
			self.resize();
			self.drawGraph();
		};
	}
	window.addEventListener('resize', resizeFunc(this), false);

	c.addEventListener('mousedown', function(self){ return function(e){
		var x = e.clientX - self.wrapper.getBoundingClientRect().left,
			y = e.clientY - self.wrapper.getBoundingClientRect().top,
			i,n, radiusSize = 5;
		if (e.button == 2) {
			console.log(x,y);
			
			// delete keyframe
			var sy = startYPos;
			for (name in self.tlData) {
				n = self.tlData[name].length;
				if (n === 0) {
					continue;
				}
				for (i = 0; i < n; i += 1) {
					t = self.tlData[name][i].time * self.timeScale - self.graphXOffset;
					if (t - radiusSize < x && x < t + radiusSize
					&&  sy - radiusSize < y && y < sy + radiusSize) {
						console.log('DEL',name,i, self.tlData[name][i].time);
						if (self.deleteCallback){
							self.deleteCallback(name, self.tlData[name][i].time);
						}
							
						return;
					}
				}
				sy += 50;
			}
		} else {
			// select keyframe
			var sy = startYPos;
			for (name in self.tlData) {
				n = self.tlData[name].length;
				if (n === 0) {
					continue;
				}
				for (i = 0; i < n; i += 1) {
					t = self.tlData[name][i].time * self.timeScale - self.graphXOffset;
					if (t - radiusSize < x && x < t + radiusSize
					&&  sy - radiusSize < y && y < sy + radiusSize) {
						if (self.selectCallback){
							self.selectCallback(name, parseFloat(self.tlData[name][i].time));
						}
						self.timeVal = parseFloat(self.tlData[name][i].time);
						self.drawGraph();
						return;
					}
				}
				sy += 50;
			}
			
			self.mspress = true;
			self.mouseMoveFunc(e);
			document.addEventListener('mousemove', self.mouseMoveFunc);
		}
	}}(this));
	document.body.addEventListener('mouseup', function(self){ return function(e){
		self.mspress = false;
		document.removeEventListener('mousemove', self.mouseMoveFunc);
	}}(this));

	this.drawGraph();
}

KTimelineClass.prototype = {
	getTimeValue : function () {
		return this.timeVal;
	},
	/*setMaxValue : function (val) {
		this.maxValue = val;
		this.value = Math.max(this.minValue, Math.min(this.value, this.maxValue));
		this.drawGraph();
	},
	setMinValue : function (val) {
		this.minValue = val;
		this.value = Math.max(this.minValue, Math.min(this.value, this.maxValue));
		this.drawGraph();
	},
	setValue : function (val) {
		this.value = val;
		this.value = Math.max(this.minValue, Math.min(this.value, this.maxValue));
		this.drawGraph();
	},
	getValue : function () {
		return this.value;
	},*/
	ChangeTimeCallback : function(func){
		this.changeTimeCallback = func;
	},
	DeleteCallback : function(func){
		this.deleteCallback = func;
	},
	SelectCallback : function(func){
		this.selectCallback = func;
	}

}
KTimelineClass.Init = function(){
	var lst = document.getElementsByClassName('KTimeline');
	for (var i=0; i<lst.length;++i) {
		_kvtools_UIs[lst[i].id] = new KTimelineClass(lst[i]);
	}
}

KTimelineClass.userInit = function(dom){
	var el = dom.getElementsByClassName('KTimeline');
	for (var k = 0; k < el.length; ++k) {
		_kvtools_UIs[el[k].id] = new KTimelineClass(el[k]);
	}
}
//------------------------------------------------------

function kvtoolsUI_init() {
	KButtonClass.Init();
	KListClass.Init();
	KWindowClass.Init();
	KCaptionClass.Init();
	KShaderListClass.Init();
	KDropdownListClass.Init();
	KColorPickerClass.Init();
	KTransferFunctionClass.Init();
	KSliderClass.Init();
	KTimelineClass.Init();
}

function kvtoolsUI_update(dom) {
	KButtonClass.userInit(dom);
	KListClass.userInit(dom);
	KWindowClass.userInit(dom);
	KCaptionClass.userInit(dom);
	KShaderListClass.userInit(dom);
	KDropdownListClass.userInit(dom);
	KColorPickerClass.userInit(dom);
	KTransferFunctionClass.userInit(dom);
	KSliderClass.userInit(dom);
	KTimelineClass.userInit(dom);
}
//------------------------------------------------------
