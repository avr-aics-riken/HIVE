import React from "react"
import ReactDOM from "react-dom"
import Store from './Store.jsx'

function colorFunction(type) {
	if (type === "string") {
		return "#14a271";
	} else if (type === "float") {
		return "#139aa5";
	} else if (type === "vec4") {
		return "#1b6ad6";
	} else if (type === "vec3") {
		return "#566f9f";
	} else if (type === "vec2") {
		return "#8222a7";
	} else if (type === "RenderObject") {
		return "#ad3b78";
	} else if (type === "Uniform") {
		return "#b19e14";
	} else if (type === "BufferMeshData") {
		return "#be1656";
	} else if (type === "BufferPointData") {
		return "#e023e0";
	} else if (type === "BufferLineData") {
		return "#20cae0";
	} else if (type === "BufferVolumeData") {
		return "#17d042";
	} else if (type === "Any") {
		return "#ef8815";
	} else { // Object
		return "#c12417";
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
			isDragging : false
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
		this.onPlugDragging = this.onPlugDragging.bind(this);
	}

	position() {
		let nodeRect = this.props.nodeRect;
		if (this.props.isInput) {
			return {
				x : nodeRect.x - (15 / 2.0),
				y : nodeRect.y + 18 * (this.props.index + 1) + 10 + 3,
			}
		} else {
			return {
				x : nodeRect.x + nodeRect.w - (15 / 2.0),
				y : nodeRect.y + 18 * (this.props.index + 1) + 10 + 3
			}
		}
	}

	holeCenterPosition() {
		let position = this.position();
		position.x = position.x + (15 / 2.0);
		position.y = position.y + (15 / 2.0);
		return position;
	}

	styles() {
		return {
			input : {
				position : "absolute",
				left : String(-15 / 2),
				top : String(18 * (this.props.index + 1) + 10),
				width : "100px",
				height : "20px"
			},
			output : {
				position : "absolute",
				right : String(-15 / 2),
				top : String(18 * (this.props.index + 1) + 10),
				width : "100px",
				height : "20px"
			},
			inhole : {
				cursor : "pointer",
				position : "absolute",
				left : "0px",
				width : "15px",
				height : "15px",
				marginTop : "3px",
				borderRadius : "15px",
				backgroundColor : colorFunction(this.props.data.type),
				border : (this.state.isDragging) ? "solid 1px" : "none"
			},
			outhole : {
				cursor : "pointer",
				position : "absolute",
				right : "0px",
				width : "15px",
				height : "15px",
				marginTop : "3px",
				borderRadius : "15px",
				backgroundColor : colorFunction(this.props.data.type),
				border : (this.state.isDragging) ? "solid 1px" : "none"
			},
			inholeText : {
				position : "absolute",
				top : "0px",
				left : "15px", // holeのサイズ
				marginLeft : "4px",
				color : "white",
				fontSize : "14px"
			},
			outholeText : {
				position : "absolute",
				top : "0px",
				right : "15px", // holeのサイズ
				marginRight : "4px",
				color : "white",
				fontSize : "14px"
			}
		}
	}

	// プラグをドラッグするActionを発行.
	onMouseDown(ev) {
		let isInput = this.props.isInput;
		let index = this.props.index;
		let id = this.props.id;
		let position = this.position();
		if (position.x < ev.clientX && ev.clientX < (position.x + 15)) {
			if (position.y < ev.clientY && ev.clientY < (position.y + 15)) {
				this.pos = {
					x : ev.clientX,
					y : ev.clientY
				}
				if (isInput) {
					this.props.nodeAction.dragPlug(id, position, {
						x : ev.clientX,
						y : ev.clientY
					});
				} else {
					this.props.nodeAction.dragPlug(id, {
						x : ev.clientX,
						y : ev.clientY
					}, position);
				}
			}
		}
		ev.preventDefault();
		ev.stopPropagation();
	}

	// プラグをドラッグするActionを発行.
	onMouseMove(ev) {
		if (this.state.isDragging) {
			let center = this.holeCenterPosition();
			if (this.props.isInput) {
				this.props.nodeAction.dragPlug(this.props.id, {
					x : center.x + (ev.clientX - this.pos.x),
					y : center.y + (ev.clientY - this.pos.y)
				}, center);
			} else {
				this.props.nodeAction.dragPlug(this.props.id, center, {
					x : center.x + (ev.clientX - this.pos.x),
					y : center.y + (ev.clientY - this.pos.y)
				});
			}
			ev.preventDefault();
			ev.stopPropagation();
		}
	}

	onMouseUp(ev) {
		if (this.state.isDragging) {
			this.setState({
				isDragging :false
			});
			let center = this.holeCenterPosition();
			this.props.nodeAction.endDragPlug(this.props.id, center, {
				x : center.x + (ev.clientX - this.pos.x),
				y : center.y + (ev.clientY - this.pos.y)
			});
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

	content() {
		const style = this.styles(this.props.index);
		if (this.props.isInput) {
			// 入力端子.
			return (<div style={style.input}>
						<div style={style.inhole}
							onMouseDown={this.onMouseDown.bind(this)}
						/>
						<div style={style.inholeText}>{this.props.data.name}</div>
					</div>);
		} else {
			// 出力端子.
			return (<div style={style.output}>
						<div style={style.outhole}
							onMouseDown={this.onMouseDown.bind(this)}
						/>
						<div style={style.outholeText}>{this.props.data.name}</div>
					</div>);
		}
	}

	render() {
		return this.content.bind(this)();
	}
}
