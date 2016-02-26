import React from "react"
import ReactDOM from "react-dom"
import Core from '../../../Core'

/**
 * ノードプロパティアイテム(TextInput)ビュー.
 */
export default class ItemTextInput extends React.Component {
	constructor(props) {
		super(props);
		this.state = {
			value : this.props.initialParam.value
		}
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
				width : "158px",
				display: "table-cell"
			}
		}
	}

	onChange() {
		return (ev) => {
			this.props.changeFunc(this.props.initialParam.name, ev.target.value);
			this.setState({
				value : ev.target.value
			});
		}
	}

	render () {
		const styles = this.styles.bind(this)();
		return (<div style={styles.view}>
					<div style={styles.key}>
						{this.props.initialParam.name}
					</div>
					<input style={styles.value}
						type="text"
						ref="text"
						value={this.state.value}
						onChange={this.onChange.bind(this)()}
					></input>
				</div>);
	}
}
