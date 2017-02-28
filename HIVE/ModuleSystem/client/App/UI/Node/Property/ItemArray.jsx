import React from "react";
import ReactDOM from "react-dom";
import Core from '../../../Core';
import ItemText from './ItemText.jsx';
import ItemVec from './ItemVec.jsx';
import ItemTextInput from './ItemTextInput.jsx';


function isHideProperty(input) {
	return input.hasOwnProperty('visible') && !input.visible;
}

/**
 * ノードプロパティアイテム(Array)ビュー.
 */
export default class ItemArray extends React.Component {
	constructor(props) {
		super(props);
		this.state = {
			value : String(this.props.initialParam.array.length)
		};
		this.currentEdit = {
			value : null
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
                fontSize: "11px",
                letterSpacing: "normal",
                padding: "1px",
				paddingLeft : "4px",
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
                width : "165px",
                verticalAlign: "middle",
                display: "inline-block",
            },
            inputs: {
				outline: "0",
                border: "0px",
                borderRadius: "2px",
                color : "#333",
                letterSpacing: "normal",
                marginLeft: "3px",
                padding: "1px",
                width : "165px",
                height: "19px",
				marginTop: "1px",
				marginBottom: "1px",
                verticalAlign: "middle",
                display: "inline-block",
            }
        };
	}

	onChange() {
		return (ev) => {
			this.currentEdit = {
				value : ev.target.value
			};
			this.setState({
				value : ev.target.value
			});
		};
	}

	submit(ev) {
		if (this.currentEdit.value) {
			this.props.changeLengthFunc(this.props.initialParam.name, this.currentEdit.value);
			this.setState({
				value : this.currentEdit.value
			});
		}
		ev.target.style.border = "none";
		ev.target.blur();
	}

	onKeyPress(ev) {
		if (ev.key === 'Enter') {
			this.submit.bind(this)(ev);
		}
	}

	onKeyDown(ev) {
		//ev.preventDefault();
		ev.stopPropagation();
	}

	onBlur(ev) {
		this.submit.bind(this)(ev);
		this.currentEdit = {
			value : null
		};
	}

	onFocus(ev) {
		ev.target.style.border = "2px solid darkgreen";
	}

	createArrayContents() {
		let contents = this.props.initialParam.array.map( (hole, key) => {
			if (isHideProperty(hole)) { return; }

			let id = String(this.props.id + "_out_" + key);
            let bottom = this.props.bottom && (this.props.initialParam.array.length - 1 === parseInt(key, 10));
            let holes = {name: "[" + key + "]", type: hole.type, isArray: true};
			if (hole.hasOwnProperty('label')) {
				holes.label = hole.label;
			}
			if (hole.type === 'vec2' || hole.type === 'vec3' || hole.type === 'vec4') {
				return (<ItemVec initialParam={holes} key={id} bottom={bottom}/>);
			} else if (hole.type === 'float') {
				/*
				return (<ItemTextInput
							changeFunc={this.props.changeArrayTextFunc(key)}
							initialParam={holes}
							key={id}
							bottom={bottom}/>);
							*/
				return (<ItemText initialParam={holes} key={id} bottom={bottom}/>);
			} else if (hole.type === 'string') {
				holes.value = hole.value;
				return (<ItemTextInput changeFunc={this.props.changeArrayTextFunc(key)} initialParam={holes} key={id} bottom={bottom}/>);
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
						<span style={{marginLeft : "12px"}} title={this.props.initialParam.name}>{this.props.initialParam.name}</span>
						</div>
						<input style={styles.inputs}
							defaultValue={this.state.value}
							value={this.state.value}
							onChange={this.onChange.bind(this)()}
							onKeyPress={this.onKeyPress.bind(this)}
							onKeyDown={this.onKeyDown.bind(this)}
							onBlur={this.onBlur.bind(this)}
							onFocus={this.onFocus.bind(this)}
						>
						</input>
					</div>
					{this.createArrayContents.bind(this)()}
				</div>);
	}
}
