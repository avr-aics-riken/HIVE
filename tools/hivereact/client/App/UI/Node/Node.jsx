import React from "react"
import ReactDOM from "react-dom"

export default class Node extends React.Component {
	constructor(props) {
		super(props);
	}

	styles() {
		return {
			node : {
				position : "absolute",
				top : "600px",
				left : "300px",
				width : "200px",
				height : "100px",
				backgroundColor : "red",
				color : "white"
			}
		}
	}

	render () {
		const styles = this.styles();
		return (<div style={styles.node}
			>hogehoge</div>);
	}
}
