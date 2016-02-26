import React from "react"
import ReactDOM from "react-dom"
import Core from '../../../Core'

/**
 * ノードプロパティアイテム(Text)ビュー.
 */
export default class ItemText extends React.Component {
	constructor(props) {
		super(props);
	}

	styles() {
		return {
			view : {
				width : "200px",
				height : "100px",
				float : "left",
				backgroundColor : "rgb(200, 100, 100)",
				color : "white"
			}
		}
	}

	onKeyUp(ev) {
		console.log(ev.target.value);
	}

	render () {
		const styles = this.styles.bind(this)();
		return (<div style={styles.view}>
					<input type="text"
						onKeyUp={this.onKeyUp.bind(this)}
					></input>
				</div>);
	}
}
