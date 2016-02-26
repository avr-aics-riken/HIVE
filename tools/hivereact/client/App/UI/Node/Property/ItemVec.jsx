import React from "react"
import ReactDOM from "react-dom"
import Core from '../../../Core'

/**
 * ノードプロパティアイテム(vector)ビュー.
 */
export default class ItemVec extends React.Component {
	constructor(props) {
		super(props);
		this.state = {
			param : this.props.initialParam
		}
	}

	styles() {
		return {
			view : {
				width : "250px",
				backgroundColor : "#aeaeae",
				display : "table-row"
			},
			key : {
				backgroundColor : "#ccc",
				width : "85px",
				display: "table-cell"
			},
			value : {
				width : "50px",
				display: "table-cell"
			}
		}
	}

	onKeyUp(ev) {
		console.log(ev.target.value);
		this.setState({
			text : ev.target.value
		});
	}

	valueElem() {
		const styles = this.styles.bind(this)();
		let values = this.state.param.value.map( (val, key) => {
			return (<input style={styles.value}
						type="text"
						ref="text"
						onKeyUp={this.onKeyUp.bind(this)}
						defaultValue={val}
						key={key}
					></input>);
		});
		return values;
	}

	render () {
		const styles = this.styles.bind(this)();
		return (<div style={styles.view}>
					<div style={styles.key}>
						{this.state.param.name}
					</div>
					{this.valueElem.bind(this)()}
				</div>);
	}
}
