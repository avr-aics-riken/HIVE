import React from "react"
import ReactDOM from "react-dom"
import Core from '../../../Core'

/**
 * ノードプロパティアイテム(Array)ビュー.
 */
export default class ItemArray extends React.Component {
	constructor(props) {
		super(props);
	}

	styles() {
		return {
			view : {
				width : "200px",
				height : "100px",
				float : "left",
				backgroundColor : "rgb(100, 200, 100)",
				color : "white"
			}
		}
	}

	render () {
		const styles = this.styles.bind(this)();
		return (<div style={styles.view}></div>);
	}
}
