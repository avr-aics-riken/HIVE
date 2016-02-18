import React from "react"
import ReactDOM from "react-dom"

import Node from "./Node.jsx"

/**
 * 全てのノードを内包するビュー.
 */
export default class View extends React.Component {
	constructor(props) {
		super(props);
		this.nodeList = this.props.nodeList;
	}

	render () {
		let nodeList = (this.props.nodes.map( (nodeData, key) => {
			return (<Node node={nodeData} store={this.props.store} action={this.props.action} key={nodeData.varname + key}></Node>);
		} ));
		return (<div>{nodeList}</div>);
	}
}
