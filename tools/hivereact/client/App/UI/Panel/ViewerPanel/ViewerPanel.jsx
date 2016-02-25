import React from "react"
import ReactDOM from "react-dom"
import Core from "../../../Core"
import buffercopy from "buffercopy"

export default class ViewerPanel extends React.Component {
	constructor(props) {
		super(props);

		this.store = props.store;
		this.action = props.action;

		this.state = {
			//image : this.store.getImage()
			//windowSize : this.store.getWindowSize()
		};

		this.componentDidUpdate = this.componentDidUpdate.bind(this);
		this.componentDidMount = this.componentDidMount.bind(this);

		this.store.on(Core.Constants.IMAGE_RECIEVED, (err, param, data) => {
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
				let canvas = ReactDOM.findDOMNode(this.refs.canvas);
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
				}
			} else {
				let imgElem = ReactDOM.findDOMNode(this.refs.image);
				imgElem.src = URL.createObjectURL(this.state.image, {type: "image/jpeg"});
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
				top : "0px"
			}
		}
	}

	content() {
		const styles = this.styles();
		if (this.props.ipc_address && this.props.ipc_address !== "") {
			return (<canvas id="screen" style={styles.canvas} ref="canvas" ></canvas>);
		} else {
			return (<img id="screen" style={styles.image} ref="image" src="" ></img>);
		}
	}

	render () {
		return (this.content.bind(this)());
	}
}
