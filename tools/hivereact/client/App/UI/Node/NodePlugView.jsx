import React from "react"
import ReactDOM from "react-dom"
import Core from '../../Core'

/**
 * ノードの端子間の接続線を全て内包するビュー.
 */
export default class NodePlugView extends React.Component {
	constructor(props) {
		super(props);
		this.nodeList = this.props.store.getNodes();

		this.state = {
			nodes : this.props.store.getNodes()
		};
		this.props.store.on(Core.Store.NODE_COUNT_CHANGED, (err, data) => {
			this.setState({
				nodes : [].concat(this.props.store.getNodes())
			});
		});
	}

	createPlug(nodeData, key) {
		return (<circle key={String(nodeData.varname + key)} cx={String(150 * key)} cy="150" r="50" fill="yellow"/>)
	}

	render() {
		let plugList = (this.state.nodes.map( (nodeData, key) => {
			return this.createPlug.bind(this)(nodeData, key);
		} ));
		return (
				<svg width="100%" height="100%" version='1.1' xmlns='http://www.w3.org/2000/svg' ref='svg'>
					{plugList}
				</svg>
		);
	}
}
