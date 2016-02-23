import React from "react"
import ReactDOM from "react-dom"
import NodeInOut from "./NodeInOut.jsx"
import Core from '../../Core'

/**
 * ノード.
 */
export default class Node extends React.Component {
	constructor(props) {
		super(props);
		this.isLeftDown = false;
		this.mousePos = { x : 0, y : 0};

		this.state = {
			node : null,
			closeHover : false
		};
		let nodes = this.props.store.getNodes();
		for (let i = 0; i < nodes.length; i = i + 1) {
			if (nodes[i].varname === this.props.node.varname) {
				this.state.node = nodes[i];
				break;
			}
		}

		this.nodeChanged = this.nodeChanged.bind(this);

		this.componentDidMount = this.componentDidMount.bind(this);
		this.componentWillUnmount = this.componentWillUnmount.bind(this);
		this.onMouseMove = this.onMouseMove.bind(this);
		this.onMouseUp = this.onMouseUp.bind(this);
		this.onMouseDown = this.onMouseDown.bind(this);
		this.styles = this.styles.bind(this);
	}

	nodeChanged(err, data) {
		if (data.varname === this.props.node.varname) {
			this.setState({
				node : data
			});
		}
	}

	styles() {
		return {
			node : {
				position : "absolute",
				left : String(this.props.node.pos[0]),
				top : String(this.props.node.pos[1]),
				width : "200px",
				height : String((Math.max(this.props.node.input.length, this.props.node.output.length) + 1) * 18 + 10),
				backgroundColor : "rgb(66, 69, 66)",
				color : "white",
				opacity : "0.8",
				padding : "5px"
			},
			title : {
				color : "rgb(239, 136, 21)",
				fontSize : "16px"
			},
			closeButton : {
				position : "absolute",
				right : "0px",
				top : "0px",
				margin : "5px",
				width: "15px",
				height: "15px",
				backgroundColor : "#ea4412",
				textAlign : "center",
				borderRadius : "5px",
				border : this.state.closeHover ? "solid 1px" : "none"
			},
			closeText : {
				lineHeight : "12px",
				cursor : "pointer"
			}
		}
	}

	componentDidMount() {
		window.addEventListener('mousemove', this.onMouseMove);
		window.addEventListener('mouseup', this.onMouseUp);
		this.props.store.on(Core.Store.NODE_CHANGED, this.nodeChanged);
	}

	componentWillUnmount() {
		window.removeEventListener('mousemove', this.onMouseMove);
		window.removeEventListener('mouseup', this.onMouseUp);
		this.props.store.removeListener(Core.Store.NODE_CHANGED, this.nodeChanged);
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
			let node = this.props.node;
			node.pos = [this.offsetLeft + mv.x, this.offsetTop + mv.y];
			this.props.action.changeNode(node);
		}
	}

	/// 閉じるボタンが押された.
	onCloseClick(ev) {
		this.props.action.deleteNode(this.props.node.varname);
	}

	/// 閉じるボタンにマウスホバーされた
	onCloseHover(ev) {
		this.setState({ closeHover : !this.state.closeHover })
	}

	/// タイトル.
	titleElem() {
		const style = this.styles.bind();
		return <div style={style.title}>{this.props.node.name}</div>
	}

	/// 入力端子.
	inputElem() {
		let inputs = this.props.node.input.map( (inputData, key) => {
			return (<NodeInOut isInput={true} data={inputData} key={inputData.name + key} index={key} />)
		});
		return (<div>{inputs}</div>);
	}

	/// 出力端子.
	outputElem() {
		let outputs = this.props.node.output.map( (outputData, key) => {
			return (<NodeInOut isInput={false} data={outputData} key={outputData.name + key} index={key} />)
		});
		return (<div>{outputs}</div>);
	}

	/// 閉じるボタン
	closeElem() {
		const style = this.styles();
		return (<div style={style.closeButton}
					onClick={this.onCloseClick.bind(this)}
					onMouseEnter={this.onCloseHover.bind(this)}
					onMouseLeave={this.onCloseHover.bind(this)}
				>
					<div style={style.closeText}>x</div>
				</div>)
	}

	render () {
		const style = this.styles();
		return (<div ref="node"
					style={style.node}
					onMouseDown={this.onMouseDown.bind(this)}
				>
					{this.titleElem.bind(this)()}
					{this.inputElem.bind(this)()}
					{this.outputElem.bind(this)()}
					{this.closeElem.bind(this)()}
				</div>);
	}
}
