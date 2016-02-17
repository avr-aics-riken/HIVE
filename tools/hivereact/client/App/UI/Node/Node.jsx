import React from "react"
import ReactDOM from "react-dom"

export default class Node extends React.Component {
	constructor(props) {
		super(props);
		this.isLeftDown = false;
		this.mousePos = { x : 0, y : 0};
		this.state = {
			pos : { x : 300, y : 600 }
		};
		this.componentDidMount = this.componentDidMount.bind(this);
		this.componentWillUnmount = this.componentWillUnmount.bind(this);
		this.onMouseMove = this.onMouseMove.bind(this);
		this.onMouseUp = this.onMouseUp.bind(this);
		this.onMouseDown = this.onMouseDown.bind(this);
	}

	styles() {
		return {
			node : {
				position : "absolute",
				left : String(this.state.pos.x),
				top : String(this.state.pos.y),
				width : "200px",
				height : "100px",
				backgroundColor : "red",
				color : "white"
			}
		}
	}

	componentDidMount() {
		window.addEventListener('mousemove', this.onMouseMove);
		window.addEventListener('mouseup', this.onMouseUp);
	}

	componentWillUnmount() {
		window.removeListener('mousemove', this.onMouseMove);
		window.removeListener('mouseup', this.onMouseUp);
	}

	onMouseDown(ev) {
		if (ev.button === 0) {
			this.isLeftDown = true;
			this.mousePos = { x : ev.clientX, y : ev.clientY };
			this.offsetLeft = ev.currentTarget.offsetLeft;
			this.offsetTop = ev.currentTarget.offsetTop;
		}
	}

	onMouseUp(ev) {
		this.isLeftDown = false;
	}

	onMouseMove(ev) {
		if (this.isLeftDown) {
			// マウスダウン位置からの差分移動量.
			let mv = { x : ev.clientX - this.mousePos.x, y : ev.clientY - this.mousePos.y };
			// マウスダウン時のoffsetLeft/offsetTopに足し込む.
			this.setState({ pos : { x : this.offsetLeft + mv.x, y : this.offsetTop + mv.y }})
		}
	}

	render () {
		const styles = this.styles.bind(this)();
		return (<div style={styles.node}
					ref="node"
					onMouseDown={this.onMouseDown.bind(this)}
			>hogehoge</div>);
	}
}
