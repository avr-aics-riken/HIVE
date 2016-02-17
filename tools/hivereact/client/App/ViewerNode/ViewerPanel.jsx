import React from "react"
import ReactDOM from "react-dom"
import HiveReact from "../HiveReact"
import buffercopy from "buffercopy"

export default class ViewerPanel extends React.Component {
	constructor(props) {
		super(props);

		this.store = props.hivestore;
		this.action = props.hiveaction;

		this.state = {
			//image : this.store.getImage()
			//windowSize : this.store.getWindowSize()
		};

		this.store.on(HiveReact.HiveStore.IMAGE_CHANGED, function (err, data) {
			this.setState({
				image : data
			});
		}.bind(this));

		this.componentDidUpdate = this.componentDidUpdate.bind(this);
		this.componentDidMount = this.componentDidMount.bind(this);
		this.onResize = this.onResize.bind(this);
	}

	componentDidUpdate() {
		// キャンバスの更新.
		if (this.state.image) {
			var canvas = ReactDOM.findDOMNode(this.refs.canvas),
				context = canvas.getContext('2d'),
				width = canvas.clientWidth,
				height = canvas.clientHeight;

			canvas.setAttribute('width', width);
			canvas.setAttribute('height', height);
			if (width * height * 4 == this.state.image.length) {
				var imageData = context.createImageData(width, height);
				buffercopy.buffercopy(this.state.image, imageData.data);
				context.putImageData(imageData, 0, 0);
			} else {
				console.error("image size err:", width, height);
			}
		}
	}

	componentDidMount() {
		// windowに対してマウスイベントを登録
		var canvas = ReactDOM.findDOMNode(this.refs.canvas);
		this.onResize(canvas);
		// canvas.tabIndex = 1000;
		// window.addEventListener('mouseup', this.onMouseUp);
		// window.addEventListener('mousemove', this.onMouseMove, true);
	}

	componentWillUnmount() {
		// window.removeEventListener('mouseup', this.onMouseUp);
		// window.removeEventListener('mousemove', this.onMouseMove, true);
	}

	onResize(element) {
		var elementHeight = element.clientHeight,
			elementWidth = element.clientWidth;

		setInterval((function (self, canvas, elementWidth, elementHeight) {
			return function() {
				if (element.clientHeight !== elementHeight || element.clientWidth !== elementWidth) {
					console.log("setWindowSize", element.clientWidth, element.clientHeight );
					elementHeight = element.clientHeight;
					elementWidth = element.clientWidth;
					element.width = element.clientWidth;
					element.height = element.clientHeight;
					console.log(self.action);
					//self.action.setWindowSize(element.width, element.height);
				}
			};
		}(this, element, elementWidth, elementHeight)), 300);
	}

	styles() {
		return {
			canvas : {
				postion : "relative",
				left : "0px",
				top : "0px"
			}
		}
	}

	render () {
		const styles = this.styles();
		return (<canvas id="screen" style={styles.canvas} ref="canvas" ></canvas>
		);
	}
}
