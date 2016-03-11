import React from "react"
import ReactDOM from "react-dom"
import Store from './Store.jsx'

/**
 * ノードの端子間の接続線.
 */
export default class NodePlug extends React.Component {
	constructor(props) {
		super(props);
	}

	stroke() {
		let inpos = this.props.plug.input.pos;
		let outpos = this.props.plug.output.pos;
		if (this.props.isSimple) {
			let simplePos = this.props.nodeStore.calcSimplePlugPosition(this.props.plug);
			inpos = simplePos[0];
			outpos = simplePos[1];
		}
		let midx = (Number(inpos[0]) + Number(outpos[0])) * 0.5;

		let str = "M"
			+ String(inpos[0]) + " "
			+ String(inpos[1]) + " "
			+ "C" + " "
			+ String(midx) + ","
			+ String(inpos[1]) + " "
			+ String(midx) + ","
			+ String(outpos[1]) + " "
			+ String(outpos[0]) + " "
			+ String(outpos[1]);
		return str;
	}

	render() {
		return <path d={this.stroke.bind(this)()} stroke="white" fill="transparent"></path>
	}
}
