import React from "react"
import ReactDOM from "react-dom"
import Core from '../../../Core'
import ItemText from './ItemText.jsx'
import ItemVec from './ItemVec.jsx'
import ItemArray from './ItemArray.jsx'

/**
 * ノードプロパティ1ノード分.
 */
export default class ItemView extends React.Component {
	constructor(props) {
		super(props);
	}

	styles() {
		return {
			view : {
				width : "200px",
				backgroundColor : "rgb(80, 80, 80)",
				color : "white"
			}
		}
	}

	contents() {
		return (
			<div>
				<ItemText />
				<ItemVec />
				<ItemArray />
			</div>
		);
	}

	render () {
		const styles = this.styles.bind(this)();
		return (<div style={styles.view}>
			{this.contents.bind(this)()}
		</div>);
	}
}
