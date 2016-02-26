import React from "react"
import ReactDOM from "react-dom"
import Core from '../../../Core'

/**
 * ノードプロパティアイテム(vector)ビュー.
 */
export default class ItemVec extends React.Component {
	constructor(props) {
		super(props);
	}

	styles() {
		return {
			view : {
				width : "200px",
				height : "100px",
				float : "left",
				backgroundColor : "rgb(100, 100, 200)",
				color : "white"
			}
		}
	}

	render () {
		const styles = this.styles.bind(this)();
		return (<div style={styles.view}></div>);
	}
}
