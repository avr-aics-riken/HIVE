import React from "react"
import ReactDOM from "react-dom"
import Store from './Store.jsx'

function colorFunction(type, hover) {
	if (type === "string") {
		return hover ? "#34c291" : "#14a271";
	} else if (type === "float") {
		return hover ? "#33bac5" : "#139aa5";
	} else if (type === "vec4") {
		return hover ? "#3b8af6" : "#1b6ad6";
	} else if (type === "vec3") {
		return hover ? "#768fbf" : "#566f9f";
	} else if (type === "vec2") {
		return hover ? "#a242c7" : "#8222a7";
	} else if (type === "RenderObject") {
		return hover ? "#cd5b98" : "#ad3b78";
	} else if (type === "Uniform") {
		return hover ? "#d1be34" : "#b19e14";
	} else if (type === "BufferMeshData") {
		return hover ? "#de3676" :"#be1656";
	} else if (type === "BufferPointData") {
		return hover ? "#ff43ff" : "#e023e0";
	} else if (type === "BufferLineData") {
		return hover ? "#40eaff" : "#20cae0";
	} else if (type === "BufferVolumeData") {
		return hover ? "#37f062" : "#17d042";
	} else if (type === "Any") {
		return hover ? "#ffa835" : "#ef8815";
	} else { // Object
		return hover ? "#e14437" : "#c12417";
	}
}

/**
 * ノードの入出力端子.
 */
export default class NodeInOut extends React.Component {
	constructor(props) {
		super(props);
		this.styles = this.styles.bind(this);

		this.state = {
			isDragging : false,
			hover : false,
		};
		this.pos = {
			x : 0,
			y : 0
		};

		this.position = this.position.bind(this);
		this.holeCenterPosition = this.holeCenterPosition.bind(this);
		this.onMouseDown = this.onMouseDown.bind(this);
		this.onMouseMove = this.onMouseMove.bind(this);
		this.onMouseUp = this.onMouseUp.bind(this);
		this.onClick = this.onClick.bind(this);
		this.onPlugDragging = this.onPlugDragging.bind(this);
		this.plugInfo = this.plugInfo.bind(this);
	}

	position() {
		let holeSize =  this.props.isClosed ? 10 : 15;
		let holeSizeW =  this.props.isClosed ? 10 : 27.5;
		let holeSizeH =  this.props.isClosed ? 10 : 8;
		let nodeRect = this.props.nodeRect;
		if (this.props.isInput) {
			return {
				x : nodeRect.x - (holeSizeW / 2.0),
				y : nodeRect.y + (holeSize + 3) * (this.props.index + 1) + 10 + 7,
			}
		} else {
			return {
				x : nodeRect.x + nodeRect.w - (holeSizeW / 2.0),
				y : nodeRect.y + (holeSize + 3) * (this.props.index + 1) + 10 + 7
			}
		}
	}

	holeCenterPosition() {
		let holeSize =  this.props.isClosed ? 10 : 15;
		let holeSizeW =  this.props.isClosed ? 10 : 27.5;
		let holeSizeH =  this.props.isClosed ? 10 : 8;
		let position = this.position();
		position.x = (position.x + (holeSizeW / 2.0));
		position.y = (position.y + (holeSize / 2.0));
		return position;
	}

	styles() {
		let holeSize =  this.props.isClosed ? 10 : 15;
		let holeSizeW =  this.props.isClosed ? 10 : 27.5;
		let holeSizeH =  this.props.isClosed ? 10 : 8;
		return {
			input : {
				position : "absolute",
				left : String(-holeSizeW / 2),
				top : String( (holeSize+3) * (this.props.index + 1) + 10 + (15 - holeSize)),
				width : "100px",
				height : this.props.isClosed ? "10px" : "20px"
			},
			output : {
				position : "absolute",
				right : String(-holeSizeW / 2),
				top : String(18 * (this.props.index + 1) + 10 + (15 - holeSize)),
				width : "100px",
				height : this.props.isClosed ? "10px" : "20px"
			},
			inhole : {
				position : "absolute",
				left : "0px",
				width : holeSizeW + "px",
				height : holeSizeH + "px",
				marginTop : this.props.isClosed ? "0px" : "7px",
				borderRadius : "7px 7px 7px 7px / 7px 7px 7px 7px",
				backgroundColor : colorFunction(this.props.data.type, this.state.hover),
				border : (this.state.isDragging) ? "solid 1px" : "none"
			},
			outhole : {
				position : "absolute",
				right : "0px",
				width : holeSizeW,
				height : holeSizeH,
				marginTop : this.props.isClosed ?  "0px" : "7px",
				borderRadius : "7px 7px 7px 7px / 7px 7px 7px 7px",
				backgroundColor : colorFunction(this.props.data.type, this.state.hover),
				border : (this.state.isDragging) ? "solid 1px" : "none",
			},
			inholeText : {
				position : "absolute",
				top : "0px",
				left : holeSizeW + 3 + "px", // holeのサイズ
				marginLeft : "4px",
				color : "white",
				fontSize : "14px"
			},
			outholeText : {
				position : "absolute",
				top : "0px",
				right : holeSizeW + 3 + "px", // holeのサイズ
				marginRight : "4px",
				color : "white",
				fontSize : "14px"
			}
		}
	}

	// プラグをドラッグするActionを発行.
	onMouseDown(ev) {
		if (ev.button === 0) {
			let id = this.props.id;
			this.pos = {
				x : ev.clientX,
				y : ev.clientY
			}
			if (this.props.isInput) {
				this.props.nodeAction.dragPlug(id, {
					x : ev.clientX,
					y : ev.clientY
				}, this.pos);
			} else {
				this.props.nodeAction.dragPlug(id, this.pos, {
					x : ev.clientX,
					y : ev.clientY
				});
			}
			this.props.nodeAction.unSelectPlugHoles();
			this.props.nodeAction.selectPlugHole(this.plugInfo());

			ev.preventDefault();
			ev.stopPropagation();
		}
	}

	onClick(ev) {
		let plugInfo = this.plugInfo();
		this.props.nodeAction.disconnectPlugHole(plugInfo);
	}

	// プラグをドラッグするActionを発行.
	onMouseMove(ev) {
		if (this.state.isDragging) {
			if (this.props.nodeAction.dispatcher.isDispatching()) { return; }
			let center = this.holeCenterPosition();
			let zoom = 1.0 / this.props.nodeStore.getZoom();
			if (this.props.isInput) {
				this.props.nodeAction.dragPlug(this.props.id, center, {
					x : center.x + (ev.clientX - this.pos.x) * zoom,
					y : center.y + (ev.clientY - this.pos.y) * zoom
				});
			} else {
				this.props.nodeAction.dragPlug(this.props.id, {
					x : center.x + (ev.clientX - this.pos.x) * zoom,
					y : center.y + (ev.clientY - this.pos.y) * zoom
				}, center);
			}
			ev.preventDefault();
			ev.stopPropagation();
		}
	}

	plugInfo() {
		return {
			data : this.props.data,
			isInput : this.props.isInput
		}
	}

	onMouseUp(ev) {
		if (this.state.isDragging) {
			this.setState({
				isDragging :false
			});
			let center = this.holeCenterPosition();
			if (this.props.isInput) {
				this.props.nodeAction.endDragPlug(this.props.id, center, {
					x : center.x + (ev.clientX - this.pos.x),
					y : center.y + (ev.clientY - this.pos.y)
				});
			} else {
				this.props.nodeAction.endDragPlug(this.props.id, {
					x : center.x + (ev.clientX - this.pos.x),
					y : center.y + (ev.clientY - this.pos.y)
				}, center);
			}
		}
	}

	onMouseUp2(ev) {
		if (!this.state.isDragging) {
			this.props.nodeAction.selectPlugHole(this.plugInfo());
		}
	}

	onPlugDragging(err, id, inpos, outpos) {
		if (id === this.props.id) {
			this.setState({
				isDragging : true
			});
		}
	}

	componentDidMount() {
		window.addEventListener('mousemove', this.onMouseMove);
		window.addEventListener('mouseup', this.onMouseUp);
		this.props.nodeStore.on(Store.PLUG_DRAGGING, this.onPlugDragging);
	}

	componentWillUnmount() {
		window.removeEventListener('mousemove', this.onMouseMove);
		window.removeEventListener('mouseup', this.onMouseUp);
		this.props.nodeStore.removeListener(Store.PLUG_DRAGGING, this.onPlugDragging);
	}

	inHoleText() {
		const style = this.styles(this.props.index);
		if (!this.props.isClosed) {
			return (<div style={style.inholeText}>{this.props.data.name}</div>);
		}
	}

	outHoleText() {
		const style = this.styles(this.props.index);
		if (!this.props.isClosed) {
			return (<div style={style.outholeText}>{this.props.data.name}</div>);
		}
	}

	/// マウスホバーされた
	onMouseEnter(ev) {
		if (ev.button === 1 || ev.button === 2) { return; }
		this.setState({ hover : true })
		ev.target.style.cursor = "pointer";
	}

	onMouseLeave(ev) {
		this.setState({ hover : false })
		ev.target.style.cursor = "default";
	}

	content() {
		const style = this.styles(this.props.index);
		if (this.props.isInput) {
			// 入力端子.
			return (<div style={style.input}>
						<div style={style.inhole}
							onMouseDown={this.onMouseDown.bind(this)}
							onMouseUp={this.onMouseUp2.bind(this)}
							onClick={this.onClick.bind(this)}
							onMouseEnter={this.onMouseEnter.bind(this)}
							onMouseLeave={this.onMouseLeave.bind(this)}
						/>
						{this.inHoleText.bind(this)()}
					</div>);
		} else {
			// 出力端子.
			return (<div style={style.output}>
						<div style={style.outhole}
							onMouseDown={this.onMouseDown.bind(this)}
							onMouseUp={this.onMouseUp2.bind(this)}
							onClick={this.onClick.bind(this)}
							onMouseEnter={this.onMouseEnter.bind(this)}
							onMouseLeave={this.onMouseLeave.bind(this)}
						/>
						{this.outHoleText.bind(this)()}
					</div>);
		}
	}

	render() {
		return this.content.bind(this)();
	}
}
