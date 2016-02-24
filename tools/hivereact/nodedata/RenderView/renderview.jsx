import React from 'react'
import ReactDOM from 'react-dom'

class RenderView extends React.Component {
	constructor(props) {
        super(props);
        console.log('RenderView Constructor:', props);
		
        this.node = props.node;
        this.action = props.action;
        this.store = props.store;

        this.state = {
            text: ''            
        }
        
        this.componentDidUpdate = this.componentDidUpdate.bind(this);
		this.componentDidMount = this.componentDidMount.bind(this);

        const Store_IMAGE_RECIEVED = "image_revieved";
		this.store.on(Store_IMAGE_RECIEVED, (err, param, data) => {
			var buffer;
			console.log(param);

			if (param.type === 'jpg') {
				buffer = new Blob([data]);
			} else {
				buffer = data;
			}
			this.setState({
				param : param,
				image : buffer
			});
		});
	}
    
    hasIPCAddress() {
		return (this.props.ipc_address && this.props.ipc_address !== "");
	}
	componentDidUpdate() {
		// キャンバスの更新.        
		if (this.state.image) {
			if (this.hasIPCAddress()) {
				/*let canvas = ReactDOM.findDOMNode(this.refs.canvas);
				let context = canvas.getContext('2d');
				let width = this.state.param.width;
				let height = this.state.param.height;
				canvas.setAttribute('width', width);
				canvas.setAttribute('height', height);

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
                let imgElem = document.getElementById('aascreen');                
                imgElem.src = URL.createObjectURL(this.state.image, {type: "image/jpeg"})
                console.log(imgElem);
			}
		}
	}

	componentDidMount() {
		// canvas.tabIndex = 1000;
		// window.addEventListener('mouseup', this.onMouseUp);
		// window.addEventListener('mousemove', this.onMouseMove, true);
	}

	componentWillUnmount() {
		// window.removeEventListener('mouseup', this.onMouseUp);
		// window.removeEventListener('mousemove', this.onMouseMove, true);
	}

    styles() {
		return {
			canvas : {
				postion : "relative",
				left : "0px",
				top : "0px"
			},
			image : {
				postion : "relative",
				left : "0px",
				top : "0px",
                width: "256px",
                height:"256px"
			}
		}
	}
    
    onChange(i, event) {
        let text = event.target.value;
        this.setState({text});
        this.node.input[5].value[i] = parseFloat(text);        
        this.action.changeNode(this.node);
    }
    
    content() {
		const styles = this.styles();
		if (this.props.ipc_address && this.props.ipc_address !== "") {
			return (<canvas id="screen" style={styles.canvas} ref="canvas" ></canvas>);
		} else {
			return (<img id="screen" style={styles.image} ref="image" src="" ></img>);
		}
	}

    
    render(){
        
        const styles = this.styles();
        return (
            <div>
                <div>
                <p>R:</p><input type="text" style={{height: 20, borderColor: 'gray', borderWidth: 1}}
                    onChange = {this.onChange.bind(this,0)}
                    defaultValue={this.node.input[5].value[0]}/>
                </div>
                <div>
                <p>G:</p><input type="text" style={{height: 20, borderColor: 'gray', borderWidth: 1}}
                    onChange = {this.onChange.bind(this,1)}
                    defaultValue={this.node.input[5].value[1]}/>
                </div>
                <div>
                <p>B:</p><input type="text" style={{height: 20, borderColor: 'gray', borderWidth: 1}}
                    onChange = {this.onChange.bind(this,2)}
                    defaultValue={this.node.input[5].value[2]}/>
                </div>
                
               <img id="aascreen" style={styles.image} src="" ></img>
                
            </div>
        );
    }
}

module.exports = RenderView;
