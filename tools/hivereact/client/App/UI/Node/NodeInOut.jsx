import React from "react"
import ReactDOM from "react-dom"

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
	}

	styles(key) {
		return {
			input : {
				position : "absolute",
				left : String(-15 / 2),
				top : String(18 * (key + 1) + 10),
				width : "100px",
				height : "20px"
			},
			output : {
				position : "absolute",
				right : String(-15 / 2),
				top : String(18 * (key + 1) + 10),
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
				backgroundColor : colorFunction(this.props.data.type)
			},
			outhole : {
				cursor : "pointer",
				position : "absolute",
				right : "0px",
				width : "15px",
				height : "15px",
				marginTop : "3px",
				borderRadius : "15px",
				backgroundColor : colorFunction(this.props.data.type)
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

	onMouseDown(ev) {
		console.log("hogehoge", ev);
	}

	onMouseMove(ev) {

	}

	onMouseUp(ev) {

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
