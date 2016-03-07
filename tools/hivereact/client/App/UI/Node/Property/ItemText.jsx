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
				width : "250px",
				backgroundColor : this.props.isHeader ? "#868686" : "#aeaeae",
				color : "black",
				display : "table-row"
			},
			key : {
				backgroundColor : this.props.isHeader ? "#868686" : "#ccc",
				width : "85px",
				display: "table-cell"
			},
			value : {
				width : "158px",
				display: "table-cell"
			}
		}
	}

	render () {
		const styles = this.styles.bind(this)();
		return (<div style={styles.view}>
					<div style={styles.key}>
						{this.props.initialParam.name}
					</div>
					<div style={styles.value}>
						{this.props.initialParam.hasOwnProperty('value') ? this.props.initialParam.value : "(Object)"}
					</div>
				</div>);
	}
}
