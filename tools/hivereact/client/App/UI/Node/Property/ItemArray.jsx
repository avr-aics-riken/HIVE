import React from "react";
import ReactDOM from "react-dom";
import Core from '../../../Core';
import ItemText from './ItemText.jsx';
import ItemVec from './ItemVec.jsx';
import ItemTextInput from './ItemTextInput.jsx';

/**
 * ノードプロパティアイテム(Array)ビュー.
 */
export default class ItemArray extends React.Component {
	constructor(props) {
		super(props);
		this.state = {
			value : String(this.props.initialParam.array.length)
		};
	}
	styles() {
        let border = ()=>{
            if(this.props.top && !this.props.bottom){
                return {
                    borderBottom: "1px solid rgb(33, 187, 151)",
                    borderRadius: "3px 3px 0px 0px",
                    letterSpacing: "-5px",
                    overflow: "hidden"
                };
            // }else if(this.props.bottom){
            //     return {
            //         border: "none",
            //         borderRadius: "0px 0px 3px 3px",
            //         letterSpacing: "-5px",
            //         overflow: "hidden"
            //     };
            }else{
                return {
                    borderBottom: "1px solid rgb(33, 187, 151)",
                    letterSpacing: "-5px",
                    overflow: "hidden"
                };
            }
        };
        return {
            view : border.bind(this)(),
            key : {
                backgroundColor: "rgb(84,84,84)",
                color : "white",
                fontSize: "smaller",
                letterSpacing: "normal",
                padding: "1px",
                width : "80px",
                verticalAlign: "middle",
                display: "inline-block",
                overflow: "hidden",
                textShadow: "0px 0px 3px black"
            },
            value : {
                color : "#333",
                letterSpacing: "normal",
                padding: "1px",
                width : "160px",
                verticalAlign: "middle",
                display: "inline-block",
            },
            inputs: {
                border: "0px",
                borderRadius: "3px",
                color : "#333",
                letterSpacing: "normal",
                marginLeft: "5px",
                padding: "1px",
                width : "153px",
                height: "20px",
                verticalAlign: "bottom",
                display: "inline-block",
            }
        };
	}

	onChange() {
		return (ev) => {
			this.props.changeLengthFunc(this.props.initialParam.name, ev.target.value);
			this.setState({
				value : ev.target.value
			});
		};
	}

	createArrayContents() {
		let contents = this.props.initialParam.array.map( (hole, key) => {
			let id = String(this.props.id + "_out_" + key);
            let bottom = this.props.bottom && (this.props.initialParam.array.length - 1 === parseInt(key, 10));
            let holes = {name: "[" + key + "]", type: hole.type, isArray: true};
			if (hole.type === 'vec2' || hole.type === 'vec3' || hole.type === 'vec4') {
				return (<ItemVec initialParam={holes} key={id} bottom={bottom}/>);
			} else if (hole.type === 'string' || hole.type === 'float') {
				return (<ItemTextInput initialParam={holes} key={id} bottom={bottom}/>);
			} else {
				return (<ItemText initialParam={holes} key={id} bottom={bottom}/>);
			}
		});
		return contents;
	}

	render () {
		const styles = this.styles.bind(this)();
		return (<div>
					<div style={styles.view}>
						<div style={styles.key}>
							{this.props.initialParam.name}
						</div>
						<input style={styles.inputs}
							defaultValue={this.state.value}
							value={this.state.value}
							onChange={this.onChange.bind(this)()}
						>
						</input>
					</div>
					{this.createArrayContents.bind(this)()}
				</div>);
	}
}
