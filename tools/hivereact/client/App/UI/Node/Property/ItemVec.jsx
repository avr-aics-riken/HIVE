import React from "react";
import ReactDOM from "react-dom";
import Core from '../../../Core';

/**
 * ノードプロパティアイテム(vector)ビュー.
 */
export default class ItemVec extends React.Component {
	constructor(props) {
		super(props);
		this.state = {
			values : this.props.initialParam.value
		};
		this.currentEdit = {
			index : -1,
			value : null
		};
	}

	styles() {
        let border = ()=>{
            if(this.props.top && this.props.bottom){
                return {
                    borderRadius: "3px 3px 3px 3px",
                    letterSpacing: "-5px",
                    overflow: "hidden"
                };
            }else if(this.props.top){
                return {
                    borderBottom: "1px solid rgb(33, 187, 151)",
                    borderRadius: "3px 3px 0px 0px",
                    letterSpacing: "-5px",
                    overflow: "hidden"
                };
            }else if(this.props.bottom){
                return {
                    border: "none",
                    borderRadius: "0px 0px 3px 3px",
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
                color : "#333",
                letterSpacing: "normal",
                //padding: "1px",
                width : "164px",
                verticalAlign: "middle",
                display: "inline-block"
            },
            flex : {
                //paddingLeft: "3px"
                // display: "flex",
                // flexDirection: "row"
            },
            inputs: {
				outline: "0",
                border: "0px",
                borderRadius: "2px",
                marginLeft: "3px",
				marginTop: "1px",
				marginBottom: "1px",
                verticalAlign: "middle",
                width: "36px",
				height: "19px",
                float: "left",
				border : "1px solid rgba(54, 196, 168, 0.0)"
                // flexGrow: 1
            }
        };
	}

	onChange(index) {
		return (ev) => {
			let val = ev.target.value;
			this.state.values[index] = val;
			this.currentEdit = {
				index : index,
				value : parseFloat(val)
			};
			this.setState({
				values : [].concat(this.state.values)
			});
		};
	}

	submit(ev) {
		if (this.currentEdit.value !== null && !isNaN(this.currentEdit.value) && this.currentEdit.index >= 0) {
			this.props.changeVecFunc(this.props.initialParam.name, this.currentEdit.index, this.currentEdit.value);
		} else if (isNaN(this.currentEdit.value) && this.currentEdit.index >= 0) {
			this.state.values[this.currentEdit.index] = 0;
			this.setState({
				values : [].concat(this.state.values)
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

	onBlur(ev) {
		this.submit.bind(this)(ev);
		this.currentEdit = {
			index : -1,
			value : null
		};
	}

	onFocus(ev) {
		ev.target.style.border = "2px solid darkgreen";
	}

	valueElem() {
		const styles = this.styles.bind(this)();
		let values = this.props.initialParam.value.map( (val, key) => {
			return (<input style={styles.inputs}
						type="text"
						ref="text"
						onChange={this.onChange.bind(this)(key)}
						defaultValue={this.state.values[key]}
						value={this.state.values[key]}
						onKeyPress={this.onKeyPress.bind(this)}
						onBlur={this.onBlur.bind(this)}
						onFocus={this.onFocus.bind(this)}
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
                    <div style={styles.value}>
                        <div style={styles.flex}>
                            {this.valueElem.bind(this)()}
                        </div>
                    </div>
				</div>);
	}
}
