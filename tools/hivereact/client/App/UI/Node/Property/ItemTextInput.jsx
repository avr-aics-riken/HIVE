import React from "react";
import ReactDOM from "react-dom";
import Core from '../../../Core';

/**
 * ノードプロパティアイテム(TextInput)ビュー.
 */
export default class ItemTextInput extends React.Component {
	constructor(props) {
		super(props);
		this.state = {
			value : this.props.initialParam.value
		};
		this.currentEdit = {
			value : null
		};
	}

	styles() {
        let border = ()=>{
            if(this.props.top && this.props.bottom){
                return {
                    borderRadius: "2px 2px 2px 2px",
                    letterSpacing: "-5px",
                    overflow: "hidden"
                };
            }else if(this.props.top){
                return {
                    borderBottom: "1px solid rgb(33, 187, 151)",
                    borderRadius: "2px 2px 0px 0px",
                    letterSpacing: "-5px",
                    overflow: "hidden"
                };
            }else if(this.props.bottom){
                return {
                    border: "none",
                    borderRadius: "0px 0px 2px 2px",
                    letterSpacing: "-5px",
                    overflow: "hidden"
                };
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
                textAlign: this.props.initialParam.name.match(/^\[\d\]$/) ? "right" : "left",
                padding: "1px",
				paddingLeft : "4px",
                width : "80px",
                verticalAlign: "middle",
                display: "inline-block",
                overflow: "hidden",
                textShadow: "0px 0px 3px black"
            },
            value : {
                border: "0px",
				outline: "0",
                borderRadius: "2px",
                color : "#333",
                letterSpacing: "normal",
                marginLeft: "3px",
				marginTop: "1px",
				marginBottom: "1px",
                verticalAlign: "middle",
                padding: "1px",
                width : "165px",
                height: "19px",
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
		if (this.props.initialParam.name === "label" || this.currentEdit.value) {
			this.props.changeFunc(this.props.initialParam.name, this.currentEdit.value);
		}
		ev.target.style.border = "none";
		ev.target.blur();
	}

	onKeyPress(ev) {
		if (ev.key === 'Enter') {
			this.submit.bind(this)(ev);
		}
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
						onKeyPress={this.onKeyPress.bind(this)}
						onBlur={this.onBlur.bind(this)}
						onFocus={this.onFocus.bind(this)}
					></input>
				</div>);
	}
}
