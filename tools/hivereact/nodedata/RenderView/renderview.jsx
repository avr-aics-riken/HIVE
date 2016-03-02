import React from 'react'
import ReactDOM from 'react-dom'

import nanomsg from 'nanomsg'
import buffercopy from 'buffercopy'

class RenderView extends React.Component {
	constructor(props) {
        super(props);
        console.log('RenderView Constructor:', props);

        this.node = props.node;
        this.action = props.action;
        this.store = props.store;

        this.varname = this.node.varname;

        // Mouse
        this.mouseState = 0;
        this.oldmx = 0;
        this.oldmy = 0;

        // View


        this.componentDidUpdate = this.componentDidUpdate.bind(this);
		this.imageRecieved = this.imageRecieved.bind(this);
	}

	imageRecieved(err, param, data) {
		var buffer;
		if (param.varname !== this.node.varname) {
			return;
		}
		if (param.type === 'jpg') {
			buffer = new Blob([data]);
		} else {
			buffer = data;
		}
		this.setState({
			param : param,
			image : buffer
		});
	}

    hasIPCAddress() {
		//return (this.props.ipc_address && this.props.ipc_address !== "");
        const r = (this.node.input[8].value !== undefined && this.node.input[8].value !== "");
        //console.log('IPCCCCC', r, this.node.input[8].value); // ipcpath
        return r;
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
        if (this.sc === undefined) {
            var nano = nanomsg;//require('nanomsg');
            var sc = nano.socket('pair');
            this.sc = sc;
            var meta = require('../lib/metabinary'); // path from index.html
            this.meta = meta;
            var ipcAddress = 'ipc:///tmp/HIVE_IPC_' + this.varname;// + this.node.ipcpath;
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
                imgElem.src = URL.createObjectURL(this.state.image, {type: "image/jpeg"})
                //console.log(imgElem);
			}
		}
	}

    viewRot(rx, ry, rz) {

        let newval = this.node.input[0].value.concat();
        newval[0] += rx;
        newval[1] += ry;
        newval[2] += rz;

        const varname = this.node.varname;
        const inputs = JSON.parse(JSON.stringify(this.node.input));
        //console.log(this.node.input[0]);
        inputs[0].value = newval;
        this.action.changeNode({
            varname: varname,
            input: inputs
        });
    }

    viewTrans(tx, ty, tz) {

    }
    viewZoom(zoom) {

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

            this.viewRot(dy, dx, 0.0);

            this.oldmx = event.clientX;
            this.oldmy = event.clientY;
        }
    }
    onImgMouseUp(event) {
        this.mouseState = 0;
    }

	componentDidMount() {
        let imgElem = document.getElementById(this.getCanvasName('img'));
        imgElem.addEventListener('mousedown', this.onImgMouseDown.bind(this), true);

        let canElem = document.getElementById(this.getCanvasName('canvas'));
        canElem.addEventListener('mousedown', this.onImgMouseDown.bind(this), true);

		// canvas.tabIndex = 1000;
		window.addEventListener('mouseup', this.onImgMouseUp.bind(this));
		window.addEventListener('mousemove', this.onImgMouseMove.bind(this), true);

        const NODE_INPUT_CHANGED = "node_input_changed"
        this.store.on(NODE_INPUT_CHANGED, () => {
            if (this.hasIPCAddress()) {
                this.readyForIPCImageTransfer();
            }
        })
        /**/
        const Store_IMAGE_RECIEVED = "image_revieved";
		this.store.on(Store_IMAGE_RECIEVED, this.imageRecieved);
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
		this.closeForIPCImageTransfer();
	}

    styles() {
		return {
			canvas : {
				postion : "relative",
				left : "0px",
				top : "0px",
                width: "512px",
                height:"512px",
                transform : "scale(1.0,-1.0)",
                display: (this.hasIPCAddress() ? "block" : "none")
			},
			image : {
				postion : "relative",
				left : "0px",
				top : "0px",
                width: "256px",
                height:"256px",
                display: (this.hasIPCAddress() ? "none" : "block")
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

    content() {
		const styles = this.styles();
		if (this.hasIPCAddress()) {
		} else {
        }
        return (<div>
            <canvas id={this.getCanvasName('canvas')} style={styles.canvas} ></canvas>
            <img id={this.getCanvasName('img')} style={styles.image} src="" ></img>
            </div>);

	}

    getCanvasName(prefix) {
        return prefix + '-' + this.varname;
    }


    render(){

        const styles = this.styles();
        return this.content()


        /*return (
            <div>
                <div>
                <p>R:</p><input type="text" style={{height: 20, borderColor: 'gray', borderWidth: 1}}
                    onChange = {this.onChange.bind(this,0)}
					value={this.node.input[5].value[0]}
                    defaultValue={this.node.input[5].value[0]}/>
                </div>
                <div>
                <p>G:</p><input type="text" style={{height: 20, borderColor: 'gray', borderWidth: 1}}
                    onChange = {this.onChange.bind(this,1)}
					value={this.node.input[5].value[1]}
                    defaultValue={this.node.input[5].value[1]}/>
                </div>
                <div>
                <p>B:</p><input type="text" style={{height: 20, borderColor: 'gray', borderWidth: 1}}
                    onChange = {this.onChange.bind(this,2)}
					value={this.node.input[5].value[2]}
                    defaultValue={this.node.input[5].value[2]}/>
                </div>

               <img id={"aascreen-" + this.varname} style={styles.image} src="" ></img>

            </div>
        );*/
    }
}

module.exports = RenderView;
