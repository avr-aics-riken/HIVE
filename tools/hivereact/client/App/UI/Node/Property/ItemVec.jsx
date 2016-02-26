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

	onKeyUp(index) {
		return (ev) => {
			let values = this.props.initialParam.value;
			values[index] = Number(ev.target.value);
				console.log(values);
			this.props.changeFunc(this.props.initialParam.name, values);
		}
	}

	valueElem() {
		const styles = this.styles.bind(this)();
		let values = this.props.initialParam.value.map( (val, key) => {
			return (<input style={styles.value}
						type="text"
						ref="text"
						onKeyUp={this.onKeyUp.bind(this)(key)}
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
						{this.props.initialParam.name}
					</div>
					{this.valueElem.bind(this)()}
				</div>);
	}
}
