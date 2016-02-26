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
			values : this.props.initialParam.value
		};
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

	onChange(index) {
		return (ev) => {
			let val = Number(ev.target.value);
			this.state.values[index] = val;
			this.props.changeVecFunc(this.props.initialParam.name, index, val);
			this.setState({
				values : [].concat(this.state.values)
			});
		}
	}

	valueElem() {
		const styles = this.styles.bind(this)();
		let values = this.props.initialParam.value.map( (val, key) => {
			return (<input style={styles.value}
						type="text"
						ref="text"
						onChange={this.onChange.bind(this)(key)}
						defaultValue={this.state.values[key]}
						value={this.state.values[key]}
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
