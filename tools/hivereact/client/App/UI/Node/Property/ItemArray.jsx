import React from "react"
import ReactDOM from "react-dom"
import Core from '../../../Core'
import ItemText from './ItemText.jsx'
import ItemVec from './ItemVec.jsx'
import ItemTextInput from './ItemTextInput.jsx'

/**
 * ノードプロパティアイテム(Array)ビュー.
 */
export default class ItemArray extends React.Component {
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

	createArrayContents() {
		let contents = this.state.param.array.map( (hole, key) => {
			let id = String(this.props.id + "_out_" + key);
			if (hole.type === 'vec2' || hole.type === 'vec3' || hole.type === 'vec4') {
				return (<ItemVec initialParam={hole} key={id} />);
			} else if (hole.type === 'string' || hole.type === 'float') {
				return (<ItemTextInput initialParam={hole} key={id} />);
			} else {
				return (<ItemText initialParam={hole} key={id} />);
			}
		});
		return contents;
	}

	render () {
		const styles = this.styles.bind(this)();
		return (<div>
					<div style={styles.view}>
						<div style={styles.key}>
							{this.state.param.name}
						</div>
						<input style={styles.value}
							defaultValue={this.state.param.array.length}>
						</input>
					</div>
					{this.createArrayContents.bind(this)()}
				</div>);
	}
}
