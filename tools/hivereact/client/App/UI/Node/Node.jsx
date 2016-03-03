import React from "react"
import ReactDOM from "react-dom"
import NodeInOut from "./NodeInOut.jsx"
import Core from '../../Core'
import Store from './Store.jsx'

/**
 * ノード.
 */
export default class Node extends React.Component {
	constructor(props) {
		super(props);
		this.isLeftDown = false;
		this.isCtrlDown = false;
		this.mousePos = { x : 0, y : 0};

		let node = this.props.store.getNode(this.props.nodeVarname).node;
		this.state = {
			node : node,
			closeHover : false,
			isSelected : node.select,
			zIndex : 1
		};

		this.nodeChanged = this.nodeChanged.bind(this);
		this.selectChanged = this.selectChanged.bind(this);
		this.moveNode = this.moveNode.bind(this);
		this.isMinimum = this.isMinimum.bind(this);

		this.componentDidMount = this.componentDidMount.bind(this);
		this.componentWillUnmount = this.componentWillUnmount.bind(this);
		this.onMouseMove = this.onMouseMove.bind(this);
		this.onMouseUp = this.onMouseUp.bind(this);
		this.onMouseDown = this.onMouseDown.bind(this);
		this.onKeyDown = this.onKeyDown.bind(this);
		this.onKeyUp = this.onKeyUp.bind(this);
		this.styles = this.styles.bind(this);
		this.nodeRect = this.nodeRect.bind(this);
		this.getNodePos = this.getNodePos.bind(this);
		this.isClosed = this.isClosed.bind(this);
	}

	nodeChanged(err, data) {
		if (data.varname === this.props.nodeVarname) {
			this.setState({
				node : Object.assign({}, data)
			});
		}
	}

	selectChanged(err, data) {
		if (data.varname === this.props.nodeVarname) {
			if (data.select) {
				this.setState({
					isSelected : data.select,
					zIndex : 2
				});
			} else {
				this.setState({
					isSelected : data.select,
					zIndex : 1
				});
			}
		}
	}

	getNodePos() {
		return this.state.node.node.pos;
	}

	moveNode(err, data) {
		if (this.state.isSelected) {
			// マウスダウン時のoffsetLeft/offsetTopに足し込む.
			let invzoom = 1.0 / this.props.nodeStore.getZoom();
			let n = JSON.parse(JSON.stringify(this.state.node.node));
			n.pos = [this.offsetLeft + data.x * invzoom, this.offsetTop + data.y * invzoom];
			setTimeout(() => {
				this.props.action.changeNode({
					varname : this.state.node.varname,
					node : n
				});
			}, 0);
		}
	}

	nodeRect() {
		let rect = this.refs.node ? this.refs.node.getBoundingClientRect() : null;
		let zoom = 1.0 / this.props.nodeStore.getZoom();
		return {
			x : this.getNodePos()[0],
			y : this.getNodePos()[1],
			w : rect ? (rect.right - rect.left) * zoom : 200 * zoom,
			h : (Math.max(this.getInputCounts.bind(this)(), this.state.node.output.length) + 1) * 18 + 10
		};
	}

	/// 入力端子の数を返す
	getInputCounts() {
		let count = 0;
		if (this.isClosed()) {
			for (let i = 0; i < this.state.node.input.length; i = i + 1) {
				let input = this.state.node.input[i];
				if (Array.isArray(input.array)) {
					for (let k = 0; k < input.array.length; k = k + 1) {
						if (this.props.nodeStore.isConnected(this.state.node.varname, input.array[k].name)) {
							count = count + 1;
						}
					}
				} else  if (this.props.nodeStore.isConnected(this.state.node.varname, input.name)) {
					count = count + 1;
				}
			}
		} else {
			for (let i = 0; i < this.state.node.input.length; i = i + 1) {
				let input = this.state.node.input[i];
				if (Array.isArray(input.array)) {
					count = count + input.array.length;
				} else {
					count = count + 1;
				}
			}
		}
		return count;
	}

	isMinimum() {
		return this.props.isSimple && !this.state.isSelected;
	}

	/// ノードの高さを計算して返す
	getHeight() {
		let holeSize = this.isClosed() ? 10 : 15;
		if (this.isClosed()) {
			return (Math.max(this.getInputCounts.bind(this)(), this.state.node.output.length) + 1) * (holeSize+3) + 20;
		} else if (this.isMinimum()) {
			return 50;
		}
		return (Math.max(this.getInputCounts.bind(this)(), this.state.node.output.length) + 1) * (holeSize+3) + 20;
	}

	styles() {
		return {
			node : {
				position : "absolute",
				left : String(this.getNodePos()[0]),
				top : String(this.getNodePos()[1]),
				minWidth : "150px",
				height : String(this.getHeight.bind(this)()) + "px",
				backgroundColor :  this.state.isSelected ? "rgba(2, 17, 27, 0.6)" : "rgba(67, 67, 67, 0.9)",
				color : "white",
				//opacity : "0.8",
				padding : "5px",
				zIndex : this.state.zIndex,
				border : this.state.isSelected ? "solid 2px" : "solid 2px rgba (0, 0, 0, 0)",
				borderRadius : "4.5px",
				borderColor : this.state.isSelected ? "rgb(54, 196, 168)" : "rgba(0, 0, 0, 0)"
			},
			title : {
				color : "white", //"rgb(239, 136, 21)",
				fontSize : "16px",
				zoom : this.isMinimum() ? "1.5" : "1.0",
				marginRight : "20px"
			},
			closeButton : {
				position : "absolute",
				right : "0px",
				top : "0px",
				margin : "5px",
				width: "17px",
				height: "17px",
				backgroundColor : "rgb(54, 196, 168)",
				textAlign : "center",
				borderRadius : "4.5px",
				opacity : this.state.closeHover ? "0.9" : "1.0",
				cursor : "pointer",
				backgroundImage : "url(./img/node_close.png)",
				backgroundRepeat: "no-repeat",
				backgroundSize: "contain"
			}
		}
	}

	componentDidMount() {
		window.addEventListener('mousemove', this.onMouseMove);
		window.addEventListener('mouseup', this.onMouseUp);
		window.addEventListener('keydown', this.onKeyDown);
		window.addEventListener('keyup', this.onKeyUp);
		this.props.store.on(Core.Constants.NODE_CHANGED, this.nodeChanged);
		this.props.store.on(Core.Constants.NODE_SELECTE_CHANGED, this.selectChanged);
		this.props.nodeStore.on(Store.NODE_MOVED, this.moveNode);
		let rect = this.refs.node.getBoundingClientRect();
		this.props.nodeStore.setNodeSize(this.props.nodeVarname, rect.right - rect.left, rect.bottom - rect.top);
		if (this.props.id === String(this.props.nodeVarname + (this.props.store.getNodes().length - 1))) {
			this.props.nodeStore.recalcPlugPosition(this.props.store);
		}
	}

	componentWillUnmount() {
		window.removeEventListener('mousemove', this.onMouseMove);
		window.removeEventListener('mouseup', this.onMouseUp);
		window.removeEventListener('keydown', this.onKeyDown);
		window.removeEventListener('keyup', this.onKeyUp);
		this.props.store.removeListener(Core.Constants.NODE_CHANGED, this.nodeChanged);
		this.props.store.removeListener(Core.Constants.NODE_SELECTE_CHANGED, this.selectChanged);
		this.props.nodeStore.removeListener(Store.NODE_MOVED, this.moveNode);
	}

	onKeyDown(ev) {
		this.isCtrlDown = ev.ctrlKey;
	}

	onKeyUp(ev) {
		this.isCtrlDown = ev.ctrlKey;
	}

	onMouseDown(ev) {
		if (ev.button === 0) {
			this.isLeftDown = true;
			this.mousePos = { x : ev.clientX, y : ev.clientY };
			this.offsetLeft = ev.currentTarget.offsetLeft;
			this.offsetTop = ev.currentTarget.offsetTop;

			if (!this.isCtrlDown) {
				this.props.action.unSelectNode([], this.props.nodeVarname);
			}
			this.props.action.selectNode([this.props.nodeVarname]);
		}
	}

	onMouseUp(ev) {
		this.isLeftDown = false;
		this.offsetLeft = this.getNodePos()[0];
		this.offsetTop = this.getNodePos()[1];
	}

	onMouseMove(ev) {
		if (this.isLeftDown) {
			// マウスダウン位置からの差分移動量.
			let mv = { x : ev.clientX - this.mousePos.x, y : ev.clientY - this.mousePos.y };

			this.props.nodeAction.moveNode(mv);
		}
	}

	/// 閉じるボタンが押された.
	onCloseClick(ev) {
		this.props.action.deleteNode(this.props.nodeVarname);
	}

	/// 閉じるボタンにマウスホバーされた
	onCloseHover(ev) {
		this.setState({ closeHover : !this.state.closeHover })
	}

	/// 簡易表示ボタンが押された
	onOpenCloseButtonClick(ev) {
		let node = JSON.parse(JSON.stringify(this.state.node.node));
		node.close = !this.isClosed();
		this.props.action.changeNode({
			varname : this.props.nodeVarname,
			node : node
		});
	}

	/// タイトル.
	titleElem() {
		const style = this.styles();
		const isClose = this.isClosed();
		return (<div style={style.title}>
					<span
						onClick={this.onOpenCloseButtonClick.bind(this)}
						style={{
							cursor : "pointer",
							fontSize : isClose ? "12px" : "16px",
							marginLeft : isClose ? "4px" : "0px",
							marginRight : isClose ? "4px" : "0px"
						}}
					>
						{isClose ? "▶" : "▼"}
					</span>
				{this.state.node.name}
				</div>)
	}

	/// ノードが閉じているかどうか返す
	isClosed() {
		return this.state.node.node.close;
	}

	/// 入力端子.
	inputElem() {
		if (this.isMinimum()) {
			return <div/>;
		}
		let inoutIndex = -1;
		const isClose = this.isClosed();
		let inputs = this.state.node.input.map( (inputData, index) => {
			if (Array.isArray(inputData.array)) {
				let arrayInputs = inputData.array.map((data, dataIndex) => {
					if (isClose) {
					 	if (this.props.nodeStore.isConnected(this.state.node.varname, data.name)) {
							// 閉じる表示のときは繋がってるものだけ表示する
							inoutIndex = inoutIndex + 1;
							return (<NodeInOut
										nodeStore={this.props.nodeStore}
										nodeAction={this.props.nodeAction}
										nodeRect={this.nodeRect(index)}
										nodeVarname={this.props.nodeVarname}
										isInput={true} data={data}
										isClosed={isClose}
										key={this.props.nodeVarname + "_" + data.name + "_" + index + "_" + dataIndex}
										id={this.props.nodeVarname + "_" + data.name + "_" + index + "_" + dataIndex}
										index={inoutIndex} />);
						}
					} else {
						inoutIndex = inoutIndex + 1;
						return (<NodeInOut
									nodeStore={this.props.nodeStore}
									nodeAction={this.props.nodeAction}
									nodeRect={this.nodeRect(index)}
									nodeVarname={this.props.nodeVarname}
									isInput={true} data={data}
									isClosed={isClose}
									key={this.props.nodeVarname + "_" + data.name + "_" + index + "_" + dataIndex}
									id={this.props.nodeVarname + "_" + data.name + "_" + index + "_" + dataIndex}
									index={inoutIndex} />);
					}
				});
				return arrayInputs;
			} else {
				if (isClose) {
					if (this.props.nodeStore.isConnected(this.state.node.varname, inputData.name)) {
						// 閉じる表示のときは繋がってるものだけ表示する
						inoutIndex = inoutIndex + 1;
						return (<NodeInOut
									nodeStore={this.props.nodeStore}
									nodeAction={this.props.nodeAction}
									nodeRect={this.nodeRect(index)}
									nodeVarname={this.props.nodeVarname}
									isInput={true} data={inputData}
									isClosed={isClose}
									key={this.props.nodeVarname + "_" + inputData.name + "_" + index}
									id={this.props.nodeVarname + "_" + inputData.name + "_" + index}
									index={inoutIndex} />);
					}
				} else {
					inoutIndex = inoutIndex + 1;
					return (<NodeInOut
								nodeStore={this.props.nodeStore}
								nodeAction={this.props.nodeAction}
								nodeRect={this.nodeRect(index)}
								nodeVarname={this.props.nodeVarname}
								isInput={true} data={inputData}
								isClosed={isClose}
								key={this.props.nodeVarname + "_" + inputData.name + "_" + index}
								id={this.props.nodeVarname + "_" + inputData.name + "_" + index}
								index={inoutIndex} />);
				}
			}
		});
		return (<div>{inputs}</div>);
	}

	/// 出力端子.
	outputElem() {
		const isClose = this.isClosed();
		if (this.isMinimum()) {
			return <div/>;
		}
		let outputs = this.state.node.output.map( (outputData, index) => {
			return (<NodeInOut
						nodeStore={this.props.nodeStore}
						nodeAction={this.props.nodeAction}
			 			nodeRect={this.nodeRect(index)}
						nodeVarname={this.props.nodeVarname}
						isInput={false}
						data={outputData}
						isClosed={isClose}
						key={this.props.nodeVarname + "_" + outputData.name + "_" + index}
						id={this.props.nodeVarname + "_" + outputData.name + "_" + index}
						index={index} />)
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
				</div>)
	}

	onDoubleClick(ev) {
		ev.preventDefault();
		ev.stopPropagation();
	}

	render () {
		const style = this.styles();
		return (<div ref="node"
					style={style.node}
					onMouseDown={this.onMouseDown.bind(this)}
					onDoubleClick={this.onDoubleClick.bind(this)}
				>
					{this.titleElem.bind(this)()}
					{this.inputElem.bind(this)()}
					{this.outputElem.bind(this)()}
					{this.closeElem.bind(this)()}
				</div>);
	}
}
