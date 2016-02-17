import React from "react"
import ReactDOM from "react-dom"

import Node from "./Node.jsx"

export default class View extends React.Component {
	constructor(props) {
		super(props);
		this.nodeList = this.props.nodeList;
	}

	render () {
		let nodeList = (this.props.nodes.map( (nodeData, key) => {
			return (<Node node={nodeData} key={key}></Node>);
		} ));
		return (<div>{nodeList}</div>);
	}
}
