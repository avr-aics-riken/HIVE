import React from "react"
import ReactDOM from "react-dom"

/**
 * ノードの端子間の接続線.
 */
export default class NodePlug extends React.Component {
	constructor(props) {
		super(props);
		this.styles = this.styles.bind(this);
	}

	content() {
		const style = this.styles(this.props.index);
		return <path></path>
	}
}
