import React from "react"
import ReactDOM from "react-dom"
import Core from '../../../Core'

/**
 * ノードプロパティアイテム(Checkbox)ビュー.
 */
export default class ItemCheckbox extends React.Component {
	constructor(props) {
		super(props);

		this.state = {
			checked : this.props.initialParam.value
		};
	}

	styles() {
		return {
			view : {
				width : "250px",
				backgroundColor : "#aeaeae",
				color : "black",
				display : "table-row"
			},
			key : {
				backgroundColor : "#ccc",
				width : "85px",
				display: "table-cell"
			},
			value : {
				width : "18px",
				height : "18px",
				display: "table-cell"
			}
		}
	}

	onChange(ev) {
		this.props.changeCheckboxFunc(this.props.initialParam.name, ev.target.checked);
		this.setState({
			checked : ev.target.checked
		});
	}

	render () {
		const styles = this.styles.bind(this)();
		return (<div style={styles.view}>
					<div style={styles.key}>
						{this.props.initialParam.name}
					</div>
					<input type="checkbox"
					 	style={styles.value}
						checked={this.props.initialParam.value}
						onChange={this.onChange.bind(this)}
					>
					</input>
				</div>);
	}
}
