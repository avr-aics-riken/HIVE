import React from 'react'

const minWidth = 256;
const minHeight = 100;
const footerHeight = 50;

class ImageView extends React.Component {
	constructor(props) {
		super(props);
		this.node = props.node;
		this.action = props.action;
		this.store = props.store;
		
		this.varname = this.node.varname;
		
		this.state = {
			width : this.canvasSize()[0],
			height : this.canvasSize()[1]
		};
		
		this.componentDidUpdate = this.componentDidUpdate.bind(this);
		this.imageRecieved = this.imageRecieved.bind(this);
		this.imageRecieveWrap = this.imageRecieveWrap.bind(this);
		this.onPanelSizeChanged = this.onPanelSizeChanged.bind(this);
		this.getInputValue = this.getInputValue.bind(this);
		this.imageSize = null;
	}

	imageUpdate(param) {
		let w = param.width;
		let h = param.height;
		const varname = this.node.varname;
	}
	
	imageRecieved(err, param, data) {
		var buffer;
		const varname = this.node.varname;
		if (param.varname !== varname) {
			return;
		}
		if (param.type === 'jpg') {
			buffer = new Blob([data]);
		} else {
			buffer = data;
		}
		this.setState({
			image : buffer
		});
		this.imageSize = [parseFloat(param.width), parseFloat(param.height)]

		this.imageUpdate(param);
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
				if (!this.meta.loadMetaBinary(data, (meta, data) => {
					var w, h,
						param = meta.param;
						
					console.log("param", param);
					if (param.type === 'jpg') {
						// resultElement is img.
						var resultElement = document.getElementById(this.getCanvasName('img'));
						resultElement.src = URL.createObjectURL(new Blob([data], {type: "image/jpeg"}));

					} else if (param.type === 'png') {
						// resultElement is img.
						var resultElement = document.getElementById(this.getCanvasName('img'));
						resultElement.src = URL.createObjectURL(new Blob([data], {type: "image/png"}));
					} else if (param.type === 'raw'){
						// resultElement is canvas.
						var resultElement = document.getElementById(this.getCanvasName('canvas'));
						if (resultElement) { // why undefined, sometime??
							resultElement.setAttribute('width', param.width),
							resultElement.setAttribute('height', param.height);
							var context = resultElement.getContext('2d');
							var imageData = context.createImageData(param.width, param.height);
							buffercopy.buffercopy(data, imageData.data);
							this.setState({
								imageData : imageData,
							});
							
							this.imageSize = [param.width, param.height];
							context.putImageData(imageData, 0, 0);

							this.imageUpdate(param);
						}
					}

				})) {
					console.error('Not metabin foramt');
				};
			});
		}
	}
	
	getCanvasName(prefix) {
		return prefix + '-' + this.varname;
	}
	
	getInputValue(key) {
		for (let i = 0; i < this.node.input.length; i = i + 1) {
			if (this.node.input[i].name === key) {
				return this.node.input[i].value;
			}
		}
	}
	
	hasIPCAddress() {
		return this.getInputValue('ipcmode');
	}
	
	imageRecieveWrap(err, data) {
		if (data.varname !== this.node.varname) {
			return;
		}
		
		if (this.hasIPCAddress()) {
			this.readyForIPCImageTransfer();
		}
	}
	
    closeForIPCImageTransfer(){
        if (this.sc === undefined) {
        } else {
            this.sc.close();
            this.sc = undefined;
        }
    }
	
	componentDidMount() {
        const Store_IMAGE_RECIEVED = "image_revieved";
		this.store.on(Store_IMAGE_RECIEVED, this.imageRecieved);
		const NODE_INPUT_CHANGED = "node_input_changed"
		this.store.on(NODE_INPUT_CHANGED, this.imageRecieveWrap);
		this.store.on("panel_size_changed", this.onPanelSizeChanged);
	}
	
	componentWillUnmount() {
		const Store_IMAGE_RECIEVED = "image_revieved";
		this.store.off(Store_IMAGE_RECIEVED, this.imageRecieved);
		const NODE_INPUT_CHANGED = "node_input_changed"		
		this.store.off(NODE_INPUT_CHANGED, this.imageRecieveWrap);
		this.store.off("panel_size_changed", this.onPanelSizeChanged);
		this.closeForIPCImageTransfer();
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
			}
		}
	}
	

	componentDidUpdate() {
		// キャンバスの更新.
		if (this.state && this.state.image) {
			if (!this.hasIPCAddress()) {
				let imgElem = document.getElementById(this.getCanvasName('img'));
				imgElem.src = URL.createObjectURL(this.state.image, {type: "image/jpeg"})
			} else {
				if (this.imageSize) {
					let canvas = document.getElementById(this.getCanvasName('canvas'));
					canvas.setAttribute('width', this.imageSize[0]);
					canvas.setAttribute('height', this.imageSize[1]);
					let context = canvas.getContext('2d');
					context.putImageData(this.state.imageData, 0, 0);
				}
			}
		}
	}
	
	canvasSize() {
		return [Math.max(this.props.node.panel.size[0], minWidth), Math.max(this.props.node.panel.size[1], minHeight)];
	}
	
	aspectCanvasSize() {
		if (this.imageSize) {
			let aspect = this.imageSize[0] / this.imageSize[1];
			let w = Math.max(this.props.node.panel.size[0], minWidth);
			return [w, w / aspect];
		}
		return [Math.max(this.props.node.panel.size[0], minWidth), Math.max(this.props.node.panel.size[1], minHeight)];
	}
	
	aspectImageSize() {
		if (this.imageSize) {
			let aspect = this.imageSize[0] / this.imageSize[1];
			let w = Math.max(this.state.width, minWidth);
			return [w, w / aspect];
		}
		return [Math.max(this.state.width, minWidth), Math.max(this.state.height,  minHeight)];
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
				width: String(this.aspectCanvasSize.bind(this)()[0]) + "px",
				height: String(this.aspectCanvasSize.bind(this)()[1]) + "px",
				transform : "scale(1.0,-1.0)",
				display: (this.hasIPCAddress() ? "block" : "none")
			},
			image : {
				postion : "relative",
				left : "0px",
				top : "0px",
				width: String(this.aspectImageSize.bind(this)()[0]) + "px",
				height: String(this.aspectImageSize.bind(this)()[1])  + "px",
				display: (this.hasIPCAddress() ? "none" : "block")
			},
		}
	}

	render() {
		const styles = this.styles();
		return (<div style={{minWidth:"250px", height:'250'}}>
			<div ref="canvas_wrap">
				<canvas id={this.getCanvasName('canvas')} style={styles.canvas}
					width={this.canvasSize.bind(this)()[0]}
					height={this.canvasSize.bind(this)()[1]} ></canvas>
			</div>
			<img id={this.getCanvasName('img')} style={styles.image} src="" ></img>
		</div>);
	}
}

module.exports = ImageView;
