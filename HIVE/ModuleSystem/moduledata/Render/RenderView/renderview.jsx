import React from 'react'
import ReactDOM from 'react-dom'

//import nanomsg from 'nanomsg'
//import buffercopy from 'buffercopy'

const minWidth = 256;
const minHeight = 100;
const footerHeight = 50;

function progressiveMin(val) {
	return parseInt(val / 16.0, 10);
}

class RenderView extends React.Component {
	constructor(props) {
        super(props);
        console.log('RenderView Constructor:', props);

        this.node = props.node;
        this.action = props.action;
        this.store = props.store;

        this.varname = this.node.varname;
        this.colorbarHeight = 15;
		this.colorbarAreaHeight = 40;

		this.state = {
			width : minWidth,
			height : minHeight,
			colorbar : null
		};
		
        // Mouse
        this.mouseState = 0;
        this.oldmx = 0;
        this.oldmy = 0; 

		// View
		this.componentDidUpdate = this.componentDidUpdate.bind(this);
		this.imageRecieved = this.imageRecieved.bind(this);
		this.imageRecieveWrap = this.imageRecieveWrap.bind(this);
		this.onPanelSizeChanged = this.onPanelSizeChanged.bind(this);
		this.getInputValue = this.getInputValue.bind(this);
		this.onEnterCameraButton = this.onEnterCameraButton.bind(this);
		this.onLeaveCameraButton = this.onLeaveCameraButton.bind(this);
		this.onClickCameraButton = this.onClickCameraButton.bind(this);
		this.onClickCameraRegisterButton = this.onClickCameraRegisterButton.bind(this);
		this.reRender = this.reRender.bind(this);

		this.updatePreset = this.updatePreset.bind(this);
		
		this.reRender(true);
	}

    progressiveUpdate(param) {
        let w = param.width;
        let h = param.height;
        const varname = this.node.varname;
        const ssize = this.getInputValue("screensize");
		if (this.mouseState > 0) { return; }
        if (w < ssize[0] || h < ssize[1]) {
            w = ssize[0];
            h = ssize[1];
            //console.log('PROGRESSIVE:', w, h);
			setTimeout(() => {
				this.action.changeNodeInput({
					varname : varname,
					input : {
						"rendersize" : [w,h]
					}
				});
			},0);
        }
    }

	imageRecieved(err, param, data) {
		var buffer;
        const varname = this.node.varname;
		if (param.varname !== varname) {
			return;
		}
		if (param.type === 'jpg') {
			buffer = new Blob([data]);
		} else if (param.type == 'png') {
			buffer = new Blob([data]);
		} else {
			buffer = data;
		}
		let colorbar = {
			rgba : param.colorbar.rgba ? JSON.parse(param.colorbar.rgba) : null,
			min :  Number(param.colorbar.min),
			max :  Number(param.colorbar.max),
			composite : (param.colorbar.composite == "true"),
			color : param.colorbar.color ? JSON.parse(param.colorbar.color) : null 
		};
			
        this.setState({
			image : buffer,
			colorbar : colorbar
		});
		this.state.colorbar = colorbar; 
		this.drawColorBar();

		
        // progressive update
        this.progressiveUpdate(param);
	}

    hasIPCAddress() {
		return this.getInputValue('ipcmode');
    }
	
	hasColorBar() {
		return this.state.colorbar && this.state.colorbar.rgba;
	}

    closeForIPCImageTransfer(){
        if (this.sc === undefined) {
        } else {
            this.sc.close();
            this.sc = undefined;
        }

    }
    readyForIPCImageTransfer(){
       	// Electron only
        if (this.sc === undefined && window && window.process && window.process.type) {
            var nano = process.mainModule.require('nanomsg');
            var buffercopy = process.mainModule.require('buffercopy');
            var sc = nano.socket('pair');
            this.sc = sc;
            var meta = process.mainModule.require(process.mainModule.require("path").resolve('./lib/metabinary')); // path from index.html
            this.meta = meta;
            var ipcAddress = 'ipc:///tmp/HIVE_IPC_' + this.varname;
            var ret = sc.bind(ipcAddress);
            console.log('IPC bind = ', ret, ipcAddress);

	        this.sc.on('data', (data) => {
	            //console.log('ONDATAA!!!!', this.meta);
	            if (meta === undefined) { // ????? Why undefined???
	                meta = meta
	                return;
	            }
	            if (!this.meta.loadMetaBinary(data, (meta, data) => {
	                var w, h,
	                    param = meta.param;
	                if (param.type === 'jpg') {
	                    // resultElement is img.
	                    var resultElement = document.getElementById(this.getCanvasName('img'));
	                    resultElement.src = URL.createObjectURL(new Blob([data], {type: "image/jpeg"}));

	                } else if (param.type === 'png') {
						// resultElement is img.
						var resultElement = document.getElementById(this.getCanvasName('img'));
						resultElement.src = URL.createObjectURL(new Blob([data], {type: "image/png"}));
					} else if (param.type === 'raw'){
	                    //console.log('UPDATE CANVAS!!!');

	                    // resultElement is canvas.
	                    var resultElement = document.getElementById(this.getCanvasName('canvas'));
                        if (resultElement) { // why undefined, sometime??
                            resultElement.setAttribute('width', param.width),
                            resultElement.setAttribute('height', param.height);
                            var context = resultElement.getContext('2d');
                            var imageData = context.createImageData(param.width, param.height);
                            buffercopy.buffercopy(data, imageData.data);
                            context.putImageData(imageData, 0, 0);

                            this.progressiveUpdate(param);
                        }
	                }

	            })) {
	                console.error('Not metabin foramt');
	            };
	        });
		}
    }


	componentDidUpdate() {
		// キャンバスの更新.
		if (this.state && this.state.image) {
			if (this.hasIPCAddress()) {
            /*    let canvas = document.getElementById(this.getCanvasName());
				//let canvas = ReactDOM.findDOMNode(this.refs.canvas);
				let context = canvas.getContext('2d');
				let width = this.state.param.width;
				let height = this.state.param.height;
				canvas.setAttribute('width', width);
				canvas.setAttribute('height', height);
                //console.error('[inst]', nanomsg, buffercopy);

				if (width * height * 4 == this.state.image.length) {
					let imageData = context.createImageData(width, height);
					buffercopy.buffercopy(this.state.image, imageData.data);
					context.putImageData(imageData, 0, 0);
				} else {
					console.error("image size err:", width, height);
				}*/
			} else {
  		//		let imgElem = ReactDOM.findDOMNode(this.refs.renderviewimage);
		//		imgElem.src = URL.createObjectURL(this.state.image, {type: "image/jpeg"});
                let imgElem = document.getElementById(this.getCanvasName('img'));
                imgElem.src = URL.createObjectURL(this.state.image, {type: "image/png"})
                //console.log(imgElem);
			}
		}
	}
	
	reRender(useProgressive) {
		let screensize = this.getInputValue("screensize");
		setTimeout( () => {
			if (useProgressive) {
				this.action.changeNodeInput({
					varname : this.props.node.varname,
					input : {
						"rendersize" : [progressiveMin(screensize[0]), progressiveMin(screensize[1])]
					}
				});
			} else {
				this.action.changeNodeInput({
					varname : this.props.node.varname,
					input : {
						"rendersize" : [(screensize[0]), (screensize[1])]
					}
				});
			}
		}, 0);
	}

	getInputValue(key) {
		for (let i = 0; i < this.node.input.length; i = i + 1) {
			if (this.node.input[i].name === key) {
				return this.node.input[i].value;
			}
		}
	}

    viewRot(rotx, roty, rotz) {
		let target = JSON.parse(JSON.stringify(this.getInputValue("target")));
		let position = JSON.parse(JSON.stringify(this.getInputValue("position")));
		let eyedir = subtract(position, target);
		let v = vec4(eyedir[0], eyedir[1], eyedir[2], 0.0);
		let az = normalize(subtract(position, target));
		let ax = normalize(cross(vec3(0, 1, 0), az));
		let ay = normalize(cross(az, ax));
		let rx = rotate(rotx, ax);
		let ry = rotate(roty, ay);
        let ssize = JSON.parse(JSON.stringify(this.getInputValue("screensize")));
        let rw = progressiveMin(ssize[0]);
        let rh = progressiveMin(ssize[1]);

		v = vec4(dot(ry[0], v), dot(ry[1], v), dot(ry[2], v), 0.0);
		v = vec3(dot(rx[0], v), dot(rx[1], v), dot(rx[2], v));
		let pos = add(target, v);

		if (Math.abs(dot(normalize(subtract(target, pos)), vec3(0, 1, 0))) < 0.99) {
			const varname = this.node.varname;
			this.action.changeNodeInput({
				varname : varname,
				input : {
					"position" : pos,
					"target" : target,
                    "rendersize" : [rw, rh]
				}
			});
		}
    }


	updatePreset() {
		let layer = Number(this.refs.presetSelect.value);
		let presets = this.getInputValue("presets");
		if (!presets) {
			presets = {};
		}
		if (!presets.hasOwnProperty(layer)) {
			presets[layer] = {};
		}
		let preset = presets[layer];
		preset.position = JSON.parse(JSON.stringify(this.getInputValue("position")));
		preset.target = JSON.parse(JSON.stringify(this.getInputValue("target")));
		preset.up = JSON.parse(JSON.stringify(this.getInputValue("up")));
		preset.fov = this.getInputValue("fov");

		this.action.changeNodeInput({
			varname : this.node.varname,
			input : {
				"presets" : presets
			}
		});
	}

    viewTrans(tx, ty, tz) {
		let target = JSON.parse(JSON.stringify(this.getInputValue("target")));
		let position = JSON.parse(JSON.stringify(this.getInputValue("position")));
		let mv = vec3(-tx, ty, 0.0);
		let az = normalize(subtract(position, target));
		let ax = normalize(cross(vec3(0, 1, 0), az));
		let ay = normalize(cross(az, ax));
		let mx = transpose(mat3(ax, ay, az));
		let mm = vec3(vec3(dot(mv, mx[0]), dot(mv, mx[1]), dot(mv, mx[2])));
		let pos = add(add(position, scale(-tx, ax)), scale(ty, ay));
		let tar = add(add(target,   scale(-tx, ax)), scale(ty, ay));
        let ssize = JSON.parse(JSON.stringify(this.getInputValue("screensize")));
        let rw = progressiveMin(ssize[0]);
        let rh = progressiveMin(ssize[1]);

		const varname = this.node.varname;
		this.action.changeNodeInput({
			varname : varname,
			input : {
				"position" : pos,
				"target" : tar,
                "rendersize" : [rw, rh]
			}
		});
    }
	
    viewZoom(zoom) {
		let target = JSON.parse(JSON.stringify(this.getInputValue("target")));
		let position = JSON.parse(JSON.stringify(this.getInputValue("position")));
		let v = subtract(position, target);
        let ssize = JSON.parse(JSON.stringify(this.getInputValue("screensize")));
        let rw = progressiveMin(ssize[0]);
        let rh = progressiveMin(ssize[1]);

		let r = 1.0 - (zoom / 1000.0);
		v = scale(r, v);
		position = add(target, v);
		const varname = this.node.varname;
		this.action.changeNodeInput({
			varname : varname,
			input : {
				"position" : position,
                "rendersize" : [rw, rh]
			}
		});
    }

    onImgMouseDown(event) {
        event.preventDefault();
        this.mouseState = 1;
        this.oldmx = event.clientX;
        this.oldmy = event.clientY;
    }
    onImgMouseMove(event) {
        if (this.mouseState > 0) {
            event.preventDefault();

            const dx = event.clientX - this.oldmx;
            const dy = event.clientY - this.oldmy;
            //console.log('mmove', dx, dy);

			if (event.button === 0) {
            	this.viewRot(-dy * 0.5, -dx * 0.5, 0.0);
			} else if (event.button === 1) {
				this.viewTrans(dx, dy);
			} else if (event.button === 2) {
				this.viewZoom(dx + dy);
			}

            this.oldmx = event.clientX;
            this.oldmy = event.clientY;
        }
    }
    onImgMouseUp(event) {
		if (this.mouseState !== 0) {
			this.reRender(false);
		}
        this.mouseState = 0;
    }
	
	imageRecieveWrap(err, data) {
		if (data.varname !== this.node.varname) {
			return;
		}
		
		if (this.hasIPCAddress()) {
			this.readyForIPCImageTransfer();
		}
		
		let screensize = this.getInputValue("screensize");
		let panelsize = data.panel.size;
		if (screensize[0] !== panelsize[0] || (screensize[1] + footerHeight) !== panelsize[1]) {
			let panel = JSON.parse(JSON.stringify(data.panel));
			panel.size[0] = Number(screensize[0]);
			panel.size[1] = Number(screensize[1] + footerHeight);
			setTimeout( ()=> {
				this.action.changeNode({
					varname : this.node.varname,
					panel : panel
				});
			}, 0);
		}
	}

	onPanelSizeChanged(err, data) {
		if (data.varname !== this.node.varname) {
			return;
		}
		if (this.props.node.panel.visible) {
			let width = Math.max(this.props.node.panel.size[0], minWidth);
			let height = Math.max(this.props.node.panel.size[1] - footerHeight, minHeight)
			if (Number(this.state.width) !== width || Number(this.state.height) !== height) {
				this.setState({
					width : width,
					height : height
				});
				
				setTimeout( ()=> {
					let width = Math.max(this.props.node.panel.size[0], minWidth);
					let height = Math.max(this.props.node.panel.size[1] - footerHeight, minHeight)
					
					this.action.changeNodeInput({
						varname : this.props.node.varname,
						input : {
							"screensize" : [width, height],
							"rendersize" : [progressiveMin(width), progressiveMin(height)]
						}
					});
				}, 0);
			}
		}
	}
	
	onPresetChange(ev) {
		let number = Number(ev.target.value);
		if (number === null || number === undefined) { console.error("invalid camera layer"); return; }
		const ssize = this.getInputValue("screensize");
		let presets = this.getInputValue("presets");
		if (!presets) {
			return;
		}
		presets.currentPreset =  number;
		let rw = progressiveMin(ssize[0]);
		let rh = progressiveMin(ssize[1]);
		if (presets.hasOwnProperty(number)) {
			let preset = presets[number];
			if (preset.hasOwnProperty("position") &&
				preset.hasOwnProperty("target") &&
				preset.hasOwnProperty("up") &&
				preset.hasOwnProperty("fov")) {
				this.action.changeNodeInput({
					varname : this.props.node.varname,
					input : {
						position : preset.position,
						target : preset.target,
						up : preset.up,
						fov : preset.fov,
						presets : presets
					}
				})
			}
		} else {
			this.action.changeNodeInput({
				varname : this.props.node.varname,
				input : {
					"position" : [0, 0, 300],
					"target" : [0, 0, 0],
					"up" : [0, 1, 0],
					"fov" : 60,
					"rendersize" : [rw, rh],
					"presets" : presets
				}
			});
		}
	}

	componentDidMount() {
        var colorbar    = ReactDOM.findDOMNode(this.refs.colorbar);
        colorbar.width  = 256;
        colorbar.height = 40;
        this.colorctx     = colorbar.getContext ('2d');
		
        let imgElem = document.getElementById(this.getCanvasName('img'));
        imgElem.addEventListener('mousedown', this.onImgMouseDown.bind(this), true);

        let canElem = document.getElementById(this.getCanvasName('canvas'));
        canElem.addEventListener('mousedown', this.onImgMouseDown.bind(this), true);

		// canvas.tabIndex = 1000;
		window.addEventListener('mouseup', this.onImgMouseUp.bind(this));
		window.addEventListener('mousemove', this.onImgMouseMove.bind(this), true);

        const NODE_INPUT_CHANGED = "node_input_changed"
        this.store.on(NODE_INPUT_CHANGED, this.imageRecieveWrap);

        const Store_IMAGE_RECIEVED = "image_revieved";
		this.store.on(Store_IMAGE_RECIEVED, this.imageRecieved);

		this.store.on("panel_size_changed", this.onPanelSizeChanged);

		let presets = this.getInputValue("presets");
		if (presets && presets.hasOwnProperty("currentPreset")) {
			this.refs.presetSelect.selectedIndex = presets.currentPreset - 1;
		}
	}

	componentWillUnmount() {
        let imgElem = document.getElementById(this.getCanvasName('img'));
        imgElem.removeEventListener('mousedown', this.onImgMouseDown.bind(this), true);
        let canElem = document.getElementById(this.getCanvasName('canvas'));
        canElem.addEventListener('mousedown', this.onImgMouseDown.bind(this), true);

		window.removeEventListener('mouseup', this.onImgMouseUp.bind(this));
		window.removeEventListener('mousemove', this.onImgMouseMove.bind(this));
		const Store_IMAGE_RECIEVED = "image_revieved";
		this.store.off(Store_IMAGE_RECIEVED, this.imageRecieved);
		
        const NODE_INPUT_CHANGED = "node_input_changed"
		this.store.off(NODE_INPUT_CHANGED, this.imageRecieveWrap);
		
		this.store.off("panel_size_changed", this.onPanelSizeChanged);
		this.closeForIPCImageTransfer();
	}

    styles() {
		return {
			bounds : {
				minWidth : String(minWidth) + "px",
				minHeight : String(minHeight) + "px"
			},
			canvas : {
				postion : "relative",
				left : "0px",
				top : "0px",
                width: String(this.canvasSize.bind(this)()[0]) + "px",
                height: String(this.canvasSize.bind(this)()[1]) + "px",
                transform : "scale(1.0,-1.0)",
                display: (this.hasIPCAddress() ? "block" : "none")
			},
			image : {
				postion : "relative",
				left : "0px",
				top : "0px",
                width: String(this.canvasSize.bind(this)()[0]) + "px",
                height: String(this.canvasSize.bind(this)()[1]) + "px",
                display: (this.hasIPCAddress() ? "none" : "block")
			},
			cameraButtonArea : {
				height : "25px",
				backgroundColor : "rgba(67, 67, 67, 0.9)",
			},
			cameraButton : {
				width : "32px",
				height : "20px",
				float : "left",
				backgroundColor : "gray",
				border : "1px solid darkgray",
				borderRadius : "3px",
				margin : "2px",
				textAlign : "center",
				fontSize : "11px"
			},
			registerButton: {
				width : "50px",
				height : "20px",
				backgroundColor : "gray",
				border : "1px solid darkgray",
				borderRadius : "3px",
				marginLeft : "2px",
				textAlign : "center",
				fontSize : "11px",
				position : "absolute"
			},
			presetArea : {
				height : "25px",
				padding : "2px",
				backgroundColor : "rgba(67, 67, 67, 0.9)",
			},
			colorbar : {
				position: "absolute",
				top : (this.state.height - 5) + "px",
				left : "3px",
				display : (this.hasColorBar() ? "block" : "none")
			},
			presetsSelect : {
				width : "100px",
				height : "20px",
				backgroundColor : "white"
			}
		}
	}

    /*onChange(i, event) {
        let text = event.target.value;
        this.setState({text});
        const inputs = JSON.parse(JSON.stringify(this.node.input));
        inputs[5].value[i] = parseFloat(text);
        const varname = this.node.varname;
        this.action.changeNode({
            varname: varname,
            input: inputs
        });
    }*/

	onEnterCameraButton(ev) {
		ev.target.style.backgroundColor = "darkgray";
		ev.target.style.cursor = "pointer";
	}

	onLeaveCameraButton(ev) {
		ev.target.style.backgroundColor = "gray";
		ev.target.style.cursor = "default";
	}

	onClickCameraRegisterButton(ev) {
		this.updatePreset();
	}

	onClickCameraButton(ev) {
		const varname = this.node.varname;
		let pos = this.getInputValue('position');
		let tar = this.getInputValue('target');
		let up = this.getInputValue('up');
		let len = length(subtract(tar, pos));
		const ssize = this.getInputValue("screensize");
		let rw = progressiveMin(ssize[0]);
		let rh = progressiveMin(ssize[1]);

		if (ev.target === this.refs.reset) {
			this.action.changeNodeInput({
				varname : varname,
				input : {
					"position" : [0, 0, 300],
					"target" : [0, 0, 0],
					"up" : [0, 1, 0],
					"fov" : 60,
					"rendersize" : [rw, rh]
				}
			});
		} else if (ev.target === this.refs.plusX) {
			this.action.changeNodeInput({
				varname : varname,
				input : {
					"position" : [len, tar[1], tar[2]],
					"rendersize" : [rw, rh]
				}
			});
		} else if (ev.target === this.refs.minusX) {
			this.action.changeNodeInput({
				varname : varname,
				input : {
					"position" : [-len, tar[1], tar[2]],
					"rendersize" : [rw, rh]
				}
			});
		} else if (ev.target === this.refs.plusY) {
			this.action.changeNodeInput({
				varname : varname,
				input : {
					"position" : [tar[0], len, tar[2] + 0.001],
					"rendersize" : [rw, rh]
				}
			});
		} else if (ev.target === this.refs.minusY) {
			this.action.changeNodeInput({
				varname : varname,
				input : {
					"position" : [tar[0], -len, tar[2] + 0.001],
					"rendersize" : [rw, rh]
				}
			});
		} else if (ev.target === this.refs.plusZ) {
			this.action.changeNodeInput({
				varname : varname,
				input : {
					"position" : [tar[0], tar[1], -len],
					"rendersize" : [rw, rh]
				}
			});
		} else if (ev.target === this.refs.minusZ) {
			this.action.changeNodeInput({
				varname : varname,
				input : {
					"position" : [tar[0], tar[1], len],
					"rendersize" : [rw, rh]
				}
			});
		}
	}

	canvasSize() {
		if (this.refs.canvas_wrap) {
			let ssize = JSON.parse(JSON.stringify(this.getInputValue("screensize")));
			return [Math.max(Number(ssize[0]), minWidth), Math.max(Number(ssize[1]), minHeight)];
		}
		return [Math.max(this.props.node.panel.size[0], minWidth), Math.max(this.props.node.panel.size[1], minHeight)];
	}
	
    drawColorBar() {
		if (this.hasColorBar()) {
			let rgba = this.state.colorbar.rgba;
			this.colorctx.fillStyle = "rgb(255, 255, 255)";
			this.colorctx.clearRect(0,0, rgba.length / 4, this.colorbarAreaHeight);
			let colorData = this.colorctx.getImageData(0,0, rgba.length / 4, this.colorbarHeight);
			
			let color = "rgb(255, 255, 255)";
			if (this.state.colorbar.color) {
				let col = this.state.colorbar.color;
				color = "rgba(" 
					+ String(Math.round(col[0] * 0xFF)) + ","
					+ String(Math.round(col[1] * 0xFF)) + ","
					+ String(Math.round(col[2] * 0xFF)) + ","
					+ String(Math.round(col[3] * 0xFF)) + ")";
			}
			for (let i = 0; i < rgba.length; i += 4) {
				let r = rgba[i + 0];
				let g = rgba[i + 1];
				let b = rgba[i + 2];
				let a = rgba[i + 3] / 255.0;
				for (let k = 0; k < this.colorbarHeight; k++) {
					let backgrd = (1.0 - a) * ((((i/4/8)|0)%2) ^ (((k/8)|0)%2)) * 155;
					colorData.data[k * rgba.length + i + 0] = r * a + backgrd;
					colorData.data[k * rgba.length + i + 1] = g * a + backgrd;
					colorData.data[k * rgba.length + i + 2] = b * a + backgrd;
					colorData.data[k * rgba.length + i + 3] = 255;
				}
			}
			this.colorctx.putImageData(colorData, 0, 0);
			
			// 枠線
			this.colorctx.strokeStyle = color;
			// 左
			this.colorctx.beginPath();
			this.colorctx.moveTo(0, 0);
			this.colorctx.lineTo(0, 22);
			this.colorctx.stroke();
			// 中央
			this.colorctx.beginPath();
			this.colorctx.moveTo(128, 0);
			this.colorctx.lineTo(128, this.colorbarHeight);
			this.colorctx.stroke();
			// 右
			this.colorctx.beginPath();
			this.colorctx.moveTo(255, 0);
			this.colorctx.lineTo(255, 22);
			this.colorctx.stroke();
			// 上
			this.colorctx.beginPath();
			this.colorctx.moveTo(0, 0);
			this.colorctx.lineTo(255, 0);
			this.colorctx.stroke();
			// 下
			this.colorctx.beginPath();
			this.colorctx.moveTo(0, this.colorbarHeight);
			this.colorctx.lineTo(255, this.colorbarHeight);
			this.colorctx.stroke();
			
			let maxVal = (this.state.colorbar.max === 0) ? "0" : this.state.colorbar.max.toFixed(5);
			let minVal = (this.state.colorbar.min === 0) ? "0" : this.state.colorbar.min.toFixed(5);
			let middleVal = (this.state.colorbar.max + this.state.colorbar.min) / 2.0;
			middleVal = (middleVal === 0) ? "0" : middleVal.toFixed(5);
			let maxWidth = this.colorctx.measureText(maxVal).width;
			
			this.colorctx.strokeStyle = color;
			this.colorctx.fillStyle = color;
			this.colorctx.fillText(minVal, 2, 25);
			this.colorctx.fillText(middleVal, 120, 25);
			this.colorctx.fillText(maxVal, 255 - maxWidth - 2, 25);
		}
    }
	
    content() {
		const styles = this.styles();
        return (<div style={styles.bounds}>
					<div ref="canvas_wrap">
						<canvas id={this.getCanvasName('canvas')} style={styles.canvas}
							width={this.canvasSize.bind(this)()[0]}
							height={this.canvasSize.bind(this)()[1]} ></canvas>
					</div>
					<img id={this.getCanvasName('img')} style={styles.image} src="" ></img>

					<canvas ref="colorbar" style={styles.colorbar}></canvas>
					
					<div style={styles.cameraButtonArea}>
						<div ref="reset"  style={styles.cameraButton}
							onClick={this.onClickCameraButton}
							onMouseEnter={this.onEnterCameraButton}
							onMouseLeave={this.onLeaveCameraButton}>
							Reset
						</div>
						<div ref="plusX"  style={styles.cameraButton}
							onClick={this.onClickCameraButton}
							onMouseEnter={this.onEnterCameraButton}
							onMouseLeave={this.onLeaveCameraButton}>
							+X
						</div>
						<div ref="minusX" style={styles.cameraButton}
							onClick={this.onClickCameraButton}
							onMouseEnter={this.onEnterCameraButton}
							onMouseLeave={this.onLeaveCameraButton}>
							-X
						</div>
						<div ref="plusY"  style={styles.cameraButton}
							onClick={this.onClickCameraButton}
							onMouseEnter={this.onEnterCameraButton}
							onMouseLeave={this.onLeaveCameraButton}>
							+Y
						</div>
						<div ref="minusY" style={styles.cameraButton}
							onClick={this.onClickCameraButton}
							onMouseEnter={this.onEnterCameraButton}
							onMouseLeave={this.onLeaveCameraButton}>
							-Y
						</div>
						<div ref="plusZ" style={styles.cameraButton}
							onClick={this.onClickCameraButton}
							onMouseEnter={this.onEnterCameraButton}
							onMouseLeave={this.onLeaveCameraButton}>
							+Z
						</div>
						<div ref="minusZ" style={styles.cameraButton}
							onClick={this.onClickCameraButton}
							onMouseEnter={this.onEnterCameraButton}
							onMouseLeave={this.onLeaveCameraButton}>
							-Z
						</div>
					</div>
					<div style={styles.presetArea}>
						<span style={{marginLeft : "3px", marginRight: "3px", fontSize : "11px"}}>Camera:</span>
						<select ref="presetSelect" style={styles.presetSelect} onChange={this.onPresetChange.bind(this)}>
							<option>1</option>
							<option>2</option>
							<option>3</option>
							<option>4</option>
							<option>5</option>
							<option>6</option>
							<option>7</option>
							<option>8</option>
							<option>9</option>
							<option>10</option>
						</select>
						<span ref="registerButton" style={styles.registerButton}
							onClick={this.onClickCameraRegisterButton}
							onMouseEnter={this.onEnterCameraButton}
							onMouseLeave={this.onLeaveCameraButton}>
							Register
						</span>
					</div>
				</div>);
	}

    getCanvasName(prefix) {
        return prefix + '-' + this.varname;
    }


    render(){
        const styles = this.styles();
        return this.content();
    }
}

module.exports = RenderView;
