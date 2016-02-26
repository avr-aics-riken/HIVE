import React from "react"
import ReactDOM from "react-dom"
import Core from '../../../Core'
import ItemText from './ItemText.jsx'
import ItemVec from './ItemVec.jsx'
import ItemArray from './ItemArray.jsx'
import ItemTextInput from './ItemTextInput.jsx'

/**
 * ノードプロパティ1ノード分.
 */
export default class ItemView extends React.Component {
	constructor(props) {
		super(props);

		this.state = {
			name : this.props.initialNodeData.name,
			input : this.props.initialNodeData.input,
			output : this.props.initialNodeData.output
		};
	}

	styles() {
		return {
			view : {
				width : "250px",
				backgroundColor : "rgb(80, 80, 80)",
				color : "black",
				display : "table"
			}
		}
	}

	contents() {
		let inputs = this.state.input.map( (hole, key) => {
			let id = String(this.props.id + "_in_" + key);
			if (Array.isArray(hole.array)) {
				return (<ItemArray initialParam={hole} key={id} id={id} />);
			} else if (hole.type === 'vec2' || hole.type === 'vec3' || hole.type === 'vec4') {
				return (<ItemVec initialParam={hole} key={id} id={id}/>);
			} else if (hole.type === 'string' || hole.type === 'float') {
				return (<ItemTextInput initialParam={hole} key={id} id={id} />);
			} else {
				return (<ItemText initialParam={hole} key={id} id={id} />);
			}
		});
		let outputs = this.state.output.map( (hole, key) => {
			let id = String(this.props.id + "_out_" + key);
			if (Array.isArray(hole.array)) {
				return (<ItemArray initialParam={hole} key={id} id={id} />);
			} else if (hole.type === 'vec2' || hole.type === 'vec3' || hole.type === 'vec4') {
				return (<ItemVec initialParam={hole} key={id} id={id} />);
			} else if (hole.type === 'string' || hole.type === 'float') {
				return (<ItemTextInput initialParam={hole} key={id} id={id} />);
			} else {
				return (<ItemText initialParam={hole} key={id} id={id} />);
			}
		});
		return (
			<div>
				<ItemText
					isHeader={true}
					initialParam={{
						name : "Node",
						value : this.state.name
					}}
					key={String(this.props.id + "_title")}
					id={String(this.props.id + "_title")} />
				{inputs}
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
