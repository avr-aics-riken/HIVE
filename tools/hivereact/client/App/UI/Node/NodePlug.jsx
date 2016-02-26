import React from "react"
import ReactDOM from "react-dom"
import Store from './Store.jsx'

/**
 * ノードの端子間の接続線.
 */
export default class NodePlug extends React.Component {
	constructor(props) {
		super(props);

		this.state = {
			inpos : this.props.plug.input.pos,
			outpos : this.props.plug.output.pos
		}
		this.onPositionChanged = this.onPositionChanged.bind(this);
		this.onTemporaryChanged = this.onTemporaryChanged.bind(this);
	}

	onTemporaryChanged(err, id, inpos, outpos) {
		if (this.props.isTemporary) {
			this.setState({
				inpos : [inpos.x, inpos.y],
				outpos : [outpos.x, outpos.y]
			});
		}
	}

	onPositionChanged(err, data) {
		if (this.props.isTemporary) {
			return;
		}
		else if (data.input.nodeVarname === this.props.plug.input.nodeVarname &&
			data.output.nodeVarname === this.props.plug.output.nodeVarname &&
			data.input.name === this.props.plug.input.name &&
			data.output.name === this.props.plug.output.name)
		{
			this.setState({
				inpos : data.input.pos,
				outpos : data.output.pos
			});
		}
	}

	componentDidMount() {
		this.props.nodeStore.on(Store.PLUG_POSITION_CHANGED, this.onPositionChanged);
		this.props.nodeStore.on(Store.PLUG_DRAGGING, this.onTemporaryChanged);
	}

	componentWillUnmount() {
		this.props.nodeStore.removeListener(Store.PLUG_POSITION_CHANGED, this.onPositionChanged);
		this.props.nodeStore.removeListener(Store.PLUG_DRAGGING, this.onTemporaryChanged);
	}

	stroke() {
		let inpos = this.state.inpos;
		let outpos = this.state.outpos;
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
