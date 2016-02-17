import React from "react"
import ReactDOM from "react-dom"

import Node from "./Node.jsx"

export default class View extends React.Component {
	constructor(props) {
		super(props);
	}

	render () {
		return (<Node ></Node>);
	}
}
