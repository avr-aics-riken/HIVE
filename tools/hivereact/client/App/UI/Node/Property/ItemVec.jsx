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
		this.inputChanged = this.inputChanged.bind(this);
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

	inputChanged(err, data) {
		if (data.varname === this.props.varname) {
			for (let i = 0; i < data.input.length; i = i + 1) {
				if (data.input[i].name === this.props.initialParam.name) {
					this.setState({
						values : [].concat(data.input[i].value)
					});
					break;
				}
			}
		}
	}

	componentDidMount() {
		this.props.store.on(Core.Constants.NODE_INPUT_CHANGED, this.inputChanged);
	}

	componentWillUnmount() {
		this.props.store.removeListener(Core.Constants.NODE_INPUT_CHANGED, this.inputChanged);
	}

	onChange(index) {
		return (ev) => {
			let values = this.props.initialParam.value;
			values[index] = Number(ev.target.value);
			this.props.changeFunc(this.props.initialParam.name, values);
			this.setState({
				values : [].concat(values)
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
						defaultValue={val}
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
